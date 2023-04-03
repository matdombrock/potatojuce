#! /bin/bash

LINE_NUMBER=98

echo "Blinking Line $LINE_NUMBER 5 times!";
# Loop 5 times
for i in {1..5}
do
  # Run command A
  gpioset 1 $LINE_NUMBER=0

  # Wait for 1 second
  sleep 1

  # Run command B
  gpioset 1 $LINE_NUMBER=1

  sleep 1
done
