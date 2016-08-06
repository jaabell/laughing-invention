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

// $Revision: 1.6 $
// $Date: 2005/03/30 03:06:02 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/DirectIntegrationAnalysis.cpp,v $


// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the implementation of the
// DirectIntegrationAnalysis class.
//
// What: "@(#) DirectIntegrationAnalysis.C, revA"


#include <DOF_Group.h>
#include <DOF_GrpIter.h>
#include <FE_EleIter.h>

#include <DirectIntegrationAnalysis.h>
#include <EquiSolnAlgo.h>
#include <AnalysisModel.h>
#include <LinearSOE.h>
#include <DOF_Numberer.h>
#include <ConstraintHandler.h>
#include <ConvergenceTest.h>
#include <TransientIntegrator.h>
#include <Domain.h>

#include <FE_Element.h>
#include <DOF_Group.h>
#include <FE_EleIter.h>
#include <DOF_GrpIter.h>
#include <Matrix.h>
#include <ID.h>
#include <Graph.h>

#include <chrono>
#include <ESSITimer.h>


// Nima Tafazzoli added August 2010
#include <stdlib.h>
#include <stdio.h>
#include <EigenSOE.h>
#include <iostream>
using namespace std;



// Constructor
//    sets theModel and theSysOFEqn to 0 and the Algorithm to the one supplied

DirectIntegrationAnalysis::DirectIntegrationAnalysis(Domain &the_Domain,
        ConstraintHandler &theHandler,
        DOF_Numberer &theNumberer,
        AnalysisModel &theModel,
        EquiSolnAlgo &theSolnAlgo,
        LinearSOE &theLinSOE,
        TransientIntegrator &theTransientIntegrator,
        ConvergenceTest *theConvergenceTest)
    : TransientAnalysis(the_Domain),
      theConstraintHandler(&theHandler),
      theDOF_Numberer(&theNumberer),
      theAnalysisModel(&theModel),
      theAlgorithm(&theSolnAlgo),
      theSOE(&theLinSOE),
      theIntegrator(&theTransientIntegrator),
      theTest(theConvergenceTest),
      theEigenSOE(0),
      domainStamp(0)
{
    // first we set up the links needed by the elements in the
    // aggregation
    theAnalysisModel->setLinks(the_Domain);
    theConstraintHandler->setLinks(the_Domain, theModel, theTransientIntegrator);
    theDOF_Numberer->setLinks(theModel);
    theIntegrator->setLinks(theModel, theLinSOE);
    theAlgorithm->setLinks(theModel, theTransientIntegrator, theLinSOE);

    if (theTest != 0)
    {
        theAlgorithm->setConvergenceTest(theTest);
    }
    else
    {
        theTest = theAlgorithm->getConvergenceTest();
    }

}

DirectIntegrationAnalysis::~DirectIntegrationAnalysis()
{
    // we don't invoke the destructors in case user switching
    // from a static to a direct integration analysis
    // clearAll() must be invoked if user wishes to invoke destructor
    //     if (theConstraintHandler != 0)
    //     delete theConstraintHandler;
    //   if (theDOF_Numberer != 0)
    //     delete theDOF_Numberer;
}

void
DirectIntegrationAnalysis::clearAll(void)
{
    // invoke the destructor on all the objects in the aggregation
    if (theAnalysisModel != 0)
    {
        delete theAnalysisModel;
    }


    //   if (theConstraintHandler != 0)
    //     delete theConstraintHandler;
    //   if (theDOF_Numberer != 0)
    //     delete theDOF_Numberer;
    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }

    if (theAlgorithm != 0)
    {
        delete theAlgorithm;
    }

    if (theSOE != 0)
    {
        delete theSOE;
    }

    if (theTest != 0)
    {
        delete theTest;
    }

    if (theEigenSOE != 0)
    {
        delete theEigenSOE;
    }


    theAnalysisModel = 0;


    //     theConstraintHandler =0;
    //     theDOF_Numberer =0;
    theIntegrator = 0;
    theAlgorithm = 0;
    theSOE = 0;
    theEigenSOE = 0;
    theTest = 0;
}

#include <NodeIter.h>
#include <Node.h>

int
DirectIntegrationAnalysis::initialize(void)
{
    cerr << "DirectIntegrationAnalysis::initialize(void)\n";
    Domain *the_Domain = this->getDomainPtr();

    // check if domain has undergone change
    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        if (this->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::initialize() - domainChanged() failed\n";
            return -1;
        }
    }

    if (theIntegrator->initialize() < 0)
    {
        cerr << "DirectIntegrationAnalysis::initialize() - integrator initialize() failed\n";
        return -2;
    }
    else
    {
        theIntegrator->commit();
    }

    cerr << "DirectIntegrationAnalysis::initialize(void) 2\n";
    return 0;
}

