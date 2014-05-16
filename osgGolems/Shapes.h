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

/**
 * \file Shapes.h
 * \brief Container for all the different shape classes. Essentially each class
 * is a convenience class for an osg::Shape object.
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "osgUtils.h"

// OpenSceneGraph includes
#include <osg/Geode>
#include <osg/ShapeDrawable>

/**
 * \namespace osgGolems
 * \brief Namespace for all the classes that are only dependent upon OpenSceneGraph
 */
namespace osgGolems {


class Shape : public osg::Geode
{
public:
    inline Shape() : _defaultColor(osg::Vec4(0.5, 0.5, 0.5, 1.0))
    {
//        osgGolems::setMaterialOverrideValue(this, osg::StateAttribute::PROTECTED);
    }

    /**
     * \brief Sets the color of the shape
     * \param newColor osg::Vec4 reference for the color in rgba format in range (0,1)
     * \return void
     */
    inline void setColor(const osg::Vec4& newColor)
    {
        osgGolems::setDiffuse(this, newColor);
    }

protected:

    osg::Vec4 _defaultColor;
};

/**
 * \class Sphere
 * \brief Convenience class for creating a sphere node. Subclasses osg::Geode
 */
class Sphere : public osgGolems::Shape
{
public:
    inline Sphere()
    {
        this->setName("Sphere");
        _sphere = new osg::Sphere();
        this->addDrawable(new osg::ShapeDrawable(_sphere));
    }

    /**
     * \brief Creates a sphere node
     * \param center osg::Vec3 reference specifying the center of the sphere in meters
     * \param radius The radius of the shpere in meters
     * \param color The color of the sphere in rgba format in range (0,1)
     */
    inline Sphere(const osg::Vec3 &center, float radius,
                  const osg::Vec4 &color=osg::Vec4(0,0,0,1))
    {
        this->setName("Sphere");
        _sphere = new osg::Sphere(center, radius);
        this->addDrawable(new osg::ShapeDrawable(_sphere));
        this->setColor(color);
    }

    inline osg::Vec3 getCenter()
    {
        return _sphere->getCenter();
    }

    inline float getRadius()
    {
        return _sphere->getRadius();
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
    osg::ref_ptr<osg::Sphere> _sphere;

}; // end class Sphere


/**
 * \class Cylinder
 * \brief Convenience class for creating a cylinder node. Subclasses osg::Geode
 */
class Cylinder : public osgGolems::Shape
{
public:

    inline Cylinder()
    {
        this->setName("Cylinder");
        _cylinder = new osg::Cylinder();
        this->addDrawable(new osg::ShapeDrawable(_cylinder));
        this->setColor(_defaultColor);
    }

    /**
     * \brief Creates a cylinder node
     * \param center osg::Vec3 reference specifying the center of the cylinder in meters
     * \param radius The radius of the cylinder in meters
     * \param height The height of the cylinder in meters
     * \param color The color of the cylinder in rgba format in range (0,1)
     */
    inline Cylinder(const osg::Vec3 &center, float radius, float height,
                    const osg::Vec4 &color=osg::Vec4(0,0,0,1))
    {
        this->setName("Cylinder");
        _cylinder = new osg::Cylinder(center, radius, height);
        this->addDrawable(new osg::ShapeDrawable(_cylinder));
        this->setColor(color);
    }

    inline osg::Vec3 getCenter()
    {
        return _cylinder->getCenter();
    }

    inline float getRadius()
    {
        return _cylinder->getRadius();
    }

    inline float getHeight()
    {
        return _cylinder->getHeight();
    }

    /**
     * \brief Sets the center of the the cylinder in meters
     * \param newCenter osg::Vec3 reference for the center
     * \return void
     */
    inline void setCenter(float x, float y, float z)
    {
        _cylinder->setCenter(osg::Vec3(x,y,z));
    }

    /**
     * \brief Sets the radius of the the cylinder in meters
     * \param newRadius float value for the radius
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
    osg::ref_ptr<osg::Cylinder> _cylinder;

}; // end class Cylinder


/**
 * \class Box
 * \brief Convenience class for creating a box node. Subclasses osg::Geode
 */
class Box : public osgGolems::Shape
{
public:

    inline Box()
    {
        this->setName("Box");
        _box = new osg::Box();
        this->addDrawable(new osg::ShapeDrawable(_box));
        this->setColor(_defaultColor);
    }

