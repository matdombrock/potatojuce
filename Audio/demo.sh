#!/bin/bash

for i in {200..1000}
do
  echo "f $i" >> /tmp/pj/in
  echo "$i"
  sleep 0.01
done

echo "Done playing demo."
