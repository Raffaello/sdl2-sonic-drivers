#pragma once

#include <drivers/midi/Device.hpp>
#include <memory>
#include <hardware/opl/OPL.hpp>
#include <drivers/midi/adlib/AdLibDriver.hpp>
#include <audio/midi/MIDIEvent.hpp>
#include <audio/opl/banks/OP2Bank.hpp>

namespace drivers
{
    namespace midi
    {
        namespace devices
        {
            class Adlib : public Device
            {
            public:
                // TOOD review the constructor and use a load bank instead..
                Adlib(const std::shared_ptr<hardware::opl::OPL>& opl, const std::shared_ptr<audio::opl::banks::OP2Bank>& op2Bank);
                ~Adlib() = default;

                void sendEvent(const audio::midi::MIDIEvent& e) const noexcept override;
                void sendMessage(const uint8_t msg[], const uint8_t size) const noexcept override;
                void sendSysEx(const audio::midi::MIDIEvent& e) const noexcept override;
                
                //void loadBankOP2();
            private:
                std::shared_ptr<drivers::midi::adlib::AdLibDriver> _adlib;
            };
        }
    }
}
