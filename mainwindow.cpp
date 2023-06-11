#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

    connect(ui->sliderPeriod, SIGNAL(valueChanged(int)), this, SLOT(periodChanged(int)));
    connect(ui->sliderPairs, SIGNAL(valueChanged(int)), this, SLOT(numberPairsChanged(int)));
    connect(ui->pushButtonNext, SIGNAL(released()), this, SLOT(nextButtonClicked()));
    connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonClicked()));
    connect(ui->pushButtonAdd, SIGNAL(released()), this, SLOT(addButtonClicked()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::periodChanged(int value)
{
    ui->labelPeriod->setText(QString::number(value) + "days");
}

void MainWindow::numberPairsChanged(int value)
{
    ui->labelPairs->setText(QString::number(value) + " pairs");
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
    qDebug() << "add";
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


