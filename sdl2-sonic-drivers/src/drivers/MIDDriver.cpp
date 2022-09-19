#include <drivers/MIDDriver.hpp>
#include <spdlog/spdlog.h>
#include <utils/algorithms.hpp>
#include <array>
#include <thread>

namespace drivers
{
    constexpr int DEFAULT_MIDI_TEMPO = 500000;
    constexpr int PAUSE_MILLIS = 100;
    constexpr unsigned int tempo_to_micros(const uint32_t tempo, const uint16_t division)
    {
        return static_cast<unsigned int>(static_cast<float>(tempo) / static_cast<float>(division));
    }

    MIDDriver::MIDDriver(const std::shared_ptr<audio::scummvm::Mixer>& mixer, const std::shared_ptr<midi::Device>& device)
        : _mixer(mixer), _device(device)
    {
    }

    MIDDriver::~MIDDriver()
    {
        stop();
    }
    
    void MIDDriver::play(const std::shared_ptr<audio::MIDI>& midi) noexcept
    {
        using audio::midi::MIDI_FORMAT;

        if (midi->format != MIDI_FORMAT::SINGLE_TRACK && midi->numTracks != 1) {
            spdlog::critical("MIDI format single track only supported");
            return;
        }

        if (midi->division & 0x8000)
        {
            // ticks per frame
            int smpte = (midi->division & 0x7FFF) >> 8;
            int ticksPerFrame = midi->division & 0xFF;
            switch (smpte)
            {
            case -24:
            case -25:
            case -29:
            case -30:
                break;
            default:
                spdlog::warn("Division SMPTE not known = {}", smpte);
            }

            spdlog::debug("Division: Ticks per frame = {}, smpte", ticksPerFrame, smpte);
            spdlog::warn("not immplemented divsion ticks per frame yet");
        }
        else
        {
            // ticks per quarter note
            spdlog::debug("Division: Ticks per quarter note = {}", midi->division & 0x7FFF);
        }

        stop();
        _isPlaying = true;
        if (!_device->acquire(this)) {
            stop();
            return;
        }
        _player = std::thread(&MIDDriver::processTrack, this, midi->getTrack(), midi->division & 0x7FFF);
    }

    void MIDDriver::stop() noexcept
    {
        _force_stop = true;
        _paused = false;
        if(_player.joinable())
            _player.join();
        _force_stop = false;
        _isPlaying = false;
        _device->release(this); // this line is making issue, must be aquired by "this"
    }

    void MIDDriver::pause() noexcept
    {
        if (_isPlaying)
            _paused = true;
    }

    void MIDDriver::resume() noexcept
    {
        if (_isPlaying)
            _paused = false;
    }

    bool MIDDriver::isPlaying() const noexcept
    {
        return _isPlaying;
    }

    bool MIDDriver::isPaused() const noexcept
    {
        return _paused;
    }

