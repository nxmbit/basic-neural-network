#ifndef UI_H
#define UI_H

void main_menu();
void dataset_menu();
void load_model_menu();
void logic_gates_menu();
void logic_gates_training_menu(const char *def_path, int def_epochs, double def_learning_rate, int def_number_of_layers, int *def_layers_sizes, const char *gate_name);
void mnist_menu();
void color_classification_menu();

#endif
