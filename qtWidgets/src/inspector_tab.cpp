#include "inspector_tab.h"
#include <iostream>
#include <dart/simulation/World.h>
#include <tree_view.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/dynamics/FreeJoint.h>
#include <dart/dynamics/RevoluteJoint.h>
#include <dart/dynamics/PrismaticJoint.h>
#include <QDebug>
#include <QString>
#include <QMetaMethod>
//using namespace std;
using namespace Eigen;

//change units
inline double DEG2RAD(double angle_deg)  { return (angle_deg * 0.01745329251994329577); } //{ return (M_PI*angle_deg/180.0); }
inline double RAD2DEG(double angle_rad)	 { return (angle_rad * 57.2957795130823208768); } //{ return (180.0/M_PI*angle_rad); }

/**
 * @function Inspector_Tab()
 * @brief constructor of the class Inspector_Tab: setup the ui widget and connect signals and slots
 */

Inspector_Tab::Inspector_Tab(QWidget *parent, dart::simulation::World *simWorld, Tree_View *treeViewer)
 : QDockWidget(parent), inspector_ui(new Ui::Inspector_Tab), simworld(simWorld), treeview(treeViewer), selected_type_from_tree(0)
{
    inspector_ui->setupUi(this);
    int position_precision_decimal = 2;
    int orientation_precision_decimal = 2;

    /// selected joint slider
    connect(inspector_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(changeSelectedJoint(int)));

    /// robot position and orientation sliders
    connect(inspector_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_1->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    inspector_ui->positionSlider_1->setdsValue(0.0);
    inspector_ui->positionSpinBox_1->setRange(-10.0,10.0);
    inspector_ui->positionSpinBox_1->setDecimals(position_precision_decimal);
    inspector_ui->positionSpinBox_1->setSingleStep(pow(10,-position_precision_decimal));


    connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_2->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    inspector_ui->positionSlider_2->setdsValue(0.0);
    inspector_ui->positionSpinBox_2->setRange(-10.0,10.0);
    inspector_ui->positionSpinBox_2->setDecimals(position_precision_decimal);
    inspector_ui->positionSpinBox_2->setSingleStep(pow(10,-position_precision_decimal));

    connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->positionSlider_3->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    inspector_ui->positionSlider_3->setdsValue(0.0);
    inspector_ui->positionSpinBox_3->setRange(-10.0,10.0);
    inspector_ui->positionSpinBox_3->setDecimals(position_precision_decimal);
    inspector_ui->positionSpinBox_3->setSingleStep(pow(10,-position_precision_decimal));

    connect(inspector_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->orientationSlider_1->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    inspector_ui->orientationSlider_1->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_1->setRange(-180.0,180.0);
    inspector_ui->orientationSpinBox_1->setDecimals(orientation_precision_decimal);
    inspector_ui->orientationSpinBox_1->setSingleStep(pow(10,-orientation_precision_decimal));

    connect(inspector_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->orientationSlider_2->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    inspector_ui->orientationSlider_2->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_2->setRange(-180.0,180.0);
    inspector_ui->orientationSpinBox_2->setDecimals(position_precision_decimal);
    inspector_ui->orientationSpinBox_2->setSingleStep(pow(10,-orientation_precision_decimal));

    connect(inspector_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    inspector_ui->orientationSlider_3->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    inspector_ui->orientationSlider_3->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_3->setRange(-180.0,180.0);
    inspector_ui->orientationSpinBox_3->setDecimals(position_precision_decimal);
    inspector_ui->orientationSpinBox_3->setSingleStep(pow(10,-orientation_precision_decimal));

    connect(treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(receiveSeletedItem(TreeViewReturn*)));
    inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
    inspector_ui->Position_Slider_GroupBox->setDisabled(true);
    inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);

}

/**
 * @function changePositionAndOrientation
 * @brief change position and orientation of the root node
 */
void Inspector_Tab::changePositionAndOrientation(int sliderValue){

//  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
    Eigen::Matrix<double, 6, 1> pose;
    pose << 0, 0, 0, 0, 0, 0;

    /// if world is defined
    if(simworld)
    {
        selected_type_from_tree = treeview->getActiveItem()->dType;

        if (selected_type_from_tree == Return_Type_Robot) //if robot, do nothing
        {
             //dart::dynamics::Skeleton* item_selected;
             //item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;

        }
        else if (selected_type_from_tree == Return_Type_Node) //if bodynode, change configuration using slider
        {
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             /// check if the joint is a free joint (6DoF)
             if(dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// double check, if the node is the root and free
                 {
                       pose(0) = inspector_ui->positionSlider_1->getdsValue();
                       pose(1) = inspector_ui->positionSlider_2->getdsValue();
                       pose(2) = inspector_ui->positionSlider_3->getdsValue();
                       pose(3) = DEG2RAD(inspector_ui->orientationSlider_1->getdsValue());
                       pose(4) = DEG2RAD(inspector_ui->orientationSlider_2->getdsValue());
                       pose(5) = DEG2RAD(inspector_ui->orientationSlider_3->getdsValue());

                       setRootTransform(simworld->getSkeleton(treeview->getActiveItem()->skeletonID), pose);
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint" << std::endl;
            }
        }
        else
        {
            std::cerr << "changePositionAndOrientation: No return type identified" << treeview->getActiveItem()->dType << std::endl;
        }

    }
    else
    {
        std::cerr << "Change Position & Orientation: either simworld is empty" << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

/**
 * @function changeSelectedJoint
 * @brief move the selected joint(other than root joint) only
 */

void Inspector_Tab::changeSelectedJoint(int sliderValue){

    if(simworld) {

        selected_type_from_tree = treeview->getActiveItem()->dType;

        if (selected_type_from_tree == Return_Type_Robot) //if robot, do nothing
        {
             //dart::dynamics::Skeleton* item_selected;
             //item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "Robot is selected" << std::endl;

        }
        else if (selected_type_from_tree == Return_Type_Node) //if bodynode, change configuration using slider
        {
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)treeview->getActiveItem()->object;

             if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint())) //if the joint is fixed
             {
                     std::cerr << "A weldjoint is selected" << std::endl;
             }
             else
             {
                     if (item_selected->getSkeletonIndex() !=0) /// double check, if the node is not the root
                     {
                         std::vector<int> indx;
                         indx.push_back( simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->getJoint(item_selected->getParentJoint()->getName())->getGenCoord(0)->getSkeletonIndex() );
                         Eigen::VectorXd config(1);
                         config[0] = DEG2RAD(inspector_ui->positionSlider_0->getdsValue());
                         simworld->getSkeleton(treeview->getActiveItem()->skeletonID)->setConfig(indx, config); //getSkeleton(i) - choose ith object

                     }
                     else
                     {
                     // do nothing
                     }
             }
        }
        else
         std::cerr << "changeSelectedJoint: No matched return type." << std::endl;

    } else {
        std::cerr << "changeSelectedJoint: simworld is empty." << __LINE__ << " of " << __FILE__ << std::endl;
    }

}

/**
 * @function ~Inspector_Tab
 * @brief deconstructor of the class Inspector_Tab
 */

Inspector_Tab::~Inspector_Tab()
{
}

/**
 * @function receiveSeletedItem
 * @brief identify type of selected item from treeview and set the sliders properly
 */
void Inspector_Tab::receiveSeletedItem(TreeViewReturn* active_item)
{
    selected_type_from_tree = active_item->dType;

    if (selected_type_from_tree == Return_Type_Robot) //if Robot, active_item->object = *skel
    {
        dart::dynamics::Skeleton* item_selected;
        item_selected = (dart::dynamics::Skeleton*)active_item->object;

        std::cerr << "ReceiveSelectedItem: Robot is seleted" << std::endl;

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

        if (item_selected->getSkeletonIndex() != 0) //double check, if the selected node's index is not zero
        {
            inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getParentBodyNode()->getName()) );
            inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );
            ///if the joint is fixed, do nothing.
            if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
            {
                std::cerr << "A weldjoint is selected" << std::endl;
            }
            ///if the joint is movable(rovolute)
            else
            {
                ///joint max,min and decimal point setting
                int joint_precision_decimal  = 2;
                //std::cerr << "Joint is selected" << std::endl;
                inspector_ui->positionSlider_0->setMinMaxDecimalValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),
                                                                      RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()),joint_precision_decimal);
                inspector_ui->positionSpinBox_0->setRange(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()));
                inspector_ui->positionSpinBox_0->setDecimals(joint_precision_decimal);
                inspector_ui->positionSpinBox_0->setSingleStep(pow(10,-joint_precision_decimal));

                inspector_ui->positionSlider_0->setdsValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
                //inspector_ui->positionSpinBox_0->setdsValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
                ///enable joint slider only
                inspector_ui->Joint_Slider_GroupBox->setEnabled(true);
