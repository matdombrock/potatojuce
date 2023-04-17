# POTATO JUCE:

## Setup:

Before you can work with this project you will need to install the required dependencies:
```
./pi-deps.sh
```
[Source](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md)

# Le Potato

The “Le Potato” single board computer available from Libre Computers is an alternative to the Raspberry Pi. It is modeled after the Raspberry Pi 3 and is largely compatible with the Raspberry Pi 3 and it’s accessories (with some limitations/caveats). It can even run Raspberry Pi OS / Raspbian.

It costs about 1/3rd of what a Raspberry Pi does while coming with 2GB of memory. It’s also widely available as well (unlike the pi).

[Internal Le Potato Docs](.docs/le-potato.md)

## Juce Lib Included?
This is because its not easy to get Projucer running on ARM and its easier to do it this way than to to depend on Projucer to setup the project correctly. 

## Project Layout
Generally speaking, the actual project code is split into two main directories/sub-projects. 

The `Audio` directory holds all of the audio (JUCE) related code. 

The `GPIO` directory holds all of the hardware interface code. 

These really are two separate projects that are treated as their own code bases. They are just kept in the same repo since they are so closely tied together at this point. 

At this point, these 2 projects do not share any code and they might be split into their own repos in the future if it remains that way. 


## Building Audio
```
cd Audio
./build.sh
```

*Note: If you have issues run the command as `.build.sh clean` to clean your build directory first.*

## Running Audio
```
cd Audio
./run.sh
```

## Building HW Interface
```
cd GPIO
./build.sh
```

## Running HW Interface
```
cd GPIO
./run.sh
```

## Inter-Process Communication
IPC is handled through a named pipe at `/tmp/pinp`. 

This pipe must be created manually in order for the IPC to start working. 

To run the synth code with "IPC enabled" do:
```
cd Audio
cat /tmp/pinp | ./run.sh
```

Then start the `GPIO/interface.out` software. 
