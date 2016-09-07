/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.3 $
// $Date: 2003/11/18 01:59:04 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/subdomain/ShadowActorSubdomain.h,v $


// Written: fmk
// Revision: A
//
// Description: This file contains the integer codes used in ShadowSubdomain
// and the ActorSubdomain classes.
//
// What: "@(#) ShadowSubdomain.h, revA"
static const int Master_addNodalLoadToPattern                          = 0 ;
static const int Master_addSP_Constraint                               = 1 ;
static const int Master_addSP_ConstraintToPattern                      = 2 ;
static const int Master_hasNode                                        = 3 ;
static const int ShadowActorSubdomain_addElement                       = 4 ;
static const int ShadowActorSubdomain_addElementalLoadToPattern        = 5 ;
static const int ShadowActorSubdomain_addExternalNode                  = 6 ;
static const int ShadowActorSubdomain_addLoadPattern                   = 7 ;
static const int ShadowActorSubdomain_addMP_Constraint                 = 8 ;
static const int ShadowActorSubdomain_addNodalLoadToPattern            = 9 ;
static const int ShadowActorSubdomain_addNode                          = 10;
static const int ShadowActorSubdomain_addRecorder                      = 11;
static const int ShadowActorSubdomain_addSP_Constraint                 = 12;
static const int ShadowActorSubdomain_addSP_ConstraintToPattern        = 13;
static const int ShadowActorSubdomain_applyLoad                        = 14;
static const int ShadowActorSubdomain_BuildElementGraph                = 15; //Guanzhou added
static const int ShadowActorSubdomain_buildSubdomain                   = 16;
static const int ShadowActorSubdomain_ChangeMPIChannel                 = 17;
static const int ShadowActorSubdomain_clearAnalysis                    = 18;
static const int ShadowActorSubdomain_commit                           = 19;
static const int ShadowActorSubdomain_output_iteration	               = 20;  // Sumeet Added [September, 2016] for writing itearations to HDF5 file
static const int ShadowActorSubdomain_output_step                      = 21;  // Sumeet Added [September, 2016] for writing converged step	
static const int ShadowActorSubdomain_computeNodalResponse             = 22;
static const int ShadowActorSubdomain_computeResidual                  = 23;
static const int ShadowActorSubdomain_computeTang                      = 24;
static const int ShadowActorSubdomain_DIE                              = 25;
static const int ShadowActorSubdomain_domainChange                     = 26;
static const int ShadowActorSubdomain_exportInternalNode               = 27;
static const int ShadowActorSubdomain_getCost                          = 28;
static const int ShadowActorSubdomain_getElement                       = 29;
static const int ShadowActorSubdomain_getElementPtr                    = 30;
static const int ShadowActorSubdomain_getNode                          = 31;
static const int ShadowActorSubdomain_getNodePtr                       = 32;
static const int ShadowActorSubdomain_getRemoteData                    = 33;
static const int ShadowActorSubdomain_getResistingForce                = 34;
static const int ShadowActorSubdomain_getTang                          = 35;
static const int ShadowActorSubdomain_hasElement                       = 36;
static const int ShadowActorSubdomain_hasExternalNode                  = 37;//Guanzhou added
static const int ShadowActorSubdomain_hasInternalNode                  = 38;//Guanzhou added
static const int ShadowActorSubdomain_hasNode                          = 39;
static const int ShadowActorSubdomain_newStep                          = 40;
static const int ShadowActorSubdomain_Partition                        = 41; //Guanzhou added
static const int ShadowActorSubdomain_Print                            = 42;
static const int ShadowActorSubdomain_recvChangedNodeList              = 43;
static const int ShadowActorSubdomain_reDistributeData                 = 44;
static const int ShadowActorSubdomain_removeElement                    = 45;
static const int ShadowActorSubdomain_removeElementalLoadFromPattern   = 46;
static const int ShadowActorSubdomain_removeLoadPattern                = 47;
static const int ShadowActorSubdomain_removeMP_Constraint              = 48;
static const int ShadowActorSubdomain_removeNodalLoadFromPattern       = 49;
static const int ShadowActorSubdomain_removeNode                       = 50;
static const int ShadowActorSubdomain_removeRecorders                  = 51;
static const int ShadowActorSubdomain_removeSP_Constraint              = 52;
static const int ShadowActorSubdomain_removeSP_ConstraintFromPattern   = 53;
static const int ShadowActorSubdomain_Repartition                      = 54;//Guanzhou added
static const int ShadowActorSubdomain_resetRecorders                   = 55;
static const int ShadowActorSubdomain_resetSubMultipleSupport          = 56;
static const int ShadowActorSubdomain_restoreChannel                   = 57;
static const int ShadowActorSubdomain_revertToLastCommit               = 58;
static const int ShadowActorSubdomain_revertToStart                    = 59;
static const int ShadowActorSubdomain_sendOutputOptions                = 60;//Jose Added... to sync HDF5 output options across processes
static const int ShadowActorSubdomain_setAnalysisAlgorithm             = 61;
static const int ShadowActorSubdomain_setAnalysisConvergenceTest       = 62;
static const int ShadowActorSubdomain_setAnalysisIntegrator            = 63;
static const int ShadowActorSubdomain_setAnalysisLinearSOE             = 64;
static const int ShadowActorSubdomain_setCommitTag                     = 65;
static const int ShadowActorSubdomain_setCommittedTime                 = 66;
static const int ShadowActorSubdomain_setCurrentTime                   = 67;
static const int ShadowActorSubdomain_setDomainDecompAnalysis          = 68;
static const int ShadowActorSubdomain_setLoadConstant                  = 69;
static const int ShadowActorSubdomain_setTag                           = 70;
static const int ShadowActorSubdomain_swapNodeFromExternalToInternal   = 71;
static const int ShadowActorSubdomain_swapNodeFromInternalToExternal   = 72;
static const int ShadowActorSubdomain_update                           = 73;
static const int ShadowActorSubdomain_updateTimeDt                     = 74;
static const int Subdomain_Redistribution_DONE                         = 75;
static const int ShadowActorSubdomain_setConstitutiveIntegrationMethod = 76;

