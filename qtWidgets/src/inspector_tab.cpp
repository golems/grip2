#include "inspector_tab.h"
#include <iostream>
#include <dart/simulation/World.h>
#include <tree_view.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
//#include <typeinfo>
#include <QDebug>
#include <QString>


Inspector_Tab::Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer)
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
//    inspector_ui->positionSlider_0->setRange(-1800,1800); //I can override settings here
//    inspector_ui->positionSlider_0->setValue(0);
//    inspector_ui->positionSpinBox_0->setRange(-180.0,180.0);
//    inspector_ui->positionSpinBox_0->setValue(0.0);
    inspector_ui->positionSlider_0->setMinMaxDecimalValue(-20.0,10.0,1);
    inspector_ui->positionSpinBox_0->setRange(-20.0,10.0);
    inspector_ui->positionSpinBox_0->setDecimals(1);
    std::cerr << "slider min vlaue :" << inspector_ui->positionSlider_0->getMinValue() << std::endl;
    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(ChangeJoint(int)));
    //std::cout << "test simWorld in inspector constructor: " << simWorld << std::endl;
    simworld = simWorld;
    //std::cout << "test world in inspector constructor: " << simworld << std::endl;
    //std::cout << "Inspector constructor" << std::endl;
    treeview = treeViewer;

    connect(treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(ReceiveSeletedItem(TreeViewReturn*)));
    //std::cout << "connected" << std::endl;
    //std::cout << "test treeview in inspector constructor: " << treeview << std::endl;

}

void Inspector_Tab::ChangePos0DoubleSpinBox(int sliderValue){
 //   positionSpinBox_0->setValue((double)sliderValue);
 //   std::cout << "slider value: " << sliderValue << std::endl;
}

void Inspector_Tab::ChangePos0Slider(double spinBoxValue){
 //   positionSlider_0->setValue((int)spinBoxValue);
}

void Inspector_Tab::ChangeJoint(int sliderValue){

    double joint_Value = 0.0;

    joint_Value = inspector_ui->positionSlider_0->getdsvalue();

    if (treeview->getActiveItem()->dType == 0) //if Robot, active_item->object = *skel
     {
         dart::dynamics::Skeleton* item_selected;
         item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
         std::cerr << "Skeleton Selected" << std::endl;
         qDebug() << QString::fromStdString(item_selected->getName()) ;
     }
    else if (treeview->getActiveItem()->dType == 1) //if Node, active_item->object = *node
     {
         dart::dynamics::BodyNode* item_selected;
         item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;
         std::cerr << "BodyNode Selected" << std::endl;
         qDebug() << QString::fromStdString(item_selected->getParentJoint()->getName()) ;
     }
    else
    {
     std::cerr << "Not identified object passed to ChangeJoint" << std::endl;
    }
    //std::cout << "test changejoint" << std::endl;


    if(simworld) {
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
         //indx.push_back( simworld->getSkeleton(1)->getJoint("LSR")->getGenCoord(0)->getSkeletonIndex() );
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
         std::cerr << "node selected" << std::endl;
         dart::dynamics::BodyNode* item_selected;
         item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

         std::vector<int> indx;
         indx.push_back( simworld->getSkeleton(1)->getJoint(item_selected->getParentJoint()->getName())->getGenCoord(0)->getSkeletonIndex() );


         Eigen::VectorXd q(1);
         q[0] = double(joint_Value*(M_PI)/180.0);
         simworld->getSkeleton(1)->setConfig(indx, q); //getSkeleton(i) - choose ith object
        // QString::fromStdString(item_selected->getParentJoint()->getName())
        }
        else
         std::cerr << "Err" << std::endl;

    } else {
        std::cerr << "No world on line " << __LINE__ << " of " << __FILE__ << std::endl;
    }

    //std::cout << "change joint invoked: "<< joint_value << std::endl;
}

Inspector_Tab::~Inspector_Tab()
{
}

void Inspector_Tab::ReceiveSeletedItem(TreeViewReturn* active_item)
{
 std::cerr << "Item seleted received" << std::endl;
 std::cerr << "Data type " << active_item->dType << std::endl;
 if (active_item->dType == 0) //if Robot, active_item->object = *skel
 {
     dart::dynamics::Skeleton* item_selected;
     item_selected = (dart::dynamics::Skeleton*)active_item->object;

     std::cerr << "Skeleton" << std::endl;
     qDebug() << QString::fromStdString(item_selected->getName()) ;
 }
 else if (active_item->dType == 1) //if Node, active_item->object = *node
 {
     dart::dynamics::BodyNode* item_selected;
     item_selected = (dart::dynamics::BodyNode*)active_item->object;
     std::cerr << "BodyNode" << std::endl;
     qDebug() << QString::fromStdString(item_selected->getParentJoint()->getName()) ;
 }
 else
 {
     std::cerr << "Not identified object selected" << __LINE__ << " of " << __FILE__ << std::endl;

 }

}
