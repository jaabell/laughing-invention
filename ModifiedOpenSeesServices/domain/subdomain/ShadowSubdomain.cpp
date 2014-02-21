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
//#include <PartitionedModelBuilder.h>

#include <EquiSolnAlgo.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>
#include <LinearSOESolver.h>
#include <ConvergenceTest.h>
// #include <Recorder.h>

#include <FE_Element.h>

#include <ShadowActorSubdomain.h>
# include <PartitionedDomain.h>

#include <iostream>
using namespace std;


extern PartitionedDomain theDomain; //Global needed, Guanzhou added
extern FEM_ObjectBroker  *OPS_OBJECT_BROKER;

#ifdef _BABAK_DEBUG
extern LoadPattern *DRM_LOAD_PATTERN ;
#endif

int ShadowSubdomain::count = 0; // MHS
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
      theVector(0), theMatrix(0), theNod(0), theSP(0), theNodalLoad(0)
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

    // does nothing
    numLoadPatterns = 0;

    msgData(0) = ShadowActorSubdomain_setTag;
    msgData(1) = tag;
    this->sendID(msgData);

    //Guanzhou out this->setCommitTag(tag);
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
      theVector(0), theMatrix(0), theNod(0), theSP(0), theNodalLoad(0)
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

    // does nothing
    numLoadPatterns = 0;
}

ShadowSubdomain::~ShadowSubdomain()
{
    // send a message to the remote actor telling it to shut sown
    msgData(0) = ShadowActorSubdomain_DIE;
    this->sendID(msgData);
    cerr << "ShadowSubdomain::~ShadowSubdomain() DONE\n";
}


// int
// ShadowSubdomain::buildSubdomain(int numSubdomains, PartitionedModelBuilder &theBuilder)
// {
//     // send a message identify setting ModelBuilder and it's class tag
//     buildRemote = true;
//     gotRemoteData = false;

//     msgData(0) = ShadowActorSubdomain_buildSubdomain;
//     msgData(1) = theBuilder.getClassTag();
//     msgData(2) = numSubdomains;
//     msgData(3) = this->getTag();
//     this->sendID(msgData);

//     // send the builder
//     this->sendObject(theBuilder);

//     // mark the domain as having been modified
//     this->domainChange();
//     return 0;
// }


int
ShadowSubdomain::getRemoteData(void)
{
    if (buildRemote == true && gotRemoteData == false)
    {
        msgData(0) = ShadowActorSubdomain_getRemoteData;
        this->sendID(msgData);

        this->recvID(msgData);
        numExternalNodes = msgData(0);
        numDOF = msgData(1);

        //cerr << "ShadowSubdomain::getRemoteData numExtNodes " << numExternalNodes << endln;
        if (theExternalNodes.Size() != numExternalNodes)
        {
            theExternalNodes[numExternalNodes - 1] = 0;    // this will resize
        }

        if (theExternalNodes.Size() != numExternalNodes)
        {
            cerr << "ShadowSubdomain::getRemoteData(void) - need to resize the ID\n";
            return -1;
        }

        if (numExternalNodes != 0)
        {
            this->recvID(theExternalNodes);
        }

        //cerr << "ShadowSubdomain::getREmoteData " << theExternalNodes;
    }

    gotRemoteData = true;
    return 0;
}

bool
ShadowSubdomain::addElement(Element *theEle)
{
    int tag = theEle->getTag();
    //cerr << "ShadowSubdomain::addElement(Element *theEle):-- Element External Nodes are " << theEle->getExternalNodes() << "\n";

#ifdef _BABAK_DEBUG
    cerr << "ShadowSubdomain::addElement(Element *theEle): -- Element Number # " << tag << "\n";
#endif

#ifdef _G3DEBUG
    // do all the checking stuff
#endif

    msgData(0) = ShadowActorSubdomain_addElement;
    msgData(1) = theEle->getClassTag();
    msgData(2) = theEle->getDbTag();
    this->sendID(msgData);
#ifdef _BABAK_DEBUG
    cerr << "ShadowSubdomain::addElement(Element *theEle): -- ID sent ... " << tag << "\n";
#endif
    this->sendObject(*theEle);
    theElements[numElements] = tag;
    numElements++;
    //    this->domainChange();

    /*
    msgData(0) = 5;
    msgData(1) = 6;
    msgData(2) = 7;
    msgData(3) = 8;
    this->sendID(msgData);
    this->recvID(msgData);
    cerr << "ShadowSubdomain::addElement() : " << msgData;
    */

    delete theEle;

    return true;
}

