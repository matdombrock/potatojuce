/*
	The base synth class which other synths will inherit
	Actual audio sample generation is handled by the WaveEngine instance

	Todo:
	- Rename this file to `AudioEngine.h`
*/

#pragma once
#include <JuceHeader.h>
#include <map>
#include "AudioUtils.h"
#include "Params.h"
#include "WaveEngine.h"
#include "WaveManager.h"

class Synth : public juce::AudioSource{
public:
	Synth() : waveManager(&params){
		waveEngine = waveManager.getEngine("default");
	}
	/*  
		Overrides
	*/
	void getNextAudioBlock(const juce::AudioSourceChannelInfo & bufferToFill) override
	{
		for (int sample = 0; sample < bufferToFill.numSamples; ++sample){
			float sampleVal = waveEngine->getNextSample();
			sampleVal *= amplitude;
			for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel){
				float* channelData = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
				channelData[sample] = sampleVal;
			}
		}
	}
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		DBG("Prepared Synth");
		//DBG("Sample Rate: ("+std::string(sampleRate)+")");
		sampleRateCache = sampleRate;
		waveEngine->prepareToPlay(sampleRate);
	}
	void releaseResources() override
	{

	}
	/*
		Custom methods
	*/	
	void setWaveEngine(std::string newWaveEngineName){
		if(waveManager.hasEngine(newWaveEngineName)){
			waveEngine = waveManager.getEngine(newWaveEngineName);
			waveEngine->prepareToPlay(sampleRateCache);
			std::cout << "Set WaveEngine: "+newWaveEngineName+"\n";
		}
		else{
			std::cout << "Can't find wave engine!\n";
		}
	}
	std::string listWaveEngines(){
		return waveManager.getWaveEngines();
	}
	// Wrapper for params.setParam()
	void setParam(int index, float value){
		params.setParam(index, value);
	}
	// Wrapper for waveEngine.setFrequency()
	void setFrequency(float newFreq=440.0){
		waveEngine->setFrequency(newFreq);
	}
	void setAmplitude(float newAmp){
		amplitude = newAmp;
	}
private:
	double sampleRateCache = 0;
	float amplitude = 0.5;
	Params params;
	std::string waveEngineName = "default";
	WaveEngine * waveEngine;
	WaveManager waveManager;
};