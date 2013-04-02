#!/usr/bin/env python3

import sys
import os

os.chdir('.metaview')

epsFileName = sys.argv[1]
epsFile = open(epsFileName, 'r')

lines = epsFile.readlines()
boundingBox = lines[2].split()
llx = boundingBox[1]
lly = boundingBox[2]
urx = boundingBox[3]
ury = boundingBox[4]
height = float(ury) - float(lly)
width = float(urx) - float(llx)

texFileName = epsFileName[:len(epsFileName)-4] + '.tex'
texFile = open(texFileName, 'w')

texFile.write('\documentclass{article}\n')
texFile.write('\setlength\\hoffset{-87pt}\n')
texFile.write('\setlength\\voffset{-72.5pt}\n')
texFile.write('\setlength\\oddsidemargin{0pt}\n')
texFile.write('\setlength\\topmargin{0pt}\n')
texFile.write('\setlength\\headheight{0pt}\n')
texFile.write('\setlength\\headsep{0pt}\n')
texFile.write('\setlength\\paperwidth{' + str(width+20) + 'pt}\n')
texFile.write('\setlength\\paperheight{' + str(height) + 'pt}\n')
#texFile.write('\setlength\\textwidth{' + str(width+20) + 'pt}\n')
#texFile.write('\setlength\\textheight{' + str(height) + 'pt}\n')
texFile.write('\setlength\\marginparsep{0pt}\n')
texFile.write('\setlength\\marginparwidth{0pt}\n')
texFile.write('\setlength\\footskip{0pt}\n')
texFile.write('\\usepackage{graphicx}\n')
texFile.write('\DeclareGraphicsRule{*}{mps}{*}{}\n')
texFile.write('\\pagestyle{empty}\n')
texFile.write('\\begin{document}%\n')
texFile.write('\includegraphics{' + epsFileName + '}%\n')
texFile.write('\end{document}\n')

texFile.close()

os.system('pdflatex ' + texFileName)
