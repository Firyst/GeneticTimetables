#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>

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
    std::string getSubject();
    std::string getTeacher();
    int getAmount();

private slots:
    void deletePushButtonClicked();
    void editPushButtonClicked();

private:
    int days;
    int pairs;
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
