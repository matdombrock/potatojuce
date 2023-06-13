#!/bin/bash

for i in {200..410}
do
  v=$(($i*10))
  echo -e "f $v\n" >>/tmp/pj/in &
  echo "$v"
  sleep 1
done

echo "Done playing demo."
