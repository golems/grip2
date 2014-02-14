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


#ifndef AXES_H
#define AXES_H

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osg/LineWidth>
#include <osg/Matrix>
#include <osg/MatrixTransform>

class Axes : public osg::Geometry
{
public:
    
    inline Axes(float scale = 1)
    {
        _verts = new osg::Vec3Array;
        _colors = new osg::Vec4Array;
        _verts->resize(5);
        
        _setElements();
        setScale(scale);
    }
    
    inline void setScale(float new_scale)
    {
        (*_verts)[0] = osg::Vec3(0,0,0);
        (*_verts)[1] = osg::Vec3(new_scale,0,0);
        (*_verts)[2] = osg::Vec3(0,new_scale,0);
        (*_verts)[3] = osg::Vec3(0,0,new_scale);
        (*_verts)[4] = osg::Vec3(new_scale,new_scale,0);
        
        setVertexArray(_verts);
    }
    
    
protected:
    
    inline void _setElements()
    {
        osg::DrawElementsUShort* x_elem =
                new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 0);
        x_elem->push_back(0); x_elem->push_back(1);
        addPrimitiveSet(x_elem);
        _colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
        
        osg::DrawElementsUShort* y_elem =
                new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 0);
        y_elem->push_back(0); y_elem->push_back(2);
        addPrimitiveSet(y_elem);
        _colors->push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
        
        osg::DrawElementsUShort* z_elem =
                new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 0);
        z_elem->push_back(0); z_elem->push_back(3);
        addPrimitiveSet(z_elem);
        _colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));

        osg::DrawElementsUShort* plane_top =
                new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 0);
        plane_top->push_back(0); plane_top->push_back(1);
        plane_top->push_back(4); plane_top->push_back(2);
        addPrimitiveSet(plane_top);
        _colors->push_back(osg::Vec4(0.6f,0.6f,1.0f,0.5));

        osg::DrawElementsUShort* plane_bottom =
                new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 0);
        plane_bottom->push_back(0); plane_bottom->push_back(2);
        plane_bottom->push_back(4); plane_bottom->push_back(1);
        addPrimitiveSet(plane_bottom);
        _colors->push_back(osg::Vec4(1.0f,1.0f,0.6f,0.5f));
        
        setColorArray(_colors);
        setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    }
    
    osg::Vec3Array* _verts;
    osg::Vec4Array* _colors;
    
};

#endif // AXES_H
