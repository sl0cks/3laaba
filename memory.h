#ifndef MEMORY_H
#define MEMORY_H

#include "operation.h"
#include "struct.h"

void clean2DArray(char **arr, size_t size); //функция очищения данных с **
void clean3DArray(char ***arr, size_t sizeX, size_t sizeY); //функция очищения данных с ***
double** AllocateMatrix(size_t lines); //функция выделения памяти под матрицу
void clean2DArrayDouble(double **arr, size_t size);
double** convertCharToDouble(char*** arr, size_t lines, int numcolumn);

#endif // MEMORY_H
