#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>

namespace Ui {
class RuleForm;
}

class RuleForm : public QWidget
{
    Q_OBJECT

public:
    explicit RuleForm(QWidget *parent = nullptr);
    ~RuleForm();

private slots:
    void pushButtonClicked();

private:
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
