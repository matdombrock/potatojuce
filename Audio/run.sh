#! /bin/bash
rm /tmp/pinp
mkfifo /tmp/pinp || exit 1

cat /tmp/pinp | ./Builds/LinuxMakefile/build/PotatoJuce
