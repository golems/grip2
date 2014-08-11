# distutils: language = c++

from libc.stdlib cimport malloc, free
from libcpp.string cimport string

cdef extern from "../include/GripInterface.h":
    cdef cppclass GripInterface:
        GripInterface() except +
        int some_var
        int create(int argc, char **argv)
        void load(string sceneFileName)
        int step()
        void render()

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

    def step(self):
        return self.thisptr.step()

    def create(self, args=None):
        if args is None:
            args = ''

        cdef char **c_argv

        # make *sure* that we have the exact string object for every argument
        #, and don't invoke __str__ on something else!!
        args = [bytes(x) for x in args]

        # or, use str(x).encode(...) above, depending on what API you want and what encoding C program expect
        c_argv = <char**>malloc(sizeof(char*) * len(args)) # + try/finally and free!!

        for idx, s in enumerate(args):
            c_argv[idx] = s

        int ret = self.thisptr.create(len(args), c_argv)
        free c_argv
        return ret

    def load(self, sceneFileName):
        self.thisptr.load(sceneFileName)