    /**
     * \brief Creates a box node
     * \param center osg::Vec3 reference specifying the center of the box in meters
     * \param width The width of the box in meters
     * \param color The color of the box in rgba format in range (0,1)
     */
    inline Box(const osg::Vec3 &center, float width,
               const osg::Vec4 &color=osg::Vec4(0,0,0,1))
    {
        this->setName("Box");
        _box = new osg::Box(center, width);
        this->addDrawable(new osg::ShapeDrawable(_box));
        this->setColor(color);
    }

    /**
     * \brief Creates a box node
     * \param center osg::Vec3 reference specifying the center of the box in meters
     * \param width The width of the box in meters
     * \param color The color of the box in rgba format in range (0,1)
     */
    inline Box(const osg::Vec3 &center, const osg::Vec3 &sideLengths,
               const osg::Vec4 &color=osg::Vec4(0,0,0,1))
    {
        this->setName("Box");
        _box = new osg::Box(center, sideLengths.x(), sideLengths.y(), sideLengths.z());
        this->addDrawable(new osg::ShapeDrawable(_box));
        this->setColor(color);
    }

    inline osg::Vec3 getCenter()
    {
        return _box->getCenter();
    }

    inline osg::Vec3 getSideLengths()
    {
        return _box->getHalfLengths() * 2;
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

    inline void setWidth(float width)
    {
        _box->setHalfLengths(osg::Vec3(width/2, width/2, width/2));
    }

    inline void setSideLengths(const osg::Vec3 &lengths)
    {
        _box->setHalfLengths(lengths/2);
    }

    //TODO add function for changing the size of the box. Hopefully the setHalfLengths function

protected:

    /// osg::Box object so that we can change the dimensions of the box
    osg::Box* _box;

}; // end class Box

/**
 * \class Cone
 * \brief Convenience class for creating a cone node. Subclasses osg::Geode
 */
class Cone : public osgGolems::Shape
{
public:
    inline Cone()
    {
        this->setName("Cone");
        _cone = new osg::Cone();
        this->addDrawable(new osg::ShapeDrawable(_cone));
        this->setColor(_defaultColor);
    }
    /**
     * \brief Creates a cone node
     * \param center osg::Vec3 reference specifying the center of the cone in meters
     * \param height The height of the cone in meters
     * \param color The color of the cone in rgba format in range (0,1)
     */
    inline Cone(const osg::Vec3 &center, float radius, float height,
                const osg::Vec4 &color=osg::Vec4(0,0,0,1))
    {
        this->setName("Cone");
        _cone = new osg::Cone(center, radius, height);
        this->addDrawable(new osg::ShapeDrawable(_cone));
        this->setColor(color);
    }

    inline osg::Vec3 getCenter()
    {
        return _cone->getCenter();
    }

    inline float getRadius()
    {
        return _cone->getRadius();
    }

    inline float getHeight()
    {
        return _cone->getHeight();
    }

    inline void setCenter(const osg::Vec3 &newCenter)
    {
        _cone->setCenter(newCenter);
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

}; // end class Cone

/**
 * \class Capsule
 * \brief Convenience class for creating a capsule node. Subclasses osg::Geode
 */
class Capsule : public osgGolems::Shape
{
public:
    inline Capsule()
    {
        this->setName("Capsule");
        _capsule = new osg::Capsule();
        this->addDrawable(new osg::ShapeDrawable(_capsule));
        this->setColor(_defaultColor);
    }

    /**
     * \brief Creates a capsule node
     * \param center osg::Vec3 reference specifying the center of the capsule in meters
     * \param radius The radius of the capsule in meters
     * \param height The height of the capsule in meters
     * \param color The color of the capsule in rgba format in range (0,1)
     */
    inline Capsule(const osg::Vec3& center, float radius, float height, const osg::Vec4& color=osg::Vec4(0,0,0,1))
    {
        this->setName("Capsule");
        _capsule = new osg::Capsule(center, radius, height);
        this->addDrawable(new osg::ShapeDrawable(_capsule));
        this->setColor(color);
    }

    inline osg::Vec3 getCenter()
    {
        return _capsule->getCenter();
    }

    inline float getRadius()
    {
        return _capsule->getRadius();
    }

    inline float getHeight()
    {
        return _capsule->getHeight();
    }

    inline void setCenter(const osg::Vec3 &newCenter)
    {
        _capsule->setCenter(newCenter);
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

}; // end class Capsule

} // end namepsace osgGolems

#endif // SHAPES_H
