import math
import sys
import os

def gauss(distance, kernel):

	s2 = sigma * sigma
	G  = 1 / math.sqrt(2 * math.pi * s2)

	return G * math.exp(- .5 * (distance * distance) / s2)
	
if __name__ == '__main__':

	if not len(sys.argv) == 3:
	
		print('Usage: ' + os.path.split(sys.argv[0])[1] + ' <kernel size> <sigma>')
	
	else:
	
		kernelSize = int(sys.argv[1])
		sigma      = float(sys.argv[2])
		
		halfKernel = int(math.floor(kernelSize / 2))
		
		G = [ gauss(offset, sigma) for offset in xrange(- halfKernel, halfKernel + 1) ]
		
		with open('gaussian_kernel.txt', 'w') as out:
		
			out.write(str(G)[1:-1])