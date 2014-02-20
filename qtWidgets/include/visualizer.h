#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "ui_visualizer.h"

//class ..

class Visualizer : public QDockWidget {
    Q_OBJECT
public:
    Visualizer(QWidget *parent = 0);
    ~Visualizer();
private:
    Ui::Visualizer visualizer_ui;

};

#endif
