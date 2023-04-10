#! /bin/bash
TARGET="GPIO_IPC_Manager"
clear
g++ $TARGET.cpp -lgpiod -lpthread -std=c++17 -o $TARGET.out || exit 1
echo "Built: $TARGET.out"