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

#include "Grid.h"
#include <iostream>

using namespace osgGolems;

//-----------------------------------------------
//            PUBLIC MEMBER FUNCTIONS
//-----------------------------------------------

Grid::Grid(uint width, uint depth, uint gridSize, const osg::Vec4& color)
{
    // Initialize protected member variables
    _verts = new osg::Vec3Array;
    _colors = new osg::Vec4Array;

    // Ensure really large or negative numbers weren't passed in
    if(width > maxLength) width = maxLength;
    if(depth > maxLength) depth = maxLength;

    // Create the perimter vertices and connect them with lines
    _createVertices(width, depth, _makeEven(gridSize));
    _drawGrid(color);
}

Grid::~Grid(){}

void Grid::setGridColor(const osg::Vec4& color)
{
    // Loop through color array and assign input color to it.
    for(size_t i=0; i<_colors->size(); ++i) {
        _colors->at(i) = color;
    }

    // Set color array of the Grid and bind it to the lines
    this->setColorArray(_colors);
    this->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
}


//-----------------------------------------------
//          PROTECTED MEMBER FUNCTIONS
//-----------------------------------------------

void Grid::_createVertices(uint width, uint depth, uint gridSize)
{
    // Compute number of lines for width and depth directions
    if(gridSize >= width || gridSize >= depth) {
        gridSize = std::min(width, depth) / 2;
        std::cerr << "[Grid] Error!. Request grid size is larger that either width or depth.\n"
                  << "Setting grid size to " << gridSize << std::endl;
    }
    uint numLinesWidth = (uint)(width / gridSize + 1);
    uint numLinesDepth = (uint)(depth / gridSize + 1);

    // Compute half width and depth to center grid in the middle
    int halfwidth = numLinesWidth * gridSize;
    int halfdepth = numLinesDepth * gridSize;

    // Resize vertex array
    _verts->resize(numLinesWidth*2 + numLinesDepth*2);

    // Create endpoints of width lines
    for(int w=-halfwidth; w<=halfwidth;) {
        _verts->push_back(osg::Vec3(w, halfdepth, 0));
        _verts->push_back(osg::Vec3(w,-halfdepth, 0));
        w += gridSize;
    }
    // Create endpoints of depth lines
    for(int d=-halfdepth; d<=halfdepth;) {
        _verts->push_back(osg::Vec3( halfwidth, d, 0));
        _verts->push_back(osg::Vec3(-halfwidth, d, 0));
        d += gridSize;
    }

    // Apply the vertices to the Grid class
    this->setVertexArray(_verts);
}

void Grid::_drawGrid(const osg::Vec4& color)
{
    for(size_t l=0; l<_verts->size()-1;) {
        osg::DrawElementsUShort* elem = new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 0);
        elem->push_back(l);
        elem->push_back(l+1);
        this->addPrimitiveSet(elem);
        _colors->push_back(color);
        l += 2;
    }

    // Set color array of the Grid and bind it to the lines
    this->setColorArray(_colors);
    this->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
}

uint Grid::_makeEven(uint num)
{
    // If even return number, else return number + 1
    if(num % 2) {
        return num;
    } else {
        return ++num;
    }
}
