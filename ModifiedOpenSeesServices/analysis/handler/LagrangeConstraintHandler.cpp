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
// $Date: 2003/02/14 23:00:46 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/handler/LagrangeConstraintHandler.cpp,v $


// File: ~/analysis/handler/LagrangeConstraintHandler.C
//
// Written: fmk
// Created: May 1998
// Revision: A
//
// What: "@(#) LagrangeConstraintHandler.C, revA"

#include <LagrangeConstraintHandler.h>
#include <stdlib.h>

#include <AnalysisModel.h>
#include <Domain.h>
#include <FE_Element.h>
#include <DOF_Group.h>
#include <Node.h>
#include <Element.h>
#include <NodeIter.h>
#include <ElementIter.h>
#include <SP_ConstraintIter.h>
#include <SP_Constraint.h>
#include <MP_ConstraintIter.h>
#include <MP_Constraint.h>
#include <Integrator.h>
#include <ID.h>
#include <Subdomain.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <LagrangeDOF_Group.h>
#include <LagrangeSP_FE.h>
#include <LagrangeMP_FE.h>
#include <iostream>
using namespace std;



LagrangeConstraintHandler::LagrangeConstraintHandler(double sp, double mp)
    : ConstraintHandler(HANDLER_TAG_LagrangeConstraintHandler),
      alphaSP(sp), alphaMP(mp),
      theFEs(0), theDOFs(0), numFE(0), numDOF(0)
{

}

LagrangeConstraintHandler::~LagrangeConstraintHandler()
{
    // delete the FE_Element and DOF_Group objects
    if (theFEs != 0)
    {
        for (int i = 0; i < numFE; i++)
            if (theFEs[i] != 0)
            {
                delete theFEs[i];
            }

        delete [] theFEs;
    }

    if (theDOFs != 0)
    {
        for (int j = 0; j < numDOF; j++)
            if (theDOFs[j] != 0)
            {
                delete theDOFs[j];
            }

        delete [] theDOFs;
    }
}

