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
Created on August 22, 2014

@author: jscholz
'''

from pygrip cimport GripInterface
from pygrip cimport show_usage
from pygrip cimport getScrewFromPose as c_getScrewFromPose
from pygrip cimport getPoseFromScrew as c_getPoseFromScrew

def getScrewFromPose(pose):
    '''
    Converts the provided pose containing [x, y, z, roll, pitch, yaw]
    (assuming XYZ ordering) to exponential coordinates

    :param pose: Query pose in cartesian-euler coordinates
    :return: Output pose in exponential coordinates
    '''
    return c_getScrewFromPose(pose)

def getPoseFromScrew(screw):
    '''
    Converts the provided exponential coordinate reprentation of a pose to
    euler angles [x, y, z, roll, pitch, yaw] (assuming XYZ ordering)
    
    :param screw: Query pose in exponential coordinates
    :return: Output pose in cartesian-euler coordinates
    '''
    return c_getPoseFromScrew(screw)

cdef class PyGrip:
    '''
    Main Python class for interfacing to Grip.
    Implements a set of pass-through methods for interacting with 
    libgrip-interface.
    '''
    cdef GripInterface *thisptr      # hold a C++ instance which we're wrapping

    def __cinit__(self):
        self.thisptr = new GripInterface()

    def __dealloc__(self):
        del self.thisptr

    def usage(self):
        '''
        Prints GripInterface usage string.
        '''
        show_usage()

    def run(self, args=None):
        '''
        Calls run, which is same as create except attempts to do in a thread.
        This is proving difficult on MAC OSX though
        '''
        if args is None:
            args = ''

        cdef char **c_argv

        # make *sure* that we have the exact string object for every argument
        #, and don't invoke __str__ on something else!!
        args = [bytes(x) for x in args]

        # or, use str(x).encode(...) above, depending on what API you want and what encoding C program expect
        c_argv = <char**>malloc(sizeof(char*) * len(args))

        for idx, s in enumerate(args):
            c_argv[idx] = s

        try:
            ret = self.thisptr.run(len(args), c_argv)
        finally:
            ret = -1
            # free(c_argv)
        return ret

    def loadScene(self, sceneFileName):
        '''
        :brief: Load the scene and renders it. This function resets 
                everything on each load.
        :param fileame: Name of scene file to load
        :return void:
        '''
        self.thisptr.loadScene(sceneFileName)

    def loadPluginFile(self, pluginFileName):
        '''
        :brief: Loads a plugin file (.so) by name. When loaded, the plugin 
                will be added to the bottom tabs area.
        :param pluginFileName: String representing the path of the plugin
        :return void:
        '''
        self.thisptr.loadPluginFile(pluginFileName)

    def render(self):
        '''
        Renders the current scene and executes all drawing callbacks
        in the render queue.
        '''
        self.thisptr.render()

    def startSimulation(self):
        self.thisptr.startSimulation()

    def stopSimulation(self):
        self.thisptr.stopSimulation()

    def simulateSingleStep(self):
        self.thisptr.simulateSingleStep()

    def getState(self):
        '''
        Returns the current world state in Dart/Grip.  Note that dart's 
        internal state reprentation is in generalized coordinates, with 
        root link poses represented in exponential coordinates.  

        The returned state vector is the concatentation of the states of
        each "skeleton" (body with possible links parameterized by DOF) 
        with DOF organized as follows:
        1-3: body orientation (exponential coordinates)
        4-6: body pose (exponential coordinates)
        7+: any child dofs, following the link order in the URDF
        n-(2n-1): Velocites corresponding to each DOF

        For the skeleton ordering see getSkeletonRootIdxs

        For position-only access see getConfig and setConfig, which also
        provide convenience flags for converting the exponential part 
        of the body state to euler angles.  
        '''
        return self.thisptr.getState()

    def setState(self, state):
        '''
        Sets the current world state in Dart/Grip.  Note that dart's 
        internal state reprentation is in generalized coordinates, with 
        root link poses represented in exponential coordinates.  

        The provided state vector must be the concatentation of the states of
        each "skeleton" (body with possible links parameterized by DOF) 
        with DOF organized as follows:
        1-3: body orientation (exponential coordinates)
        4-6: body pose (exponential coordinates)
        7+: any child dofs, following the link order in the URDF
        n-(2n-1): Velocites corresponding to each DOF

        For the skeleton ordering see getSkeletonRootIdxs

        For position-only access see getConfig and setConfig, which also
        provide convenience flags for converting the exponential part 
        of the body state to euler angles.  
        '''
        self.thisptr.setState(state)

    def getSkeletonRootIdxs(self):
        '''
        Retrieves the first indices of each skeleton's DOF in the dart state 
        vector.

        For example, if this world has three skeletons and their dof are
        6, 1 and 2 then the mIndices goes like this: [0 6 7].
        '''
        return self.thisptr.getSkeletonRootIdxs()

    def getConfig(self, toEulerXYZ=True):
        '''
        Gets the current world configuration in Dart/Grip.  Note that dart's 
        internal state reprentation is in generalized coordinates, with 
        root link poses represented in exponential coordinates.  

        The returned config vector is the concatentation of the poses of
        each "skeleton" (body with possible links parameterized by DOF) 
        with DOF organized as follows:
        1-3: body orientation (exponential coordinates)
        4-6: body pose (exponential coordinates)
        7+: any child dofs, following the link order in the URDF

        For the skeleton ordering see getSkeletonRootIdxs

        :param toEulerXYZ: If true, convert the pose DOFs to exponential
                           coordinates, assuming euler XYZ ordering.
        '''
        return self.thisptr.getConfig(toEulerXYZ)

    def setConfig(self, config, fromEulerXYZ=True):
        '''
        Sets the current world configuration in Dart/Grip.  Note that dart's 
        internal state reprentation is in generalized coordinates, with 
        root link poses represented in exponential coordinates.  

        The provided config vector must be the concatentation of the poses of
        each "skeleton" (body with possible links parameterized by DOF) 
        with DOF organized as follows:
        1-3: body orientation (exponential coordinates)
        4-6: body pose (exponential coordinates)
        7+: any child dofs, following the link order in the URDF

        For the skeleton ordering see getSkeletonRootIdxs

        :param config: The desired configuration as a list or array
        :param fromEulerXYZ: If true, interpret the pose DOFs (each skeleton's
                             first 6 DOF) as euler angles (XYZ ordering), and
                             convert to exponential coordinates.
        '''
        self.thisptr.setConfig(config, fromEulerXYZ)

    def checkCollision(self):
        return self.thisptr.checkCollision()
