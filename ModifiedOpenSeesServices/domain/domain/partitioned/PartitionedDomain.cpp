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
// $Date: 2003/02/14 23:00:56 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/domain/partitioned/PartitionedDomain.cpp,v $


// Written: fmk
// Revision: A
//
// Description: This file contains the class definition for PartitionedDomain.
// PartitionedDomain is an abstract class. The class is responsible for holding
// and providing access to the Elements, Nodes, LoadCases, SP_Constraints
// and MP_Constraints just like a normal domain. In addition the domain provides
// a method to partition the domain into Subdomains.
//
// ModelBuilder. There are no partitions in a PartitionedDomain.
//
// What: "@(#) PartitionedDomain.C, revA"

#include <PartitionedDomain.h>
#include <stdlib.h>
#include <MultiSupportPattern.h>
#include <GroundMotion.h>
#include <ImposedMotionSP.h>
#include <SP_ConstraintIter.h>

#include <DomainPartitioner.h>
#include <Element.h>
#include <Node.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <ArrayOfTaggedObjects.h>
#include <ArrayOfTaggedObjectsIter.h>
#include <Subdomain.h>
#include <DomainPartitioner.h>
#include <PartitionedDomain.h>
#include <PartitionedDomainEleIter.h>
#include <PartitionedDomainSubIter.h>
#include <SingleDomEleIter.h>
#include <Vertex.h>
#include <Graph.h>
#include <LoadPattern.h>
#include <NodalLoad.h>
#include <ElementalLoad.h>
#include <SP_Constraint.h>
//#include <Recorder.h>

#include <iostream>
using namespace std;


PartitionedDomain::PartitionedDomain()
    : Domain(),
      theSubdomains(0), theDomainPartitioner(0),
      theSubdomainIter(0), mySubdomainGraph(0), have_populated_static_mesh_data(false)
{
    elements = new ArrayOfTaggedObjects(1024);
    theSubdomains = new ArrayOfTaggedObjects(32);
    theSubdomainIter = new PartitionedDomainSubIter(theSubdomains);

    mainEleIter = new SingleDomEleIter(elements);
    theEleIter = new PartitionedDomainEleIter(this);

    numberofDomainElements = 0; //Added by Babak to store the number domain elements 2/25/13
    Element_Node_Number_List = new int[1]; //Added by Babak to store the number of nodes for each element 2/25/13

    if (theSubdomains == 0 || elements == 0 ||
            theSubdomainIter == 0 ||
            theEleIter == 0 || mainEleIter == 0)
    {

        cerr << "FATAL: PartitionedDomain::PartitionedDomain ";
        cerr << "  - ran out of memory\n";
        exit(-1);
    }
}


PartitionedDomain::PartitionedDomain(DomainPartitioner &thePartitioner)
    : Domain(),
      theSubdomains(0), theDomainPartitioner(&thePartitioner),
      theSubdomainIter(0), mySubdomainGraph(0), have_populated_static_mesh_data(false)
{
    elements = new ArrayOfTaggedObjects(1024);
    theSubdomains = new ArrayOfTaggedObjects(32);
    theSubdomainIter = new PartitionedDomainSubIter(theSubdomains);

    mainEleIter = new SingleDomEleIter(elements);
    theEleIter = new PartitionedDomainEleIter(this);

    numberofDomainElements = 0; //Added by Babak to store the number domain elements 2/25/13
    Element_Node_Number_List = new int[1]; //Added by Babak to store the number of nodes for each element 2/25/13

    if (theSubdomains == 0 || elements == 0 ||
            theSubdomainIter == 0 || theDomainPartitioner == 0 ||
            theEleIter == 0 || mainEleIter == 0)
    {

        cerr << "FATAL: PartitionedDomain::PartitionedDomain ";
        cerr << "  - ran out of memory\n";
        exit(-1);
    }
}


PartitionedDomain::PartitionedDomain(int numNodes, int numElements,
                                     int numSPs, int numMPs, int numLoadPatterns,
                                     int numUniaxialMat, int numNDMaterial, int numSections, //Added by Babak on December 2012
                                     int numofSectionRepresents, int num3dGeomTransfs,//Added by Babak on December 2012
                                     int nummultipleexcitation, int numBodyForces, int numDamping,//Added by Babak on December 2012
                                     int numSubdomains,
                                     DomainPartitioner &thePartitioner)

    : Domain(numNodes, 0, numSPs, numMPs, numLoadPatterns,
             numUniaxialMat, numNDMaterial, numSections,
             numofSectionRepresents,
             nummultipleexcitation, numBodyForces, numDamping),
    theSubdomains(0), theDomainPartitioner(&thePartitioner),
    theSubdomainIter(0), mySubdomainGraph(0), have_populated_static_mesh_data(false)
{
    elements = new ArrayOfTaggedObjects(numElements);
    theSubdomains = new ArrayOfTaggedObjects(numSubdomains);
    theSubdomainIter = new PartitionedDomainSubIter(theSubdomains);

    mainEleIter = new SingleDomEleIter(elements);
    theEleIter = new PartitionedDomainEleIter(this);
    numberofDomainElements = 0; //Added by Babak to store the number domain elements 2/25/13
    Element_Node_Number_List = new int[1]; //Added by Babak to store the number of nodes for each element 2/25/13

    if (theSubdomains == 0 || elements == 0 ||
            theSubdomainIter == 0 ||
            theEleIter == 0 || mainEleIter == 0)
    {

        cerr << "FATAL: PartitionedDomain::PartitionedDomain(int ..) ";
        cerr << "  - ran out of memory\n";
        exit(-1);
    }
}




PartitionedDomain::~PartitionedDomain()
{
    if (elements != 0)
    {
        delete elements;
    }

    if (theSubdomains != 0)
    {
        delete theSubdomains;
    }

    if (theSubdomainIter != 0)
    {
        delete theSubdomainIter;
    }

    if (theEleIter != 0)
    {
        delete theEleIter;
    }

    if (Element_Node_Number_List != 0)
    {
        delete Element_Node_Number_List;
    }
}

void
PartitionedDomain::clearAll(void)
{
    this->Domain::clearAll();

    elements->clearAll();
    theSubdomains->clearAll();
    this->Domain::theOutputWriter.finalize();
}



