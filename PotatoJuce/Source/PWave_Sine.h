#pragma once
#include "PWaveEngine.h"


class PWave_Sine : public PWaveEngine{
public:
  PWave_Sine(PParams * params) : PWaveEngine(params){

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