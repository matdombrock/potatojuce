/*
  Handles the command line interface
  which doubles as an IPC layer to interpret 
  signals from the hardware interface
*/

#pragma once
#include <JuceHeader.h>
#include "Synth.h"

class CLI{
public:
  CLI(Synth * pSynth){
    synth = pSynth;
  }
  void cliLoop(){
    DBG("Starting CLI Loop...");
    //
    DBG("Available commands:");
    DBG("p[0->3] [val] - Set a param value");
    DBG("f [val] - Set frequency");
    DBG("a [val] - Set amplitude");
    DBG("w [wave-name] - Set wave mode");
    DBG("q - Exit the process");
    //
    while (true)
    {
      DBG("Usage: [cmd] [val]");
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
      else if(cmd == "a"){
        synth->setAmplitude(val);
      }
      else if(cmd == "w"){
        synth->setWaveEngine(valS);
      }
      else if(cmd == "q"){
        // Quit
        return;
      }
      // Not found
      else{
        std::cout << "Unknown cmd: " << cmd << std::endl;
      }
    }
  }
private:
  Synth * synth;
  float frequency = 440.0f;
  float pVals[4];
};