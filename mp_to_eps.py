#!/usr/bin/env python3

import os
import sys

if not os.path.exists('.metaview'):
    os.mkdir('.metaview')

os.chdir('.metaview')

FileName = sys.argv[1]
File = open('../' + FileName, 'r')

mpFile = open(FileName, 'w')
mpFile.write('outputtemplate := "%j-%c.mps";\n')
lines = File.readlines()
mpFile.writelines(lines)

File.close()
mpFile.close()

os.system('mpost ' + FileName)
