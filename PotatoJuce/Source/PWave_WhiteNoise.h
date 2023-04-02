#pragma once
#include "PWaveEngine.h"
#include "PAudioUtils.h"
#include "RALowpass1.h"


class PWave_WhiteNoise : public PWaveEngine{
public:
  PWave_WhiteNoise(PParams * params) : PWaveEngine(params){

  }
  float getNextSample() override{
    float pVal = params->getParam(0);
    //DBG(amp);
    float sample = PAudioUtils::whiteNoise();
    sample = lowpass.process(sample, pVal*12*1000);
    return sample;
  }
  void prepareToPlayWave(double sampleRate) override{
    lowpass.prepareToPlay(sampleRate);
  }
private:
  RALowpass1 lowpass;
};