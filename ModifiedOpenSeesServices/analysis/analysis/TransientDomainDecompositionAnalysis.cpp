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
// Description: This file contains the implementation of TransientDomainDecompositionAnalysis.
//
// What: "@(#) TransientDomainDecompositionAnalysis.C, revA"

#include <TransientDomainDecompositionAnalysis.h>
#include <EquiSolnAlgo.h>
#include <AnalysisModel.h>
#include <LinearSOE.h>
#include <LinearSOESolver.h>
#include <DOF_Numberer.h>
#include <ConstraintHandler.h>
#include <TransientIntegrator.h>
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

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

#include <time.h>


#include <iostream>
using namespace std;


TransientDomainDecompositionAnalysis::TransientDomainDecompositionAnalysis(Subdomain &the_Domain)
    : DomainDecompositionAnalysis(ANALYSIS_TAGS_TransientDomainDecompositionAnalysis, the_Domain),
      theConstraintHandler(0),
      theDOF_Numberer(0),
      theAnalysisModel(0),
      theAlgorithm(0),
      theSOE(0),
      theIntegrator(0),
      theTest(0),
      domainStamp(0),whether_initial_step(true)
{

}

TransientDomainDecompositionAnalysis::TransientDomainDecompositionAnalysis(Subdomain &the_Domain,
        ConstraintHandler &theHandler,
        DOF_Numberer &theNumberer,
        AnalysisModel &theModel,
        EquiSolnAlgo &theSolnAlgo,
        LinearSOE &theLinSOE,
        TransientIntegrator &theTransientIntegrator,
        ConvergenceTest *theConvergenceTest,
        bool setLinks)
    : DomainDecompositionAnalysis(ANALYSIS_TAGS_TransientDomainDecompositionAnalysis, the_Domain),
      theConstraintHandler(&theHandler),
      theDOF_Numberer(&theNumberer),
      theAnalysisModel(&theModel),
      theAlgorithm(&theSolnAlgo),
      theSOE(&theLinSOE),
      theIntegrator(&theTransientIntegrator),
      theTest(theConvergenceTest),
      domainStamp(0),whether_initial_step(true)
{

    if (setLinks == true)
    {
        // set up the links needed by the elements in the aggregation
        theAnalysisModel->setLinks(the_Domain);
        theConstraintHandler->setLinks(the_Domain, theModel, theTransientIntegrator);
        theDOF_Numberer->setLinks(theModel);
        theIntegrator->setLinks(theModel, theLinSOE);
        theAlgorithm->setLinks(theModel, theTransientIntegrator, theLinSOE);
        theAlgorithm->setConvergenceTest(theTest);
    }



}


TransientDomainDecompositionAnalysis::~TransientDomainDecompositionAnalysis()
{
    // we don't invoke the destructors in case user switching
    // from a static to a direct integration analysis
    // clearAll() must be invoked if user wishes to invoke destructor
}

void
TransientDomainDecompositionAnalysis::clearAll(void)
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
TransientDomainDecompositionAnalysis::doesIndependentAnalysis(void)
{
    return true;
}


int
TransientDomainDecompositionAnalysis::analyze(double dT)
{
    int result = 0;
    Domain *the_Domain = this->getDomainPtr();

    if(whether_initial_step){
        cout << "Writing Initial Conditions " << " " ;
        result = theIntegrator->output_step();  // Sumeet September, 2016
        whether_initial_step = false;
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
            cerr << "TransientDomainDecompositionAnalysis::analyze() -- domainChanged failed";
            return -1;
        }
    }


    // result = theAnalysisModel->newStepDomain();
    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::analyze() -- the AnalysisModel failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();

        return -2;
    }


    result = theIntegrator->newStep(dT);

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::analyze() -- the Integrator failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();

        return -2;
    }

    result = theAlgorithm->solveCurrentStep();

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::analyze() -- the Algorithm failed";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;

        the_Domain->revertToLastCommit();
        theIntegrator->revertToLastStep();

        /****************************************************************************************************
        * Adde by Sumeet 2nd August, 2016
        * This function basically, saves and performs the iterations of a current analysis_step from the 
        * beginning of that step and also save sthe output to hdf5 file
        *****************************************************************************************************/

        cout << endl << "################## Started Writing Iteration Output ######################" << endl;; 
        theAlgorithm->switchOutputIterationOption(true);  // Switch on the writer to write incremental output
        theIntegrator->newStep(dT);
        theAlgorithm->solveCurrentStep();  

        return -3;
    }


    result = theIntegrator->commit();
    result = theIntegrator->output_step();  // Sumeet September, 2016

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::analyze() - ";
        cerr << "the Integrator failed to commit";
        cerr << " with domain at load factor ";
        cerr << the_Domain->getCurrentTime() << endln;
        the_Domain->revertToLastCommit();
        theIntegrator->revertToLastStep();

        return -4;
    }


    // std::cerr << "TransientDomainDecompositionAnalysis::analyze() --  Step end.\n";

    return 0;
}