    void MIDDriver::processTrack(const audio::midi::MIDITrack& track, const uint16_t division)
    {
        using audio::midi::MIDI_EVENT_TYPES_HIGH;
        using audio::midi::MIDI_META_EVENT_TYPES_LOW;
        using audio::midi::MIDI_META_EVENT;

        _isPlaying = true;
        _force_stop = false;
        uint32_t tempo = DEFAULT_MIDI_TEMPO; //120 BPM;
        int cur_time = 0; // ticks
        unsigned int tempo_micros = tempo_to_micros(tempo, division);
        spdlog::debug("tempo_micros = {}", tempo_micros);
        unsigned int start = utils::getMicro<unsigned int>();
        const auto& tes = track.getEvents();
        
        for (const auto& e : tes)
        {
            bool paused = false;
            while(_paused)
            {
                if (!paused)
                {
                    paused = true;
                    _device->pause();
                }
                utils::delayMillis(PAUSE_MILLIS);
                start = utils::getMicro<unsigned int>();
            }

            if (paused)
            {
                _device->resume();
                //paused = false; // it will be false next loop iteration
            }

            if (_force_stop)
                break;

            std::string str;
            switch (static_cast<MIDI_EVENT_TYPES_HIGH>(e.type.high))
            {
            case MIDI_EVENT_TYPES_HIGH::META_SYSEX:
            {
                switch (static_cast<MIDI_META_EVENT_TYPES_LOW>(e.type.low))
                {
                case MIDI_META_EVENT_TYPES_LOW::META: {
                    const uint8_t type = e.data[0]; // must be < 128
                    switch (static_cast<audio::midi::MIDI_META_EVENT>(type))
                    {
                    case MIDI_META_EVENT::CHANNEL_PREFIX:
                        spdlog::warn("CHANNEL_PREFIX {:d} not implemented", e.data[1]);
                        break;
                    case MIDI_META_EVENT::COPYRIGHT:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("CopyRight: {}", str);
                        break;
                    case MIDI_META_EVENT::CUE_POINT:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("Cue Point: {}",  str);
                        break;
                    case MIDI_META_EVENT::DEVICE_NAME:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::warn("[Not Implemented] Device Name: {}", str);
                        break;
                    case MIDI_META_EVENT::END_OF_TRACK:
                        spdlog::debug("MIDI end of track.");
                        //_force_stop = true;
                        break;
                    case MIDI_META_EVENT::INSTRUMENT_NAME:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("Instrument name: {}", str);
                        break;
                    case MIDI_META_EVENT::KEY_SIGNATURE:
                        spdlog::info("KEY_SIGNATURE: {:d} {:d}", e.data[1], e.data[2]);
                        break;
                    case MIDI_META_EVENT::LYRICS:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("Lyrics: {}", str);
                        break;
                    case MIDI_META_EVENT::MARKER:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("Marker: {}", str);
                        break;
                    case MIDI_META_EVENT::MIDI_PORT:
                        spdlog::warn("MIDI_PORT {:d} not implemented", e.data[1]);
                        break;
                    case MIDI_META_EVENT::PROGRAM_NAME:
                        str = utils::chars_vector_to_string_skip_first(e.data);
                        spdlog::info("PROGRAM_NAME: {}", str);
                        break;
                    case MIDI_META_EVENT::SEQUENCER_SPECIFIC:
                        spdlog::warn("SEQUENCE_SPECIFIC not implemented");
                        break;
                    case MIDI_META_EVENT::SEQUENCE_NAME: // a.k.a track name
                        str = utils::chars_vector_to_string(++(e.data.begin()), e.data.end());
                        spdlog::info("SEQUENCE NAME: {}", str);
                        break;
                    case MIDI_META_EVENT::SEQUENCE_NUMBER:
                        spdlog::warn("Sequence number not implemented");
                        break;
                    case MIDI_META_EVENT::SET_TEMPO: {
                        const int skip = 1;
                        tempo = (e.data[skip] << 16) + (e.data[skip + 1] << 8) + (e.data[skip + 2]);
                        tempo_micros = tempo_to_micros(tempo, division);
                        spdlog::info("Tempo {}, ({} bpm) -- microseconds/tick {}", tempo, 60000000 / tempo, tempo_micros);
                        break;
                    }
                    case MIDI_META_EVENT::SMPTE_OFFSET:
                        spdlog::warn("SMPTE_OFFSET not implemented");
                        break;
                    case MIDI_META_EVENT::TEXT:
                        str = utils::chars_vector_to_string(++(e.data.begin()), e.data.end());
                        spdlog::info("Text: {}", str);
                        break;
                    case MIDI_META_EVENT::TIME_SIGNATURE:
                    spdlog::info("TIME_SIGNATURE: {:d}/{:d} - clocks {:d} - bb {:d} ", e.data[1], utils::powerOf2(e.data[2]), e.data[3], e.data[4]);
                        break;
                    default:
                        spdlog::warn("MIDI_META_EVENT_TYPES_LOW not implemented/recognized: {:#02x}", type);
                        break;
                    }

                    continue; // META event processed, go on next MIDI event
                }
                case MIDI_META_EVENT_TYPES_LOW::SYS_EX0: {
                    spdlog::debug("SYS_EX0 META event...");
                    _device->sendSysEx(e);
                    continue;
                }
                case MIDI_META_EVENT_TYPES_LOW::SYS_EX7: {
                    spdlog::debug("SYS_EX7 META event...");
                    _device->sendSysEx(e);
                    continue;
                    //break;
                }
                default: {
                    break;
                }
                }
                break;
            }
            case MIDI_EVENT_TYPES_HIGH::NOTE_OFF:
            case MIDI_EVENT_TYPES_HIGH::NOTE_ON:
            case MIDI_EVENT_TYPES_HIGH::AFTERTOUCH:
            case MIDI_EVENT_TYPES_HIGH::CONTROLLER:
            case MIDI_EVENT_TYPES_HIGH::PITCH_BEND:
                /*msg[0] = e.type.val;
                msg[1] = e.data[0];
                msg[2] = e.data[1];
                msg_size = 3;*/
                break;
            case MIDI_EVENT_TYPES_HIGH::PROGRAM_CHANGE:
            case MIDI_EVENT_TYPES_HIGH::CHANNEL_AFTERTOUCH:
                /*msg[0] = e.type.val;
                msg[1] = e.data[0];
                msg_size = 2;*/
                break;
            default:
                break;
            }

            if (e.delta_time != 0)
            {
                cur_time += e.delta_time;
                const unsigned int delta_delay = tempo_micros * e.delta_time;
                const unsigned int end = utils::getMicro<unsigned int>();
                const long dd = static_cast<long>(delta_delay - (end - start));
                start = end;

                if (dd > 0) {
                    utils::delayMicro(dd);
                    start += dd;
                }
                else {
                    spdlog::warn("cur_time={}, delta_delay={}, micro_delay_time={}", cur_time, delta_delay, dd);
                }
            }

            _device->sendEvent(e);
        }

        _isPlaying = false;
        _device->release(this);
    }
}
