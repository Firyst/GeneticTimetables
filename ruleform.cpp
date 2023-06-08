#include "ruleform.h"
#include "ui_ruleform.h"

RuleForm::RuleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleForm)
{
    ui->setupUi(this);
    connect(ui->pushButtonEdit, SIGNAL(released()), this, SLOT(pushButtonClicked()));
}

RuleForm::~RuleForm()
{
    delete ui;
}

void RuleForm::pushButtonClicked()
{

}

