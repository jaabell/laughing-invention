#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 17 10:56:48 2013

@author: jaabell
"""

import sys
import os
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("filename", help="name of the file to process")

parser.parse_args()



def print_usage():
    print "plot_fei_displacements"
    print "-----------------------------------------------------------"
    print "Part of the ESSI simulator python postprocessor"
    print ""
    print "Usage:"
    print "$ plot_fei_displacements filename node_number [node_number...] dof <dof dof ... up to 7> "
    print "  dof takes values from [ux uy uz Ux Uy Uz rx ry rz p]"
    print ""
    print "Example: plot dofs ux and uy for nodes 10 and 12"
    print "  plot_fei_displacements analysis.feioutput 10 12 ux uy "


def dof_2_number(dof):
    retval = -1
    if dof == "ux":
        retval = 1
    elif dof == "uy":
        retval = 2
    elif dof == "uz":
        retval = 3
    elif dof == "Ux":
        retval = 4
    elif dof == "Uy":
        retval = 5
    elif dof == "Uz":
        retval = 6
    elif dof == "rx":
        retval = 4    
    elif dof == "ry":
        retval = 5 
    elif dof == "rz":
        retval = 6
    elif dof == "p":
        retval = 7
    
    if retval == -1:
        print "Unknown dof ", dof
        print_usage()
        exit(-1)
    
    return retval

# ============================================================================
#   
# ============================================================================

if len(sys.argv) < 3:
    print_usage()
else:
