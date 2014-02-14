#ifndef OSG_ASSIMP_SCENEREADER_H
#define OSG_ASSIMP_SCENEREADER_H

#include <assimp/scene.h>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osgDB/ReaderWriter>

class osgAssimpSceneReader : public osgDB::ReaderWriter
{
public:
    osgAssimpSceneReader();

    virtual ~osgAssimpSceneReader();

    osg::Node* traverseAIScene(const struct aiScene* aiScene, const struct aiNode* aiNode) const;

protected:
    void createMaterialData( osg::StateSet* ss, const aiMaterial* aiMtl) const;

    osg::TexEnv::Mode getEnvMode( aiTextureOp mode ) const;

    osg::Texture::WrapMode getWrapMode( aiTextureMapMode mode ) const;
};

#endif // OSG_ASSIMP_SCENEREADER_H
