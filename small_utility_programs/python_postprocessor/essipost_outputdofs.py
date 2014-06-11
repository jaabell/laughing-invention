#!/usr/bin/python
#plot_node_output.py - Plot all DOF outputs in a subplot for a given node tag
import sys

#Import the HDF5 interface for python
import h5py

#Import the plotting tools
import matplotlib.pyplot as plt

def print_usage():
    print """
essipost_outputnodes.py
=======================

Produce textual output for given nodes.

##Usage:
    
    essipost_outputnodes.py filename_in filename_out node_number1 node_number2....

##Example: Generate output for nodes 1 through 10 for database `analysis_stage.h5.feioutput` and store
    in `nodal_out.txt`

    essipost_outputnodes.py  analysis_stage.h5.feioutput nodal_out.txt 1 2 3 4 5 6 7 8 9 10 


---
CompGeoMech 2014 - Jose Abell
"""
    return 0

#Plot all dofs of this file and node
narg  =len(sys.argv)

if narg >= 4:
    fileNameIn = sys.argv[1]
    fileNameOut = sys.argv[2]
    nodeTags = []
    nargs = len(sys.argv)
    nnodes = nargs - 3
    print nnodes
    for i in range(nnodes):
        nodeTags.append(int(sys.argv[i+3]))
else:
    print_usage()
    exit(-1)


fid = open(fileNameOut,"w")



if h5py.is_hdf5(fileNameIn): #Check if the file is valid

    #Open f ile and get model name, stage name and time vector
    f = h5py.File(fileNameIn, "r")    
    modelName = f['Model_Name'][:]
    stageName = f['Stage_Name'][:]
    time = f['time'][:]

    #Print this info
    print "\nOpened file: {} \n".format(fileNameIn)
    print "Model Name = ", modelName[0]
    print "Stage Name = ", stageName[0]

    for nodeTag in nodeTags:
        
        #Get position, number of DOFS, and coordinates for this node
        pos = f['Model/Nodes/Index_to_Generalized_Displacements'][nodeTag]
        ndof = f['Model/Nodes/Number_of_DOFs'][nodeTag]
        pos_coord = f['Model/Nodes/Index_to_Coordinates'][nodeTag]
        coords = f['Model/Nodes/Coordinates'][pos_coord:(pos_coord+3)]
        
        #Print info
        print "Node number {} has {} DOFs and is located at ({}*m, {}*m, {}*m)".format(nodeTag, ndof, coords[0], coords[1], coords[2])
        
        #Get the displacements outputs for this node
        u = f['Model/Nodes/Generalized_Displacements'][pos:(pos+ndof), :]

        for dof in range(ndof):
            u[dof,:].tofile(fid, sep=" ")
            fid.write("\n")


    #Close HDF5 file
    f.close()
    

else:
    print "The file ", fileNameIn, " is not a valid HDF5 file."


fid.close()