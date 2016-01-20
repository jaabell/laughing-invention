#!/usr/bin/python
import h5py
import sys
import numpy as np
import os
import re

# automatically find the script directory.
# sys.path.append("/home/yuan/Dropbox/3essi_self_verification/test_suite/scripts" )
# script_dir=sys.argv[1]
cur_dir=os.getcwd()
sep='test_cases'
test_DIR=cur_dir.split(sep,1)[0]
scriptDIR=test_DIR+'scripts'

sys.path.append(scriptDIR)

# import my own command line color function
from essi_max_disp_fun import find_max_disp
from mycolor_fun import *

# The allowable tolerance between the previous and current values.
# tolerance=1e-6;

file_in=open("previous_values.txt","r")
# ignore the first line.
file_in.readline()
# Input the 2nd line, which is the HDF5 output filename.
output_filename=file_in.readline()
current=find_max_disp(output_filename);
file_in.close()

old_stdout = sys.stdout
sys.stdout=open("previous_values.txt","w")
print '{0:e}'.format(current)
print output_filename,
# sys.stdout.close()
sys.stdout = old_stdout

# error = abs((current-previous)/previous);
# flag='failed';

# if(abs(error)<tolerance):
# 	flag ='pass'

# print headstart()   , "Running test cases..."
# print headlocation(), os.path.dirname(os.path.abspath(__file__))
# print headrun()     , "-----------Testing results-----------------"
# print headstep()    ,'{0}     {1}        {2}   {3}'.format('previous  ','current  ','error ','flag')
# if(abs(error)<1e-6):
# 	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous,current,error,flag)
# else:
# 	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous,current,error,flag)


