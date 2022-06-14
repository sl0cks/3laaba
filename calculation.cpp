#include "calculation.h"

//функция вычисления максимального числа
double searchMax(char*** data, size_t lines, int numcolumn) {
    double maximum = atof(data[0][numcolumn - 1]);
    for(size_t i = 1; i < lines; i++) {
        const char *number = data[i][numcolumn - 1];
        if (*number) {
            double value = atof(number);
            if (value > maximum)
                maximum = value;
        }
    }

    return maximum;
}

//функция вычисления минимального числа
double searchMin(char*** data, size_t lines, int numcolumn) {
    double minimum = atof(data[0][numcolumn - 1]);
    for(size_t i = 1; i < lines; i++) {
        const char *number = data[i][numcolumn - 1];
        if (*number) {
            double value = atof(number);
            if (value < minimum)
                minimum = value;
        }
    }

    return minimum;
}

//функция вычисления среднего числа
double searchMed(char*** data, size_t lines, int numcolumn) {
    double medium = 0;
    size_t count = 0;
    for (size_t i = 0; i < lines; i++) {
        const char *number = data[i][numcolumn - 1];
        if (*number) {
            medium += atof(number);
            count++;
        }
    }

    return medium / count;
}

//функция для запоминания значений для графика
double **serchForGraphic(char*** data, size_t lines, int numcolumn) {
    double** tmpgraph = AllocateMatrix(lines);
    for (size_t i = 0; i < lines; i++) {
        tmpgraph[i][0] = atof(data[i][0]);
        tmpgraph[i][1] = atof(data[i][numcolumn - 1]);
    }

   return tmpgraph;
}