bool
PartitionedDomain::addElement(Element *elePtr)
{

    if (elePtr->isSubdomain() == true)
    {
        return this->addSubdomain((Subdomain *)elePtr);
    }

    int eleTag = elePtr->getTag();
#ifdef _DEBUG

    if (check == true)
    {

        // check ele Tag >= 0
        if (eleTag < 0)
        {
            cerr << "PartitionedDomain::addElement - Element " << eleTag;
            cerr << " tag must be >= 0\n";
            return false;
        }

        // check its not in this or any of the subdomains
        // MISSING CODE

        // check all the elements nodes exist in the domain
        const ID &nodes = elePtr->getExternalNodes();

        for (int i = 0; i < nodes.Size(); i++)
        {
            int nodeTag = nodes(i);
            Node *nodePtr = this->Domain::getNode(nodeTag);

            if (nodePtr == 0)
            {
                cerr << "PartitionedDomain::addElement - In element " << eleTag;
                cerr << " no node " << nodeTag << " exists in the domain\n";
                return false;
            }
        }

    }

#endif

    TaggedObject *other = elements->getComponentPtr(eleTag);

    if (other != 0)
    {
        return false;
    }

    bool result = elements->addComponent(elePtr);

    if (result == true)
    {
        elePtr->setDomain(this);
        this->domainChange();
        //Added by babak 2/25/13
        //----
        //expanding the allocation of the memory for the Element_Node_Number_List
        int *temp = new int[max(numberofDomainElements, eleTag) + 1];
        std::copy(Element_Node_Number_List, Element_Node_Number_List + numberofDomainElements, temp);
        delete [] Element_Node_Number_List;
        temp[eleTag - 1] = elePtr->getNumExternalNodes();
        Element_Node_Number_List = temp;
        numberofDomainElements++;
        //----
    }

    return result;
}



bool
PartitionedDomain::addNode(Node *nodePtr)
{
    return (this->Domain::addNode(nodePtr));
}


bool
PartitionedDomain::addLoadPattern(LoadPattern *loadPattern)
{
    bool result = true;

    int tag = loadPattern->getTag();

    if (this->getLoadPattern(tag) != 0)
    {
        cerr << "PartitionedDomain::addLoadPattern - cannot add as LoadPattern with tag" <<
             tag << "already exists in model\n";
        return false;
    }

    this->Domain::addLoadPattern(loadPattern);
    this->domainChange();

    SubdomainIter &theSubdomains = this->getSubdomains();
    Subdomain *theSub;

    while ((theSub = theSubdomains()) != 0)
    {
        bool res = theSub->addLoadPattern(loadPattern);
        theSub->domainChange();

        if (res != true)
        {
            cerr << "PartitionedDomain::addLoadPattern - cannot add as LoadPattern with tag: " <<
                 tag << " to subdomain\n";
            result = res;
        }
    }

    return result;
}

#ifdef _PARALLEL_PROCESSING
//Guanzhou added this func
int
PartitionedDomain::resetMultipleSupport(const int loadPatternTag)
{
    MultiSupportPattern *theLP = (MultiSupportPattern *)this->getLoadPattern(loadPatternTag);
    SP_ConstraintIter &theIter = theLP->getSPs();

    ImposedMotionSP *sp;

    while ((sp = (ImposedMotionSP *)theIter()) != 0)
    {
        int nodeTag = sp->getNodeTag();
        int myTag = sp->getTag();

        if ( this->Domain::getNode(nodeTag) == 0 )
        {
            theLP->removeSP_Constraint(myTag);
            delete sp;
        }
    }

    SubdomainIter &theSubdomains = this->getSubdomains();
    Subdomain *theSub;

    while ((theSub = theSubdomains()) != 0)
    {
        theSub->resetSubMultipleSupport(loadPatternTag);
    }


    return 0;

}

#endif

bool
PartitionedDomain::addSP_Constraint(SP_Constraint *load)
{
    int nodeTag = load->getNodeTag();

    // check the Node exists in the Domain or one of Subdomains

    // if in Domain add it as external .. ignore Subdomains
    Node *nodePtr = this->Domain::getNode(nodeTag);

    if (nodePtr != 0)
    {
        return (this->Domain::addSP_Constraint(load));
    }

    // find subdomain with node and add it .. break if find as internal node
    SubdomainIter &theSubdomains = this->getSubdomains();
    Subdomain *theSub;

    while ((theSub = theSubdomains()) != 0)
    {
        bool res = theSub->hasNode(nodeTag);

        if (res == true)
        {
            bool res = theSub->addSP_Constraint(load);
            delete load;
            return res;
        }
    }


    // if no subdomain .. node not in model .. error message and return failure
    cerr << "PartitionedDomain::addSP_Constraint - cannot add as node with tag" <<
         nodeTag << "does not exist in model\n";

    return false;

}

bool
PartitionedDomain::addSP_Constraint(SP_Constraint *load, int pattern)
{
    int nodeTag = load->getNodeTag();

    // check the Node exists in the Domain or one of Subdomains

    // if in Domain add it as external .. ignore Subdomains
    Node *nodePtr = this->Domain::getNode(nodeTag);

    if (nodePtr != 0)
    {
        return (this->Domain::addSP_Constraint(load, pattern));
    }

    // find subdomain with node and add it .. break if find as internal node
    SubdomainIter &theSubdomains = this->getSubdomains();
    Subdomain *theSub;

    while ((theSub = theSubdomains()) != 0)
    {
        bool res = theSub->hasNode(nodeTag);

        if (res == true)
        {
            return theSub->addSP_Constraint(load, pattern);
        }
    }


    // if no subdomain .. node not in model .. error message and return failure
    cerr << "PartitionedDomain::addSP_Constraint - cannot add as node with tag" <<
         nodeTag << "does not exist in model\n";

    return false;

}

bool
PartitionedDomain::addNodalLoad(NodalLoad *load, int pattern)
{
    int nodeTag = load->getNodeTag();

    // check the Node exists in the Domain or one of Subdomains

    // if in Domain add it as external .. ignore Subdomains
    Node *nodePtr = this->Domain::getNode(nodeTag);

    if (nodePtr != 0)
    {
        //    cerr << "HI " << *nodePtr;
        // cerr << "PartitionedDomain::addLoadPattern(LoadPattern *loadPattern) MAIN " << *load;

        return (this->Domain::addNodalLoad(load, pattern));
    }


    // find subdomain with node and add it .. break if find as internal node
    SubdomainIter &theSubdomains = this->getSubdomains();
    Subdomain *theSub;

    while ((theSub = theSubdomains()) != 0)
    {
        bool res = theSub->hasNode(nodeTag);

        if (res == true)
        {
            // cerr << "PartitionedDomain::addLoadPattern(LoadPattern *loadPattern) SUB " << theSub->getTag() << *load;
            return theSub->addNodalLoad(load, pattern);
        }
    }

    // if no subdomain .. node not in model
    cerr << "PartitionedDomain::addNodalLoad - cannot add as node with tag" <<
         nodeTag << "does not exist in model\n";
    return false;
}


bool
PartitionedDomain::addElementalLoad(ElementalLoad *load, int pattern)
{
    //Guanzhou out cerr << "PartitionedDomain::addElementalLoad - not yet implemented\n";
    //Guanzhou out return false;
    return this->Domain::addElementalLoad(load, pattern);

}

//Guanzhou added for parallel processing
Element *
PartitionedDomain::removeElement(int tag)
{
    // we first see if its in the original domain
    TaggedObject *res = elements->removeComponent(tag);
    Element *result = 0;

    if (res != 0)
    {
        result = (Element *)res;
        this->domainChange();
        return result;
    }

    // if not there we must check all the other subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            result = theSub->removeElement(tag);

            if (result != 0)
            {
                return result;
            }
        }
    }

    // its not there
    return 0;
}


