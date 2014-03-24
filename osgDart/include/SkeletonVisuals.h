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

/**
 * \file SkeletonVisuals.h
 * \brief Class that combines all the visualization objects at the skeleton level.
 * This class subclasses osg::MatrixTransform.
 */

#ifndef SKELETON_VISUALS_H
#define SKELETON_VISUALS_H

// OpenSceneGraph includes
#include <osg/MatrixTransform>
#include <osg/CullFace>

// Local includes
#include "Shapes.h"

/**
 * \namespace osgDart
 * \brief Namespace containing all the classes and functionality relating to the
 * intersection of DART and OpenSceneGraph.
 */
namespace osgDart {

/**
 * \class SkeletonVisuals SkeletonVisuals.h
 * \brief Class that combines all the visualization objects at the skeleton level.
 * This class subclasses osg::MatrixTransform.
 */
class SkeletonVisuals : public osg::MatrixTransform
{
public:

    /**
     * \brief Constructs a SkeletonVisuals object
     */
    SkeletonVisuals();

    /**
     * \brief Destructs a SkeletonVisuals object
     */
    ~SkeletonVisuals();

    /**
     * \brief Adds a center of mass sphere to the class
     * \return void
     */
    void addCenterOfMass();

    /**
     * \brief Add a projected center of mass circle to the class
     * \return void
     */
    void addProjectedCenterOfMass();

    /**
     * \brief Getter for getting the TF that holds the center of mass sphere
     * \return osg::MatrixTransform pointer
     */
    osg::MatrixTransform* getCenterOfMassTF();

    /**
     * \brief Getter for getting the TF that holds the projected center of mass circle
     * \return osg::MatrixTransform pointer
     */
    osg::MatrixTransform* getProjectedCenterOfMassTF();

    /**
     * \brief Sets the color of the sphere representing the center of mass
     * \param newColor An osg::Vec4 reference specifying the color in rgba format in range (0,1)
     * \return void
     */
    void setCenterOfMassColor(const osg::Vec4& newColor);

    /**
     * \brief Sets the color of the circle representing the projected center of mass
     * \param newColor An osg::Vec4 reference specifying the color in rgba format in range (0,1)
     * \return void
     */
    void setProjectedCenterOfMassColor(const osg::Vec4& newColor);

protected:

    /**
     * \brief Set the modes for the geode passed in
     * \param geode osg::Geode pointer of which to change the modes
     * \return void
     */
    void _setGeodeModes(osg::Geode* geode);

    /// osg::MatrixTransform pointer for the TF that hold the center of mass node
    osg::MatrixTransform* _centerOfMassTF;

    /// osg::MatrixTransform pointer for the TF that holds the projected center of mass node
    osg::MatrixTransform* _projectedCenterOfMassTF;

    /// Center of mass node represented by an osgGolems::Sphere pointer
    osgGolems::Sphere* _centerOfMass;

    /// Projected center of mass node represent by an osgGolems::Cylinder pointer
    osgGolems::Cylinder* _projectedCenterOfMass;

    /// Culling pointer to tell OpenSceneGraph how to render the geodes
    osg::CullFace* _cullFace;

}; // end class SkeletonVisuals.h

} // end namespace osgDart

#endif // SKELETON_VISUALS_H
