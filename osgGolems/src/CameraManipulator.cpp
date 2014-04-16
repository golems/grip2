/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
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

#include "CameraManipulator.h"

using namespace osgGolems;

CameraManipulator::CameraManipulator()
{
    this->setAllowThrow(false);
}

CameraManipulator::~CameraManipulator()
{

}

void CameraManipulator::setCenter(osg::Vec3 center)
{
    osgGA::OrbitManipulator::setCenter(center);
}


bool CameraManipulator::handleMouseWheel(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    osgGA::GUIEventAdapter::ScrollingMotion scrollMotion = ea.getScrollingMotion();

    // handle centering
    if (_flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT)
    {

        if (((scrollMotion == osgGA::GUIEventAdapter::SCROLL_DOWN && _wheelZoomFactor > 0.))
            || ((scrollMotion == osgGA::GUIEventAdapter::SCROLL_UP && _wheelZoomFactor < 0.)))
        {
            if (getAnimationTime() <= 0.) {
                // center by mouse intersection (no animation)
                setCenterByMousePointerIntersection(ea, us);
            } else {
                // start new animation only if there is no animation in progress
                if (!isAnimating()) {
                    startAnimationByMousePointerIntersection(ea, us);
                }
            }
        }
    }

    switch (scrollMotion)
    {
        // mouse scroll up event
        case osgGA::GUIEventAdapter::SCROLL_DOWN: {
            // perform zoom
            zoomModel(_wheelZoomFactor, true);
            us.requestRedraw();
            us.requestContinuousUpdate(isAnimating() || _thrown);
            return true;
        }

        // mouse scroll down event
        case osgGA::GUIEventAdapter::SCROLL_UP: {
            // perform zoom
            zoomModel(-_wheelZoomFactor, true);
            us.requestRedraw();
            us.requestContinuousUpdate(isAnimating() || _thrown);
            return true;
        }
        // unhandled mouse scrolling motion
        default:
            return false;
   }
}
