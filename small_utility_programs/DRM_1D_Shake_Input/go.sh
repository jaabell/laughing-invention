#!/bin/bash
###############################################################################
# Written by: Nima Tafazzoli October 2010

make clean
make
DRM_1D_Shake_Input drmnodes.dat Input_Disp_1000.txt Input_Acc_1000.txt
