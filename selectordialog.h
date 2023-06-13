#ifndef SELECTORDIALOG_H
#define SELECTORDIALOG_H

#include <QDialog>
#include "mycheckbox.h"
#include <vector>
#include <QLabel>

namespace Ui {
class SelectorDialog;
}

class SelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectorDialog(QWidget *parent = nullptr);
    ~SelectorDialog();
    void changeState(int x, int y, bool newState);

private:
    Ui::SelectorDialog *ui;
    std::vector<std::unique_ptr<MyCheckBox>> table;
    std::vector<std::unique_ptr<QLabel>> labelFirst;
    void addCheckBox(int x, int y);
    void addLabel(int x, int y, QString label);
};

#endif // SELECTORDIALOG_H
