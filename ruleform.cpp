#include "ruleform.h"
#include "ui_ruleform.h"
#include "mainwindow.h"
#include "selectordialog.h"

RuleForm::RuleForm(QWidget *parent, int days, int pairs) :
    QWidget(parent),
    ui(new Ui::RuleForm),
    selected(days+2, std::vector<bool>(pairs+2, false))
{
    this->days = days;
    this->pairs = pairs;
    ui->setupUi(this);
    connect(ui->pushButtonMore, SIGNAL(released()), this, SLOT(editPushButtonClicked()));
    connect(ui->pushButtonDelete, SIGNAL(released()), this, SLOT(deletePushButtonClicked()));

    ui->spinBoxNumberPairs->setMaximum(days*pairs);

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
    SelectorDialog selector(this, days, pairs);
    selector.exec();
}

std::string RuleForm::getSubject()
{
    return ui->lineSubject->text().toStdString();
}

std::string RuleForm::getTeacher()
{
    return ui->lineTeacher->text().toStdString();
}

int RuleForm::getAmount()
{
    return ui->spinBoxNumberPairs->value();
}
