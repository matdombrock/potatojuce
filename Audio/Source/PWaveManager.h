#pragma once
#include <JuceHeader.h>
#include "PParams.h"
#include "PWaveEngine.h"
#include "PWave_WhiteNoise.h"
#include "PWave_Sine.h"
#include "PWave_Sine2.h"
#include "PWave_Demo.h"
class PWaveManager {
public:
  PWaveManager(PParams * params) : 
    wave_whiteNoise(params), 
    wave_sine(params),
    wave_sine2(params),
    wave_demo(params)
  {

  }
  PWaveEngine * getEngine(juce::String waveEngineName){
    return waveEngines[waveEngineName];
  }
  bool hasEngine(juce::String waveEngineName){
    if(waveEngines.find(waveEngineName) != waveEngines.end()){
      return true;
    }
    return false;
  }
private:
  PWave_WhiteNoise wave_whiteNoise;
  PWave_Sine wave_sine;
  PWave_Sine2 wave_sine2;
  PWave_Demo wave_demo;
  std::map<juce::String, PWaveEngine*> waveEngines = {
		{"default", &wave_whiteNoise},
		{"noise", &wave_whiteNoise},
		{"sine", &wave_sine},
    {"sine2", &wave_sine2},
    {"demo", &wave_demo}
	};
};
	