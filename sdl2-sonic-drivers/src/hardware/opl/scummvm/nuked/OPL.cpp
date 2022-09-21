#include <hardware/opl/scummvm/nuked/OPL.hpp>
#include <hardware/opl/scummvm/nuked/opl3.h>
#include <cstdlib>
#include <cstring>

namespace hardware::opl::scummvm::nuked
{
    OPL::OPL(const OplType type, const std::shared_ptr<audio::scummvm::Mixer>& mixer)
        : EmulatedOPL(type, mixer), _reg({ 0 })
    {
        chip = std::make_unique<opl3_chip>();
    }

    OPL::~OPL()
    {
        stop();
    }

    bool OPL::init()
    {
        memset(&_reg, 0, sizeof(_reg));
        _rate = _mixer->getOutputRate();
        OPL3_Reset(chip.get(), _rate);

        if (type == OplType::DUAL_OPL2) {
            OPL3_WriteReg(chip.get(), 0x105, 0x01);
        }

        _init = true;
        return _init;
    }

    void OPL::reset()
    {
        OPL3_Reset(chip.get(), _rate);
    }

    void OPL::write(int port, int val)
    {
        if (port & 1)
        {
            switch (type)
            {
            case OplType::OPL2:
            case OplType::OPL3:
                if (!_chip[0].write(_reg.normal, val)) {
                    OPL3_WriteRegBuffered(chip.get(), _reg.normal, (uint8_t)val);
                }
                break;
            case OplType::DUAL_OPL2:
                // Not a 0x??8 port, then write to a specific port
                if (!(port & 0x8)) {
                    uint8_t index = (port & 2) >> 1;
                    dualWrite(index, _reg.dual[index], val);
                }
                else {
                    //Write to both ports
                    dualWrite(0, _reg.dual[0], val);
                    dualWrite(1, _reg.dual[1], val);
                }
                break;
            default:
                break;
            }
        }
        else {
            switch (type) {
            case OplType::OPL2:
                //address[0] = val & 0xff;
                //_reg.normal = _emulator->WriteAddr(port, val) & 0xff;
                _reg.normal = val & 0xff;
                break;
            case OplType::DUAL_OPL2:
                // Not a 0x?88 port, when write to a specific side
                if (!(port & 0x8)) {
                    uint8_t index = (port & 2) >> 1;
                    //address[index] = val & 0xff;
                    _reg.dual[index] = val & 0xff;
                }
                else {
                    //address[0] = val & 0xff;
                    //address[1] = val & 0xff;
                    _reg.dual[0] = _reg.dual[1] = val & 0xff;
                }
                break;
            case OplType::OPL3:
                //address[0] = (val & 0xff) | ((port << 7) & 0x100);
                _reg.normal = (val & 0xff) | ((port << 7) & 0x100);
                break;
            default:
                break;
            }
        }
    }

    void OPL::writeReg(int r, int v)
    {
        int tempReg = 0;
        switch (type)
        {
        case OplType::OPL2:
        case OplType::DUAL_OPL2:
        case OplType::OPL3:
            // We can't use _handler->writeReg here directly, since it would miss timer changes.

            // Backup old setup register
            tempReg = _reg.normal;

            // We directly allow writing to secondary OPL3 registers by using
            // register values >= 0x100.
            if (type == OplType::OPL3 && r >= 0x100) {
                // We need to set the register we want to write to via port 0x222,
                // since we want to write to the secondary register set.
                write(0x222, r);
                // Do the real writing to the register
                write(0x223, v);
            }
            else {
                // We need to set the register we want to write to via port 0x388
                write(0x388, r);
                // Do the real writing to the register
                write(0x389, v);
            }

            // Restore the old register
            if (type == OplType::OPL3 && tempReg >= 0x100) {
                write(0x222, tempReg & ~0x100);
            }
            else {
                write(0x388, tempReg);
            }
            break;
        default:
            break;
        };

//        OPL3_WriteRegBuffered(chip.get(), (uint16_t)r, (uint8_t)v);
    }

    void OPL::dualWrite(uint8_t index, uint8_t reg, uint8_t val)
    {
        // Make sure you don't use opl3 features
        // Don't allow write to disable opl3
        if (reg == 5)
            return;

        // Only allow 4 waveforms
        if (reg >= 0xE0 && reg <= 0xE8)
            val &= 3;

        // Write to the timer?
        if (_chip[index].write(reg, val))
            return;

        // Enabling panning
        if (reg >= 0xC0 && reg <= 0xC8) {
            val &= 15;
            val |= index ? 0xA0 : 0x50;
        }

        uint32_t fullReg = reg + (index ? 0x100 : 0);
        OPL3_WriteRegBuffered(chip.get(), (uint16_t)fullReg, (uint8_t)val);
    }

    uint8_t OPL::read(int port)
    {
        switch (type)
        {
        case OplType::OPL2:
            if (!(port & 1))
                //Make sure the low bits are 6 on opl2
                return _chip[0].read() | 0x6;
            break;
        case OplType::OPL3:
            if (!(port & 1))
                return _chip[0].read();
            break;
        case OplType::DUAL_OPL2:
            // Only return for the lower ports
            if (port & 1)
                return 0xff;
            // Make sure the low bits are 6 on opl2
            return _chip[(port >> 1) & 1].read() | 0x6;
        default:
            break;
        }
        return 0;
    }

    void OPL::generateSamples(int16_t* buffer, int length) {
        OPL3_GenerateStream(chip.get(), (int16_t*)buffer, (uint16_t)length / 2);
    }
}
