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
    const long step = 200;

    void run(void);

signals:
    void progressSignal(long );
    void finishedSignal(void );

};

/**
 * @brief The MainWindow class used for working in main window
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief removeRule used for deleting whole line subject
     * @param objectIndex is int number of this subject
     */
    void removeRule(int objectIndex);

    /**
     * @brief createParameterWidgets used for creating new sliders
     * @param name is string parameter of slider name
     * @param value is int default value of slider
     * @param start is int default start value of slider
     * @param end is int default end value of slider
     * @param step is float default step value of slider
     * @param target is pointer to layout
     * @param suffix is string parameter name of slider value
     */
    void createParameterWidgets(QString name, int value, int start, int end, float step, QVBoxLayout* target, QString suffix="x");

    /**
     * @brief parameters is pointer's array of slider's parameters
     */
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
    void saveTable();

private:
    Ui::MainWindow *ui;

    /**
     * @brief addedRules is pointer's array of subject's information(name, teacher, amount)
     */
    std::vector<std::unique_ptr<RuleForm>> addedRules;
    GAThread workerThread;
    std::vector<Timetable> bestResultBuffer;
    void setTimetableOutput(Timetable* table);
    void exportCurrentTable(const QString& filePath);
    QStandardItemModel outputTableModel;

};
#endif // MAINWINDOW_H
