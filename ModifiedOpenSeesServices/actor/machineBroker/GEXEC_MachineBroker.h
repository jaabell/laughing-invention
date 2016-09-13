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
// Purpose: This file contains the class definition for GEXEC_MachineBroker.
// GEXEC_MachineBroker is the broker responsible for monitoring the usage of
// the processes in a gexec run.
//
// What: "@(#) GEXEC_MachineBroker.h, revA"

#ifndef GEXEC_MachineBroker_h
#define GEXEC_MachineBroker_h

#include <MachineBroker.h>
class ID;
class TCP_Socket;

class GEXEC_MachineBroker : public MachineBroker
{
    public:
        GEXEC_MachineBroker(FEM_ObjectBroker* theBroker, int argc, char** argv);
        ~GEXEC_MachineBroker();

        // methods to return info about local process id and num processes
        int getPID(void);
        int getNP(void);

        // methods to get and free Channels (processes)
        Channel* getMyChannel(void);
        Channel* getRemoteProcess(void);
        int freeProcess(Channel*);

    protected:

    private:
        int rank;
        int size;
        ID* usedChannels;
        TCP_Socket** theChannels;
};

#endif