bool
ShadowSubdomain::addNode(Node *theNode)
{
#ifdef _BABAK_DEBUG
    cerr << *theNode << "\n"; //Added by Babak Kamrani for debugging purposes
#endif
    int tag = theNode->getTag();
#ifdef _G3DEBUG
    // do all the checking stuff
#endif
    msgData(0) = ShadowActorSubdomain_addNode;
    msgData(1) = theNode->getClassTag();
    msgData(2) = theNode->getDbTag();
    this->sendID(msgData);
    this->sendObject(*theNode);
    theNodes[numNodes] = tag;
    numNodes++;
    numDOF += theNode->getNumberDOF();
    //this->domainChange();

    delete theNode;

    return true;
}

bool
ShadowSubdomain::addExternalNode(Node *theNode)
{
    int tag = theNode->getTag();
#ifdef _G3DEBUG
    // do all the checking stuff
#endif

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

    //    this->domainChange();

    return true;
}

bool
ShadowSubdomain::addSP_Constraint(SP_Constraint *theSP)
{

#ifdef _G3DEBUG
    // do all the checking stuff
#endif
    msgData(0) = ShadowActorSubdomain_addSP_Constraint;
    msgData(1) = theSP->getClassTag();
    msgData(2) = theSP->getDbTag();
    this->sendID(msgData);
    this->sendObject(*theSP);
    numSPs++;
    // this->domainChange();

    //this->Subdomain::addSP_Constraint(theSP);
    return true;
}

bool
ShadowSubdomain::addMP_Constraint(MP_Constraint *theMP)
{
#ifdef _G3DEBUG
    // do all the checking stuff
#endif
    msgData(0) = ShadowActorSubdomain_addMP_Constraint;
    msgData(1) = theMP->getClassTag();
    msgData(2) = theMP->getDbTag();
    this->sendID(msgData);
    this->sendObject(*theMP);
    numMPs++;
    // // this->domainChange();

    //this->Subdomain::addMP_Constraint(theMP);
    return true;
}


bool
ShadowSubdomain::addLoadPattern(LoadPattern *thePattern)
{
#ifdef _G3DEBUG
    // do all the checking stuff
#endif
    msgData(0) = ShadowActorSubdomain_addLoadPattern;
    msgData(1) = thePattern->getClassTag();
    msgData(2) = thePattern->getDbTag();
    this->sendID(msgData);
#ifdef _BABAK_DEBUG
    //      cerr << "ShadowSubdomain::addLoadPattern(LoadPattern *thePattern)--BEFORE this->sendObject(*thePattern) -- DRM INFORMATION:\n";
    //      DRM_LOAD_PATTERN->Print(cerr);
#endif
    this->sendObject(*thePattern);
    //    this->domainChange();

    //this->Subdomain::addLoadPattern(thePattern);
    numLoadPatterns++;
    return true;
}



bool
ShadowSubdomain::addSP_Constraint(SP_Constraint *theSP, int loadPattern)
{
#ifdef _G3DEBUG
    // do all the checking stuff
#endif

    /*
    LoadPattern *thePattern = this->Subdomain::getLoadPattern(loadPattern);
    if ((thePattern == 0) || (thePattern->addSP_Constraint(theSP) == false)) {
      cerr << "ShadowSubdomain::addSP_Constraint() - could not add SP_Constraint: " << *theSP;
      return false;
    }
    */

    msgData(0) = ShadowActorSubdomain_addSP_ConstraintToPattern;
    msgData(1) = theSP->getClassTag();
    msgData(2) = theSP->getDbTag();
    msgData(3) = loadPattern;
    this->sendID(msgData);
    this->sendObject(*theSP);
    numSPs++;
    // this->domainChange();

    return true;
}

