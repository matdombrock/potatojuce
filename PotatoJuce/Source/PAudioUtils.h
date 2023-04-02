/*
  ==============================================================================

    Util.h
    Created: 1 Apr 2023 9:24:28pm
    Author:  mathieu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
/*
	Utility Functions
*/
class PAudioUtils{
public:
  // Generate a white noise sample
  static float whiteNoise(){
    juce::Random random;
	  return random.nextFloat() * 2.0f - 1.0f; // Generate random values between -1 and 1
  }
};