import numpy as np
import math
import json
import random

from pointsout import WriteCartesianPoints
from pointsout import Plot3DSpherePoints

def UniformPointSphereAnnulus(min, max):

	r     = np.random.uniform(min, max)
	theta = np.random.uniform(0, 2 * math.pi)
	phi   = np.random.uniform(0, math.pi)
	
	x = r * math.sin(theta) * math.cos(phi)
	z = r * math.sin(theta) * math.sin(phi)
	y = r * math.cos(theta)
	
	return np.array([x, y, z])
	
def UniformPointSphere():

	return UniformPointSphereAnnulus(0, 1)
	
def GeneratePoissonDiscSphere(N, r, k = 100):

	# http://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

	cellSize = r / math.sqrt(3)
	
	gridSize = int(math.ceil(2 / cellSize))
	
	grid    = np.empty((gridSize, gridSize, gridSize))
	grid[:] = -1
	
	samples = [ UniformPointSphere() ]
	
	active = [ 0 ]
	
	while active:
	
		activeIndex = random.randrange(0, len(active))
		
		i  = active[activeIndex]
		xi = samples[i]
		
		sampleFound = False
	
		for i in range(k):
		
			offset = UniformPointSphereAnnulus(r, 2 * r)
			sample = xi + offset
			
			sampleOriginDistance = np.linalg.norm(sample)
			
			if sampleOriginDistance <= 1:
			
				samplePosition = 0.5 * gridSize * (sample + np.array([1, 1, 1]))
				samplePosition = (int(samplePosition[0]), int(samplePosition[1]), int(samplePosition[2]))
				
				if grid[samplePosition] == -1:
				
					sampleFound = True
					
					newSampleIndex       = len(samples)
					grid[samplePosition] = newSampleIndex
					
					samples.append(sample)
					active.append(newSampleIndex)
					
					break
					
		if not sampleFound:
		
			del active[activeIndex]
			
		elif len(samples) == N:
			
			break
			
	return samples

if __name__ == '__main__':

	points = GeneratePoissonDiscSphere(16, 0.8)
	
	Plot3DSpherePoints(points)
	
	WriteCartesianPoints('sphere_poisson_disc_samples.txt', points)