bool
ShadowSubdomain::addNodalLoad(NodalLoad *theLoad, int loadPattern)
{
#ifdef _G3DEBUG
    // do all the checking stuff
#endif

    /*
    LoadPattern *thePattern = this->Subdomain::getLoadPattern(loadPattern);
    if ((thePattern == 0) || (thePattern->addNodalLoad(theLoad) == false)) {
      cerr << "ShadowSubdomain::addNodalLoad() - could not add the load: " << *theLoad;
      return false;
    }
    cerr << "ShadowSubdomain::addNodalLoad : " << this->getTag() << " " << theLoad->getNodeTag() << endln;
    */

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
#ifdef _G3DEBUG
    // do all the checking stuff
#endif
    //Guanzhou out LoadPattern *thePattern = this->Subdomain::getLoadPattern(loadPattern);
    //Guanzhou out if ((thePattern == 0) || (thePattern->addElementalLoad(theLoad) == false)) {
    //Guanzhou out   cerr << "ShadowSubdomain::addElementalLoad() - could not add the load: " << *theLoad;
    //Guanzhou out   return false;
    //Guanzhou out }

    msgData(0) = ShadowActorSubdomain_addElementalLoadToPattern;
    msgData(1) = theLoad->getClassTag();
    msgData(2) = theLoad->getDbTag();
    msgData(3) = loadPattern;
    this->sendID(msgData);
    this->sendObject(*theLoad);

    delete theLoad;//GZ

    return true;
}


bool
ShadowSubdomain::hasNode(int tag)
{
    msgData(0) = ShadowActorSubdomain_hasNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->recvID(msgData);

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
    msgData(0) = ShadowActorSubdomain_hasInternalNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->recvID(msgData);

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
    msgData(0) = ShadowActorSubdomain_hasExternalNode;
    msgData(1) = tag;
    this->sendID(msgData);
    this->recvID(msgData);

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
    msgData(0) = ShadowActorSubdomain_hasElement;
    msgData(1) = tag;
    this->sendID(msgData);
    this->recvID(msgData);

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
        // this->domainChange();

        // get the element from the actor

        //Note, Guanzhou took out these unnecessary communications!!!
        //Guanzhou out this->recvID(msgData);
        //Guanzhou out int theType = msgData(0);
        //Guanzhou out
        //Guanzhou out if (theType == -1) // the ele was not there!
        //Guanzhou out     return 0;
        //Guanzhou out
        //Guanzhou out Element *theEle = theBroker->getNewElement(theType);
        //Guanzhou out if (theEle != 0)
        //Guanzhou out     this->recvObject(*theEle);
        //Guanzhou out
        //Guanzhou out return theEle;
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

        // this->domainChange();
        // remove from external as well
        loc = theExternalNodes.removeValue(tag);

        if (loc >= 0)
        {
            numExternalNodes--;
        }

        // receive the node from the actor
        this->recvID(msgData);
        //int theType = msgData(0);

        if (msgData(0) == 0)
        {
            numDOF -= msgData(1);
        }
        else
        {
            cerr << "ShadowSubdomain::removeNode() - failed to remove node from subdomain!\n";
            exit(1);
        }

        //Guanzhou out     return 0;
        //Guanzhou out
        //Guanzhou out Node *theNode = theBroker->getNewNode(theType);
        //Guanzhou out if (theNode != 0) {
        //Guanzhou out     this->recvObject(*theNode);
        //Guanzhou out     if (loc >= 0)
        //Guanzhou out  numDOF -= theNode->getNumberDOF();
        //Guanzhou out }
        //Guanzhou out return theNode;
    }
}

SP_Constraint *
ShadowSubdomain::removeSP_Constraint(int tag)
{
    //SP_Constraint *spPtr = this->Subdomain::removeSP_Constraint(tag);
    //if (spPtr == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeSP_Constraint;
    msgData(1) = tag;

    this->sendID(msgData);
    numSPs--;

    //return spPtr;
}

MP_Constraint *
ShadowSubdomain::removeMP_Constraint(int tag)
{
    //MP_Constraint *mpPtr = this->Subdomain::removeMP_Constraint(tag);
    //if (mpPtr == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeMP_Constraint;
    msgData(1) = tag;

    this->sendID(msgData);

    numMPs--;
    //return mpPtr;
}

LoadPattern *
ShadowSubdomain::removeLoadPattern(int loadTag)
{
    //LoadPattern *res = this->Subdomain::removeLoadPattern(loadTag);
    //if (res == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeLoadPattern;
    msgData(1) = loadTag;

    this->sendID(msgData);
    //return res;
}

NodalLoad *
ShadowSubdomain::removeNodalLoad(int loadTag, int loadPattern)
{
    //NodalLoad *res = this->Subdomain::removeNodalLoad(loadTag, loadPattern);
    //if (res == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeNodalLoadFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);
    //return res;
}



