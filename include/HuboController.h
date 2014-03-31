/*
 * Copyright (c) 2013, Georgia Tech Research Corporation
 * 
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the Georgia Tech Research Corporation nor
 *       the names of its contributors may be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEORGIA TECH RESEARCH CORPORATION ''AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GEORGIA
 * TECH RESEARCH CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file HuboController.h
 * \brief Controller wrapper for Hubo position commands
 * \author T. Kunz
 * \author Saul Reynolds-Haertle
 */

#ifndef HUBO_CONTROLLER_H
#define HUBO_CONTROLLER_H

// C++ Stanadard includes
#include <vector>

// Eigen includes
#include <Eigen/Core>

// DART includes
#include <dynamics/Skeleton.h>

/**
 * \class HuboController HuboController.h
 * \brief Controller wrapper for Hubo position commands
 */
class HuboController {
public:
    /**
     * \brief Constructs a HuboController object
     * \param skeleton Skeleton to control
     * \param p Vector of proportional gains
     * \param i Vector of integral gains
     * \param d Vector of derivative gains
     * \param mask Mask for skeleton
     * \param tInit Initialization time
     */
    HuboController(dart::dynamics::Skeleton *skeleton,
                   const Eigen::VectorXd p,
                   const Eigen::VectorXd i,
                   const Eigen::VectorXd d,
                   const Eigen::VectorXd mask,
                   double tInit);

    /**
     * \brief Destructs a HuboController object. Virtual
     */
    virtual ~HuboController() {};

    /**
     * \brief Gets the torques for the skeleton given desired
     * positions and velocities for the joints
     * \param curPos Current position of the joints
     * \param curVel Current velocity of the joints
     * \param t Time
     * \return Eigen::VectorXd containing the joint torques
     */
    Eigen::VectorXd getTorques(const Eigen::VectorXd &curPos,
                               const Eigen::VectorXd &curVel,
                               double t);

    Eigen::VectorXd refPos;
    Eigen::VectorXd refVel;
    Eigen::MatrixXd Kp;
    Eigen::MatrixXd Kd;
    Eigen::MatrixXd Ki;

    Eigen::VectorXd errorLast;
    Eigen::VectorXd errorDeriv;
    Eigen::VectorXd errorInteg;

    Eigen::MatrixXd jointMask;

    double tLast;

    dart::dynamics::Skeleton* skel;
};

#endif // HUBO_CONTROLLER_H
