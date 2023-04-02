#! /bin/bash
clear
if [ "$1" == "clean" ]; then
  echo "Cleaning up..."
  rm -fr PotatoJuce/Builds/LinuxMakefile/build/*
else
  echo "No clean argument provided."
fi

cd PotatoJuce/Builds/LinuxMakefile && make
