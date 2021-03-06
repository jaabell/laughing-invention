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
// $Date: 2001/07/20 22:36:37 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/LinearSOE.cpp,v $


// File: ~/system_of_eqn/LinearSOE.C
//
// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the implementation of LinearSOE.
//
// What: "@(#) LinearSOE.C, revA"

#include <LinearSOE.h>
#include <LinearSOESolver.h>
#include <iostream>

using namespace std;

LinearSOE::LinearSOE(LinearSOESolver& theLinearSOESolver, int classtag)
    : SystemOfEqn(classtag), theSolver(&theLinearSOESolver)
{

}

LinearSOE::~LinearSOE()
{
    delete theSolver;
}

int
LinearSOE::solve(void)
{
    return (theSolver->solve());
}


double
LinearSOE::getDeterminant(void)
{
    return theSolver->getDeterminant();
}



int
LinearSOE::setSolver(LinearSOESolver& newSolver)
{
    theSolver = &newSolver;
    return 0;
}

LinearSOESolver*
LinearSOE::getSolver(void)
{
    return theSolver;
}


// Added default versions of these (Jose Abell, Fri 31 Jan 2014 04:14:52 PM PST)
// Must overload the one you want to use
// # ifdef _PARALLEL_PROCESSING
int LinearSOE::setSize(int MaxDOFtag)
{
    cerr << "LinearSOE::setSize(int MaxDOFtag) -- this is the base class virtual function. " << endl;
    cerr << "                      It is the subclass's responsability to implement. " << endl;
    return -1;
}

// #else
int LinearSOE::setSize(Graph& theGraph)
{
    cerr << "LinearSOE::setSize(Graph &theGraph) -- this is the base class virtual function. " << endl;
    cerr << "                      It is the subclass's responsability to implement. " << endl;
    return -1;
}
// #endif



