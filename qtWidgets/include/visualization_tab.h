#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "ui_visualization_tab.h"

//class ..

class Visualization_Tab : public QDockWidget {
    Q_OBJECT
public:
    Visualization_Tab(QWidget *parent = 0);
    ~Visualization_Tab();

public slots:
    void slotShowHideJointAxes();
private:
    Ui::Visualization_Tab *visualizer_ui;

};

#endif
