#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "ruleform.h"
#include <vector>
#include <memory>
#include "parameterwidget.h"

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
    void createParameterWidgets(QString name, int value, int start, int end, float step, QVBoxLayout* target, QString suffix="x");

private slots:
    void nextButtonClicked();
    void backButtonClicked();
    void addButtonClicked();
    void pageChanged(int index);

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<RuleForm>> addedRules;
    std::vector<std::unique_ptr<ParameterWidget>> parameters;

};
#endif // MAINWINDOW_H