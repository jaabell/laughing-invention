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

// $Revision: 1.4 $
// $Date: 2003/11/18 01:59:04 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/subdomain/ShadowSubdomain.cpp,v $

// Written: fmk
// Revision: A
//
// Description: This file contains the class definition for ShadowSubdomain.
// ShadowSubdomain is a container class. The class is responsible for holding
// and providing access to the Elements, Nodes, LoadCases, SP_Constraints
// and MP_Constraints that have been added to the ShadowSubdomain.
//
// What: "@(#) ShadowSubdomain.C, revA"


#include <ShadowSubdomain.h>
#include <stdlib.h>

#include <Node.h>
#include <Element.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <DomainDecompositionAnalysis.h>
#include <NodalLoad.h>
#include <ElementalLoad.h>
#include <FEM_ObjectBroker.h>
#include <Timer.h>
#include <LoadPattern.h>

#include <ArrayOfTaggedObjects.h>
#include <SingleDomNodIter.h>
#include <SingleDomEleIter.h>
#include <Graph.h>
#include <FE_Element.h>


#include <EquiSolnAlgo.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>
#include <LinearSOESolver.h>
#include <ConvergenceTest.h>


#include <FE_Element.h>

#include <ShadowActorSubdomain.h>
# include <PartitionedDomain.h>

#include <iostream>
using namespace std;


extern PartitionedDomain theDomain;
extern FEM_ObjectBroker  *OPS_OBJECT_BROKER;

int ShadowSubdomain::count = 0;
int ShadowSubdomain::numShadowSubdomains = 0;
ShadowSubdomain **ShadowSubdomain::theShadowSubdomains = 0;


ShadowSubdomain::ShadowSubdomain(int tag,
                                 MachineBroker &theMachineBroker,
                                 FEM_ObjectBroker &theObjectBroker)
    : Shadow(ACTOR_TAGS_SUBDOMAIN, theObjectBroker, theMachineBroker, 0),

      Subdomain(tag),
      msgData(4),
      theElements(0, 128),
      theNodes(0, 128),
      theExternalNodes(0, 128),
      theLoadCases(0, 128),
      numDOF(0), numElements(0), numNodes(0), numExternalNodes(0),
      numSPs(0), numMPs(0), buildRemote(false), gotRemoteData(false),
      theFEele(0),
      theNod(0), theNodalLoad(0), theSP(0),
      theVector(0), theMatrix(0)
{

    numShadowSubdomains++;

    ShadowSubdomain **theCopy = new ShadowSubdomain *[numShadowSubdomains];

    for (int i = 0; i < numShadowSubdomains - 1; i++)
    {
        theCopy[i] = theShadowSubdomains[i];
    }

    if (theShadowSubdomains != 0)
    {
        delete [] theShadowSubdomains;
    }

    theCopy[numShadowSubdomains - 1] = this;

    theShadowSubdomains = theCopy;

    numLoadPatterns = 0;

    //Set the tag on the actor subdomain
    msgData(0) = ShadowActorSubdomain_setTag;
    msgData(1) = tag;
    this->sendID(msgData);
}


ShadowSubdomain::ShadowSubdomain(int tag,
                                 Channel &the_Channel,
                                 FEM_ObjectBroker &theObjectBroker)
    : Shadow(the_Channel, theObjectBroker),
      Subdomain(tag),
      msgData(4),
      theElements(0, 128),
      theNodes(0, 128),
      theExternalNodes(0, 128),
      theLoadCases(0, 128),
      numDOF(0), numElements(0), numNodes(0), numExternalNodes(0),
      numSPs(0), numMPs(0), buildRemote(false), gotRemoteData(false),
      theFEele(0),
      theNod(0), theNodalLoad(0), theSP(0),
      theVector(0), theMatrix(0)
{

    numShadowSubdomains++;

    ShadowSubdomain **theCopy = new ShadowSubdomain *[numShadowSubdomains];

    for (int i = 0; i < numShadowSubdomains - 1; i++)
    {
        theCopy[i] = theShadowSubdomains[i];
    }

    if (theShadowSubdomains != 0)
    {
        delete [] theShadowSubdomains;
    }

    theCopy[numShadowSubdomains - 1] = this;

    theShadowSubdomains = theCopy;

    numLoadPatterns = 0;
}

ShadowSubdomain::~ShadowSubdomain()
{
    // send a message to the remote actor telling it to shut sown
    msgData(0) = ShadowActorSubdomain_DIE;
    this->sendID(msgData);
}



