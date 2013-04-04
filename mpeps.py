#!/usr/bin/env python3

import sys
import os

#os.chdir('.metaview')

if os.system('mpost --halt-on-error ' + sys.argv[1]) > 0:
    sys.exit(1)
