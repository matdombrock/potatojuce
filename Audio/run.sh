#! /bin/bash
rm /tmp/pj/in
mkdir /tmp/pj
mkfifo /tmp/pj/in || exit 1

cat /tmp/pj/in | ./Builds/LinuxMakefile/build/PotatoJuce
