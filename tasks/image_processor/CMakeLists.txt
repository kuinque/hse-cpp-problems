cmake_minimum_required(VERSION 3.8)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)

add_executable(image_processor image_processor.cpp matrix.hpp bmp_image.hpp input_controller.hpp)
# add_executable(image_processor bmp_image.hpp) 
