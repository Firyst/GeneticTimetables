#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QVBoxLayout>
#include "ruleform.h"
#include <vector>
#include <memory>
#include "parameterwidget.h"
#include "GeneticCore/ga/ga.h"
#include <QThread>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class GAThread : public QThread {
    Q_OBJECT

public:
    Population population = Population(0, 0, 0, {0}, {1.0});
    long iterations{0};

    void run(void)
    {
        // Код, выполняемый в отдельном потоке
    }

signals:
    void progressSignal(int );
    void finishedSignal(void );

};


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
    void startGeneration();
    void generationProgress(int generation);
    void generationFinished();

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<RuleForm>> addedRules;
    GAThread workerThread;

};
#endif // MAINWINDOW_H
