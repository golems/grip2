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

// Local includes
#include "ViewerWidget.h"
#include "osgUtils.h"
#include "Axes.h"
#include "CameraCallback.h"
#include "CameraManipulator.h"
#include "Grid.h"

// OpenSceneGraph includes
#include <osg/io_utils>
#include <osg/ShapeDrawable>

void ViewerWidget::addGrid(uint width, uint depth, uint gridSize)
{
    osg::Geode* gridGeode = new osg::Geode();
    osgGolems::Grid* grid = new osgGolems::Grid(width, depth, gridSize, osg::Vec4(1, 1, 1, 1));
    gridGeode->addDrawable(grid);
    addNodeToScene(gridGeode);
}

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
	std::cerr << "ThreadingModel: " << threadingModel << std::endl;
    setThreadingModel(threadingModel);
//    this->setRunFrameScheme(osgViewer::CompositeViewer::ON_DEMAND);

    // Create scene data
//    osg::Node* sceneData = getSceneData();
    osg::Group* sceneData = new osg::Group;

    // Create view widget with camera and scene data
    QWidget* widget1 = addViewWidget(createCamera(0,0,100,100), sceneData);

    // Create QLayout and add view widget to it. Then set it as the main layout
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(widget1);
    setLayout(layout);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(1);
}

QWidget* ViewerWidget::addViewWidget(osg::Camera* camera, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera(camera);
    this->addView(view);

    view->setSceneData(scene);

    view->addEventHandler(new osgViewer::StatsHandler);

    osgGolems::CameraManipulator* cameraManipulator = new osgGolems::CameraManipulator();
    view->setCameraManipulator(cameraManipulator);

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
    camera->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 0.0));
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
    if (viewNumIsValid(i)) {
        this->getView(i)->getCameraManipulator()->setByMatrix(m);
        osg::ref_ptr<osgGA::OrbitManipulator> c =
            dynamic_cast<osgGA::OrbitManipulator*>(this->getView(i)->getCameraManipulator());
        c->setCenter(osg::Vec3f(0, 0, 0));
    }
}

void ViewerWidget::setToTopView()
{
    osg::Matrixd m;
    m.makeRotate(M_PI/2, osg::Vec3(0, 0, 1));
    m.setTrans(0, 0, 1);
    this->setCameraMatrix(m);
    // 0  1  0  0
    //-1  0  0  0
    // 0  0  1  0
    // 0  0  1  1
}

void ViewerWidget::setToFrontView()
{
    osg::Matrixd m;
    m.makeRotate(M_PI/2, osg::Vec3(0, 0, 1));
    m.postMultRotate(osg::Quat(M_PI/2, osg::Vec3(0, 1, 0)));
    m.setTrans(1, 0, 0);
    this->setCameraMatrix(m);
    // 0  1  0  0
    // 0  0  1  0
    // 1  0  0  0
    // 1  0  0  1
}

void ViewerWidget::setToSideView()
{
    osg::Matrixd m;
    m.makeRotate(M_PI, osg::Vec3(0, 0, 1));
    m.postMultRotate(osg::Quat(-M_PI/2, osg::Vec3(1, 0, 0)));
    m.setTrans(0, 1, 0);
    this->setCameraMatrix(m);
    //-1  0  0  0
    // 0  0  1  0
    // 0  1  0  0
    // 0  1  0  1
}

void ViewerWidget::setCameraMatrix(osg::Matrix& newMatrix, uint viewNum)
{
    if (viewNumIsValid(viewNum)) {
        this->getCameraManipulator(viewNum)->setByMatrix(newMatrix);
        osg::ref_ptr<osgGA::OrbitManipulator> c =
            dynamic_cast<osgGA::OrbitManipulator*>(this->getCameraManipulator(viewNum));
        c->setCenter(osg::Vec3f(0, 0, 0));
    }
}

void ViewerWidget::addNodeToScene(osg::Node* node, uint viewNum)
{
    osg::Group* scene = this->getView(viewNum)->getSceneData()->asGroup();
    if (scene != scene) {
        std::cerr << "Error! Can not convert from osg::Node to osg::Group."
                  << std::endl;
    } else {
        scene->addChild(node);
    }
}

//void ViewerWidget::addCenterAxes()
//{
//    osg::Node* axes = osgUtils::createNode();
//    addNodeToScene(axes);
//}

void ViewerWidget::setBackgroundColor(const osg::Vec4 &color, uint viewNum)
{
    // If the view number is valid, set the background color the view's camera
    if (viewNumIsValid(viewNum)) {
        this->getView(viewNum)->getCamera()->setClearColor(color);
    }
}

osgGA::CameraManipulator* ViewerWidget::getCameraManipulator(uint viewNum)
{
    // If the view number is valid, get its camera manipulator,
    // otherwise return a NULL pointer
    if (viewNumIsValid(viewNum)) {
        return this->getView(viewNum)->getCameraManipulator();
    } else {
        return NULL;
    }
}

void ViewerWidget::setCameraToHomePosition(uint viewNum)
{
    if (viewNumIsValid(viewNum)) {
        this->getCameraManipulator(viewNum)->home(1.0);
    }
}


bool ViewerWidget::viewNumIsValid(uint viewNum)
{
    // If viewNum exists, return true, otherwise report error and return false
    if (viewNum >= 0 && viewNum < this->getNumViews()) {
        return true;
    } else {
        fprintf(stderr, "Error! Requested view #%d, but only %d views exist\n", viewNum, this->getNumViews());
        fprintf(stderr, "Valid view #'s are 0 - %d\n", this->getNumViews() - 1);
        return false;
    }
}
