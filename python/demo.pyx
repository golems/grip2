
from libc.stdlib cimport malloc, free

cdef extern from "../include/gripInterface.h":
    int run(int argc, char **argv)

cpdef prun(args):
     cdef char **c_argv

     # make *sure* that we have the exact string object for every argument
     #, and don't invoke __str__ on something else!!
     args = [b'calling_from_cython'] + [bytes(x) for x in args]

     # or, use str(x).encode(...) above, depending on what API you want and what encoding C program expect
     c_argv = <char**>malloc(sizeof(char*) * len(args)) # + try/finally and free!!

     for idx, s in enumerate(args):
         c_argv[idx] = s

     run(len(args), c_argv)