int
TransientDomainDecompositionAnalysis::initialize(void)
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
TransientDomainDecompositionAnalysis::domainChanged(void)
{

    cout << "TransientDomainDecompositionAnalysis - The domain has changed. (Re)Creating model.\n";
    Domain* the_Domain = this->getDomainPtr();
    int stamp = the_Domain->hasDomainChanged();

    domainStamp = stamp;

    int result = 0;

    theAnalysisModel->clearAll();
    theConstraintHandler->clearAll();


    // now we invoke handle() on the constraint handler which
    // causes the creation of FE_Element and DOF_Group objects
    // and their addition to the AnalysisModel.

    cout << "   * Handling constraints\n";

    result = theConstraintHandler->handle();


    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::handle() - ";
        cerr << "ConstraintHandler::handle() failed";
        return -1;
    }


    // we now invoke number() on the numberer which causes
    // equation numbers to be assigned to all the DOFs in the
    // AnalysisModel.

    // cout << "   * Numbering DOFS\n";

    result = theDOF_Numberer->numberDOF();

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::handle() - ";
        cerr << "DOF_Numberer::numberDOF() failed";
        return -2;
    }



    cout << "   * Forming DOF Graph\n";
    Graph &theGraph = theAnalysisModel->getDOFGraph();

    cout << "   * Setting SOE Size\n";
    result = theSOE->setSize(theGraph);
    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::handle() - ";
        cerr << "LinearSOE::setSize() failed";
        return -3;
    }



    // finally we invoke domainChanged on the Integrator and Algorithm
    // objects .. informing them that the model has changed
    cout << "   * Setting up integrator\n";

    result = theIntegrator->domainChanged();

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::setAlgorithm() - ";
        cerr << "Integrator::domainChanged() failed";
        return -4;
    }

    cout << "   * Setting up algorithm\n";

    result = theAlgorithm->domainChanged();

    if (result < 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::setAlgorithm() - ";
        cerr << "Algorithm::domainChanged() failed";
        return -5;
    }

    cout << "   * Done with domain change\n";

    // if get here successfull
    return 0;
}


int
TransientDomainDecompositionAnalysis::getNumExternalEqn(void)
{
    cerr << "TransientDomainDecompositionAnalysis::getNumExternalEqn() - should never be called\n";
    return 0;
}
int
TransientDomainDecompositionAnalysis::getNumInternalEqn(void)
{
    cerr << "TransientDomainDecompositionAnalysis::getNumInternalEqn() - should never be called\n";
    return 0;
}
int
TransientDomainDecompositionAnalysis::newStep(double dT)
{
    this->analyze(dT);
    return 0;

}
int
TransientDomainDecompositionAnalysis::computeInternalResponse(void)
{
    cerr << "TransientDomainDecompositionAnalysis::computeInternalResponse() - should never be called\n";
    return 0;
}
int
TransientDomainDecompositionAnalysis::formTangent(void)
{
    cerr << "TransientDomainDecompositionAnalysis::formTangent() - should never be called\n";
    return 0;
}
int
TransientDomainDecompositionAnalysis::formResidual(void)
{
    cerr << "TransientDomainDecompositionAnalysis::formResidual() - should never be called\n";
    return 0;
}
int
TransientDomainDecompositionAnalysis::formTangVectProduct(Vector &force)
{
    cerr << "TransientDomainDecompositionAnalysis::formTangVectProduct() - should never be called\n";
    return 0;
}