int
ShadowSubdomain::getRemoteData(void)
{
    if (buildRemote == true && gotRemoteData == false)
    {
        //Tell remote to send the data
        msgData(0) = ShadowActorSubdomain_getRemoteData;

        //Listen to what remote is saying
        this->sendID(msgData);
        this->receiveID(msgData);

        //Unwrap the message and interpret it
        numExternalNodes = msgData(0);
        numDOF = msgData(1);

        if (theExternalNodes.Size() != numExternalNodes)
        {
            theExternalNodes[numExternalNodes - 1] = 0;    // this will resize
        }

        if (theExternalNodes.Size() != numExternalNodes)
        {
            cerr << "ShadowSubdomain::getRemoteData(void) - need to resize the ID\n";
            return -1;
        }

        //If all went well, the remote will send the external nodes... listen to it
        if (numExternalNodes != 0)
        {
            this->receiveID(theExternalNodes);
        }
    }

    gotRemoteData = true;
    return 0;
}

bool
ShadowSubdomain::addElement(Element *theEle)
{
    //Tell the actor domain to add an element with the properties in the message
    int tag = theEle->getTag();

    msgData(0) = ShadowActorSubdomain_addElement;
    msgData(1) = theEle->getClassTag();
    msgData(2) = theEle->getDbTag();
    msgData(3) = theEle->getMaterialTag();

    this->sendID(msgData);
    this->sendObject(*theEle);

    theElements[numElements] = tag;
    numElements++;

    delete theEle;

    return true;
}

bool
ShadowSubdomain::addNode(Node *theNode)
{
    int tag = theNode->getTag();

    msgData(0) = ShadowActorSubdomain_addNode;
    msgData(1) = theNode->getClassTag();
    msgData(2) = theNode->getDbTag();

    this->sendID(msgData);
    this->sendObject(*theNode);

    theNodes[numNodes] = tag;
    numNodes++;
    numDOF += theNode->getNumberDOF();

    delete theNode;

    return true;
}

bool
ShadowSubdomain::addExternalNode(Node *theNode)
{
    int tag = theNode->getTag();

    msgData(0) = ShadowActorSubdomain_addExternalNode;
    msgData(1) = theNode->getClassTag();
    msgData(2) = theNode->getDbTag();

    this->sendID(msgData);
    this->sendObject(*theNode);

    theNodes[numNodes] = tag;
    theExternalNodes[numExternalNodes] = tag;

    numNodes++;
    numExternalNodes++;
    numDOF += theNode->getNumberDOF();

    return true;
}

bool
ShadowSubdomain::addSP_Constraint(SP_Constraint *theSP)
{

    msgData(0) = ShadowActorSubdomain_addSP_Constraint;
    msgData(1) = theSP->getClassTag();
    msgData(2) = theSP->getDbTag();

    this->sendID(msgData);
    this->sendObject(*theSP);
    numSPs++;

    return true;
}

bool
ShadowSubdomain::addMP_Constraint(MP_Constraint *theMP)
{
    msgData(0) = ShadowActorSubdomain_addMP_Constraint;
    msgData(1) = theMP->getClassTag();
    msgData(2) = theMP->getDbTag();

    this->sendID(msgData);
    this->sendObject(*theMP);

    numMPs++;

    return true;
}


bool
ShadowSubdomain::addLoadPattern(LoadPattern *thePattern)
{

    msgData(0) = ShadowActorSubdomain_addLoadPattern;
    msgData(1) = thePattern->getClassTag();
    msgData(2) = thePattern->getDbTag();

    this->sendID(msgData);
    this->sendObject(*thePattern);

    numLoadPatterns++;
    return true;
}



bool
ShadowSubdomain::addSP_Constraint(SP_Constraint *theSP, int loadPattern)
{
    // Tell remote to add constraint
    msgData(0) = ShadowActorSubdomain_addSP_ConstraintToPattern;
    msgData(1) = theSP->getClassTag();
    msgData(2) = theSP->getDbTag();
    msgData(3) = loadPattern;
    this->sendID(msgData);
    this->sendObject(*theSP);
    numSPs++;

    return true;
}

bool
ShadowSubdomain::addNodalLoad(NodalLoad *theLoad, int loadPattern)
{
    //Tell remote to add a new nodal load
    msgData(0) = ShadowActorSubdomain_addNodalLoadToPattern;
    msgData(1) = theLoad->getClassTag();
    msgData(2) = theLoad->getDbTag();
    msgData(3) = loadPattern;
    this->sendID(msgData);
    this->sendObject(*theLoad);

    return true;
}

bool
ShadowSubdomain::addElementalLoad(ElementalLoad *theLoad, int loadPattern)
{
    //Tell remote about a new element load

    msgData(0) = ShadowActorSubdomain_addElementalLoadToPattern;
    msgData(1) = theLoad->getClassTag();
    msgData(2) = theLoad->getDbTag();
    msgData(3) = loadPattern;
    this->sendID(msgData);
    this->sendObject(*theLoad);

    // delete theLoad;

    return true;
}


