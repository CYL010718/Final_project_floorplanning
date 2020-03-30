import drawSvg as draw
import numpy as np
import sys

inputBlockFile = sys.argv[1]
outputFile = sys.argv[2]
treeFile = sys.argv[3]

block = open(inputBlockFile)
out = open(outputFile)
tree = open(treeFile)

line = block.readline()
line = line.split()
line = np.array(line)


d = draw.Drawing(int(line[1]), int(line[2]))

d.append(draw.Rectangle(0,0, int(line[1]), int(line[2]), fill = 'white', stroke = 'black', stroke_width = '1'))

for line in out.readlines():
    line = line.split()
    line = np.array(line)
    if np.size(line) < 5:
        continue
    d.append(draw.Rectangle(int(line[1]),int(line[2]),int(line[3]) - int(line[1]),int(line[4]) - int(line[2]),fill = '#46A3FF', stroke = 'black', stroke_width = '5'))

for line in tree.readlines():
    line = line.split()
    line = np.array(line)
    if line[2] != '0':
        d.append(draw.Line(int(line[0]),int(line[1]),int(line[2]),int(line[3]),  fill = '#FF2D2D', stroke = '#FF2D2D', stroke_width = '5'))
    if line[4] != '0':
        d.append(draw.Line(int(line[0]),int(line[1]),int(line[4]),int(line[5]),  fill = '#2828FF', stroke = '#2828FF' , stroke_width = '5'))
    
    
d.savePng('myPng.png')
    
