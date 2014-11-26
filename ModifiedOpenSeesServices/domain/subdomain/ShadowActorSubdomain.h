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
static const int Master_addNodalLoadToPattern                        = 0;
static const int Master_addSP_Constraint                             = 1;
static const int Master_addSP_ConstraintToPattern                    = 2;
static const int Master_hasNode                                      = 3;
static const int ShadowActorSubdomain_addElement                     = 4;
static const int ShadowActorSubdomain_addElementalLoadToPattern      = 5;
static const int ShadowActorSubdomain_addExternalNode                = 6;
static const int ShadowActorSubdomain_addLoadPattern                 = 7;
static const int ShadowActorSubdomain_addMP_Constraint               = 8;
static const int ShadowActorSubdomain_addNodalLoadToPattern          = 9;
static const int ShadowActorSubdomain_addNode                        = 10;
static const int ShadowActorSubdomain_addRecorder                    = 11;
static const int ShadowActorSubdomain_addSP_Constraint               = 12;
static const int ShadowActorSubdomain_addSP_ConstraintToPattern      = 13;
static const int ShadowActorSubdomain_applyLoad                      = 14;
static const int ShadowActorSubdomain_BuildElementGraph              = 15; //Guanzhou added
static const int ShadowActorSubdomain_buildSubdomain                 = 16;
static const int ShadowActorSubdomain_ChangeMPIChannel               = 17;
static const int ShadowActorSubdomain_clearAnalysis                  = 18;
static const int ShadowActorSubdomain_commit                         = 19;
static const int ShadowActorSubdomain_computeNodalResponse           = 20;
static const int ShadowActorSubdomain_computeResidual                = 21;
static const int ShadowActorSubdomain_computeTang                    = 22;
static const int ShadowActorSubdomain_DIE                            = 23;
static const int ShadowActorSubdomain_domainChange                   = 24;
static const int ShadowActorSubdomain_exportInternalNode             = 25;
static const int ShadowActorSubdomain_getCost                        = 26;
static const int ShadowActorSubdomain_getElement                     = 27;
static const int ShadowActorSubdomain_getElementPtr                  = 28;
static const int ShadowActorSubdomain_getNode                        = 29;
static const int ShadowActorSubdomain_getNodePtr                     = 30;
static const int ShadowActorSubdomain_getRemoteData                  = 31;
static const int ShadowActorSubdomain_getResistingForce              = 32;
static const int ShadowActorSubdomain_getTang                        = 33;
static const int ShadowActorSubdomain_hasElement                     = 34;
static const int ShadowActorSubdomain_hasExternalNode                = 35;//Guanzhou added
static const int ShadowActorSubdomain_hasInternalNode                = 36;//Guanzhou added
static const int ShadowActorSubdomain_hasNode                        = 37;
static const int ShadowActorSubdomain_newStep                        = 38;
static const int ShadowActorSubdomain_Partition                      = 39; //Guanzhou added
static const int ShadowActorSubdomain_Print                          = 40;
static const int ShadowActorSubdomain_recvChangedNodeList            = 41;
static const int ShadowActorSubdomain_reDistributeData               = 42;
static const int ShadowActorSubdomain_removeElement                  = 43;
static const int ShadowActorSubdomain_removeElementalLoadFromPattern = 44;
static const int ShadowActorSubdomain_removeLoadPattern              = 45;
static const int ShadowActorSubdomain_removeMP_Constraint            = 46;
static const int ShadowActorSubdomain_removeNodalLoadFromPattern     = 47;
static const int ShadowActorSubdomain_removeNode                     = 48;
static const int ShadowActorSubdomain_removeRecorders                = 49;
static const int ShadowActorSubdomain_removeSP_Constraint            = 50;
static const int ShadowActorSubdomain_removeSP_ConstraintFromPattern = 51;
static const int ShadowActorSubdomain_Repartition                    = 52;//Guanzhou added
static const int ShadowActorSubdomain_resetRecorders                 = 53;
static const int ShadowActorSubdomain_resetSubMultipleSupport        = 54;
static const int ShadowActorSubdomain_restoreChannel                 = 55;
static const int ShadowActorSubdomain_revertToLastCommit             = 56;
static const int ShadowActorSubdomain_revertToStart                  = 57;
static const int ShadowActorSubdomain_sendOutputOptions              = 58;//Jose Added... to sync HDF5 output options across processes
static const int ShadowActorSubdomain_setAnalysisAlgorithm           = 59;
static const int ShadowActorSubdomain_setAnalysisConvergenceTest     = 60;
static const int ShadowActorSubdomain_setAnalysisIntegrator          = 61;
static const int ShadowActorSubdomain_setAnalysisLinearSOE           = 62;
static const int ShadowActorSubdomain_setCommitTag                   = 63;
static const int ShadowActorSubdomain_setCommittedTime               = 64;
static const int ShadowActorSubdomain_setCurrentTime                 = 65;
static const int ShadowActorSubdomain_setDomainDecompAnalysis        = 66;
static const int ShadowActorSubdomain_setLoadConstant                = 67;
static const int ShadowActorSubdomain_setTag                         = 68;
static const int ShadowActorSubdomain_swapNodeFromExternalToInternal = 69;
static const int ShadowActorSubdomain_swapNodeFromInternalToExternal = 70;
static const int ShadowActorSubdomain_update                         = 71;
static const int ShadowActorSubdomain_updateTimeDt                   = 72;
static const int Subdomain_Redistribution_DONE                       = 73;


