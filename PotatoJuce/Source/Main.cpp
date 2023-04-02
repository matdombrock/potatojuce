#include <JuceHeader.h>

/////
using namespace juce;

/*
	Utility Functions
*/

float whiteNoise(){
	Random random;
	return random.nextFloat() * 2.0f - 1.0f; // Generate random values between -1 and 1
}

/*
	The base synth class which other synths will inheret
*/
class Synth : public AudioSource {
public:
	Synth(){

	}
	/*  
		Overrides
	*/
	void getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill) override
	{
		// Fill the buffer with white noise
   		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
   		{
   		    float* channelData = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
   		    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
   		    {
				float sampleVal = whiteNoise();
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

/*
	Main Entry Point
*/
int main(int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI gui_init;// This does not actually init GUI
	AudioDeviceManager adm;
	String err = adm.initialiseWithDefaultDevices(0, 2);
	if (!err.isEmpty())
	{
		std::cout << "Could not open device: " << err << "\n";
		return 0;
	}
	std::cout << "Device opened: " << adm.getCurrentAudioDevice()->getName() << "\n";
	AudioSourcePlayer asp; // wraps the tone generator so it can be used as an AudioIODeviceCallback
	Synth synth;
	asp.setSource(&synth);
	adm.addAudioCallback(&asp);
	while (true)
	{
		float amp = 0.0;
		std::cin >> amp;// Get input from CLI
		if (amp > 0.0){
			jmin(amp, 1.0f);
			synth.setAmplitude(amp);
		}
		else
			break;
	}
	std::cout << "Closing device...\n";
	return 0;
}
