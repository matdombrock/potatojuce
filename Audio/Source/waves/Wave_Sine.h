#pragma once
#include "../WaveEngine.h"


class Wave_Sine : public WaveEngine{
public:
  Wave_Sine(Params * params) : WaveEngine(params){

  }
  float getNextSample() override{
    float sample = sin(currentAngle);
    tick();
    return sample;
  }
  void prepareToPlayWave(double sampleRate) override{
    return;
  }
private:
};