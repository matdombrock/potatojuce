#pragma once
#include <JuceHeader.h>
#include "Params.h"
#include "WaveEngine.h"
#include "Wave_WhiteNoise.h"
#include "Wave_Sine.h"
#include "Wave_Sine2.h"
#include "Wave_Demo.h"
class WaveManager {
public:
  WaveManager(Params * params) : 
    wave_whiteNoise(params), 
    wave_sine(params),
    wave_sine2(params),
    wave_demo(params)
  {

  }
  WaveEngine * getEngine(juce::String waveEngineName){
    return waveEngines[waveEngineName];
  }
  bool hasEngine(juce::String waveEngineName){
    if(waveEngines.find(waveEngineName) != waveEngines.end()){
      return true;
    }
    return false;
  }
private:
  Wave_WhiteNoise wave_whiteNoise;
  Wave_Sine wave_sine;
  Wave_Sine2 wave_sine2;
  Wave_Demo wave_demo;
  std::map<juce::String, WaveEngine*> waveEngines = {
		{"default", &wave_whiteNoise},
		{"noise", &wave_whiteNoise},
		{"sine", &wave_sine},
    {"sine2", &wave_sine2},
    {"demo", &wave_demo}
	};
};
	