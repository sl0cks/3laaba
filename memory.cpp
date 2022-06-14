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
    double** tmp = (double**)malloc(sizeof(double*) * GRAPH_VLAUE);
    if (tmp == NULL) {
        return tmp;
    } else {
        for (size_t i = 0; i < GRAPH_VLAUE; i++) {
            *(tmp + i) = (double*)malloc(sizeof(double) * lines);
            if (*(tmp + i) == NULL) {
                clean2DArrayDouble(tmp, i);
                tmp = NULL;
                break;
            }
        }
    }

    return tmp;
}

void clean2DArrayDouble(double **arr, size_t size) {
    for (size_t i = 0; i < size; i++)
    {
        free(*(arr + i));
    }
    free(arr);
}

double** convertCharToDouble(char*** arr, size_t lines, int numcolumn)
{
    double** tmp = AllocateMatrix(lines);
    for (size_t i = 0; i < lines; i++) {
        tmp[0][i] = atof(arr[i][0]);
        tmp[1][i] = atof(arr[i][numcolumn - 1]);
    }

    return tmp;
}
