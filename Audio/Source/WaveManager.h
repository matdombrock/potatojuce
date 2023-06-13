/*
  Manages a list of all available wave engines
*/

#pragma once
#include <JuceHeader.h>
#include "Params.h"
#include "./WaveEngine.h"
#include "./waves/Wave_WhiteNoise.h"
#include "./waves/Wave_Sine.h"
#include "./waves/Wave_Sine2.h"
#include "./waves/Wave_Demo.h"
class WaveManager {
public:
  WaveManager(Params * params) : 
    wave_whiteNoise(params), 
    wave_sine(params),
    wave_sine2(params),
    wave_demo(params)
  {

  }
  // Returns a pointer to the requested engine
  WaveEngine * getEngine(std::string waveEngineName){
    return waveEngines[waveEngineName];
  }
  // Returns a pointer to the next engine in the list
  WaveEngine * getNextEngine(){
    // Todo: make this work
    return &wave_demo;// Bad
  }
  // Returns a pointer to the previous engine in the list
  WaveEngine * getPrevEngine(){
    // Todo: make this work
    return &wave_demo;// Bad
  }
  std::string getWaveEngines(){
    std::string out;
    for(auto i : waveEngines){
      out += i.first + "\n";
    }
    return out;
  }
  
  // Returns true if `waveEngineName` is a valid name
  bool hasEngine(std::string waveEngineName){
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
  std::map<std::string, WaveEngine*> waveEngines = {
		{"default", &wave_whiteNoise},
		{"noise", &wave_whiteNoise},
		{"sine", &wave_sine},
    {"sine2", &wave_sine2},
    {"demo", &wave_demo}
	};
};
	