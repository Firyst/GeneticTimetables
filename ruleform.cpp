#include "ruleform.h"
#include "ui_ruleform.h"
#include "mainwindow.h"
#include "selectordialog.h"


RuleForm::RuleForm(QWidget *parent, int days, int pairs) :
    QWidget(parent),
    ui(new Ui::RuleForm),
    selected(days+2, std::vector<bool>(pairs+2, false)),
    subjectData(this->id, "Unnamed", "Unnamed", selected)
{
    this->days = days;
    this->pairs = pairs;
    ui->setupUi(this);
    connect(ui->pushButtonMore, SIGNAL(released()), this, SLOT(editPushButtonClicked()));
    connect(ui->pushButtonDelete, SIGNAL(released()), this, SLOT(deletePushButtonClicked()));

    // subjectData = Subject(1, "undefined", "undefined", selected);


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

QString RuleForm::getSubject()
{
    return ui->lineSubject->text();
}

QString RuleForm::getTeacher()
{
    return ui->lineTeacher->text();
}

QString RuleForm::getGroup()
{
    return ui->lineGroup->text();
}


void RuleForm::setValues(QString subject, QString teacher, QString group)
{
    ui->lineSubject->setText(subject);
    ui->lineTeacher->setText(teacher);
    ui->lineGroup->setText(group);
}

Subject* RuleForm::getSubjectData() {
    subjectData.id = id;
    subjectData.name = getSubject().toStdString();
    subjectData.teacher = getTeacher().toStdString();
    subjectData.availableSlots = selected;
    return &subjectData;
}
