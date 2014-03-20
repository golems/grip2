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
 * \file Shapes.h
 * \brief Class
 */

#ifndef SHAPES_H
#define SHAPES_H

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/ShapeDrawable>

namespace osgGolems {

class Sphere : public osg::Geode
{
public:
    inline Sphere(const osg::Vec3& center, float radius, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _sphere = new osg::Sphere(center, radius);
        _shapeDrawable = new osg::ShapeDrawable(_sphere);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    inline void setCenter(const osg::Vec3& newCenter)
    {
        _sphere->setCenter(newCenter);
    }

    inline void setRadius(float newRadius)
    {
        _sphere->setRadius(newRadius);
    }

protected:

    osg::Sphere* _sphere;
    osg::ShapeDrawable* _shapeDrawable;
};

class Cylinder : public osg::Geode
{
public:
    inline Cylinder(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _cylinder = new osg::Cylinder(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_cylinder);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    inline void setRadius(float newRadius)
    {
        _cylinder->setRadius(newRadius);
    }

    inline void setHeight(float newHeight)
    {
        _cylinder->setHeight(newHeight);
    }

protected:
    osg::Cylinder* _cylinder;
    osg::ShapeDrawable* _shapeDrawable;
};

class Box : public osg::Geode
{
public:
    inline Box(const osg::Vec3& center, float width, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _box = new osg::Box(center, width);
        _shapeDrawable = new osg::ShapeDrawable(_box);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    inline void setCenter(const osg::Vec3& newCenter)
    {
        _box->setCenter(newCenter);
    }

protected:
    osg::Box* _box;
    osg::ShapeDrawable* _shapeDrawable;
};

class Cone : public osg::Geode
{
public:
    inline Cone(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _cone = new osg::Cone(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_cone);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    inline void setRadius(float newRadius)
    {
        _cone->setRadius(newRadius);
    }

    inline void setHeight(float newHeight)
    {
        _cone->setHeight(newHeight);
    }

protected:
    osg::Cone* _cone;
    osg::ShapeDrawable* _shapeDrawable;
};

class Capsule : public osg::Geode
{
public:
    inline Capsule(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _capsule = new osg::Capsule(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_capsule);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    inline void setRadius(float newRadius)
    {
        _capsule->setRadius(newRadius);
    }

    inline void setHeight(float newHeight)
    {
        _capsule->setHeight(newHeight);
    }

protected:
    osg::Capsule* _capsule;
    osg::ShapeDrawable* _shapeDrawable;
};

} // end namepsace osgGolems

#endif // SHAPES_H
