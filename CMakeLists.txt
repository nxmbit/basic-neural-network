cmake_minimum_required(VERSION 3.25)
project(basic_neural_network C)

set(CMAKE_C_STANDARD 11)

add_executable(basic_neural_network main.c include/matrixf_s.c)
target_link_libraries(basic_neural_network m)
