# distutils: language = c++

from libc.stdlib cimport malloc, free

cdef extern from "../include/GripInterface.h":
    cdef cppclass GripInterface:
        GripInterface() except +
        int some_var
        int run(int argc, char **argv)
        int step()
        void render()

# Place static interface declarations here
cdef extern from "../include/GripInterface.h" namespace "GripInterface":
    void show_usage()
    void sayhello()

cdef class PyGrip:
    cdef GripInterface *thisptr      # hold a C++ instance which we're wrapping

    property some_var:
        def __get__(self): return self.thisptr.some_var
        def __set__(self, some_var): self.thisptr.some_var = some_var

    def __cinit__(self):
        self.thisptr = new GripInterface()

    def __dealloc__(self):
        del self.thisptr

    def step(self):
        return self.thisptr.step()

    def run(self, args):
        cdef char **c_argv

        # make *sure* that we have the exact string object for every argument
        #, and don't invoke __str__ on something else!!
        args = [bytes(x) for x in args]

        # or, use str(x).encode(...) above, depending on what API you want and what encoding C program expect
        c_argv = <char**>malloc(sizeof(char*) * len(args)) # + try/finally and free!!

        for idx, s in enumerate(args):
            c_argv[idx] = s

        return self.thisptr.run(len(args), c_argv)
