#!/bin/bash
g++ stressTest/stressTest.cpp -o ./bin/stress --std=c++11 -pthread
./bin/stress