bool
ShadowSubdomain::hasNode(int tag)
{
    //Enquire whether the remote has a certain node tag
    msgData(0) = ShadowActorSubdomain_hasNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);

    if (msgData(0) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
ShadowSubdomain::hasInternalNode(int tag)
{
    //Enquire wheter a the remote has an internal node with a given tag
    msgData(0) = ShadowActorSubdomain_hasInternalNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);

    if (msgData(0) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
ShadowSubdomain::hasExternalNode(int tag)
{

    //Enquire wheter a the remote has an external node with a given tag
    msgData(0) = ShadowActorSubdomain_hasExternalNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);

    if (msgData(0) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
ShadowSubdomain::hasElement(int tag)
{

    //Enquire wheter a the remote has an element with a given tag
    msgData(0) = ShadowActorSubdomain_hasElement;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);

    if (msgData(0) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


Element *
ShadowSubdomain::removeElement(int tag)
{
    int loc = theElements.removeValue(tag);

    if (loc < 0)
    {
        return 0;
    }
    else   // the element is there go get it
    {
        // send a message to remove the object from the actor
        msgData(0) = ShadowActorSubdomain_removeElement;
        msgData(1) = tag;
        this->sendID(msgData);

        numElements--;

        return 0;
    }
}

Node *
ShadowSubdomain::removeNode(int tag)
{
    int loc = theNodes.removeValue(tag);

    if (loc < 0)
    {
        cerr << "ShadowSubdomain::removeNode - failed to remove node from subdomain!\n";
        return 0;
    }
    else   // the node is there, go get it
    {
        // send a message to actor requesting node
        msgData(0) = ShadowActorSubdomain_removeNode;
        msgData(1) = tag;
        this->sendID(msgData);

        if ( loc >= 0 )
        {
            numNodes--;
        }

        // remove from external as well
        loc = theExternalNodes.removeValue(tag);

        if (loc >= 0)
        {
            numExternalNodes--;
        }

        // receive the node from the actor
        this->receiveID(msgData);

        if (msgData(0) == 0)
        {
            numDOF -= msgData(1);
        }
        else
        {
            cerr << "ShadowSubdomain::removeNode() - failed to remove node from subdomain!\n";
            exit(1);
        }

        return 0;
    }
}

SP_Constraint *
ShadowSubdomain::removeSP_Constraint(int tag)
{
    msgData(0) = ShadowActorSubdomain_removeSP_Constraint;
    msgData(1) = tag;

    this->sendID(msgData);
    numSPs--;

    return 0;
}

MP_Constraint *
ShadowSubdomain::removeMP_Constraint(int tag)
{
    msgData(0) = ShadowActorSubdomain_removeMP_Constraint;
    msgData(1) = tag;

    this->sendID(msgData);

    numMPs--;
    return 0;
}

LoadPattern *
ShadowSubdomain::removeLoadPattern(int loadTag)
{
    msgData(0) = ShadowActorSubdomain_removeLoadPattern;
    msgData(1) = loadTag;

    this->sendID(msgData);
    return 0;
}

NodalLoad *
ShadowSubdomain::removeNodalLoad(int loadTag, int loadPattern)
{
    msgData(0) = ShadowActorSubdomain_removeNodalLoadFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);

    return 0;
}



ElementalLoad *
ShadowSubdomain::removeElementalLoad(int loadTag, int loadPattern)
{
    msgData(0) = ShadowActorSubdomain_removeElementalLoadFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);

    return 0;
}

SP_Constraint *
ShadowSubdomain::removeSP_Constraint(int loadTag, int loadPattern)
{
    msgData(0) = ShadowActorSubdomain_removeSP_ConstraintFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);

    return 0;
}


ElementIter &
ShadowSubdomain::getElements()
{
    cerr << "ShadowSubdomain::getElements() ";
    cerr << " - SHOULD NEVER BE CALLED - EXITING\n";
    exit(-1);

    // this will never be called - just for a strict compiler
    ArrayOfTaggedObjects *theEror = new ArrayOfTaggedObjects(1);
    ElementIter *theIter = new SingleDomEleIter(theEror);
    return *theIter;
}

NodeIter &
ShadowSubdomain::getNodes()
{
    cerr << "ShadowSubdomain::getNodes() ";
    cerr << " - SHOULD NEVER BE CALLED - EXITING\n";
    exit(-1);

    // this will never be called - just for a strict compiler
    ArrayOfTaggedObjects *theEror = new ArrayOfTaggedObjects(1);
    NodeIter *theIter = new SingleDomNodIter(theEror);
    return *theIter;
}

NodeIter &
ShadowSubdomain::getInternalNodeIter(void)
{
    cerr << "ShadowSubdomain::getInternalNodeIter() ";
    cerr << " - SHOULD NEVER BE CALLED - EXITING\n";
    exit(-1);

    // this will never be called - just for a strict compiler
    ArrayOfTaggedObjects *theEror = new ArrayOfTaggedObjects(1);
    NodeIter *theIter = new SingleDomNodIter(theEror);
    return *theIter;
}

NodeIter &
ShadowSubdomain::getExternalNodeIter(void)
{
    cerr << "ShadowSubdomain::getExternalNodeIter() ";
    cerr << " - SHOULD NEVER BE CALLED - EXITING\n";
    exit(-1);

    // this will never be called - just for a strict compiler
    ArrayOfTaggedObjects *theEror = new ArrayOfTaggedObjects(1);
    NodeIter *theIter = new SingleDomNodIter(theEror);
    return *theIter;
}


Element *
ShadowSubdomain::getElement(int tag)
{
    if (theElements.getLocation(tag) < 0)
    {
        return 0;
    }

    msgData(0) = ShadowActorSubdomain_getElementPtr;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);
    int theType = msgData(0);

    Element *theEle = theBroker->getNewElement(theType);

    if (theEle != 0)
    {
        this->receiveObject(*theEle);
    }


    return theEle;
}


