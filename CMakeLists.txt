cmake_minimum_required(VERSION 3.22.1)
project(badhon_game_profile)

add_library(badhongp SHARED main.cpp)

find_library(log-lib log)
target_link_libraries(badhongp ${log-lib})

target_compile_features(badhongp PRIVATE cxx_std_17)
