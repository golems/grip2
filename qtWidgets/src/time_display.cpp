#include "time_display.h"
#include <cmath>

Time_Display::Time_Display (QWidget *parent)
 : QDockWidget(parent), time_display_ui(new Ui::Time_Display)
{

    time_display_ui->setupUi(this);
}

Time_Display::~Time_Display()
{
}

void Time_Display::Update_Time(double sim_time, double rel_time)
{
    time_display_ui->time_edit_sim->setText(QString("%1").arg(sim_time));
    time_display_ui->time_edit_rel->setText(QString("%1").arg(ceil(rel_time*100)/100));
}
