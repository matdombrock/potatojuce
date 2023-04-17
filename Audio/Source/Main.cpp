#include <JuceHeader.h>
#include "AudioProcessor.h"

/////
using namespace juce;

/*
	Main Entry Point
*/
int main(int argc, char* argv[])
{
	AudioProcessor pap;
	pap.mainLoop();
	return 0;
}
