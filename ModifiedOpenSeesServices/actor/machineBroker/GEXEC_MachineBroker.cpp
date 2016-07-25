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


#include <GEXEC_MachineBroker.h>
#include <TCP_Socket.h>
#include <ID.h>
#include <OPS_Globals.h>
#include <iostream>
using namespace std;


GEXEC_MachineBroker::GEXEC_MachineBroker(FEM_ObjectBroker* theBroker,
        int argc,
        char** argv)
    : MachineBroker(theBroker)
{
    // get the enviroment variables which define process groups
    const char* pid = getenv("GEXEC_MY_VNN");
    const char* addr = getenv("GEXEC_SVRS");
    const char* NP = getenv("GEXEC_NPROCS");

    if (pid != 0)
    {
        fprintf(stderr, "%s\n", pid);
        fprintf(stderr, "%s\n", addr);
        fprintf(stderr, "%s\n", NP);
    }

    rank = atoi(pid);
    size = atoi(NP);

    // create the TCP_Sockets used for the communication
    theChannels = new TCP_Socket *[size];

    for (int i = 0; i < size; i++)
    {
        theChannels[i] = new TCP_Socket();
    }

    usedChannels = new ID(rank);
    usedChannels->Zero();
}


GEXEC_MachineBroker::~GEXEC_MachineBroker()
{
    for (int i = 0; i < size; i++)
    {
        delete theChannels[i];
    }

    delete [] theChannels;
    delete usedChannels;
}


int
GEXEC_MachineBroker::getPID(void)
{
    return rank;
}


int
GEXEC_MachineBroker::getNP(void)
{
    return rank;
}



Channel*
GEXEC_MachineBroker::getMyChannel(void)
{
    return theChannels[rank];
}



Channel*
GEXEC_MachineBroker::getRemoteProcess(void)
{
    if (rank != 0)
    {
        cerr << "GEXEC_MachineBroker::getRemoteProcess() - child process cannot not yet allocate processes\n";
        return 0;
    }

    for (int i = 0; i < size; i++)
        if (i != rank)
            if ((*usedChannels)(i) != 0)
            {
                (*usedChannels)(i) = 1;
                return theChannels[i];
            }

    // no processes available
    return 0;
}



int
GEXEC_MachineBroker::freeProcess
(Channel* theChannel)
{
    for (int i = 0; i < size; i++)
        if (i != rank)
            if (theChannels[i] == theChannel)
            {
                (*usedChannels)(i) = 0;
                return 0;
            }

    // channel not found!
    return -1;
}

