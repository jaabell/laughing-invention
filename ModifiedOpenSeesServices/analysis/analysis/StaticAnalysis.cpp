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

// $Revision: 1.7 $
// $Date: 2003/03/04 00:48:07 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/StaticAnalysis.cpp,v $


// Written: fmk
//
// Description: This file contains the implementation of StaticAnalysis.
//
// What: "@(#) StaticAnalysis.C, revA"

#include <StaticAnalysis.h>
#include <EquiSolnAlgo.h>
#include <AnalysisModel.h>
#include <LinearSOE.h>
#include <DOF_Numberer.h>
#include <ConstraintHandler.h>
#include <ConvergenceTest.h>
#include <StaticIntegrator.h>
#include <Domain.h>
#include <FE_Element.h>
#include <DOF_Group.h>
#include <FE_EleIter.h>
#include <DOF_GrpIter.h>
#include <Matrix.h>
#include <ID.h>
#include <Graph.h>
#include <Timer.h>

// Nima Tafazzoli added August 2010
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

# ifdef _PDD //Guanzhou added
# include <DomainPartitioner.h>
# include <PartitionedDomain.h>
extern int OPS_NUM_SUBDOMAINS;
extern DomainPartitioner *OPS_DOMAIN_PARTITIONER;
//extern GraphPartitioner  *OPS_GRAPH_PARTITIONER;
//extern  PartitionedDomain theDomain;
# endif



// Constructor
//    sets theModel and theSysOFEqn to 0 and the Algorithm to the one supplied

StaticAnalysis::StaticAnalysis(Domain &the_Domain,
                               ConstraintHandler &theHandler,
                               DOF_Numberer &theNumberer,
                               AnalysisModel &theModel,
                               EquiSolnAlgo &theSolnAlgo,
                               LinearSOE &theLinSOE,
                               StaticIntegrator &theStaticIntegrator,
                               ConvergenceTest *theConvergenceTest)
    : Analysis(the_Domain), theConstraintHandler(&theHandler),
      theDOF_Numberer(&theNumberer), theAnalysisModel(&theModel),
      theAlgorithm(&theSolnAlgo), theSOE(&theLinSOE),
      theIntegrator(&theStaticIntegrator), theTest(theConvergenceTest),
      domainStamp(0)
{
    // first we set up the links needed by the elements in the
    // aggregation
    theAnalysisModel->setLinks(the_Domain);
    theConstraintHandler->setLinks(the_Domain, theModel, theStaticIntegrator);
    theDOF_Numberer->setLinks(theModel);

    theIntegrator->setLinks(theModel, theLinSOE);
    theAlgorithm->setLinks(theModel, theStaticIntegrator, theLinSOE);

    if (theTest != 0)
    {
        theAlgorithm->setConvergenceTest(theTest);
    }


}


StaticAnalysis::~StaticAnalysis()
{
    // we don't invoke the destructors in case user switching
    // from a static to a direct integration analysis
    // clearAll() must be invoked if user wishes to invoke destructor
}

void
StaticAnalysis::clearAll(void)
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

    // now set the pointers to NULL
    theAnalysisModel = 0;


    //   theConstraintHandler =0;
    //   theDOF_Numberer =0;
    theIntegrator = 0;
    theAlgorithm = 0;
    theSOE = 0;
    theTest = 0;


}


int
StaticAnalysis::analyze(int numSteps)
{
    int result = 0;
    Domain *the_Domain = this->getDomainPtr();

    for (int i = 0; i < numSteps; i++)
    {

        // Nima Tafazzoli added (august 2010)
        // FIXME: This is not working nicely when there is an error.
        // every call to cerr maybe should be preceded by a std::endl
        cout.flush() << "\r Static Analysis: Step Number is : " << i + 1 << " out of " << numSteps;

        result = theAnalysisModel->newStepDomain();

        if (result < 0)
        {
            cerr << "StaticAnalysis::analyze() - the AnalysisModel failed";
            cerr << " at iteration: " << i << " with domain at load factor ";
            cerr << the_Domain->getCurrentTime() << endln;
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

            result = this->domainChanged();

            if (result < 0)
            {
                cerr << "StaticAnalysis::analyze() - domainChanged failed";
                cerr << " at step " << i << " of " << numSteps << endln;
                return -1;
            }
        }


        result = theIntegrator->newStep();

        if (result < 0)
        {
            cerr << "StaticAnalysis::analyze() - the Integrator failed";
            cerr << " at iteration: " << i << " with domain at load factor ";
            cerr << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();

            return -2;
        }

        result = theAlgorithm->solveCurrentStep();

        if (result < 0)
        {
            cerr << "StaticAnalysis::analyze() - the Algorithm failed";
            cerr << " at iteration: " << i << " with domain at load factor ";
            cerr << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();
            theIntegrator->revertToLastStep();

            return -3;
        }



        result = theIntegrator->commit();

        if (result < 0)
        {
            cerr << "StaticAnalysis::analyze() - ";
            cerr << "the Integrator failed to commit";
            cerr << " at iteration: " << i << " with domain at load factor ";
            cerr << the_Domain->getCurrentTime() << endln;
            the_Domain->revertToLastCommit();
            theIntegrator->revertToLastStep();

            return -4;
        }

# ifdef _PDD //Guanzhou added repartitioning
        //GraphPartitioner *theGraphPartitioner = new ParMetis;
        //DomainPartitioner *theDomainPartitioner = new DomainPartitioner(*theGraphPartitioner);
        //theDomain.setPartitioner(theDomainPartitioner);
        PartitionedDomain *myDomain = (PartitionedDomain *)this->getDomainPtr();
        //myDomain->setPartitioner(OPS_DOMAIN_PARTITIONER);
        myDomain->repartition(OPS_NUM_SUBDOMAINS);
# endif

    }

    cout << endl;


    return 0;
}


