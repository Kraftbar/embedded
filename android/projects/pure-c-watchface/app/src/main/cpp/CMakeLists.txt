cmake_minimum_required(VERSION 3.22.1)

project(purecwatchface)

add_library(purecwatchface SHARED render.c)

find_library(log-lib log)
find_library(jnigraphics-lib jnigraphics)

target_link_libraries(purecwatchface ${log-lib} ${jnigraphics-lib} m)
