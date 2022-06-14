#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//функия при нажатии на открыть файл
void MainWindow::on_pushButton_namefile_clicked() {
    ///Users/ivankarpov/Desktop
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "CSV File (*.csv)"); //открывает каталог файлов
    ui->namefile->setText(FileName);
    if(FileName.isEmpty()) { //isEmpty() проверяет, является ли QString пустым
        ui->error->setText("Файл не выбран!");
        return;
    }
}

//функция при нажатии загрузить данные
void MainWindow::on_pushButton_loaddata_clicked() {
    QString FileName = ui->namefile->text();
    QString RegName = ui->nameregion->text();
    if (FileName.isEmpty()) { //isEmpty() проверяет, является ли QString пустым
        ui->error->setText("Файл не выбран!");
        return;
    } else if (RegName.isEmpty()) { //isEmpty() проверяет, является ли QString пустым
        ui->error->setText("Не введено название региона!");
        return;
    } else {
        ui->error->setText("");
        FunctionArgument struct_for_output = {
            .filename = QStringToCharLine(FileName),
            .regionname = QStringToCharLine(RegName)
        };
        FuncReturningValue* for_output = entryPoint(getDataFromFile, &struct_for_output);
        if(for_output->lines == 0)
            ui->error->setText("Данный регион не найден!");
        showData(for_output);
        entryPoint(cleanData, &struct_for_output);
        free(for_output);
    }
}

//функция при нажатии вычисления
void MainWindow::on_pushButton_calculations_clicked() {
    ui->error->setText("");
    QString FileName = ui->namefile->text();
    QString RegName = ui->nameregion->text();
    if(FileName.isEmpty() || RegName.isEmpty()) { //isEmpty() проверяет, является ли QString пустым
        ui->error->setText("Вычисления невозможны!");
    } else {
        ui->error->setText("");
        FunctionArgument struct_for_output = {
            .numcolumns = ui->numcolomn->value(), //value() - это свойство содержит значение поля
            .table_headers = getHeaderFromTable(),
            .data = getDataFromTable(),
            .lines = (size_t)ui->tableWidget->rowCount(), //rowCount() - возвращает количество строк.
            .columns = (size_t)ui->tableWidget->columnCount() //columnCount() - возвращает количество столбцов.
        };
        if ((size_t)struct_for_output.numcolumns > struct_for_output.columns)
            ui->error->setText("Вычисления недопустимы!");
        else {
            FuncReturningValue* tmp = entryPoint(calculationData, &struct_for_output);
            tmp->table_headers = struct_for_output.table_headers;
            ui->maximum->setText(QString::number(tmp->calculation_res[NUM_MAX]));
            ui->minimum->setText(QString::number(tmp->calculation_res[NUM_MIN]));
            ui->median->setText(QString::number(tmp->calculation_res[NUM_MED]));
            showGraphic(tmp->graphic, tmp->table_headers, tmp->lines, tmp->calculation_res, tmp->numcolumns);
            entryPoint(cleanData, &struct_for_output);
            free(tmp);
        }
    }
}

//функция выведения данных в таблицу
void MainWindow::showData(FuncReturningValue* for_output) {
    if (for_output->is_file_error) {
        ui->error->setText("Ошибка чтения");
        return;
    }

    ui->tableWidget->setColumnCount(for_output->columns); //устанавливает количество колонок в модели этой таблицы наcolumns. Если это меньше, чем columnCount (), данные в нежелательных столбцах отбрасываются.
    QStringList QColumns = ConvertRowToQTFormat(for_output->table_headers, for_output->columns);
    ui->tableWidget->setHorizontalHeaderLabels(QColumns); //устанавливает горизонтальные метки заголовков с помощью labels.

    if (for_output->data != NULL) {
        ui->tableWidget->setRowCount(for_output->lines); //устанавливает количество строк в модели этой таблицы на rows. Если оно меньше rowCount (), данные в нежелательных строках отбрасываются.
        for(size_t i = 0; i < for_output->lines; i++) {
            QStringList currentSeries = ConvertRowToQTFormat(*((for_output)->data + i), for_output->columns);
            for(int j = 0; j < currentSeries.count(); j++) {
                QTableWidgetItem *Item = new QTableWidgetItem(); //класс QTableWidgetItem предоставляет элемент для использования с классом QTableWidget
                                                                //создает элемент таблицы с заданным значком и текстом.
                Item->setData(Qt::EditRole, currentSeries.at(j));  //устанавливает данные элемента для данной роли в указанное значение.
                Item->setText(currentSeries.at(j)); //устанавливает текст элемента в соответствии с указанным текстом.
                ui->tableWidget->setItem(i, j, Item); //устанавливает элемент для данной строки и столбца в значение item.
            }
        }
    }
}

//функция получения данных из таблицы
char*** MainWindow::getDataFromTable() {
    char*** data = (char***)malloc(sizeof(char**) * ui->tableWidget->rowCount());
    for(size_t i = 0; i < (size_t)ui->tableWidget->rowCount(); i++) {
        *(data + i) = (char**)malloc(sizeof(char*) * ui->tableWidget->columnCount());
        for(size_t j = 0; j < (size_t)ui->tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget->item(i,j); //item() - возвращает элемент для данной строки и столбца, если таковой был задан; в противном случае возвращает nullptr.
            char *str = QStringToCharLine(item->text());
            data[i][j] = str;
        }
    }

    return data;
}

