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

#ifdef DEBUG_BUILD
#define DEBUG(x) do { std::cerr << x; } while (0); std::cerr << std::endl;
#else
#define DEBUG(x)
#endif

// DART includes
#include <dart/dynamics/Shape.h>
#include <dart/dynamics/BoxShape.h>
#include <dart/dynamics/EllipsoidShape.h>
#include <dart/dynamics/CylinderShape.h>
#include <dart/dynamics/MeshShape.h>

// OpenSceneGraph includes
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Node>

// Assimp includes
#include <assimp/scene.h>

// Local includes
#include "osgDartShapes.h"
#include "osgAssimpSceneReader.h"
#include "osgUtils.h"
#include <osg/io_utils>

using namespace dart;

// TODO: get colors working for sdf files
// TODO: find out what ellipsoid shapes are possible in DART. currently only doing spheres
osg::Node* osgDart::convertShapeToOsgNode(dynamics::Shape* inputShape)
{
    osg::Geode* geode = new osg::Geode;
    osg::Matrix shapeMatrix = osgGolems::eigToOsgMatrix(inputShape->getLocalTransform());

    switch (inputShape->getShapeType()) {
        case dynamics::Shape::BOX: {
            DEBUG("Box");
            dynamics::BoxShape* shape = (dynamics::BoxShape*)inputShape;
            osg::Vec3f size = osgGolems::eigToOsgVec3(shape->getDim());
            osg::ShapeDrawable* osgShape =
                    new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), size.x(), size.y(), size.z()));
            osg::Vec4 color(osgGolems::eigToOsgVec3(shape->getColor()), .5);
//            std::cerr << "color: " << shape->getColor().transpose() << std::endl;
//            std::cerr << "osgColor: " << color << std::endl;
            osgShape->setColor(color);
            geode->addDrawable(osgShape);
            break;
        }
        case dynamics::Shape::ELLIPSOID: {
            DEBUG("Ellipsoid");
            dynamics::EllipsoidShape* shape = (dynamics::EllipsoidShape*)shape;
            osg::ShapeDrawable* osgBox =
                    new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), .3));
            osg::Vec4 color(osgGolems::eigToOsgVec3(shape->getColor()), 1.0);
//            std::cerr << "color: " << shape->getColor().transpose() << std::endl;
//            std::cerr << "osgColor: " << color << std::endl;
            osgBox->setColor(color);
            geode->addDrawable(osgBox);
            break;
        }
        case dynamics::Shape::CYLINDER: {
            DEBUG("Cylinder");
            dynamics::CylinderShape* shape = (dynamics::CylinderShape*)inputShape;
            osg::ShapeDrawable* osgShape =
                    new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,0,0), (float)shape->getRadius(), (float)shape->getHeight()));
            osg::Vec4 color(osgGolems::eigToOsgVec3(shape->getColor()), 1.0);
//            std::cerr << "color: " << shape->getColor().transpose() << std::endl;
//            std::cerr << "osgColor: " << color << std::endl;
            osgShape->setColor(color);
            geode->addDrawable(osgShape);
            break;
        }
        default: {
            std::cerr << "Error: Shape is not a valid shape type. Reported by " << __FILE__ << " on line " << __LINE__ << std::endl;
        }
    }

    // Add shape to a Geode, add it to a MatrixTransform whose matrix is set to
    // the local TF of the shape and add the MatrixTransform to the node as a child
    osg::ref_ptr<osg::MatrixTransform> shapeTF = new osg::MatrixTransform;
    shapeTF->setMatrix(shapeMatrix);
    shapeTF->addChild(geode);
    return shapeTF.release();
}

osg::Node* osgDart::convertMeshToOsgNode(dynamics::Shape* mesh)
{
    DEBUG("Mesh");
    dynamics::MeshShape* meshShape = (dynamics::MeshShape*)mesh;
    const aiScene* aiscene = meshShape->getMesh();
    aiNode* ainode = NULL;
    if(aiscene) {
        try {
            ainode = aiscene->mRootNode;
        } catch(std::exception const& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
        if(ainode) {
            return osgAssimpSceneReader::traverseAIScene(aiscene, aiscene->mRootNode);
        } else {
            std::cerr << "Error: aiNode no good. Exiting at line " << __LINE__ << " of file " << __FILE__ << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Error: aiScene no good. Exiting at line " << __LINE__ << " of file " << __FILE__ << std::endl;
        exit(1);
    }
}