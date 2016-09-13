// File: ~/system_of_eqn/eigenSOE/EigenSOE.C
//
// Written: Jun Peng
// Created: Sat Feb. 6, 1999
// Revision: A
//
// Description: This file contains the class definition of EigenSOE.
// EigenSOE is a subclass of MovableObject.
// It has pure virtual functions which must be implemented in it's derived
// subclasses. To solve the genreal eigen value equations means that
// by the given K and M, find the corresponding eigen value and eigen
// vectors.
//
// This class is inheritanted from the base class of MovableObject
// which was created by fmk (Frank).


#include <EigenSOE.h>
#include <EigenSolver.h>
#include <iostream>
using namespace std;


EigenSOE::EigenSOE(EigenSolver& theEigenSolver, int classTag)
    : MovableObject(classTag), theSolver(&theEigenSolver)
{

}


EigenSOE::EigenSOE(int classTag)
    : MovableObject(classTag), theSolver(0)
{

}


EigenSOE::~EigenSOE()
{
    delete theSolver;
}

int
EigenSOE::solve(int numModes)
{
    if (theSolver == 0)
    {
        return -1;
    }
    else
    {
        return (theSolver->solve(numModes));
    }
}

// int
// EigenSOE::solve(void)
// {
//     cerr << "ERROR EigenSOE::solve(void) - need to specify numModes\n";
//     return -1;
// }

int
EigenSOE::setSolver(EigenSolver& newSolver)
{
    theSolver = &newSolver;
    return 0;
}

EigenSolver*
EigenSOE::getSolver(void)
{
    return theSolver;
}

const Vector&
EigenSOE::getEigenvector(int mode)
{
    return theSolver->getEigenvector(mode);
}

double
EigenSOE::getEigenvalue(int mode)
{
    return theSolver->getEigenvalue(mode);
}


// Nima Tafazzoli added for eigen analysis, June 2012
int
EigenSOE::setLinks(AnalysisModel& theModel)
{
    return 0;
}
