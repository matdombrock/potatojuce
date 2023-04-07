#! /bin/bash

IN=/tmp/pgpio-in
OUT=/tmp/pgpio-out

echo "Removing Pipes"
echo $IN
rm $IN || exit 1
echo $OUT
rm $OUT || exit 1

echo "Setting Up Pipes"
echo $IN
mkfifo $IN || exit 1
echo $OUT
mkfifo $OUT || exit 1


