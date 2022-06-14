#ifndef LOGIC_H
#define LOGIC_H

#include "operation.h"
#include "memory.h"
#include "struct.h"

enum FunctionType {
    getDataFromFile, //чтение данных из файла csv
    calculationData, //вычисление данных таблицы
    cleanData //очищение данных
};

FuncReturningValue* entryPoint(FunctionType func, FunctionArgument* funcA); //единая точка входа

#endif // LOGIC_H
