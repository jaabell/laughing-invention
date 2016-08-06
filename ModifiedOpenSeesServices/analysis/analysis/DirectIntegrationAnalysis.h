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
// $Date: 2003/03/04 00:48:07 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/DirectIntegrationAnalysis.h,v $


#ifndef DirectIntegrationAnalysis_h
#define DirectIntegrationAnalysis_h

// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the class definition for
// DirectIntegrationAnalysis. DirectIntegrationAnalysis is a
// subclass of TransientAnalysis. It is used to perform a
// dynamic analysis on the FE\_Model using a direct integration scheme.
//
// What: "@(#) DirectIntegrationAnalysis.h, revA"

#include <TransientAnalysis.h>


class ConstraintHandler;
class DOF_Numberer;
class AnalysisModel;
class TransientIntegrator;
class LinearSOE;
class EquiSolnAlgo;
class ConvergenceTest;
class EigenSOE;


class DirectIntegrationAnalysis: public TransientAnalysis
{
    public:
        DirectIntegrationAnalysis(Domain& theDomain,
                                  ConstraintHandler& theHandler,
                                  DOF_Numberer& theNumberer,
                                  AnalysisModel& theModel,
                                  EquiSolnAlgo& theSolnAlgo,
                                  LinearSOE& theSOE,
                                  TransientIntegrator& theIntegrator,
                                  ConvergenceTest* theTest = 0);

        virtual ~DirectIntegrationAnalysis();

        void clearAll(void);

        int analyze(int numSteps, double dT);
        int initialize(void);

        int domainChanged(void);

        int setNumberer(DOF_Numberer& theNumberer);
        int setAlgorithm(EquiSolnAlgo& theAlgorithm);
        int setIntegrator(TransientIntegrator& theIntegrator);
        int setLinearSOE(LinearSOE& theSOE);
        int setConvergenceTest(ConvergenceTest& theTest);
        int save_substeps(int num_of_sub_steps, double dT); // Added by Sumeet 5d August, 2016

        int checkDomainChange(void);
        EquiSolnAlgo* getAlgorithm(void);
        TransientIntegrator* getIntegrator(void);
        ConvergenceTest* getConvergenceTest(void);


        // Nima Tafazzoli added for engein analysis, June 2012
        int setEigenSOE(EigenSOE& theSOE);
        int eigen(int numMode);
        

    protected:

    private:
        ConstraintHandler*   theConstraintHandler;
        DOF_Numberer*    theDOF_Numberer;
        AnalysisModel*   theAnalysisModel;
        EquiSolnAlgo*    theAlgorithm;
        LinearSOE*       theSOE;
        TransientIntegrator* theIntegrator;
        ConvergenceTest*     theTest;
        EigenSOE*        theEigenSOE;

        int domainStamp;
        int Global_Sub_Step_No;                // Added  by Sumeet 3rd August, 2016 



};

#endif