Element *
ShadowSubdomain::getElementPtr(int tag)
{
    if (theElements.getLocation(tag) < 0)
    {
        return 0;
    }

    msgData(0) = ShadowActorSubdomain_getElementPtr;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);
    int theType = msgData(0);

    Element *theEle = theBroker->getNewElement(theType);

    if (theEle != 0)
    {
        this->receiveObject(*theEle);
    }

    cerr << "ShadowSubdomain::getElementPtr() ";
    cerr << " - SHOULD BE AVOIDED IF POSSIBLE \n";

    return theEle;
}

Node *
ShadowSubdomain::getNodePtr(int tag)
{
    if (theNodes.getLocation(tag) < 0)
    {
        return 0;
    }

    msgData(0) = ShadowActorSubdomain_getNodePtr;
    msgData(1) = tag;
    this->sendID(msgData);
    this->receiveID(msgData);
    int theType = msgData(0);

    theNod = NULL;
    theNod = theBroker->getNewNode(theType);

    if (theNod != 0)
    {
        this->receiveObject(*theNod);
    }

    return theNod;
}

Node *
ShadowSubdomain::getNode(int tag)
{
    if (theNodes.getLocation(tag) < 0)
    {
        return 0;
    }

    msgData(0) = ShadowActorSubdomain_getNodePtr;
    msgData(1) = tag;
    this->sendID(msgData);


    theNod = NULL;
    theNod = theBroker->getNewNode(NOD_TAG_Node);

    if (theNod != 0)
    {
        this->receiveObject(*theNod);
    }

    //cerr << "ShadowSubdomain::getNodPtr() ";
    //cerr << " - SHOULD BE AVOIDED IF POSSIBLE \n";

    return theNod;
}


int
ShadowSubdomain::getNumElements(void) const
{
    return numElements;
}

int
ShadowSubdomain::getNumLoadPatterns(void) const
{
    return numLoadPatterns;
}

int
ShadowSubdomain::getNumNodes(void) const
{
    return numNodes;
}

int
ShadowSubdomain::getNumSPs(void) const
{
    return numSPs;
}

int
ShadowSubdomain::getNumMPs(void) const
{
    return numMPs;
}


Graph *
ShadowSubdomain::getElementGraph(void)
{
    cerr << "ShadowSubdomain::getElementGraph() ";
    cerr << " - NOT YET IMPLEMENTED - EXITING\n";
    exit(-1);

    // this will never be called, needed for a strict compiler
    Graph *theGraph = new Graph;
    return theGraph;
}

Graph &
ShadowSubdomain::getNodeGraph(void)
{
    cerr << "ShadowSubdomain::getNodeGraph() ";
    cerr << " - NOT YET IMPLEMENTED - EXITING\n";
    exit(-1);

    // this will never be called, needed for a strict compiler
    Graph *theGraph = new Graph;
    return *theGraph;
}


void
ShadowSubdomain::applyLoad(double time)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_applyLoad;
        Vector data(4);
        data(0) = time;

        this->sendID(msgData);
        this->sendVector(data);
    }
}


void
ShadowSubdomain::setCommitTag(int newTag)
{
    msgData(0) = ShadowActorSubdomain_setCommitTag;
    msgData(1) = newTag;

    this->sendID(msgData);
}

void
ShadowSubdomain::setCurrentTime(double time)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_setCurrentTime;
        Vector data(4);
        data(0) = time;

        this->sendID(msgData);
        this->sendVector(data);
    }
}

void
ShadowSubdomain::setCommittedTime(double time)
{
    msgData(0) = ShadowActorSubdomain_setCommittedTime;
    Vector data(4);
    data(0) = time;

    this->sendID(msgData);
    this->sendVector(data);
}

