#ifndef GRID_H
#define GRID_H

#include <osg/Geometry>

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
