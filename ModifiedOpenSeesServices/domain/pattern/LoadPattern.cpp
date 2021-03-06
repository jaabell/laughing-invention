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

// $Revision: 1.9 $
// $Date: 2003/11/19 19:16:20 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/pattern/LoadPattern.cpp,v $

// Written: fmk 07/99
// Revised:
//
// Purpose: This file contains the method definitions for class LoadPattern.
// LoadPattern is a container class.
//
// The LoadPattern interface:


#include <LoadPattern.h>
#include <stdlib.h>
#include <ID.h>
#include <TimeSeries.h>
#include <NodalLoad.h>
#include <ElementalLoad.h>
#include <SP_Constraint.h>
#include <ArrayOfTaggedObjects.h>
#include <ElementalLoadIter.h>
#include <NodalLoadIter.h>
#include <SingleDomSP_Iter.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <OPS_Globals.h>

#include <iostream>
using namespace std;


LoadPattern::LoadPattern(int tag, int clasTag)
    : DomainComponent(tag, clasTag),
      isConstant(1),
      theSPs(0),
      loadFactor(0),
      theSeries(0),
      theNodalLoads(0),
      theElementalLoads(0),
      theNodIter(0),
      theEleIter(0),
      theSpIter(0)
{
    // constructor for subclass
    theNodalLoads = new ArrayOfTaggedObjects(32);
    theElementalLoads = new ArrayOfTaggedObjects(32);
    theSPs = new ArrayOfTaggedObjects(32);

    if (theNodalLoads == 0 || theElementalLoads == 0 || theSPs == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

    theEleIter = new ElementalLoadIter(theElementalLoads);
    theNodIter = new NodalLoadIter(theNodalLoads);
    theSpIter = new SingleDomSP_Iter(theSPs);

    if (theEleIter == 0 || theNodIter == 0 || theSpIter == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

}


LoadPattern::LoadPattern()
    : DomainComponent(0, PATTERN_TAG_LoadPattern),
      isConstant(1),
      theSPs(0),
      loadFactor(0),
      theSeries(0),
      theNodalLoads(0),
      theElementalLoads(0),
      theNodIter(0),
      theEleIter(0),
      theSpIter(0),
      lastChannel(0)
{
    theNodalLoads = new ArrayOfTaggedObjects(32);
    theElementalLoads = new ArrayOfTaggedObjects(32);
    theSPs = new ArrayOfTaggedObjects(32);

    if (theNodalLoads == 0 || theElementalLoads == 0 || theSPs == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

    theEleIter = new ElementalLoadIter(theElementalLoads);
    theNodIter = new NodalLoadIter(theNodalLoads);
    theSpIter = new SingleDomSP_Iter(theSPs);

    if (theEleIter == 0 || theNodIter == 0 || theSpIter == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

}


LoadPattern::LoadPattern(int tag)
    : DomainComponent(tag, PATTERN_TAG_LoadPattern),
      isConstant(1),
      theSPs(0),
      loadFactor(0.),
      theSeries(0),
      theNodalLoads(0),
      theElementalLoads(0),
      theNodIter(0),
      theEleIter(0),
      theSpIter(0),
      lastChannel(0)
{
    theNodalLoads = new ArrayOfTaggedObjects(32);
    theElementalLoads = new ArrayOfTaggedObjects(32);
    theSPs = new ArrayOfTaggedObjects(32);

    if (theNodalLoads == 0 || theElementalLoads == 0 || theSPs == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

    theEleIter = new ElementalLoadIter(theElementalLoads);
    theNodIter = new NodalLoadIter(theNodalLoads);
    theSpIter = new SingleDomSP_Iter(theSPs);

    if (theEleIter == 0 || theNodIter == 0 || theSpIter == 0)
    {
        cerr << " LoadPattern::LoadPattern() - ran out of memory\n";
        exit(-1);
    }

}


// ~LoadPattern()
//  destructor

LoadPattern::~LoadPattern()
{
    if (theSeries != 0)
    {
        delete theSeries;
    }

    if (theNodalLoads != 0)
    {
        delete theNodalLoads;
    }

    if (theElementalLoads != 0)
    {
        delete theElementalLoads;
    }

    if (theSPs != 0)
    {
        delete theSPs;
    }

    if (theEleIter != 0)
    {
        delete theEleIter;
    }

    if (theNodIter != 0)
    {
        delete theNodIter;
    }

    if (theSpIter != 0)
    {
        delete theSpIter;
    }


}


void
LoadPattern::setTimeSeries(TimeSeries *theTimeSeries)
{
    // invoke the destructor on the old TimeSeries
    if (theSeries != 0)
    {
        delete theSeries;
    }

    // set the pointer to the new series object
    theSeries = theTimeSeries;
}


void
LoadPattern::setDomain(Domain *theDomain)
{
    // if subclass does not implement .. check for 0 pointer
    if (theNodalLoads != 0)
    {
        NodalLoad *nodLoad;
        NodalLoadIter &theNodalIter = this->getNodalLoads();

        while ((nodLoad = theNodalIter()) != 0)
        {
            nodLoad->setDomain(theDomain);
        }

        ElementalLoad *eleLoad;
        ElementalLoadIter &theElementalIter = this->getElementalLoads();

        while ((eleLoad = theElementalIter()) != 0)
        {
            eleLoad->setDomain(theDomain);
        }

        SP_Constraint *theSP;
        SP_ConstraintIter &theSpConstraints = this->getSPs();

        while ((theSP = theSpConstraints()) != 0)
        {
            theSP->setDomain(theDomain);
        }
    }

    // now we set this load patterns domain
    this->DomainComponent::setDomain(theDomain);
}



bool
LoadPattern::addNodalLoad(NodalLoad *load)
{
    Domain *theDomain = this->getDomain();

    bool result = theNodalLoads->addComponent(load);

    if (result == true)
    {
        if (theDomain != 0)
        {
            load->setDomain(theDomain);
        }

        load->setLoadPatternTag(this->getTag());
    }
    else
    {
        cerr << "WARNING: LoadPattern::addNodalLoad() - load could not be added\n";
    }

    return result;
}

bool
LoadPattern::addElementalLoad(ElementalLoad *load)
{
    Domain *theDomain = this->getDomain();

    bool result = theElementalLoads->addComponent(load);

    if (result == true)
    {
        if (theDomain != 0)
        {
            load->setDomain(theDomain);
        }

        load->setLoadPatternTag(this->getTag());
    }
    else
    {
        cerr << "WARNING: LoadPattern::addElementalLoad() - load could not be added\n";
    }

    return result;
}

bool
LoadPattern::addSP_Constraint(SP_Constraint *theSp)
{
    Domain *theDomain = this->getDomain();

    bool result = theSPs->addComponent(theSp);

    if (result == true)
    {
        if (theDomain != 0)
        {
            theSp->setDomain(theDomain);
        }

        theSp->setLoadPatternTag(this->getTag());
    }
    else
    {
        cerr << "WARNING: LoadPattern::addSP_Constraint() - load could not be added\n";
    }

    return result;
}


NodalLoadIter &
LoadPattern::getNodalLoads(void)
{
    theNodIter->reset();
    return *theNodIter;
}

ElementalLoadIter &
LoadPattern::getElementalLoads(void)
{
    theEleIter->reset();
    return *theEleIter;
}

SP_ConstraintIter &
LoadPattern::getSPs(void)
{
    theSpIter->reset();
    return *theSpIter;
}

void
LoadPattern::clearAll(void)
{
    theElementalLoads->clearAll();
    theNodalLoads->clearAll();
    theSPs->clearAll();
    lastChannel = 0;
}

NodalLoad *
LoadPattern::removeNodalLoad(int tag)
{
    TaggedObject *obj = theNodalLoads->removeComponent(tag);

    if (obj == 0)
    {
        return 0;
    }

    NodalLoad *result = (NodalLoad *)obj;
    result->setDomain(0);
    return result;
}

ElementalLoad *
LoadPattern::removeElementalLoad(int tag)
{
    TaggedObject *obj = theElementalLoads->removeComponent(tag);

    if (obj == 0)
    {
        return 0;
    }

    ElementalLoad *result = (ElementalLoad *)obj;
    result->setDomain(0);
    return result;
}

SP_Constraint *
LoadPattern::removeSP_Constraint(int tag)
{
    TaggedObject *obj = theSPs->removeComponent(tag);

    if (obj == 0)
    {
        return 0;
    }

    SP_Constraint *result = (SP_Constraint *)obj;
    result->setDomain(0);
    return result;
}


void
LoadPattern::applyLoad(double pseudoTime)
{
    // first determine the load factor
    if (theSeries != 0 && isConstant != 0)
    {
        loadFactor = theSeries->getFactor(pseudoTime);
    }

    NodalLoad *nodLoad;
    NodalLoadIter &theNodalIter = this->getNodalLoads();

    while ((nodLoad = theNodalIter()) != 0)
    {
        nodLoad->applyLoad(loadFactor);
    }

    ElementalLoad *eleLoad;
    ElementalLoadIter &theElementalIter = this->getElementalLoads();

    while ((eleLoad = theElementalIter()) != 0)
    {
        eleLoad->applyLoad(loadFactor);
    }

    SP_Constraint *sp;
    SP_ConstraintIter &theIter = this->getSPs();

    while ((sp = theIter()) != 0)
    {
        sp->applyConstraint(loadFactor);
    }
}

void
LoadPattern::setLoadConstant(void)
{

    if ( (this->getClassTag() == PATTERN_TAG_Domain_Reduction_Method_Modified_Input) ||
            (this->getClassTag() == PATTERN_TAG_Domain_Reduction_Method) ||
            (this->getClassTag() == PATTERN_TAG_Domain_Reduction_Method_Restore_Forces) ||
            (this->getClassTag() == PATTERN_TAG_Linear_Force_TimeHistory) ||
            (this->getClassTag() == PATTERN_TAG_PathSeries_Force_TimeHistory) ||
            (this->getClassTag() == PATTERN_TAG_Linear_Disp_TimeHistory_Penalty) ||
            (this->getClassTag() == PATTERN_TAG_PathSeries_Disp_TimeHistory_Penalty) ||
            (this->getClassTag() == PATTERN_TAG_PathTimeSeries_Force_TimeHistory) )
    {
        this->setLoadConstant();
    }
    else
    {
        isConstant = 0;
    }

}




void
LoadPattern::unsetLoadConstant(void)
{
    isConstant = 1;
}

double
LoadPattern::getLoadFactor(void)
{
    if (theSeries != 0)
    {
        return loadFactor;
    }
    else
    {
        return 0.0;
    }
}

int
LoadPattern::sendSelf(int cTag, Channel &theChannel)
{

    // get my current database tag
    // NOTE - dbTag equals 0 if not sending to a database OR has not yet been sent
    // int 0 = this->getDbTag();

    // into an ID we place all info needed to determine state of LoadPattern
    int numNodLd, numEleLd, numSPs;
    ID lpData(11);

    numNodLd = theNodalLoads->getNumComponents();
    numEleLd = theElementalLoads->getNumComponents();
    numSPs = theSPs->getNumComponents();

    lpData(10) = this->getTag();
    lpData(0) = 0;//currentGeoTag;
    lpData(1) = numNodLd;
    lpData(2) = numEleLd;
    lpData(3) = numSPs;

    // if (dbNod == 0)
    // {
    //     dbNod = theChannel.getDbTag();
    //     dbEle = theChannel.getDbTag();
    //     dbSPs = theChannel.getDbTag();
    // }

    lpData(4) = 0;//dbNod;
    lpData(5) = 0;//dbEle;
    lpData(6) = 0;//dbSPs;

    lpData(7) = isConstant;

    if (theSeries != 0)
    {
        // int dbtag = theSeries->getDbTag();
        int classtag = theSeries->getClassTag();

        // if (dbtag == 0)
        // {
        //     // dbtag = theChannel.getDbTag();
        //     // theSeries->setDbTag(dbtag);
        // }

        lpData(8) = classtag;
        lpData(9) = 0;//dbtag;
    }
    else
    {
        lpData(8) = -1;
    }

    // see if we can save sending the vector containing just the load factor
    // will happen in parallel if sending the loadPattern .. not in database

    if (theChannel.sendID(0, cTag, lpData) < 0)
    {
        cerr << "LoadPattern::sendSelf - channel failed to send the initial ID\n";
        return -1;
    }

    if (isConstant == 0)
    {
        Vector data(1);
        data(0) = loadFactor;

        if (theChannel.sendVector(0, cTag, data) < 0)
        {
            cerr << "LoadPattern::sendSelf - channel failed to send the Vector\n";
            return -2;
        }

    }

    if (theSeries != 0)
    {
        if (theSeries->sendSelf(cTag, theChannel) < 0)
        {
            cerr << "LoadPattern::sendSelf - the TimeSeries failed to send\n";
            return -3;
        }
    }

    // now check if data defining the objects in the LoadPAttern needs to be sent
    // NOTE THIS APPROACH MAY NEED TO CHANGE FOR VERY LARGE PROBLEMS IF CHANNEL CANNOT
    // HANDLE VERY LARGE ID OBJECTS.

    /*
    if (theChannel.isDatastore() == 1) {
      static ID theLastSendTag(1);
      if (theChannel.receiveID(0,0,theLastSendTag) == 0)
        lastGeoSendTag = theLastSendTag(0);
      else
        lastGeoSendTag = -1;
    }
    */

    if (lastChannel != theChannel.getTag() )//|| lastGeoSendTag != currentGeoTag || theChannel.isDatastore() == 0)
    {

        lastChannel = theChannel.getTag();

        //
        // into an ID we are gonna place the class and db tags for each node so can rebuild
        // this ID we then send to the channel
        //

        // create the ID and get the node iter
        if (numNodLd != 0)
        {
            ID nodeData(numNodLd * 2);
            NodalLoad *theNode;
            NodalLoadIter &theNodes = this->getNodalLoads();
            int loc = 0;

            // loop over nodes in domain adding their classTag and dbTag to the ID
            while ((theNode = theNodes()) != 0)
            {
                nodeData(loc) = theNode->getClassTag();
                // int dbTag = theNode->getDbTag();

                // if dbTag still 0 get one from Channel;
                // if this tag != 0 set the dbTag in node
                // if (dbTag == 0 && 0 != 0)  // go get a new tag and setDbTag in ele if this not 0
                // {
                //     dbTag = theChannel.getDbTag();

                //     if (dbTag != 0)
                //     {
                //         theNode->setDbTag(dbTag);
                //     }
                // }

                nodeData(loc + 1) = 0.;//dbTag;
                loc += 2;
            }

            // now send the ID
            if (theChannel.sendID(0, 0, nodeData) < 0)
            {
                cerr << "LoadPattern::sendSelf - channel failed to send the NodalLoads ID\n";
                return -4;
            }
        }

        // we do the same for elemental loads as we did for nodal loads above .. see comments above!

        if (numEleLd != 0)
        {
            ID elementData(numEleLd * 2);
            ElementalLoad *theEle;
            ElementalLoadIter &theElements = this->getElementalLoads();
            int loc = 0;

            while ((theEle = theElements()) != 0)
            {
                elementData(loc) = theEle->getClassTag();
                // int dbTag = theEle->getDbTag();

                // if (dbTag == 0 && 0 != 0)  // go get a new tag and setDbTag in ele if this not 0
                // {
                //     dbTag = theChannel.getDbTag();

                //     if (dbTag != 0)
                //     {
                //         theEle->setDbTag(dbTag);
                //     }
                // }

                elementData(loc + 1) = 0;//dbTag;
                loc += 2;
            }

            // now send the ID
            if (theChannel.sendID(0, 0, elementData) < 0)
            {
                cerr << "Domain::send - channel failed to send the element ID\n";
                return -5;
            }
        }

        // we do the same for SP_Constraints as for NodalLoads above .. see comments above!

        if (numSPs != 0)
        {
            ID spData(numSPs * 2);
            SP_Constraint *theSP;
            SP_ConstraintIter &theSPs = this->getSPs();
            int loc = 0;

            while ((theSP = theSPs()) != 0)
            {
                spData(loc) = theSP->getClassTag();
                // int dbTag = theSP->getDbTag();

                // if (dbTag == 0 && 0 != 0)  // go get a new tag and setDbTag in ele if this not 0
                // {
                //     dbTag = theChannel.getDbTag();

                //     if (dbTag != 0)
                //     {
                //         theSP->setDbTag(dbTag);
                //     }
                // }

                spData(loc + 1) = 0;//dbTag;
                loc += 2;
            }

            if (theChannel.sendID(0, 0, spData) < 0)
            {
                cerr << "LoadPAttern::sendSelf - channel failed sending SP_Constraint ID\n";
                return -6;
            }
        }

        // set the lst send db tag so we don't have to do all that again
        // lastGeoSendTag = currentGeoTag;

        // if (theChannel.isDatastore() == 1)
        // {
        //     static ID theLastSendTag(1);
        //     theLastSendTag(0) = lastGeoSendTag;
        //     theChannel.sendID(0, 0, theLastSendTag);
        // }
    }


    // now we invoke sendSelf on all the NodalLoads, ElementalLoads and SP_Constraints
    // which have been added to the LoadCase
    NodalLoad *theNode;
    NodalLoadIter &theNodes = this->getNodalLoads();

    while ((theNode = theNodes()) != 0)
    {
        if (theNode->sendSelf(cTag, theChannel) < 0)
        {
            cerr << "LoadPattern::sendSelf - node with tag " << theNode->getTag() << " failed in sendSelf\n";
            return -7;
        }
    }

    ElementalLoad *theEle;
    ElementalLoadIter &theElements = this->getElementalLoads();

    while ((theEle = theElements()) != 0)
    {
        if (theEle->sendSelf(cTag, theChannel) < 0)
        {
            cerr << "LoadPattern::sendSelf - element with tag " << theEle->getTag() << " failed in sendSelf\n";
            return -8;
        }
    }

    SP_Constraint *theSP;
    SP_ConstraintIter &theSPs = this->getSPs();

    while ((theSP = theSPs()) != 0)
    {
        if (theSP->sendSelf(cTag, theChannel) < 0)
        {

            cerr << "LoadPattern::sendSelf - SP_Constraint failed sendSelf\n";
            //       cerr << "LoadPattern::sendSelf - SP_Constraint: " << *theSP << " failed sendSelf\n";
            return -9;
        }
    }

    // if we get here we are successfull
    return 0;
}



int
LoadPattern::receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{

    // get my current database tag
    // NOTE - dbTag equals 0 if not sending to a database OR has not yet been sent
    // int 0 = this->getDbTag();

    // into an ID we place all info needed to determine state of LoadPattern
    int numNod, numEle, numSPs;
    ID lpData(11);

    if (theChannel.receiveID(0, cTag, lpData) < 0)
    {
        cerr << "LoadPattern::receiveSelf - channel failed to recv the initial ID\n";
        return -1;
    }

    isConstant = lpData(7);

    this->setTag(lpData(10));

    if (isConstant == 0)   // we must recv the load factor in a Vector
    {
        Vector data(1);

        if (theChannel.receiveVector(0, cTag, data) < 0)
        {
            cerr << "LoadPattern::receiveSelf - channel failed to recv the Vector\n";
            return -2;
        }

        loadFactor = data(0);
    }

    // read data about the time series
    if (lpData(8) != -1)
    {
        if (theSeries == 0)
        {
            theSeries = theBroker.getNewTimeSeries(lpData(8));
        }
        else if (theSeries->getClassTag() != lpData(8))
        {
            delete theSeries;
            theSeries = theBroker.getNewTimeSeries(lpData(8));
        }

        if (theSeries == 0)
        {
            cerr << "LoadPattern::receiveSelf - failed to create TimeSeries\n";
            return -3;
        }

        theSeries->setDbTag(lpData(9));

        if (theSeries->receiveSelf(cTag, theChannel, theBroker) < 0)
        {
            cerr << "LoadPattern::receiveSelf - the TimeSeries failed to recv\n";
            return -3;
        }
    }

    /*
    if (theChannel.isDatastore() == 1) {
      static ID theLastSendTag(1);
      if (theChannel.receiveID(0,0,theLastSendTag) == 0)
        lastGeoSendTag = theLastSendTag(0);
    }
    */

    if (lastChannel != theChannel.getTag())// || currentGeoTag != lpData(0) || theChannel.isDatastore() == 0)
    {

        // clear out the all the components in the current load pattern
        this->clearAll();
        lastChannel = theChannel.getTag();
        // currentGeoTag = lpData(0);

        numNod = lpData(1);
        numEle = lpData(2);
        numSPs = lpData(3);
        // dbNod = lpData(4);
        // dbEle = lpData(5);
        // dbSPs = lpData(6);

        //
        // now we rebuild the nodal loads
        //

        // first get the information from the domainData about the nodes
        if (numNod != 0)
        {
            ID nodeData(2 * numNod);


            // now receive the ID about the nodes, class tag and dbTags
            if (theChannel.receiveID(0, 0, nodeData) < 0)
            {
                cerr << "LoadPAttern::receiveSelf - channel failed to recv the NodalLoad ID\n";
                return -2;
            }

            // now for each NodalLoad we 1) get a new node of the correct type from the ObjectBroker
            // 2) ensure the node exists and set it's dbTag, 3) we invoke receiveSelf on this new
            // blank node and 4) add this node to the domain

            int loc = 0;

            for (int i = 0; i < numNod; i++)
            {
                int classTag = nodeData(loc);
                int dbTag = nodeData(loc + 1);

                NodalLoad *theNode = theBroker.getNewNodalLoad(classTag);

                if (theNode == 0)
                {
                    cerr << "LoadPattern::recv - cannot create NodalLoad with classTag " << classTag << endln;
                    return -2;
                }

                theNode->setDbTag(dbTag);

                if (theNode->receiveSelf(cTag, theChannel, theBroker) < 0)
                {
                    cerr << "LoadPattern::receiveSelf - NodalLoad with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if (this->addNodalLoad(theNode) == false)
                {
                    cerr << "LoadPattern::receiveSelf - failed adding NodalLoad tagged " << theNode->getTag() << " into LP!\n";
                    return -3;
                }

                loc += 2;
            }
        }

        //
        // now we rebuild the ElementalLoads .. same as NodalLoads above .. see comments above
        //

        if (numEle != 0)
        {
            ID eleData(2 * numEle);

            if (theChannel.receiveID(0, 0, eleData) < 0)
            {
                cerr << "LoadPattern::receiveSelf - channel failed to recv the EleLoad ID\n";
                return -2;
            }

            int loc = 0;

            for (int i = 0; i < numEle; i++)
            {
                int classTag = eleData(loc);
                int dbTag = eleData(loc + 1);

                ElementalLoad *theEle = theBroker.getNewElementalLoad(classTag);

                if (theEle == 0)
                {
                    cerr << "LoadPattern::recv - cannot create ElementalLoad with classTag " << classTag << endln;
                    return -2;
                }

                // theEle->setDbTag(dbTag);

                if (theEle->receiveSelf(cTag, theChannel, theBroker) < 0)
                {
                    cerr << "LoadPattern::receiveSelf - Ele with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if (this->addElementalLoad(theEle) == false)
                {
                    cerr << "LoadPattern::receiveSelf - could not add Ele with tag " << theEle->getTag() << " into LP!\n";
                    return -3;
                }

                loc += 2;
            }
        }

        //
        // now we rebuild the SP_Constraints .. same as nodes above .. see above if can't understand!!
        //

        if (numSPs != 0)
        {
            ID spData(2 * numSPs);

            if (theChannel.receiveID(0, 0, spData) < 0)
            {
                cerr << "LoadPattern::receiveSelf - channel failed to recv the SP_Constraints ID\n";
                return -2;
            }

            int loc = 0;

            for (int i = 0; i < numSPs; i++)
            {
                int classTag = spData(loc);
                // int dbTag = spData(loc + 1);

                SP_Constraint *theSP = theBroker.getNewSP(classTag);

                if (theSP == 0)
                {
                    cerr << "LoadPattern::recv - cannot create SP_Constraint with classTag " << classTag << endln;
                    return -2;
                }

                // theSP->setDbTag(dbTag);

                if (theSP->receiveSelf(cTag, theChannel, theBroker) < 0)
                {
                    cerr << "LoadPattern::receiveSelf - SP_Constraint  failed in receiveSelf\n";
                    return -2;
                }

                if (this->addSP_Constraint(theSP) == false)
                {
                    cerr << "LoadPattern::receiveSelf - could not add SP_Constraint with tag " << theSP->getTag()
                         << " into LP!\n";

                    return -3;
                }

                loc += 2;
            }
        }

        // now set the load pattern db count
        // currentGeoTag = lpData(0);
        // lastGeoSendTag  = currentGeoTag;

    }
    else
    {
        if (theSeries != 0)
            if (theSeries->receiveSelf(cTag, theChannel, theBroker) < 0)
            {
                cerr << "LoadPattern::receiveSelf - the TimeSeries failed to recv\n";
                return -3;
            }

        NodalLoad *theNode;
        NodalLoadIter &theNodes = this->getNodalLoads();

        while ((theNode = theNodes()) != 0)
        {
            if (theNode->receiveSelf(cTag, theChannel, theBroker) < 0)
            {
                cerr << "LoadPattern::receiveSelf - node with tag " << theNode->getTag() << " failed in receiveSelf\n";
                return -7;
            }
        }

        ElementalLoad *theEle;
        ElementalLoadIter &theElements = this->getElementalLoads();

        while ((theEle = theElements()) != 0)
        {
            if (theEle->receiveSelf(cTag, theChannel, theBroker) < 0)
            {
                cerr << "LoadPattern::receiveSelf - element with tag " << theEle->getTag() << " failed in receiveSelf\n";
                return -8;
            }
        }

        SP_Constraint *theSP;
        SP_ConstraintIter &theSPs = this->getSPs();

        while ((theSP = theSPs()) != 0)
        {
            if (theSP->receiveSelf(cTag, theChannel, theBroker) < 0)
            {
                cerr << "LoadPattern::receiveSelf - SP_Constraint tagged " << theSP->getTag() << "  failed receiveSelf\n";
                return -9;
            }
        }
    }

    // if we get here we are successfull
    return 0;
}

void
LoadPattern::Print(ostream &s, int flag)
{
    s << "Load Pattern: " << this->getTag() << "\n";

    if (theSeries != 0)
    {
        theSeries->Print(s, flag);
    }

    cerr << "  Nodal Loads: \n";
    theNodalLoads->Print(s, flag);
    cerr << "\n  Elemental Loads: \n";
    theElementalLoads->Print(s, flag);
    cerr << "\n  Single Point Constraints: \n";
    theSPs->Print(s, flag);
}


LoadPattern *
LoadPattern::getCopy(void)
{
    LoadPattern *theCopy = new LoadPattern(this->getTag());

    if (theCopy == 0)
    {
        cerr << "LoadPattern::getCopy() - ran out of memory\n";
        return theCopy; // in case fatal() does not exit
    }

    theCopy->loadFactor = loadFactor;
    theCopy->isConstant = isConstant;
    theCopy->theSeries = theSeries;
    return theCopy;
}


