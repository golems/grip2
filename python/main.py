
# import sys; sys.
from pygrip import PyGrip
import threading

if __name__ == '__main__':
	pg = PyGrip()
	print 'instantiated'
	pg.create()
	print 'created'
	pg.loadScene("/Users/jscholz/vc/my_robot_models/scenes/krang_techway.urdf")
	print 'loaded'

	import ipdb;ipdb.set_trace()