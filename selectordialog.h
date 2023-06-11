#ifndef SELECTORDIALOG_H
#define SELECTORDIALOG_H

#include <QDialog>

namespace Ui {
class SelectorDialog;
}

class SelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectorDialog(QWidget *parent = nullptr);
    ~SelectorDialog();

private:
    Ui::SelectorDialog *ui;
};

#endif // SELECTORDIALOG_H
