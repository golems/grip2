#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui_inspector.h"

//class ..

class Inspector : public QDockWidget {
    Q_OBJECT
public:
    Inspector(QWidget *parent = 0);
    ~Inspector();
private slots:
    void ChangePos0DoubleSpinBox(int sliderValue);
    void ChangePos0Slider(double spinBoxValue);
private:
    Ui::Inspector inspector_ui;
    QSlider *positionSlider_0;
    QDoubleSpinBox *positionSpinBox_0;
};

#endif

