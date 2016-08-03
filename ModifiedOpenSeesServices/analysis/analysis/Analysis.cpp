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

// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/Analysis.cpp,v $


// File: ~/analysis/analysis/Analysis.C
//
// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the implementation of Analysis.
// Analysis is an abstract base class, i.e. no objects of it's
// type can be created.
//
// What: "@(#) Analysis.C, revA"

#include <Analysis.h>
#include <Domain.h>

Analysis::Analysis(Domain& theDom)
	: theDomain(&theDom)
{

}

Analysis::~Analysis()
{

}

//Added by Jose to time analysis for profiling. Instances of analysis have to implement the actual timing of the parts.
// int Analysis::setTimer(std::string reportfilename_, std::string header)
// {
// 	timers.setReportFileName(reportfilename_, header);
// }

/****************************************************************************************************
* Adde by Sumeet 2nd August, 2016
* For outputting sub_increaments of the last non-converged step
*
*****************************************************************************************************/
int Analysis::output_Substeps_of_Last_Non_Converged_Step(AnalysisModel* theAnalysisModel, Integrator* theIntegrator, EquiSolnAlgo* theAlgorithm){


	cout << " I am here " << endl;
	// this->theDomain->revertToLastCommit();
	// theIntegrator.revertToLastStep();
	// theAnalysisModel.newStepDomain();
 //    theIntegrator.newStep();
 //    theAlgorithm.solveCurrentStep();

	if (StaticIntegrator* newIntegrator = dynamic_cast<StaticIntegrator*>(theIntegrator)) {
       cout << " This is a static integerator " << endl;
       theAnalysisModel->newStepDomain();
       newIntegrator->newStep();
       theAlgorithm->solveCurrentStep();
    } else if (TransientIntegrator* newIntegrator = dynamic_cast<TransientIntegrator*>(theIntegrator)) {
        cout << " This is a dynamic integerator " << endl;
    }

    return 0;
} 

Domain*
Analysis::getDomainPtr(void)
{
	return theDomain;
}


