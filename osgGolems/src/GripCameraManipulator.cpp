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
#include "GripCameraManipulator.h"
#include <iostream>

using namespace osgGolems;

GripCameraManipulator::GripCameraManipulator()
{
    this->setAllowThrow(false);
    /// initialize member variables
    _keyPressed = false; // no key is pressed, initially
    _pressedKey = 0;
    _currentX = 0.0;
    _currentY = 0.0;
    _previousX = 0.0;
    _previousY = 0.0;
    _keyboardMouseBinding = false;
}

GripCameraManipulator::~GripCameraManipulator()
{

}

void GripCameraManipulator::setCenter(osg::Vec3 center)
{
    osgGA::OrbitManipulator::setCenter(center);
}

/// Handles keyboard and mouse event for camera manipulation
bool GripCameraManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& as )
{
    float dx = 0.03f * _distance;
    float dy = 0.03f * _distance;
    float drotate = 0.01f;
    float zoomScale = 0.05;

    switch(ea.getEventType()) {
        case(osgGA::GUIEventAdapter::KEYDOWN): {
            if (_keyPressed == false) {
                _keyPressed = true;
//                std::cerr << "KEYDOWN is detected" << std::endl;
            }
            else {
                std::cerr << "KEYDOWN is detected but keyPressed is true. Something is wrong" << std::endl;
                _keyPressed = true;
//              return false;
            }

            switch(ea.getKey()) {
                case (osgGA::GUIEventAdapter::KEY_Equals): {
//                    std::cout << "'+' key pressed: zoom in" << std::endl;
                    zoomModel( -zoomScale, true );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Equals;
                    return true;
                    break;
                }
                case (osgGA::GUIEventAdapter::KEY_Minus): {
//                    std::cout << "'-' key pressed: zoom out" << std::endl;
                    zoomModel( zoomScale, true );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Minus;
                    return true;
                    break;
                }
                case (osgGA::GUIEventAdapter::KEY_Right): {
//                    std::cout << "'->' key pressed: pan right" << std::endl;
//                    float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
                    panModel(dx,0.0);
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Right;
                    return true;
                    break;
                }
                case (osgGA::GUIEventAdapter::KEY_Left): {
//                    std::cout << "'<-' key pressed: pan right" << std::endl;
                    panModel(-dx,0.0);
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Left;
                    return true;
                    break;
                }
                case (osgGA::GUIEventAdapter::KEY_Up): {
//                    std::cout << "'Up' key pressed: pan up" << std::endl;
                    panModel(0.0,dy);
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Up;
                    return true;
                    break;
                }
                case (osgGA::GUIEventAdapter::KEY_Down): {
//                    std::cout << "'Down' key pressed: pan up" << std::endl;
                    panModel(0.0,-dy);
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Down;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_H: {
//                    std::cout << "'H' key pressed. Rotate to left." << std::endl;
                    rotateWithFixedVertical( drotate, 0.0 );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_H;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_L: {
//                    std::cout << "'L' key pressed. Rotate to right." << std::endl;
                    rotateWithFixedVertical( -drotate, 0.0 );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_L;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_J: {
//                    std::cout << "'J' key pressed. Rotate up." << std::endl;
                    rotateWithFixedVertical( 0.0, -drotate );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_J;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_K: {
//                    std::cout << "'K' key pressed. Rotate down." << std::endl;
                    rotateWithFixedVertical( 0.0, drotate );
                    _pressedKey = osgGA::GUIEventAdapter::KEY_K;

                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_Control_L: {
//                    std::cout << "'Control_Left' key pressed. Zoom with mouse." << std::endl;
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Control_L;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_Shift_L: {
//                    std::cout << "'Shift_Left' key pressed. Move with mouse." << std::endl;
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Shift_L;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_Alt_L: {
//                    std::cout << "'Alt_Left' key pressed. Rotattion with mouse." << std::endl;
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Alt_L;
                    return true;
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_Space : {
                    flushMouseEventStack();
                    _thrown = false;
                    home(ea,as);
//                    std::cerr << "Space key pressed" << std::endl;
                    _pressedKey = osgGA::GUIEventAdapter::KEY_Space;
                    return true;
                    break;
                }
                default: {
                    std::cerr << "Key pressed but no predefined binding is found." << std::endl;
                    _pressedKey = 0;
                    return true;
                    //break;
                }
            }// end of switch(ea.getKey())
            break;
        } // case(osgGA::GUIEventAdapter::KEYDOWN)
        default: {
            return false;
            //break;
        }
    } ///end of switch(ea.getEventType())

   return false;
}


/// Handles keyboard and mouse event for camera manipulation
bool GripCameraManipulator::handleKeyUp( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& as )
{
    switch(ea.getEventType()) {
        case(osgGA::GUIEventAdapter::KEYUP): {
            if (_keyPressed == true) {
                _keyPressed = false;
                _pressedKey = 0;
//                std::cerr << "KEYUP event detected" << std::endl;
            }
            else {
                _keyPressed = false;
                _pressedKey = 0;
                std::cerr << "KEYUP is detected but keyPressed is false. Something is wrong" << std::endl;
            }
            return true;
            break;
        } // case(osgGA::GUIEventAdapter::KEYUP)
        default: {
            return false;
            //break;
        }
    } ///end of switch(ea.getEventType())

   return false;
}


bool GripCameraManipulator::handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& as )
{
    switch(ea.getEventType()) {
        case(osgGA::GUIEventAdapter::MOVE): {
            _currentX = ea.getX();
            _currentY = ea.getY();

            if (!_keyboardMouseBinding) {
                _previousX = _currentX;
                _previousY = _currentY;
                _keyboardMouseBinding = true; //
            }

            float dx = _currentX - _previousX;
            float dy = _currentY - _previousY;
//            std::cerr << "Mouse movement is detected." << std::endl;
            if (_keyPressed == true && _pressedKey == osgGA::GUIEventAdapter::KEY_Control_L) {
//                std::cout << "'Control_L + Mouse move'" << std::endl;
                float scale = 0.001;
                zoomModel( dy*scale , true );

            }
            else if (_keyPressed == true && _pressedKey == osgGA::GUIEventAdapter::KEY_Shift_L) {
//                std::cout << "'Shift_L + Mouse move'" << std::endl;
                float scale = -0.0005 * _distance ;
                panModel( dx*scale, dy*scale );

            }
            else if (_keyPressed == true && _pressedKey == osgGA::GUIEventAdapter::KEY_Alt_L) {
//                std::cout << "'Alt_L + Mouse move'" << std::endl;
                float scale = 0.001;
                rotateWithFixedVertical( dx*scale, dy*scale );

            }
            else {
//                std::cout << "'No defined keyboard binding with mouse movement'" << std::endl;
                _keyboardMouseBinding = false;
            }
            _previousX = _currentX;
            _previousY = _currentY;
            return true;
            break;
        } // case(osgGA::GUIEventAdapter::MOVE)
        default: {
            return false;
            //break;
        }
    } ///end of switch(ea.getEventType())

   return false;
}



