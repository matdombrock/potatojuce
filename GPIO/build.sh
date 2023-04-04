#! /bin/bash
TARGET="GPIO_Manager"
clear
g++ $TARGET.cpp -lgpiod -std=c++17 -o $TARGET.out
echo "Built: $TARGET.out"