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

// $Revision$
// $Date$
// $Source$

// Written: fmk
// Revision: A
//
// Description: This file contains the implementation of StaticDomainDecompositionAnalysis.
//
// What: "@(#) StaticDomainDecompositionAnalysis.C, revA"

#include <StaticDomainDecompositionAnalysis.h>
#include <EquiSolnAlgo.h>
#include <AnalysisModel.h>
#include <LinearSOE.h>
#include <LinearSOESolver.h>
#include <DOF_Numberer.h>
#include <ConstraintHandler.h>
#include <StaticIntegrator.h>
#include <ConvergenceTest.h>
#include <Subdomain.h>
#include <Timer.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>



#include <FE_Element.h>
#include <DOF_Group.h>
#include <FE_EleIter.h>
#include <DOF_GrpIter.h>
#include <Matrix.h>
#include <ID.h>
#include <Graph.h>

#include <Vector.h>
#include <Matrix.h>

#include <iostream>
using namespace std;

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

StaticDomainDecompositionAnalysis::StaticDomainDecompositionAnalysis(Subdomain& the_Domain)
    : DomainDecompositionAnalysis(ANALYSIS_TAGS_StaticDomainDecompositionAnalysis, the_Domain),
      theConstraintHandler(0),
      theDOF_Numberer(0),
      theAnalysisModel(0),
      theAlgorithm(0),
      theSOE(0),
      theIntegrator(0),
      theTest(0),
      domainStamp(0)
{

}

StaticDomainDecompositionAnalysis::StaticDomainDecompositionAnalysis(Subdomain& the_Domain,
        ConstraintHandler& theHandler,
        DOF_Numberer& theNumberer,
        AnalysisModel& theModel,
        EquiSolnAlgo& theSolnAlgo,
        LinearSOE& theLinSOE,
        StaticIntegrator& theStaticIntegrator,
        ConvergenceTest* theConvergenceTest,
        bool setLinks)
    : DomainDecompositionAnalysis(ANALYSIS_TAGS_StaticDomainDecompositionAnalysis, the_Domain),
      theConstraintHandler(&theHandler),
      theDOF_Numberer(&theNumberer),
      theAnalysisModel(&theModel),
      theAlgorithm(&theSolnAlgo),
      theSOE(&theLinSOE),
      theIntegrator(&theStaticIntegrator),
      theTest(theConvergenceTest),
      domainStamp(0)
{

    if (setLinks == true)
    {
        // set up the links needed by the elements in the aggregation
        theAnalysisModel->setLinks(the_Domain);
        theConstraintHandler->setLinks(the_Domain, theModel, theStaticIntegrator);
        theDOF_Numberer->setLinks(theModel);
        theIntegrator->setLinks(theModel, theLinSOE);
        theAlgorithm->setLinks(theModel, theStaticIntegrator, theLinSOE);
        theAlgorithm->setConvergenceTest(theTest);
    }



}


StaticDomainDecompositionAnalysis::~StaticDomainDecompositionAnalysis()
{
    // we don't invoke the destructors in case user switching
    // from a static to a direct integration analysis
    // clearAll() must be invoked if user wishes to invoke destructor
}

void
StaticDomainDecompositionAnalysis::clearAll(void)
{
    // invoke the destructor on all the objects in the aggregation
    if (theAnalysisModel != 0)
    {
        delete theAnalysisModel;
    }

    if (theConstraintHandler != 0)
    {
        delete theConstraintHandler;
    }

    if (theDOF_Numberer != 0)
    {
        delete theDOF_Numberer;
    }

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
    theConstraintHandler = 0;
    theDOF_Numberer = 0;
    theIntegrator = 0;
    theAlgorithm = 0;
    theSOE = 0;
    theTest = 0;
}


bool
StaticDomainDecompositionAnalysis::doesIndependentAnalysis(void)
{
    return true;
}


int
StaticDomainDecompositionAnalysis::analyze(double dT)
{
    int result = 0;
    Domain* the_Domain = this->getDomainPtr();

    // check for change in Domain since last step. As a change can
    // occur in a commit() in a domaindecomp with load balancing
    // this must now be inside the loop

    int stamp = the_Domain->hasDomainChanged();

    if (stamp != domainStamp)
    {
        domainStamp = stamp;
        result = this->domainChanged();
        std::cerr << "StaticDomainDecompositionAnalysis::analyze(double dT) -- after result = this->domainChanged();\n" ;

        if (result < 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::analyze() - domainChanged failed";
            return -1;
        }
    }


    // result = theAnalysisModel->newStepDomain();
    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::analyze() - the AnalysisModel failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();

        return -2;
    }

    //  cerr << " StaticDomainDecompositionAnalysis::analyze() - 2\n";

    result = theIntegrator->newStep();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::analyze() - the Integrator failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();

        return -2;
    }

    result = theAlgorithm->solveCurrentStep();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::analyze() - the Algorithm failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();
        theIntegrator->revertToLastStep();

        return -3;
    }




    result = theIntegrator->commit();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::analyze() - ";
        cerr << "the Integrator failed to commit";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();
        theIntegrator->revertToLastStep();

        return -4;
    }

    //   cerr << " StaticDomainDecompositionAnalysis::analyze() - done COMMIT\n";

    return 0;
}