//Guanzhou added for parallel processing
Node *
PartitionedDomain::removeNode(int tag)
{
    // we first remove it form the original domain (in case on boundary)
    Node *result = this->Domain::removeNode(tag);

    // we must also try removing from the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;

            if ( theSub->hasNode(tag) )
            {
                theSub->removeNode(tag);
            }
        }
    }

    if (result != 0)
    {
        this->domainChange();
    }

    return result;
}


//Guanzhou added for parallel processing
LoadPattern *
PartitionedDomain::removeLoadPattern(int tag)
{
    LoadPattern *result = this->Domain::removeLoadPattern(tag);

    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->removeLoadPattern(tag);
        }
    }

    // its not there
    return result;
}


//Guanzhou added for parallel processing
SP_Constraint *
PartitionedDomain::removeSP_Constraint(int tag)
{
    // we first see if its in the original domain
    SP_Constraint *result = this->Domain::removeSP_Constraint(tag);

    if (result != 0)
    {
        this->domainChange();
        return result;
    }


    // if not there we must check all the other subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            result = theSub->removeSP_Constraint(tag);

            if (result != 0)
            {
                return result;
            }
        }
    }

    // its not there
    return 0;
}


MP_Constraint *
PartitionedDomain::removeMP_Constraint(int tag)
{
    // we first see if its in the original domain
    MP_Constraint *result = this->Domain::removeMP_Constraint(tag);

    if (result != 0)
    {
        this->domainChange();
        return result;
    }

    // if not there we must check all the other subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            result = theSub->removeMP_Constraint(tag);

            if (result != 0)
            {
                return result;
            }
        }
    }

    // its not there
    return 0;
}


bool//Guanzhou added
PartitionedDomain::addMP_Constraint(MP_Constraint *mpConstraint)
{
    // perform the checks
    int nodeConstrained = mpConstraint->getNodeConstrained();
    Node *nodePtr = this->Domain::getNode(nodeConstrained);

    if (nodePtr == 0)
    {
        SubdomainIter &theSubdomains = this->getSubdomains();
        Subdomain *theSub;

        while ((theSub = theSubdomains()) != 0)
        {
            bool res = theSub->hasInternalNode(nodeConstrained);

            if (res == true)
            {
                theSub->swapNodeFromInternalToExternal(nodeConstrained);
                break;
            }
        }
    }

    int nodeRetained = mpConstraint->getNodeRetained();
    nodePtr = this->Domain::getNode(nodeRetained);

    if (nodePtr == 0)
    {
        SubdomainIter &theSubdomains = this->getSubdomains();
        Subdomain *theSub;

        while ((theSub = theSubdomains()) != 0)
        {
            bool res = theSub->hasInternalNode(nodeRetained);

            if (res == true)
            {
                theSub->swapNodeFromInternalToExternal(nodeRetained);
                break;
            }
        }
    }

    bool res = this->Domain::addMP_Constraint(mpConstraint);
    return res;

}

// public member functions which have to be modified
ElementIter &
PartitionedDomain::getElements()
{
    theEleIter->reset();
    return *theEleIter;
}


Element *
PartitionedDomain::getElement(int tag)
{
    // we first see if its in the original domain
    TaggedObject *res = elements->getComponentPtr(tag);
    Element *result = 0;

    if (res != 0)
    {
        result = (Element *)res;
        return result;
    }

# ifdef _PDD
# else

    // go through the other subdomains until we find it or we run out of subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            result = theSub->getElement(tag);

            if (result != 0)
            {
                return result;
            }
        }
    }

    // its not there
# endif
    return 0;
}

//Guanzhou added
Node *
PartitionedDomain::getNode(int tag)
{
    Node *result = this->Domain::getNode(tag);

    if ( result != 0 )
    {
        return result;
    }

    //# ifdef _PDD
    //# else
    //    // go through the other subdomains until we find it or we run out of subdomains
    //    if (theSubdomains != 0) {
    //  ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
    //  TaggedObject *theObject;
    //  while ((theObject = theSubsIter()) != 0) {
    //      Subdomain *theSub = (Subdomain *)theObject;
    //      result = theSub->getNode(tag);
    //      if (result != 0)
    //      return result;
    //  }
    //    }
    //
    //    // its not there
    //# endif
    return 0;

}

//Guanzhou added
Node *
PartitionedDomain::getOutsideNode(int tag)
{
    Node *result = this->Domain::getNode(tag);

    if ( result != 0 )
    {
        return result;
    }

    // go through the other subdomains until we find it or we run out of subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            result = theSub->getNode(tag);

            if (result != 0)
            {
                return result;
            }
        }
    }

    // its not there
    return 0;

}

int
PartitionedDomain::getNumElements(void) const
{
    int result = elements->getNumComponents();

    // add the number of subdomains
    result +=  theSubdomains->getNumComponents();
    return result;
}

void
PartitionedDomain::applyLoad(double timeStep)
{
    this->Domain::applyLoad(timeStep);

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->applyLoad(timeStep);
        }
    }
}


void
PartitionedDomain::setCommitTag(int newTag)
{
    this->Domain::setCommitTag(newTag);

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->setCommitTag(newTag);
        }
    }
}



void
PartitionedDomain::setCurrentTime(double newTime)
{
    this->Domain::setCurrentTime(newTime);

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->setCurrentTime(newTime);
        }
    }
}


void
PartitionedDomain::setCommittedTime(double newTime)
{
    this->Domain::setCommittedTime(newTime);

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->setCommittedTime(newTime);
        }
    }
}


void
PartitionedDomain::setLoadConstant(void)
{
    this->Domain::setLoadConstant();

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->setLoadConstant();
        }
    }
}

int
PartitionedDomain::update(void)
{
    int res = this->Domain::update();

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->computeNodalResponse();
            theSub->update();
        }
    }

#ifdef _PARALLEL_PROCESSING
    return this->barrierCheck(res);
#endif
    return 0;
}




#ifdef _PARALLEL_PROCESSING
int
PartitionedDomain::barrierCheck(int res)
{
    int result = res;

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            int subResult = theSub->barrierCheckIN();

            if (subResult != 0)
            {
                result = subResult;
            }
        }

        ArrayOfTaggedObjectsIter theSubsIter1(*theSubdomains);

        while ((theObject = theSubsIter1()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->barrierCheckOUT(result);
        }
    }

    return result;
}
#endif

int
PartitionedDomain::update(double newTime, double dT)
{
    this->applyLoad(newTime);
    int res = 0;//this->Domain::update();

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            theSub->computeNodalResponse();
            theSub->update(newTime, dT);
        }
    }

    // #ifdef _PARALLEL_PROCESSING
    //     return this->barrierCheck(res);
    // #endif
    return 0;

    /*

    cerr << "PartitionedDomain::update(double newTime, double dT) -1\n";
    int result = 0;


    cerr << "PartitionedDomain::update(double newTime, double dT) -2\n";
    this->update();
    cerr << "PartitionedDomain::update(double newTime, double dT) -2a\n";

    // do the same for all the subdomains
    if (theSubdomains != 0) {
      ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
      TaggedObject *theObject;
      while ((theObject = theSubsIter()) != 0) {
        Subdomain *theSub = (Subdomain *)theObject;
        theSub->update(newTime, dT);
      }
      this->barrierCheck(result);
    }
    cerr << "PartitionedDomain::update(double newTime, double dT) -3\n";
    return result;

    */

}



