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

