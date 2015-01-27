/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Sungmoon Joo <sungmoon.joo@gmail.com>
 * Date: Feb 2014
 *
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of the Humanoid Robotics Lab nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

// Local includes
#include "InspectorTab.h"
#include <iostream>
#include "TreeView.h"

// DART includes
#include <dart/simulation/World.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/dynamics/Joint.h>
#include <dart/dynamics/WeldJoint.h>
#include <dart/dynamics/FreeJoint.h>
#include <dart/dynamics/RevoluteJoint.h>
#include <dart/dynamics/PrismaticJoint.h>

// Qt includes
#include <QDebug>
#include <QString>
#include <QMetaMethod>


//change units
inline double DEG2RAD(double angle_deg)  { return (angle_deg * 0.01745329251994329577); } //{ return (M_PI*angle_deg/180.0); }
inline double RAD2DEG(double angle_rad)	 { return (angle_rad * 57.2957795130823208768); } //{ return (180.0/M_PI*angle_rad); }

InspectorTab::InspectorTab(QWidget *parent, dart::simulation::World *simWorld, TreeView *treeViewer)
 : QDockWidget(parent), _ui(new Ui::InspectorTab), _simWorld(simWorld), _treeview(treeViewer), _selectedTypeFromTree(Return_Type_Robot)
{
    _ui->setupUi(this);
    int position_precision_decimal = 2;
    int orientation_precision_decimal = 2;
//    int joint_precision_decimal = 2;  /* moved to receiveSeletedItem */
    /// selected joint slider
    connect(_ui->positionSlider_0, SIGNAL(valueChanged(int)),this, SLOT(changeSelectedJoint(int)));

    /// robot position and orientation sliders
    //connect(inspector_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->positionSlider_1->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    _ui->positionSlider_1->setdsValue(0.0);
    _ui->positionSpinBox_1->setRange(-10.0,10.0);
    _ui->positionSpinBox_1->setDecimals(position_precision_decimal);
    _ui->positionSpinBox_1->setSingleStep(pow(10,-position_precision_decimal));


    //connect(inspector_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->positionSlider_2->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    _ui->positionSlider_2->setdsValue(0.0);
    _ui->positionSpinBox_2->setRange(-10.0,10.0);
    _ui->positionSpinBox_2->setDecimals(position_precision_decimal);
    _ui->positionSpinBox_2->setSingleStep(pow(10,-position_precision_decimal));

 //   connect(inspector_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->positionSlider_3->setMinMaxDecimalValue(-10.0,10.0,position_precision_decimal);
    _ui->positionSlider_3->setdsValue(0.0);
    _ui->positionSpinBox_3->setRange(-10.0,10.0);
    _ui->positionSpinBox_3->setDecimals(position_precision_decimal);
    _ui->positionSpinBox_3->setSingleStep(pow(10,-position_precision_decimal));

//    connect(inspector_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->orientationSlider_1->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    _ui->orientationSlider_1->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_1->setRange(-180.0,180.0);
    _ui->orientationSpinBox_1->setDecimals(orientation_precision_decimal);
    _ui->orientationSpinBox_1->setSingleStep(pow(10,-orientation_precision_decimal));

//    connect(inspector_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->orientationSlider_2->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    _ui->orientationSlider_2->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_2->setRange(-180.0,180.0);
    _ui->orientationSpinBox_2->setDecimals(position_precision_decimal);
    _ui->orientationSpinBox_2->setSingleStep(pow(10,-orientation_precision_decimal));

//    connect(inspector_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    _ui->orientationSlider_3->setMinMaxDecimalValue(-180.0,180.0,position_precision_decimal);
    _ui->orientationSlider_3->setdsValue(0.0);
//    inspector_ui->orientationSpinBox_3->setRange(-180.0,180.0);
    _ui->orientationSpinBox_3->setDecimals(position_precision_decimal);
    _ui->orientationSpinBox_3->setSingleStep(pow(10,-orientation_precision_decimal));

    connect(_treeview, SIGNAL(itemSelected(TreeViewReturn*)),this, SLOT(receiveSeletedItem(TreeViewReturn*)));
    _ui->Joint_Slider_GroupBox->setDisabled(true);
    _ui->Position_Slider_GroupBox->setDisabled(true);
    _ui->Orientation_Slider_GroupBox->setDisabled(true);

    connect(_ui->positionSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(_ui->positionSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(_ui->positionSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(_ui->orientationSlider_1, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(_ui->orientationSlider_2, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
    connect(_ui->orientationSlider_3, SIGNAL(valueChanged(int)),this, SLOT(changePositionAndOrientation(int)));
}

/**
 * \brief change position and orientation of the root node
 */
void InspectorTab::changePositionAndOrientation(int sliderValue){

//  QMetaMethod metaMethod = sender()->metaObject()->method(senderSignalIndex());
    Eigen::Matrix<double, 6, 1> pose;
    pose << 0, 0, 0, 0, 0, 0;

    /// if world is defined
    if(_simWorld)
    {
        _selectedTypeFromTree = _treeview->getActiveItem()->dType;

        if (_selectedTypeFromTree == Return_Type_Robot) //if robot, do nothing
        {
             //dart::dynamics::Skeleton* item_selected;
             //item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "ChangeXPosition: Skeleton itself is selected. Not movable." << std::endl;

        }
        else if (_selectedTypeFromTree == Return_Type_Node) //if bodynode, change configuration using slider
        {
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)_treeview->getActiveItem()->object;

             /// check if the joint is a free joint (6DoF)
             if(dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))
             {
                 if (item_selected->getSkeletonIndex() == 0 && item_selected->getParentJoint()->getNumGenCoords() == 6 ) /// double check, if the node is the root and free
                 {
                       pose(0) = _ui->positionSlider_1->getdsValue();
                       pose(1) = _ui->positionSlider_2->getdsValue();
                       pose(2) = _ui->positionSlider_3->getdsValue();
                       pose(3) = DEG2RAD(_ui->orientationSlider_1->getdsValue());
                       pose(4) = DEG2RAD(_ui->orientationSlider_2->getdsValue());
                       pose(5) = DEG2RAD(_ui->orientationSlider_3->getdsValue());

                       setRootTransform(_simWorld->getSkeleton(_treeview->getActiveItem()->skeletonId), pose);
                 }

            }
            else
            {
                 std::cerr << "Selected joint is not a free joint" << std::endl;
            }
        }
        else
        {
            std::cerr << "changePositionAndOrientation: No return type identified" << _treeview->getActiveItem()->dType << std::endl;
        }

    }
    else
    {
        std::cerr << "Change Position & Orientation: either simworld is empty" << __LINE__ << " of " << __FILE__ << std::endl;
    }
}

/**
 * \brief move the selected joint(other than root joint) only
 */

void InspectorTab::changeSelectedJoint(int sliderValue){

    if(_simWorld) {

        _selectedTypeFromTree = _treeview->getActiveItem()->dType;

        if (_selectedTypeFromTree == Return_Type_Robot) //if robot, do nothing
        {
             //dart::dynamics::Skeleton* item_selected;
             //item_selected = (dart::dynamics::Skeleton*)treeview->getActiveItem()->object;
             std::cerr << "Robot is selected" << std::endl;

        }
        else if (_selectedTypeFromTree == Return_Type_Node) //if bodynode, change configuration using slider
        {
             dart::dynamics::BodyNode* item_selected;
             item_selected = (dart::dynamics::BodyNode*)_treeview->getActiveItem()->object;

             if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint())) //if the joint is fixed
             {
                     std::cerr << "A weldjoint is selected" << std::endl;
             }
             else
             {
                     if (item_selected->getSkeletonIndex() !=0) /// double check, if the node is not the root
                     {
                         std::vector<int> indx;
                         indx.push_back( _simWorld->getSkeleton(_treeview->getActiveItem()->skeletonId)->getJoint(item_selected->getParentJoint()->getName())->getGenCoord(0)->getSkeletonIndex() );
                         Eigen::VectorXd config(1);
                         config[0] = DEG2RAD(_ui->positionSlider_0->getdsValue());
                         _simWorld->getSkeleton(_treeview->getActiveItem()->skeletonId)->setConfig(indx, config); //getSkeleton(i) - choose ith object

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
 * \brief deconstructor of the class Inspector_Tab
 */

InspectorTab::~InspectorTab()
{
}

/**
 * \brief identify type of selected item from treeview and set the sliders properly
 */
void InspectorTab::receiveSeletedItem(TreeViewReturn* active_item)
{

    _selectedTypeFromTree = active_item->dType;

    if (_selectedTypeFromTree == Return_Type_Robot) //if Robot, active_item->object = *skel
    {
        dart::dynamics::Skeleton* item_selected;
        item_selected = (dart::dynamics::Skeleton*)active_item->object;

        std::cerr << "ReceiveSelectedItem: Robot is seleted" << std::endl;

        _ui->parent_selected_display->setText(QString::fromStdString(item_selected->getName()));
        _ui->item_selected_display->setText(QString::fromStdString(item_selected->getName()));
        _ui->Joint_Slider_GroupBox->setDisabled(true);
        _ui->Position_Slider_GroupBox->setDisabled(true);
        _ui->Orientation_Slider_GroupBox->setDisabled(true);
    }
    else if (_selectedTypeFromTree == Return_Type_Node) //if Node, active_item->object = *node
    {
        dart::dynamics::BodyNode* item_selected;
        item_selected = (dart::dynamics::BodyNode*)active_item->object;

        //std::cerr << "ReceiveSelectedItem: BodyNode is selected" << std::endl;

        if (item_selected->getSkeletonIndex() != 0) //double check, if the selected node's index is not zero
        {
            _ui->parent_selected_display->setText( QString::fromStdString(item_selected->getParentBodyNode()->getName()) );
            _ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );
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
                _ui->positionSlider_0->setMinMaxDecimalValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),
                                                                      RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()),joint_precision_decimal);
                _ui->positionSpinBox_0->setRange(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMin()),RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_qMax()));
                _ui->positionSpinBox_0->setDecimals(joint_precision_decimal);
                _ui->positionSpinBox_0->setSingleStep(pow(10,-joint_precision_decimal));

                _ui->positionSlider_0->setdsValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
                //inspector_ui->positionSpinBox_0->setdsValue(RAD2DEG(item_selected->getParentJoint()->getGenCoord(0)->get_q()));
                ///enable joint slider only
                _ui->Joint_Slider_GroupBox->setEnabled(true);
//                inspector_ui->Position_Slider_GroupBox->setDisabled(true);
//               inspector_ui->Orientation_Slider_GroupBox->setDisabled(true);

                Eigen::Matrix<double, 6, 1> pose = Eigen::Matrix<double, 6, 1>::Zero();
                pose = getPoseFromTransform(item_selected->getWorldTransform());
                _ui->positionSlider_1->setdsValue(pose(0));
                _ui->positionSlider_2->setdsValue(pose(1));
                _ui->positionSlider_3->setdsValue(pose(2));

                _ui->orientationSlider_1->setdsValue(RAD2DEG(pose(3)));
                _ui->orientationSlider_2->setdsValue(RAD2DEG(pose(4)));
                _ui->orientationSlider_3->setdsValue(RAD2DEG(pose(5)));
                _ui->Position_Slider_GroupBox->setDisabled(true);
                _ui->Orientation_Slider_GroupBox->setDisabled(true);

            }

        }
        else // node index is zero, i.e. it is the root node
        {
            _ui->parent_selected_display->setText( QString::fromStdString(item_selected->getName()) );
            _ui->item_selected_display->setText( QString::fromStdString(item_selected->getName()) );

            /// if the root node is fixed
            if (dynamic_cast<dart::dynamics::WeldJoint*>(item_selected->getParentJoint()))
            {
                std::cerr << "A weldjoint is selected" << std::endl;
            }
            else if  (dynamic_cast<dart::dynamics::FreeJoint*>(item_selected->getParentJoint()))// if the root node is not fixed
            {
                std::cerr << "Root node is free to move" << std::endl;
                ///enable position and orientation sliders
                _ui->Joint_Slider_GroupBox->setDisabled(true);
                _ui->Position_Slider_GroupBox->setEnabled(true);
                _ui->Orientation_Slider_GroupBox->setEnabled(true);

                Eigen::Matrix<double, 6, 1> pose = Eigen::Matrix<double, 6, 1>::Zero();
                //pose = getPoseFromTransform(item_selected->getWorldTransform());
                pose = getRootTransform(_simWorld->getSkeleton(_treeview->getActiveItem()->skeletonId));
                //std::cerr << "Pose: " << pose << std::endl;
                _ui->positionSlider_1->setdsValue(pose(0));
                _ui->positionSlider_2->setdsValue(pose(1));
                _ui->positionSlider_3->setdsValue(pose(2));

                _ui->orientationSlider_1->setdsValue(RAD2DEG(pose(3)));
                _ui->orientationSlider_2->setdsValue(RAD2DEG(pose(4)));
                _ui->orientationSlider_3->setdsValue(RAD2DEG(pose(5)));

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
        _ui->parent_selected_display->setText(QString("%1").arg("Not identified"));
        _ui->item_selected_display->setText(QString("%1").arg("Not identified"));
    }

}

/**
 * \brief initialize inspector tab when a new model is loaded
 */

void InspectorTab::initializeTab()
{
    if (_ui->positionSlider_0->getdsValue() != 0.0)
        _ui->positionSlider_0->setdsValue(0.0);
    if (_ui->positionSlider_1->getdsValue() != 0.0)
        _ui->positionSlider_1->setdsValue(0.0);
    if (_ui->positionSlider_2->getdsValue() != 0.0)
        _ui->positionSlider_2->setdsValue(0.0);
    if (_ui->positionSlider_3->getdsValue() != 0.0)
        _ui->positionSlider_3->setdsValue(0.0);
    if (_ui->orientationSlider_1->getdsValue() != 0.0)
        _ui->orientationSlider_1->setdsValue(0.0);
    if (_ui->orientationSlider_2->getdsValue() != 0.0)
        _ui->orientationSlider_2->setdsValue(0.0);
    if (_ui->orientationSlider_3->getdsValue() != 0.0)
        _ui->orientationSlider_3->setdsValue(0.0);


    if (_ui->Joint_Slider_GroupBox->isEnabled())
        _ui->Joint_Slider_GroupBox->setDisabled(true);
    if (_ui->Position_Slider_GroupBox->isEnabled())
        _ui->Position_Slider_GroupBox->setDisabled(true);
    if (_ui->Orientation_Slider_GroupBox->isEnabled());
        _ui->Orientation_Slider_GroupBox->setDisabled(true);
}

 /**
 * \brief Return <x,y,z, r, p, y> Remember that get_q() gives you the screw so
 * do NOT use it directly
 */

Eigen::Matrix<double, 6, 1> InspectorTab::getRootTransform(dart::dynamics::Skeleton* robot)
{
    dart::dynamics::Joint *joint = robot->getRootBodyNode()->getParentJoint();
    Eigen::Matrix<double, 6, 1> pose;

    if (joint->getJointType() == dart::dynamics::Joint::FREE) {
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
 * \brief Set q (SCREW) from pose <x,y,z,r,p,y>
 */
void InspectorTab::setRootTransform(dart::dynamics::Skeleton* robot, const Eigen::Matrix<double, 6, 1>& pose )
{
    dart::dynamics::Joint* joint = robot->getRootBodyNode()->getParentJoint();
    //dart::dynamics::Joint* joint = robot->getParentJoint();
    if (dynamic_cast<dart::dynamics::FreeJoint*>(joint)) {
        Eigen::Matrix<double, 6, 1> q;
        Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
        transform.translation() = pose.head<3>();
        transform.linear() = dart::math::eulerXYZToMatrix(pose.tail<3>());
        q = dart::math::logMap(transform);
        joint->set_q( q );

        std::cout << "pose: " << pose.transpose() << std::endl;
        std::cout << "q: " << q.transpose() << std::endl;
        // std::cout << "robot state: " << robot->getState().transpose() << std::endl;

        Eigen::Isometry3d T = dart::math::expMap(q);
        std::cout << "q as pose from transform: " << getPoseFromTransform(T).transpose() << std::endl;

        Eigen::Matrix3d R = dart::math::expMapRot(q.topLeftCorner(3,1));
        std::cout << "q rotation part as euler XYZ : " << dart::math::matrixToEulerXYZ(R).transpose() << std::endl;

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
 * \brief Get a vector <x,y,z,r,p,y> from Transform. NO SCREW
 */
Eigen::Matrix<double, 6, 1> InspectorTab::getPoseFromTransform(const Eigen::Isometry3d& tf)
{
    Eigen::Matrix<double, 6, 1> pose;
    pose.head<3>() = tf.translation();
    pose.tail<3>() = dart::math::matrixToEulerXYZ(tf.linear());
    return pose;
}