int
PartitionedDomain::newStep(double dT)
{
    this->Domain::newStep(dT);

    int res = 0;

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            res += theSub->newStep(dT);

            if (res != 0)
            {
                cerr << "PartitionedDomain::step - subdomain " << theSub->getTag() << " failed in step\n";
            }
        }
    }

    return res;
}



int
PartitionedDomain::commit(void)
{
    int result = this->Domain::commit();

    if (result < 0)
    {
        cerr << "PartitionedDomain::commit(void) - failed in Domain::commit()\n";
        return result;
    }

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            int res = theSub->commit();

            if (res < 0)
            {
                cerr << "PartitionedDomain::commit(void)";
                cerr << " - failed in Subdomain::commit()\n";
                return res;
            }
        }
    }

    // now we load balance if we have subdomains and a partitioner
    //Guanzhou out int numSubdomains = this->getNumSubdomains();
    //Guanzhou out if (numSubdomains != 0 && theDomainPartitioner != 0)  {
    //Guanzhou out   Graph &theSubGraphs = this->getSubdomainGraph();
    //Guanzhou out   theDomainPartitioner->balance(theSubGraphs);
    //Guanzhou out }

    return 0;
}


int
PartitionedDomain::revertToLastCommit(void)
{
    int result = this->Domain::revertToLastCommit();

    if (result < 0)
    {
        cerr << "PartitionedDomain::revertToLastCommit(void) - failed in Domain::revertToLastCommit()\n";
        return result;
    }

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            int res = theSub->revertToLastCommit();

            if (res < 0)
            {
                cerr << "PartitionedDomain::revertToLastCommit(void)";
                cerr << " - failed in Subdomain::revertToLastCommit()\n";
                return res;
            }
        }
    }

    return 0;
}

int
PartitionedDomain::revertToStart(void)
{
    int result = this->Domain::revertToStart();

    if (result < 0)
    {
        cerr << "PartitionedDomain::revertToLastCommit(void) - failed in Domain::revertToLastCommit()\n";
        return result;
    }

    // do the same for all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            Subdomain *theSub = (Subdomain *)theObject;
            int res = theSub->revertToStart();

            if (res < 0)
            {
                cerr << "PartitionedDomain::revertToLastCommit(void)";
                cerr << " - failed in Subdomain::revertToLastCommit()\n";
                return res;
            }
        }
    }

    return 0;
}


// int
// PartitionedDomain::addRecorder(Recorder& theRecorder)
// {
//     if (this->Domain::addRecorder(theRecorder) < 0)
//     {
//         return -1;
//     }

//     // do the same for all the subdomains
//     if (theSubdomains != 0)
//     {
//         ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
//         TaggedObject* theObject;

//         while ((theObject = theSubsIter()) != 0)
//         {
//             Subdomain* theSub = (Subdomain*)theObject;
//             int res = theSub->addRecorder(theRecorder);

//             if (res < 0)
//             {
//                 cerr << "PartitionedDomain::revertToLastCommit(void)";
//                 cerr << " - failed in Subdomain::revertToLastCommit()\n";
//                 return res;
//             }
//         }
//     }

//     return 0;
// }

// int
// PartitionedDomain::removeRecorders(void)
// {
//     if (this->Domain::removeRecorders() < 0)
//     {
//         return -1;
//     }

//     // do the same for all the subdomains
//     if (theSubdomains != 0)
//     {
//         ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
//         TaggedObject* theObject;

//         while ((theObject = theSubsIter()) != 0)
//         {
//             Subdomain* theSub = (Subdomain*)theObject;
//             int res = theSub->removeRecorders();

//             if (res < 0)
//             {
//                 cerr << "PartitionedDomain::revertToLastCommit(void)";
//                 cerr << " - failed in Subdomain::revertToLastCommit()\n";
//                 return res;
//             }
//         }
//     }

//     return 0;
// }

void
PartitionedDomain::Print(ostream &s, int flag)
{
    this->Domain::Print(s, flag);

    s << "\nELEMENT DATA: NumEle: " << elements->getNumComponents() << "\n";
    //elements->Print(s);

    // print all the subdomains
    if (theSubdomains != 0)
    {
        ArrayOfTaggedObjectsIter theSubsIter(*theSubdomains);
        TaggedObject *theObject;

        while ((theObject = theSubsIter()) != 0)
        {
            theObject->Print(s, flag);
        }
    }
}


int
PartitionedDomain::setPartitioner(DomainPartitioner *thePartitioner)
{
    theDomainPartitioner = thePartitioner;
    return 0;
}


int
PartitionedDomain::partition(int numPartitions)
{
    //Jose and Babak added for HDF5 output
    //
    // NOTE: This is done in Domain::Commit in the case of sequential processing.
    //
    //This outputs the mesh information to the HDF5 writer. This is important because it builds the
    // arrays of indexes into node output data and element output data.

    cout << "\n\nPartitionedDomain::partition(" << numPartitions << "); \n";

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();
    Element *elePtr;
    ElementIter &theElemIter = this->getElements();


    if (output_is_enabled)
    {
        // theHDF5_Channel.setTime(currentTime);/
        // theOutputWriter.setTime(currentTime);

        //Write out static mesh data once!
        if (!have_populated_static_mesh_data)
        {
            //Write Node Mesh data!
            theOutputWriter.writeNumberOfNodes(this->getNumNodes());
            while ( ( nodePtr = theNodeIter() ) != 0 )
            {
                theOutputWriter.writeNodeMeshData(nodePtr->getTag(), nodePtr->getCrds(), nodePtr->getNumberDOF());
            }

            //Write Element Mesh data!
            theOutputWriter.writeNumberOfElements(this->getNumElements());
            while ( ( elePtr = theElemIter() ) != 0 )
            {
                int eleTag           = elePtr->getTag();
                std::string eleName  = elePtr->getElementName();
                ID eleConnectivity   = elePtr->getExternalNodes();
                int eleMaterialTag   = 0;       // To be implemented somehow
                Matrix eleGaussCoord = elePtr->getGaussCoordinates();
                int eleOutputSize    = elePtr->getOutputSize();
                if ( eleName.compare("Element") != 0  )  // If the type of the element is the base class, then dont add it!
                {
                    theOutputWriter.writeElementMeshData( eleTag,
                                                          eleName,
                                                          eleConnectivity,
                                                          eleMaterialTag ,
                                                          eleGaussCoord,
                                                          eleOutputSize);
                }
                ;
            }
        }
        have_populated_static_mesh_data = true;
    }


    // need to create element graph before create new subdomains
    // DO NOT REMOVE THIS LINE __ EVEN IF COMPILER WARNING ABOUT UNUSED VARIABLE
    Graph *theEleGraph = this->getElementGraph();

    // check to see that they have ones with the correct tags
    if (theSubdomains != 0)
    {
        for (int i = 1; i <= numPartitions; i++)
        {
            TaggedObject *theObject = theSubdomains->getComponentPtr(i);

            if (theObject == 0)   // create a subdomain with appropriate tag
            {
                Subdomain *theSubdomain = new Subdomain(i);
                this->addSubdomain(theSubdomain);
            }
        }
    }

    //now we call partition on the domainPartitioner which does the partitioning
    DomainPartitioner *thePartitioner = this->getPartitioner();

    if (thePartitioner != 0)
    {
        thePartitioner->setPartitionedDomain(*this);
        thePartitioner->partition(numPartitions);
    }
    else
    {
        cerr << "PartitionedDomain::partition(int numPartitions) - no associated partitioner\n";
        return -1;
    }

    cout << "PartitionedDomain::partition()   -- END \n";

    return 0;

}



