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
#include <memory>
#include <chrono>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class GAThread : public QThread {
    Q_OBJECT

public:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::milliseconds lastExecutionTime;
    std::unique_ptr<Population> population;
    long iterations{0};
    const long step = 250;

    void run(void);

signals:
    void progressSignal(long );
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
    void generationProgress(long generation);
    void generationFinished();
    void viewResult(int position);

private:
    Ui::MainWindow *ui;
    std::vector<std::unique_ptr<RuleForm>> addedRules;
    GAThread workerThread;
    std::vector<Timetable> bestResultBuffer;
    void setTimetableOutput(Timetable* table);
    QStandardItemModel outputTableModel;

};
#endif // MAINWINDOW_H
