
# import sys; sys.
from pygrip import PyGrip
import threading

if __name__ == '__main__':
	pg = PyGrip()
	print 'instantiated'
	pg.run()
	print 'running'
	#pg.loadScene("/Users/jscholz/vc/my_robot_models/scenes/krang_techway.urdf")
	pg.loadScene("/home/jscholz/vc/kore/etc/scenes/02-World-Collision.urdf")
	print 'loaded'

	import ipdb;ipdb.set_trace()