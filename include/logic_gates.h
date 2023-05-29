#ifndef LOGIC_GATES_H
#define LOGIC_GATES_H

#include "network.h"

neural_network_s *train_logic_gates(const char *path, int epochs, double learning_rate, int layers_count,  int *layers_sizes);
void logic_gates_test(neural_network_s *network, int val1, int val2);

#endif
