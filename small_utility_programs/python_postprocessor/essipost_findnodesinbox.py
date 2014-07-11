#!/usr/bin/python
#plot_node_output.py - Plot all DOF outputs in a subplot for a given node tag
import sys

#Import the HDF5 interface for python
import h5py

#Import the plotting tools
import scipy as sp

def print_usage():
    print """
essipost_findnodesinbox.py
=======================

Find tags of all nodes defined within a box, write into standard output.

##Usage:
    
    essipost_findnodesinbox.py filename_in xmin xmax ymin ymax zmin zmax

##Example: Find all nodes within a unit-length side cube.

    essipost_findnodesinbox.py analysis_stage.h5.feioutput  0 1 0 1 0 1


---
CompGeoMech 2014 - Jose Abell
"""
    return 0

#Plot all dofs of this file and node
narg  =len(sys.argv)

if narg == 8:
    fileNameIn = sys.argv[1]
    xmin = float(sys.argv[2])
    xmax = float(sys.argv[3])
    ymin = float(sys.argv[4])
    ymax = float(sys.argv[5])
    zmin = float(sys.argv[6])
    zmax = float(sys.argv[7])
else:
    print_usage()
    exit(-1)

if h5py.is_hdf5(fileNameIn): #Check if the file is valid

    #Open f ile and get model name, stage name and time vector
    f = h5py.File(fileNameIn, "r")    
    modelName = f['Model_Name'][:]
    stageName = f['Stage_Name'][:]
    time = f['time'][:]

    index_to_coordindates = f['Model/Nodes/Index_to_Coordinates'][:]
    coordinates = f['Model/Nodes/Coordinates'][:]
    Nnodes = coordinates.size/3
    tags = sp.zeros(Nnodes,dtype=int)

    i = 0
    for tag in range(index_to_coordindates.size):
        pos = index_to_coordindates[tag]
        if pos >= 0:
            ix = (xmin <= coordinates[pos]) and (coordinates[pos] <= xmax)
            iy = (ymin <= coordinates[pos+1]) and (coordinates[pos+1] <= ymax)
            iz = (zmin <= coordinates[pos+2]) and (coordinates[pos+2] <= zmax)

            if ix and iy and iz:
                tags[i] = tag
                i += 1

    #Close HDF5 file
    ntags = i-1
    f.close()
    for i in range(ntags):
        sys.stdout.write(str(tags[i])+" ")
    

else:
    print "The file ", fileNameIn, " is not a valid HDF5 file."

