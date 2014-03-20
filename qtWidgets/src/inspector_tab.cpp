#include "inspector_tab.h"
#include <iostream>
#include <dart/simulation/World.h>
#include <tree_view.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/dynamics/FreeJoint.h>
#include <QDebug>
#include <QString>
#include <QMetaMethod>

//change units
inline double DEG2RAD(double deg) { return (M_PI*deg/180.0); }
inline double RAD2DEG(double rad) { return (180.0/M_PI*rad); }

Inspector_Tab::Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer)
 : QDockWidget(parent), inspector_ui(new Ui::Inspector_Tab), simworld(simWorld), treeview(treeViewer), selected_type_from_tree(0)
{
    inspector_ui->setupUi(this);
    /// selected joint slider
    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(changeSelectedJoint(int)));

    /// robot position and orientation sliders
    connect(inspector_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_1->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_1->setValue(0);

    connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_2->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_2->setValue(0);

    connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_3->setMinMaxDecimalValue(-10.0,10.0,2);
    inspector_ui->positionSlider_3->setValue(0);

    connect(inspector_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(inspector_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(inspector_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));

    connect(treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(receiveSeletedItem(TreeViewReturn*)));
    inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
    inspector_ui->Position_Slider_GroupBox->setDisabled(true);
    inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);

}