int
DirectIntegrationAnalysis::analyze(int numSteps, double dT)
{

    int result = 0;
    Domain *the_Domain = this->getDomainPtr();

    //////////////////// Added by Sumeet (Initial Conditions ) ///////////

    cout << "Writing Initial Conditions " << " " ;

    result = theIntegrator->commit();

    if (result < 0)
    {
        cout << "\nTransient Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
        cerr << "the Integrator failed to commit at ";
        cerr << the_Domain->getCurrentTime() << endln;

        return -4;
    }

    ////////////////////////////////////////////////////////////////////

    for (int i = 0; i < numSteps; i++)
    {

        cout << "\nTransient Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";

        std::chrono::high_resolution_clock::time_point step_start;
        std::chrono::high_resolution_clock::duration estimated_time_to_completion;
        step_start = std::chrono::high_resolution_clock::now();


        if (i > 0)
        {
            cout << " [ETA: "
                 << std::chrono::duration_cast<std::chrono::hours>(  ((numSteps - i) * estimated_time_to_completion)).count() << " h, "
                 << std::chrono::duration_cast<std::chrono::minutes>(  ((numSteps - i) * estimated_time_to_completion) % std::chrono::hours(1)).count() << " m, "
                 << std::chrono::duration_cast<std::chrono::seconds>(  ((numSteps - i) * estimated_time_to_completion) % std::chrono::minutes(1)).count() << " s]\n";
        }
        else
        {
            cout << "\n";
        }

        globalESSITimer.start("Domain_Step");
        result = theAnalysisModel->newStepDomain(dT);
        globalESSITimer.stop("Domain_Step");

        
        if (result< 0)  //call "theAnalysisModel->newStepDomain(dT)" only once and move that call above for globalESSITimer  --Yuan
        {
            cout << "\nDirectIntegrationAnalysis Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
            cerr << " Integration failed at time " << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();

            return -2;
        }

        // check for change in Domain since last step. As a change can
        // occur in a commit() in a domaindecomp with load balancing
        // this must now be inside the loop

        int stamp = the_Domain->hasDomainChanged();

        if (stamp != domainStamp)
        {
            domainStamp = stamp;

            // cout << "       this->domainChanged() \n";//Jdebug
            if (this->domainChanged() < 0)
            {
                cout << "\nDirectIntegrationAnalysis Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
                cerr << " Domain change failed " << the_Domain->getCurrentTime() << endln;
                return -1;
            }
        }


        globalESSITimer.start("Integrator_Step");
        result = theIntegrator->newStep(dT) ;
        globalESSITimer.stop("Integrator_Step");
        if (result < 0) //call "theIntegrator->newStepDomain(dT)" only once and move that call above for globalESSITimer  --Yuan
        {
            cout << "\nDirectIntegrationAnalysis Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
            cerr << " Integrator failed at time " << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();
            return -2;
        }

        globalESSITimer.start("SOE_Solution");
        result = theAlgorithm->solveCurrentStep();
        globalESSITimer.stop("SOE_Solution");

        if (result < 0)
        {
            cout << "\nDirectIntegrationAnalysis Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
            cerr << " Algorithm failed at time " << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();
            theIntegrator->revertToLastStep();
            return -3;
        }



        globalESSITimer.start("Output");
        result = theIntegrator->commit();
        globalESSITimer.stop("Output");
        if (result < 0)
        {
            cout << "\nDirectIntegrationAnalysis Analysis: ["<< std::setw(5) << 0 << "/" << left << std::setw(5) << numSteps << "] ";
            cerr << " Integrator failed at time " << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();
            theIntegrator->revertToLastStep();
            return -4;
        }

        estimated_time_to_completion =  std::chrono::high_resolution_clock::now() - step_start;

    }

    cout << endl;

    return result;
}