void
ShadowSubdomain::setLoadConstant(void)
{
    msgData(0) = ShadowActorSubdomain_setLoadConstant;

    this->sendID(msgData);
}


int
ShadowSubdomain::update(void)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) =  ShadowActorSubdomain_update;
        this->sendID(msgData);
    }

    return 0;
}

int
ShadowSubdomain::update(double newTime, double dT)
{
    static Vector data(2);
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) =  ShadowActorSubdomain_updateTimeDt;
        this->sendID(msgData);
        data(0) = newTime;
        data(1) = dT;
        this->sendVector(data);
    }

    return 0;
}


int
ShadowSubdomain::barrierCheckIN(void)
{
    static ID data(1);
    this->receiveID(data);
    return data(0);
}

int
ShadowSubdomain::barrierCheckOUT(int result)
{
    static ID data(1);
    data(0) = result;
    this->sendID(data);

    return 0;
}


int
ShadowSubdomain::commit(void)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_commit;
        this->sendID(msgData);
        return 0;
    }

    return 0;
}

/*************************************************************************************
* Added by Sumeet 3rd September, 2016, to output converged step 
* Call this function to output at every converged step and write in HDF5 file
**************************************************************************************/
int
ShadowSubdomain::output_step(void)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_output_step;
        this->sendID(msgData);
        return 0;
    }

    return 0;
}

/*************************************************************************************
* Added by Sumeet 3rd August, 2016, to output substep iteration steps for debugging 
* The function commits at every substep i,e the trail displacements and trial element
* output HDF5 Output file. It does not commit any displacements or element output
**************************************************************************************/
int
ShadowSubdomain::output_iteration( int global_iteration_no )
{
    static Vector data(1);
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_output_iteration;
        data(0)    = global_iteration_no;
        this->sendID(msgData);
        this->sendVector(data);
        return 0;
    }

    return 0;
}

int
ShadowSubdomain::revertToLastCommit(void)
{
    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        msgData(0) = ShadowActorSubdomain_revertToLastCommit;
        this->sendID(msgData);
        return 0;
    }

    return 0;
}

int
ShadowSubdomain::revertToStart(void)
{
    msgData(0) = ShadowActorSubdomain_revertToStart;
    this->sendID(msgData);
    return 0;
}



void
ShadowSubdomain::setDomainDecompAnalysis(DomainDecompositionAnalysis &theDDAnalysis)
{
    msgData(0) = ShadowActorSubdomain_setDomainDecompAnalysis;
    msgData(1) = theDDAnalysis.getClassTag();

    this->sendID(msgData);
    this->sendObject(theDDAnalysis);

    this->Subdomain::setDomainDecompAnalysis(theDDAnalysis);
}

int
ShadowSubdomain::setAnalysisAlgorithm(EquiSolnAlgo &theAlgorithm)
{
    msgData(0) = ShadowActorSubdomain_setAnalysisAlgorithm;
    msgData(1) = theAlgorithm.getClassTag();

    this->sendID(msgData);
    this->sendObject(theAlgorithm);
    return 0;
}

int
ShadowSubdomain::setAnalysisIntegrator(IncrementalIntegrator &theIntegrator)
{
    msgData(0) = ShadowActorSubdomain_setAnalysisIntegrator;
    msgData(1) = theIntegrator.getClassTag();

    this->sendID(msgData);
    this->sendObject(theIntegrator);
    return 0;
}

int
ShadowSubdomain::setAnalysisLinearSOE(LinearSOE &theSOE)
{
    msgData(0) = ShadowActorSubdomain_setAnalysisLinearSOE;
    msgData(1) = theSOE.getClassTag();

    LinearSOESolver *theSolver = theSOE.getSolver();
    msgData(2) = theSolver->getClassTag();

    this->sendID(msgData);
    this->sendObject(theSOE);
    this->sendObject(*theSolver);

    return 0;
}

int
ShadowSubdomain::setAnalysisConvergenceTest(ConvergenceTest &theTest)
{
    msgData(0) = ShadowActorSubdomain_setAnalysisConvergenceTest;
    msgData(1) = theTest.getClassTag();

    this->sendID(msgData);
    this->sendObject(theTest);

    cerr << "ShadowSubdomain::setAnalysisConvergenceTest(ConvergenceTest &theTest)\n";
    return 0;
}



void
ShadowSubdomain::domainChange(void)
{
    msgData(0) = ShadowActorSubdomain_domainChange;
    this->sendID(msgData);
}

void
ShadowSubdomain::clearAnalysis(void)
{
    msgData(0) = ShadowActorSubdomain_clearAnalysis;
    this->sendID(msgData);
}

int
ShadowSubdomain::getNumExternalNodes(void) const
{
    if (gotRemoteData == false && buildRemote == true)
    {
        cerr << "WARNING: ShadowSubdomain::getNumExternalNodes()";
        cerr << " - not yet received the data\n";
    }

    return numExternalNodes;
}

