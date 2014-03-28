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
 * \brief Container for all the different shape classes. Essentially each class
 * is a convenience class for an osg::Shape object.
 */

#ifndef SHAPES_H
#define SHAPES_H

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/ShapeDrawable>

/**
 * \namespace osgGolems
 * \brief Namespace for all the classes that are only dependent upon OpenSceneGraph
 */
namespace osgGolems {

/**
 * \class Sphere
 * \brief Convenience class for creating a sphere node. Subclasses osg::Geode
 */
class Sphere : public osg::Geode
{
public:
    /**
     * \brief Creates a sphere node
     * \param center osg::Vec3 reference specifying the center of the sphere in meters
     * \param radius The radius of the shpere in meters
     * \param color The color of the sphere in rgba format in range (0,1)
     */
    inline Sphere(const osg::Vec3& center, float radius, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _sphere = new osg::Sphere(center, radius);
        _shapeDrawable = new osg::ShapeDrawable(_sphere);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    /**
     * \brief Sets the color of the sphere
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    /**
     * \brief Sets the center of the sphere in meters
     * \param newCenter osg::Vec3 reference for the center
     * \return void
     */
    inline void setCenter(const osg::Vec3& newCenter)
    {
        _sphere->setCenter(newCenter);
    }

    /**
     * \brief Sets the radius of the the sphere in meters
     * \param newRadius osg::Vec3 reference for the radius
     * \return void
     */
    inline void setRadius(float newRadius)
    {
        _sphere->setRadius(newRadius);
    }

protected:

    /// osg::Sphere object so that we can change the dimensions of the sphere
    osg::Sphere* _sphere;

    /// osg::ShapeDrawable object so that we can change the color of the sphere
    osg::ShapeDrawable* _shapeDrawable;

}; // end class Sphere


/**
 * \class Cylinder
 * \brief Convenience class for creating a cylinder node. Subclasses osg::Geode
 */
class Cylinder : public osg::Geode
{
public:
    /**
     * \brief Creates a cylinder node
     * \param center osg::Vec3 reference specifying the center of the cylinder in meters
     * \param radius The radius of the cylinder in meters
     * \param height The height of the cylinder in meters
     * \param color The color of the cylinder in rgba format in range (0,1)
     */
    inline Cylinder(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _cylinder = new osg::Cylinder(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_cylinder);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    /**
     * \brief Sets the color of the cylinder
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    /**
     * \brief Sets the radius of the the cylinder in meters
     * \param newRadius osg::Vec3 reference for the radius
     * \return void
     */
    inline void setRadius(float newRadius)
    {
        _cylinder->setRadius(newRadius);
    }

    /**
     * \brief Sets the height of the the cylinder in meters
     * \param newHeight osg::Vec3 reference for the height
     * \return void
     */
    inline void setHeight(float newHeight)
    {
        _cylinder->setHeight(newHeight);
    }

protected:

    /// osg::Cylinder object so that we can change its dimensions
    osg::Cylinder* _cylinder;

    /// osg::ShapeDrawable object so that we can change its color
    osg::ShapeDrawable* _shapeDrawable;

}; // end class Cylinder


/**
 * \class Box
 * \brief Convenience class for creating a box node. Subclasses osg::Geode
 */
class Box : public osg::Geode
{
public:
    /**
     * \brief Creates a box node
     * \param center osg::Vec3 reference specifying the center of the box in meters
     * \param width The width of the box in meters
     * \param color The color of the box in rgba format in range (0,1)
     */
    inline Box(const osg::Vec3& center, float width, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _box = new osg::Box(center, width);
        _shapeDrawable = new osg::ShapeDrawable(_box);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    /**
     * \brief Sets the color of the box
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    /**
     * \brief Sets the center of the box in meters
     * \param newCenter osg::Vec3 reference for the center
     * \return void
     */
    inline void setCenter(const osg::Vec3& newCenter)
    {
        _box->setCenter(newCenter);
    }

    //TODO add function for changing the size of the box. Hopefully the setHalfLengths function

protected:

    /// osg::Box object so that we can change the dimensions of the box
    osg::Box* _box;

    /// osg::ShapeDrawable so that we can change the color of the box
    osg::ShapeDrawable* _shapeDrawable;

}; // end class Box

/**
 * \class Cone
 * \brief Convenience class for creating a cone node. Subclasses osg::Geode
 */
class Cone : public osg::Geode
{
public:
    /**
     * \brief Creates a cone node
     * \param center osg::Vec3 reference specifying the center of the cone in meters
     * \param height The height of the cone in meters
     * \param color The color of the cone in rgba format in range (0,1)
     */
    inline Cone(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _cone = new osg::Cone(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_cone);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    /**
     * \brief Sets the color of the cone
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    /**
     * \brief Sets the radius of the the cone in meters
     * \param newRadius osg::Vec3 reference for the radius
     * \return void
     */
    inline void setRadius(float newRadius)
    {
        _cone->setRadius(newRadius);
    }

    /**
     * \brief Sets the height of the the cone in meters
     * \param newHeight osg::Vec3 reference for the height
     * \return void
     */
    inline void setHeight(float newHeight)
    {
        _cone->setHeight(newHeight);
    }

protected:

    /// osg::Cone object so that we can change the dimensions of the cone
    osg::Cone* _cone;

    /// osg::ShapeDrawable object so that we can change the color of the cone
    osg::ShapeDrawable* _shapeDrawable;

}; // end class Cone

/**
 * \class Capsule
 * \brief Convenience class for creating a capsule node. Subclasses osg::Geode
 */
class Capsule : public osg::Geode
{
public:
    /**
     * \brief Creates a capsule node
     * \param center osg::Vec3 reference specifying the center of the capsule in meters
     * \param radius The radius of the capsule in meters
     * \param height The height of the capsule in meters
     * \param color The color of the capsule in rgba format in range (0,1)
     */
    inline Capsule(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        _capsule = new osg::Capsule(center, radius, height);
        _shapeDrawable = new osg::ShapeDrawable(_capsule);
        _shapeDrawable->setColor(color);
        this->addDrawable(_shapeDrawable);
    }

    /**
     * \brief Sets the color of the capsule
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        _shapeDrawable->setColor(newColor);
    }

    /**
     * \brief Sets the radius of the the capsule in meters
     * \param newRadius osg::Vec3 reference for the radius
     * \return void
     */
    inline void setRadius(float newRadius)
    {
        _capsule->setRadius(newRadius);
    }

    /**
     * \brief Sets the height of the the capsule in meters
     * \param newHeight osg::Vec3 reference for the height
     * \return void
     */
    inline void setHeight(float newHeight)
    {
        _capsule->setHeight(newHeight);
    }

protected:
    /// osg::Capsule object so the we can change the dimensions of the capsule
    osg::Capsule* _capsule;

    /// osg::ShapeDrawable so that we can change the color of the capsule
    osg::ShapeDrawable* _shapeDrawable;

}; // end class Capsule

} // end namepsace osgGolems

#endif // SHAPES_H
