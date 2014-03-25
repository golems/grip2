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

#include "BodyNodeVisuals.h"
#include <iostream>

using namespace osgDart;

BodyNodeVisuals::BodyNodeVisuals() :
    _jointAxisTF(NULL), _jointAxis(NULL), _bodyNodeAxesTF(NULL), _bodyNodeAxes(NULL)
{
    _cullFace = new osg::CullFace(osg::CullFace::BACK);
    _lineWidth = new osg::LineWidth(3.0);
}

BodyNodeVisuals::~BodyNodeVisuals()
{

}

void BodyNodeVisuals::addJointAxis()
{
    _jointAxisTF = new osg::MatrixTransform;
    _jointAxisTF->addChild(_makeJointAxis());
    this->addChild(_jointAxisTF);
}

void BodyNodeVisuals::addBodyNodesAxes()
{
    _bodyNodeAxesTF = new osg::MatrixTransform;
    _bodyNodeAxesTF->addChild(_makeBodyNodeAxes());
    this->addChild(_bodyNodeAxesTF);
}

void BodyNodeVisuals::addCenterOfMass()
{

}

osg::MatrixTransform* BodyNodeVisuals::getJointAxisTF()
{
        return _jointAxisTF;
}

osg::MatrixTransform* BodyNodeVisuals::getBodyNodeAxesTF()
{
        return _bodyNodeAxesTF;
}

void BodyNodeVisuals::setJointAxisColor(const osg::Vec4& color)
{
    _jointAxis->setColor(color);
}

void BodyNodeVisuals::setBodyNodeAxesColors(const osg::Vec4& xAxis, const osg::Vec4& yAxis, const osg::Vec4& zAxis)
{
    _bodyNodeAxes->setColors(xAxis, yAxis, zAxis);
}

void BodyNodeVisuals::setLineWidth(float lineWidth)
{
    _lineWidth->setWidth(lineWidth);
    for (int i=0; i<this->getNumChildren(); ++i) {
        this->getChild(i)->getOrCreateStateSet()->setAttribute(_lineWidth);
    }
}

osg::Geode* BodyNodeVisuals::_makeJointAxis()
{
    _jointAxis = new osgGolems::Line(osgGolems::LINE_ENDING_WITH_ARROW, .1);
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(_jointAxis);
    _setGeodeModes(geode);
    return geode;
}

osg::Geode* BodyNodeVisuals::_makeBodyNodeAxes()
{
    _bodyNodeAxes = new osgGolems::Axes(.1);
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(_bodyNodeAxes);
    _setGeodeModes(geode);
    return geode;
}

void BodyNodeVisuals::_setGeodeModes(osg::Geode* geode)
{
    // Turn off lighting
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    // Set line width of axis and axes
    geode->getOrCreateStateSet()->setAttributeAndModes(_lineWidth);
    // Set culling mode
//    geode->getOrCreateStateSet()->setAttributeAndModes(_cullFace, osg::StateAttribute::ON);
    // Turn on proper blending of transparent and opaque nodes
    geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
}
