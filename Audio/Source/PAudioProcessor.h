/*
  ==============================================================================

    PAudioProcessor.h
    Created: 1 Apr 2023 9:51:14pm
    Author:  mathieu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PSynth.h"
#include "PWave_WhiteNoise.h"

class PAudioProcessor{
public:
  PAudioProcessor(){
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
    synth.setWaveEngine("demo");
    float pVals[4];
    float frequency = 440.0;
    synth.setFrequency(frequency);
    // Runs with CLI input
    while (true)
    {
      // freq p0 p1 p2 p3
      DBG("Usage: freq p0 p1 p2 p3");
      std::cin >> frequency;// Get input from CLI
      synth.setFrequency(frequency);
      for(int i = 0; i < 4; i++){
        std::cin >> pVals[i];// Get input from CLI
        juce::jmin(pVals[i], 1.0f);
        synth.setParam(i,pVals[i]);
      }
    }
    std::cout << "Closing device...\n";
  }

private:
  juce::ScopedJuceInitialiser_GUI gui_init;// This does not actually init GUI
  juce::AudioDeviceManager adm;
  juce::AudioSourcePlayer asp;
  PSynth synth;
};