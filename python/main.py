
# import sys; sys.
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
	pg.loadScene("/home/jscholz/vc/kore/etc/scenes/02-World-Collision.urdf")
	state = [2.012993984976851, -2.012993984976851, -0.3880059162624672, 0.25116356559559316, 0.2923448103481566, 0.05634965471842588, 0.019547687622336492, 0.0, 1.8542377973187758, 0.0, -0.5658357434965616, 0.0, 0.0, -0.7674212721019067, 0.0, -0.6113888369736137, 0.0, -1.0014699247943464, 0.0, 0.0, 0.0, 0.0, 0.0, 0.7154104603924757, 0.0, 0.6113888369736137, 0.0, 1.0534807365037773, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.2092005444063132, 1.2092049860560816, 1.2092005444063132, -1.4278191914231189, 0.7124639919814808, -0.48464862974945033, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.2092005444063132, 1.2092049860560816, 1.2092005444063132, -0.4484057508771614, 1.6918751227371513, -1.113471913263889, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
	pg.setState(state)

	## Test getting state:
	# pg.loadScene("/Users/jscholz/vc/my_robot_models/scenes/krang_techway.urdf")
	# pg.loadScene("/home/jscholz/vc/kore/etc/scenes/02-World-Collision.urdf")
	pg.startSimulation()
	np.set_printoptions(suppress=True, precision=3)
	for i in range(100):
		print "state: ", np.array(pg.getState())
	pg.stopSimulation()

	# pg.loadScene("/home/jscholz/vc/kore/etc/scenes/00-World-Test.urdf")
	# ns = np.array(pg.getState())

	import ipdb;ipdb.set_trace()