        
cdef extern from "../include/TestMainWindow.h" namespace "TestMainWindow":
    void TestSlot()

cpdef test_slot():
    print 'calling real testslot function'
    TestSlot()

cdef extern from "../include/TestMainWindow.h" namespace "TestMainWindow":
    void sayhello()

cpdef say_hello():
    print 'calling real sayhello function'
    sayhello()