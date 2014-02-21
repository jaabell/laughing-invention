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

// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:30 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.cpp,v $


// File: ~/system_of_eqn/linearSOE/profileSPD/ProfileSPDLinSolver.C
//
// Written: fmk
// Created: Tue Sep 26 16:27:47: 1996
// Revision: A
//
// Description: This file contains the class definition for ProfileSPDLinSolver.
// ProfileSPDLinSolver is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes.  Instances of ProfileSPDLinSolver
// are used to solve a system of equations of type ProfileSPDLinSOE.
//
// What: "@(#) ProfileSPDLinSolver.C, revA"

#include <ProfileSPDLinSolver.h>
#include <ProfileSPDLinSOE.h>

ProfileSPDLinSolver::ProfileSPDLinSolver(int theClassTag)
    : LinearSOESolver(theClassTag),
      theSOE(0)
{

}

ProfileSPDLinSolver::~ProfileSPDLinSolver()
{

}

int
ProfileSPDLinSolver::setLinearSOE(ProfileSPDLinSOE& theProfileSPDSOE)
{
    theSOE = &theProfileSPDSOE;
    return 0;
}

