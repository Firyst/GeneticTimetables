#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ruleform.h"
#include <vector>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void removeRule(int objectIndex);

private slots:
    void periodChanged(int value);
    void numberPairsChanged(int value);
    void nextButtonClicked();
    void backButtonClicked();
    void addButtonClicked();
    void pageChanged(int index);


private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<RuleForm>> addedRules;
};
#endif // MAINWINDOW_H
