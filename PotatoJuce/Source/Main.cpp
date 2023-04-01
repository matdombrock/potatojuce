#include <JuceHeader.h>

/////
using namespace juce;

int main(int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI gui_init;
	AudioDeviceManager aman;
	String err = aman.initialiseWithDefaultDevices(0, 2);
	if (err.isEmpty())
	{
		std::cout << "device opened : " << aman.getCurrentAudioDevice()->getName() << "\n";
		ToneGeneratorAudioSource tonesource; // Juce provided AudioSource based sine generator
		tonesource.setFrequency(440.0);
		tonesource.setAmplitude(0.1);
		AudioSourcePlayer asplayer; // wraps the tone generator so it can be used as an AudioIODeviceCallback
		asplayer.setSource(&tonesource);
		aman.addAudioCallback(&asplayer);
		while (true)
		{
			double freq = 0.0;
			std::cin >> freq;
			if (freq > 0.0)
				tonesource.setFrequency(freq);
			else
				break;
		}
		std::cout << "closing device...\n";
	}
	else
		std::cout << "could not open device : " << err << "\n";
	return 0;
}
