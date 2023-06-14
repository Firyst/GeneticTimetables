#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>

namespace Ui {
class ParameterWidget;
}

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    ParameterWidget(QWidget *parent = nullptr, QString title = "Empty", QString name = " cm", int parameter = 28, float newMinimum = 0, float newMaximum = 10, float newStep = 2);
    float getCurrentValue();
    float setCurrentValue(float value);
    ~ParameterWidget();

private slots:
    void chooseParameter(int value);
    void onMinimumChanged(int newMinimum);
    void onMaximumChanged(int newMinimum);
    void stepChanged(double newStep);
    void valueChanged(int parameter);

private:
    Ui::ParameterWidget *ui;
    float step;
    QString name;
};

#endif // PARAMETERWIDGET_H
