cmake_minimum_required(VERSION 3.21)
project(EconomicsMicrofoundations)

set(CMAKE_CXX_STANDARD 20)

link_libraries(boost_system boost_iostreams) # boost_filesystem boost_serialization)

add_executable(EconomicsMicrofoundations src/main.cpp src/Person.cpp src/Simulation.cpp src/Company.cpp src/mystd/Random.cpp)
