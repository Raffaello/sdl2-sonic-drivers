#pragma once

#include <cstdint>
#include <hardware/opl/scummvm/EmulatedOPL.hpp>
#include <hardware/opl/Config.hpp>
#include <memory>
#include <audio/scummvm/Mixer.hpp>
#include <hardware/opl/scummvm/nuked/opl3.h>
#include <hardware/opl/Chip.hpp>

namespace hardware::opl::scummvm::nuked
{
    class OPL : public EmulatedOPL
    {
    private:
        unsigned int _rate = 0;
        std::unique_ptr<opl3_chip> chip;
        hardware::opl::Chip _chip[2];
        union {
            uint16_t normal;
            uint8_t dual[2];
        } _reg;

        //unsigned int address[2];

        // TODO: this is same in DOSBOX, it can be bring into the parent, and just change the last line
        void dualWrite(uint8_t index, uint8_t reg, uint8_t val);

    public:
        OPL(const OplType type, const std::shared_ptr<audio::scummvm::Mixer>& mixer);
        ~OPL();

        bool init() override;
        void reset() override;

        // TODO this is the same as DOSBOX, it changes only the call to the emulated chip.
        void write(int a, int v) override;
        // TODO this is the same as DOSBOX, here works only on OPL3
        uint8_t read(int a) override;

        void writeReg(int r, int v) override;

        bool isStereo() const override { return true; }

    protected:
        void generateSamples(int16_t* buffer, int length) override;
    };
}
