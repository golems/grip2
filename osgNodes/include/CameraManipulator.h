#ifndef CAMERA_MANIPULATOR_H
#define CAMERA_MANIPULATOR_H

#include <osgGA/OrbitManipulator>

class CameraManipulator : public osgGA::OrbitManipulator
{
public:
    CameraManipulator();
    ~CameraManipulator();
    void setCenter(osg::Vec3 center);

protected:



};

#endif // CAMERA_MANIPULATOR_H
