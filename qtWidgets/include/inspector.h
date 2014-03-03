#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "ui_inspector.h"

//class ..

class Inspector : public QDockWidget {
    Q_OBJECT
public:
    Inspector(QWidget *parent = 0);
    ~Inspector();
private:
    Ui::Inspector inspector_ui;

};

#endif
