#!/bin/sh
###############################################################################
# Dec 30 2015 Yuan 
# this script will compile ESSI by pgi in -O2 mode..
# this script requires the Makefile.pgi.Compilers file in this folder.
# this script will change and restore the make.sh file
# this script will extract the warnings to the RealESSI root folder.
cd ..
# RealESSI_SRC=${PWD}
# Rename the compiler setting for GNU
mv Makefile.Compilers Makefile.GNU.Compilers 

# copy out the compiler setting for PGI
cp ./compile_by_all_compilers/Makefile.PGI.Compilers Makefile.Compilers

# rename the final executable essi to essi_pgi
cd DSL
sed -i "s/\/\$\@/\/essi_pgi/" Makefile
# sed -i "/\%\.o\ \:\ \%\.cpp/i \CC++=g++" Makefile
# sed -i "/\%\.o\ \:\ \%\.cpp/i \C++FLAGS= -O2 -std=c++11" Makefile

cd ..
sed -i "s/\/essi/\/essi_pgi/" make.sh


# compile by the PGI compiler
script -c "tcsh make.sh" warn_pgi.log

aha -f warn_pgi.log > warn_pgi.html


# Restore to the original state, 
# because we do not always need different compilers. 
# keep the ESSI folder clean and simple.
rm Makefile.Compilers
mv Makefile.GNU.Compilers Makefile.Compilers 
cd DSL
sed -i "s/essi_pgi/\$\@/" Makefile
# sed -i "/CC++=g++/ d" Makefile
# sed -i "/C++FLAGS= -O2 -std=c++11/ d" Makefile
cd ..
sed -i "s/essi_pgi/essi/" make.sh


