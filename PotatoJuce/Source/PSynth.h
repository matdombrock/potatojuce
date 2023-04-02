/*
  ==============================================================================

    Synth.h
    Created: 1 Apr 2023 9:23:55pm
    Author:  mathieu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "./PAudioUtils.h"
/*
	The base synth class which other synths will inheret
*/
class PSynth : public juce::AudioSource {
public:
	PSynth(){

	}
	/*  
		Overrides
	*/
	void getNextAudioBlock(const juce::AudioSourceChannelInfo & bufferToFill) override
	{
		// Fill the buffer with white noise
		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			float* channelData = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				float sampleVal = PAudioUtils::whiteNoise();
				sampleVal *= amplitude;
				channelData[sample] = sampleVal; 
			}
		} 
	}
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{

	}
	void releaseResources() override
	{

	}
	/*
		Custom methods
	*/	
	void setAmplitude(float newAmp){
		amplitude = newAmp;
	}
private:
	float amplitude = 0.5;
};