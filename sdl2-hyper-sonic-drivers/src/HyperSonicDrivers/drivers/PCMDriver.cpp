#include <algorithm>
#include <HyperSonicDrivers/drivers/PCMDriver.hpp>

namespace HyperSonicDrivers::drivers
{
    using audio::streams::PCMStream;

    PCMDriver::PCMDriver(const std::shared_ptr<audio::IMixer>& mixer, const uint8_t max_channels) :
        max_streams(std::min(mixer->max_channels, max_channels)), m_mixer(mixer)
    {
        m_PCMStreams.resize(max_streams);
    }

    bool PCMDriver::isPlaying() const noexcept
    {
        for(const auto& ss: m_PCMStreams)
        {
            if (isPCMStreamPlaying_(ss))
                return true;
        }

        return false;
    }

    bool PCMDriver::isPlaying(const std::shared_ptr<audio::Sound>& sound) const noexcept
    {
        // TODO:
        // should map channelId to check directly in the mixer?
        // how to find a free slot then? 
        // does we need to really track it?
        // probably using a map instead of a vector is ok,
        // no need to define nether max-channels.
        // but that is because if wanting to reserve some channels for something
        // else that is not PCM related...
        // anyway... it could be achieved having the mixer a "lock or reserved channel"
        // feature or something that that one won't be used unless
        // it is for the resources that has been reserved for.....
        for(const auto& ss : m_PCMStreams)
        {
            if (ss->getSound().lock() == sound)
                return isPCMStreamPlaying_(ss);
        }

        return false;
    }

    std::optional<uint8_t> PCMDriver::play(const std::shared_ptr<audio::Sound>& sound, const uint8_t volume, const int8_t pan)
    {
        // find first free slot
        auto it = std::ranges::find_if_not(m_PCMStreams, isPCMStreamPlaying_);
        if (it == m_PCMStreams.end())
            return std::nullopt;

        *it = std::make_shared<PCMStream>(sound);

        auto channelId =  m_mixer->play(
            sound->group,
            *it,
            volume,
            pan
        );

        if (!channelId.has_value())
            *it = nullptr;

        return channelId;
    }

    inline bool PCMDriver::isPCMStreamPlaying_(const std::shared_ptr<audio::streams::PCMStream>& ss) noexcept
    {
        return ss != nullptr && !ss->isEnded();
    }
}