const ID &
ShadowSubdomain::getExternalNodes(void)
{
    // if the subdoamin was built remotly need to get it's data
    if (gotRemoteData == false && buildRemote == true)
    {
        this->getRemoteData();
    }

    return theExternalNodes;
}

int
ShadowSubdomain::getNumDOF(void)
{
    // if the subdoamin was built remotly need to get it's data
    if (gotRemoteData == false && buildRemote == true)
    {
        this->getRemoteData();
    }

    //GZ return numDOF;
    return 1; //Just to get rayleigh damping
}


const Matrix &
ShadowSubdomain::getTang(void)
{
    cerr << "ShadowSubdomain::getTang(void) should never be called!\n";

    theMatrix = new Matrix(1, 1);

    return *theMatrix;
}



const Vector &
ShadowSubdomain::getResistingForce(void)
{
    cerr << "ShadowSubdomain::getResistingForce(void) should never be called!\n";

    theVector = new Vector(1);

    return *theVector;
}


int
ShadowSubdomain::computeTang(void)
{
    count++;

    if (count == 1)
    {
        msgData(0) = ShadowActorSubdomain_computeTang;
        msgData(1) = this->getTag();
        this->sendID(msgData);

        for (int i = 0; i < numShadowSubdomains; i++)
        {
            ShadowSubdomain *theShadow = theShadowSubdomains[i];

            if (theShadow != this)
            {
                theShadow->computeTang();
            }
        }
    }
    else if (count <= numShadowSubdomains)
    {
        msgData(0) = ShadowActorSubdomain_computeTang;
        msgData(1) = this->getTag();
        this->sendID(msgData);
    }
    else if (count == 2 * numShadowSubdomains - 1)
    {
        count = 0;
    }

    return 0;
}

int
ShadowSubdomain::computeResidual(void)
{
    count++;

    if (count == 1)
    {
        msgData(0) = ShadowActorSubdomain_computeResidual;
        this->sendID(msgData);

        for (int i = 0; i < numShadowSubdomains; i++)
        {
            ShadowSubdomain *theShadow = theShadowSubdomains[i];

            if (theShadow != this)
            {
                theShadow->computeResidual();
            }
        }
    }
    else if (count <= numShadowSubdomains)
    {
        msgData(0) = ShadowActorSubdomain_computeResidual;
        this->sendID(msgData);
    }
    else if (count == 2 * numShadowSubdomains - 1)
    {
        count = 0;
    }

    return 0;
}



const Vector &
ShadowSubdomain::getLastExternalSysResponse(void)
{
    cerr << "ShadowSubdomain::getLastExternalSysResponse() ";
    cerr << " SHOULD NEVER BE CALLED\n";
    exit(0);

    // this should never be called, provided for a strict compiler
    Vector *theVect = new Vector(0);
    return *theVect;
}

int
ShadowSubdomain::computeNodalResponse(void)
{

    DomainDecompositionAnalysis *theDDA = this->getDDAnalysis();

    if (theDDA != 0 && theDDA->doesIndependentAnalysis() != true)
    {
        FE_Element *theFePtr = this->getFE_ElementPtr();

        if (theFePtr != 0)
        {

            const Vector &lastChange = theFePtr->getLastResponse();
            msgData(0) =  ShadowActorSubdomain_computeNodalResponse;
            msgData(1) = lastChange.Size();

            if (numDOF != msgData(1))
            {
                cerr << "ShadowSubdomain::update(void)";
                cerr << " - numDOF " << numDOF << " and size of Vector ";
                cerr << msgData(1) << "do not agree?\n";
                numDOF = msgData(1);
            }

            this->sendID(msgData);
            Vector theChange(lastChange);
            this->sendVector(theChange);
        }
    }

    return 0;
}


int
ShadowSubdomain::newStep(double dT)
{
    msgData(0) =  ShadowActorSubdomain_newStep;
    this->sendID(msgData);
    static Vector timeStep(4);
    timeStep(0) = dT;
    this->sendVector(timeStep);
    return 0;
}


double
ShadowSubdomain::getCost(void)
{
    cerr << "ShadowSubdomain::getCost()";
    cerr << " - Deprecated";

    return 0.0;
}


int
ShadowSubdomain::sendSelf(int cTag, Channel &the_Channel)
{
    cerr << "ShadowSubdomain::sendSelf() ";
    cerr << " - NOT YET IMPLEMENTED\n";
    return -1;
}

int
ShadowSubdomain::receiveSelf(int cTag, Channel &the_Channel,
                             FEM_ObjectBroker &the_Broker)
{
    cerr << "ShadowSubdomain::receiveSelf() ";
    cerr << " - NOT YET IMPLEMENTED\n";
    return -1;
}


