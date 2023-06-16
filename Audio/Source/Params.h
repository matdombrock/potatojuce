/*
  This class handles the paramater controls
*/
#pragma once
#include <JuceHeader.h>
class Params{
public:
  void setParam(int index, float value){
		// Sanitize
		value = juce::jmax(value,0.0f);
		value = juce::jmin(value,1.0f);

		params[index] = value;
    DBG("Set Param: "+juce::String(index)+" - "+juce::String(value));
	}
  float getParam(int index){
    return params[index];
  }
  float[] getParams(){
    return params;
  }
private:
  float params[4] = {0,0,0,0};
};