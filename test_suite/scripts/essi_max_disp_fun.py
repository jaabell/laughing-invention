#!/usr/bin/python
import h5py
import sys
import numpy as np

def find_max_disp(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# Select the first timestep.
	disp=dispall[:,0]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp

def find_max_disp_step1(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# Select the first timestep.
	disp=dispall[:,1]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp

def find_max_disp_step2(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# Select the second timestep.
	disp=dispall[:,2]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp


def find_max_disp_step3(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# Select the second timestep.
	disp=dispall[:,3]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp


def find_max_disp_step_mid(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# find the mid size:
	midsize=len(dispall[0,:])/2
	# Select the second timestep.
	disp=dispall[:,midsize]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp


def find_max_disp_step_end(h5in_file_name):

	# h5in_file_name=sys.argv[1]
	# h5in_file_name="t_1.h5.feioutput"

	h5file_in=h5py.File(h5in_file_name,"r")

	# [()] is a must. Otherwise, disp will be a dataset, not an array.
	dispall=h5file_in['/Model/Nodes/Generalized_Displacements'][()]

	# find the mid size:
	endsize=len(dispall[0,:])-2
	# Select the second timestep.
	disp=dispall[:,endsize]
	# axis =0 is a must. Otherwise, disp will be sorted along the first axis.
	disp=np.sort(disp,axis=0)

	# If disp[0] is negative, this will return the positive value.
	# Elseif disp[0] is positive, disp[0] will be the smaller positive value 
	# in the sorted array. 
	disp[0]=-disp[0]

	# disp[-1] is the last (biggest) element in the sorted array. 
	max_disp=max(disp[0],disp[-1])

	return max_disp