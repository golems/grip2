#ifndef DARTNODE_CALLBACK_H
#define DARTNODE_CALLBACK_H

#include <osg/NodeCallback>
#include <osg/io_utils>
#include "DartNode.h"

namespace osgDart
{

class DartNodeCallback : public osg::NodeCallback
{
public:
    DartNodeCallback(){}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* visitor)
    {
        osg::ref_ptr<DartNode> dartNode = dynamic_cast<DartNode*> (node);

        if(dartNode) {
            dartNode->update();
        }

        traverse(node, visitor);
    }

}; // end class DartNodeCallback

} // end namespace osgDart

#endif // DARTNODE_CALLBACK_H