int
DirectIntegrationAnalysis::domainChanged(void)
{
    int result = 0;
    Domain *the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();
    domainStamp = stamp;

    theAnalysisModel->clearAll();
    theConstraintHandler->clearAll();

    // now we invoke handle() on the constraint handler which
    // causes the creation of FE_Element and DOF_Group objects
    // and their addition to the AnalysisModel.

    result = theConstraintHandler->handle();

    if (result < 0)
    {
        cerr << "DirectIntegrationAnalysis::handle() - ";
        cerr << "ConstraintHandler::handle() failed";
        return -1;
    }

    // we now invoke number() on the numberer which causes
    // equation numbers to be assigned to all the DOFs in the
    // AnalysisModel.


    theDOF_Numberer->numberDOF();


    theConstraintHandler->doneNumberingDOF();


    // we invoke setGraph() on the LinearSOE which
    // causes that object to determine its size


#ifdef _PARALLEL_PROCESSING
    Graph &theGraph = theAnalysisModel->getDOFGroupGraph();
    result = theSOE->setSize(theGraph);
#else
    Graph &theGraph = theAnalysisModel->getDOFGraph();
    result = theSOE->setSize(theGraph);

    // Nima Tafazzoli added for eigen analysis, June 2012
    // revert back by Yuan in Feb, 2016.
    if (theEigenSOE != 0)
    {
        theEigenSOE->setSize(theAnalysisModel->getDOFGraph());
    }

#endif


    if (result < 0)
    {
        cerr << "DirectIntegrationAnalysis::handle() - ";
        cerr << "LinearSOE::setSize() failed";
        return -3;
    }



    // finally we invoke domainChanged on the Integrator and Algorithm
    // objects .. informing them that the model has changed

    result = theIntegrator->domainChanged();

    if (result < 0)
    {
        cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
        cerr << "Integrator::domainChanged() failed";
        return -4;
    }

    result = theAlgorithm->domainChanged();

    if (result < 0)
    {
        cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
        cerr << "Algorithm::domainChanged() failed";
        return -5;
    }
    return 0;
}



int
DirectIntegrationAnalysis::setNumberer(DOF_Numberer &theNewNumberer)
{
    int result = 0;

    // invoke the destructor on the old one
    if (theDOF_Numberer != 0)
    {
        delete theDOF_Numberer;
    }

    // first set the links needed by the Algorithm
    theDOF_Numberer = &theNewNumberer;
    theDOF_Numberer->setLinks(*theAnalysisModel);

    // invoke domainChanged() either indirectly or directly
    Domain *the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();
    domainStamp = stamp;
    result = this->domainChanged();

    if (result < 0)
    {
        cerr << "StaticAnalysis::setNumberer() - setNumberer() failed";
        return -1;
    }

    return 0;
}



int
DirectIntegrationAnalysis::setAlgorithm(EquiSolnAlgo &theNewAlgorithm)
{
    // invoke the destructor on the old one
    if (theAlgorithm != 0)
    {
        delete theAlgorithm;
    }

    // first set the links needed by the Algorithm
    theAlgorithm = &theNewAlgorithm;
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);

    // invoke domainChanged() either indirectly or directly
    Domain *the_Domain = this->getDomainPtr();
    // check if domain has undergone change
    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        if (this->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
            cerr << "domainChanged failed";
            return -1;
        }
    }
    else
    {
        if (theAlgorithm->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
            cerr << "algorithm::domainChanged() failed";
            return -2;
        }
    }

    return 0;
}


int
DirectIntegrationAnalysis::setIntegrator(TransientIntegrator &theNewIntegrator)
{
    // set the links needed by the other objects in the aggregation
    Domain *the_Domain = this->getDomainPtr();
    theIntegrator = &theNewIntegrator;
    theIntegrator->setLinks(*theAnalysisModel, *theSOE);
    theConstraintHandler->setLinks(*the_Domain, *theAnalysisModel, *theIntegrator);
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);

    // invoke domainChanged() either indirectly or directly
    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        if (this->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
            cerr << "domainChanged failed";
            return -1;
        }
    }
    else
    {
        if (theIntegrator->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
            cerr << "Integrator::domainChanged failed";
            return -1;
        }
    }

    return 0;
}
int
DirectIntegrationAnalysis::setLinearSOE(LinearSOE &theNewSOE)
{
    // invoke the destructor on the old one
    if (theSOE != 0)
    {
        delete theSOE;
    }

    // set the links needed by the other objects in the aggregation
    theSOE = &theNewSOE;
    theIntegrator->setLinks(*theAnalysisModel, *theSOE);
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);

    // set the size either indirectly or directly
    Domain *the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        if (this->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
            cerr << "domainChanged failed";
            return -1;
        }
    }
    else
    {
// #ifdef _PARALLEL_PROCESSING
//         int MaxDOFtag = theAnalysisModel->getMaxDOFtag();
//         if (theSOE->setSize(MaxDOFtag) < 0); //Added by Babak 6/4/13
//         {
//             cerr << "DirectIntegrationAnalysis::setAlgorithm() - ";
//             cerr << "LinearSOE::setSize() failed";
//             return -2;
//         }

// #else

//         Graph &theGraph = theAnalysisModel->getDOFGraph(); //Out by Babak 06/4/13
//         if (theSOE->setSize(theGraph) < 0) //Out by Babak 06/4/13
//         {
//             theAnalysisModel->getMaxDOFtag();    //Added by Babak 6/4/13
//         }
// #endif

    }

    return 0;
}