//                inspector_ui->Position_Slider_GroupBox->setDisabled(true);
//               inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);

                Eigen::Matrix<double, 6, 1> pose = Eigen::Matrix<double, 6, 1>::Zero();
                pose = getPoseFromTransform(item_selected->getWorldTransform());
                inspector_ui->positionSlider_1->setdsValue(pose(0));
                inspector_ui->positionSlider_2->setdsValue(pose(1));
                inspector_ui->positionSlider_3->setdsValue(pose(2));

                inspector_ui->orientationSlider_1->setdsValue(RAD2DEG(pose(3)));
                inspector_ui->orientationSlider_2->setdsValue(RAD2DEG(pose(4)));
                inspector_ui->orientationSlider_3->setdsValue(RAD2DEG(pose(5)));
                inspector_ui->Position_Slider_GroupBox->setDisabled(true);
                inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);

            }

        }
        else // node index is zero, i.e. it is the root node
        {
            inspector_ui->parent_selected_display->setText( QString::fromStdString(item_selected->getName()) );
            inspector_ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );

            /// if the root node is fixed
            if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
            {
                std::cerr << "A weldjoint is selected" << std::endl;
            }
            else if  (dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))// if the root node is not fixed
            {
                std::cerr << "Root node is free to move" << std::endl;
                ///enable position and orientation sliders
                inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
                inspector_ui->Position_Slider_GroupBox->setEnabled(true);
                inspector_ui->Orientation_Slider_GroupBox->setEnabled(true);

                Eigen::Matrix<double, 6, 1> pose = Eigen::Matrix<double, 6, 1>::Zero();
                //pose = getPoseFromTransform(item_selected->getWorldTransform());
                pose = getRootTransform(simworld->getSkeleton(treeview->getActiveItem()->skeletonID));
                //std::cerr << "Pose: " << pose << std::endl;
                inspector_ui->positionSlider_1->setdsValue(pose(0));
                inspector_ui->positionSlider_2->setdsValue(pose(1));
                inspector_ui->positionSlider_3->setdsValue(pose(2));

                inspector_ui->orientationSlider_1->setdsValue(RAD2DEG(pose(3)));
                inspector_ui->orientationSlider_2->setdsValue(RAD2DEG(pose(4)));
                inspector_ui->orientationSlider_3->setdsValue(RAD2DEG(pose(5)));

            }
            else
            {
                std::cerr << "The root joint is neither fixed nor free." << std::endl;
            }
        }


    }
    else
    {
        std::cerr << "receiveSelectedItem: No identified return type" << __LINE__ << " of " << __FILE__ << std::endl;
        inspector_ui->parent_selected_display->setText(QString("%1").arg("Not identified"));
        inspector_ui->item_selected_display->setText(QString("%1").arg("Not identified"));
    }

}

