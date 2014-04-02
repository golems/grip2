/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Pete Vieira <pete.vieira@gatech.edu>
 * Date: Feb 2014
 *
 * Humanoid skeletonics Lab      Georgia Institute of Technology
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
 * \file DartNodeCallback.h
 * \brief Callback class for updating a DartNode.
 * This class must subclass osg::NodeCallback
 */

#ifndef DARTNODE_CALLBACK_H
#define DARTNODE_CALLBACK_H

// OpenSceneGraph includes
#include <osg/NodeCallback>
#include <osg/io_utils>

// Local includes
#include "DartNode.h"

namespace osgDart {

/**
 * \class DartNodeCallback DartNodeCallback.h
 * \brief Callback class for updating a DartNode.
 * This class must subclass osg::NodeCallback
 */
class DartNodeCallback : public osg::NodeCallback
{
public:
    /**
     * \brief Constructs a DartNodeCallback object
     */
    DartNodeCallback(){}

    /**
     * \brief Core function of the callback where everything happens. This is
     * the function that gets called when a node visitor encounter this DartNode
     * \param node Node found by the node visitor (ie. The DartNode)
     * \param visitor The node visitor that encounter the DartNode
     * \return void
     */
    virtual void operator()(osg::Node* node, osg::NodeVisitor* visitor)
    {
        osg::ref_ptr<DartNode> dartNode = dynamic_cast<DartNode*> (node);

        // If the node is valid, update it
        if(dartNode) {
            dartNode->update();
        }

        // Continue traversing the scenegraph
        traverse(node, visitor);
    }

}; // end class DartNodeCallback

} // end namespace osgDart

#endif // DARTNODE_CALLBACK_H