int
StaticDomainDecompositionAnalysis::initialize(void)
{
    Domain* the_Domain = this->getDomainPtr();

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
StaticDomainDecompositionAnalysis::domainChanged(void)
{
    int rank = 0;
#ifdef _PARALLEL_PROCESSING

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    if (rank == 0)
    {
        cout << "StaticDomainDecompositionAnalysis - The domain has changed. (Re)Creating model.\n";
    }
    Domain* the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();

    domainStamp = stamp;

    int result = 0;

    theAnalysisModel->clearAll();
    theConstraintHandler->clearAll();


    // now we invoke handle() on the constraint handler which
    // causes the creation of FE_Element and DOF_Group objects
    // and their addition to the AnalysisModel.

    if (rank == 0)
    {
        cout << "   * Handling constraints\n";
    }

    result = theConstraintHandler->handle();


    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::handle() - ";
        cerr << "ConstraintHandler::handle() failed";
        return -1;
    }


    // we now invoke number() on the numberer which causes
    // equation numbers to be assigned to all the DOFs in the
    // AnalysisModel.

    // if (rank == 0)
    // {
    //     cout << "   * Numbering DOFS\n";
    // }

    result = theDOF_Numberer->numberDOF();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::handle() - ";
        cerr << "DOF_Numberer::numberDOF() failed";
        return -2;
    }



    if (rank == 0)
    {
        cout << "   * Forming DOF Graph\n";
    }
    Graph &theGraph = theAnalysisModel->getDOFGraph();

    if (rank == 0)
    {
        cout << "   * Setting SOE Size\n";
    }
    result = theSOE->setSize(theGraph);
    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::handle() - ";
        cerr << "LinearSOE::setSize() failed";
        return -3;
    }



    // finally we invoke domainChanged on the Integrator and Algorithm
    // objects .. informing them that the model has changed
    if (rank == 0)
    {
        cout << "   * Setting up integrator\n";
    }

    result = theIntegrator->domainChanged();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::setAlgorithm() - ";
        cerr << "Integrator::domainChanged() failed";
        return -4;
    }

    if (rank == 0)
    {
        cout << "   * Setting up algorithm\n";
    }

    result = theAlgorithm->domainChanged();

    if (result < 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::setAlgorithm() - ";
        cerr << "Algorithm::domainChanged() failed";
        return -5;
    }

    if (rank == 0)
    {
        cout << "   * Done with domain change\n";
    }

    // if get here successfull
    return 0;
}


int
StaticDomainDecompositionAnalysis::getNumExternalEqn(void)
{
    //GZ out cerr << "StaticDomainDecompositionAnalysis::getNumExternalEqn() - should never be called\n";
    return 0;
}
int
StaticDomainDecompositionAnalysis::getNumInternalEqn(void)
{
    cerr << "StaticDomainDecompositionAnalysis::getNumInternalEqn() - should never be called\n";
    return 0;
}
int
StaticDomainDecompositionAnalysis::newStep(double dT)
{
    this->analyze(dT);
    return 0;

}
int
StaticDomainDecompositionAnalysis::computeInternalResponse(void)
{
    cerr << "StaticDomainDecompositionAnalysis::computeInternalResponse() - should never be called\n";
    return 0;
}
int
StaticDomainDecompositionAnalysis::formTangent(void)
{
    cerr << "StaticDomainDecompositionAnalysis::formTangent() - should never be called\n";
    return 0;
}
int
StaticDomainDecompositionAnalysis::formResidual(void)
{
    cerr << "StaticDomainDecompositionAnalysis::formResidual() - should never be called\n";
    return 0;
}
int
StaticDomainDecompositionAnalysis::formTangVectProduct(Vector& force)
{
    cerr << "StaticDomainDecompositionAnalysis::formTangVectProduct() - should never be called\n";
    return 0;
}

