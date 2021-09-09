#include <drivers/VOCDriver.hpp>
#include <cassert>

namespace drivers
{
    using audio::scummvm::Mixer;
    using audio::scummvm::SoundHandle;
    using audio::scummvm::AudioStream;

    VOCDriver::VOCDriver(std::shared_ptr<audio::scummvm::Mixer> mixer, std::shared_ptr<files::VOCFile> voc_file) :
        _mixer(mixer), _voc_file(voc_file)
    {
        _handle = new SoundHandle();
        _stereo = voc_file->getChannels() == 2;
        _sampleRate = voc_file->getSampleRate();
        _dataSize = voc_file->getDataSize();
        _data = voc_file->getData();

        // convert VOC File to PCM data
       // _voc_file->_data_blocks
    }

    VOCDriver::~VOCDriver()
    {
        delete _handle;
    }

    int VOCDriver::readBuffer(int16_t* buffer, const int numSamples)
    {
        // TODO convert PCM 8 bit to 16 bit
        int monoFactor = _stereo ? 1 : 2;
        int len = numSamples / monoFactor; // mixer is stereo
        int rest = (_dataSize - _curPos);
        int remaining = std::min(len, rest);
        
        // if voc is stereo.
        if (_stereo) {
            for (int i = 0; i < remaining; i++) {
                buffer[i] = _data[_curPos + i];
            }

            _curPos += remaining;
        }
        else {
            // mono VO
            for (int i = 0; i < remaining * 2; i += 2) {
                buffer[i] = buffer[i + 1] = _data[_curPos++];
            }
        }


        if (rest < len) {
           // std::memset(&buffer[_curPos], 0, (len - rest)*monoFactor);
        }

        return remaining;
    }

    bool VOCDriver::isStereo() const
    {
        return _stereo;
    }

    int VOCDriver::getRate() const
    {
        return _sampleRate;
    }

     

    bool VOCDriver::endOfData() const
    {
        return _curPos == _dataSize;
    }


    void VOCDriver::play()
    {
        // TODO review, could be speech instead of SFX
        _curPos = 0;
        _mixer->playStream(
            Mixer::SoundType::SFX,
            _handle,
            this,
            -1,
            Mixer::MaxVolume::CHANNEL,
            0,
            false
        );
    }
}
