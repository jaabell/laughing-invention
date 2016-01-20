#!/usr/bin/python
import h5py
import sys
import numpy as np
import os

# automatically find the script directory.
# sys.path.append("/home/yuan/Dropbox/3essi_self_verification/test_suite/scripts" )
# script_dir=sys.argv[1]
cur_dir=os.getcwd()
sep='test_cases'
test_DIR=cur_dir.split(sep,1)[0]
scriptDIR=test_DIR+'scripts'

sys.path.append(scriptDIR)

# import my own command line color function
# from essi_max_disp_fun import find_max_disp
from mycolor_fun import *


print headstart()   , "Running test cases..."
print headlocation(), os.path.dirname(os.path.abspath(__file__))



