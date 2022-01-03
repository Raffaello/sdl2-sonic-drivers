#pragma once

#include <drivers/midi/Device.hpp>
#include <drivers/midi/scummvm/MidiDriver_ADLIB.hpp>
#include <memory>
#include <cstdint>
#include <hardware/opl/OPL.hpp>


namespace drivers
{
    namespace midi
    {
        namespace devices
        {
            /**
             * @brief Wrapper around ScummVM MidiDriver
             * At the moment support only OPL
            */
            class ScummVM : public Device
            {
            public:
                explicit ScummVM(std::shared_ptr<hardware::opl::OPL> opl, const bool opl3mode);
                ~ScummVM();

                inline void sendEvent(const audio::midi::MIDIEvent& e) const noexcept override;
                inline void sendMessage(const uint8_t msg[], const uint8_t size) const noexcept override;

            private:
                std::shared_ptr<drivers::midi::scummvm::MidiDriver_ADLIB> _adlib;
            };
        }
    }
}
