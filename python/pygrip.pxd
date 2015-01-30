# distutils: language = c++
# Copyright (c) 2008-2015, Georgia Tech Research Corporation
# All rights reserved.
#
# Author(s): Jonathan Scholz <jkscholz@gatech.edu>
# Georgia Instutiute of Technology
#
# This file is provided under the following "BSD-style" License:
#
#   Redistribution and use in source and binary forms, with or
#   without modification, are permitted provided that the following
#   conditions are met:
#
#  * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.

'''
Created on Jan 29, 2015

@author: jscholz
'''

from libc.stdlib cimport malloc, free
from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "../include/GripInterface.h":
    cdef cppclass GripInterface:
        GripInterface() except +
        int _create(int argc, char **argv)
        int run(int argc, char **argv)
        void loadScene(string sceneFileName)
        void loadPluginFile(string pluginFileName)
        void render()
        void startSimulation()
        void stopSimulation()
        void simulateSingleStep()
        vector[double] getState()
        void setState(vector[double] state)
        vector[int] getSkeletonRootIdxs()
        vector[double] getConfig(bint toEulerXYZ)
        void setConfig(vector[double] state, bint fromEulerXYZ)
        bint checkCollision()

# Place any static Grip interface declarations here
cdef extern from "../include/GripInterface.h" namespace "GripInterface":
    void show_usage()
    vector[double] getScrewFromPose(vector[double])
    vector[double] getPoseFromScrew(vector[double])