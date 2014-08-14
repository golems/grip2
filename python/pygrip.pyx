# distutils: language = c++

from libc.stdlib cimport malloc, free
from libcpp.string cimport string

cdef extern from "../include/GripInterface.h":
    cdef cppclass GripInterface:
        GripInterface() except +
        int some_var
        int _create(int argc, char **argv)
        int run(int argc, char **argv)
        void loadScene(string sceneFileName)
        void loadPluginFile(string pluginFileName)
        void render()
        void startSimulation()
        void stopSimulation()
        void simulateSingleStep()     

# Place static interface declarations here
cdef extern from "../include/GripInterface.h" namespace "GripInterface":
    void show_usage()

cdef class PyGrip:
    '''
    Main Python class for interfacing to Grip.
    Implements a set of pass-through methods for interacting with 
    libgrip-interface.
    '''
    cdef GripInterface *thisptr      # hold a C++ instance which we're wrapping

    property some_var:
        def __get__(self): return self.thisptr.some_var
        def __set__(self, some_var): self.thisptr.some_var = some_var

    def __cinit__(self):
        self.thisptr = new GripInterface()

    def __dealloc__(self):
        del self.thisptr

    def usage(self):
        '''
        Prints GripInterface usage string
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
            free(c_argv)
        return ret

    def loadScene(self, sceneFileName):
        self.thisptr.loadScene(sceneFileName)

    def loadPluginFile(self, pluginFileName):
        self.thisptr.loadPluginFile(pluginFileName)

    def render(self):
        self.thisptr.render()

    def startSimulation(self):
        self.thisptr.startSimulation()

    def stopSimulation(self):
        self.thisptr.stopSimulation()

    def simulateSingleStep(self):
        self.thisptr.simulateSingleStep()
