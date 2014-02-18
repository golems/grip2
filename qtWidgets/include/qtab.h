#ifndef QTAB_H
#define QTAB_H

#include <QTabWidget>
#include "ui_qtab.h"

//class QButtonGroup;

class Qtab : public QTabWidget {
    Q_OBJECT
public:
    Qtab();
private:
    Ui::Qtab ui;

};

#endif

