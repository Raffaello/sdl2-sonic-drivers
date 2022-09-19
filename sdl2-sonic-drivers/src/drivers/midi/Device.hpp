#pragma once

#include <audio/midi/MIDIEvent.hpp>
#include <cstdint>
#include <atomic>


// TODO: namespace drivers::midi::devices could be considered
//       to be replaced instead as devices:: and devices::midi ?
namespace drivers
{
    namespace midi
    {
        class Device
        {
        public:
            Device() = default;
            ~Device() = default;

            virtual void sendEvent(const audio::midi::MIDIEvent& e) const noexcept = 0;
            virtual void sendMessage(const uint8_t msg[], const uint8_t size) const noexcept = 0;
            virtual void sendSysEx(const audio::midi::MIDIEvent& e) const noexcept = 0;
            virtual void pause() const noexcept = 0;
            virtual void resume() const noexcept = 0;

            inline bool isAcquired() const noexcept { return _acquired; }
            inline bool isOwned(const void* owner) const noexcept { return _owner == owner; }

            inline bool acquire(void* owner)
            {
                if (!_acquired) {
                    _acquired = true;
                    _owner = owner;
                    return true;
                }
                else return false;
            }

            bool release(const void* owner)
            {
                if (_owner == owner) {
                    _acquired = false;
                    return true;
                }

                return !isAcquired();
            }
        private:
            volatile std::atomic<bool> _acquired = false;
            void* _owner;
        };
    }
}
