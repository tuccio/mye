import argparse
import json
import os
import sys

if __name__ == '__main__':

	if not len(sys.argv) == 2:
		print('Error: Need to specify the new shader folder.')
		exit()
	
	for f in os.listdir('.'):
	
		name, ext = os.path.splitext(f)
		
		if ext.lower() == '.msh':
		
			shader = None
			
			with open(f, 'r+') as mshIn:
			
				shader = json.loads(mshIn.read())
					
				with open(name + '.old', 'w') as mshOld:
				
					out = json.dumps(shader, indent=4, separators=(',', ': '))
				
					mshOld.write(out)
					
				for attribute in shader:
					
					path, file = os.path.split(shader[attribute]['source'])
					
					shader[attribute]['source'] = sys.argv[1] + '/' + file
					
				mshIn.close()
					
				with open(f, 'w') as mshNew:
				
					out = json.dumps(shader, indent=4, separators=(',', ': '))
				
					mshNew.write(out)
					