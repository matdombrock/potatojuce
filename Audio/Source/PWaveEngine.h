#pragma once
#include "PParams.h"
class PWaveEngine{
public:
  PWaveEngine(PParams * pparams){
    params = pparams;
  }
  virtual float getNextSample(){
    return 0.0f;
  }
  virtual void prepareToPlayWave(double sampleRate){
    return;
  }
  void prepareToPlay(double sampleRate){
    sampleRateCache = sampleRate;
    prepareToPlayWave(sampleRate);
    DBG("WaveEngine Prepared");
  }
  void tick(){
    iterateDelta();
  }
  void setFrequency(float newFreq){
    frequency = newFreq;
    calcDelta();
    DBG("Set Frequency: "+juce::String(frequency));
  }
  PParams *params;
  float frequency = 440.0f;
  double currentAngle = 0.0;
  double angleDelta = 0.0;
private:
  void iterateDelta(){
    currentAngle += angleDelta;
  }
  void calcDelta(){
    jassert(sampleRateCache > 0);
    double newDelta = (frequency/sampleRateCache);
    DBG("NEW DELTA:");
    DBG(newDelta);
    newDelta *= juce::MathConstants<double>::twoPi;
    angleDelta = newDelta;
  }
  double sampleRateCache = 0;
};