#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <optional>
#include <HyperSonicDrivers/audio/IMixer.hpp>
#include <HyperSonicDrivers/audio/streams/PCMStream.hpp>
#include <HyperSonicDrivers/audio/PCMSound.hpp>
#include <HyperSonicDrivers/audio/mixer/config.hpp>

namespace HyperSonicDrivers::drivers
{
    /**
     * @brief Managing PCM sound and their playback
     * @details It is a sort of bridge between Mixer and Sound objects
     *          so just simplify playing digital sounds.
     * TODO: should be PCMPlayer instead?
     */
    class PCMDriver final
    {
    public:
        explicit PCMDriver(const std::shared_ptr<audio::IMixer>& mixer, const uint8_t max_channels = 0xFF);
        ~PCMDriver() = default;

        bool isPlaying() const noexcept;
        bool isPlaying(const std::shared_ptr<audio::PCMSound>& sound) const noexcept;
        std::optional<uint8_t> play(
            const std::shared_ptr<audio::PCMSound>& sound,
            const uint8_t volume = audio::mixer::Channel_max_volume,
            const int8_t pan = 0
        );

        const uint8_t max_streams;
    private:
        std::shared_ptr<audio::IMixer> m_mixer;
        std::vector<std::shared_ptr<audio::streams::PCMStream>> m_PCMStreams;

        static bool isPCMStreamPlaying_(const std::shared_ptr<audio::streams::PCMStream>& stream) noexcept;
    };
}
