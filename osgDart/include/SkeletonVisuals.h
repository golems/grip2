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

#ifndef SKELETON_VISUALS_H
#define SKELETON_VISUALS_H

#include <osg/MatrixTransform>
#include <osg/CullFace>
#include "Shapes.h"

namespace osgDart {

class SkeletonVisuals : public osg::MatrixTransform
{
public:
    SkeletonVisuals();
    ~SkeletonVisuals();

    void addCenterOfMass();
    void addProjectedCenterOfMass();

    osg::MatrixTransform* getCenterOfMassTF();
    osg::MatrixTransform* getProjectedCenterOfMassTF();
    void setCenterOfMassColor(const osg::Vec4 &newColor);
    void setProjectedCenterOfMassColor(const osg::Vec4& newColor);

protected:

    void _setGeodeModes(osg::Geode* geode);

    osg::MatrixTransform* _centerOfMassTF;
    osg::MatrixTransform* _projectedCenterOfMassTF;

    osgGolems::Sphere* _centerOfMass;
    osgGolems::Cylinder* _projectedCenterOfMass;

    osg::CullFace* _cullFace;
};

}

#endif // SKELETON_VISUALS_H

/// Skeleton level
// Center of mass
// Project center of mass
// Contact forces
