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
#include <osgGA/OrbitManipulator>
#include "GRIPCameraManipulator.h"
#include <iostream>

using namespace osgGolems;

GRIPCameraManipulator::GRIPCameraManipulator()
{
    this->setAllowThrow(false);
}

GRIPCameraManipulator::~GRIPCameraManipulator()
{

}

void GRIPCameraManipulator::setCenter(osg::Vec3 center)
{
    osgGA::OrbitManipulator::setCenter(center);
}

/// Handles GUIEventAdapter::KEYDOWN event.
bool GRIPCameraManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& as )
{
   float dx = 0.03f * _distance;
   float dy = 0.03f * _distance;
   float drotate = 0.01f;
   float zoomScale = 0.05;

   switch(ea.getEventType())
   {
   case(osgGA::GUIEventAdapter::KEYDOWN):
      {
         switch(ea.getKey())
         {
         case (osgGA::GUIEventAdapter::KEY_Equals):
            //std::cout << "'+' key pressed: zoom in" << std::endl;
            // zoom model
            zoomModel( -zoomScale, true );
            //zoomModel( dy * getThrowScale( eventTimeDelta ), true );
             return true;

            break;

         case (osgGA::GUIEventAdapter::KEY_Minus):
            //std::cout << "'-' key pressed: zoom out" << std::endl;
            zoomModel( zoomScale, true );
            //zoomModel( dy * getThrowScale( eventTimeDelta ), true );
             return true;
            break;

         case (osgGA::GUIEventAdapter::KEY_Right):
            //std::cout << "'->' key pressed: pan right" << std::endl;
            //float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
            panModel(dx,0.0);

            return true;
            break;

         case (osgGA::GUIEventAdapter::KEY_Left):
            //std::cout << "'<-' key pressed: pan right" << std::endl;
            panModel(-dx,0.0);

            return true;
            break;

         case (osgGA::GUIEventAdapter::KEY_Up):
            //std::cout << "'Up' key pressed: pan up" << std::endl;
            panModel(0.0,dy);

            return true;
            break;

         case (osgGA::GUIEventAdapter::KEY_Down):
            //std::cout << "'Down' key pressed: pan up" << std::endl;
            panModel(0.0,-dy);

            return true;
            break;

         case osgGA::GUIEventAdapter::KEY_H:
            //std::cout << "'H' key pressed. Rotate to left." << std::endl;
            rotateWithFixedVertical( drotate, 0.0 );
            return true;
            break;

         case osgGA::GUIEventAdapter::KEY_L:
            //std::cout << "'L' key pressed. Rotate to right." << std::endl;
            rotateWithFixedVertical( -drotate, 0.0 );
            return true;
            break;

         case osgGA::GUIEventAdapter::KEY_J:
            //std::cout << "'J' key pressed. Rotate up." << std::endl;
            rotateWithFixedVertical( 0.0, -drotate );
            return true;
            break;

         case osgGA::GUIEventAdapter::KEY_K:
            //std::cout << "'K' key pressed. Rotate down." << std::endl;
            // rotate camera
            rotateWithFixedVertical( 0.0, drotate );

            return true;
            break;

         case osgGA::GUIEventAdapter::KEY_Space :
            flushMouseEventStack();
            _thrown = false;
            home(ea,as);
             std::cerr << "Space key pressed" << std::endl;
            return true;
            break;

         default:
            return false;
         }
      }

   default:
      return false;
   }

   return false;
}
