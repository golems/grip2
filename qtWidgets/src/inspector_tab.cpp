#include "inspector_tab.h"
#include <iostream>
#include <dart/simulation/World.h>
#include <tree_view.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/dynamics/FreeJoint.h>
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
//    inspector_ui->positionSlider_0->setMinMaxDecimalValue(-20.0,10.0,1);
//    inspector_ui->positionSpinBox_0->setRange(-20.0,10.0);
//    inspector_ui->positionSpinBox_0->setDecimals(1);
//    std::cerr << "slider min vlaue :" << inspector_ui->positionSlider_0->getMinValue() << std::endl;
    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(ChangeSelectedJoint(int)));
    connect(inspector_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(ChangeXPosition(int)));
    connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(ChangeYPosition(int)));
    connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(ChangeZPosition(int)));
    connect(inspector_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(ChangeROrientation(int)));
    connect(inspector_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(ChangePOrientation(int)));
    connect(inspector_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(ChangeYOrientation(int)));
   // connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(ChangeYPosition(int)));
   // connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(ChangeZPosition(int)));
    //std::cout << "test simWorld in inspector constructor: " << simWorld << std::endl;
    simworld = simWorld;
    //std::cout << "test world in inspector constructor: " << simworld << std::endl;
    //std::cout << "Inspector constructor" << std::endl;
    treeview = treeViewer;

    connect(treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(ReceiveSeletedItem(TreeViewReturn*)));
    //std::cout << "connected" << std::endl;
    //std::cout << "test treeview in inspector constructor: " << treeview << std::endl;

}



