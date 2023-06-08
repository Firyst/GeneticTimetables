#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ruleform.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void periodChanged(int value);
    void numberPairsChanged(int value);
    void nextButtonClicked();
    void backButtonClicked();
    void addButtonClicked();
    void pageChanged(int index);

private:
    Ui::MainWindow *ui;
    std::vector<RuleForm*> addedRules;
};
#endif // MAINWINDOW_H