int
StaticAnalysis::initialize(void)
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

    if (theIntegrator->initialize() < 0)
    {
        cerr << "DirectIntegrationAnalysis::initialize() - integrator initialize() failed\n";
        return -2;
    }
    else
    {
        theIntegrator->commit();
    }

    return 0;
}

int
StaticAnalysis::domainChanged(void)
{
    int result = 0;

    Domain *the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();
    domainStamp = stamp;

    // Timer theTimer; theTimer.start();
    // cerr << "StaticAnalysis::domainChanged(void)\n";

    theAnalysisModel->clearAll();
    theConstraintHandler->clearAll();

    // theTimer.pause();
    // cout <<  "StaticAnalysis::clearAll() " << theTimer.getReal();
    // cout << theTimer.getCPU() << endln;
    // theTimer.start();

    // now we invoke handle() on the constraint handler which
    // causes the creation of FE_Element and DOF_Group objects
    // and their addition to the AnalysisModel.

    result = theConstraintHandler->handle();

    if (result < 0)
    {
        cerr << "StaticAnalysis::handle() - ";
        cerr << "ConstraintHandler::handle() failed";
        return -1;
    }

    // we now invoke number() on the numberer which causes
    // equation numbers to be assigned to all the DOFs in the
    // AnalysisModel.

    result = theDOF_Numberer->numberDOF();

    if (result < 0)
    {
        cerr << "StaticAnalysis::handle() - ";
        cerr << "DOF_Numberer::numberDOF() failed";
        return -2;
    }

    result = theConstraintHandler->doneNumberingDOF();

    if (result < 0)
    {
        cerr << "StaticAnalysis::handle() - ";
        cerr << "ConstraintHandler::doneNumberingDOF() failed";
        return -2;
    }

    // we invoke setSize() on the LinearSOE which
    // causes that object to determine its size



#ifdef _PARALLEL_PROCESSING
    int MaxDOFtag = theAnalysisModel->getMaxDOFtag(); //Added by Babak 6/4/13
    result = theSOE->setSize(MaxDOFtag); //Added by Babak 6/4/13
#else
    Graph &theGraph = theAnalysisModel->getDOFGraph();//Out by Babak 06/4/13
    result = theSOE->setSize(theGraph);//Out by Babak 06/4/13
#endif
    if (result < 0)
    {
        cerr << "StaticAnalysis::handle() - ";
        cerr << "LinearSOE::setSize() failed";
        return -3;
    }

    // finally we invoke domainChanged on the Integrator and Algorithm
    // objects .. informing them that the model has changed

    result = theIntegrator->domainChanged();

    if (result < 0)
    {
        cerr << "StaticAnalysis::setAlgorithm() - ";
        cerr << "Integrator::domainChanged() failed";
        return -4;
    }

    result = theAlgorithm->domainChanged();

    if (result < 0)
    {
        cerr << "StaticAnalysis::setAlgorithm() - ";
        cerr << "Algorithm::domainChanged() failed";
        return -5;
    }

    // if get here successfull
    return 0;
}



int
StaticAnalysis::setNumberer(DOF_Numberer &theNewNumberer)
{
    // invoke the destructor on the old one
    if (theDOF_Numberer != 0)
    {
        delete theDOF_Numberer;
    }

    // first set the links needed by the Algorithm
    theDOF_Numberer = &theNewNumberer;
    theDOF_Numberer->setLinks(*theAnalysisModel);

    // invoke domainChanged() either indirectly or directly
    domainStamp = 0;

    return 0;
}


int
StaticAnalysis::setAlgorithm(EquiSolnAlgo &theNewAlgorithm)
{
    // invoke the destructor on the old one
    if (theAlgorithm != 0)
    {
        delete theAlgorithm;
    }

    // first set the links needed by the Algorithm
    theAlgorithm = &theNewAlgorithm;
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);

    if (theTest != 0)
    {
        theAlgorithm->setConvergenceTest(theTest);
    }
    else   // this else is for backward compatability.
    {
        theTest = theAlgorithm->getConvergenceTest();
    }

    // invoke domainChanged() either indirectly or directly
    domainStamp = 0;

    return 0;
}

int
StaticAnalysis::setIntegrator(StaticIntegrator &theNewIntegrator)
{
    // invoke the destructor on the old one
    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }

    // set the links needed by the other objects in the aggregation
    Domain *the_Domain = this->getDomainPtr();

    theIntegrator = &theNewIntegrator;
    theIntegrator->setLinks(*theAnalysisModel, *theSOE);
    theConstraintHandler->setLinks(*the_Domain, *theAnalysisModel, *theIntegrator);
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);

    // cause domainChanged to be invoked on next analyze
    domainStamp = 0;

    return 0;

}

int
StaticAnalysis::setLinearSOE(LinearSOE &theNewSOE)
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

    // cause domainChanged to be invoked on next analyze
    domainStamp = 0;

    return 0;
}


int
StaticAnalysis::setConvergenceTest(ConvergenceTest &theNewTest)
{
    // invoke the destructor on the old one
    if (theTest != 0)
    {
        delete theTest;
    }

    // set the links needed by the other objects in the aggregation
    theTest = &theNewTest;
    return theAlgorithm->setConvergenceTest(theTest);
}














