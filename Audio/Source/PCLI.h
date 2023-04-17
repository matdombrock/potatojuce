/*
  Handles the command line interface
  which doubles as an IPC layer to interpret 
  signals from the harware interface
*/

#pragma once
#include <JuceHeader.h>
#include "PSynth.h"

class PCLI{
public:
  PCLI(PSynth * pSynth){
    synth = pSynth;
  }
  void cliLoop(){
    DBG("Starting CLI Loop...");
    while (true)
    {
      DBG("Usage: cmd val");
      std::string cmd;
      std::cin >> cmd;
      std::string valS;// A string is needed for some cmds
      std::cin >> valS;
      float val;
      try{
        val = std::stof(valS);
      }
      catch (const std::invalid_argument& e) {
        //std::cout << "Error: " << e.what() << std::endl;
        val = 0;
      }
      
      if(cmd == "f"){
        synth->setFrequency(val);
      }
      else if(cmd == "p0"){
        synth->setParam(0, val);
      }
      else if(cmd == "p1"){
        synth->setParam(1, val);
      }
      else if(cmd == "p2"){
        synth->setParam(2, val);
      }
      else if(cmd == "p3"){
        synth->setParam(3, val);
      }
      // Extras
      else if(cmd == "q"){
        // Quit
        return;
      }
      else if(cmd == "a"){
        synth->setAmplitude(val);
      }
      else if(cmd == "e"){
        synth->setWaveEngine(valS);
      }
      // Not found
      else{
        std::cout << "Uknown cmd: " << cmd << std::endl;
      }
      // freq p0 p1 p2 p3
      // DBG("Usage: freq p0 p1 p2 p3");
      // std::cin >> frequency;// Get input from CLI
      // synth->setAmplitude(frequency);
      // for(int i = 0; i < 4; i++){
      //   std::cin >> pVals[i];// Get input from CLI
      //   juce::jmin(pVals[i], 1.0f);
      //   synth->setParam(i,pVals[i]);
      // }
    }
  }
private:
  PSynth * synth;
  float frequency = 440.0f;
  float pVals[4];
};