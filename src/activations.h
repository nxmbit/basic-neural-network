#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

double relu(double x);
double d_relu (double x);
double sigmoid(double x);
double d_sigmoid(double x);
double tanh(double x);
double d_tanh(double x);
double leaky_relu(double x, double slope);
double d_leaky_relu(double x, double slope);
double softmax(double x, double **tab, int size);
double d_softmax(double x, double **tab, int size);
double elu(double x, double alpha);
double d_elu(double x, double alpha);
double swish(double x, double beta);
double d_swish(double x, double beta);

#endif