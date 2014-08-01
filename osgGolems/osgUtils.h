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

/**
 * \file osgUtils.h
 * \brief Utility functions for OpenSceneGraph
 */

#ifndef OSG_UTILS_H
#define OSG_UTILS_H

// OpenSceneGraph includes
#include <osgViewer/View>
#include <osgGA/OrbitManipulator>
#include <osg/io_utils>
#include <osg/StateSet>
#include <osg/PolygonMode>
#include <osg/Material>
#include <osg/BlendFunc>

// Eigen includes
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

// C++ Standard Library includes
#include <iostream>

/**
 * \namespace osgGolems
 * \brief Namespace to encompass the general OpenSceneGraph utilities
 */
namespace osgGolems {

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

/**
 * \brief Convert Eigen::Isometry3d matrix to an osg::Matrix.
 * osg::Matrix is transposed.
 * \param tf Eigen::Isometry3d to be converted. Passed in by reference
 * \return osg::Matrix
 */
inline osg::Matrix eigToOsgMatrix(const Eigen::Isometry3d& tf)
{
    osg::Matrix output;
    for(ushort i=0; i<4; ++i)
        for(ushort j=0; j<4; ++j)
            output(i,j) = tf(j,i);
    return output;
}

/**
 * \brief Convert Eigen::Isometry3f matrix to an osg::Matrix.
 * osg::Matrix is transposed.
 * \param tf Eigen::Isometry3f to be converted. Passed in by reference
 * \return osg::Matrix
 */
inline osg::Matrix eigToOsgMatrix(const Eigen::Isometry3f& tf)
{
    osg::Matrix output;
    for(ushort i=0; i<4; ++i)
        for(ushort j=0; j<4; ++j)
            output(i,j) = tf(j,i);

    return output;
}

/**
 * \brief Convert Eigen::Vector3d vector to an osg::Vec3d
 * \param vec Eigen::Vector3d to be converted. Passed in by reference
 * \return osg::Vec3d
 */
inline osg::Vec3 eigToOsgVec3(const Eigen::Vector3d& vec)
{
    osg::Vec3 output;
    for(ushort i=0; i<3; ++i) {
        output[i] = vec[i];
    }
    return output;
}

inline void addMaterialAttribute(osg::Node *node)
{
    if (!node->getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL)) {
        node->getOrCreateStateSet()->setAttribute(new osg::Material);
    }
}

inline void addBlendFuncAttribute(osg::Node *node)
{
    if (!node->getOrCreateStateSet()->getAttribute(osg::StateAttribute::BLENDFUNC)) {
        node->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc);
    }
}

/**
 * \brief Adds a wireframe mode to the node passed in
 * \param[out] node Node for which to add a wireframe mode
 * \return void
 * \todo Fix everything
 */
inline void addWireFrameMode(osg::Node* node)
{
    if(!node) {
        std::cerr << "Invalid node. Line " << __LINE__ << " of " << __FILE__ << std::endl;
        return;
    }

    osg::PolygonMode* polyModeObj;

    polyModeObj = dynamic_cast<osg::PolygonMode*>(node->getOrCreateStateSet()->
                                                  getAttribute(osg::StateAttribute::POLYGONMODE));

    if(!polyModeObj) {
        polyModeObj = new osg::PolygonMode;
        node->getOrCreateStateSet()->setAttribute(polyModeObj);
    }
}

/**
 * \brief Turns on wireframe mode for the passed in node
 * If it doesn't have a wireframe mode, one is created.
 * \param node Node for which to turn on wireframe mode
 * \return void
 */
inline void setWireFrameOn(osg::Node* node)
{
    if(!node) {
        std::cerr << "Invalid node. Line " << __LINE__ << " of " << __FILE__ << std::endl;
        return;
    }

    if(!node->getStateSet()) {
        addWireFrameMode(node);
    }

    osg::PolygonMode* polyModeObj;
    polyModeObj = dynamic_cast<osg::PolygonMode*>(node->getStateSet()->
                                                  getAttribute(osg::StateAttribute::POLYGONMODE));

    if(!polyModeObj) {
        polyModeObj = new osg::PolygonMode;
        node->getStateSet()->setAttribute(polyModeObj);
    }

    polyModeObj->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
}

/**
 * \brief Turns off wireframe mode for the passed in node
 * If it doesn't have a wireframe mode, one is created.
 * \param node Node for which to turn off wireframe mode
 * \return void
 */
