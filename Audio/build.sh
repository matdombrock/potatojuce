#! /bin/bash
clear
if [ "$1" == "clean" ]; then
  echo "Cleaning up..."
  rm -fr Builds/LinuxMakefile/build/*
else
  echo "No clean argument provided."
fi

Builds/LinuxMakefile && make
