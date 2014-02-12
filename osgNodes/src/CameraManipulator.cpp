
#include "CameraManipulator.h"

CameraManipulator::CameraManipulator()
{
    this->setAllowThrow(false);
}

CameraManipulator::~CameraManipulator()
{

}

void CameraManipulator::setCenter(osg::Vec3 center)
{
    osgGA::OrbitManipulator::setCenter(center);
}
