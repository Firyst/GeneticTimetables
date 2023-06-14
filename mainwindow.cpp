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


    createParameterWidgets( "Select time period", 3, 2, 12, 1, ui->mainLayout, "days");
    createParameterWidgets( "Select the number of pairs per day", 2, 2, 8, 1, ui->mainLayout, "pairs");


    createParameterWidgets( "Conflicts", 20, 2, 9, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Time bounds", 25, 2, 9, 0.2, ui->criteriaLayout);
    createParameterWidgets( "Repeats", 30, 2, 9, 0.4, ui->criteriaLayout);
    createParameterWidgets( "Gaps", 35, 2, 9, 0.1, ui->criteriaLayout);
    createParameterWidgets( "Week balance", 15, 2, 9, 0.4, ui->criteriaLayout);
    createParameterWidgets( "Diversity", 20, 2, 9, 1.0, ui->criteriaLayout);
    createParameterWidgets( "Preferred begin and End time", 50, 2, 9, 2.0, ui->criteriaLayout);

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
    std::ifstream file("data.json");
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

}


void MainWindow::saveConfigurationClicked()
{
    rapidjson::Document data; // создание объекта Document
    rapidjson::Document::AllocatorType& dAlloc = data.GetAllocator();
    data.SetObject(); // задание типа объекта

    // добавление пар ключ-значение в объект
    for (int i=0; i<parameters.size(); i++){
        std::string parName = "Slider_" + std::to_string(i);
        rapidjson::Value jsonKey;
        jsonKey.SetString(parName.c_str(), static_cast<rapidjson::SizeType>(parName.length()), dAlloc);
        data.AddMember(jsonKey, parameters[i].get()->getCurrentValue(), dAlloc);
    }

    rapidjson::Value ruleArray(rapidjson::kArrayType);

    for (int i = 0; i < addedRules.size() ; i++){
        rapidjson::Value ruleObject(rapidjson::kObjectType);
        rapidjson::Value jsonKey1;
        std::string parName1 = (addedRules[i].get()->getSubject());
        jsonKey1.SetString(parName1.c_str(), static_cast<rapidjson::SizeType>(parName1.length()), dAlloc);
        ruleObject.AddMember("Subject", jsonKey1, dAlloc);

        rapidjson::Value jsonKey2;
        std::string parName2 = (addedRules[i].get()->getTeacher());
        jsonKey2.SetString(parName2.c_str(), static_cast<rapidjson::SizeType>(parName2.length()), dAlloc);
        ruleObject.AddMember("Teacher", jsonKey2, dAlloc);

        ruleObject.AddMember("Amount", addedRules[i].get()->getAmount(), dAlloc);

        std::vector<std::vector<bool>> matrix = addedRules[i].get()->selected;
        rapidjson::Value ruleMatrix(rapidjson::kArrayType);

        for (const auto& row : matrix)
        {
            rapidjson::Value jsonRow(rapidjson::kArrayType);
            for (const auto& value : row)
            {
                rapidjson::Value jsonValue(value);
                jsonRow.PushBack(jsonValue, dAlloc);
            }
            ruleMatrix.PushBack(jsonRow, dAlloc);
        }

        ruleObject.AddMember("Selected", ruleMatrix, dAlloc);
        ruleArray.PushBack(ruleObject, dAlloc);
    }
    data.AddMember("rules", ruleArray, dAlloc);



    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить JSON файл", "", "JSON Files (*.json)");




    // Если пользователь выбрал файл
    if (!fileName.isEmpty()) {
        // Создание файла для записи
        QFile file(fileName);

        // Попытка открыть файл в режиме записи
        if (file.open(QIODevice::WriteOnly)) {
            // Преобразование rapidjson::Document в QJsonDocument
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data.GetString());

            // Запись QJsonDocument в файл
            file.write(jsonDoc.toJson());
            file.close();
            qDebug() << "Файл сохранен:" << fileName;
        } else {
            qDebug() << "Ошибка при открытии файла для записи.";
        }
    }

}




