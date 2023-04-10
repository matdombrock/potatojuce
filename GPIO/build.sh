#! /bin/bash
TARGET="test"
clear
g++ $TARGET.cpp -lgpiod -lpthread -std=c++17 -o $TARGET.out || exit 1
echo "Built: $TARGET.out"