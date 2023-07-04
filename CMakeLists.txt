cmake_minimum_required(VERSION 3.25)
project(basic_neural_network C)

set(CMAKE_C_STANDARD 11)

add_executable(basic_neural_network nn.c src/matrixf_s.c src/activations.c src/network.c src/dataset.c src/training.c src/user_io.c src/user_io.h src/mnist.c src/mnist.h src/logic_gates.c src/logic_gates.h src/ui.c src/ui.h src/matrix_s.c src/matrix_s.h src/color_classification.c src/color_classification.h src/custom_dataset.c src/custom_dataset.h)
target_link_libraries(basic_neural_network m)

find_package (Python REQUIRED
        COMPONENTS Interpreter Development)

target_include_directories(basic_neural_network PUBLIC ${Python_INCLUDE_DIRS})
target_link_libraries(basic_neural_network ${Python_LIBRARIES})
target_link_libraries(basic_neural_network ${Python_LIBRARIES})

#openmp
find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    target_link_libraries(basic_neural_network OpenMP::OpenMP_C)
endif()

