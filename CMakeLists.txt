cmake_minimum_required(VERSION 3.22)
project(DMT VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)


if(MSVC)
    add_compile_options(/Qspectre )#/Wall /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif() 

add_definitions(-D_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)