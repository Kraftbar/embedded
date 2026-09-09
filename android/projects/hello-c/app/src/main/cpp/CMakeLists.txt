cmake_minimum_required(VERSION 3.22.1)

project(helloc)

add_library(helloc SHARED main.c)

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(helloc ${log-lib} ${android-lib})