ElementalLoad *
ShadowSubdomain::removeElementalLoad(int loadTag, int loadPattern)
{
    //ElementalLoad *res = this->Subdomain::removeElementalLoad(loadTag, loadPattern);
    //if (res == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeElementalLoadFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);
    //return res;
}

SP_Constraint *
ShadowSubdomain::removeSP_Constraint(int loadTag, int loadPattern)
{
    //SP_Constraint *res = this->Subdomain::removeSP_Constraint(loadTag, loadPattern);
    //if (res == 0)
    //  return 0;

    msgData(0) = ShadowActorSubdomain_removeSP_ConstraintFromPattern;
    msgData(1) = loadTag;
    msgData(2) = loadPattern;

    this->sendID(msgData);
    //return res;
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

//Guanzhou added for PBLoading
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
    this->recvID(msgData);
    int theType = msgData(0);

    Element *theEle = theBroker->getNewElement(theType);

    if (theEle != 0)
    {
        this->recvObject(*theEle);
    }

    //cerr << "ShadowSubdomain::getElementPtr() ";
    //cerr << " - SHOULD BE AVOIDED IF POSSIBLE \n";

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
    this->recvID(msgData);
    int theType = msgData(0);

    Element *theEle = theBroker->getNewElement(theType);

    if (theEle != 0)
    {
        this->recvObject(*theEle);
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
    this->recvID(msgData);
    int theType = msgData(0);

    theNod = NULL;
    theNod = theBroker->getNewNode(theType);

    if (theNod != 0)
    {
        this->recvObject(*theNod);
    }

    //cerr << "ShadowSubdomain::getNodPtr() ";
    //cerr << " - SHOULD BE AVOIDED IF POSSIBLE \n";

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
    //this->recvID(msgData);
    //int theType = msgData(0);

    theNod = NULL;
    theNod = theBroker->getNewNode(NOD_TAG_Node);

    if (theNod != 0)
    {
        this->recvObject(*theNod);
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
    this->recvID(data);
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


// int
// ShadowSubdomain::addRecorder(Recorder &theRecorder)
// {
//     msgData(0) = ShadowActorSubdomain_addRecorder;
//     msgData(1) = theRecorder.getClassTag();
//     this->sendID(msgData);
//     this->sendObject(theRecorder);
//     return 0;
// }

// int
// ShadowSubdomain::removeRecorders(void)
// {
//     msgData(0) = ShadowActorSubdomain_removeRecorders;
//     this->sendID(msgData);
//     return 0;
// }

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

    //Guanzhou if (theVector == 0)
    //Guanzhou  theVector = new Vector(numDOF);
    //Guanzhouelse if (theVector->Size() != numDOF) {
    //Guanzhou  delete theVector;
    //Guanzhou  theVector = new Vector(numDOF);
    //Guanzhou}
    //Guanzhou
    //Guanzhouif (theMatrix == 0)
    //Guanzhou  theMatrix = new Matrix(numDOF,numDOF);
    //Guanzhouelse if (theMatrix->noRows() != numDOF) {
    //Guanzhou  delete theMatrix;
    //Guanzhou  theMatrix = new Matrix(numDOF,numDOF);
    //Guanzhou}
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
    //Guanzhou// if the subdoamin was built remotly need to get it's data
    //Guanzhouif (gotRemoteData == false && buildRemote == true)
    //Guanzhou  this->getRemoteData();
    //Guanzhou
    //Guanzhou  msgData(0) =  ShadowActorSubdomain_getTang;
    //Guanzhou  this->sendID(msgData);
    //Guanzhou
    //Guanzhou  if (theMatrix == 0)
    //Guanzhou    theMatrix = new Matrix(numDOF,numDOF);
    //Guanzhou  else if (theMatrix->noRows() != numDOF) {
    //Guanzhou    delete theMatrix;
    //Guanzhou    theMatrix = new Matrix(numDOF,numDOF);
    //Guanzhou  }
    //Guanzhou
    //Guanzhou  this->recvMatrix(*theMatrix);
    //Guanzhou  return *theMatrix;
}



const Vector &
ShadowSubdomain::getResistingForce(void)
{
    cerr << "ShadowSubdomain::getResistingForce(void) should never be called!\n";
    // if the subdoamin was built remotly need to get it's data
    //Guanzhouif (gotRemoteData == false && buildRemote == true)
    //Guanzhou  this->getRemoteData();
    //Guanzhou
    //Guanzhou  msgData(0) = ShadowActorSubdomain_getResistingForce;
    //Guanzhou  this->sendID(msgData);
    //Guanzhou
    //Guanzhou  if (theVector == 0)
    //Guanzhou    theVector = new Vector(numDOF);
    //Guanzhou  else if (theVector->Size() != numDOF) {
    //Guanzhou    delete theVector;
    //Guanzhou    theVector = new Vector(numDOF);
    //Guanzhou  }
    //Guanzhou
    //Guanzhou  this->recvVector(*theVector);
    //Guanzhou  return *theVector;
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
    /*
      msgData(0) = ShadowActorSubdomain_getCost;

      this->sendID(msgData);
      Vector cost(2);
      this->recvVector(cost);
      return cost(0);
    */
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
ShadowSubdomain::recvSelf(int cTag, Channel &the_Channel,
                          FEM_ObjectBroker &the_Broker)
{
    cerr << "ShadowSubdomain::recvSelf() ";
    cerr << " - NOT YET IMPLEMENTED\n";
    return -1;
}


void
ShadowSubdomain::Print(ostream &s, int flag)
{
    msgData(0) = ShadowActorSubdomain_Print;

    this->sendID(msgData);
    this->recvID(msgData);
}


int
//GZ ShadowSubdomain::buildEleGraph(Graph *theEleGraph)
ShadowSubdomain::buildEleGraph()
{
    //GZ cerr << "ShadowSubdomain::buildEleGraph() ";
    //GZ cerr << " - NOT YET IMPLEMENTED\n";
    //GZ return -1;
# ifdef _PDD
    msgData(0) = ShadowActorSubdomain_BuildElementGraph;
    this->sendID(msgData);
# endif
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


#ifdef _PDD
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
        this->recvID(msgData);
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
                    this->recvObject(*theSP);
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
                    this->recvObject(*theNodalLoad);
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
                    this->recvObject(*theSP);
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
        this->recvID(msgData);
        int theType = msgData(1);
        int dbTag = msgData(2);
        //if ( theNod != 0 ) delete theNod;
        theNod = theBroker->getNewNode(theType);

        if (theNod != 0)
        {
            theNod->setDbTag(dbTag);
            this->recvObject(*theNod);
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

// int
// ShadowSubdomain::resetRecorders(void)
// {
//     msgData(0) = ShadowActorSubdomain_resetRecorders;
//     this->sendID(msgData);
//     return 0;
// }

int
ShadowSubdomain::addNodeTag(int nodeTag, int dof)
{
    theNodes[numNodes] = nodeTag;
    numDOF += dof;
    numNodes++;

    return 0;
}

#endif

#ifdef _PARALLEL_PROCESSING
//Guanzhou added this func
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

    this->recvID(msgData);//get the node back in master
    int theType = msgData(1);
    int dbTag = msgData(2);
    //if ( theNod != 0 ) delete theNod;
    theNod = theBroker->getNewNode(theType);

    if (theNod != 0)
    {
        theNod->setDbTag(dbTag);
        this->recvObject(*theNod);
        //theDomain.recvNode(theNod);
        theDomain.addNode(theNod);
        int tag = theNod->getTag();
        //theNodes[numNodes] = tag;
        //delete theNod;
        theExternalNodes[numExternalNodes] = tag;
        //numNodes++;
        numExternalNodes++;
        //numDOF += theNod->getNumberDOF();
    }

    return 0;
}


#endif


//Added by Babak Kamrani for debugging purposes
////////////////////////////////////////////////
// bool
// ShadowSubdomain::addLoadPattern_Fake(LoadPattern *thePattern)
// {
// #ifdef _G3DEBUG
//  // do all the checking stuff
// #endif
//     msgData(0) = ShadowActorSubdomain_addLoadPattern;
//     msgData(1) = thePattern->getClassTag();
//     msgData(2) = thePattern->getDbTag();
//     this->sendID(msgData);
//     this->sendObject(*thePattern);
//     //    this->domainChange();
//
//     //this->Subdomain::addLoadPattern(thePattern);
//     numLoadPatterns++;
//     return true;
// }

//////////////////////////////////////////////////


