#!/bin/tcsh
###############################################################################
# Dec 30 2015 Yuan 
# this script will compile ESSI by four compiler (gcc, clang, icc, pgi). 
# this script will compile both no_debug and debug modes.
# this script will extract all the warnings to the folder ./compile_by_all_compilers/warnings

# ----------------------------
#// compile by gcc no_debug
# ----------------------------
tcsh clean.sh
cd compile_by_all_compilers
sh compile_by_gcc.sh
cd ..

# ----------------------------
#// compile by gcc debug
# ----------------------------
tcsh clean.sh
cd compile_by_all_compilers
sh compile_by_gcc_debug.sh
cd ..

# ----------------------------
#// compile by clang no_debug
# ----------------------------
tcsh clean.sh
cd compile_by_all_compilers
sh compile_by_clang.sh
cd ..
# ----------------------------
#// compile by clang debug
# ----------------------------
tcsh clean.sh
cd compile_by_all_compilers
sh compile_by_clang_debug.sh
cd ..


# ----------------------------
#// compile by icc no_debug
# ----------------------------
# tcsh clean.sh
# cd compile_by_all_compilers
# sh compile_by_icc.sh
# cd ..
# ----------------------------
#// compile by icc debug
# ----------------------------
# tcsh clean.sh
# cd compile_by_all_compilers
# sh compile_by_icc_debug.sh
# cd ..


# ----------------------------
#// compile by pgi no_debug
# ----------------------------
# tcsh clean.sh
# cd compile_by_all_compilers
# sh compile_by_pgi.sh
# cd ..
# ----------------------------
#// compile by pgi debug
# ----------------------------
# tcsh clean.sh
# cd compile_by_all_compilers
# sh compile_by_pgi_debug.sh
# cd ..


mv warn*html ./compile_by_all_compilers/warnings


