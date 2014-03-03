#include "inspector.h"

Inspector::Inspector(QWidget *parent)
 : QDockWidget(parent)
{
    QDockWidget *widget = new QDockWidget(this);
    inspector_ui.setupUi(widget);
    //setMainWidget(widget);
    //Ui_Visualizer::setupUi(this);
    //this->addDockWidget(Qt::BottomDockWidgetArea, widget);
    //widget->show();
}

Inspector::~Inspector()
{
}
