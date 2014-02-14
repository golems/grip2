#include <assimp/ai_assert.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>

#include <osg/PolygonMode>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>

#include "osgAssimpSceneReader.h"

osgAssimpSceneReader::osgAssimpSceneReader()
{
}

osgAssimpSceneReader::~osgAssimpSceneReader()
{
}

osg::Node* osgAssimpSceneReader::traverseAIScene(const struct aiScene* aiScene, const struct aiNode* aiNode) const
{
    osg::Geode* geode = new osg::Geode;
    for ( unsigned int n=0; n<aiNode->mNumMeshes; ++n )
    {
        // Create geometry basic properties
        const struct aiMesh* mesh = aiScene->mMeshes[ aiNode->mMeshes[n] ];
        osg::Geometry* geom = new osg::Geometry;
        geode->addDrawable( geom );

        osg::Vec3Array* va = new osg::Vec3Array(mesh->mNumVertices);
        osg::Vec3Array* na = (mesh->mNormals ? new osg::Vec3Array(mesh->mNumVertices) : NULL);
        osg::Vec4Array* ca = (mesh->mColors[0] ? new osg::Vec4Array(mesh->mNumVertices) : NULL);
        for ( unsigned int i=0; i<mesh->mNumVertices; ++i )
        {
            const aiVector3D& v = mesh->mVertices[i];
            (*va)[i].set( v.x, v.y, v.z );
            if ( na )
            {
                const aiVector3D& n = mesh->mNormals[i];
                (*na)[i].set( n.x, n.y, n.z );
            }
            if ( ca )
            {
                const aiColor4D& c = mesh->mColors[0][i];
                (*ca)[i].set( c.r, c.g, c.b, c.a );
            }
        }

        geom->setVertexArray( va );
        if ( na )
        {
            geom->setNormalArray( na );
            geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
        }
        if ( ca )
        {
            geom->setColorArray( ca );
            geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
        }

        // Create geometry texture coordinates
        unsigned int unit = 0;
        const aiVector3D* aiTexCoords = mesh->mTextureCoords[unit];
        while ( aiTexCoords!=NULL )
        {
            switch ( mesh->mNumUVComponents[unit] )
            {
            case 1:
                {
                    osg::FloatArray* ta = new osg::FloatArray(mesh->mNumVertices);
                    for ( unsigned int i=0; i<mesh->mNumVertices; ++i )
                        (*ta)[i] = aiTexCoords[i].x;
                    geom->setTexCoordArray( unit, ta );
                }
                break;
            case 2:
                {
                    osg::Vec2Array* ta = new osg::Vec2Array(mesh->mNumVertices);
                    for ( unsigned int i=0; i<mesh->mNumVertices; ++i )
                    {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set( t.x, t.y );
                    }
                    geom->setTexCoordArray( unit, ta );
                }
                break;
            case 3:
                {
                    osg::Vec3Array* ta = new osg::Vec3Array(mesh->mNumVertices);
                    for ( unsigned int i=0; i<mesh->mNumVertices; ++i )
                    {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set( t.x, t.y, t.z );
                    }
                    geom->setTexCoordArray( unit, ta );
                }
                break;
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
        for ( unsigned int f=0; f<mesh->mNumFaces; ++f )
        {
            const struct aiFace& face = mesh->mFaces[f];
            if ( face.mNumIndices>4 ) current = de[0].get();
            else current = de[face.mNumIndices].get();

            for ( unsigned i=0; i<face.mNumIndices; ++i )
                current->push_back( face.mIndices[i] );
        }

        for ( unsigned int i=0; i<5; ++i )
        {
            if ( de[i]->size()>0 )
                geom->addPrimitiveSet( de[i].get() );
        }
    }

    aiMatrix4x4 m = aiNode->mTransformation;
    m.Transpose();

    // Create the node and continue looking for children
    osg::ref_ptr<osg::MatrixTransform> mt;
    mt = new osg::MatrixTransform;
    mt->setMatrix( osg::Matrixf((float*)&m) );
    for ( unsigned int n=0; n<aiNode->mNumChildren; ++n )
    {
        osg::Node* child = traverseAIScene(aiScene, aiNode->mChildren[n]);
        if ( child ) mt->addChild( child );
    }
    mt->addChild( geode );
    return mt.release();
}

void osgAssimpSceneReader::createMaterialData( osg::StateSet* ss, const aiMaterial* aiMtl) const
{
    aiColor4D c;
    osg::Material* material = new osg::Material;
    if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_AMBIENT, &c)==AI_SUCCESS)
        material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_DIFFUSE, &c)==AI_SUCCESS)
        material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_SPECULAR, &c)==AI_SUCCESS)
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
    if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_EMISSIVE, &c)==AI_SUCCESS)
        material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));

    unsigned int maxValue = 1;
    float shininess = 0.0f, strength = 1.0f;
    if (aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS, &shininess, &maxValue)==AI_SUCCESS)
    {
        maxValue = 1;
        if (aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &maxValue)==AI_SUCCESS)
            shininess *= strength;
        material->setShininess(osg::Material::FRONT_AND_BACK, shininess );
    }
    else
    {
        material->setShininess(osg::Material::FRONT_AND_BACK, 0.0f);
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4());
    }
    ss->setAttributeAndModes(material);

    int wireframe = 0; maxValue = 1;
    if ( aiGetMaterialIntegerArray(aiMtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &maxValue)==AI_SUCCESS )
    {
        ss->setAttributeAndModes( new osg::PolygonMode(
            osg::PolygonMode::FRONT_AND_BACK, wireframe ? osg::PolygonMode::LINE : osg::PolygonMode::FILL) );
    }
}

osg::TexEnv::Mode osgAssimpSceneReader::getEnvMode( aiTextureOp mode ) const
{
    switch ( mode )
    {
    case aiTextureOp_Multiply: return osg::TexEnv::MODULATE;
    case aiTextureOp_Add: return osg::TexEnv::ADD;
    case aiTextureOp_Subtract: return osg::TexEnv::DECAL;
    case aiTextureOp_SmoothAdd: case aiTextureOp_SignedAdd:
        return osg::TexEnv::ADD;
    }
    return osg::TexEnv::REPLACE;
}

osg::Texture::WrapMode osgAssimpSceneReader::getWrapMode( aiTextureMapMode mode ) const
{
    switch ( mode )
    {
    case aiTextureMapMode_Wrap: return osg::Texture::REPEAT;
    case aiTextureMapMode_Clamp: return osg::Texture::CLAMP;
    case aiTextureMapMode_Decal: return osg::Texture::CLAMP_TO_BORDER;
    case aiTextureMapMode_Mirror: return osg::Texture::MIRROR;
    }
    return osg::Texture::CLAMP;
}
