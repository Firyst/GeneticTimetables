#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDebug>
#include <memory>
#include <vector>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <iostream>
#include <fstream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

    connect(ui->pushButtonNext, SIGNAL(released()), this, SLOT(nextButtonClicked()));
    connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonClicked()));
    connect(ui->pushButtonAdd, SIGNAL(released()), this, SLOT(addButtonClicked()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));
    connect(ui->importButton, SIGNAL(released()), this, SLOT(importClicked()));
    connect(ui->saveConfigurationButton, SIGNAL(released()), this, SLOT(saveConfigurationClicked()));
    pageChanged(0);


    // create all sliders
    createParameterWidgets( "Select time period", 3, 2, 12, 1, ui->mainLayout, " days");
    createParameterWidgets( "Select the number of pairs per day", 2, 2, 8, 1, ui->mainLayout, " pairs");


    createParameterWidgets( "Conflicts", 25, 10, 100, 1, ui->criteriaLayout);
    createParameterWidgets( "Time bounds", 20, 0, 10, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Repeats", 10, -5, 5, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Gaps", 20, 0, 10, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Week balance", 10, 0, 10, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Diversity", 10, -5, 5, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Preferred begin and End time", 10, 0, 10, 0.1, ui->criteriaLayout);

    createParameterWidgets( "Population size", 20, 50, 2500, 50, ui->layoutParameters);
    createParameterWidgets( "Crossover chance", 35, 0, 100, 1, ui->layoutParameters, "%");
    createParameterWidgets( "Mutation chance", 24, 0, 100, 1, ui->layoutParameters, "%");
    createParameterWidgets( "Mutation threshold", 10, 0, 1, 0.005, ui->layoutParameters);

    ui->mainLayout->setAlignment(Qt::AlignTop);
    ui->criteriaLayout->setAlignment(Qt::AlignTop);
    ui->layoutParameters->setAlignment(Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextButtonClicked()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() + 1);
}

void MainWindow::backButtonClicked()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() - 1);
}

void MainWindow::addButtonClicked()
{
    ui->scrollArea->hide();

    addedRules.push_back(std::make_unique<RuleForm>(nullptr, parameters[0].get()->getCurrentValue(), parameters[1].get()->getCurrentValue()));
    ui->scrollAreaWidgetContents->layout()->addWidget(addedRules[addedRules.size() - 1].get());
    addedRules[addedRules.size() - 1].get()->show();
    addedRules[addedRules.size() - 1].get()->id = addedRules.size() - 1;
    addedRules[addedRules.size() - 1].get()->parentWindow = this;
    ui->scrollArea->show();
}

void MainWindow::removeRule(int objectIndex) {
    addedRules.erase(addedRules.begin() + objectIndex);
    for (int currentIndex{objectIndex}; currentIndex < this->addedRules.size(); currentIndex++) {
        addedRules[currentIndex].get()->id -= 1;
    }
}

void MainWindow::pageChanged(int index)
{
    if (index==4){
        ui->pushButtonNext->hide();
    }else{
        ui->pushButtonNext->show();
    }

    if (index<=1){
        ui->pushButtonBack->hide();
    }else{
        ui->pushButtonBack->show();
    }
}

void MainWindow::createParameterWidgets(QString name, int value, int start, int end, float step, QVBoxLayout* target, QString suffix){
    std::unique_ptr<ParameterWidget> testWidget = std::make_unique<ParameterWidget>(this, name, suffix, value, start, end, step);
    target->addWidget(testWidget.get());
    testWidget.get()->show();
    parameters.push_back(std::move(testWidget));
}

void MainWindow::importClicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON File", "", "JSON Files (*.json)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (!jsonDoc.isNull()) {
                QJsonObject data = jsonDoc.object();

                for (int i=0; i<parameters.size(); i++){
                    parameters[i].get()->setCurrentValue(data.value("Slider_" + QString::number(i)).toDouble());
                }

                for (const QJsonValue& value : data.value("rules").toArray()) {
                    QJsonObject object = value.toObject();
                    addedRules.push_back(std::make_unique<RuleForm>(nullptr, parameters[0].get()->getCurrentValue(), parameters[1].get()->getCurrentValue()));
                    RuleForm* addedRule = addedRules[addedRules.size() - 1].get();
                    ui->scrollAreaWidgetContents->layout()->addWidget(addedRule);
                    addedRule->show();
                    addedRule->id = addedRules.size() - 1;
                    addedRule->parentWindow = this;
                    addedRule->setValues(object.value("Subject").toString(), object.value("Teacher").toString(), object.value("Amount").toInt());

                    QJsonArray matrix = object.value("Selected").toArray();
                    int j=0;
                    for (const QJsonValue& rowValue : matrix){
                        int i=0;
                        QJsonObject object = value.toObject();
                        for (const QJsonValue& cell : rowValue.toArray()){
                            addedRule->selected[j][i]=cell.toBool();
                            i++;
                        }
                        j++;
                    }
                }

            } else {
                qDebug() << "Failed to parse JSON document.";
            }
        } else {
            qDebug() << "Failed to open file:" << file.errorString();
        }
    }

}


void MainWindow::saveConfigurationClicked()
{
    //QJsonDocument data; // создание объекта Document
    QJsonObject data;

    // добавление пар ключ-значение в объект
    for (int i=0; i<parameters.size(); i++){
        data.insert("Slider_" + QString::number(i), parameters[i].get()->getCurrentValue());
    }

    QJsonArray ruleArray;

    for (int i = 0; i < addedRules.size() ; i++){
        QJsonObject ruleObject;
        ruleObject.insert("Subject", addedRules[i].get()->getSubject());
        ruleObject.insert("Teacher", addedRules[i].get()->getTeacher());
        ruleObject.insert("Amount", addedRules[i].get()->getAmount());

        std::vector<std::vector<bool>> matrix = addedRules[i].get()->selected;
        QJsonArray ruleMatrix;

        for (const auto& row : matrix)
        {
            QJsonArray jsonRow;
            for (const auto& value : row)
            {
                jsonRow.append(value);
            }
            ruleMatrix.append(jsonRow);
        }

        ruleObject.insert("Selected", ruleMatrix);
        ruleArray.append(ruleObject);
    }
    data.insert("rules", ruleArray);

    QJsonDocument result(data);

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить JSON файл", "", "JSON Files (*.json)");
    // Если пользователь выбрал файл
    if (!fileName.isEmpty()) {
        // Создание файла для записи
        QFile file(fileName);

        // Попытка открыть файл в режиме записи
        if (file.open(QIODevice::WriteOnly)) {
            // Запись QJsonDocument в файл
            file.write(result.toJson());
            file.close();
            qDebug() << "Файл сохранен:" << fileName;
        } else {
            qDebug() << "Ошибка при открытии файла для записи.";
        }
    }

}




