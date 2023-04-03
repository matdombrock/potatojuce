#! /bin/bash
TARGET="LED_Manager"
clear
g++ $TARGET.cpp -lgpiod -o $TARGET.out