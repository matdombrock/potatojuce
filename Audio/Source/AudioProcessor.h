/*
  The main audio processor class
*/

#pragma once
#include <JuceHeader.h>
#include "Synth.h"
#include "CLI.h"

class AudioProcessor{
public:
  AudioProcessor(){
    DBG("AudioProcessor Init");
  }

  void prepareToPlay(){
    juce::String err = adm.initialiseWithDefaultDevices(0, 2);
    if (!err.isEmpty())
    {
      std::cout << "Could not open device: " << err << "\n";
      return;
    }
    std::cout << "Device opened: " << adm.getCurrentAudioDevice()->getName() << "\n";
    asp.setSource(&synth);
    adm.addAudioCallback(&asp);
    DBG("AudioProcessor Prepared");
  }

  void mainLoop(){
    prepareToPlay();
    // Setup the synth
    synth.setWaveEngine("sine2");
    float frequency = 440.0f;
    synth.setFrequency(frequency);
    // Runs with CLI input
    CLI cli(&synth);
    cli.cliLoop();
    std::cout << "Closing device...\n";
  }

private:
  juce::ScopedJuceInitialiser_GUI gui_init;// This does not actually init GUI
  juce::AudioDeviceManager adm;
  juce::AudioSourcePlayer asp;
  Synth synth;
};