//функция получения заголовков из таблицы
char** MainWindow::getHeaderFromTable() {
    char** header = (char**)malloc(sizeof(char*) * ui->tableWidget->columnCount());
    for(size_t i = 0; i < (size_t)ui->tableWidget->columnCount(); i++) {
        QTableWidgetItem *item = ui->tableWidget->horizontalHeaderItem(i);
        char *str = QStringToCharLine(item->text());
        header[i] = str;
    }

    return header;
}

//функция перевода QString в char
char* QStringToCharLine(QString FileName) {
    char* new_str = (char*)malloc(sizeof(char) * (FileName.size() + 1));
    if(new_str != NULL){
        for(qsizetype i = 0; i < FileName.size(); i++) { //size() количество символов в строке
            *(new_str + i) = FileName.at(i).unicode(); //at() - возвращает символ по указанному индексу position в строке.
            //unicode() - возвращает нультерминальное Unicode-представление строки. Результат остаётся допустимым, пока строка не изменена.
        }
        *(new_str + FileName.size()) = 0;
    }

    return new_str;
}

//функция перевода массива строк в формат QT
QStringList ConvertRowToQTFormat(char** row, size_t size) {
    QStringList nstr;
    for(size_t i = 0; i < size; i++) {
        nstr.append(*(row + i)); //append() - добавление к строке справа строки.
    }

    return nstr;
}

//функция выведения графика
void MainWindow::showGraphic(double** dGr, char** header, size_t lines, double* metrics, int numcolumn) {
    QPixmap *pix = new QPixmap(600, 600);
    QPainter paint(pix);
    basisForGraph(paint);
    drawText(paint, header, numcolumn);
    drawGraphic(paint, dGr, lines, metrics);
    ui->graphic->setPixmap(*pix);
}

//функция для создания основания для рисования и осей
void basisForGraph(QPainter &paint) {
    paint.fillRect(0, 0, 600, 600, QBrush(QColor(Qt::GlobalColor::white)));
    paint.translate(10,300); //translate() - функция для перемещения центра координат
    paint.setPen(*(new QColor(255, 34, 255, 255)));
    paint.drawRect(-10, -300, 600, 600);
    paint.setPen(QColor(Qt::GlobalColor::red));
    //оси
    paint.drawLine( 10, 290, 10, -290);
    paint.drawLine(-5, 0, 580, 0);
    //стрелочки к осям
    paint.drawLine(5, -280, 10, -295);
    paint.drawLine(10, -295, 15, -280);
    paint.drawLine(570, -5, 585, 0);
    paint.drawLine(585, 0, 570, 5);
    //начало координат
    paint.drawText(1, 12, "0");
}

//функция для рисования текста графика
void drawText(QPainter &paint, char** header, int numcolumn) {
    QString  result1(header[0]);
    paint.drawText(520, 10, result1);
    QString  result2(header[numcolumn - 1]);
    paint.save();
    paint.translate(10,300);
    paint.rotate(-90);
    paint.drawText(470,-17,100,60,Qt::AlignHCenter,result2);
    paint.restore();
}

//функция для рисования графика
void drawGraphic(QPainter &paint, double** dGr, size_t lines, double* metrics) {
    if (abs(dGr[0][1]) < 10) {
        drawLine(paint, dGr, lines, 30, metrics);
    } else if ((abs(dGr[0][1]) >= 10) && (abs(dGr[0][1]) <= 50)) {
        drawLine(paint, dGr, lines, 10, metrics);
    } else if ((abs(dGr[0][1]) > 50) && (abs(dGr[0][1]) <= 80)) {
        drawLine(paint, dGr, lines, 3, metrics);
    } else if (abs(dGr[0][1]) > 80 ) {
        drawLine(paint, dGr, lines, 2, metrics);
    }
}

//функция рисования линий графика
void drawLine(QPainter &paint, double** dGr, size_t lines, int n, double* metrics) {
    int count_max = 0, count_min = 0;
    for (size_t i = 0; i < lines - 1; i++) {
        paint.drawLine((int)((dGr[i][0] - YEARS) * RATIO), (int)((-n)*dGr[i][1]), (int)((dGr[i+1][0] - YEARS) * RATIO), (int)((-n)*dGr[i+1][1]));
        if (dGr[i][1] == metrics[NUM_MIN] && count_min == 0) {
            paint.drawText((int)((dGr[i][0] - YEARS) * RATIO), (int)((-n)*dGr[i][1]), "MIN");
            paint.drawText((int)((dGr[i][0] - YEARS) * RATIO), 0, QString::number(dGr[i][0]));
            paint.drawText(-3, (int)((-n)*dGr[i][1]), QString::number(dGr[i][1]));
            count_min += 1;
        } else if (dGr[i][1] == metrics[NUM_MAX] && count_max == 0) {
            paint.drawText((int)((dGr[i][0] - YEARS) * RATIO), (int)((-n)*dGr[i][1]), "MAX");
            paint.drawText((int)((dGr[i][0] - YEARS) * RATIO), 0, QString::number(dGr[i][0]));
            paint.drawText(-3, (int)((-n)*dGr[i][1]), QString::number(dGr[i][1]));
            count_max += 1;
        }
        paint.drawLine(0, (int)((-n)*metrics[NUM_MED]), 580, (int)((-n)*metrics[NUM_MED]));
        paint.drawText(560, (int)((-n)*metrics[NUM_MED]), "MED");
    }
}
