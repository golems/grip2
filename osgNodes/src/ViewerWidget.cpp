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


#include "ViewerWidget.h"
#include <osg/io_utils>
#include "osgUtils.h"
#include "Axes.h"
#include "CameraCallback.h"
#include "CameraManipulator.h"
#include "Grid.h"

osg::Node* ViewerWidget::getSceneData()
{
    // Add root
    osg::Group* root = new osg::Group();

    // Add robot
    osg::Node* robot = osgDB::readNodeFile("../data/robot.osg");
    osg::MatrixTransform* tf = new osg::MatrixTransform();
    tf->addChild(robot);
    std::cerr << "tf:\n" << tf->getMatrix() << std::endl;
    root->addChild(tf);

    // Axes for center of rotation
    osg::MatrixTransform* gridTF = new osg::MatrixTransform();
    gridTF->setDataVariance(osg::Object::DYNAMIC);
//    CameraData* cameraData = new CameraData;
//    axesTF->setUserData(this->getView(0)->getCamera());
//    gridTF->setUserData(cameraData);
    osg::Geode* gridGeode = new osg::Geode();
    Grid* grid = new Grid(20, 20, 3, osg::Vec4(1, 1, 1, 1));
    gridGeode->addDrawable(grid);
    gridTF->addChild(gridGeode);
    osg::Matrix m;
    m.makeTranslate(0, 0, -2.5);
    gridTF->setMatrix(m);
    root->addChild(gridTF);

    return root;
}

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
    setThreadingModel(threadingModel);

    // Create scene data
    osg::Node* sceneData = getSceneData();

    // Create view widget with camera and scene data
    QWidget* widget1 = addViewWidget(createCamera(0,0,100,100), sceneData);

    // Create QLayout and add view widget to it. Then set it as the main layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(widget1);
    setLayout(layout);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

QWidget* ViewerWidget::addViewWidget(osg::Camera* camera, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera(camera);
    this->addView(view);

    view->setSceneData(scene);

    view->addEventHandler(new osgViewer::StatsHandler);

    CameraManipulator* cameraManipulator = new CameraManipulator();
//    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
//    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    std::cerr << "Center: " << cameraManipulator->getCenter() << std::endl;

    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());
    return gw ? gw->getGLWidget() : NULL;
}

osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
{
    // Get display settings instance in order to set our traits
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

    // Create GraphicsContext traits and set them.
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));
    camera->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f);
    return camera.release();
}

osg::Matrixd ViewerWidget::getViewMatrix()
{
    osg::Matrixd m = getView(0)->getCameraManipulator()->getMatrix();
    return m;
}

void ViewerWidget::setViewMatrix(uint i, osg::Matrixd m)
{
    if(viewNumIsValid(i)) {
        this->getView(i)->getCameraManipulator()->setByMatrix(m);
        osg::ref_ptr<osgGA::OrbitManipulator> c =
            dynamic_cast<osgGA::OrbitManipulator*>(this->getView(0)->getCameraManipulator());
        c->setCenter(osg::Vec3f(0, 0, 0));
    }
}

void ViewerWidget::addNodeToScene(osg::Node* node, uint viewNum)
{
    osg::Group* scene = this->getView(viewNum)->getSceneData()->asGroup();
    if(scene != scene) {
        std::cerr << "Error! Can not convert from osg::Node to osg::Group."
                  << std::endl;
    } else {
        scene->addChild(node);
    }
}

void ViewerWidget::addCenterAxes()
{
    osg::Node* axes = createNode();
    addNodeToScene(axes);
}

void ViewerWidget::setBackgroundColor(const osg::Vec4 &color, uint viewNum)
{
    // If the view number is valid, set the background color the view's camera
    if(viewNumIsValid(viewNum)) {
        this->getView(viewNum)->getCamera()->setClearColor(color);
    }
}

osgGA::CameraManipulator* ViewerWidget::getCameraManipulator(uint viewNum)
{
    // If the view number is valid, get its camera manipulator,
    // otherwise return a NULL pointer
    if(viewNumIsValid(viewNum)) {
        return this->getView(viewNum)->getCameraManipulator();
    } else {
        return NULL;
    }
}

void ViewerWidget::setCameraToHomePosition(uint viewNum)
{
    if(viewNumIsValid(viewNum)) {
        this->getCameraManipulator(viewNum)->home(1.0);
    }
}


bool ViewerWidget::viewNumIsValid(uint viewNum)
{
    // If viewNum exists, return true, otherwise report error and return false
    if(viewNum >= 0 && viewNum < this->getNumViews()) {
        return true;
    } else {
        fprintf(stderr, "Error! Requested view #%d, but only %d views exist\n", viewNum, this->getNumViews());
        fprintf(stderr, "Valid view #'s are 0 - %d\n", this->getNumViews() - 1);
        return false;
    }
}
