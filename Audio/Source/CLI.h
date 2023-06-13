/*
  Handles the command line interface
  which doubles as an IPC layer to interpret 
  signals from the hardware interface
*/

#pragma once
#include <iostream>
#include <fstream>
#include <JuceHeader.h>
#include <unistd.h> // sleep
#include "Synth.h"

// this should really be called IPC
class CLI{
public:
  CLI(Synth * pSynth){
    synth = pSynth;
    std::ofstream file;
    file.open ("/tmp/pj/waveEngines.txt");
    file << synth->listWaveEngines();
    DBG("Wrote wave engines to /tmp/pj/waveEngines.txt");
    file.close();
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
    DBG("-------");
    DBG("Write Commands to /tmp/pj/in");
    //
    while (true)
    {
      DBG("Usage: [cmd] [val]");
      std::string cmd;
      std::cin >> cmd;
      DBG("Got CMD: "+cmd);
      std::string valS;// A string is needed for some cmds
      
      float val;
      
      if(cmd == "f"){
        std::cin >> val;
        synth->setFrequency(val);
      }
      else if(cmd == "p0"){
        std::cin >> val;
        synth->setParam(0, val);
      }
      else if(cmd == "p1"){
        std::cin >> val;
        synth->setParam(1, val);
      }
      else if(cmd == "p2"){
        std::cin >> val;
        synth->setParam(2, val);
      }
      else if(cmd == "p3"){
        std::cin >> val;
        synth->setParam(3, val);
      }
      // Extras
      else if(cmd == "hax"){
        // Play a little song
        // blocking
        for(int i = 0; i < 32; i++){
          synth->setFrequency(880);
          usleep(500000/i);
          synth->setFrequency(440);
          usleep(500000/i);
          synth->setFrequency(666);
          usleep(500000/i);
        }
        synth->setFrequency(440);
      }
      else if(cmd == "a"){
        std::cin >> val;
        synth->setAmplitude(val);
      }
      else if(cmd == "w"){
        std::cin >> valS;
        synth->setWaveEngine(valS);
      }
      else if(cmd == "wg"){
        DBG("Wave Engines:");
        DBG(synth->listWaveEngines());
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