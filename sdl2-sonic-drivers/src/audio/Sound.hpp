#pragma once

#include <audio/scummvm/AudioStream.hpp>
#include <audio/scummvm/SoundHandle.hpp>
#include <audio/scummvm/Mixer.hpp>
#include <cstdint>
#include <memory>

namespace audio
{
    class Sound final: public scummvm::AudioStream
    {
    public:
        Sound(const Sound&) = delete;
        Sound(const scummvm::Mixer::SoundType soundType, const bool isStereo, const int rate, const uint8_t bitsDepth, const uint32_t dataSize, const std::shared_ptr<uint8_t[]> data);

        virtual int readBuffer(int16_t* buffer, const int numSamples);
        virtual bool isStereo() const;
        virtual int getRate() const;
        virtual bool endOfData() const;

        uint8_t getBitsDepth() const noexcept;
        scummvm::SoundHandle* getHandle() const noexcept;
        const scummvm::Mixer::SoundType getSoundType() const noexcept;

        // TODO: should be in the driver
        // BODY: with the curposition and the handler i think
        // BODY: as it is not possible to play twice the same sound
        // BODY: plus reset could be called while playing resulting in unwanted results.
        void reset();

    private:
        uint32_t _dataSize;
        std::shared_ptr<uint8_t[]> _data;
        bool _stereo;
        int _rate;
        uint8_t _bitsDepth;
        scummvm::SoundHandle* _handle;

        uint32_t _curPos;
        int _bitsFactor;

        scummvm::Mixer::SoundType _soundType;
    };
}
