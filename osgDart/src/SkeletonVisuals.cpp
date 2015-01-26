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

#include "SkeletonVisuals.h"
#include <iostream>

using namespace osgDart;

SkeletonVisuals::SkeletonVisuals() :
    _centerOfMassTF(NULL), _centerOfMass(NULL), _projectedCenterOfMassTF(NULL), _projectedCenterOfMass(NULL)
{
    _cullFace = new osg::CullFace(osg::CullFace::BACK);
}

SkeletonVisuals::~SkeletonVisuals()
{

}

void SkeletonVisuals::addCenterOfMass()
{
    _centerOfMassTF = new osg::MatrixTransform;
    _centerOfMass = new osgGolems::Sphere(osg::Vec3(0,0,0), 0.03, osg::Vec4(1,1,.8,1));
    this->_setGeodeModes(_centerOfMass);
    _centerOfMassTF->addChild(_centerOfMass);
    this->addChild(_centerOfMassTF);
}

void SkeletonVisuals::addProjectedCenterOfMass()
{
    _projectedCenterOfMassTF = new osg::MatrixTransform;
    _projectedCenterOfMass = new osgGolems::Cylinder(osg::Vec3(0,0,0), 0.03, 0.001f, osg::Vec4(1,1,.8,1));
    this->_setGeodeModes(_projectedCenterOfMass);
    _projectedCenterOfMassTF->addChild(_projectedCenterOfMass);
    this->addChild(_projectedCenterOfMassTF);
}

osg::MatrixTransform* SkeletonVisuals::getCenterOfMassTF()
{
        return _centerOfMassTF;
}

osg::MatrixTransform* SkeletonVisuals::getProjectedCenterOfMassTF()
{
        return _projectedCenterOfMassTF;
}

void SkeletonVisuals::setCenterOfMassColor(const osg::Vec4& newColor)
{
    _centerOfMass->setColor(newColor);
}

void SkeletonVisuals::setProjectedCenterOfMassColor(const osg::Vec4& newColor)
{
    _projectedCenterOfMass->setColor(newColor);
}

void SkeletonVisuals::_setGeodeModes(osg::Geode* geode)
{
    // Turn off lighting
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // Set culling mode
    geode->getOrCreateStateSet()->setAttributeAndModes(_cullFace, osg::StateAttribute::ON);
    // Turn on proper blending of transparent and opaque nodes
    geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
}
