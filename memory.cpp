#include "memory.h"

//функция очищения char**
void clean2DArray(char **arr, size_t size) {
    for (size_t i = 0; i < size; i++)
    {
        free(*(arr + i));
    }
    free(arr);
}

//функция очищения char***
void clean3DArray(char ***arr, size_t sizeX, size_t sizeY) {
    for (size_t i = 0; i < sizeX; i++)
    {
        clean2DArray(*(arr + i), sizeY);
    }
    free(arr);
}

//функция выделения памяти под матрицу
double** AllocateMatrix(size_t lines) {
    double** tmp = (double**)malloc(sizeof(double*) * lines);
    if (tmp == NULL) {
        printf("error");
    } else {
        for (size_t i = 0; i < lines; i++) {
            *(tmp + i) = (double*)malloc(sizeof(double) * GRAPH_VLAUE);
            if (*(tmp + i) == NULL) {
                free(*(tmp + i));
            }
        }
    }

    return tmp;
}