const Matrix &
TransientDomainDecompositionAnalysis::getTangent(void)
{
    static Matrix errMatrix;
    cerr << "TransientDomainDecompositionAnalysis::getTangent() - should never be called\n";
    return errMatrix;
}

const Vector &
TransientDomainDecompositionAnalysis::getResidual(void)
{
    static Vector errVector;
    cerr << "TransientDomainDecompositionAnalysis::getResidual() - should never be called\n";
    return errVector;
}

const Vector &
TransientDomainDecompositionAnalysis::getTangVectProduct(void)
{
    static Vector errVector;
    cerr << "TransientDomainDecompositionAnalysis::getTangVectProduct() - should never be called\n";
    return errVector;
}

int
TransientDomainDecompositionAnalysis::sendSelf(int commitTag, Channel &theChannel)
{

    // receive the data identifyng the objects in the aggregation
    int dataTag = this->getDbTag();
    static ID data(8);

    if (theAlgorithm == 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - no objects exist!\n";
        return -1;
    }

    LinearSOESolver *theSolver = theSOE->getSolver();

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
    if (  theConstraintHandler->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send handler\n";
        return -1;
    }

    if (  theDOF_Numberer->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send numberer\n";
        return -1;
    }

    if (  theAnalysisModel->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send model\n";
        return -1;
    }

    if (  theAlgorithm->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send algorithm\n";
        return -1;
    }

    if (  theSOE->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send SOE\n";
        return -1;
    }
    else
    {
        theSOE->setAnalysisModel(*theAnalysisModel);
    }

    if (  theSolver->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send Solver\n";
        return -1;
    }

    if (  theIntegrator->sendSelf(commitTag, theChannel) != 0)
    {
        cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send integrator\n";
        return -1;
    }

    if (theTest != 0)
        if (theTest->sendSelf(commitTag, theChannel) != 0)
        {
            cerr << "TransientDomainDecompositionAnalysis::sendSelf() - failed to send integrator\n";
            return -1;
        }

    return 0;
}
int
TransientDomainDecompositionAnalysis::receiveSelf(int commitTag, Channel &theChannel,
        FEM_ObjectBroker &theBroker)
{

    Domain *the_Domain = this->getSubdomainPtr();

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
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
            cerr << " - failed to get the LinearSOE\n";
            return -1;
        }
    }

    theSOE->receiveSelf(commitTag, theChannel, theBroker);
    LinearSOESolver *theSolver = theSOE->getSolver();
    theSolver->receiveSelf(commitTag, theChannel, theBroker);
    theSOE->setAnalysisModel(*theAnalysisModel);

    if (theIntegrator == 0 || theIntegrator->getClassTag() != data(6))
    {
        if (theIntegrator != 0)
        {
            delete theIntegrator;
        }

        theIntegrator = theBroker.getNewTransientIntegrator(data(6));

        if (theIntegrator == 0)
        {
            cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
                cerr << "TransientDomainDecompositionAnalysis::receiveSelf";
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
TransientDomainDecompositionAnalysis::setAlgorithm(EquiSolnAlgo &theNewAlgorithm)
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
TransientDomainDecompositionAnalysis::setIntegrator(IncrementalIntegrator &theNewIntegrator)
{
    // invoke the destructor on the old one
    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }

    // set the links needed by the other objects in the aggregation
    Domain *the_Domain = this->getDomainPtr();

    theIntegrator = (TransientIntegrator *)(&theNewIntegrator);

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
TransientDomainDecompositionAnalysis::setLinearSOE(LinearSOE &theNewSOE)
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
TransientDomainDecompositionAnalysis::setConvergenceTest(ConvergenceTest &theConvergenceTest)
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

