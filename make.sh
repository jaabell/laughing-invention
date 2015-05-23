#!/bin/tcsh
###############################################################################

rm ./bin/essi

# building supporting libraries
# 16 CPUs 
cd nDarray         	 				 ;    time make -j 16   ;   cd ..
cd ModifiedOpenSeesServices          ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_MaterialModeling   ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_FiniteElements     ;    time make -j 16   ;   cd ..
cd CompGeoMechUCD_Miscellaneous      ;    time make -j 16   ;   cd ..
cd small_utility_programs            ;    time make -j 16   ;   cd ..
cd UCB_Elements                      ;    time make -j 16   ;   cd ..
cd DSL                               ;    time make         ;   cd ..

echo ""

echo "Contents of "$PWD"/lib"
dir lib

echo ""
echo ""

set essiexecutable = $PWD/bin/essi
if ( -e $essiexecutable) then
	echo "Compilation successful! "
	echo "ESSI execcutable can be found in "$PWD"/bin"
else
	echo "Compilation FAILED."
endif 


