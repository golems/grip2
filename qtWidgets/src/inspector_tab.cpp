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
#include <QMetaMethod>

//change units
inline double DEG2RAD(double deg) { return (M_PI*deg/180.0); }
inline double RAD2DEG(double rad) { return (180.0/M_PI*rad); }

Inspector_Tab::Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer)
 : QDockWidget(parent), inspector_ui(new Ui::Inspector_Tab)
{
    inspector_ui->setupUi(this);
    /// selected joint slider
    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(ChangeSelectedJoint(int)));

    /// robot position and orientation slider
    connect(inspector_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));
    inspector_ui->positionSlider_1->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_1->setValue(0);

    connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));
    inspector_ui->positionSlider_2->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_2->setValue(0);

    connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));
    inspector_ui->positionSlider_3->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_3->setValue(0);

    connect(inspector_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));
    connect(inspector_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));
    connect(inspector_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(ChangeSliders(int)));

    simworld = simWorld;
    treeview = treeViewer;

    connect(treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(ReceiveSeletedItem(TreeViewReturn*)));

}

void Inspector_Tab::ChangeSliders(int sliderValue){
    double Slider_Value = 0.0;
    QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());

    //int num_gen_coordinate;


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

             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             /// check if the joint is a free joint (6DoF)
             if(dart::dynamics::FreeJoint* joint = dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 int joint_index = 0;
                 std::vector<int> indx;
                 Eigen::VectorXd q(1);

                 if (sender() == inspector_ui->positionSlider_1)
                 {
                     joint_index = 3;
                     q[0]  = inspector_ui->positionSlider_1->getdsvalue();

                 }
                 else if (sender() == inspector_ui->positionSlider_2)
                 {
                     joint_index = 4;
                     q[0] = inspector_ui->positionSlider_2->getdsvalue();

                 }
                 else if (sender() == inspector_ui->positionSlider_3)
                 {
                     joint_index = 5;
                     q[0] = inspector_ui->positionSlider_3->getdsvalue();

                 }
                 else if (sender() == inspector_ui->orientationSlider_1)
                 {
                     joint_index = 0;
                     q[0] = DEG2RAD(inspector_ui->orientationSlider_1->getdsvalue());

                 }
                 else if (sender() == inspector_ui->orientationSlider_2)
                 {
                     joint_index = 1;
                     q[0] = DEG2RAD(inspector_ui->orientationSlider_2->getdsvalue());

                 }
                 else if (sender() == inspector_ui->orientationSlider_3)
                 {
                     joint_index = 2;
                     q[0] = DEG2RAD(inspector_ui->orientationSlider_3->getdsvalue());

                 }

                 else
                 {
                     std::cerr << "Error in slider joint index" << std::endl;
                     return;
                 }

                 indx.push_back( item_selected->getParentJoint()->getGenCoord(joint_index)->getSkeletonIndex());

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// if the node is the root
                 {

                     simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object

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
        std::cerr << "Change Position & Orientation: No world is loaded " << __LINE__ << " of " << __FILE__ << std::endl;
    }
}


void Inspector_Tab::ChangeSelectedJoint(int sliderValue){

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
                 q[0] = DEG2RAD(inspector_ui->positionSlider_0->getdsvalue());
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
            inspector_ui->positionSlider_0->setMinMaxDecimalValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()),1);
            inspector_ui->positionSlider_0->setValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
            inspector_ui->positionSpinBox_0->setRange(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()));
            inspector_ui->positionSpinBox_0->setDecimals(1);
            inspector_ui->positionSpinBox_0->setValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
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