void Inspector_Tab::ChangeXPosition(int sliderValue){
    double joint_Value = 0.0;

    //int num_gen_coordinate;

    joint_Value = inspector_ui->positionSlider_1->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;
             //qDebug() << QString::fromStdString(item_selected->getName()) ;
             //std::cerr << "ChangeXPosition: Seleted item is " << item_selected->getName() << std::endl;
             //std::cerr << "ChangeXPosition: Skeleton id: " << treeview->getActiveItem()->skeletonID << std::endl;
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeXPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;
             //std::cerr << "ChangeJoint: node selected step2" << std::endl;
             //std::cerr << item_selected->getName() << std::endl;
             //std::cerr << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;
             //std::cerr << simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getGenCoord(0) << std::endl;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 //std::cerr << "Free joint" << std::endl;

                 //num_gen_coordinate = item_selected->getParentJoint()->getNumGenCoords();
                 //std::cerr << "num_gen_coordiate: " << num_gen_coordinate << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(3)->getSkeletonIndex()); // X direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeXPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeXPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

void Inspector_Tab::ChangeYPosition(int sliderValue){
    double joint_Value = 0.0;

    joint_Value = inspector_ui->positionSlider_2->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;

        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeYPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;


             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(4)->getSkeletonIndex()); // Y direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeYPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeYPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

void Inspector_Tab::ChangeZPosition(int sliderValue){
    double joint_Value = 0.0;

    //int num_gen_coordinate;

    joint_Value = inspector_ui->positionSlider_3->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;

        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeZPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(5)->getSkeletonIndex()); // X direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeZPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeZPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

void Inspector_Tab::ChangeROrientation(int sliderValue){
    double joint_Value = 0.0;

    //int num_gen_coordinate;

    joint_Value = inspector_ui->orientationSlider_1->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;
             //qDebug() << QString::fromStdString(item_selected->getName()) ;
             //std::cerr << "ChangeXPosition: Seleted item is " << item_selected->getName() << std::endl;
             //std::cerr << "ChangeXPosition: Skeleton id: " << treeview->getActiveItem()->skeletonID << std::endl;
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeXPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;
             //std::cerr << "ChangeJoint: node selected step2" << std::endl;
             //std::cerr << item_selected->getName() << std::endl;
             //std::cerr << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;
             //std::cerr << simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getGenCoord(0) << std::endl;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 //std::cerr << "Free joint" << std::endl;

                 //num_gen_coordinate = item_selected->getParentJoint()->getNumGenCoords();
                 //std::cerr << "num_gen_coordiate: " << num_gen_coordinate << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(0)->getSkeletonIndex()); // X direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeXPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeXPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

void Inspector_Tab::ChangePOrientation(int sliderValue){
    double joint_Value = 0.0;

    //int num_gen_coordinate;

    joint_Value = inspector_ui->orientationSlider_2->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;
             //qDebug() << QString::fromStdString(item_selected->getName()) ;
             //std::cerr << "ChangeXPosition: Seleted item is " << item_selected->getName() << std::endl;
             //std::cerr << "ChangeXPosition: Skeleton id: " << treeview->getActiveItem()->skeletonID << std::endl;
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeXPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;
             //std::cerr << "ChangeJoint: node selected step2" << std::endl;
             //std::cerr << item_selected->getName() << std::endl;
             //std::cerr << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;
             //std::cerr << simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getGenCoord(0) << std::endl;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 //std::cerr << "Free joint" << std::endl;

                 //num_gen_coordinate = item_selected->getParentJoint()->getNumGenCoords();
                 //std::cerr << "num_gen_coordiate: " << num_gen_coordinate << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(1)->getSkeletonIndex()); // X direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeXPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeXPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}


void Inspector_Tab::ChangeYOrientation(int sliderValue){
    double joint_Value = 0.0;

    //int num_gen_coordinate;

    joint_Value = inspector_ui->orientationSlider_3->getdsvalue();
    //std::cerr << "ChangeXPositionCalled" << std::endl;

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;
             //qDebug() << QString::fromStdString(item_selected->getName()) ;
             //std::cerr << "ChangeXPosition: Seleted item is " << item_selected->getName() << std::endl;
             //std::cerr << "ChangeXPosition: Skeleton id: " << treeview->getActiveItem()->skeletonID << std::endl;
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeXPosition: node selected step1" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;
             //std::cerr << "ChangeJoint: node selected step2" << std::endl;
             //std::cerr << item_selected->getName() << std::endl;
             //std::cerr << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;
             //std::cerr << simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getGenCoord(0) << std::endl;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 //std::cerr << "Free joint" << std::endl;

                 //num_gen_coordinate = item_selected->getParentJoint()->getNumGenCoords();
                 //std::cerr << "num_gen_coordiate: " << num_gen_coordinate << "skeleton index: " << item_selected->getSkeletonIndex() << std::endl;

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {
                     std::vector<int> indx;
                     indx.push_back( item_selected->getParentJoint()->getGenCoord(2)->getSkeletonIndex()); // X direction
                     Eigen::VectorXd q(1);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     q[0] = double(joint_Value*(M_PI)/180.0);
                     //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                     // QString::fromStdString(item_selected->getParentJoint()->getName())
                     //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint";
            }
        }
        else
        {
            std::cerr << "ChangeXPosition: Err" << std::endl;
        }

    }
    else
    {
        std::cerr << "ChangeXPosition: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

void Inspector_Tab::ChangeSelectedJoint(int sliderValue){

    double joint_Value = 0.0;
    joint_Value = inspector_ui->positionSlider_0->getdsvalue();

    if(simworld) {

        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;

        if (treeview->getActiveItem()->dType == 0) //if robot, do nothing
        {
         dart::dynamics::Skeleton* item_selected;
         item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
         //std::cerr << "ChangeJoint: Skeleton is Selected" << std::endl;
         //qDebug() << QString::fromStdString(item_selected->getName()) ;
         //std::cerr << "ChangeJoint: Seleted item is " << item_selected->getName() << std::endl;
         //std::cerr << "ChangeJoint: Skeleton id: " << treeview->getActiveItem()->skeletonID << std::endl;
        }
        else if (treeview->getActiveItem()->dType == 1) //if bodynode, change configuration using slider
        {
         //std::cerr << "ChangeJoint: node selected" << std::endl;
         dart::dynamics::BodyNode* item_selected;
         item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

         if (dart::dynamics::WeldJoint* joint = dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
         {
                 std::cerr << "Weldjoint is selected" << std::endl;
         }
         else
         {
                 if (item_selected->getSkeletonIndex() !=0) /// if the node is not the root
                 {
                 std::vector<int> indx;
                 indx.push_back( simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getJoint(item_selected->getParentJoint()->getName())->getGenCoord(0)->getSkeletonIndex() );
                 Eigen::VectorXd q(1);
                 //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                 q[0] = double(joint_Value*(M_PI)/180.0);
                 //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                 simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object
                 // QString::fromStdString(item_selected->getParentJoint()->getName())
                 //item_selected->getParentJoint()->getGenCoord(0)->q_max
                 }
         }
        }
        else
         std::cerr << "ChangeJoint: Err" << std::endl;

    } else {
        std::cerr << "ChangeJoint: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }

}

Inspector_Tab::~Inspector_Tab()
{
}

void Inspector_Tab::ReceiveSeletedItem(TreeViewReturn* active_item)
{
 //std::cerr << "ReceiveSelectedItem: Item seleted received" << std::endl;
 //std::cerr << "ReceiveSelectedItem: Data type " << active_item->dType << std::endl;

 if (active_item->dType == 0) //if Robot, active_item->object = *skel
 {
     dart::dynamics::Skeleton* item_selected;
     item_selected = (dart::dynamics::Skeleton*)active_item->object;

     std::cerr << "ReceiveSelectedItem: Skeleton is seleted" << std::endl;
     //qDebug() << QString::fromStdString(item_selected->getName()) ;
    inspector_ui->parent_selected_display->setText(QString::fromStdString(item_selected->getName()));
    inspector_ui->item_selected_display->setText(QString::fromStdString(item_selected->getName()));
 }
 else if (active_item->dType == 1) //if Node, active_item->object = *node
 {
     dart::dynamics::BodyNode* item_selected;
     item_selected = (dart::dynamics::BodyNode*)active_item->object;
     //std::cerr << "ReceiveSelectedItem: BodyNode is selected" << std::endl;
     //qDebug() << QString::fromStdString(item_selected->getParentJoint()->getName()) ;
    if (item_selected->getSkeletonIndex() !=0) //if the selected node's index is not zero
    {
        inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getParentBodyNode()->getName()) );
        inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );
        if (dart::dynamics::WeldJoint* joint = dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
        {
            std::cerr << "Weldjoint is selected" << std::endl;
        }
        else
        {
            ///joint max,min and decimal point setting
            inspector_ui->positionSlider_0->setMinMaxDecimalValue((item_selected->getParentJoint()->getGenCoord(0)->get_qMin())*180.0/M_PI,(item_selected->getParentJoint()->getGenCoord(0)->get_qMax())*180.0/M_PI,1);
            inspector_ui->positionSlider_0->setValue((item_selected->getParentJoint()->getGenCoord(0)->get_q())*180.0/M_PI);
            inspector_ui->positionSpinBox_0->setRange((item_selected->getParentJoint()->getGenCoord(0)->get_qMin())*180.0/M_PI,(item_selected->getParentJoint()->getGenCoord(0)->get_qMax())*180.0/M_PI);
            inspector_ui->positionSpinBox_0->setDecimals(1);
            inspector_ui->positionSpinBox_0->setValue((item_selected->getParentJoint()->getGenCoord(0)->get_q())*180.0/M_PI);
        }
    }
    else // node index is zero, i.e. does not have a parent node
    {
        inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getName()) );
        inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );

        /// if the root node is fixed
        if (dart::dynamics::WeldJoint* joint = dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
        {
            std::cerr << "Weldjoint is selected" << std::endl;
        }
        else // if the root node is not fixed
        {
            std::cerr << "Root link/joint: free!!" << std::endl;
            ///joint max,min and decimal point setting
            //double link0_max = simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getJoint(item_selected->getSkeletonIndex())->getGenCoord(0)->get_qMax();
            //double link0_min = simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getJoint(item_selected->getSkeletonIndex())->getGenCoord(0)->get_qMin();
            //std::cerr << "link0 min: " << link0_min << " link0_max: " << link0_max << std::endl;
            //inspector_ui->positionSlider_0->setMinMaxDecimalValue(link0_min*180.0/M_PI,link0_max*180.0/M_PI,1);
            //inspector_ui->positionSlider_0->setValue(0);
            //inspector_ui->positionSpinBox_0->setRange(link0_min*180.0/M_PI,link0_max*180.0/M_PI);
            //inspector_ui->positionSpinBox_0->setDecimals(1);
            //inspector_ui->positionSpinBox_0->setValue(0.0);
        }
    }
    //std::cerr <<"skeleton index : " << item_selected->get << std::endl;




 }
 else
 {
    std::cerr << "ReceiveSelectedItem: No identified object selected" << __LINE__ << " of " << __FILE__ << std::endl;
    inspector_ui->parent_selected_display->setText(QString("%1").arg("Not identified"));
    inspector_ui->item_selected_display->setText(QString("%1").arg("Not identified"));

 }



}
