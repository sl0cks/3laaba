 #include "logic.h"

FuncReturningValue* getDataFromFileDo(const char* filename, const char* regiononename); //чтение данных из файла
FuncReturningValue* calculationDataDo(char*** data, size_t lines, int numcolumn); //функция вычисления макс, мин, мед
void clean_All(FunctionArgument *arg); //функция очищения данных

//единая точка входа
FuncReturningValue* entryPoint(FunctionType func, FunctionArgument* funcA) {
    FuncReturningValue *result = NULL;
    switch(func)
    {
        case getDataFromFile:
            result = getDataFromFileDo(funcA->filename, funcA->regionname);
            break;
        case calculationData:
            result = calculationDataDo(funcA->data, funcA->lines, funcA->numcolumns);
            break;
        case cleanData:
            clean_All(funcA);
            break;
        default:
            break;
    }

    return result;
}

//чтение данных из файла
FuncReturningValue* getDataFromFileDo(const char* filename, const char* regiononename) {
    FuncReturningValue* smt = (FuncReturningValue*)calloc(1, sizeof(FuncReturningValue)); //структура, куда считываются данные
    FILE* file = fopen(filename, "r");
    if(file != NULL || smt != NULL){
        size_t lines, box; //количество строк и колонок
        char **crudeData = readFromFile(file, &lines); //массив строк
        char ***data = (char***)malloc(sizeof(char**) * (lines - 1)); //массив строк, поделённых на слова
        if (data == NULL || crudeData == NULL)
            smt->is_file_error = true;
        for(size_t i = 0; i < lines - 1; i++) {
           *(data + i) = strSplit(*(crudeData + (i + 1)), &box, ',');
           if (*(data + i) == NULL)
               smt->is_file_error = true;
           free(*(crudeData + (i + 1)));
        }
        lines--;
        char **table_headers = strSplit(*(crudeData + HEADER), &box, ',');
        if (table_headers == NULL)
            smt->is_file_error = true;
        free(*(crudeData + HEADER));
        free(crudeData);

        smt->lines = lines;
        smt->columns = box;
        smt->table_headers = table_headers;
        smt->data = data;

        smt = sortingByRegion(smt, regiononename);

        fclose(file);
    } else
        smt->is_file_error = true;

    return smt;
}

//функция вычисления
FuncReturningValue* calculationDataDo(char*** data, size_t lines, int numcolumn) {
    FuncReturningValue* smt = (FuncReturningValue*)malloc(sizeof(FuncReturningValue)); //структура, куда считываются данные
    if (smt != NULL) {
        smt->calculation_res[NUM_MAX] = searchMax(data, lines, numcolumn);
        smt->calculation_res[NUM_MIN] = searchMin(data, lines, numcolumn);
        smt->calculation_res[NUM_MED] = searchMed(data, lines, numcolumn);
        smt->graphic = serchForGraphic(data, lines, numcolumn);
        smt->lines = lines;
        smt->numcolumns = numcolumn;
    }

    return smt;
}

//функция очищения всей структуры
void clean_All(FunctionArgument *arg) {
    if(arg->filename != NULL) {
        free(arg->filename);
    } else if(arg->regionname != NULL) {
        free(arg->regionname);
    } else if(arg->data != NULL) {
        clean3DArray(arg->data, arg->lines, arg->columns);
    } else if(arg->table_headers != NULL) {
        clean2DArray(arg->table_headers, arg->columns);
    }
}
