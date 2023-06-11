#include "selectordialog.h"
#include "ui_selectordialog.h"
#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>

SelectorDialog::SelectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectorDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(4);


    // Устанавливаем виджеты QCheckBox в каждую ячейку таблицы
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        ui->tableWidget->setRowHeight(row, 32);
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            ui->tableWidget->setColumnWidth(col, 32);
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setGeometry(0,0, 32, 32);
            ui->tableWidget->setCellWidget(row, col, checkBox);
        }
    }
}

SelectorDialog::~SelectorDialog()
{
    delete ui;
}

