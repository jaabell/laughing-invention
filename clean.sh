#!/bin/tcsh
###############################################################################
# # CLEANALL   CLEANALL  CLEANALL CLEANALL  CLEANALL   CLEANALL 
cd nDarray                           ;    make cleanall  ;   cd ..
cd ModifiedOpenSeesServices          ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_Miscellaneous      ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_MaterialModeling   ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_FiniteElements     ;    make cleanall  ;   cd .. 
cd CompGeoMechUCD_Miscellaneous      ;    make cleanall  ;   cd .. 
cd UCB_Elements                      ;    make cleanall  ;   cd .. 
cd DSL                               ;    make cleanall  ;   cd ..  
find . -name "*.o" -delete
find . -name "*.a" -delete
find . -name "log" -delete
find . -name "*.log" -delete
dir lib



