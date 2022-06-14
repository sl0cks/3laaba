#ifndef CALCULATION_H
#define CALCULATION_H

#include "struct.h"
#include "memory.h"

double searchMax(char*** data, size_t lines, int numcolumn); //функция поиска максимума
double searchMin(char*** data, size_t lines, int numcolumn); //функция поиска минимума
double searchMed(char*** data, size_t lines, int numcolumn); //функция поиска среднего
double **serchForGraphic(char*** data, size_t lines, int numcolumn); //функция для запоминания значений для графика

#endif // CALCULATION_H
