#include "ruleform.h"
#include "ui_ruleform.h"
#include "mainwindow.h"

RuleForm::RuleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleForm)
{
    ui->setupUi(this);
    connect(ui->pushButtonEdit, SIGNAL(released()), this, SLOT(editPushButtonClicked()));
    connect(ui->pushButtonDelete, SIGNAL(released()), this, SLOT(deletePushButtonClicked()));
}

RuleForm::~RuleForm()
{
    delete ui;
}

void RuleForm::deletePushButtonClicked()
{
    parentWindow->removeRule(id);
}


void RuleForm::editPushButtonClicked()
{

}


