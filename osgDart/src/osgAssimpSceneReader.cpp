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

// C++ Standard Library includes
#include <iostream>

// OpenSceneGraph includes
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/PolygonMode>

// Assimp includes
#include <assimp/scene.h>

// Local includes
#include "osgAssimpSceneReader.h"

osgAssimpSceneReader::osgAssimpSceneReader()
{
}

osgAssimpSceneReader::~osgAssimpSceneReader()
{
}

osg::Node* osgAssimpSceneReader::traverseAIScene(const struct aiScene* aiScene, const struct aiNode* aiNode)
{
    // Create main geode and loop through meshes
    osg::Geode* geode = new osg::Geode;
    for(uint n=0; n<aiNode->mNumMeshes; ++n) {
        const struct aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[n]];
        osg::Geometry* geom = new osg::Geometry;
        geode->addDrawable(geom);

        // Allocate memory for osg vertex, normal and color arrays if they exist
        osg::Vec3Array* va = new osg::Vec3Array(mesh->mNumVertices);
        osg::Vec3Array* na = (mesh->mNormals ? new osg::Vec3Array(mesh->mNumVertices) : NULL);
        osg::Vec4Array* ca = (mesh->mColors[0] ? new osg::Vec4Array(mesh->mNumVertices) : NULL);

        // Add vertices, normals and colors to osg arrays from assimp arrays
        for(uint i=0; i<mesh->mNumVertices; ++i) {
            const aiVector3D& v = mesh->mVertices[i];
            (*va)[i].set(v.x, v.y, v.z);
            if(na) {
                const aiVector3D& n = mesh->mNormals[i];
                (*na)[i].set(n.x, n.y, n.z);
            }
            if(ca) {
                std::cerr << "has color" << std::endl;
                const aiColor4D& c = mesh->mColors[0][i];
                (*ca)[i].set(c.r, c.g, c.b, c.a);
            }
        }

        // Set vertex, normal and color arrays of the osg::Geometry
        geom->setVertexArray(va);
        if(na) {
            geom->setNormalArray(na);
            geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        }
        if(ca) {
            geom->setColorArray(ca);
            geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
        }

        // Create geometry texture coordinates
        uint unit = 0;
        const aiVector3D* aiTexCoords = mesh->mTextureCoords[unit];
        while(aiTexCoords!=NULL) {
            switch(mesh->mNumUVComponents[unit]) {
                case 1: {
                    osg::FloatArray* ta = new osg::FloatArray(mesh->mNumVertices);
                    for(uint i=0; i<mesh->mNumVertices; ++i) {
                        (*ta)[i] = aiTexCoords[i].x;
                    }
                    geom->setTexCoordArray(unit, ta);
                    break;
                }
                case 2: {
                    osg::Vec2Array* ta = new osg::Vec2Array(mesh->mNumVertices);
                    for(uint i=0; i<mesh->mNumVertices; ++i) {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set(t.x, t.y);
                    }
                    geom->setTexCoordArray(unit, ta);
                    break;
                }
                case 3: {
                    osg::Vec3Array* ta = new osg::Vec3Array(mesh->mNumVertices);
                    for(uint i=0; i<mesh->mNumVertices; ++i) {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set(t.x, t.y, t.z);
                    }
                    geom->setTexCoordArray(unit, ta);
                    break;
                }
            }
            aiTexCoords = mesh->mTextureCoords[++unit];
        }

        // Create geometry primitives
        osg::ref_ptr<osg::DrawElementsUInt> de[5];
        de[1] = new osg::DrawElementsUInt(GL_POINTS);
        de[2] = new osg::DrawElementsUInt(GL_LINES);
        de[3] = new osg::DrawElementsUInt(GL_TRIANGLES);
        de[4] = new osg::DrawElementsUInt(GL_QUADS);
        de[0] = new osg::DrawElementsUInt(GL_POLYGON);

        osg::DrawElementsUInt* current = NULL;
        for(uint f=0; f<mesh->mNumFaces; ++f) {
            const struct aiFace& face = mesh->mFaces[f];
            if(face.mNumIndices>4) {
                current = de[0].get();
            } else {
                current = de[face.mNumIndices].get();
            }
            for(unsigned i=0; i<face.mNumIndices; ++i) {
                current->push_back(face.mIndices[i]);
            }
        }

        for(uint i=0; i<5; ++i) {
            if(de[i]->size()>0) {
                geom->addPrimitiveSet( de[i].get());
            }
        }

        // Create materials
        osg::StateSet* ss = geom->getOrCreateStateSet();
        const aiMaterial* aiMtl = aiScene->mMaterials[mesh->mMaterialIndex];
        createMaterialData( ss, aiMtl );
    }

    aiMatrix4x4 m = aiNode->mTransformation;
    m.Transpose();

    // Create the node and continue looking for children
    osg::ref_ptr<osg::MatrixTransform> mt;
    mt = new osg::MatrixTransform;
    mt->setMatrix( osg::Matrixf((float*)&m));

    for(uint n=0; n<aiNode->mNumChildren; ++n)
    {
        osg::Node* child = traverseAIScene(aiScene, aiNode->mChildren[n]);
        if(child) {
            mt->addChild(child);
        }
    }
    mt->addChild(geode);
    return mt.release();
}

void osgAssimpSceneReader::createMaterialData(osg::StateSet* ss, const aiMaterial* aiMtl)
{
    aiColor4D c;
    osg::Material* material = new osg::Material;
    if(aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_AMBIENT, &c)==AI_SUCCESS) {
        material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    }
    if(aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_DIFFUSE, &c)==AI_SUCCESS) {
        material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    }
    if(aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_SPECULAR, &c)==AI_SUCCESS) {
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    }
    if(aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_EMISSIVE, &c)==AI_SUCCESS) {
        material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    }

    unsigned int maxValue = 1;
    float shininess = 0.0f, strength = 1.0f;
    if(aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS, &shininess, &maxValue)==AI_SUCCESS) {
        maxValue = 1;
        if(aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &maxValue)==AI_SUCCESS)
            shininess *= strength;
        material->setShininess(osg::Material::FRONT_AND_BACK, shininess);
    }
    else {
        material->setShininess(osg::Material::FRONT_AND_BACK, 0.0f);
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4());
    }
    ss->setAttributeAndModes(material);

    int wireframe = 0; maxValue = 1;
    if(aiGetMaterialIntegerArray(aiMtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &maxValue)==AI_SUCCESS) {
        ss->setAttributeAndModes( new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, wireframe ? osg::PolygonMode::LINE : osg::PolygonMode::FILL) );
    }
}
