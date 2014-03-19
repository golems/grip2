/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael X. Grey <mxgrey@gatech.edu>
 * Date: Jan 2014
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

/**
 * \file Line.h
 * \brief Class that subclasses osg::Geode to create x,y,z axes
 */

#ifndef LINE_H
#define LINE_H

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osg/LineWidth>
#include <osg/Matrix>
#include <osg/MatrixTransform>

namespace osgGolems {

typedef enum lineType
{
    LINE = 0,
    LINE_ENDING_WITH_ARROW,
    LINE_WITH_ARROWS,
    NUM_LINE_TYPES
} lineType_t;

class Line : public osg::Geometry
{
public:
    inline Line(lineType_t lineType, float length = 0.5f, float width = 3) :
        _lineType(lineType)
    {
        switch(lineType) {
            case LINE_ENDING_WITH_ARROW: _verts = new osg::Vec3Array(5); break;
            case LINE_WITH_ARROWS: _verts = new osg::Vec3Array(8); break;
            case LINE:
            default: _verts = new osg::Vec3Array(2); break;
        }

        _color = new osg::Vec4Array;
        _lineWidth = new osg::LineWidth(width);

        this->_createLine();
        this->setLength(length);
        this->setWidth(width);
    }

    inline std::string lineTypeToString(lineType_t lineType)
    {
        switch(lineType) {
            case LINE: return "LINE";
            case LINE_ENDING_WITH_ARROW: return "LINE_ENDING_WITH_ARROW";
            case LINE_WITH_ARROWS: return "LINE_WITH_ARROWS";
            case NUM_LINE_TYPES: return "NUM_LINE_TYPES";
            default: return "INVALID_LINE_TYPE";
        }
    }

    inline void setLength(float length)
    {
        // Arrow vertices layout
        //               2 \
        //        0------1---3
        //               4 /

        // Set line vertices
        (*_verts)[0].set(0,0,0);
        (*_verts)[1].set(length,0,0);

        float arrowWidth = 0.003 * _lineWidth->getWidth();
        float arrowLength = 0.015 * _lineWidth->getWidth();

        switch(_lineType) {
            case LINE_ENDING_WITH_ARROW: {
                // Set end arrow vertices
                (*_verts)[2].set(length,arrowWidth, 0);
                (*_verts)[3].set(length + arrowLength, 0, 0);
                (*_verts)[4].set(length,-arrowWidth, 0);
                break;
            }
            case LINE_WITH_ARROWS: {
                // Set end arrow vertices
                (*_verts)[2].set(length,arrowWidth, 0);
                (*_verts)[3].set(length + arrowLength, 0, 0);
                (*_verts)[4].set(length,-arrowWidth, 0);
                // Set start arrow vertices
                (*_verts)[5].set(0,arrowWidth, 0);
                (*_verts)[6].set(-arrowLength, 0, 0);
                (*_verts)[7].set(0,-arrowWidth, 0);
                break;
            }
        }

        // Move vertices to VertexBuffer
        setVertexArray(_verts);
    }

    inline void setWidth(float width)
    {
        _lineWidth->setWidth(width);
        this->getOrCreateStateSet()->setAttribute(_lineWidth);
    }

    inline void setColor(const osg::Vec4f& newColor)
    {
        (*_color)[0] = newColor;
        this->setColorArray(_color);
    }

protected:

    inline void _createLine()
    {
        osg::DrawElementsUShort* elem =
                new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 0);
        elem->push_back(0); elem->push_back(1);
        this->addPrimitiveSet(elem);

        switch(_lineType) {
            case LINE_ENDING_WITH_ARROW: {
                osg::DrawElementsUShort* endArrow =
                        new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 0);
                endArrow->push_back(2); endArrow->push_back(3); endArrow->push_back(4);
                this->addPrimitiveSet(endArrow);
                break;
            }
            case LINE_WITH_ARROWS: {
                osg::DrawElementsUShort* endArrow =
                        new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 0);
                endArrow->push_back(2); endArrow->push_back(3); endArrow->push_back(4);
                this->addPrimitiveSet(endArrow);

                osg::DrawElementsUShort* startArrow =
                        new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 0);
                startArrow->push_back(5); startArrow->push_back(6); startArrow->push_back(7);
                this->addPrimitiveSet(startArrow);
                break;
            }
        }

        _color->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
        this->setColorArray(_color);
        this->setColorBinding(osg::Geometry::BIND_OVERALL);
    }

    osg::Vec3Array* _verts;
    osg::Vec4Array* _color;
    osg::LineWidth* _lineWidth;
    const lineType_t _lineType;
};

} // end namepsace osgGolems

#endif // LINE_H