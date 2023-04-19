/*
  Audio related utility functions
*/
#pragma once
#include <JuceHeader.h>

class AudioUtils{
public:
  // Generate a white noise sample
  static float whiteNoise(){
    juce::Random random;
	  return random.nextFloat() * 2.0f - 1.0f; // Generate random values between -1 and 1
  }
};