void
ShadowSubdomain::Print(ostream &s, int flag)
{
    msgData(0) = ShadowActorSubdomain_Print;

    this->sendID(msgData);
    this->receiveID(msgData);
}


int
ShadowSubdomain::buildEleGraph()
{
    msgData(0) = ShadowActorSubdomain_BuildElementGraph;
    this->sendID(msgData);

    return 0;
}

int
ShadowSubdomain::buildNodeGraph(Graph *theNodeGraph)
{
    cerr << "ShadowSubdomain::buildNodeGraph() ";
    cerr << " - NOT YET IMPLEMENTED\n";
    return -1;
}

int
ShadowSubdomain::buildMap(void)
{
    mapBuilt = true;
    return 0;
}



int
ShadowSubdomain::recvChangedNodes(const ID &nodeList, int numNodes)
{
    msgData(0) = ShadowActorSubdomain_recvChangedNodeList;
    msgData(1) = numNodes;
    this->sendID(msgData);
    //static ID data(nodeList, numNodes);
    this->sendID(nodeList);
    return 0;
}

int
ShadowSubdomain::partition(int numParts) //Guanzhou added for ParMetis
{
    msgData(0) = ShadowActorSubdomain_Partition;
    msgData(1) = numParts;

    this->sendID(msgData);
    return 0;

}


int
ShadowSubdomain::repartition(int numParts)//Guanzhou added
{
    msgData(0) = ShadowActorSubdomain_Repartition;
    msgData(1) = numParts;

    this->sendID(msgData);
    return 0;
}

int
ShadowSubdomain::ChangeMPIChannel(int other)
{
    msgData(0) = ShadowActorSubdomain_ChangeMPIChannel;
    msgData(1) = other;

    this->sendID(msgData);
    return 0;

}

int
ShadowSubdomain::restoreChannel(void)
{
    msgData(0) = ShadowActorSubdomain_restoreChannel;
    this->sendID(msgData);
    return 0;
}


