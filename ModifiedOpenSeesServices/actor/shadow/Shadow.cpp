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
// $Date: 2003/10/15 00:35:22 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/shadow/Shadow.cpp,v $


// Written: fmk
// Revision: A
//
// Purpose: This file contains the implementation of Shadow.
//
// What: "@(#) Shadow.C, revA"

#include <Shadow.h>
#include <stdlib.h>

#include <Channel.h>
#include <MachineBroker.h>
#include <Message.h>
#include <MovableObject.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


#ifdef _BABAK_DEBUG
#include <LoadPattern.h>
extern LoadPattern* DRM_LOAD_PATTERN ;
#endif
Shadow::Shadow(Channel& theChan,
               FEM_ObjectBroker& myBroker)
    : theChannel(&theChan), theBroker(&myBroker), theRemoteActorsAddress(0), commitTag(0)
{
    theChannel->setUpConnection();
}


Shadow::Shadow(Channel& theChan,
               FEM_ObjectBroker& myBroker,
               ChannelAddress& theAddress)
    : theChannel(&theChan), theBroker(&myBroker), theRemoteActorsAddress(&theAddress),
      commitTag(0)
{
    theChannel->setUpConnection();
}

Shadow::Shadow(int actorType,
               FEM_ObjectBroker& myBroker,
               MachineBroker& theMachineBroker,
               int compDemand)
    : theBroker(&myBroker), theRemoteActorsAddress(0), commitTag(0)
{
    // start the remote actor process running
    theChannel = theMachineBroker.startActor(actorType, compDemand);

    if (theChannel < 0)
    {
        cerr << "Shadow::Shadow - could not start remote actor\n";
        cerr << " using program " << actorType << endln;
        exit(-1);
    }

    // now call setUpShadow on the channel
    theChannel->setUpConnection();
    theRemoteActorsAddress = theChannel->getLastSendersAddress();
}

Shadow::~Shadow()
{

}

int
Shadow::sendObject(MovableObject& theObject)
{
    #ifdef _BABAK_DEBUG
    //cerr << "Shadow::sendObject--BEFORE theSubdomain->addLoadPattern(newLoadPattern) -- DRM INFORMATION:\n";
    //DRM_LOAD_PATTERN->Print(cerr);
    #endif
    return theChannel->sendObj(commitTag, theObject, theRemoteActorsAddress);
}

int
Shadow::receiveObject(MovableObject& theObject)
{
    return theChannel->receiveObj(commitTag, theObject, *theBroker, theRemoteActorsAddress);
}


int
Shadow::recvMessage(Message& theMessage)
{
    return theChannel->receiveMsg(0, commitTag, theMessage, theRemoteActorsAddress);
}

int
Shadow::sendMessage(const Message& theMessage)
{
    return theChannel->sendMsg(0, commitTag, theMessage, theRemoteActorsAddress);
}

int
Shadow::sendMatrix(const Matrix& theMatrix)
{
    return theChannel->sendMatrix(0, commitTag, theMatrix, theRemoteActorsAddress);
}

int
Shadow::receiveMatrix(Matrix& theMatrix)
{
    return theChannel->receiveMatrix(0, commitTag, theMatrix, theRemoteActorsAddress);
}

int
Shadow::sendVector(const Vector& theVector)
{
    return theChannel->sendVector(0, commitTag, theVector, theRemoteActorsAddress);
}

int
Shadow::receiveVector(Vector& theVector)
{
    return theChannel->receiveVector(0, commitTag, theVector, theRemoteActorsAddress);
}

int
Shadow::sendID(const ID& theID)
{
    #ifdef _BABAK_DEBUG

    if (theID(1) == 14)
    {
        cerr << "Babak@ Shadow::sendID(const ID &theID) ... theID " << theID << "\n";
    }

    #endif
    return theChannel->sendID(0, commitTag, theID, theRemoteActorsAddress);
}

int
Shadow::receiveID(ID& theID)
{
    #ifdef _BABAK_DEBUG

    if (theID(1) == 14)
    {
        cerr << "Babak@ Shadow::receiveID(ID &theID) ... theID " << theID << "\n";
    }

    #endif
    return theChannel->receiveID(0, commitTag, theID, theRemoteActorsAddress);
}


void
Shadow::setCommitTag(int tag)
{
    commitTag = tag;
}


Channel*
Shadow::getChannelPtr(void) const
{
    return theChannel;
}

FEM_ObjectBroker*
Shadow::getObjectBrokerPtr(void) const
{
    return theBroker;
}

ChannelAddress*
Shadow::getActorAddressPtr(void) const
{
    return theRemoteActorsAddress;
}


