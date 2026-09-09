cmake_minimum_required(VERSION 3.22.1)

project(hellocpure)

add_library(hellocpure SHARED main.c)

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(hellocpure ${log-lib} ${android-lib})