int
ShadowSubdomain::reDistributeData(int numParts)
{
    msgData(0) = ShadowActorSubdomain_reDistributeData;
    msgData(1) = numParts;
    this->sendID(msgData);
    bool done = false;

    while (!done)
    {
        this->receiveID(msgData);
        int action = msgData(0);
        //NodalLoad *theNodalLoad;
        //SP_Constraint *theSP;
        int theType, dbTag, loadPatternTag;

        switch (action)
        {

        case Master_addSP_Constraint:
            theType = msgData(1);
            dbTag = msgData(2);

            theSP = OPS_OBJECT_BROKER->getNewSP(theType);

            if (theSP != 0)
            {
                theSP->setDbTag(dbTag);
                this->receiveObject(*theSP);
                bool result = theDomain.addSP_Constraint(theSP);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case Master_addNodalLoadToPattern:
            theType = msgData(1);
            dbTag = msgData(2);
            loadPatternTag = msgData(3);

            theNodalLoad = OPS_OBJECT_BROKER->getNewNodalLoad(theType);

            if (theNodalLoad != 0)
            {
                theNodalLoad->setDbTag(dbTag);
                this->receiveObject(*theNodalLoad);
                bool result = theDomain.addNodalLoad(theNodalLoad, loadPatternTag);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case Master_addSP_ConstraintToPattern:
            theType = msgData(1);
            dbTag = msgData(2);
            loadPatternTag = msgData(3);

            theSP = OPS_OBJECT_BROKER->getNewSP(theType);

            if (theSP != 0)
            {
                theSP->setDbTag(dbTag);
                this->receiveObject(*theSP);
                bool result = theDomain.addSP_Constraint(theSP, loadPatternTag);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case Master_hasNode:
            theType = msgData(1);

            //bool res = theDomain.hasNode(theType);
            //if (res == true)
            if ( theDomain.getNode(theType) != 0)
            {
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            break;

        case Subdomain_Redistribution_DONE:
            done = true;
            break;

        default:
            cerr << "ShadowSubdomain::invalid action " << action << "received\n";
            msgData(0) = -1;
            break;
        }
    }

    buildRemote = true;
    gotRemoteData = false;
    this->getRemoteData();

    return 0;
}

int
ShadowSubdomain::swapNodeFromExternalToInternal(int nodeTag, int dof)
{
    msgData(0) = ShadowActorSubdomain_swapNodeFromExternalToInternal;
    msgData(1) = nodeTag;
    this->sendID(msgData);

    //numNodes--;
    int loc = theExternalNodes.removeValue(nodeTag);

    if (loc >= 0)
    {
        numExternalNodes--;
        //numDOF -= dof;
    }
    else
    {
        cerr << "ShadowSubdomain::swapNodeFromExternalToInternal - failed to remove from external nodes " << nodeTag << "\n";
    }

    return 0;
}

int
ShadowSubdomain::exportInternalNode(int nodeTag, int destination, int dof)
{
    msgData(0) = ShadowActorSubdomain_exportInternalNode;
    msgData(1) = nodeTag;
    msgData(2) = destination;

    this->sendID(msgData);

    if ( destination == 0 )   //export internal to master
    {
        this->receiveID(msgData);
        int theType = msgData(1);
        int dbTag = msgData(2);
        //if ( theNod != 0 ) delete theNod;
        theNod = theBroker->getNewNode(theType);

        if (theNod != 0)
        {
            theNod->setDbTag(dbTag);
            this->receiveObject(*theNod);
            theDomain.addNode(theNod);
            //theDomain.recvNode(theNod);
            //delete theNod;
        }
    }

    int loc = theNodes.removeValue(nodeTag);

    if (loc < 0)
    {
        return 0;
    }
    else   // the node is there, go get it
    {
        numNodes--;
        numDOF -= dof;
    }

    return 0;
}


int
ShadowSubdomain::addNodeTag(int nodeTag, int dof)
{
    theNodes[numNodes] = nodeTag;
    numDOF += dof;
    numNodes++;

    return 0;
}


int
ShadowSubdomain::resetSubMultipleSupport(const int loadPatternTag)
{
    msgData(0) = ShadowActorSubdomain_resetSubMultipleSupport;
    msgData(1) = loadPatternTag;
    this->sendID(msgData);
    return 0;
}


int
ShadowSubdomain::swapNodeFromInternalToExternal(int nodeTag)
{
    msgData(0) = ShadowActorSubdomain_swapNodeFromInternalToExternal;
    msgData(1) = nodeTag;
    this->sendID(msgData);

    this->receiveID(msgData);//get the node back in master
    int theType = msgData(1);
    int dbTag = msgData(2);

    theNod = theBroker->getNewNode(theType);

    if (theNod != 0)
    {
        theNod->setDbTag(dbTag);
        this->receiveObject(*theNod);

        theDomain.addNode(theNod);
        int tag = theNod->getTag();

        theExternalNodes[numExternalNodes] = tag;
        numExternalNodes++;
    }

    return 0;
}






// int ShadowSubdomain::setNumberOfOutputSteps(int nsteps)
// {
//     return this->setNumberOfOutputSteps(nsteps);
// }

// int ShadowSubdomain::setOutputEveryNsteps(int output_every_nsteps)
// {
//     return this->setOutputEveryNsteps(output_every_nsteps);
// }


// int ShadowSubdomain::enableOutput(bool is_output_enabled)
// {
//     return this->enableOutput(is_output_enabled);
// }

// int ShadowSubdomain::enableElementOutput(bool is_element_output_enabled)
// {
//     return this->enableElementOutput(is_element_output_enabled);
// }

int ShadowSubdomain::sendOutputOptionsToSubdomain()
{

    msgData(0) = ShadowActorSubdomain_sendOutputOptions;

    this->sendID(msgData);

    ID options(7);
    options(0) = output_is_enabled;
    options(1) = element_output_is_enabled;
    options(2) = have_written_static_mesh_data;
    options(3) = output_every_nsteps;
    options(4) = countdown_til_output;
    options(5) = theOutputWriter.get_number_of_time_steps();
    options(6) = theOutputWriter.get_zlib_compression_level();

    this->sendID(options);

    return 0;
}



int ShadowSubdomain::setConstitutiveIntegrationMethod(int algorithm,
        double f_relative_tol, double stress_relative_tol, int n_max_iterations)
{

    // cout << "ShadowSubdomain # " << this->getTag() << " sending info\n";

    msgData(0) = ShadowActorSubdomain_setConstitutiveIntegrationMethod;

    this->sendID(msgData);

    Vector dbl_data(4);
    dbl_data(0) = algorithm;
    dbl_data(1) = f_relative_tol;
    dbl_data(2) = stress_relative_tol;
    dbl_data(3) = n_max_iterations;

    this->sendVector(dbl_data);

    return 0;

    // int errorflag = -1;
    // if ((errorflag = NDMaterialLT::set_constitutive_integration_method(algorithm, f_relative_tol, stress_relative_tol, n_max_iterations)))
    // {
    //     errorflag = 0;
    // }
    // else
    // {
    //     cerr << "define_NDMaterialLT_constitutive_integration_algorithm() - Error. Constitutive algorithm not available.\n";
    // }
    // return errorflag;
}


int ShadowSubdomain::getOutputSize() const
{
    // cerr << "ShadowSubdomain::getOutputSize() -- Subclass responsability.\n\n";
    return 0;
}

const Vector &ShadowSubdomain::getOutput()
{
    // cerr << "ShadowSubdomain::getOutput() -- Subclass responsability.\n\n";
    return *Element::SizedVector1;
}

