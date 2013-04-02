#!/usr/bin/env python3

import sys
import os
import shutil

shutil.copy(sys.argv[1], '.metaview');
os.chdir('.metaview')

os.system('mpost ' + sys.argv[1])
