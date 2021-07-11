#pragma once
#include <cstdint>
#include <mutex>
#include <functional>
#ifdef __GNUC__
#define _In_
#endif

/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

// TODO
//      audio/softsynths could be the generators. OR THE INTERFACE TO USE THE HARDWARE
//      HARDWARE SOMEHOW WILL BE THE LOW LEVEL API OR NOT REALLY ACCESSIBLE
//      WHILE AUDIO IS THE INTERFACE HIGH LEVEL API TO MAKE SOUNDS
//      DRIVERS IS IN BETWEEN... ALSO MIGHT CONTAIN MILES SOUND DRIVERS (AIL)
//      AND READING FILE FORMATS LIKE ADL OR XMI
namespace audio
{
    namespace hardware
    {
        class PCSpeaker final
        {
        public:
            /// <summary>
            /// Wave From Generators
            /// </summary>
            enum class eWaveForm
            {
                SQUARE = 0,
                SINE,
                SAW,
                TRIANGLE,
                PULSE
            };

            /// <summary>
            /// used for SDL_Mixer
            /// </summary>
            /// <param name="userdata"></param>
            /// <param name="audiobuf"></param>
            /// <param name="len"></param>
            static void callback16bits(void* userdata, _In_ uint8_t* audiobuf, int len);
            
            static void callback8bits(void* userdata, _In_ uint8_t* audiobuf, int len);

            PCSpeaker(const int rate = 44100, const int audio_channels = 2);
            ~PCSpeaker();

            // atomic?
            uint8_t volume = 128;

            // if length == -1 then infinte playing
            void play(const eWaveForm wave, const int freq, const int32_t length);
            /** Stop the currently playing note after delay ms. */
            void stop(const int32_t delay = 0);
            bool isPlaying() const noexcept;
            int readBuffer(int16_t* buffer, const int numSamples);
            int readBuffer8bits(int8_t* buffer, const int numSamples);
            int getRate() const noexcept;
        private:
            std::mutex _mutex;
            
            eWaveForm _wave = eWaveForm::SQUARE;
            
            const int _rate;
            const uint8_t _channels;
            
            bool _loop = false;
            uint32_t _oscLength = 0;
            uint32_t _oscSamples = 0;
            uint32_t _remainingSamples = 0;

            inline void _setRemainingSamples(const int32_t length) noexcept {
                _remainingSamples = (_rate * length) / 1000;
                _loop = false;
            }
        };
    }
} // End of namespace Audio
    