int
LagrangeConstraintHandler::handle(const ID* nodesLast)
{
    // first check links exist to a Domain and an AnalysisModel object
    Domain* theDomain = this->getDomainPtr();
    AnalysisModel* theModel = this->getAnalysisModelPtr();
    Integrator* theIntegrator = this->getIntegratorPtr();

    if ((theDomain == 0) || (theModel == 0) || (theIntegrator == 0))
    {
        cerr << "WARNING LagrangeConstraintHandler::handle() - ";
        cerr << " setLinks() has not been called\n";
        return -1;
    }

    // get number ofelements and nodes in the domain
    // and init the theFEs and theDOFs arrays

    int numConstraints = 0;
    SP_ConstraintIter& theSPss = theDomain->getDomainAndLoadPatternSPs();
    SP_Constraint* spPtr;

    while ((spPtr = theSPss()) != 0)
    {
        numConstraints++;
    }

    numConstraints += theDomain->getNumMPs();
    numFE = theDomain->getNumElements() + numConstraints;
    numDOF = theDomain->getNumNodes() + numConstraints;

    // create an array for the FE_elements and zero it
    if ((numFE <= 0) || ((theFEs  = new FE_Element *[numFE]) == 0))
    {
        cerr << "WARNING LagrangeConstraintHandler::handle() - ";
        cerr << "ran out of memory for FE_elements";
        cerr << " array of size " << numFE << endln;
        return -2;
    }

    int i;

    for (i = 0; i < numFE; i++)
    {
        theFEs[i] = 0;
    }

    // create an array for the DOF_Groups and zero it
    if ((numDOF <= 0) || ((theDOFs = new DOF_Group *[numDOF]) == 0))
    {
        cerr << "WARNING LagrangeConstraintHandler::handle() - ";
        cerr << "ran out of memory for DOF_Groups";
        cerr << " array of size " << numDOF << endln;
        return -3;
    }

    for (i = 0; i < numDOF; i++)
    {
        theDOFs[i] = 0;
    }

    //create a DOF_Group for each Node and add it to the AnalysisModel.
    //    : must of course set the initial IDs
    NodeIter& theNod = theDomain->getNodes();
    Node* nodPtr;
    MP_Constraint* mpPtr;
    DOF_Group* dofPtr;

    int numDofGrp = 0;
    int count3 = 0;
    int countDOF = 0;

    while ((nodPtr = theNod()) != 0)
    {
        if ((dofPtr = new DOF_Group(numDofGrp, nodPtr)) == 0)
        {
            cerr << "WARNING LagrangeConstraintHandler::handle() ";
            cerr << "- ran out of memory";
            cerr << " creating DOF_Group " << i << endln;
            return -4;
        }

        // initially set all the ID value to -2

        const ID& id = dofPtr->getID();

        for (int j = 0; j < id.Size(); j++)
        {
            dofPtr->setID(j, -2);
            countDOF++;
        }

        nodPtr->setDOF_GroupPtr(dofPtr);
        theDOFs[numDofGrp++] = dofPtr;
        theModel->addDOF_Group(dofPtr);
    }

    // create the FE_Elements for the Elements and add to the AnalysisModel
    ElementIter& theEle = theDomain->getElements();
    Element* elePtr;

    int numFeEle = 0;
    FE_Element* fePtr;

    while ((elePtr = theEle()) != 0)
    {

        // only create an FE_Element for a subdomain element if it does not
        // do independent analysis .. then subdomain part of this analysis so create
        // an FE_element & set subdomain to point to it.
        if (elePtr->isSubdomain() == true)
        {
            #ifdef _PARALLEL_PROCESSING
            Subdomain* theSub = (Subdomain*)elePtr;

            if (theSub->doesIndependentAnalysis() == false)
            {
                if ((fePtr = new FE_Element(elePtr)) == 0)
                {
                    cerr << "WARNING PlainHandler::handle() - ran out of memory";
                    cerr << " creating FE_Element " << elePtr->getTag() << endln;
                    return -5;
                }

                theFEs[numFeEle] = fePtr;
                numFeEle++;
                theModel->addFE_Element(fePtr);

                theSub->setFE_ElementPtr(fePtr);
            } //  if (theSub->doesIndependentAnalysis() == false) {

            #endif
        }
        else
        {

            // just a regular element .. create an FE_Element for it & add to AnalysisModel
            if ((fePtr = new FE_Element(elePtr)) == 0)
            {
                cerr << "WARNING PlainHandler::handle() - ran out of memory";
                cerr << " creating FE_Element " << elePtr->getTag() << endln;
                return -5;
            }

            theFEs[numFeEle] = fePtr;
            numFeEle++;
            theModel->addFE_Element(fePtr);
        }
    }

    // create the LagrangeSP_FE for the SP_Constraints and
    // add to the AnalysisModel

    SP_ConstraintIter& theSPs = theDomain->getDomainAndLoadPatternSPs();

    while ((spPtr = theSPs()) != 0)
    {
        if ((dofPtr = new LagrangeDOF_Group(numDofGrp, *spPtr)) == 0)
        {
            cerr << "WARNING LagrangeConstraintHandler::handle()";
            cerr << " - ran out of memory";
            cerr << " creating LagrangeDOFGroup " << endln;
            return -5;
        }

        const ID& id = dofPtr->getID();

        for (int j = 0; j < id.Size(); j++)
        {
            dofPtr->setID(j, -2);
            countDOF++;
        }

        theDOFs[numDofGrp++] = dofPtr;
        theModel->addDOF_Group(dofPtr);

        if ((fePtr = new LagrangeSP_FE(*theDomain, *spPtr,
                                       *dofPtr, alphaSP)) == 0)
        {
            cerr << "WARNING LagrangeConstraintHandler::handle()";
            cerr << " - ran out of memory";
            cerr << " creating LagrangeSP_FE " << endln;
            return -5;
        }

        theFEs[numFeEle++] = fePtr;
        theModel->addFE_Element(fePtr);
    }

    // create the LagrangeMP_FE for the MP_Constraints and
    // add to the AnalysisModel

    MP_ConstraintIter& theMPs = theDomain->getMPs();

    while ((mpPtr = theMPs()) != 0)
    {
        if ((dofPtr = new LagrangeDOF_Group(numDofGrp, *mpPtr)) == 0)
        {
            cerr << "WARNING LagrangeConstraintHandler::handle()";
            cerr << " - ran out of memory";
            cerr << " creating LagrangeDOFGroup " << endln;
            return -5;
        }

        const ID& id = dofPtr->getID();

        for (int j = 0; j < id.Size(); j++)
        {
            dofPtr->setID(j, -2);
            countDOF++;
        }

        theDOFs[numDofGrp++] = dofPtr;
        theModel->addDOF_Group(dofPtr);

        if ((fePtr = new LagrangeMP_FE(*theDomain, *mpPtr,
                                       *dofPtr, alphaMP)) == 0)
        {
            cerr << "WARNING LagrangeConstraintHandler::handle()";
            cerr << " - ran out of memory";
            cerr << " creating LagrangeMP_FE " << endln;
            return -5;
        }

        theFEs[numFeEle++] = fePtr;
        theModel->addFE_Element(fePtr);
    }

    theModel->setNumEqn(countDOF);

    // set the number of eqn in the model
    // now see if we have to set any of the dof's to -3
    //    int numLast = 0;
    if (nodesLast != 0)
        for (i = 0; i < nodesLast->Size(); i++)
        {
            int nodeID = (*nodesLast)(i);
            Node* nodPtr = theDomain->getNode(nodeID);

            if (nodPtr != 0)
            {
                dofPtr = nodPtr->getDOF_GroupPtr();

                const ID& id = dofPtr->getID();

                // set all the dof values to -3
                for (int j = 0; j < id.Size(); j++)
                    if (id(j) == -2)
                    {
                        dofPtr->setID(j, -3);
                        count3++;
                    }
                    else
                    {
                        cerr << "WARNING LagrangeConstraintHandler::handle() ";
                        cerr << " - boundary sp constraint in subdomain";
                        cerr << " this should not be - results suspect \n";
                    }
            }
        }

    return count3;
}


