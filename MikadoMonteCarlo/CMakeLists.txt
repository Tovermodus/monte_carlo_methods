cmake_minimum_required(VERSION 3.16)
project(MikadoMonteCarlo)

set(CMAKE_CXX_STANDARD 14)

add_executable(MikadoMonteCarlo main.cpp Rod.cpp Rod.h Medium.cpp Medium.h MediumParameters.h)