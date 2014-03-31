/*
 * Copyright (c) 2011, Georgia Tech Research Corporation
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

// C++ Standard includes
#include <iostream>

// DART includes
#include <dart/dynamics/BodyNode.h>
#include <dart/dynamics/Skeleton.h>
#include <dart/planning/Trajectory.h>

// Local includes
#include "HuboController.h"

HuboController::HuboController(dart::dynamics::Skeleton *skeleton,
                               const Eigen::VectorXd p,
                               const Eigen::VectorXd i,
                               const Eigen::VectorXd d,
                               const Eigen::VectorXd mask,
                               double tInit) {
    skel = skeleton;
    Kp = p.asDiagonal();
    Ki = i.asDiagonal();
    Kd = d.asDiagonal();
    jointMask = mask.asDiagonal();
    tLast = tInit;
    errorLast = Eigen::VectorXd::Zero(skel->getNumGenCoords());
    errorDeriv = Eigen::VectorXd::Zero(skel->getNumGenCoords());
    errorInteg = Eigen::VectorXd::Zero(skel->getNumGenCoords());
    refVel = Eigen::VectorXd::Zero(skel->getNumGenCoords());
    refPos = Eigen::VectorXd::Zero(skel->getNumGenCoords());
}

Eigen::VectorXd HuboController::getTorques(const Eigen::VectorXd &curPos,
                                           const Eigen::VectorXd &curVel,
                                           double t) {
    // update time
    double dt = t - tLast;
    tLast = t;

    // SPD controller
    // J. Tan, K. Liu, G. Turk. Stable Proportional-Derivative Controllers. IEEE Computer Graphics and Applications, Vol. 31, No. 4, pp 34-44, 2011.
    Eigen::MatrixXd M = skel->getMassMatrix() + Kd * dt;
    Eigen::MatrixXd invM = M.inverse();
    Eigen::VectorXd p = -Kp * (curPos - refPos + curVel * dt);
    Eigen::VectorXd d = -Kd * (curVel - refVel);
    Eigen::VectorXd qddot = invM * (-skel->getCombinedVector() + p + d);
    Eigen::VectorXd torques = p + d - Kd * qddot * dt;

    return jointMask * torques;
}
