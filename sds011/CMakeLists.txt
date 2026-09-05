cmake_minimum_required(VERSION 3.0)
project(sds011_driver)

# Add the source files
set(SOURCES sds011_driver.c)

# Add the executable
add_executable(sds011_driver ${SOURCES})

# Link the executable against the necessary libraries
target_link_libraries(sds011_driver m)
