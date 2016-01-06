#!/bin/sh
###############################################################################
# Dec 30 2015 Yuan 

# This script wraps the previous "make.sh", which is 
# the compile by g++ in no_debug mode

cd ..

script -c "tcsh make.sh" warn_gcc.log
aha -f warn_gcc.log > warn_gcc.html



cd compile_by_all_compilers