int
DirectIntegrationAnalysis::setConvergenceTest(ConvergenceTest &theNewTest)
{
    // invoke the destructor on the old one
    if (theTest != 0)
    {
        delete theTest;
    }

    // set the links needed by the other objects in the aggregation
    theTest = &theNewTest;
    theAlgorithm->setConvergenceTest(theTest);

    return 0;
}


int
DirectIntegrationAnalysis::checkDomainChange(void)
{
    Domain *the_Domain = this->getDomainPtr();

    // check if domain has undergone change
    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        if (this->domainChanged() < 0)
        {
            cerr << "DirectIntegrationAnalysis::initialize() - domainChanged() failed\n";
            return -1;
        }
    }

    return 0;
}


EquiSolnAlgo *
DirectIntegrationAnalysis::getAlgorithm(void)
{
    return theAlgorithm;
}


TransientIntegrator *
DirectIntegrationAnalysis::getIntegrator(void)
{
    return theIntegrator;
}

ConvergenceTest *
DirectIntegrationAnalysis::getConvergenceTest(void)
{
    return theTest;
}



// Nima Tafazzoli added for eigen analysis, June 2012
int
DirectIntegrationAnalysis::setEigenSOE(EigenSOE &theNewSOE)
{
    // invoke the destructor on the old one
    if (theEigenSOE != 0)
    {
        delete theEigenSOE;
    }

    // set the links needed by the other objects in the aggregation
    theEigenSOE = &theNewSOE;
    theEigenSOE->setLinks(*theAnalysisModel);

    // cause domainChanged to be invoked on next analyze
    domainStamp = 0;

    return 0;
}




int
DirectIntegrationAnalysis::eigen(int numMode)
{
    if (theAnalysisModel == 0 || theEigenSOE == 0)
    {
        cerr << "WARNING DirectIntegrationAnalysis::eigen() - no EigenSOE has been set\n";
        return -1;
    }

    int result = 0;
    Domain *the_Domain = this->getDomainPtr();

    result = theAnalysisModel->eigenAnalysis(numMode);

    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;

        result = this->domainChanged();

        if (result < 0)
        {
            cerr << "DirectIntegrationAnalysis::eigen() - domainChanged failed";
            return -1;
        }
    }

    //
    // zero A and M
    //
    theEigenSOE->zeroA();
    theEigenSOE->zeroM();

    //
    // form K
    //

    FE_EleIter &theEles = theAnalysisModel->getFEs();
    FE_Element *elePtr;

    while ((elePtr = theEles()) != 0)
    {
        elePtr->zeroTangent();
        elePtr->addKtToTang(1.0);

        if (theEigenSOE->addA(elePtr->getTangent(0), elePtr->getID()) < 0)
        {
            cerr << "WARNING DirectIntegrationAnalysis::eigen() -";
            //  cerr << " failed in addA for ID " << elePtr->getID();
            result = -2;
        }
    }



    //
    // form M
    //
    FE_EleIter &theEles2 = theAnalysisModel->getFEs();

    while ((elePtr = theEles2()) != 0)
    {
        elePtr->zeroTangent();
        elePtr->addMtoTang(1.0);

        if (theEigenSOE->addM(elePtr->getTangent(0), elePtr->getID()) < 0)
        {
            cerr << "WARNING DirectIntegrationAnalysis::eigen() -";
            //    cerr << " failed in addA for ID " << elePtr->getID();
            result = -2;
        }
    }

    DOF_Group *dofPtr;
    DOF_GrpIter &theDofs = theAnalysisModel->getDOFs();

    while ((dofPtr = theDofs()) != 0)
    {
        dofPtr->zeroTangent();
        dofPtr->addMtoTang(1.0);

        if (theEigenSOE->addM(dofPtr->getTangent(0), dofPtr->getID()) < 0)
        {
            cerr << "WARNING DirectIntegrationAnalysis::eigen() -";
            //    cerr << " failed in addM for ID " << dofPtr->getID();
            result = -3;
        }
    }


    //
    // solve for the eigen values & vectors
    //

    if (theEigenSOE->solve(numMode) < 0)
    {
        cerr << "WARNING DirectIntegrationAnalysis::eigen() - EigenSOE failed in solve()\n";
        return -4;
    }

    //
    // now set the eigenvalues and eigenvectors in the model
    //

    theAnalysisModel->setNumEigenvectors(numMode);
    Vector theEigenvalues(numMode);

    for (int i = 1; i <= numMode; i++)
    {
        theEigenvalues[i - 1] = theEigenSOE->getEigenvalue(i);
        theAnalysisModel->setEigenvector(i, theEigenSOE->getEigenvector(i));
    }

    theAnalysisModel->setEigenvalues(theEigenvalues);

    return 0;
}

