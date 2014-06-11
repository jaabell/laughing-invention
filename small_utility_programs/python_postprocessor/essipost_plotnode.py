#!/usr/bin/python
#plot_node_output.py - Plot all DOF outputs in a subplot for a given node tag
import sys

#Import the HDF5 interface for python
import h5py

#Import the plotting tools
import matplotlib.pyplot as plt

def print_usage():
    print """
essipost_plotnode.py

Plot nodal DOF time-history outputs from a .h5.feioutput ESSI HDF5 database output.  Alternatively,
produce a pdf file of the plot.

Usage: 
    
    essipost_plotnode.py filename node_number [pdf_file]

Example: Plot for node number 10 in analysis_stage.h5.feioutput

    essipost_plotnode.py analysis_stage.h5.feioutput 10


---
CompGeoMech 2014 - Jose Abell
"""
    return 0

#Plot all dofs of this file and node
narg  =len(sys.argv)

if narg == 3:
    fileName = sys.argv[1]
    nodeTag = int(sys.argv[2])
    pdffile = ""
elif narg == 4:
    fileName = sys.argv[1]
    nodeTag = int(sys.argv[2])
    pdffile = sys.argv[3]
else:
    print_usage()
    exit(-1)





if h5py.is_hdf5(fileName): #Check if the file is valid

    #Open file and get model name, stage name and time vector
    f = h5py.File(fileName, "r")    
    modelName = f['Model_Name'][:]
    stageName = f['Stage_Name'][:]
    time = f['time'][:]

    #Print this info
    print "\nOpened file: {} \n".format(fileName)
    print "Model Name = ", modelName[0]
    print "Stage Name = ", stageName[0]
    
    #Get position, number of DOFS, and coordinates for this node
    pos = f['Model/Nodes/Index_to_Generalized_Displacements'][nodeTag]
    ndof = f['Model/Nodes/Number_of_DOFs'][nodeTag]
    pos_coord = f['Model/Nodes/Index_to_Coordinates'][nodeTag]
    coords = f['Model/Nodes/Coordinates'][pos_coord:(pos_coord+3)]
    
    #Print info
    print "Node number {} has {} DOFs and is located at ({}*m, {}*m, {}*m) \n".format(nodeTag, ndof, coords[0], coords[1], coords[2])
    
    #Get the displacements outputs for this node
    u = f['Model/Nodes/Generalized_Displacements'][pos:(pos+ndof), :]
    
    #Create a NDOFx1 subplot for showing the output time series
    plt.close("all")
    fig = plt.figure()
    for i in range(ndof):
        plt.subplot(ndof,1,1+i)
        plt.plot(time,u[i,:])
        plt.ylabel("DOF # {}".format(i+1))
        plt.grid()
    plt.xlabel("Time (s)")
    fig.suptitle("Node # {} at ({}*m,{}*m,{}*m)".format(nodeTag, ndof, coords[0], coords[1], coords[2]), fontsize=14)
    
    if len(pdffile) == 0:
        plt.show()
    else:
        plt.savefig(pdffile)
    plt.close()
else:
    print "The file ", fileName, " is not a valid HDF5 file."