void
LagrangeConstraintHandler::clearAll(void)
{
    // delete the FE_Element and DOF_Group objects
    for (int i = 0; i < numFE; i++)
        if (theFEs[i] != 0)
        {
            delete theFEs[i];
        }

    for (int j = 0; j < numDOF; j++)
        if (theDOFs[j] != 0)
        {
            delete theDOFs[j];
        }

    // delete the arrays
    if (theFEs != 0)
    {
        delete [] theFEs;
    }

    if (theDOFs != 0)
    {
        delete [] theDOFs;
    }

    // reset the numbers
    numDOF = 0;
    numFE =  0;
    theFEs = 0;
    theDOFs = 0;

    // for the nodes reset the DOF_Group pointers to 0
    Domain* theDomain = this->getDomainPtr();

    if (theDomain == 0)
    {
        return;
    }

    NodeIter& theNod = theDomain->getNodes();
    Node* nodPtr;

    while ((nodPtr = theNod()) != 0)
    {
        nodPtr->setDOF_GroupPtr(0);
    }
}

int
LagrangeConstraintHandler::sendSelf(int cTag, Channel& theChannel)
{
    Vector data(2);
    int result = 0;
    data(0) = alphaSP;
    data(1) = alphaMP;
    result = theChannel.sendVector(this->getDbTag(), cTag, data);

    if (result != 0)
    {
        cerr << "LagrangeConstraintHandler::sendSelf() - error sending Vector\n";
    }

    return result;
}

int
LagrangeConstraintHandler::receiveSelf(int cTag,
                                    Channel& theChannel,
                                    FEM_ObjectBroker& theBroker)
{
    Vector data(2);
    int result = 0;
    result = theChannel.receiveVector(this->getDbTag(), cTag, data);
    alphaSP = data(0);
    alphaMP = data(1);

    if (result != 0)
    {
        cerr << "LagrangeConstraintHandler::receiveSelf() - error receiving Vector\n";
    }

    return result;
}

