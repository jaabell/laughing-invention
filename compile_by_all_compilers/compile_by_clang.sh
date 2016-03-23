#!/bin/sh
###############################################################################
# Dec 30 2015 Yuan 
# this script will compile ESSI by clang in -O2 mode..
# this script requires the Makefile.clang.Compilers file in this folder.
# this script will change and restore the make.sh file
# this script will extract the warnings to the RealESSI root folder.

cd ..
# RealESSI_SRC=${PWD}
# Rename the compiler setting for GNU
mv Makefile.Compilers Makefile.GNU.Compilers 

# copy out the compiler setting for Intel
cp ./compile_by_all_compilers/Makefile.clang.Compilers Makefile.Compilers

# rename the final executable essi to essi_clang
cd DSL
sed -i "s/\/\$\@/\/essi_clang/" Makefile
cd ..
sed -i "s/\/essi/\/essi_clang/" make.sh


# compile by the Intel compiler
script -c "tcsh make.sh" warn_clang.log

aha -f warn_clang.log > warn_clang.html

# Restore to the original state, 
# because we do not always need different compilers. 
# keep the ESSI folder clean and simple.
rm Makefile.Compilers
mv Makefile.GNU.Compilers Makefile.Compilers 
cd DSL
sed -i "s/essi_clang/\$\@/" Makefile
cd ..
sed -i "s/essi_clang/essi/" make.sh