void Inspector_Tab::changePositionAndOrientation(int sliderValue){

    QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());

    if(simworld)
    { /// if world is defined
        //std::cerr << "Num skels: " << simworld->getNumSkeletons() << std::endl;
        selected_type_from_tree = treeview->getActiveItem()->dType;

        if (selected_type_from_tree == Return_Type_Robot) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;

        }
        else if (selected_type_from_tree == Return_Type_Node) //if bodynode, change configuration using slider
        {

             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             /// check if the joint is a free joint (6DoF)
             if(dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
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

                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// double check, if the node is the root and free
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


void Inspector_Tab::changeSelectedJoint(int sliderValue){

    if(simworld) {

        selected_type_from_tree = treeview->getActiveItem()->dType;

        if (selected_type_from_tree == Return_Type_Robot) //if robot, do nothing
        {
             dart::dynamics::Skeleton* item_selected;
             item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;

        }
        else if (selected_type_from_tree == Return_Type_Node) //if bodynode, change configuration using slider
        {
             //std::cerr << "ChangeJoint: node selected" << std::endl;
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
             {
                     std::cerr << "Weldjoint is selected" << std::endl;
             }
             else
             {
                     if (item_selected->getSkeletonIndex() !=0) /// double check, if the node is not the root
                     {
                         std::vector<int> indx;
                         indx.push_back( simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getJoint(item_selected->getParentJoint()->getName())->getGenCoord(0)->getSkeletonIndex() );
                         Eigen::VectorXd q(1);
                         //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                         q[0] = DEG2RAD(inspector_ui->positionSlider_0->getdsvalue());
                         //std::cerr<< "Num of gen. coordinate in selected item : " << item_selected->getParentJoint()->getNumGenCoords() << std::endl;
                         simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, q); //getSkeleton(i) - choose ith object

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

void Inspector_Tab::receiveSeletedItem(TreeViewReturn* active_item)
{
    selected_type_from_tree = active_item->dType;

    if (selected_type_from_tree == Return_Type_Robot) //if Robot, active_item->object = *skel
    {
        dart::dynamics::Skeleton* item_selected;
        item_selected = (dart::dynamics::Skeleton*)active_item->object;

        std::cerr << "ReceiveSelectedItem: Skeleton is seleted" << std::endl;

        inspector_ui->parent_selected_display->setText(QString::fromStdString(item_selected->getName()));
        inspector_ui->item_selected_display->setText(QString::fromStdString(item_selected->getName()));
        inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
        inspector_ui->Position_Slider_GroupBox->setDisabled(true);
        inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);
    }
    else if (selected_type_from_tree == Return_Type_Node) //if Node, active_item->object = *node
    {
        dart::dynamics::BodyNode* item_selected;
        item_selected = (dart::dynamics::BodyNode*)active_item->object;
        //std::cerr << "ReceiveSelectedItem: BodyNode is selected" << std::endl;
        //qDebug() << QString::fromStdString(item_selected->getParentJoint()->getName()) ;

        if (item_selected->getSkeletonIndex() !=0) //double check, if the selected node's index is not zero
        {
            inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getParentBodyNode()->getName()) );
            inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );
            if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
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
                inspector_ui->Joint_Slider_GroupBox->setEnabled(true);
                inspector_ui->Position_Slider_GroupBox->setDisabled(true);
                inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);
            }

        }
        else // node index is zero, i.e. does not have a parent node
        {
            inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getName()) );
            inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );

            /// if the root node is fixed
            if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
            {
                std::cerr << "Weldjoint is selected" << std::endl;
            }
            else // if the root node is not fixed
            {
                std::cerr << "Root link/joint: free!!" << std::endl;
                inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
                inspector_ui->Position_Slider_GroupBox->setEnabled(true);
                inspector_ui->Orientation_Slider_GroupBox->setEnabled(true);

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



///**
// * @function getRootTransform
// * @brief Return <x,y,z, r, p, y> Remember that get_q() gives you the screw so
// * do NOT use it directly
// */

//Eigen::Matrix<double, 6, 1> Inspector_Tab::getRootTransform(dart::dynamics::Skeleton* robot)
//{
//    dart::dynamics::Joint *joint = robot->getRootBodyNode()->getParentJoint();
//    Eigen::Matrix<double, 6, 1> pose;

//    if(joint->getJointType() == dart::dynamics::Joint::FREE) {
//        Matrix<double, 6, 1> q = joint->get_q();
//        Eigen::Isometry3d Tf = dart::math::expMap( joint->get_q() );
//        pose.head<3>() = Tf.translation();
//        pose.tail<3>() = dart::math::matrixToEulerXYZ( Tf.linear() );
//    }
//    else {
//        pose = getPoseFromTransform(joint->getTransformFromParentBodyNode());
//    }

//    return pose;
//}

///**
// * @function setRootTransform
// * @brief Set q (SCREW) from pose <x,y,z,r,p,y>
// */
//void Inspector_Tab::setRootTransform(dart::dynamics::Skeleton* robot, const Eigen::Matrix<double, 6, 1>& pose )
//{
//    dart::dynamics::Joint* joint = robot->getRootBodyNode()->getParentJoint();

//    if(dynamic_cast<dart::dynamics::FreeJoint*>(joint)) {
//        Matrix<double, 6, 1> q;
//        Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
//        transform.translation() = pose.head<3>();
//        transform.linear() = dart::math::eulerXYZToMatrix(pose.tail<3>());
//        q = dart::math::logMap(transform);
//        joint->set_q( q );

//    }
//    else {
//        Eigen::Isometry3d transform;
//        transform.makeAffine();
//        transform.linear() = dart::math::eulerXYZToMatrix(pose.tail<3>());
//        transform.translation() = pose.head<3>();
//        joint->setTransformFromParentBodyNode(transform);
//    }

//    for (int i = 0; i < robot->getNumBodyNodes(); ++i) {
//        robot->getBodyNode(i)->updateTransform();
//    }
//}

///**
// * @function getPoseTransform
// * @brief Get a vector <x,y,z,r,p,y> from Transform. NO SCREW
// */
//Eigen::Matrix<double, 6, 1> Inspector_Tab::getPoseFromTransform(const Eigen::Isometry3d& tf)
//{
//    Eigen::Matrix<double, 6, 1> pose;
//    pose.head<3>() = tf.translation();
//    pose.tail<3>() = dart::math::matrixToEulerXYZ(tf.linear());
//    return pose;
//}


