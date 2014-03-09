#include "inspector.h"
#include <iostream>

Inspector::Inspector(QWidget *parent)
 : QDockWidget(parent)
{
 //   QDockWidget *widget = new QDockWidget(this);
 //   inspector_ui.setupUi(widget);
 //   positionSlider_0 = new QSlider(this);//(widget);
 //   positionSlider_0->setRange(0,100);
 //   positionSpinBox_0 =  new QDoubleSpinBox(this);//(widget);
 //   positionSpinBox_0->setRange(0.0,100.0);

 //   std::cout << "slider value: " << std::endl;

 //   connect(positionSlider_0, SIGNAL(valueChanged(int)),positionSpinBox_0, SLOT(ChangePos0DoubleSpinBox(int)));
 //   connect(positionSpinBox_0, SIGNAL(vlaueChanged(double)), positionSlider_0, SLOT(ChangePos0Slider(double)));
    //setMainWidget(widget);
    //Ui_Visualizer::setupUi(this);
    //this->addDockWidget(Qt::BottomDockWidgetArea, widget);
    //widget->show();
}

void Inspector::ChangePos0DoubleSpinBox(int sliderValue){
    positionSpinBox_0->setValue((double)sliderValue);
    std::cout << "slider value: " << sliderValue << std::endl;
}

void Inspector::ChangePos0Slider(double spinBoxValue){
    positionSlider_0->setValue((int)spinBoxValue);
}

Inspector::~Inspector()
{
}
