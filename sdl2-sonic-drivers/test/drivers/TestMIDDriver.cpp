#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <audio/midi/MIDIEvent.hpp>
#include <audio/midi/MIDITrack.hpp>
#include <audio/midi/types.hpp>
#include <audio/scummvm/Mixer.hpp>
#include "../audio/stubs/StubMixer.hpp"
#include <drivers/MIDDriver.hpp>
#include <drivers/midi/Device.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ostream_sink.h>
#include <sstream>
#include <algorithm>
#include <drivers/midi/devices/SpyDevice.hpp>
#include <drivers/MIDDriverMock.hpp>


namespace drivers
{
    TEST(MIDDriver, SEQUENCE_NAME_META_EVENT)
    {
        using audio::stubs::StubMixer;
        using audio::midi::MIDIEvent;
        using audio::midi::MIDI_EVENT_TYPES_HIGH;
        using audio::midi::MIDI_META_EVENT_TYPES_LOW;
        using audio::midi::MIDI_META_EVENT;

        auto mixer = std::make_shared<StubMixer>();
        auto device = std::make_shared<midi::devices::SpyDevice>();

        // capture spdlog output
        std::ostringstream _oss;
        auto ostream_logger = spdlog::get("gtest_logger");
        if (!ostream_logger)
        {
            auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(_oss);
            ostream_logger = std::make_shared<spdlog::logger>("gtest_logger", ostream_sink);
            ostream_logger->set_pattern(">%v<");
            ostream_logger->set_level(spdlog::level::debug);
        }
        auto default_logger = spdlog::default_logger();
        spdlog::set_default_logger(ostream_logger);
        // ---

        MIDIEvent e;
        e.delta_time = 0;
        e.type.high = (uint8_t)MIDI_EVENT_TYPES_HIGH::META_SYSEX;
        e.type.low = (uint8_t)MIDI_META_EVENT_TYPES_LOW::META;
        
        // this can be a parameter
        e.data.push_back((uint8_t)MIDI_META_EVENT::SEQUENCE_NAME);

        std::string s = "sequence_name";
        e.data.insert(e.data.end(),s.begin(), s.end());

        auto midi_track = audio::midi::MIDITrack();
        midi_track.addEvent(e);
        
        MIDDriverMock middrv(mixer, device);
        middrv.protected_processTrack(midi_track, 0);

        // retrieve spdlog output
        std::string output = _oss.str();
        
        EXPECT_NE(_oss.str().find("SEQUENCE NAME: " + s), std::string::npos);
        
        // TODO the spdlog output capture shouold be encapsulated in a class with a SetUp method (and teardown)
        _oss.clear();
        spdlog::set_default_logger(default_logger);
    }

    TEST(MIDDriver, acquire)
    {
        using audio::stubs::StubMixer;
        using audio::midi::MIDIEvent;
        using audio::midi::MIDI_EVENT_TYPES_HIGH;
        using audio::midi::MIDI_META_EVENT_TYPES_LOW;
        using audio::midi::MIDI_META_EVENT;
        using audio::midi::MIDI_FORMAT;

        auto midi_track = audio::midi::MIDITrack();

        MIDIEvent e;

        e.delta_time = 50;
        e.type.high = (uint8_t)MIDI_EVENT_TYPES_HIGH::PROGRAM_CHANGE;
        e.type.low = 0;
        e.data.push_back(0);
        e.data.push_back(0);
        midi_track.addEvent(e);

        e.delta_time = 0;
        e.type.high = (uint8_t)MIDI_EVENT_TYPES_HIGH::META_SYSEX;
        e.type.low = (uint8_t)MIDI_META_EVENT_TYPES_LOW::META;
        e.data.push_back((uint8_t)MIDI_META_EVENT::END_OF_TRACK);
        e.data.push_back(0);
        midi_track.addEvent(e);

        auto midi = std::make_shared<audio::MIDI>(MIDI_FORMAT::SINGLE_TRACK, 1, 96);
        midi->addTrack(midi_track);
        auto mixer = std::make_shared<StubMixer>();
        auto device = std::make_shared<midi::devices::SpyDevice>();

        MIDDriverMock middrv1(mixer, device);
        EXPECT_EQ(device.use_count(), 2);
        MIDDriverMock middrv2(mixer, device);
        EXPECT_EQ(device.use_count(), 3);

        ASSERT_FALSE(device->isAcquired());
        middrv1.play(midi);
        ASSERT_TRUE(device->isAcquired());
        middrv2.play(midi);
        ASSERT_FALSE(middrv2.isPlaying());
        middrv1.stop();
        ASSERT_FALSE(device->isAcquired());
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