const Matrix&
StaticDomainDecompositionAnalysis::getTangent(void)
{
    static Matrix errMatrix;
    cerr << "StaticDomainDecompositionAnalysis::getTangent() - should never be called\n";
    return errMatrix;
}

const Vector&
StaticDomainDecompositionAnalysis::getResidual(void)
{
    static Vector errVector;
    cerr << "StaticDomainDecompositionAnalysis::getResidual() - should never be called\n";
    return errVector;
}

const Vector&
StaticDomainDecompositionAnalysis::getTangVectProduct(void)
{
    static Vector errVector;
    cerr << "StaticDomainDecompositionAnalysis::getTangVectProduct() - should never be called\n";
    return errVector;
}

int
StaticDomainDecompositionAnalysis::sendSelf(int commitTag, Channel& theChannel)
{
    // receive the data identifyng the objects in the aggregation
    int dataTag = this->getDbTag();
    static ID data(8);

    if (theAlgorithm == 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - no objects exist!\n";
        return -1;
    }

    LinearSOESolver* theSolver = theSOE->getSolver();

    data(0) = theConstraintHandler->getClassTag();
    data(1) = theDOF_Numberer->getClassTag();
    data(2) = theAnalysisModel->getClassTag();
    data(3) = theAlgorithm->getClassTag();
    data(4) = theSOE->getClassTag();
    data(5) = theSolver->getClassTag();
    data(6) = theIntegrator->getClassTag();

    if (theTest != 0)
    {
        data(7) = theTest->getClassTag();
    }
    else
    {
        data(7) = -1;
    }

    theChannel.sendID(dataTag, commitTag, data);

    // invoke sendSelf() on all the objects
    if (theConstraintHandler->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send handler\n";
        return -1;
    }

    if (theDOF_Numberer->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send numberer\n";
        return -1;
    }

    if (theAnalysisModel->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send model\n";
        return -1;
    }

    if (theAlgorithm->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send algorithm\n";
        return -1;
    }

    if (theSOE->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send SOE\n";
        return -1;
    }
    else
    {
        theSOE->setAnalysisModel(*theAnalysisModel);
    }


    if (theSolver->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send Solver\n";
        return -1;
    }

    if (theIntegrator->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send integrator\n";
        return -1;
    }

    if (theTest != 0)
        if (theTest->sendSelf(commitTag, theChannel) != 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::sendSelf() - failed to send integrator\n";
            return -1;
        }

    return 0;
}
int
StaticDomainDecompositionAnalysis::receiveSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    Domain* the_Domain = this->getSubdomainPtr();

    // receive the data identifyng the objects in the aggregation
    static ID data(8);
    int dataTag = this->getDbTag();
    theChannel.receiveID(dataTag, commitTag, data);

    // for all objects in the aggregation:
    //  1. make sure objects exist & are of correct type; create new objects if not
    //  2. invoke receiveSelf on the object
    if (theConstraintHandler == 0 || theConstraintHandler->getClassTag() != data(0))
    {
        if (theConstraintHandler != 0)
        {
            delete theConstraintHandler;
        }

        theConstraintHandler = theBroker.getNewConstraintHandler(data(0));

        if (theConstraintHandler == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the ConstraintHandler\n";
            return -1;
        }
    }

    theConstraintHandler->receiveSelf(commitTag, theChannel, theBroker);

    if (theDOF_Numberer == 0 || theDOF_Numberer->getClassTag() != data(1))
    {
        if (theDOF_Numberer != 0)
        {
            delete theDOF_Numberer;
        }

        theDOF_Numberer = theBroker.getNewNumberer(data(1));

        if (theDOF_Numberer == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the ConstraintHandler\n";
            return -1;
        }
    }

    theDOF_Numberer->receiveSelf(commitTag, theChannel, theBroker);

    if (theAnalysisModel == 0 || theAnalysisModel->getClassTag() != data(2))
    {
        if (theAnalysisModel != 0)
        {
            delete theAnalysisModel;
        }

        theAnalysisModel = theBroker.getNewAnalysisModel(data(2));

        if (theAnalysisModel == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the Analysis Model\n";
            return -1;
        }
    }

    theAnalysisModel->receiveSelf(commitTag, theChannel, theBroker);

    if (theAlgorithm == 0 || theAlgorithm->getClassTag() != data(3))
    {
        if (theAlgorithm != 0)
        {
            delete theAlgorithm;
        }

        theAlgorithm = theBroker.getNewEquiSolnAlgo(data(3));

        if (theAlgorithm == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the Solution Algorithm\n";
            return -1;
        }
    }

    theAlgorithm->receiveSelf(commitTag, theChannel, theBroker);

    if (theSOE == 0 || theSOE->getClassTag() != data(4))
    {
        if (theSOE != 0)
        {
            delete theSOE;
        }

        theSOE = theBroker.getNewLinearSOE(data(4), data(5));

        if (theSOE == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the LinearSOE\n";
            return -1;
        }
    }

    theSOE->receiveSelf(commitTag, theChannel, theBroker);
    LinearSOESolver* theSolver = theSOE->getSolver();
    theSolver->receiveSelf(commitTag, theChannel, theBroker);
    theSOE->setAnalysisModel(*theAnalysisModel);

    if (theIntegrator == 0 || theIntegrator->getClassTag() != data(6))
    {
        if (theIntegrator != 0)
        {
            delete theIntegrator;
        }

        theIntegrator = theBroker.getNewStaticIntegrator(data(6));

        if (theIntegrator == 0)
        {
            cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the Integrator\n";
            return -1;
        }
    }

    theIntegrator->receiveSelf(commitTag, theChannel, theBroker);


    if (theTest == 0 || theTest->getClassTag() != data(7))
    {
        if (theTest != 0)
        {
            delete theIntegrator;
        }

        if (data(7) != -1)
        {
            theTest = theBroker.getNewConvergenceTest(data(7));

            if (theTest == 0)
            {
                cerr << "StaticDomainDecompositionAnalysis::receiveSelf";
                cerr << " - failed to get the ConvergenceTest\n";
                return -1;
            }
        }
    }

    if (theTest != 0)
    {
        theTest->receiveSelf(commitTag, theChannel, theBroker);
    }

    // set up the links needed by the elements in the aggregation
    theAnalysisModel->setLinks(*the_Domain);
    theConstraintHandler->setLinks(*the_Domain, *theAnalysisModel, *theIntegrator);
    theDOF_Numberer->setLinks(*theAnalysisModel);
    theIntegrator->setLinks(*theAnalysisModel, *theSOE);
    theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);
    theAlgorithm->setConvergenceTest(theTest);

    return 0;
}