# ifdef _PDD //Guanzhou added for PDD
int
PartitionedDomain::repartition(int numPartitions)
{

    if (theSubdomains != 0)
    {
        for (int i = 1; i <= numPartitions; i++)
        {
            TaggedObject *theObject = theSubdomains->getComponentPtr(i);

            if (theObject == 0)
            {
                cerr << "PartitionedDomain::repartition, can NOT retrieve subdomains!!!\n";
                return -1;
            }
        }
    }

    // now we call partition on the domainPartitioner which does the partitioning
    DomainPartitioner *thePartitioner = this->getPartitioner();

    if (thePartitioner != 0)
    {
        thePartitioner->setPartitionedDomain(*this);
        return thePartitioner->repartition(numPartitions);
    }
    else
    {
        cerr << "PartitionedDomain::repartition - no associated partitioner\n";
        return -1;
    }
}
# endif


bool
PartitionedDomain::addSubdomain(Subdomain *theSubdomain)
{
    int eleTag = theSubdomain->getTag();
    TaggedObject *other = theSubdomains->getComponentPtr(eleTag);

    if (other != 0)
    {
        return false;
    }

    bool result = theSubdomains->addComponent(theSubdomain);

    if (result == true)
    {
        theSubdomain->setDomain(this);
        this->domainChange();
    }

    return result;

}

int
PartitionedDomain::getNumSubdomains(void)
{
    return theSubdomains->getNumComponents();
}

Subdomain *
PartitionedDomain::getSubdomainPtr(int tag)
{
    TaggedObject *mc = theSubdomains->getComponentPtr(tag);

    if (mc == 0)
    {
        return 0;
    }

    Subdomain *result = (Subdomain *)mc;
    return result;
}

SubdomainIter &
PartitionedDomain::getSubdomains(void)
{
    theSubdomainIter->reset();
    return *theSubdomainIter;
}



DomainPartitioner *
PartitionedDomain::getPartitioner(void) const
{
    return theDomainPartitioner;
}




int
PartitionedDomain::buildEleGraph(Graph *theEleGraph)
{
    int numVertex = elements->getNumComponents();

    // see if quick return

    if (numVertex == 0)
    {
        return 0;
    }

    // create another vertices array which aids in adding edges

    Element *elePtr;

    int *theElementTagVertices = 0;
    int maxEleNum = 0;

    TaggedObject *tagdObjPtr;
    TaggedObjectIter &theEles = elements->getComponents();

    while ((tagdObjPtr = theEles()) != 0)
    {
        elePtr = (Element *)tagdObjPtr;
        int eleClassTag = elePtr->getClassTag();
        if ( eleClassTag == ELE_TAG_EightNodeBrick
                || eleClassTag == ELE_TAG_EightNodeBrick
                || eleClassTag == ELE_TAG_TwentySevenNodeBrick
                || eleClassTag == ELE_TAG_TwentySevenNodeBrickElastic
                || eleClassTag == ELE_TAG_ElasticBeam
                || eleClassTag == ELE_TAG_FourNodeAndesShell
                || eleClassTag == ELE_TAG_EightNodeBrickElastic
                || eleClassTag == ELE_TAG_ThreeNodeAndesShell
                || eleClassTag == ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam
                || eleClassTag == ELE_TAG_PenaltyElement
                || eleClassTag == ELE_TAG_PenaltyElementApplyDisplacement
                || eleClassTag == ELE_TAG_ContactElement_3DOF_3DOF
                || eleClassTag == ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF
                || eleClassTag == ELE_TAG_Truss
                || eleClassTag == ELE_TAG_EightNodeBrickLT
                || eleClassTag == ELE_TAG_TwentySevenNodeBrickLT
           )
        {
            //Guanzhou added
            if (tagdObjPtr->getTag() > maxEleNum)
            {
                maxEleNum = tagdObjPtr->getTag();
            }
        }
        else
        {
            cerr << "PartitionedDomain::buildEleGraph() -- (1) Unknown element classTag = " << eleClassTag << " \n";
        }
    }



    theElementTagVertices = new int[maxEleNum + 1];

    if (theElementTagVertices == 0)
    {
        cerr << "WARNING Domain::buildEleGraph ";
        cerr << " - Not Enough Memory for ElementTagVertices\n";
        return -1;
    }

    for (int j = 0; j <= maxEleNum; j++)
    {
        theElementTagVertices[j] = -1;
    }

    // now create the vertices with a reference equal to the element number.
    // and a tag which ranges from 0 through numVertex-1

    TaggedObjectIter &theEles2 = elements->getComponents();

    int count = START_VERTEX_NUM;

    while ((tagdObjPtr = theEles2()) != 0)
    {

        elePtr = (Element *)tagdObjPtr;

        //if ( elePtr->getClassTag() != ELE_TAG_EightNodeBrick  ) continue;//Guanzhou added //out by Babak on October 17 2012
        int eleClassTag = elePtr->getClassTag();
        if ( eleClassTag != ELE_TAG_EightNodeBrick
                && eleClassTag != ELE_TAG_TwentySevenNodeBrick
                && eleClassTag != ELE_TAG_TwentySevenNodeBrickElastic                  // Babak added on November 16 2012d
                && eleClassTag != ELE_TAG_ElasticBeam                                  // Added by Babak to consider the Elastic Beam Element in the Graph 12/12/12
                && eleClassTag != ELE_TAG_EightNodeBrickElastic                        // Added by Babak to consider the Elastic eigh node Brick in the Graph 1/18/13
                && eleClassTag != ELE_TAG_ThreeNodeAndesShell                          // Added by Babak to consider the Three Node Andes Shell in the Graph 2/11/13
                && eleClassTag != ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam // Added by Babak to consider the ank_one_deficient_elastic_pinned_fixed_beaml in the Graph 5/9/13
                && eleClassTag != ELE_TAG_PenaltyElement                               // Babak added on June 12 2013
                && eleClassTag != ELE_TAG_PenaltyElementApplyDisplacement              // Babak added on June 12 2013
                && eleClassTag != ELE_TAG_ContactElement_3DOF_3DOF                     // Babak added on November 5 2013
                && eleClassTag != ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF           // Babak added on November 25 2013
                && eleClassTag != ELE_TAG_Truss                                        // Babak added on January  22 2013
                && eleClassTag != ELE_TAG_FourNodeAndesShell                           // Babak added sometime
                && eleClassTag != ELE_TAG_EightNodeBrickLT                             // Jose Added Oct 2014
                && eleClassTag != ELE_TAG_TwentySevenNodeBrickLT                             // Jose Added Oct 2014
           )
        {
            cerr << "PartitionedDomain::buildEleGraph() -- (2) Unknown element classTag = " << eleClassTag << " \n";
            continue;    //Added by Babak Kamrani to include the 4NodeAndesShell in the element graph 12/28/12
        }

        int ElementTag = tagdObjPtr->getTag();

        Vertex *vertexPtr = new Vertex(count, ElementTag);

#ifdef _BABAK_GRAPH_DEBUG
        std::cerr << " BABAK@ PartitionedDomain::buildEleGraph --Allocating Memory to make vertexPtr to add  Vertex to Element Graph ...  tag (count) # " << count << " and ref (ElementTag) #" << ElementTag << "\n" << flush;
# endif

        if (vertexPtr == 0)
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Vertex\n";
            delete [] theElementTagVertices;
            return -1;
        }

        //# ifdef _PDD //Guanzhou added
        //  int size = tagdObjPtr->getObjectSize();
        //  vertexPtr->setVertexSize(size);
        //  //represents the (memory) size of vertex, which equals to the size of data
        //  //should be sent through SendSelf!
        //# endif

        theEleGraph->addVertex(vertexPtr);
#ifdef _BABAK_GRAPH_DEBUG
        std::cerr << " BABAK@ PartitionedDomain::buildEleGraph --Adding Vertex to Element Graph ...  tag (count) # " << count << " and ref (ElementTag) #" << ElementTag << "\n" << flush;
# endif
        theElementTagVertices[ElementTag] = count++;

    }

    // We now need to determine which elements are asssociated with each node.
    // As this info is not in the Node interface we must build it; which we
    // do using vertices for each node, when we addVertex at thes nodes we
    // will not be adding vertices but element tags.

    Vertex **theNodeTagVertices = 0;
    int maxNodNum = 0;
    Node *nodPtr;
    NodeIter &nodeIter = this->getNodes();

    while ((nodPtr = nodeIter()) != 0)
    {
        //  if ( nodPtr->getNumberDOF() != 3) continue;//Guanzhou added -- Out by babak on 12/12/12
        if ( nodPtr->getNumberDOF() != 6 && nodPtr->getNumberDOF() != 3 )
        {
            continue;    //Babak Added to count the Beam Elements in ... 12/12/12
        }

        if (nodPtr->getTag() > maxNodNum)
        {
            maxNodNum = nodPtr->getTag();
        }
    }

