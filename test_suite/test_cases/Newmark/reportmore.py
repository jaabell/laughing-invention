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
from essi_max_disp_fun import *
from mycolor_fun import *


# The allowable tolerance between the previous and current values.
tolerance=1e-6;

# print headstart()   , "Running test cases..."
# print headlocation(), os.path.dirname(os.path.abspath(__file__))

file_in=open("previous_values.txt","r")
# Input the first 5 line, which is the previous value.
previous1= float(file_in.readline())
previous2= float(file_in.readline())
previous3= float(file_in.readline())
previous_mid= float(file_in.readline())
previous_end= float(file_in.readline())

# Input the next line and extract values at 5 steps, which is the HDF5 output filename.
h5name=file_in.readline();
current1=find_max_disp_step1(h5name);
current2=find_max_disp_step2(h5name);
current3=find_max_disp_step3(h5name);
current_mid=find_max_disp_step_mid(h5name);
current_end=find_max_disp_step_end(h5name);
file_in.close()

error1 = abs((current1-previous1)/previous1);
error2 = abs((current2-previous2)/previous2);
error3 = abs((current3-previous3)/previous3);
error4 = abs((current_mid-previous_mid)/previous_mid);
error5 = abs((current_end-previous_end)/previous_end);
flag1='bad_step';
flag2='bad_step';
flag3='bad_step';
flag4='bad_step';
flag5='bad_step';

if(abs(error1)<tolerance):
	flag1 ='good_step'
if(abs(error2)<tolerance):
	flag2 ='good_step'
if(abs(error3)<tolerance):
	flag3 ='good_step'
if(abs(error4)<tolerance):
	flag4 ='good_step'
if(abs(error5)<tolerance):
	flag5 ='good_step'

print headrun()     , "-----------Testing results-----------------"
print headstep()    ,'{0}     {1}        {2}   {3}'.format('previous  ','current  ','error ','flag')
if(abs(error1)<tolerance):
	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous1,current1,error1,flag1)
else:
	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous1,current1,error1,flag1)

if(abs(error2)<tolerance):
	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous2,current2,error2,flag2)
else:
	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous2,current2,error2,flag2)

if(abs(error3)<tolerance):
	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous3,current3,error3,flag3)
else:
	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous3,current3,error3,flag3)

if(abs(error4)<tolerance):
	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_mid,current_mid,error4,flag4)
else:
	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_mid,current_mid,error4,flag4)

if(abs(error5)<tolerance):
	print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_end,current_end,error5,flag5)
else:
	print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_end,current_end,error5,flag5)

caseflag='This case failed.';
if (abs(error1)<tolerance and abs(error2)<tolerance and abs(error3)<tolerance and abs(error4)<tolerance and abs(error5)<tolerance ):
	caseflag='pass case.';
	print headOK()      ,'{0}'.format(caseflag)
else: 
	print headFailed()  ,'{0}'.format(caseflag)

	
	# print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous2,current2,error2,flag2)
	# print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous3,current3,error3,flag3)
	# print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_mid,current_mid,error4,flag4)
	# print headOK()      ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_end,current_end,error5,flag5)

	# print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous2,current2,error2,flag2)
	# print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous3,current3,error3,flag3)
	# print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_mid,current_mid,error4,flag4)
	# print headFailed()  ,'{0:e}   {1:e}     {2:0.2f}     {3}'.format(previous_end,current_end,error5,flag5)


