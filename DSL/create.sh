#!/bin/sh
#
# FEI parser source code creator.
#
# This small script invokes 'flex' and 'bison' on the 
# lexer definition file (feiparser.l) and on the 
# grammar definition file (feiparser.yy) to generate
# the lexer and parser C++ code.
#
# Effects:
# This script generates the following files:
#
#   * Creates the following files
#    + feiparser.lex.cpp          : Lexer cpp implementation
#    + feiparser.tab.hh           : Parser cpp header file
#    + feiparser.tab.cpp          : Parser cpp implementation
#    + location.hh		  : 
#    + position.hh		  : 
#    + stack.hh			  : 
#
# This file should be run to update the above files whenever
# feiparser.l and/or feiparser.yy are changed.
#
# 2012 UCD CompGeoMech - Jose Abell 

echo Creating the executables ...

bison -vd feiparser.yy
flex -ofeiparser.lex.c feiparser.l

mv feiparser.tab.cc feiparser.tab.cpp
mv feiparser.lex.c feiparser.lex.cpp

echo Done
