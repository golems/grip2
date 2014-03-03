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


#include "osgUtils.h"
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include "Axes.h"

osgViewer::View* osgGolems::createView(int x, int y, int w, int h, osg::Node* scene)
{
    osgViewer::View* view = new osgViewer::View;
    osgGA::OrbitManipulator* cameraManipulator = new osgGA::OrbitManipulator();
    cameraManipulator->setAllowThrow(false);
    view->setCameraManipulator(cameraManipulator);
    view->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 0.5, 0.5));
    view->setUpViewInWindow(x, y, w, h);
    view->addEventHandler(new osgViewer::StatsHandler);
    if(scene == scene) {
        view->setSceneData(scene);
    }
    return view;
}

void osgGolems::addNodeToScene(osg::Group* scene)
{
    scene->addChild(osgGolems::createNode());
}

osg::Node* osgGolems::createNode()
{
    //                 --- tf1 --- geode --- cylinder
    // root --- rootTF |
    //                 --- tf2 --- geode2 --- cylinderClone

//    osg::Group* root = new osg::Group();
    // Create cylinder
    osg::Geode* geode = new osg::Geode;
    osg::TessellationHints* hints = new osg::TessellationHints;
    osg::ShapeDrawable* cylinder = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0.0f), .1, 1), hints);
    cylinder->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    geode->addDrawable(cylinder);
//    osg::Geode* geode2 = new osg::Geode;
//    osg::ShapeDrawable* cylinder2 = clone(cylinder);
//    cylinder2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//    geode2->addDrawable(cylinder2);

    // Create dynamics TF
    osg::MatrixTransform* rootTF = new osg::MatrixTransform;
    osg::MatrixTransform* tf1 = new osg::MatrixTransform;
//    osg::MatrixTransform* tf2 = new osg::MatrixTransform;
    tf1->setDataVariance(osg::Object::DYNAMIC);
//    tf1->setUpdateCallback(new LineCallback);
//    tf2->setDataVariance(osg::Object::DYNAMIC);
//    tf2->setUpdateCallback(new LineCallback);
    osg::Matrix m;
    m.makeTranslate(3, 0, 0);
    tf1->setMatrix(m);
    osg::Geode* g = new osg::Geode;
    Axes* a = new Axes();
    g->addDrawable(a);
    tf1->addChild(g);
//    tf2->addChild(geode2);

    // Add TF to root
    rootTF->addChild(tf1);
    //rootTF->addChild(tf2);
    //root->addChild(rootTF);

    // switch off lighting as we haven't assigned any normals.
//    root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    return rootTF;
}
