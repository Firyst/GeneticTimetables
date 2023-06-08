#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    qDebug() << "add";
    RuleForm newForm = RuleForm();
    qDebug() << "init";
    ui->scrollArea->layout()->addWidget(&newForm);
    //addedRules.push_back(&newForm);
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
