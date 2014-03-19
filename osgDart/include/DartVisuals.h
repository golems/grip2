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

#ifndef DARTVISUAL_H
#define DARTVISUAL_H

#include <osg/MatrixTransform>
#include <osg/CullFace>
#include "Axes.h"
#include "Line.h"

namespace osgDart {

class DartVisuals : public osg::MatrixTransform
{
public:
    DartVisuals();
    ~DartVisuals();

    void addJointAxis();
    void addBodyNodesAxes();
    void addCenterOfMass();

    osg::MatrixTransform* getJointAxisTF();
    osg::MatrixTransform* getBodyNodeAxesTF();
    void setJointAxisColor(const osg::Vec4 &color);
    void setBodyNodeAxesColors(const osg::Vec4& xAxis, const osg::Vec4& yAxis, const osg::Vec4& zAxis);

    void setLineWidth(float lineWidth);


protected:
    osg::Geode* _makeJointAxis();
    osg::Geode* _makeBodyNodeAxes();

    void _setGeodeModes(osg::Geode* geode);

    osg::MatrixTransform* _jointAxisTF;
    osg::MatrixTransform* _bodyNodeAxesTF;

    osgGolems::Line* _jointAxis;
    osgGolems::Axes* _bodyNodeAxes;

    osg::LineWidth* _lineWidth;
    osg::CullFace* _cullFace;
};

}

#endif // DARTVISUAL_H


/// Joint level
// Joint axes
// BodyNode frames
// Center of mass
// Projected center of mass
// Collision mesh

/// Skeleton level
// Center of mass
// Project center of mass
// Contact forces
