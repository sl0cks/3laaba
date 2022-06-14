#include "calculation.h"

//функция вычисления максимального числа
double searchMax(double* data, size_t lines) {
    double maximum = data[0];
    for(size_t i = 1; i < lines; i++) {
        if (data[i] > maximum)
            maximum = data[i];
    }

    return maximum;
}

//функция вычисления минимального числа
double searchMin(double* data, size_t lines) {
    double minimum = data[0];
    for(size_t i = 1; i < lines; i++) {
        if (data[i] < minimum)
            minimum = data[i];
    }

    return minimum;
}

//функция вычисления среднего числа
double searchMed(double* data, size_t lines) {
    double medium = 0;
    size_t count = 0;
    for (size_t i = 0; i < lines; i++) {
        medium += data[i];
        count++;
    }

    return medium / count;
}
