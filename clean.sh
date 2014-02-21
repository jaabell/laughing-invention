#!/bin/tcsh
###############################################################################
# # CLEANALL   CLEANALL  CLEANALL CLEANALL  CLEANALL   CLEANALL 
cd ModifiedOpenSeesServices          ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_Miscellaneous      ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_MaterialModeling   ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_FiniteElements     ;    make cleanall  ;   cd .. 
cd UCB_Elements                      ;    make cleanall  ;   cd .. 
cd DSL                               ;    make cleanall  ;   cd ..  
dir lib