#ifdef _BABAK_GRAPH_DEBUG
    std::cerr << " BABAK@ PartitionedDomain::buildEleGraph -- The Max Number of the Nodes are # " << maxNodNum << "\n";
# endif

    // cerr << "maxNodNum: " <<maxNodNum<<"\n";

    theNodeTagVertices = new Vertex *[maxNodNum + 1];

    if (theNodeTagVertices == 0)
    {
        cerr << "WARNING Domain::buildEleGraph ";
        cerr << " - Not Enough Memory for NodeTagVertices\n";
        return -1;
    }

    for (int l = 0; l <= maxNodNum; l++)
    {
        theNodeTagVertices[l] = 0;
    }

    // now create the vertices with a reference equal to the node number.
    // and a tag which ranges from 0 through numVertex-1 and placed in
    // theNodeTagVertices at a position equal to the node's tag.

    NodeIter &nodeIter2 = this->getNodes();
    count = START_VERTEX_NUM;

    while ((nodPtr = nodeIter2()) != 0)
    {
        //  if ( nodPtr->getNumberDOF() != 3) continue;//Guanzhou added -- Out by babak on 12/12/12
        if ( nodPtr->getNumberDOF() != 6 && nodPtr->getNumberDOF() != 3 )
        {
            continue;    //Guanzhou added
        }

        int nodeTag = nodPtr->getTag();
        Vertex *vertexPtr = new Vertex(count++, nodeTag);

#ifdef _BABAK_GRAPH_DEBUG
        std::cerr << " BABAK@ PartitionedDomain::buildEleGraph -- Allocating Memory for Node Vertex  ...  tag (count++) # " << count << " and ref(nodeTag) #" << nodeTag << "\n" << flush;
# endif

        theNodeTagVertices[nodeTag] = vertexPtr;

        if (vertexPtr == 0)
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Node Vertex\n";
            delete [] theNodeTagVertices;
            return -1;
        }
    }

    // now add the the Elements to the nodes
    TaggedObjectIter &theEles3 = elements->getComponents();

    while ((tagdObjPtr = theEles3()) != 0)
    {

        elePtr = (Element *)tagdObjPtr;
        int eleClassTag = elePtr->getClassTag();
        if ( eleClassTag != ELE_TAG_EightNodeBrick
                && eleClassTag != ELE_TAG_TwentySevenNodeBrick
                && eleClassTag != ELE_TAG_TwentySevenNodeBrickElastic                  // Babak added on October 17 2012d                                                                 // Added the elastic on 11/18/12
                && eleClassTag != ELE_TAG_ElasticBeam                                  // Added by Babak to consider the Beam Element in the Graph 12/12/12
                && eleClassTag != ELE_TAG_EightNodeBrickElastic                        // Added by Babak to include the Elastic Eight Node Brick in the Graph 1/18/13
                && eleClassTag != ELE_TAG_ThreeNodeAndesShell                          // Added by Babak to consider the Three Node Andes Shell in the Graph 2/11/13
                && eleClassTag != ELE_TAG_rank_one_deficient_elastic_pinned_fixed_beam // Added by Babak to consider the ank_one_deficient_elastic_pinned_fixed_beaml in the Graph 5/9/13
                && eleClassTag != ELE_TAG_PenaltyElement                               // Babak added on June 12 2013
                && eleClassTag != ELE_TAG_PenaltyElementApplyDisplacement              // Babak added on June 12 2013
                && eleClassTag != ELE_TAG_ContactElement_3DOF_3DOF                     // Babak added on November 5 2013
                && eleClassTag != ELE_TAG_ContactElement_Nonlinear_3DOF_3DOF           // Babak added on November 25 2013
                && eleClassTag != ELE_TAG_Truss                                        // Babak added on January  22 2013
                && eleClassTag != ELE_TAG_FourNodeAndesShell
                && eleClassTag != ELE_TAG_EightNodeBrickLT
                && eleClassTag != ELE_TAG_TwentySevenNodeBrickLT
           )
        {
            cerr << "PartitionedDomain::buildEleGraph() -- (3) Unknown element class tag = " << eleClassTag << "\n";
            continue;    //Added by Babak to include the Beam Element in the Graph 12/28/12
        }


        int eleTag = elePtr->getTag();

        const ID &id = elePtr->getExternalNodes();

        int size = id.Size();



        for (int i = 0; i < size; i++)
        {
#ifdef _BABAK_GRAPH_DEBUG
            std::cerr <<  "BABAK @ PartitionedDomain::buildEleGraph -- Adding Element number " << eleTag << " as an edge to  theNodeTagVertice [ " << id(i) << "] ...\n" << flush;
# endif
            theNodeTagVertices[id(i)]->addEdge(eleTag);
        }
    }

