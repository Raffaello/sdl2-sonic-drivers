#pragma once

#include <cstdint>
#include <memory>
#include <HyperSonicDrivers/hardware/opl/OPL.hpp>
#include <HyperSonicDrivers/audio/IAudioStream.hpp>
#include <HyperSonicDrivers/audio/IMixer.hpp>
#include <HyperSonicDrivers/hardware/opl/OplType.hpp>

namespace HyperSonicDrivers::audio::streams
{
    class OplStream;
}

namespace HyperSonicDrivers::hardware::opl
{
    constexpr int FIXP_SHIFT = 16;

    /**
     * An OPL that represents an emulated OPL.
     *
     * This will send callbacks based on the number of samples
     * decoded in readBuffer().
     */
    class EmulatedOPL : public OPL
    {
        friend audio::streams::OplStream;

    public:
        EmulatedOPL(const OplType type, const std::shared_ptr<audio::IMixer>& mixer);
        ~EmulatedOPL() override;

        // OPL API
        uint32_t setCallbackFrequency(const int timerFrequency) override;

        std::shared_ptr<audio::IMixer> getMixer() const noexcept;
        inline audio::IAudioStream* getStreamTest() const noexcept { return m_stream.get(); };
    protected:
        std::shared_ptr<audio::IMixer> m_mixer;
        // OPL API
        void startCallbacks(
            const audio::mixer::eChannelGroup group,
            const uint8_t volume,
            const uint8_t pan,
            const int timerFrequency) override;
        void stopCallbacks() override;

        /**
         * Read up to 'length' samples.
         *
         * Data will be in native endianess, 16 bit per sample, signed.
         * For stereo OPL, buffer will be filled with interleaved
         * left and right channel samples, starting with a left sample.
         * Furthermore, the samples in the left and right are summed up.
         * So if you request 4 samples from a stereo OPL, you will get
         * a total of two left channel and two right channel samples.
         */
        virtual void generateSamples(int16_t* buffer, const size_t length) noexcept = 0;
    private:
        std::optional<uint8_t> m_channel_id;
        std::shared_ptr<audio::IAudioStream> m_stream;
    };
}