inline void setWireFrameOff(osg::Node* node)
{
    if(!node) {
        std::cerr << "Invalid node. Line " << __LINE__ << " of " << __FILE__ << std::endl;
        return;
    }

    if(!node->getStateSet()) {
        addWireFrameMode(node);
    }

    osg::PolygonMode* polyModeObj;
    polyModeObj = dynamic_cast<osg::PolygonMode*>(node->getStateSet()->
                                                  getAttribute(osg::StateAttribute::POLYGONMODE));

    if(!polyModeObj) {
        polyModeObj = new osg::PolygonMode;
        node->getStateSet()->setAttribute(polyModeObj);
    }

    polyModeObj->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
}

inline void setStateAttribute(osg::Node *node,
                              osg::StateAttribute::Type attribute,
                              osg::StateAttribute::OverrideValue attributeValue)
{
    if (!node)
        return;

    node->getOrCreateStateSet()->setAttributeAndModes(
                node->getOrCreateStateSet()->getAttribute(attribute),
                 attributeValue);
}

inline void setStateMode(osg::Node *node,
                         uint attribute,
                         osg::StateAttribute::OverrideValue attributeValue)
{
    if (!node)
        return;

    node->getOrCreateStateSet()->setMode(attribute, attributeValue);
}

/**
 * \brief Set the transparency value of a node
 * Reference: OSG Cookbook p. 239
 * \param node Node of which to change the transparency value
 * \param transparencyValue New transparency value for the node (between 0 and 1)
 * \return void
 */
inline void setTransparency(osg::Node* node, float transparencyValue)
{
    if (!node)
        return;

    std::cerr << "TL: Group: " << node->getName() << std::endl;

//    addMaterialAttribute(node);
//    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    if (!mat.valid()) {
        return;
    }
    osg::Vec4 diffuse = mat->getDiffuse(osg::Material::FRONT_AND_BACK);
//    std::cerr << "Diffuse: " << diffuse << std::endl;
//    std::cerr << "Ambient: " << mat->getAmbient(osg::Material::FRONT_AND_BACK) << std::endl;
//    std::cerr << "Emissive:" << mat->getEmission(osg::Material::FRONT_AND_BACK) << std::endl;
//    std::cerr << "Specular:" << mat->getSpecular(osg::Material::FRONT_AND_BACK) << std::endl;

    diffuse.set(diffuse.r(), diffuse.g(), diffuse.b(), transparencyValue);
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, diffuse);
    mat->setAlpha(osg::Material::FRONT_AND_BACK, transparencyValue);
//    node->getOrCreateStateSet()->setAttributeAndModes(
//                mat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

inline void setTransparencyRecursive(osg::Group *group, float transparencyValue)
{
    if (!group)
        return;

    osgGolems::setTransparency(group, transparencyValue);
    for (uint i = 0; i < group->getNumChildren(); ++i) {
        osgGolems::setTransparencyRecursive(group->getChild(i)->asGroup(),
                                            transparencyValue);
    }
}

inline void setDiffuse(osg::Node *node, const osg::Vec4 &diffuse,
                      osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setDiffuse(face, diffuse);
}

inline void setAmbient(osg::Node *node, const osg::Vec4 &ambient,
                       osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setAmbient(face, ambient);
}

inline void setEmission(osg::Node *node, const osg::Vec4 &emission,
                      osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setEmission(face, emission);
}

inline void setSpecular(osg::Node *node, const osg::Vec4 &specular,
                        osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setSpecular(face, specular);
}

inline void setAlpha(osg::Node *node, float alpha,
                        osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setAlpha(face, alpha);
}

inline void setColors(osg::Node *node, const osg::Vec4 &diffuse,
                      const osg::Vec4 &ambient, const osg::Vec4 &emission,
                      const osg::Vec4 &specular, float alpha,
                      osg::Material::Face face=osg::Material::FRONT_AND_BACK)
{
    if (!node)
        return;

    addMaterialAttribute(node);
    addBlendFuncAttribute(node);

    node->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    osg::ref_ptr<osg::Material> mat = (osg::Material*)node->getOrCreateStateSet()->
            getAttribute(osg::StateAttribute::MATERIAL);
    mat->setDiffuse(face, diffuse);
    mat->setAmbient(face, ambient);
    mat->setEmission(face, emission);
    mat->setSpecular(face, specular);
    mat->setAlpha(face, alpha);
}

} // end of osgGolems namespace

#endif // OSG_UTILS_H
