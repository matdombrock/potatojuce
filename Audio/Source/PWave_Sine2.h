#pragma once
#include "PWaveEngine.h"
#include "PAudioUtils.h"
#include "RALowpass1.h"


class PWave_Sine2 : public PWaveEngine{
public:
  PWave_Sine2(PParams * params) : PWaveEngine(params){

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

    float sig1 = sin(currentAngle);// Fast square
    float sig2 = sin(currentAngle * detune);// Not real detune
    float sample = (sig1 + sig2) / 2;

    sample = lowpass.process(sample, lowpassLvl);

    tick();
    return sample;
  }
  void prepareToPlayWave(double sampleRate) override{
    lowpass.prepareToPlay(sampleRate);
  }
private:
  RALowpass1 lowpass;
};