#include "inspector_tab.h"
#include <iostream>
#include <dart/simulation/World.h>
#include <tree_view.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>



Inspector_Tab::Inspector_Tab(QWidget *parent, simulation::World *simWorld, Tree_View *treeViewer)
 : QDockWidget(parent), inspector_ui(new Ui::Inspector_Tab)
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
    inspector_ui->setupUi(this);
    inspector_ui->positionSlider_0->setRange(-1800,1800); //I can override settings here
    inspector_ui->positionSlider_0->setValue(0);
    inspector_ui->positionSpinBox_0->setRange(-180.0,180.0);
    inspector_ui->positionSpinBox_0->setValue(0.0);

    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(ChangeJoint(int)));
    std::cout << "test simWorld in inspector constructor: " << simWorld << std::endl;
    world = simWorld;
    std::cout << "test world in inspector constructor: " << world << std::endl;
    std::cout << "Inspector constructor" << std::endl;
    treeview = treeViewer;

    std::cout << "test treeview in inspector constructor: " << treeview << std::endl;
}

void Inspector_Tab::ChangePos0DoubleSpinBox(int sliderValue){
    positionSpinBox_0->setValue((double)sliderValue);
    std::cout << "slider value: " << sliderValue << std::endl;
}

void Inspector_Tab::ChangePos0Slider(double spinBoxValue){
    positionSlider_0->setValue((int)spinBoxValue);
}

void Inspector_Tab::ChangeJoint(int sliderValue){
    int joint_id = 10;
    double joint_Value = 0.0;
    //if (inspec_ui.positionSlider_0 == inspec_ui.positionSlider_0->getdsvalue())
    //    joint_value = inspec_ui.positionSlider_0->getdsvalue();
    //else

    joint_Value = inspector_ui->positionSlider_0->getdsvalue();
    std::cout << joint_Value << std::endl;
    std::cout << treeview->getActiveItem() << std::endl;

    std::cout << "test changejoint" << std::endl;
    std::vector<int> indx;
    std::cout << world << std::endl;
    indx.push_back(world->getSkeleton(1)->getJoint("LSR")->getGenCoord(0)->getSkeletonIndex());


    Eigen::VectorXd q(1);
    q[0] = double(joint_Value*(3.14)/180.0);
    world->getSkeleton(1)->setConfig(indx, q); //getSkeleton(i) - choose ith object


    //std::cout << "change joint invoked: "<< joint_value << std::endl;
}

Inspector_Tab::~Inspector_Tab()
{
}
