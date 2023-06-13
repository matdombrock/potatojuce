#! /bin/bash
rm /tmp/pj/in
mkfifo /tmp/pj/in || exit 1

cat /tmp/pj/in | ./Builds/LinuxMakefile/build/PotatoJuce
