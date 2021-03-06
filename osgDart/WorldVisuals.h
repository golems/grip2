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
 * \file WorldVisuals.h
 * \brief Library that contains visuals for the world. Currently this only
 * include contact forces and the lines representing them. This is because
 * skeletons can contact each other and therefore it become a world level interface,
 * as opposed to contained within a skeleton or BodyNode.
 */

#ifndef WORLD_VISUALS_H
#define WORLD_VISUALS_H

// Local includes
#include "../osgGolems/Line.h"

// OpenSceneGraph includes
#include <osg/MatrixTransform>

// Eigen includes
#include <Eigen/Core>
#include <Eigen/Dense>

namespace osgDart {

/**
 * \class ContactForceVisual WorldVisuals.h
 * \brief Class that creates a force vector visualization node
 */
class ContactForceVisual : public osg::MatrixTransform
{
public:
    /**
     * \brief Constructs a ContactForceVisual object
     * \param debug Whether or not to print debug statements
     */
    ContactForceVisual(bool debug=false);

    /**
     * \brief Destructs the ContactForceVisual object
     */
    ~ContactForceVisual();

    /**
     * \brief Creates a force vector for the contact forces
     * \param lineLength Length of the force vector
     * \param contactPoint Vector specifying the point of the contact
     * \param contactForce Force of the contact force
     * \return osg::MatrixTransform pointer to the TF containing the force vector node
     */
    void createForceVector(float lineLength,
                           const Eigen::Vector3d &contactPoint,
                           const Eigen::Vector3d &contactForce);

    /**
     * \brief update the contact force visual
     * \param lineLength Length of the force vector
     * \param contactPoint Vector specifying the point of the contact
     * \param contactForce Force of the contact force
     * \return void
     */
    void update(float lineLength, const Eigen::Vector3d &contactPoint,
                const Eigen::Vector3d &contactForce);

protected:
    /**
     * \brief Converts two Eigen::Vector3d's specifying the point of contact
     * and direction of the force to an osg::Matrix
     * \param point Vector specifying the point of contact
     * \param vector Vector specifying the direction of the force
     * \return osg::Matrix pointer which is the transform of the force vector
     */
    osg::Matrix _convertVectorToOsgTF(const Eigen::Vector3d &point, const Eigen::Vector3d &vector);

    /// Geometry creating the Line in OpenSceneGraph
    osgGolems::Line* _contactForceLine;

    /// Whether or not to print out debug statements
    bool _debug;

}; // end class ContactForceVisual

} // end namespace osgDart

#endif // WORLD_VISUALS_H
