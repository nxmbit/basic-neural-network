#include <math.h>

double relu(double x) {
    return fmax(0, x);
}

double d_relu (double x) {
    return x > 0 ? 1 : 0;
}

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double d_sigmoid(double x) {
    return sigmoid(x) * (1.0 - sigmoid(x));
}

double tanh(double x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

double d_tanh(double x) {
    return 1.0 - pow(tanh(x), 2);
}

double leaky_relu(double x, double slope) {
    return x > 0 ? x : x * slope;
}

double d_leaky_relu(double x, double slope) {
    return x > 0 ? 1 : slope;
}

double softmax(double x, double **tab, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += exp(tab[i][0]);
    }
    return exp(x) / sum;
}

double d_softmax(double x, double **tab, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += exp(tab[i][0]);
    }
    return (sum * exp(x) - exp(x) * exp(x)) / (sum * sum);
}

double elu(double x, double alpha) {
    return x > 0 ? x : alpha * (exp(x) - 1);
}

double d_elu(double x, double alpha) {
    return x > 0 ? 1 : elu(x, alpha) + alpha;
}

double swish(double x, double beta) {
    return x * sigmoid(beta * x);
}

double d_swish(double x, double beta) {
    return swish(x, beta) + sigmoid(beta * x) * (1 - swish(x, beta));
}