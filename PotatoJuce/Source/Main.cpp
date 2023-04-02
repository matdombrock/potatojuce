#include <JuceHeader.h>
#include "PAudioProcessor.h"

/////
using namespace juce;

/*
	Main Entry Point
*/
int main(int argc, char* argv[])
{
	PAudioProcessor pap;
	pap.mainLoop();
	return 0;
}
