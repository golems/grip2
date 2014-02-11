#ifndef CAMERA_CALLBACK_H
#define CAMERA_CALLBACK_H

#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osgViewer/View>
#include <iostream>
#include <osg/io_utils>


class CameraData : public osg::Referenced
{
public:
    osg::MatrixTransform* tf;
    osgViewer::View* view;
};

class CameraCallback : public osg::NodeCallback
{
public:
    CameraCallback()
    {

    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::ref_ptr<CameraData> data =
                dynamic_cast<CameraData*> (node->getUserData());
        if(data) {
            osg::Matrix m = data->view->getCamera()->getInverseViewMatrix();
            data->tf->setMatrix(m);
        }

        traverse(node, nv);
    }
};


#endif // CAMERA_CALLBACK_H
