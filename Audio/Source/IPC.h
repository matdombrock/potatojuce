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

// DBG("Available commands:");
// DBG("p[0->3] [val] - Set a param value");
// DBG("f [val] - Set frequency");
// DBG("a [val] - Set amplitude");
// DBG("w [wave-name] - Set wave mode");
// DBG("q - Exit the process");
// DBG("-------");
// DBG("Write Commands to /tmp/pj/in");

// this should really be called IPC
class IPC{
public:
  IPC(Synth * pSynth){
    synth = pSynth;
    std::ofstream file;
    file.open ("/tmp/pj/waveEngines.txt");
    file << synth->listWaveEngines();
    file.close();
  }
  void ipcWatcher(){
    DBG("Watching at /tmp/pj/in");
    std::string cmd;
    std::string valS;// A string is needed for some cmds
    float val;
    while (true)
    {
      std::cin >> cmd;
     // DBG("Got cmd: "+cmd);
      if(cmd == "f"){
        //DBG("Freq");
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
      else if(cmd == "a"){
        std::cin >> val;
        synth->setAmplitude(val);
      }
      else if(cmd == "w"){
        std::cin >> valS;
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