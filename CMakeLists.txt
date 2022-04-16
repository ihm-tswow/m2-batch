project(m2-batch)
cmake_minimum_required(VERSION 3.11)

add_executable(m2-batch m2.h m2.cpp main.cpp)
target_compile_features(m2-batch PRIVATE cxx_std_20)