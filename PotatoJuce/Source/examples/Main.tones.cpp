#include <JuceHeader.h>

/////
using namespace juce;

int main(int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI gui_init;// This does not actually init GUI
	AudioDeviceManager adm;
	String err = adm.initialiseWithDefaultDevices(0, 2);
	if (err.isEmpty())
	{
		std::cout << "Device opened: " << adm.getCurrentAudioDevice()->getName() << "\n";
		ToneGeneratorAudioSource tonesource; // Juce provided AudioSource based sine generator
		tonesource.setFrequency(440.0);
		tonesource.setAmplitude(0.1);
		AudioSourcePlayer asp; // wraps the tone generator so it can be used as an AudioIODeviceCallback
		asp.setSource(&tonesource);
		adm.addAudioCallback(&asp);
		while (true)
		{
			double freq = 0.0;
			std::cin >> freq;// Get input from CLI
			if (freq > 0.0)
				tonesource.setFrequency(freq);
			else
				break;
		}
		std::cout << "Closing device...\n";
	}
	else
		std::cout << "Could not open device: " << err << "\n";
	return 0;
}