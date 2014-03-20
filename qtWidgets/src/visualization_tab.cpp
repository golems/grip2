#include "visualization_tab.h"
#include <iostream>

Visualization_Tab::Visualization_Tab(QWidget *parent)
 : QDockWidget(parent), visualizer_ui(new Ui::Visualization_Tab)
{
    //QDockWidget *widget = new QDockWidget(this);
    //visualizer_ui.setupUi(widget);
    //setMainWidget(widget);
    //Ui_Visualizer::setupUi(this);
    //this->addDockWidget(Qt::BottomDockWidgetArea, widget);
    //widget->show();
    visualizer_ui->setupUi(this);
}

Visualization_Tab::~Visualization_Tab()
{
}
