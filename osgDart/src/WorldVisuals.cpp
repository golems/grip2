/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
 *
 * Humanoid skeletonics Lab      Georgia Institute of Technology
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
#include "Line.h"
#include "WorldVisuals.h"
#include "osgUtils.h"

using namespace osgDart;

ContactForceVisual::ContactForceVisual(bool debug)
    : _debug(debug),
      _contactForceLine(new osgGolems::Line(osgGolems::LINE_ENDING_WITH_ARROW))
{

}

ContactForceVisual::~ContactForceVisual()
{

}

void ContactForceVisual::createForceVector(float lineLength,
                                           const Eigen::Vector3d &contactPoint,
                                           const Eigen::Vector3d &contactForce)
{
    // Create new line, geode and TF to put the geode and line in
    _contactForceLine->setLength(lineLength);
    osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode;

    osg::Matrix m = _convertVectorToOsgTF(contactPoint, contactForce);
    this->setMatrix(m);

    for(int r=0; r<4; ++r) {
        for(int c=0; c<4; ++c) {
            assert(m(r,c) == m(r,c));
        }
    }

    lineGeode->addDrawable(_contactForceLine);
    this->addChild(lineGeode);
}

void ContactForceVisual::update(float lineLength,
                                const Eigen::Vector3d &contactPoint,
                                const Eigen::Vector3d &contactForce)
{
    // update length of line
    _contactForceLine->setLength(lineLength);

    // update transform of line
    this->setMatrix(_convertVectorToOsgTF(contactPoint, contactForce));
}

osg::Matrix ContactForceVisual::_convertVectorToOsgTF(const Eigen::Vector3d &point,
                                                      const Eigen::Vector3d &vector)
{
    Eigen::Isometry3d forceTF = Eigen::Isometry3d::Identity();

    // Translate to contact point
    forceTF.translate(point);

    // Rotate
    Eigen::Quaterniond forceQuat;
    if (vector.norm() < 1e-3) {
        forceQuat = Eigen::Quaterniond::Identity();
    } else {
        forceQuat.setFromTwoVectors(Eigen::Vector3d(1,0,0), vector);
        forceTF.rotate(forceQuat);
    }

    for(int r=0; r<4; ++r) {
        for(int c=0; c<4; ++c) {
            assert(forceTF(r,c) == forceTF(r,c));
        }
    }

    return osgGolems::eigToOsgMatrix(forceTF);
}
