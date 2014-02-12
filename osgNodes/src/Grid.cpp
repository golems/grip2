
#include "Grid.h"
#include <iostream>

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
        std::cerr << "Error!. Request grid size is larger that either width or depth.\n"
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
