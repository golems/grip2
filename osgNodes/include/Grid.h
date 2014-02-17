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
 * \file Grid.h
 * \brief Class for creating an OpenSceneGraph osg::Geometry\n
 * grid of user-specified width, depth and grid square size.
 */

#ifndef GRID_H
#define GRID_H

#include <osg/Geometry>

/**
 * \class Grid Grid.h
 * \brief Class for creating an OpenSceneGraph osg::Geomtry grid
 * of user-specified width, depth and grid square size.
 */
class Grid : public osg::Geometry
{
public:

    /**
     * \brief Constructor for Grid class.
     * \param width Width of the grid.
     * \param depth Depth of the grid.
     * \param gridSize Length of a side of each grid square.
     * \param color Color of the grid lines
     * \return void
     */
    Grid(uint width, uint depth, uint gridSize, const osg::Vec4& color);

    /**
     * \brief Destructor for Grid classs
     */
    ~Grid();

    void setGridColor(const osg::Vec4& color);

protected:

    /**
     * \brief Creates vertices around the perimeter of the grid according
     * to the width, depth and grid size specified by the user.
     * \param width Width of the grid.
     * \param depth Depth of the grid.
     * \param gridSize Length of a side of each grid square.
     * \return void
     */
    void _createVertices(uint width, uint depth, uint gridSize);

    /**
     * \brief Draw the lines in between the previously created vertices.
     * \return void
     */
    void _drawGrid(const osg::Vec4& color);

    /**
     * \brief Makes input number even if odd and does nothing if number is even.
     * It modifies the input number by reference.
     * \param num Integer to make even.
     * \return void
     */
    uint _makeEven(uint num);

    osg::ref_ptr<osg::Vec3Array> _verts;  // Array of vertices corresponding to grid line endpoints
    osg::ref_ptr<osg::Vec4Array> _colors; // Array of colors corresponding to grid line colors
    static const uint maxLength = 500;    // Maximum grid perimeter side length

};

#endif // GRID_H