#ifdef _BABAK_GRAPH_DEBUG
    Vertex *tempVertex ;
    tempVertex = theNodeTagVertices[3];

    const ID &id = tempVertex->getAdjacency();
    int size = id.Size();
    //cerr << "BABAK @ PartitionedDomain::buildEleGraph: ---------- ID  " <<  id << "\n";
# endif

    // now add the edges to the vertices of our element graph;
    // this is done by looping over the Node vertices, getting their
    // Adjacenecy and adding edges between elements with common nodes
    //GZ # ifdef _PDD
    Vertex *vertexPtr;

    for (int k = 0; k <= maxNodNum; k++)
        if ((vertexPtr = theNodeTagVertices[k]) != 0)
        {

            const ID &id = vertexPtr->getAdjacency();

            int size = id.Size();

# ifdef _BABAK_GRAPH_DEBUG
            cerr << "BABAK @ PartitionedDomain::buildEleGraph -- Node # " << k << " is connected to Elements :" << id << " \n";
# endif



            for (int i = 0; i < size; i++)
            {
                int Element1 = id(i);

                Element *ele1 = this->getElement(Element1);

                const ID &nodes1 = ele1->getExternalNodes();
                int num1 = nodes1.Size();

                //How many elements are in the boundary of an element (added 8/20/13):
                //-------------------------
                int Num1_Boundary_Nodes = 1;

                if (num1 == 27)
                {
                    Num1_Boundary_Nodes = 9;
                }

                if (num1 == 4)
                {
                    Num1_Boundary_Nodes = 2;
                }

                if (num1 == 2)
                {
                    Num1_Boundary_Nodes = 1;
                }

                //--------------------------


                int vertexTag1 = theElementTagVertices[Element1];

                for (int j = 0; j < size; j++)
                    if (i != j)
                    {

                        int Element2 = id(j);
                        int vertexTag2 = theElementTagVertices[Element2];

                        Element *ele2 = this->getElement(Element2);

                        const ID &nodes2 = ele2->getExternalNodes();
                        int num2 = nodes2.Size();
                        //How many elements are in the boundary of an element (added 8/20/13):
                        //-------------------------
                        int Num2_Boundary_Nodes = 1;

                        if (num2 == 27)
                        {
                            Num2_Boundary_Nodes = 9;
                        }

                        if (num2 == 4)
                        {
                            Num2_Boundary_Nodes = 2;
                        }

                        if (num2 == 2)
                        {
                            Num2_Boundary_Nodes = 1;
                        }

                        //--------------------------
                        int num_comm = Num1_Boundary_Nodes;

                        if ( Num1_Boundary_Nodes > Num2_Boundary_Nodes)
                        {
                            num_comm = Num2_Boundary_Nodes;
                        }

                        //--------------------------


                        int common = 0;

                        for (int k = 0; k < num1; k++)
                        {
                            for (int l = 0; l < num2; l++)
                            {
                                if ( nodes1(k) == nodes2(l) )
                                {
                                    common++;
                                }
                            }
                        }

                        // addEdge() adds for both vertices - do only once

                        //Based on the talk which I had with Boris, any common node is considered as an edge in the element graph ... //Babak on Dec 14 2012
                        if ( (vertexTag1 > vertexTag2) && (common == num_comm) ) //Added by Babak on 12/12/12
                        {
                            theEleGraph->addEdge(vertexTag1, vertexTag2);
                            theEleGraph->addEdge(vertexTag2, vertexTag1);
# ifdef _BABAK_GRAPH_DEBUG
                            std::cerr << "BABAK @ PartitionedDomain::buildEleGraph -- " << "vertexTag1: " << vertexTag1 << "   vertexTag2: " << vertexTag2 << "\n";
# endif
                        }
                    }
            }
        }


    // done now delete theElementTagVertices, the node Vertices and
    // theNodeTagVertices

    delete [] theElementTagVertices;

    for (int i = 0; i <= maxNodNum; i++)
        if ((vertexPtr = theNodeTagVertices[i]) != 0)
        {
            delete vertexPtr;
        }

    delete [] theNodeTagVertices;

# ifdef _BABAK_GRAPH_DEBUG
    std::cerr << "PartitionedDomain::buildEleGraph(Graph *theEleGraph) -- End \n" << flush;
# endif


    //Added by Babak to store built graph information in a file to make sure it is fine 4/2/13:
#ifdef _BABAK_GRAPH_DEBUG


    ofstream Element_Graph_File;
    Element_Graph_File.open("Element_Graph.out", ios::out | ios::app);

    Element_Graph_File << "Number of Vertices in the Element Graph are # " << theEleGraph->getNumVertex() << "\n";
    Element_Graph_File << "Number of Edges in the Element Graph are # " << theEleGraph->getNumEdge() << "\n";

    int number_of_vertex = theEleGraph->getNumVertex();
    int number_of_edges  = theEleGraph->getNumEdge();
    int Vertex_Ref = 0;
    ID Vertex_Adj ;
    int Vertex_Adj_size = 0;




    Vertex *Element_Vertex;

    for (int i_vertex = 0; i_vertex < number_of_vertex; i_vertex++)
    {
        Element_Vertex = theEleGraph->getVertexPtr(i_vertex);
        Vertex_Ref = Element_Vertex->getRef();
        Vertex_Adj = Element_Vertex->getAdjacency();
        Vertex_Adj_size = Vertex_Adj.Size();

        Element_Graph_File << "Ref#" <<   Vertex_Ref << "	Connected to " ;

        for (int i_adj = 0; i_adj < Vertex_Adj_size; i_adj++)
        {
            Element_Graph_File << Vertex_Adj(i_adj) << "-&-";
        }

        Element_Graph_File << "\n";
    }



    Element_Graph_File.close();
# endif

    std::cerr << "PartitionedDomain::buildEleGraph(Graph *theEleGraph) -- Element Graph is made \n" << flush;


    //abort();
    return 0;

}



// a method which will only remove a node from the partitioned domain
// it does not touch the subdomains .. can be dangerous to use.
Node *
PartitionedDomain::removeExternalNode(int tag)
{
    return (this->Domain::removeNode(tag));
}

