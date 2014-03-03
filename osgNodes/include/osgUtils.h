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


#ifndef OSG_UTILS_H
#define OSG_UTILS_H

#include <osgViewer/View>
#include <osgGA/OrbitManipulator>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <osg/io_utils>

/**
 * \namespace osgGolems
 * \brief Namespace to encompass the general OpenSceneGraph utilities
 */
namespace osgGolems {

/**
 * \brief Creates a osgViewer::View pointer with the specified location
 * of the upper-left corner of the window, width, height and scene.
 * \param x X-coordinate of the upper-left corner of the window
 * \param x Y-coordinate of the upper-left corner of the window
 * \param w Width of the window
 * \param h Height of the window
 * \param scene A osg::Node pointer to be put into the viewer window
 * \return osgViewer::View pointer
 */
osgViewer::View* createView(int x, int y, int w, int h, osg::Node* scene = NULL);

osg::Node* createNode();

void addNodeToScene(osg::Group* scene);

/**
 * \brief Convert Eigen::Isometry3d matrix to an osg::Matrix.
 * osg::Matrix is transposed.
 * \param tf Eigen::Isometry3d to be converted. Passed in by reference
 * \return osg::Matrix
 */
inline osg::Matrix eigToOsgMatrix(const Eigen::Isometry3d& tf)
{
    osg::Matrix output;
    for(ushort i=0; i<4; ++i)
        for(ushort j=0; j<4; ++j)
            output(i,j) = tf(j,i);
    return output;
}

/**
 * \brief Convert Eigen::Isometry3f matrix to an osg::Matrix.
 * osg::Matrix is transposed.
 * \param tf Eigen::Isometry3f to be converted. Passed in by reference
 * \return osg::Matrix
 */
inline osg::Matrix eigToOsgMatrix(const Eigen::Isometry3f& tf)
{
    osg::Matrix output;
    for(ushort i=0; i<4; ++i)
        for(ushort j=0; j<4; ++j)
            output(i,j) = tf(j,i);

    return output;
}

/**
 * \brief Convert Eigen::Vector3d vector to an osg::Vec3d
 * \param vec Eigen::Vector3d to be converted. Passed in by reference
 * \return osg::Vec3d
 */
inline osg::Vec3 eigToOsgVec3(const Eigen::Vector3d& vec)
{
    osg::Vec3 output;
    for(ushort i=0; i<3; ++i) {
        output[i] = vec[i];
    }
    return output;
}


/**
 * \brief Convert Eigen::Vector3d vector to an osg::Vec3d
 * \param vec Eigen::Vector3d to be converted. Passed in by reference
 * \return osg::Vec3d
 */
//inline osg::Vec3f eigToOsgVec3f(const Eigen::Vector3d& vec)
//{
//    osg::Vec3f output;
//    for(ushort i=0; i<3; ++i) {
//        output[i] = (float)vec[i];
//    }
//    return output;
//}

} // end of osgUtils namespace

#endif // OSG_UTILS_H