int
StaticDomainDecompositionAnalysis::setAlgorithm(EquiSolnAlgo& theNewAlgorithm)
{
    // invoke the destructor on the old one
    if (theAlgorithm != 0)
    {
        delete theAlgorithm;
    }

    // first set the links needed by the Algorithm
    theAlgorithm = &theNewAlgorithm;

    if (theAnalysisModel != 0 && theIntegrator != 0 && theSOE != 0)
    {
        theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);
    }

    if (theTest != 0)
    {
        theAlgorithm->setConvergenceTest(theTest);
    }

    // invoke domainChanged() either indirectly or directly
    domainStamp = 0;

    return 0;
}

int
StaticDomainDecompositionAnalysis::setIntegrator(IncrementalIntegrator& theNewIntegrator)
{
    // invoke the destructor on the old one
    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }

    // set the links needed by the other objects in the aggregation
    Domain* the_Domain = this->getDomainPtr();

    theIntegrator = (StaticIntegrator*)(&theNewIntegrator);

    if (theIntegrator != 0 && theConstraintHandler != 0 && theAlgorithm != 0 && theAnalysisModel != 0 && theSOE != 0)
    {
        theIntegrator->setLinks(*theAnalysisModel, *theSOE);
        theConstraintHandler->setLinks(*the_Domain, *theAnalysisModel, *theIntegrator);
        theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);
    }

    // cause domainChanged to be invoked on next analyze
    domainStamp = 0;

    return 0;
}


int
StaticDomainDecompositionAnalysis::setLinearSOE(LinearSOE& theNewSOE)
{
    // invoke the destructor on the old one
    if (theSOE != 0)
    {
        delete theSOE;
    }

    // set the links needed by the other objects in the aggregation
    theSOE = &theNewSOE;

    if (theIntegrator != 0 && theAlgorithm != 0 && theAnalysisModel != 0 && theSOE != 0)
    {
        theIntegrator->setLinks(*theAnalysisModel, *theSOE);
        theAlgorithm->setLinks(*theAnalysisModel, *theIntegrator, *theSOE);
    }

    // cause domainChanged to be invoked on next analyze
    domainStamp = 0;

    return 0;
}


int
StaticDomainDecompositionAnalysis::setConvergenceTest(ConvergenceTest& theConvergenceTest)
{
    // invoke the destructor on the old one
    if (theTest != 0)
    {
        delete theTest;
    }

    theTest = &theConvergenceTest;

    if (theAlgorithm != 0)
    {
        return theAlgorithm->setConvergenceTest(theTest);
    }

    return 0;
}




