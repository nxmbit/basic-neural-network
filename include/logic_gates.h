#ifndef LOGIC_GATES_H
#define LOGIC_GATES_H

#include "network.h"

#define AND_PATH "../datasets/logic_gates/and.csv"
#define OR_PATH "../datasets/logic_gates/or.csv"
#define XOR_PATH "../datasets/logic_gates/xor.csv"
#define NAND_PATH "../datasets/logic_gates/nand.csv"
#define NOR_PATH "../datasets/logic_gates/nor.csv"

neural_network_s *train_logic_gates(const char *path, int epochs, double learning_rate, int layers_count,  int *layers_sizes);
void logic_gates_test(neural_network_s *network, int val1, int val2);

#endif
