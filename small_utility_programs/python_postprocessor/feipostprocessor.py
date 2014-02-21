# -*- coding: utf-8 -*-
"""
Created on Wed Jun 27 21:53:04 2012

@author: jaabell
"""


from scipy import zeros, double
import os
import sys
import inspect 
    
def read_fei_dispalcement_output(filename, nodes = [], components = [], Nsteps = -1, silent = True):    

    print "read_fei_output:: Reading ", filename
    
    fileObject = open(filename,'r')
    Nnodes = len(nodes)
    Ncomp = len(components)
    NStepsInFile = 0
    for line in fileObject:
        NStepsInFile += 1
    fileObject.seek(0)
    line = fileObject.readline()
    fileObject.seek(0)
    
    #Input checking
    if(len(nodes) == 0):
        spline = line.split()
        Nnodes = (len(spline)-1)/7
        nodes = range(1,Nnodes+1)
        report("Auto-detected {} nodes in file. Outputting all.".format(Nnodes))
    if(len(components)==0):
        Ncomp = 7
        components = range(1,8)   # [1,2,3,4,5,6,7]
        report("Outputting all (7) components for each node.")
    if(Nsteps <= 0):
        Nsteps = NStepsInFile
        report("Auto-detected {} time-steps in file.".format(Nsteps))
    if(Nsteps > NStepsInFile):
        warning("Nsteps = {} requested but only {} steps in file.".format(Nsteps, NStepsInFile))
        Nsteps = NStepsInFile

    #Pre-allocae memory
    print "Nsteps = ", Nsteps
    print "Nnodes = ", Nnodes
    print "Ncomp  = ", Ncomp
    d = zeros((Nsteps,Nnodes*Ncomp))    
    t = zeros(Nsteps)
    step = 0
    try:
        for line in fileObject:
            spline = line.split()
            for i in range(Nnodes):
                for j in range(Ncomp):
                    n = nodes[i]
                    c = components[j]
                    t[step] = double(spline[0])
                    d[step, Ncomp*i + j ] = double(spline[7*n-7 + c])
                    pass
                pass
            step += 1
            if step >= Nsteps:
                break
    except:
        print "step = {}, Ncomp = {}, node = {}, comp = {}".format(step, Ncomp, n, c)
        print "i = {}, j = {}, [step, Ncomp*i + j ] = [{},{}]".format(i,j,step,Ncomp*i + j )
        print "d.shape = ", d.shape
        print "len(spline) = {}".format(len(spline)), "spline[7*n-7 + c] = spline[{}]".format(7*n-7 + c)
        error("")
        print sys.exc_info()[0]
        print "Returning empty arrays."
        fileObject.close()
        raise
    fileObject.close()
    return t, d

#For backwards compatibility 
def read_fei_output(filename, nodes = [], components = [], Nsteps = -1, silent = True):
    return read_fei_dispalcement_output(filename, nodes , components, Nsteps, silent )


#Functions to do meaningful error reporting    

def report(msg):
    callername = inspect.stack()[1][3]
    print callername+" : "+msg
    
def warning(msg):
    callername = inspect.stack()[1][3]
    print callername+" : Warning! "+msg
    
def error(msg):
    callername = inspect.stack()[1][3]
    print callername+" : ERROR! -> "+msg