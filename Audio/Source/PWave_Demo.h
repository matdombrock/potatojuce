#pragma once
#include "PWaveEngine.h"
#include "PAudioUtils.h"
#include "RALowpass1.h"


class PWave_Demo : public PWaveEngine{
public:
  PWave_Demo(PParams * params) : PWaveEngine(params){
    
  }
  float getNextSample() override{
    float detune = params->getParam(0);
    detune = detune <= 0 ? 0.0001 : detune;// Ensure we are not zero
    detune *= 6.24;//2pi
    
    float noiseLvl = params->getParam(1);
    noiseLvl /= 32;// Only a small amount
    float noiseSignal = PAudioUtils::whiteNoise();
    noiseSignal *= noiseLvl;

    // Apply noise to waveform
    currentAngle += noiseSignal;

    float lowpassLvl = params->getParam(2);
    lowpassLvl *= 20000;

    float sig1 = sin(currentAngle) > 0 ? 1 : 0;// Fast square
    float sig2 = sin(currentAngle * detune);// Not real detune
    float sample = (sig1 + sig2) / 2;

    sample = lowpass.process(sample, lowpassLvl);

    tick();
    return sample;
  }
  void prepareToPlayWave(double sampleRate) override{
    // We can override params here
    // if we want to
    params->setParam(0, 0);
    params->setParam(1, 0);
    params->setParam(2, 1);
    params->setParam(0, 0);
    DBG("Prepared Demo Synth");
    DBG("detune noise lowpass null");
    lowpass.prepareToPlay(sampleRate);
  }
private:
  RALowpass1 lowpass;
};