/**
 * @function initializeTab
 * @brief initialize inspector tab when a new model is loaded
 */

void Inspector_Tab::initializeTab()
{
    if (inspector_ui->positionSlider_0->getdsValue() != 0.0)
        inspector_ui->positionSlider_0->setdsValue(0.0);
    if (inspector_ui->positionSlider_1->getdsValue() != 0.0)
        inspector_ui->positionSlider_1->setdsValue(0.0);
    if (inspector_ui->positionSlider_2->getdsValue() != 0.0)
        inspector_ui->positionSlider_2->setdsValue(0.0);
    if (inspector_ui->positionSlider_3->getdsValue() != 0.0)
        inspector_ui->positionSlider_3->setdsValue(0.0);
    if (inspector_ui->orientationSlider_1->getdsValue() != 0.0)
        inspector_ui->orientationSlider_1->setdsValue(0.0);
    if (inspector_ui->orientationSlider_2->getdsValue() != 0.0)
        inspector_ui->orientationSlider_2->setdsValue(0.0);
    if (inspector_ui->orientationSlider_3->getdsValue() != 0.0)
        inspector_ui->orientationSlider_3->setdsValue(0.0);


    if (inspector_ui->Joint_Slider_GroupBox->isEnabled())
        inspector_ui->Joint_Slider_GroupBox->setDisabled(true);
    if (inspector_ui->Position_Slider_GroupBox->isEnabled())
        inspector_ui->Position_Slider_GroupBox->setDisabled(true);
    if (inspector_ui->Orientation_Slider_GroupBox->isEnabled());
        inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);
}

 /**
 * @function getRootTransform
 * @brief Return <x,y,z, r, p, y> Remember that get_q() gives you the screw so
 * do NOT use it directly
 */

