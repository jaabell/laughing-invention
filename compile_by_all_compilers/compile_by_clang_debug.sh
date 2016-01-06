#!/bin/sh
###############################################################################
# Dec 30 2015 Yuan 
# this script will compile ESSI by clang in debug mode.
# this script requires the Makefile.clang.debug.Compilers file in this folder.
# this script will change and restore the make.sh file
# this script will extract the warnings to the RealESSI root folder.

cd ..
# RealESSI_SRC=${PWD}
# Rename the compiler setting for GNU
mv Makefile.Compilers Makefile.GNU.Compilers 

# copy out the compiler setting for clang
cp ./compile_by_all_compilers/Makefile.clang.debug.Compilers Makefile.Compilers

# rename the final executable essi to essi_clang_debug
cd DSL
sed -i "s/\/\$\@/\/essi_clang_debug/" Makefile
cd ..
sed -i "s/\/essi/\/essi_clang_debug/" make.sh


# compile by the Intel compiler
script -c "tcsh make.sh" warn_clang_debug.log

aha -f warn_clang_debug.log > warn_clang_debug.html


# Restore to the original state, 
# because we do not always need different compilers. 
# keep the ESSI folder clean and simple.
rm Makefile.Compilers
mv Makefile.GNU.Compilers Makefile.Compilers 
cd DSL
sed -i "s/essi_clang_debug/\$\@/" Makefile
cd ..
sed -i "s/essi_clang_debug/essi/" make.sh


