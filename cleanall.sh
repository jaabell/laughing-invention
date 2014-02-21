#!/bin/tcsh
###############################################################################
# # CLEANALL   CLEANALL  CLEANALL CLEANALL  CLEANALL   CLEANALL 
cd CBLAS                             ;    make cleanall  ;   cd ..
cd nDarray                           ;    make cleanall  ;   cd ..
cd LAPACK3.4.1                       ;    make cleanall  ;   cd ..
cd SuperLU                           ;    make cleanall  ;   cd ..
cd UMFPACK5.5.1                      ;    make cleanall  ;   cd ..
cd ARPACK                            ;    make cleanall  ;   cd ..
cd ModifiedOpenSeesServices          ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_Miscellaneous      ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_MaterialModeling   ;    make cleanall  ;   cd ..
cd CompGeoMechUCD_FiniteElements     ;    make cleanall  ;   cd .. 
cd CompGeoMechUCD_Miscellaneous      ;    make cleanall  ;   cd .. 
cd UCB_Elements                      ;    make cleanall  ;   cd .. 
cd DSL                               ;    make cleanall  ;   cd ..  
dir lib



