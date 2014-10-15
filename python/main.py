
import sys
import time
from pygrip import PyGrip
import numpy as np

# Note: for this to work we currently have to set the python and library
# paths manually:
# $ export LD_LIBRARY_PATH=`pwd`/../lib
# $ ADD_TO_PATH_VAR PYTHONPATH ./build/lib.linux-x86_64-2.7/

if __name__ == '__main__':
	pg = PyGrip()
	pg.run()

	## Test setting state:
	pg.loadScene("/home/jscholz/vc/kore/etc/scenes/01-World-Robot.urdf")
	state = [-2.221441469079183, 2.221441469079183, -0.0, -0.2998945983256897, -0.2998945983256897, 0.0, 0.019547687622336492, 0.0, 1.8542377973187758, 0.0, -0.5658357434965616, 0.0, 0.0, -0.7674212721019067, 0.0, -0.6791076119509937, 0.0, -1.0014699247943464, 0.0, 0.0, 0.0, 0.0, 1.0752973521537064, 0.0, 0.7358308126408094, 0.0, 1.0752973521537064, 1.0534807365037773, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
	pg.setState(state)

	## Test getting state:
	# pg.loadScene("/Users/jscholz/vc/my_robot_models/scenes/krang_techway.urdf")
	# pg.loadScene("/home/jscholz/vc/kore/etc/scenes/02-World-Collision.urdf")
	# time.sleep(1)
	# pg.startSimulation()
	# np.set_printoptions(suppress=True, precision=3)
	# for i in range(100):
	# 	print "state: ", np.array(pg.getState())
	# 	time.sleep(0.05)
	# pg.stopSimulation()

	# pg.loadScene("/home/jscholz/vc/kore/etc/scenes/00-World-Test.urdf")
	# ns = np.array(pg.getState())

	import ipdb;ipdb.set_trace()