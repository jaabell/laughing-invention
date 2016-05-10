#!/bin/sh
###############################################################################
# Dec 30 2015 Yuan 
# this script will compile ESSI by gcc in debug mode..
# this script requires the Makefile.gcc.debug.Compilers file in this folder.
# this script will change and restore the make.sh file
# this script will extract the warnings to the RealESSI root folder.

cd ..
# RealESSI_SRC=${PWD}
# Rename the compiler setting for GNU
mv Makefile.Compilers Makefile.GNU.Compilers 

# copy out the compiler setting for gcc
cp ./compile_by_all_compilers/Makefile.gcc.debug.Compilers Makefile.Compilers

# rename the final executable essi to essi_gcc_debug
cd DSL
sed -i "s/\/\$\@/\/essi_gcc_debug/" Makefile
cd ..
sed -i "s/\/essi/\/essi_gcc_debug/" make.sh


# compile by the gcc compiler
script -c "tcsh make.sh" warn_gcc_debug.log

aha -f warn_gcc_debug.log > warn_gcc_debug.html


# Restore to the original state, 
# because we do not always need different compilers. 
# keep the ESSI folder clean and simple.
rm Makefile.Compilers
mv Makefile.GNU.Compilers Makefile.Compilers 
cd DSL
sed -i "s/essi_gcc_debug/\$\@/" Makefile
cd ..
sed -i "s/essi_gcc_debug/essi/" make.sh


