import numpy as np
import math
import json
import random

from pointsout import WriteCartesianPoints
from pointsout import Plot3DSpherePoints

def normalize(x):
	return x / np.linalg.norm(x)

def CosineWeightedRandomHemisphereDirection():

	# https://pathtracing.wordpress.com/2011/03/03/cosine-weighted-hemisphere/

	r1 = np.random.uniform(0, 1)
	r2 = np.random.uniform(0, 1)
	
	theta = math.acos(math.sqrt(1 - r1))
	phi   = 2 * math.pi * r2
	
	xs = math.sin(theta) * math.cos(phi)
	ys = math.cos(theta)
	zs = math.sin(theta) * math.sin(phi)
	
	return np.array([xs, ys, zs])
	
def ProjectToHemisphereDirection(N, s):

	h = np.copy(N)
	y = np.copy(N)
	
	if abs(h[0]) <= abs(h[1]) and abs(h[0]) <= abs(h[2]):
		h[0] = 1
	elif abs(h[1]) <= abs(h[0]) and abs(h[1]) <= abs(h[2]):
		h[1] = 1
	else:
		h[2] = 1
		
	x = normalize(np.cross(h, y))
	z = normalize(np.cross(x, y))
	
	direction = normalize(s[0] * x + s[1] * y + s[2] * z)
	
	return direction
	
def GenerateCosineWeightedDirections(count):

	return [ ProjectToHemisphereDirection(np.array([0, 0, 1]), CosineWeightedRandomHemisphereDirection()) for i in range(count) ]
	
def UniformPointHemisphere():

	r     = np.random.uniform(0, 1)
	theta = np.random.uniform(0, math.pi)
	phi   = np.random.uniform(0, math.pi)
	
	x = r * math.sin(theta) * math.cos(phi)
	z = r * math.sin(theta) * math.sin(phi)
	y = r * math.cos(theta)
	
	return np.array([x, y, z])	
	
def GenerateUniformPointsHemisphere(count):

	return [ UniformPointHemisphere() for i in range(count) ]

if __name__ == '__main__':

	points  = GenerateCosineWeightedDirections(16)
	
	#points = GenerateUniformPointsHemisphere(16)
	
	Plot3DSpherePoints(points)
	
	WriteCartesianPoints('hemishphere_samples.txt', points)