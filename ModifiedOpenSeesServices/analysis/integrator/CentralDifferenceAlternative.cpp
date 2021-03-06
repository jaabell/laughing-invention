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
// $Date: 2005/01/27 04:32:07 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/integrator/CentralDifferenceAlternative.cpp,v $

// Written: fmk
// Created: 11/98
// Revision: A
//
// Description: This file contains the implementation of the CentralDifferenceAlternative
// class.
//
// What: "@(#) CentralDifferenceAlternative.C, revA"

#include <CentralDifferenceAlternative.h>
#include <FE_Element.h>
#include <LinearSOE.h>
#include <AnalysisModel.h>
#include <Vector.h>
#include <DOF_Group.h>
#include <DOF_GrpIter.h>
#include <AnalysisModel.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


CentralDifferenceAlternative::CentralDifferenceAlternative()
    : TransientIntegrator(INTEGRATOR_TAGS_CentralDifferenceAlternative),
      updateCount(0), Ut(0), Utp1(0),  Udot(0), deltaT(0)
{

}

CentralDifferenceAlternative::~CentralDifferenceAlternative()
{
    // clean up the memory created
    if (Ut != 0)
    {
        delete Ut;
    }

    if (Utp1 != 0)
    {
        delete Utp1;
    }

    if (Udot != 0)
    {
        delete Udot;
    }
}

int
CentralDifferenceAlternative::newStep(double _deltaT)
{
    updateCount = 0;

    deltaT = _deltaT;

    if (deltaT <= 0.0)
    {
        cerr << "CentralDifference::newStep() - error in variable\n";
        cerr << "dT = " << deltaT << endln;
        return -2;
    }

    AnalysisModel* theModel = this->getAnalysisModelPtr();
    double time = theModel->getCurrentDomainTime();
    theModel->applyLoadDomain(time);

    return 0;
}

int
CentralDifferenceAlternative::formEleTangent(FE_Element* theEle)
{
    theEle->zeroTangent();
    theEle->addMtoTang();

    return 0;
}

int
CentralDifferenceAlternative::formNodTangent(DOF_Group* theDof)
{
    theDof->zeroTangent();
    theDof->addMtoTang();
    return(0);
}

int
CentralDifferenceAlternative::domainChanged()
{
    AnalysisModel* myModel = this->getAnalysisModelPtr();
    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    const Vector& x = theLinSOE->getX();
    int size = x.Size();

    // create the new Vector objects
    if (Ut == 0 || Ut->Size() != size)
    {

        // delete the old
        if (Ut != 0)
        {
            delete Ut;
        }

        if (Utp1 != 0)
        {
            delete Utp1;
        }

        if (Udot != 0)
        {
            delete Udot;
        }

        // create the new
        Ut = new Vector(size);
        Utp1 = new Vector(size);
        Udot = new Vector(size);

        // cheack we obtained the new
        if (Ut == 0 || Ut->Size() != size ||
                Utp1 == 0 || Utp1->Size() != size ||
                Udot == 0 || Udot->Size() != size)
        {

            cerr << "CentralDifferenceAlternative::domainChanged - ran out of memory\n";

            // delete the old
            if (Ut != 0)
            {
                delete Ut;
            }

            if (Utp1 != 0)
            {
                delete Utp1;
            }

            if (Udot != 0)
            {
                delete Udot;
            }

            Ut = 0;
            Utp1 = 0;
            Udot = 0;
            return -1;
        }
    }

    // now go through and populate U and Udot by iterating through
    // the DOF_Groups and getting the last committed velocity and accel

    DOF_GrpIter& theDOFs = myModel->getDOFs();
    DOF_Group* dofPtr;

    while ((dofPtr = theDOFs()) != 0)
    {
        const Vector& disp = dofPtr->getCommittedDisp();
        const Vector& vel = dofPtr->getCommittedVel();
        const ID& id = dofPtr->getID();

        for (int i = 0; i < id.Size(); i++)
        {
            int loc = id(i);

            if (loc >= 0)
            {
                (*Ut)(loc) = disp(i);
                (*Udot)(loc) = vel(i);
            }
        }
    }

    return 0;
}


int
CentralDifferenceAlternative::update(const Vector& X)
{
    updateCount++;

    if (updateCount > 1)
    {
        cerr << "ERROR CentralDifferenceAlternative::update() - called more than once -";
        cerr << " Central Difference integraion schemes require a LINEAR solution algorithm\n";
        return -1;
    }

    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel == 0)
    {
        cerr << "ERROR CentralDifferenceAlternative::update() - no AnalysisModel set\n";
        return -2;
    }

    // check domainChanged() has been called, i.e. Ut will not be zero
    if (Ut == 0)
    {
        cerr << "WARNING CentralDifferenceAlternative::update() - domainChange() failed or not called\n";
        return -2;
    }

    // check deltaU is of correct size
    if (X.Size() != Ut->Size())
    {
        cerr << "WARNING CentralDifferenceAlternative::update() - Vectors of incompatable size ";
        cerr << " expecting " << Ut->Size() << " obtained " << X.Size() << endln;
        return -3;
    }


    //  determine the displacement at t+delta t
    Utp1->addVector(0.0, X, deltaT * deltaT);
    (*Utp1) += *Ut;
    Utp1->addVector(1.0, *Udot, deltaT);

    //  determine the vel at t+ 0.5 * delta t
    (*Udot) =  *Utp1;
    (*Udot) -= *Ut;
    (*Udot) *= (1.0 / deltaT);

    // update the disp & responses at the DOFs
    theModel->setDisp(*Utp1);
    theModel->setVel(*Udot);
    theModel->updateDomain();

    return 0;
}

int
CentralDifferenceAlternative::commit(void)
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel == 0)
    {
        cerr << "WARNING CentralDifferenceAlternative::commit() - no AnalysisModel set\n";
        return -1;
    }

    *Ut = *Utp1;

    // update time in Domain to T + deltaT & commit the domain
    double time = theModel->getCurrentDomainTime() + deltaT;
    theModel->setCurrentDomainTime(time);

    return theModel->commitDomain();

    return 0;
}

int
CentralDifferenceAlternative::sendSelf(int cTag, Channel& theChannel)
{
    return 0;
}

int
CentralDifferenceAlternative::receiveSelf(int cTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}

void
CentralDifferenceAlternative::Print(ostream& s, int flag)
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel != 0)
    {
        double currentTime = theModel->getCurrentDomainTime();
        s << "\t CentralDifferenceAlternative - currentTime: " << currentTime;
    }
    else
    {
        s << "\t CentralDifferenceAlternative - no associated AnalysisModel\n";
    }
}

