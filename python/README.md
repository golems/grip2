
# Building the bindings:
First the main grip2 library must be built, which includes the grip-interface
shared library that the python bindings depend on.  The python setup
script currently points at the local build of this library (../lib/) rather
than the system-wide version, in order to save time while developing grip2 and
the bindings simultaneously.  Once this project stabilizes it's probably a
good idea to switch the build to depend on system-wide grip2.

$ python setup.py build 

# Installing the bidings:
$ python setup.py install 

# Running in-place:
$ export LD_LIBRARY_PATH=`pwd`/../lib
$ export PATH=$PWD/build/lib.linux-x86_64-2.7:$PATH
$ python main.py