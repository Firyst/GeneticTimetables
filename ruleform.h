#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>
#include "GeneticCore/timetable/timetable.h"

class MainWindow;

namespace Ui {
class RuleForm;
}

class RuleForm : public QWidget
{
    Q_OBJECT

public:
    explicit RuleForm(QWidget *parent = nullptr, int days=2, int pairs=2);
    MainWindow* parentWindow = nullptr;
    int id;
    ~RuleForm();

    std::vector<std::vector<bool>> selected;
    QString getSubject();
    QString getTeacher();
    int getAmount();
    void setValues(QString subject, QString teacher, int amount);
    Subject* getSubjectData();

private slots:
    void deletePushButtonClicked();
    void editPushButtonClicked();

private:
    int days;
    int pairs;
    Subject subjectData;
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
