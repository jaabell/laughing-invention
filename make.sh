#!/bin/tcsh
###############################################################################

# building supporting libraries
# 16 CPUs 
cd nDarray          ;    time make -j 16   ;   cd ..
cd ModifiedOpenSeesServices          ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_MaterialModeling   ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_FiniteElements     ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_Miscellaneous      ;    time make -j 16   ;   cd ..
cd small_utility_programs            ;    time make -j 16   ;   cd ..
cd UCB_Elements                      ;    time make -j 16   ;   cd ..
cd DSL                               ;    time make         ;   cd ..
dir lib
