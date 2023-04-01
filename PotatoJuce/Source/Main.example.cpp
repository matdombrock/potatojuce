/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

/////
using namespace juce;

int main (int argc, char* argv[])
{
    String fileLoc = "/home/mathieu/test.wav";
    DBG("Attempting to play file at: ");
    DBG(fileLoc);
    File input (fileLoc);
    AudioFormatManager fmgr;
    fmgr.registerBasicFormats();
    ScopedPointer<AudioFormatReaderSource> source =
        new AudioFormatReaderSource (fmgr.createReaderFor(input), true);

    AudioDeviceManager devmgr;
    devmgr.initialiseWithDefaultDevices (0, 2);
    AudioIODevice* device = devmgr.getCurrentAudioDevice();
    if (device && source) {
        source->prepareToPlay (device->getDefaultBufferSize(),
                               device->getCurrentSampleRate());
        ScopedPointer<AudioSourcePlayer> player = new AudioSourcePlayer ();
        player->setSource (source);
        devmgr.addAudioCallback (player);
        while (source->getNextReadPosition() < source->getTotalLength())
            Thread::sleep(100);
        return 0;
    }
    else{
    	DBG("ERROR: Cant play file");
    }
    return -1;
}


//==============================================================================
int mainx (int argc, char* argv[])
{

    // ..your code goes here!


    return 0;
}
