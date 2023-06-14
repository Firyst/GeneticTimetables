#ifndef SELECTORDIALOG_H
#define SELECTORDIALOG_H

#include <QDialog>
#include "mycheckbox.h"
#include <vector>
#include <QLabel>
#include "ruleform.h"

namespace Ui {
class SelectorDialog;
}

class SelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectorDialog(RuleForm *parent = nullptr, int days=2, int pairs=2);
    ~SelectorDialog();
    void changeState(int x, int y, bool newState);

    std::vector<std::vector<bool>> selected;

private:
    Ui::SelectorDialog *ui;
    RuleForm* parentForm{nullptr};
    std::vector<std::unique_ptr<MyCheckBox>> table;
    std::vector<std::unique_ptr<QLabel>> labelFirst;
    void addCheckBox(int x, int y);
    void addLabel(int x, int y, QString label);
};

#endif // SELECTORDIALOG_H
