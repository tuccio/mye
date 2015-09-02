from PIL import Image
import os
import re

from normalHeightMap import MakeNormalHeightMap
	
if __name__ == '__main__':

	
	#MakeNormalHeightMap('lion_normalheight.png', 'lion_ddn.tga', 'lion_bump.png')
	
	for f in os.listdir('.'):
	
		m = re.search(r'(.+)_ddn\..+', f)
		
		if m:
		
			bump = m.group(1) + '_bump.png'
			
			if os.path.isfile(bump):
			
				MakeNormalHeightMap('crytek_sponza_' + m.group(1) + '_normalheight.png', f, bump)
			