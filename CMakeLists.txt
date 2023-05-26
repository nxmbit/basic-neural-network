cmake_minimum_required(VERSION 3.25)
project(basic_neural_network C)

set(CMAKE_C_STANDARD 11)

add_executable(basic_neural_network main.c include/matrixf_s.c include/activations.c include/network.c include/dataset.c include/training.c include/tensorflow_addon.c include/tensorflow_addon.h include/user_io.c include/user_io.h include/mnist.c include/mnist.h)
target_link_libraries(basic_neural_network m)

find_package (Python REQUIRED
        COMPONENTS Interpreter Development)

target_include_directories(basic_neural_network PUBLIC ${Python_INCLUDE_DIRS})
target_link_libraries(basic_neural_network ${Python_LIBRARIES})
target_link_libraries(basic_neural_network ${Python_LIBRARIES})

