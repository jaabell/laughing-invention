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
// $Date: 2004/11/24 22:41:22 $
// $Source: /usr/local/cvs/OpenSees/SRC/recorder/Recorder.cpp,v $


// Written: fmk
// Created: 11/98
// Revision: A
//
// Description: This file contains the class implementation for Recorder.
//
// What: "@(#) Recorder.cpp, revA"

#include <Recorder.h>
#include <OPS_Globals.h>
#include <iostream>
using namespace std;


int Recorder::lastRecorderTag(0);

Recorder::Recorder(int classTag)
    : MovableObject(classTag), TaggedObject(lastRecorderTag)
{
    lastRecorderTag++;
}

Recorder::~Recorder()
{

}

int
Recorder::restart(void)
{
    return 0;
}

int
Recorder::setDomain(Domain& theDomain)
{
    return 0;
}

int
Recorder::sendSelf(int commitTag, Channel& theChannel)
{
    cerr << "Recorder::sendSelf() - not yet implemented\n";
    return 0;
}

int
Recorder::recvSelf(int commitTag, Channel& theChannel,
                   FEM_ObjectBroker& theBroker)
{
    cerr << "Recorder::recvSelf() - not yet implemented\n";
    return 0;
}

void
Recorder::Print(ostream& s, int flag)
{
    return;
}


int Recorder::initialize(void)
{
    return 0;   //Guanzhou added
}

