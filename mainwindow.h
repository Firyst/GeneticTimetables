#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "ruleform.h"
#include <vector>
#include <memory>
#include "parameterwidget.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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
    std::vector<std::unique_ptr<ParameterWidget>> parameters;


private slots:
    void nextButtonClicked();
    void backButtonClicked();
    void addButtonClicked();
    void pageChanged(int index);
    void importClicked();
    void saveConfigurationClicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<RuleForm>> addedRules;

};
#endif // MAINWINDOW_H
