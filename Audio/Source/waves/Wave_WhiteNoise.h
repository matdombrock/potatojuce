#pragma once
#include "../WaveEngine.h"
#include "../AudioUtils.h"
#include "../ra/RALowpass1.h"


class Wave_WhiteNoise : public WaveEngine{
public:
  Wave_WhiteNoise(Params * params) : WaveEngine(params){

  }
  float getNextSample() override{
    float pVal = params->getParam(0);
    //DBG(amp);
    float sample = AudioUtils::whiteNoise();
    sample = lowpass.process(sample, pVal*12*1000);
    return sample;
  }
  void prepareToPlayWave(double sampleRate) override{
    lowpass.prepareToPlay(sampleRate);
  }
private:
  RALowpass1 lowpass;
};