Eigen::Matrix<double, 6, 1> Inspector_Tab::getRootTransform(dart::dynamics::Skeleton* robot)
{
    dart::dynamics::Joint *joint = robot->getRootBodyNode()->getParentJoint();
    Eigen::Matrix<double, 6, 1> pose;

    if(joint->getJointType() == dart::dynamics::Joint::FREE) {
        Matrix<double, 6, 1> q = joint->get_q();
        Eigen::Isometry3d Tf = dart::math::expMap( joint->get_q() );
        pose.head<3>() = Tf.translation();
        pose.tail<3>() = dart::math::matrixToEulerXYZ( Tf.linear() );
    }
    else {
        pose = getPoseFromTransform(joint->getTransformFromParentBodyNode());
    }

    return pose;
}

/**
 * @function setRootTransform
 * @brief Set q (SCREW) from pose <x,y,z,r,p,y>
 */
void Inspector_Tab::setRootTransform(dart::dynamics::Skeleton* robot, const Eigen::Matrix<double, 6, 1>& pose )
{
    dart::dynamics::Joint* joint = robot->getRootBodyNode()->getParentJoint();
    //dart::dynamics::Joint* joint = robot->getParentJoint();
    if(dynamic_cast<dart::dynamics::FreeJoint*>(joint)) {
        Matrix<double, 6, 1> q;
        Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
        transform.translation() = pose.head<3>();
        transform.linear() = dart::math::eulerXYZToMatrix(pose.tail<3>());
        q = dart::math::logMap(transform);
        joint->set_q( q );

    }
    else {
        Eigen::Isometry3d transform;
        transform.makeAffine();
        transform.linear() = dart::math::eulerXYZToMatrix(pose.tail<3>());
        transform.translation() = pose.head<3>();
        joint->setTransformFromParentBodyNode(transform);
        std::cerr << "not a free joint??" << std::endl;
    }

    for (int i = 0; i < robot->getNumBodyNodes(); ++i) {
        robot->getBodyNode(i)->updateTransform();
    }
}

/**
 * @function getPoseTransform
 * @brief Get a vector <x,y,z,r,p,y> from Transform. NO SCREW
 */
Eigen::Matrix<double, 6, 1> Inspector_Tab::getPoseFromTransform(const Eigen::Isometry3d& tf)
{
    Eigen::Matrix<double, 6, 1> pose;
    pose.head<3>() = tf.translation();
    pose.tail<3>() = dart::math::matrixToEulerXYZ(tf.linear());
    return pose;
}


