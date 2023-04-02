/*
  ==============================================================================

    PAudioProcessor.h
    Created: 1 Apr 2023 9:51:14pm
    Author:  mathieu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "./PSynth.h"

class PAudioProcessor{
public:
  PAudioProcessor(){
    
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
    adm.
    addAudioCallback(&asp);
  }
  void mainLoop(){
    prepareToPlay();
    while (true)
    {
      float amp = 0.0;
      std::cin >> amp;// Get input from CLI
      if (amp > 0.0){
        juce::jmin(amp, 1.0f);
        synth.setAmplitude(amp);
      }
      else
        break;
    }
    std::cout << "Closing device...\n";
  }

private:
  juce::ScopedJuceInitialiser_GUI gui_init;// This does not actually init GUI
  juce::AudioDeviceManager adm;
  juce::AudioSourcePlayer asp;
  PSynth synth;
};