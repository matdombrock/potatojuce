#! /bin/bash
TARGET="GPIO_Manager"
clear
g++ $TARGET.cpp -lgpiod -o $TARGET.out
echo "Built: $TARGET.out"