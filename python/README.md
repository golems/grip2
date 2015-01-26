
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
This is a little trickly b/c it assumes that the grip2 libraries have been 
built in a directory called "build/lib".  If working with multiple build
directories, just symlink the desired one to "build" before running setup.py.

In the shell:
export LD_LIBRARY_PATH=`pwd`/../build/lib
export PATH=$PWD/`ls -d build/lib.*`:$PATH
export PYTHONPATH=$PWD/`ls -d build/lib.*`:$PYTHONPATH
python main.py

### NOTES ###
As of 10/16/2014 Grip2 is build against Dart 3.0.  There is an issue in this 
release (#122 on dart wiki) related to updating transforms for ball and free
joints.  Despite the fix that JS put in place, setting the base DOF on skeletons
wasn't working.  As a work-around I commented out the block containing calls
to these functions in setState:
updateTransform_Issue122  	(Skeleton.cpp:278)
updateVelocity_Issue122 	(Skeleton.cpp:279)

I believe this issue was addressed in the 4.0 release, so it should just work 
if/when we update grip2 to use dart 4.0.