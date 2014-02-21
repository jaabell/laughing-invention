/* ****************************************************************** **
**    Opines - Open System for Earthquake Engineering Simulation      **
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
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.cpp,v $


// File: ~/system_of_eqn/linearSOE/umfGEN/UmfpackGenLinSolver.C
//
// Written: fmk
// Created: 11/98
// Revision: A
//
// Description: This file contains the class definition for
// UmfpackGenLinSolver. It solves the UmfpackGenLinSOEobject by calling
// UMFPACK2.2.1 routines.
//
// What: "@(#) UmfpackGenLinSolver.C, revA"

#include <UmfpackGenLinSOE.h>
#include <UmfpackGenLinSolver.h>
#include <f2c.h>
#include <math.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


extern "C" {
#include <umfpack.h>
}


UmfpackGenLinSolver::
UmfpackGenLinSolver()
    : LinearSOESolver(SOLVER_TAGS_UmfpackGenLinSolver),
      Control(0), Info(0), Symbolic(0), Numeric(0), theSOE(0)
{
    Control = new double[UMFPACK_CONTROL];
    Info = new double[UMFPACK_INFO];

    umfpack_defaults(Control);
}


UmfpackGenLinSolver::~UmfpackGenLinSolver()
{
    if (Symbolic != 0)
    {
        umfpack_free_symbolic (&Symbolic) ;
    }

    if (Numeric != 0)
    {
        umfpack_free_numeric (&Numeric) ;
    }

    if (Control != 0)
    {
        delete [] Control;
    }

    if (Info != 0)
    {
        delete [] Info;
    }
}

int
UmfpackGenLinSolver::solve(void)
{
    if (theSOE == 0)
    {
        cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
        cerr << " No LinearSOE object has been set\n";
        return -1;
    }

    int n = theSOE->size;

    // check for quick return
    if (n == 0)
    {
        return 0;
    }

    // first copy B into X
    double* Xptr = theSOE->X;
    double* Bptr = theSOE->B;
    double* Aptr = theSOE->A;

    int* Ap = theSOE->rowStartA;
    int* Ai = theSOE->colA;
    int status = 0;

    if (theSOE->factored == false)
    {

        // factor the matrix
        status =  umfpack_numeric (Ap, Ai, Aptr, Symbolic, &Numeric, Control, Info) ;

        if (status != UMFPACK_OK)
        {
            cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
            cerr << status << " returned returned in umfpack_numeric()\n";
            return -status;
        }

        theSOE->factored = true;
    }

    // do forward and backward substitution
    status =  umfpack_solve ("Ax=b", Ap, Ai, Aptr, Xptr, Bptr, Numeric, Control, Info) ;

    if (status != UMFPACK_OK)
    {
        cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
        cerr << status << " returned in umpack_solve()\n";
        return -status;
    }

    return 0;
}


int
UmfpackGenLinSolver::setSize()
{
    int n = theSOE->size;
    int* Ap = theSOE->rowStartA;
    int* Ai = theSOE->colA;

    if (n > 0)
    {

        // do the symbolic factorization
        int status =  umfpack_symbolic (n, Ap, Ai, &Symbolic, Control, Info) ;

        if (status != status)
        {
            cerr << "WARNING UmfpackGenLinSolver::setSize(void)- ";
            cerr << status << " returned in umfpack_symbolic()\n";
            return -status;
        }
    }

    return 0;
}

int
UmfpackGenLinSolver::setLinearSOE(UmfpackGenLinSOE& theLinearSOE)
{
    theSOE = &theLinearSOE;
    return 0;
}

int
UmfpackGenLinSolver::sendSelf(int cTag, Channel& theChannel)
{
    // nothing to do
    return 0;
}

int
UmfpackGenLinSolver::recvSelf(int ctag,
                              Channel& theChannel,
                              FEM_ObjectBroker& theBroker)
{
    // nothing to do
    return 0;
}

