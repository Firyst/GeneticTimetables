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
    explicit RuleForm(QWidget *parent = nullptr);
    MainWindow* parentWindow = nullptr;
    int id;
    ~RuleForm();

private slots:
    void deletePushButtonClicked();
    void editPushButtonClicked();

private:
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
