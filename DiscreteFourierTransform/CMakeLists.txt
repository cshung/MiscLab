cmake_minimum_required(VERSION 2.6)
project(DiscreteFourierTransform CXX)
add_definitions(-std=c++11)
set (SRCS
DiscreteFourierTransform.cpp
)
add_executable (DiscreteFourierTransform ${SRCS})

