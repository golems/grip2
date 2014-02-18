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
 * \file osgAssimpSceneReader.h
 * \brief Class for converting Assimp scenes to OpenSceneGraph nodes
 */

#ifndef OSG_ASSIMP_SCENEREADER_H
#define OSG_ASSIMP_SCENEREADER_H

// Assimp includes
#include <assimp/scene.h>

// OpenSceneGraph includes
#include <osgDB/ReaderWriter>

/**
 * \class osgAssimpSceneReader osgAssimpSceneReader.h
 * \brief Class for converting Assimp scenes to OpenSceneGraph nodes
 */
class osgAssimpSceneReader : public osgDB::ReaderWriter
{
public:
    osgAssimpSceneReader();

    virtual ~osgAssimpSceneReader();

    static osg::Node* traverseAIScene(const struct aiScene* aiScene, const struct aiNode* aiNode);

protected:
    void createMaterialData( osg::StateSet* ss, const aiMaterial* aiMtl) const;
};

#endif // OSG_ASSIMP_SCENEREADER_H
