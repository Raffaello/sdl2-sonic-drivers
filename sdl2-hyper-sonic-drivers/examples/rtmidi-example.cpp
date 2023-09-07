#include <HyperSonicDrivers/files/MIDFile.hpp>
#include <HyperSonicDrivers/drivers/MIDDriver.hpp>
#include <HyperSonicDrivers/drivers/midi/devices/Native.hpp>
#include <HyperSonicDrivers/utils/algorithms.hpp>

#include <iostream>
#include <SDL2/SDL.h>

#include <rtmidi/RtMidi.h>
#include <memory>
#include <vector>

#include <spdlog/spdlog.h>

using namespace HyperSonicDrivers;

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    // TODO: as this example shows rtMidi is outside the mixer
    //       it implies it can be muted, stopped panned, chnage volume, etc..
    //       like other music/sounds going through the mixer.
    //       Check if it can be wired to the mixer
    //       Otherwise this device Native and rtMidi dependency must be optional (or even removed or not official)

    std::shared_ptr<RtMidiOut> midiout;

    try {
        midiout = std::make_shared<RtMidiOut>();
    }
    catch (RtMidiError& error) {
        // Handle the exception here
        error.printMessage();
    }

    // Check outputs.
    unsigned int nPorts = midiout->getPortCount();
    std::string portName;
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midiout->getPortName(i);
        }
        catch (RtMidiError& error) {
            error.printMessage();
        }
        std::cout << "  Output Port #" << i + 1 << ": " << portName << endl;
    }
    std::cout << endl;

    cout << "Midi note test...";
    // Open first available port.
    midiout->openPort(0);
    // Send out a series of MIDI messages.
    std::vector<unsigned char> message;
    // Program change: 192, 5
    message.push_back(192);
    message.push_back(5);
    midiout->sendMessage(&message);
    // Control Change: 176, 7, 100 (volume)
    message[0] = 176;
    message[1] = 7;
    message.push_back(100);
    midiout->sendMessage(&message);
    // Note On: 144, 64, 90
    message[0] = 144;
    message[1] = 64;
    message[2] = 90;
    midiout->sendMessage(&message);
    utils::delayMillis(1000);
    // Note Off: 128, 64, 40
    message[0] = 128;
    message[1] = 64;
    message[2] = 40;
    midiout->sendMessage(&message);

    cout << "OK" << endl;
    midiout->closePort();

    // Device Native is using RtMidi
    auto native = std::make_shared<drivers::midi::devices::Native>();
    drivers::MIDDriver middrv(native);
    auto midi = files::MIDFile("midifile_sample.mid").getMIDI();
    middrv.play(midi);

    return 0;
}
