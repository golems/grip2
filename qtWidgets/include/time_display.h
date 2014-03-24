#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include "ui_time_display.h"

//class ..

class Time_Display : public QDockWidget {

public:
    Time_Display(QWidget *parent = 0);
    ~Time_Display();
    void Update_Time(double sim_time, double rel_time);

private:
    Ui::Time_Display *time_display_ui;

};

#endif
