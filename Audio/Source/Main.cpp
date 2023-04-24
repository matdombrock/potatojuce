/*
	Main Entry Point
*/

#include <JuceHeader.h>
#include "AudioProcessor.h"

using namespace juce;

int main(int argc, char* argv[])
{
	AudioProcessor pap;
	pap.prepareToPlay();
	pap.setupSynth();
	pap.mainLoop();
	return 0;
}
