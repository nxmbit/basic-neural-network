#ifndef CUSTOM_DATASET_H
#define CUSTOM_DATASET_H

void custom_dataset_menu(int model_loaded, neural_network_s *model);
neural_network_s *train_custom_dataset(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes, int samples_count);
void test_custom_dataset(neural_network_s *network);

#endif
