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

// $Revision: 1.1 $
// $Date: 2003/08/29 07:17:38 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/machineBroker/MachineBroker.cpp,v $


// Written: fmk
// Revision: A

#include <MachineBroker.h>
#include <OPS_Globals.h>
#include <ID.h>
#include <Channel.h>

#include <Actor.h>
#include <FEM_ObjectBroker.h>

#include <OPS_Globals.h>
#include <iostream>
using namespace std;


MachineBroker::MachineBroker(FEM_ObjectBroker *theBroker)
    : theObjectBroker(theBroker), actorChannels(0), numActorChannels(0), numActiveChannels(0), activeChannels(0)
{

}

MachineBroker::~MachineBroker()
{
    if (actorChannels != 0)
    {
        delete [] actorChannels;
        delete activeChannels;
    }
}


int
MachineBroker::shutdown(void)
{
    // cerr << " MachineBroker::shutdown()\n";

    // send the termination notice to all machineBrokers running actorProcesses
    for (int i = 0; i < numActorChannels; i++)
    {
        ID idData(1);
        idData(0) = 0;
        Channel *theChannel = actorChannels[i];

        if (theChannel->sendID(0, 0, idData) < 0)
        {
            cerr << "MachineBroker::run(void) - failed to send ID\n";
        }

        // cerr << " MachineBroker::shutdown() 2\n";

        if (theChannel->receiveID(0, 0, idData) < 0)
        {
            cerr << "MachineBroker::run(void) - failed to send ID\n";
        }

        // cerr << " MachineBroker::shutdown 3()\n";
        this->freeProcess(theChannel);
    }

    if (actorChannels != 0)
    {
        delete [] actorChannels;
        delete activeChannels;
        actorChannels = 0;
        activeChannels = 0;
        numActorChannels = 0;
        numActiveChannels = 0;
    }

    return 0;
}


int
MachineBroker::runActors(void)
{
    Channel *theChannel = this->getMyChannel();
    ID idData(1);
    int done = 0;
    // cerr << "MachineBroker::runActors()\n";

    // loop until recv kill signal
    while (done == 0)
    {
        if (theChannel->receiveID(0, 0, idData) < 0)
        {
            cerr << "MachineBroker::run(void) - failed to recv ID\n";
        }

        int actorType = idData(0);

        // switch on data type
        if (idData(0) == 0)
        {
            cout << "    Finalizing current actor " << endl;

            done = 1;

            if (theChannel->sendID(0, 0, idData) < 0)
            {
                cerr << "MachineBroker::run(void) - failed to send ID\n";
            }
        }
        else
        {
            // cout << "    Spawning new actor domain - actorType: " << actorType << endln;;

            // create an actor of approriate type
            Actor *theActor = theObjectBroker->getNewActor(actorType, theChannel);

            if (theActor == 0)
            {
                cerr << "MachineBroker::run(void) - invalid actor type\n";
                idData(0) = 1;
            }
            else
            {
                idData(0) = 0;
            }

            // send ID back indicating wheter actor was created
            if (theChannel->sendID(0, 0, idData) < 0)
            {
                cerr << "MachineBroker::run(void) - failed to send ID\n";
            }

            // run the actor object
            if (theActor->run() != 0)
            {
                cerr << "MachineBroker::run(void) - actor failed while running\n";
            }

            // destroying theActor
            delete theActor;
        }
    }

    cout << "Actor DONE! " << endln;

    return 0;
}


Channel *
MachineBroker::startActor(int actorType, int compDemand)
{
    if (compDemand != 0)
    {
        cerr << "MachineBroker::startActor() - does not take computational demand variable into account\n";
    }

    Channel *theChannel = 0;

    // check if have an available machine broker running runActors() and waiting to start an actor running
    if (numActiveChannels < numActorChannels)
    {

        for (int i = 0; i < numActorChannels; i++)
        {
            if ((*activeChannels)(i) == 0)
            {
                theChannel = actorChannels[i];
                i = numActorChannels;
                numActiveChannels++;
                (*activeChannels)(i) = 1;
            }
        }
    }

    // if no available connection established .. establish a new one
    if (theChannel == 0)
    {

        theChannel = this->getRemoteProcess();

        if (theChannel == 0)
        {
            cerr << "MachineBroker::startActor() - no available channel available\n";
            return 0;
        }

        Channel **nextChannels = new Channel *[numActorChannels + 1];
        ID *nextChannelID = new ID(numActorChannels + 1);

        for (int i = 0; i < numActorChannels; i++)
        {
            nextChannels[i] = actorChannels[i];
            (*nextChannelID)(i) = (*activeChannels)(i);
        }

        nextChannels[numActorChannels] = theChannel;
        (*nextChannelID)(numActorChannels) = 0;

        // clean up old memory
        if (actorChannels != 0)
        {
            delete [] actorChannels;
            delete activeChannels;
        }

        // reset the arrays
        actorChannels = nextChannels;
        activeChannels = nextChannelID;
        numActorChannels++;
        numActiveChannels++;
    }

    // now that we have a channel to a machine broker waiting to run some actor processes, start the actor
    ID idData(1);
    idData(0) = actorType;

    if (theChannel->sendID(0, 0, idData) != 0)
    {
        cerr << "MachineBroker::startActor() - failed to send actorType\n";
        this->freeProcess(theChannel);
        return 0;
    }

    if (theChannel->receiveID(0, 0, idData) != 0)
    {
        cerr << "MachineBroker::startActor() - remote process failure\n";
        return 0;
    }

    if (idData(0) != 0)
    {
        cerr << "MachineBroker::startActor() - could not start actorType: " << actorType << endln;
        this->freeProcess(theChannel);
        return 0;
    }

    return theChannel;

}


int
MachineBroker::finishedWithActor(Channel *theChannel)
{
    // send the termination notice to all machineBrokers running actorProcesses
    for (int i = 0; i < numActorChannels; i++)
    {
        if (theChannel == actorChannels[i])
        {
            numActiveChannels--;
            (*activeChannels)(i) = 0;
            return 0;
        }
    }

    // if get here .. startActor() not called or subclass override
    return -1;
}
