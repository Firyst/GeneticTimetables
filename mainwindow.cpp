#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDebug>
#include <memory>
#include <vector>

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
    pageChanged(0);


    createParameterWidgets( "select time period", 3, 2, 12, 1, ui->mainLayout, "days");
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
    addedRules.push_back(std::make_unique<RuleForm>());
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

    if (index==0){
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


