cmake_minimum_required(VERSION 3.21)
project(EconomicsMicrofoundations)

set(CMAKE_CXX_STANDARD 20)

add_executable(EconomicsMicrofoundations src/main.cpp src/Agent.cpp src/Simulation.cpp src/MutableCategoricalArray.cpp src/Random.cpp)