Graph &
PartitionedDomain::getSubdomainGraph(void)
{
    // delete the old always - only object that will
    // use this is a DomainBalancer & it is always looking for latest
    if (mySubdomainGraph != 0)
    {
        delete mySubdomainGraph;
        mySubdomainGraph = 0;
    }

    // create a new graph
    if (mySubdomainGraph == 0)
    {
        mySubdomainGraph = new Graph(this->getNumSubdomains() + START_VERTEX_NUM);
    }

    if (mySubdomainGraph == 0) // if still 0 try a smaller one
    {
        mySubdomainGraph = new Graph();
    }

    int numVertex = theSubdomains->getNumComponents();

    // see if quick return

    if (numVertex == 0)
    {
        return *mySubdomainGraph;
    }

    // create another vertices array which aids in adding edges

    int *theElementTagVertices = 0;
    int maxEleNum = 0;

    TaggedObject *tagdObjPtr;
    TaggedObjectIter &theEles = theSubdomains->getComponents();

    while ((tagdObjPtr = theEles()) != 0)
        if (tagdObjPtr->getTag() > maxEleNum)
        {
            maxEleNum = tagdObjPtr->getTag();
        }

    theElementTagVertices = new int[maxEleNum + 1];

    if (theElementTagVertices == 0)
    {
        cerr << "WARNING PartitionedDomain::buildEleGraph ";
        cerr << " - Not Enough Memory for ElementTagVertices\n";
        exit(-1);
    }

    for (int j = 0; j <= maxEleNum; j++)
    {
        theElementTagVertices[j] = -1;
    }

    // now create the vertices with a reference equal to the subdomain number.
    // and a tag equal to the subdomain number and a weighed according to
    // the subdomain cost

    TaggedObjectIter &theEles2 = theSubdomains->getComponents();

    while ((tagdObjPtr = theEles2()) != 0)
    {
        Subdomain *theSub = (Subdomain *)tagdObjPtr; // upward cast ok as
        // only subdomais can be added
        int ElementTag = tagdObjPtr->getTag();

        Vertex *vertexPtr = new Vertex(ElementTag, ElementTag, theSub->getCost());

        if (vertexPtr == 0)
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << ElementTag << "th Vertex\n";
            delete [] theElementTagVertices;
            exit(-1);
        }

        mySubdomainGraph->addVertex(vertexPtr);

        theElementTagVertices[ElementTag] = ElementTag;
    }

    // We now need to determine which theSubdomains are asssociated with each node.
    // As this info is not in the Node interface we must build it; which we
    // do using vertices for each node, when we addVertex at thes nodes we
    // will not be adding vertices but element tags.

    Vertex **theNodeTagVertices = 0;
    int maxNodNum = 0;
    Node *nodPtr;
    NodeIter &nodeIter = this->getNodes();

    while ((nodPtr = nodeIter()) != 0)
        if (nodPtr->getTag() > maxNodNum)
        {
            maxNodNum = nodPtr->getTag();
        }

    theNodeTagVertices = new Vertex *[maxNodNum + 1];

    if (theNodeTagVertices == 0)
    {
        cerr << "WARNING Domain::buildEleGraph ";
        cerr << " - Not Enough Memory for NodeTagVertices\n";
        exit(-1);
    }

    for (int l = 0; l <= maxNodNum; l++)
    {
        theNodeTagVertices[l] = 0;
    }

    // now create the vertices with a reference equal to the node number.
    // and a tag which ranges from 0 through numVertex-1 and placed in
    // theNodeTagVertices at a position equal to the node's tag.

    NodeIter &nodeIter2 = this->getNodes();
    int count = START_VERTEX_NUM;

    while ((nodPtr = nodeIter2()) != 0)
    {
        int nodeTag = nodPtr->getTag();
        Vertex *vertexPtr = new Vertex(count++, nodeTag);
        theNodeTagVertices[nodeTag] = vertexPtr;

        if (vertexPtr == 0)
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Node Vertex\n";
            delete [] theNodeTagVertices;
            exit(-1);
        }
    }

    // now add the the TheSubdomains to the nodes
    Element *elePtr;
    TaggedObjectIter &theEles3 = theSubdomains->getComponents();

    while ((tagdObjPtr = theEles3()) != 0)
    {
        elePtr = (Element *)tagdObjPtr;
        int eleTag = elePtr->getTag();
        const ID &id = elePtr->getExternalNodes();

        int size = id.Size();

        for (int i = 0; i < size; i++)
        {
            theNodeTagVertices[id(i)]->addEdge(eleTag);
        }
    }

    // now add the edges to the vertices of our element graph;
    // this is done by looping over the Node vertices, getting their
    // Adjacenecy and adding edges between theSubdomains with common nodes

    Vertex *vertexPtr;

    for (int k = 0; k <= maxNodNum; k++)
        if ((vertexPtr = theNodeTagVertices[k]) != 0)
        {

            const ID &id = vertexPtr->getAdjacency();

            int size = id.Size();

            for (int i = 0; i < size; i++)
            {
                int Element1 = id(i);

                int vertexTag1 = theElementTagVertices[Element1];

                for (int j = 0; j < size; j++)
                    if (i != j)
                    {

                        int Element2 = id(j);
                        int vertexTag2 = theElementTagVertices[Element2];

                        // addEdge() adds for both vertices - do only once
                        if (vertexTag1 > vertexTag2)
                        {
                            mySubdomainGraph->addEdge(vertexTag1, vertexTag2);
                        }

                        mySubdomainGraph->addEdge(vertexTag2, vertexTag1);
                    }
            }
        }

    // done now delete theElementTagVertices, the node Vertices and
    // theNodeTagVertices

    delete [] theElementTagVertices;

    for (int i = 0; i <= maxNodNum; i++)
        if ((vertexPtr = theNodeTagVertices[i]) != 0)
        {
            delete vertexPtr;
        }

    delete [] theNodeTagVertices;

    return *mySubdomainGraph;
}

//Copied from Domain by Babak on October 2012:
//-------------

// BodyForce *
// Domain::removeBodyForce(int tag)
// {
//   // remove the object from the container
//   TaggedObject *mc = theBodyForces->removeComponent(tag);
//
//   // if not there return 0
//   if (mc == 0)
//       return 0;
//
//   // otherwise mark the domain as having changed
//   this->domainChange();
//
//   BodyForce *result = (BodyForce *)mc;
//   //  result->setDomain(0);
//   return result;
// }
//
// Damping *
// Domain::removeDamping(int tag)
// {
//   // remove the object from the container
//   TaggedObject *mc = theDampings->removeComponent(tag);
//
//   // if not there return 0
//   if (mc == 0)
//       return 0;
//
//   // otherwise mark the domain as having changed
//   this->domainChange();
//
//   Damping *result = (Damping *)mc;
//   //  result->setDomain(0);
//   return result;
// }
// //--------------


//added by babak at 2/25/13
//------
int
PartitionedDomain::return_element_numberofNodes(int tag)
{
    //tag starts from 1 ... but array starts from zero ...
    return Element_Node_Number_List[tag - 1];
}
//------