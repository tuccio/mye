import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def WriteCartesianPoints(fn, points):

	with open(fn, 'w') as out:
	
		out.write('{\n')
		
		for p in points:
		
			out.write('\t{ ' + str(p[0]) + ', ' + str(p[1]) + ', ' + str(p[2]) + ' },\n')
			
		out.write('}')
		
def Plot3DSpherePoints(points):
	
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	
	# draw sphere
	u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
	
	xs = np.cos(u) * np.sin(v)
	ys = np.sin(u) * np.sin(v)
	zs = np.cos(v)
	
	ax.plot_wireframe(xs, ys, zs, color='g')
	
	# draw points
	
	x = [ p[0] for p in points ]
	y = [ p[1] for p in points ]
	z = [ p[2] for p in points ]
	
	plt.plot(x, y, z, 'ro')
	
	plt.show()	