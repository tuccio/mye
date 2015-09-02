from PIL import Image
import os

# A function to merge normal maps and height maps into a single 4 channel image

def MakeNormalHeightMap(filename, normalMap, heightMap):

	normalMap = Image.open(normalMap)
	heightMap = Image.open(heightMap)
	
	nx, ny, nz = normalMap.split()
	
	result = Image.merge("RGBA", (nx, ny, nz, heightMap))
	result.save(filename, os.path.splitext(filename)[1][1:])