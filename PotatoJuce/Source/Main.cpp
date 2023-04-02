#include <JuceHeader.h>

/////
using namespace juce;

class Synth : public AudioSource {
	void getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill) override
	{
	   Random random;
	   // Fill the buffer with white noise
   		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
   		{
   		    float* channelData = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
   		    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
   		    {
   		        channelData[sample] = random.nextFloat() * 2.0f - 1.0f; // Generate random values between -1 and 1
   		    }
   		} 

	}
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{

	}
	void releaseResources() override
	{

	}	
};


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
		double freq = 0.0;
		std::cin >> freq;// Get input from CLI
		if (freq > 0.0)
			//tonesource.setFrequency(freq);
			DBG("White Noise...");
		else
			break;
	}
	std::cout << "Closing device...\n";
	return 0;
}
