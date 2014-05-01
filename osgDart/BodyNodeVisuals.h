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
 * \file BodyNodeVisuals.h
 * \brief Class that holds all the visualization nodes for a link,
 * excluding the actual link.
 */

#ifndef BODYNODE_VISUALS_H
#define BODYNODE_VISUALS_H

// OpenSceneGraph includes
#include <osg/MatrixTransform>
#include <osg/CullFace>

// Local includes
#include "../osgGolems/Axes.h"
#include "../osgGolems/Line.h"

namespace osgDart {

/**
 * \brief Class that holds all the visualization nodes for a link,
 * excluding the actual link.
 */
class BodyNodeVisuals : public osg::MatrixTransform
{
public:
    /**
     * \brief Constructs a BodyNodeVisuals object
     */
    BodyNodeVisuals();

    /**
     * \brief Destructs a BodyNodeVisuals object
     */
    virtual ~BodyNodeVisuals();

    /**
     * \brief Adds a joint axis node to the class
     * \return void
     */
    void addJointAxis();

    /**
     * \brief Adds a link frame node to the class
     * \return void
     */
    void addBodyNodesAxes();

    /**
     * \brief Adds a center of mass sphere node to the class
     * Not implemented yet
     * \return void
     */
    void addCenterOfMass();

    /**
     * \brief Gets the osg::MatrixTransform pointer that holds the joint axis node
     * \return osg::MatrixTransform pointer
     */
    osg::MatrixTransform* getJointAxisTF();

    /**
     * \brief Gets the osg::MatrixTransform pointer that holds the link frame node
     * \return osg::MatrixTransform pointer
     */
    osg::MatrixTransform* getBodyNodeAxesTF();

    /**
     * \brief Sets the color of the joint axis node
     * \param color An osg::Vec4 reference specifying the color in rgba format in range (0,1)
     * \return void
     */
    void setJointAxisColor(const osg::Vec4 &color);

    /**
     * \brief Sets the color of the link frame node
     * \param color An osg::Vec4 reference specifying the color in rgba format in range (0,1)
     * \return void
     */
    void setBodyNodeAxesColors(const osg::Vec4& xAxis, const osg::Vec4& yAxis, const osg::Vec4& zAxis);

    /**
     * \brief Sets the line width of the joint axis and link frame axes. This is used only if
     * the joint axis and link frame are not created using 3D objects
     */
    void setLineWidth(float lineWidth);


protected:
    /**
     * \brief Makes a joint axis arrow node
     * \return osg::Geode pointer pointing to the newly created arrow node
     */
    osg::Geode* _makeJointAxis();

    /**
     * \brief Make a link frame node with three axes
     * \return osg::Geode pointer pointing to the newly created frame node
     */
    osg::Geode* _makeBodyNodeAxes();

    /**
     * \brief Set the modes of the geode passed in
     * \param geode The osg::Geode* of which to set the modes
     * \return void
     */
    void _setGeodeModes(osg::Geode* geode);

    /// The osg::MatrixTransform pointer holding the joint axis node
    osg::ref_ptr<osg::MatrixTransform> _jointAxisTF;

    /// The osgGolems::Line pointer representing the joint axis
    osg::ref_ptr<osgGolems::Line> _jointAxis;

    /// The osg::MatrixTransform pointer holding the link frame node
    osg::ref_ptr<osg::MatrixTransform> _bodyNodeAxesTF;

    /// The osgGolems::Axes pointer representing the link frame
    osg::ref_ptr<osgGolems::Axes> _bodyNodeAxes;

    /// The osg::LineWidth pointer holding the line width parameter of the 2D objects in the class
    osg::ref_ptr<osg::LineWidth> _lineWidth;

    /// The osg::CullFace pointer holding the cull setting of the class
    osg::ref_ptr<osg::CullFace> _cullFace;
};

} // end namespace osgDart

#endif // BODYNODE_VISUALS_H
