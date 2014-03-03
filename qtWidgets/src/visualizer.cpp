#include "visualizer.h"

Visualizer::Visualizer(QWidget *parent)
 : QDockWidget(parent)
{
    QDockWidget *widget = new QDockWidget(this);
    visualizer_ui.setupUi(widget);
    //setMainWidget(widget);
    //Ui_Visualizer::setupUi(this);
    //this->addDockWidget(Qt::BottomDockWidgetArea, widget);
    //widget->show();
}

Visualizer::~Visualizer()
{
}
