#ifndef OSG_UTILS_H
#define OSG_UTILS_H

#include <osgViewer/View>
#include <osgGA/OrbitManipulator>

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



#endif // OSG_UTILS_H
