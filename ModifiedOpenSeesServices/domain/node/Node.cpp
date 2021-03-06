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

// $Revision: 1.16 $
// $Date: 2003/10/30 22:38:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/node/Node.cpp,v $


// Written: fmk
// Created: 11/96
// Revision: A
//
// This file contains the implementation of the Node class
//
// What: "@(#) Node.h, revA"

#include <Node.h>
#include <stdlib.h>

#include <Element.h>
#include <Vector.h>
#include <Matrix.h>
#include <Channel.h>
// #include <HDF5_Channel.h>
#include <FEM_ObjectBroker.h>
#include <DOF_Group.h>
#include <string.h>
#include <Information.h>

#include <Domain.h>
#include <Element.h>
#include <ElementIter.h>

#include <OPS_Globals.h>

#include <iostream>
using namespace std;


// Matrix **Node::theMatrices = 0;
// int Node::numMatrices = 0;

// for FEM_Object Broker to use
Node::Node(int theClassTag)
    : DomainComponent(0, theClassTag),
      numberDOF(0), theDOF_GroupPtr(0),
      Crd(0), commitDisp(0), commitVel(0), commitAccel(0),
      trialDisp(0), trialVel(0), trialAccel(0), unbalLoad(0), incrDisp(0),
      incrDeltaDisp(0),
      disp(0), vel(0), accel(0),
      R(0), mass(0), unbalLoadWithInertia(0), a0(0.0), theEigenvectors(0), sizedMatrix(0), reaction(0),
      penalty_load(0)
{
    // for FEM_ObjectBroker, receiveSelf() must be invoked on object

}


Node::Node(int tag, int theClassTag)
    : DomainComponent(tag, theClassTag),
      numberDOF(0), theDOF_GroupPtr(0),
      Crd(0), commitDisp(0), commitVel(0), commitAccel(0),
      trialDisp(0), trialVel(0), trialAccel(0), unbalLoad(0), incrDisp(0),
      incrDeltaDisp(0),
      disp(0), vel(0), accel(0),
      R(0), mass(0), unbalLoadWithInertia(0), a0(0.0), theEigenvectors(0), sizedMatrix(0), reaction(0),
      penalty_load(0)
{
    // for subclasses - they must implement all the methods with
    // their own data structures.


}



Node::Node(int tag, int ndof, double Crd1, double Crd2, double Crd3)
    : DomainComponent(tag, NOD_TAG_Node),
      numberDOF(ndof), theDOF_GroupPtr(0),
      Crd(0), commitDisp(0), commitVel(0), commitAccel(0),
      trialDisp(0), trialVel(0), trialAccel(0), unbalLoad(0), incrDisp(0),
      incrDeltaDisp(0),
      disp(0), vel(0), accel(0),
      R(0), unbalLoadWithInertia(0), a0(0.0), theEigenvectors(0), sizedMatrix(0), reaction(0),
      penalty_load(0)
{


    Crd = new Vector(3);
    (*Crd)(0) = Crd1;
    (*Crd)(1) = Crd2;
    (*Crd)(2) = Crd3;


    mass = new Matrix(numberDOF, numberDOF);
    mass->Zero();

    sizedMatrix = new Matrix(numberDOF, numberDOF);


    // should be added to the sendseld/recvself functions later
    penalty_load = new Vector(numberDOF);



}


// used for domain decomposition & external nodes
//  copy everything but the mass
//  we should really set the mass to 0.0
Node::Node(const Node &otherNode, bool copyMass)
    : DomainComponent(otherNode.getTag(), NOD_TAG_Node),
      numberDOF(otherNode.numberDOF), theDOF_GroupPtr(0),
      Crd(0), commitDisp(0), commitVel(0), commitAccel(0),
      trialDisp(0), trialVel(0), trialAccel(0), unbalLoad(0), incrDisp(0),
      incrDeltaDisp(0),
      disp(0), vel(0), accel(0),
      R(0), mass(0), unbalLoadWithInertia(0), a0(0.0), theEigenvectors(0), sizedMatrix(0), reaction(0)
{


    Crd = new Vector(otherNode.getCrds());

    if (Crd == 0)
    {
        cerr << " FATAL Node::Node(node *) - ran out of memory for Crd\n";
        exit(-1);
    }

    if (otherNode.commitDisp != 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for displacement\n";
            exit(-1);
        }

        for (int i = 0; i < 4 * numberDOF; i++)
        {
            disp[i] = otherNode.disp[i];
        }
    }

    if (otherNode.commitVel != 0)
    {
        if (this->createVel() < 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for velocity\n";
            exit(-1);
        }

        for (int i = 0; i < 2 * numberDOF; i++)
        {
            vel[i] = otherNode.vel[i];
        }
    }

    if (otherNode.commitAccel != 0)
    {
        if (this->createAccel() < 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for acceleration\n";
            exit(-1);
        }

        for (int i = 0; i < 2 * numberDOF; i++)
        {
            accel[i] = otherNode.accel[i];
        }
    }


    if (otherNode.unbalLoad != 0)
    {
        unbalLoad = new Vector(*(otherNode.unbalLoad));

        if (unbalLoad == 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for Load\n";
            exit(-1);
        }

        //GZ out unbalLoad->Zero();
    }

    if (otherNode.mass != 0 && copyMass == true)
    {
        mass = new Matrix(*(otherNode.mass)) ;

        if (mass == 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for mass\n";
            exit(-1);
        }
    }

    if (otherNode.R != 0)
    {
        R = new Matrix(*(otherNode.R));

        if (R == 0)
        {
            cerr << " FATAL Node::Node(node *) - ran out of memory for R\n";
            exit(-1);
        }
    }


}


Node::~Node()
{
    // delete anything that we created with new
    if (Crd != 0)
    {
        delete Crd;
    }

    if (commitDisp != 0)
    {
        delete commitDisp;
    }

    if (commitVel != 0)
    {
        delete commitVel;
    }

    if (commitAccel != 0)
    {
        delete commitAccel;
    }

    if (trialDisp != 0)
    {
        delete trialDisp;
    }

    if (trialVel != 0)
    {
        delete trialVel;
    }

    if (trialAccel != 0)
    {
        delete trialAccel;
    }

    if (incrDisp != 0)
    {
        delete incrDisp;
    }

    if (incrDeltaDisp != 0)
    {
        delete incrDeltaDisp;
    }

    if (unbalLoad != 0)
    {
        delete unbalLoad;
    }

    if (disp != 0)
    {
        delete [] disp;
    }

    if (vel != 0)
    {
        delete [] vel;
    }

    if (accel != 0)
    {
        delete [] accel;
    }

    if (mass != 0)
    {
        delete mass;
    }

    if (R != 0)
    {
        delete R;
    }

    if (unbalLoadWithInertia != 0)
    {
        delete unbalLoadWithInertia;
    }

    if (theEigenvectors != 0)
    {
        delete theEigenvectors;
    }

    //Guanzhou added
    if ( theDOF_GroupPtr != 0)
    {
        theDOF_GroupPtr->unSetMyNode();
        delete theDOF_GroupPtr;
    }


    if (reaction != 0)
    {
        delete reaction;
    }

    if (penalty_load != 0)
    {
        delete penalty_load;
    }

    if (sizedMatrix != 0)
    {
        delete sizedMatrix;
    }


}


int
Node::getNumberDOF(void) const
{
    // return the number of dof
    return  numberDOF;
}


void
Node::setDOF_GroupPtr(DOF_Group *theDOF_Grp)
{
    // set the DOF_Group pointer
    theDOF_GroupPtr = theDOF_Grp;
}


DOF_Group *
Node::getDOF_GroupPtr(void)
{
    // return the DOF_Group pointer
    return theDOF_GroupPtr;
}


const Vector &
Node::getCrds() const
{
    // return the vector of nodal coordinates
    return *Crd;
}




const Vector &
Node::getDisp(void)
{
    // construct memory and Vectors for trial and committed
    // displacement on first call to this method, getTrialDisp()
    // setTrialDisp() or incrTrialDisp()
    if (commitDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::getDisp() -- ran out of memory\n";
            exit(-1);
        }
    }

    // return the committed disp
    return *commitDisp;
}

const Vector &
Node::getVel(void)
{
    // construct memory and Vectors for trial and committed
    // velocity on first call to this method, getTrialVel()
    // setTrialVel() or incrTrialVel()
    if (commitVel == 0)
    {
        if (this->createVel() < 0)
        {
            cerr << "FATAL Node::getVel() -- ran out of memory\n";
            exit(-1);
        }
    }

    // return the velocity
    return *commitVel;
}


const Vector &
Node::getAccel(void)
{
    // construct memory and Vectors for trial and committed
    // accel on first call to this method, getTrialAccel()
    // setTrialAccel() or incrTrialAccel()
    if (commitAccel == 0)
    {
        if (this->createAccel() < 0)
        {
            cerr << "FATAL Node::getAccel() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *commitAccel;
}


/* *********************************************************************
**
**   Methods to return the trial response quantities similar to committed
**
** *********************************************************************/

const Vector &
Node::getTrialDisp(void)
{
    if (trialDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::getTrialDisp() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *trialDisp;
}



const Vector &
Node::getTrialVel(void)
{
    if (trialVel == 0)
    {
        if (this->createVel() < 0)
        {
            cerr << "FATAL Node::getTrialVel() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *trialVel;
}



const Vector &
Node::getTrialAccel(void)
{
    if (trialAccel == 0)
    {
        if (this->createAccel() < 0)
        {
            cerr << "FATAL Node::getTrialAccel() - ran out of memory\n";
            exit(0);
        }
    }

    return *trialAccel;
}

const Vector &
Node::getIncrDisp(void)
{
    if (incrDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::getTrialDisp() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *incrDisp;
}

const Vector &
Node::getIncrDeltaDisp(void)
{
    if (incrDeltaDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::getTrialDisp() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *incrDeltaDisp;
}


int
Node::setTrialDisp(double value, int dof)
{

    // check vector arg is of correct size
    if (dof < 0 || dof >=  numberDOF)
    {
        cerr << "WARNING Node::setTrialDisp() - incompatable sizes\n";
        return -2;
    }

    // construct memory and Vectors for trial and committed
    // accel on first call to this method, getTrialDisp(),
    // getDisp(), or incrTrialDisp()
    if (trialDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::setTrialDisp() - ran out of memory\n";
            exit(-1);
        }
    }

    // perform the assignment .. we dont't go through Vector interface
    // as we are sure of size and this way is quicker
    double tDisp = value;
    disp[dof + 2 * numberDOF] = tDisp - disp[dof + numberDOF];
    disp[dof + 3 * numberDOF] = tDisp - disp[dof];
    disp[dof] = tDisp;

    return 0;
}

int
Node::setTrialDisp(const Vector &newTrialDisp)
{
    // check vector arg is of correct size
    if (newTrialDisp.Size() != numberDOF)
    {
        cerr << "WARNING Node::setTrialDisp() - incompatable sizes\n";
        return -2;
    }

    // construct memory and Vectors for trial and committed
    // accel on first call to this method, getTrialDisp(),
    // getDisp(), or incrTrialDisp()
    if (trialDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::setTrialDisp() - ran out of memory\n";
            exit(-1);
        }
    }

    // perform the assignment .. we dont't go through Vector interface
    // as we are sure of size and this way is quicker
    for (int i = 0; i < numberDOF; i++)
    {
        double tDisp = newTrialDisp(i);
        disp[i + 2 * numberDOF] = tDisp - disp[i + numberDOF];
        disp[i + 3 * numberDOF] = tDisp - disp[i];
        disp[i] = tDisp;
    }

    return 0;
}

int
Node::setTrialVel(const Vector &newTrialVel)
{
    // check vector arg is of correct size
    if (newTrialVel.Size() != numberDOF)
    {
        cerr << "WARNING Node::setTrialVel() - incompatable sizes\n";
        return -2;
    }

    // construct memory and Vectors for trial and committed
    // accel on first call to this method, getTrialVEl(),
    // getVEl(), or incrTrialVel()
    if (trialVel == 0)
    {
        if (this->createVel() < 0)
        {
            cerr << "FATAL Node::setTrialVel() - ran out of memory\n";
            exit(-1);
        }
    }

    // set the trial quantities
    for (int i = 0; i < numberDOF; i++)
    {
        vel[i] = newTrialVel(i);
    }

    return 0;
}


int
Node::setTrialAccel(const Vector &newTrialAccel)
{
    // check vector arg is of correct size
    if (newTrialAccel.Size() != numberDOF)
    {
        cerr << "WARNING Node::setTrialAccel() - incompatable sizes\n";
        return -2;
    }

    // create a copy if no trial exists
    if (trialAccel == 0)
    {
        if (this->createAccel() < 0)
        {
            cerr << "FATAL Node::setTrialAccel() - ran out of memory\n";
            exit(-1);
        }
    }

    // use vector assignment otherwise
    for (int i = 0; i < numberDOF; i++)
    {
        accel[i] = newTrialAccel(i);
    }

    return 0;
}

int
Node::incrTrialDisp(const Vector &incrDispl)
{
    // check vector arg is of correct size
    if (incrDispl.Size() != numberDOF)
    {
        cerr << "WARNING Node::incrTrialDisp() - incompatable sizes\n";
        return -2;
    }

    // create a copy if no trial exists andd add committed
    if (trialDisp == 0)
    {
        if (this->createDisp() < 0)
        {
            cerr << "FATAL Node::incrTrialDisp() - ran out of memory\n";
            exit(-1);
        }

        for (int i = 0; i < numberDOF; i++)
        {
            double incrDispI = incrDispl(i);
            disp[i] = incrDispI;
            disp[i + 2 * numberDOF] = incrDispI;
            disp[i + 3 * numberDOF] = incrDispI;
        }

        return 0;
    }

    // otherwise set trial = incr + trial
    for (int i = 0; i < numberDOF; i++)
    {
        double incrDispI = incrDispl(i);
        disp[i] += incrDispI;
        disp[i + 2 * numberDOF] += incrDispI;
        disp[i + 3 * numberDOF] = incrDispI;
    }

    //Guanzhou added
    //cerr << "Node::incrTrialDisp - Node " << this->getTag() << endln;
    //cerr << "Incr " << incrDispl << endln;
    //cerr << "Trial" << *trialDisp << endln;
    return 0;
}


int
Node::incrTrialVel(const Vector &incrVel)
{
    // check vector arg is of correct size
    if (incrVel.Size() != numberDOF)
    {
        cerr << "WARNING Node::incrTrialVel() - incompatable sizes\n";
        return -2;
    }

    // create Vectors and array if none exist and set trial
    if (trialVel == 0)
    {
        if (this->createVel() < 0)
        {
            cerr << "FATAL Node::incrTrialVel - ran out of memory\n";
            exit(-1);
        }

        for (int i = 0; i < numberDOF; i++)
        {
            vel[i] = incrVel(i);
        }

        return 0;
    }

    // otherwise set trial = incr + trial
    for (int i = 0; i < numberDOF; i++)
    {
        vel[i] += incrVel(i);
    }

    return 0;
}


int
Node::incrTrialAccel(const Vector &incrAccel)
{
    // check vector arg is of correct size
    if (incrAccel.Size() != numberDOF)
    {
        cerr << "WARNING Node::incrTrialAccel() - incompatable sizes\n";
        return -2;
    }

    // create a copy if no trial exists andd add committed
    if (trialAccel == 0)
    {
        if (this->createAccel() < 0)
        {
            cerr << "FATAL Node::incrTrialAccel() - ran out of memory\n";
            exit(-1);
        }

        for (int i = 0; i < numberDOF; i++)
        {
            accel[i] = incrAccel(i);
        }

        return 0;
    }

    // otherwise set trial = incr + trial
    for (int i = 0; i < numberDOF; i++)
    {
        accel[i] += incrAccel(i);
    }

    return 0;
}


void
Node::zeroUnbalancedLoad(void)
{
    if (unbalLoad != 0)
    {
        unbalLoad->Zero();
    }
}

int
Node::addUnbalancedLoad(const Vector &add, double fact)
{


    // check vector arg is of correct size
    if (add.Size() != numberDOF)
    {
        cerr << "Node::addunbalLoad - load to add of incorrect size ";
        cerr << add.Size() << " should be " <<  numberDOF << endln;
        return -1;
    }

    // if no load yet create it and assign
    if (unbalLoad == 0)
    {
        unbalLoad = new Vector(add);

        if (unbalLoad == 0)
        {
            cerr << "FATAL Node::addunbalLoad - ran out of memory\n";
            exit(-1);
        }

        if ( fact != 1.0 )
        {
            (*unbalLoad) *= fact;
        }

        return 0;
    }

    // add fact*add to the unbalanced load
    unbalLoad->addVector(1.0, add, fact);

    return 0;
}



int
Node::addInertiaLoadToUnbalance(const Vector &accelG, double fact)
{
    // simply return if node has no mass or R matrix
    if (mass == 0 || R == 0)
    {
        return 0;
    }

    // otherwise we must determine MR accelG
    if (accelG.Size() != R->noCols())
    {
        cerr << "Node::addInertiaLoadToUnbalance - accelG not of correct dimension";
        return -1;
    }

    // if no load yet create it and assign
    if (unbalLoad == 0)
    {
        unbalLoad = new Vector(numberDOF);

        if (unbalLoad == 0 || unbalLoad->Size() != numberDOF)
        {
            cerr << "FATAL Node::addunbalLoad - ran out of memory\n";
            exit(-1);
        }
    }

    // form - fact * M*R*accelG and add it to the unbalanced load
    //(*unbalLoad) -= ((*mass) * (*R) * accelG)*fact;

    Matrix MR(mass->noRows(), R->noCols());
    MR.addMatrixProduct(0.0, *mass, *R, 1.0);
    unbalLoad->addMatrixVector(1.0, MR, accelG, -fact);

    return 0;
}




const Vector &
Node::getUnbalancedLoad(void)
{
    // make sure it was created before we return it
    if (unbalLoad == 0)
    {
        unbalLoad = new Vector(numberDOF);

        if (unbalLoad == 0 || unbalLoad->Size() != numberDOF)
        {
            cerr << "FATAL Node::getunbalLoad() -- ran out of memory\n";
            exit(-1);
        }
    }

    return *unbalLoad;
}



const Vector &
Node::getUnbalancedLoadIncInertia(void)
{
    // make sure it was created before we return it
    if (unbalLoadWithInertia == 0)
    {
        unbalLoadWithInertia = new Vector(this->getUnbalancedLoad());

        if (unbalLoadWithInertia == 0)
        {
            cerr << "FATAL Node::getunbalLoadWithInertia -- ran out of memory\n";
            exit(-1);
        }
    }
    else
    {
        (*unbalLoadWithInertia) = this->getUnbalancedLoad();
    }

    if (mass != 0)
    {
        const Vector &theAccel = this->getTrialAccel(); // in case accel not created
        unbalLoadWithInertia->addMatrixVector(1.0, *mass, theAccel, -1.0);

        if (a0 != 0.0)
        {
            const Vector &theVel = this->getTrialVel(); // in case accel not created
            unbalLoadWithInertia->addMatrixVector(1.0, *mass, theVel, -a0);
        }
    }

    return *unbalLoadWithInertia;
}



int
Node::commitState()
{
    // check disp exists, if does set commit = trial, incr = 0.0
    if (trialDisp != 0)
    {
        for (int i = 0; i < numberDOF; i++)
        {
            disp[i + numberDOF] = disp[i];
            disp[i + 2 * numberDOF] = 0.0;
            disp[i + 3 * numberDOF] = 0.0;
        }
    }

    // check vel exists, if does set commit = trial
    if (trialVel != 0)
    {
        for (int i = 0; i < numberDOF; i++)
        {
            vel[i + numberDOF] = vel[i];
        }
    }

    // check accel exists, if does set commit = trial
    if (trialAccel != 0)
    {
        for (int i = 0; i < numberDOF; i++)
        {
            accel[i + numberDOF] = accel[i];
        }
    }

    // if we get here we are done
    return 0;
}



int
Node::revertToLastCommit()
{
    // check disp exists, if does set trial = last commit, incr = 0
    if (disp != 0)
    {
        for (int i = 0 ; i < numberDOF; i++)
        {
            disp[i] = disp[i + numberDOF];
            disp[i + 2 * numberDOF] = 0.0;
            disp[i + 3 * numberDOF] = 0.0;
        }
    }

    // check vel exists, if does set trial = last commit
    if (vel != 0)
    {
        for (int i = 0 ; i < numberDOF; i++)
        {
            vel[i] = vel[numberDOF + i];
        }
    }

    // check accel exists, if does set trial = last commit
    if (accel != 0)
    {
        for (int i = 0 ; i < numberDOF; i++)
        {
            accel[i] = accel[numberDOF + i];
        }
    }

    // if we get here we are done
    return 0;
}


int
Node::revertToStart()
{
    // check disp exists, if does set all to zero
    if (disp != 0)
    {
        for (int i = 0 ; i < 4 * numberDOF; i++)
        {
            disp[i] = 0.0;
        }
    }

    // check vel exists, if does set all to zero
    if (vel != 0)
    {
        for (int i = 0 ; i < 2 * numberDOF; i++)
        {
            vel[i] = 0.0;
        }
    }

    // check accel exists, if does set all to zero
    if (accel != 0)
    {
        for (int i = 0 ; i < 2 * numberDOF; i++)
        {
            accel[i] = 0.0;
        }
    }

    if (unbalLoad != 0)
    {
        (*unbalLoad) *= 0;
    }



    // if we get here we are done
    return 0;
}


const Matrix &
Node::getMass(void)
{
    // make sure it was created before we return it
    //     if (mass == 0)
    //     {
    //       theMatrices[index]->Zero();
    //       return *theMatrices[index];
    //     } else
    return *mass;
}


int
Node::setDampingFactor(double A0)
{
    a0 = A0;
    return 0;
}




const Matrix &
Node::getDamp(void)
{

    // make sure it was created before we return it
    if (a0 == 0.0)
    {
        sizedMatrix->Zero();
    }
    else
    {

        *sizedMatrix = *mass;

        (*sizedMatrix) *= a0;

        //      cerr << *sizedMatrix;

    }

    return *sizedMatrix;

}




int
Node::setMass(const Matrix &newMass)
{
    // check right size
    if (newMass.noRows() != numberDOF || newMass.noCols() != numberDOF)
    {
        cerr << "Node::setMass - incompatable matrices\n";
        return -1;
    }

    // create a matrix if no mass yet set
    if (mass == 0)
    {
        mass = new Matrix(newMass);

        if (mass == 0 || mass->noRows() != numberDOF)
        {
            cerr << "FATAL Node::setMass - ran out of memory\n";
            return -1;
        }

        return 0;
    }

    // assign if mass has already been created
    (*mass) = newMass;


    return 0;
}



int
Node::setNumColR(int numCol)
{
    if (R != 0)
    {
        if (R->noCols() != numCol)
        {
            delete R;
            R = new Matrix(numberDOF, numCol);
        }
    }
    else
    {
        R = new Matrix(numberDOF, numCol);
    }

    if (R == 0 || R->noRows() != numberDOF)
    {
        cerr << "FATAL Node::setNumColR() - out of memory\n";
        exit(-1);
    }

    R->Zero();
    return 0;
}

int
Node::setR(int row, int col, double Value)
{
    // ensure R had been set
    if (R == 0)
    {
        cerr << "Node:setR() - R has not been initialised\n";
        return -1;
    }

    // ensure row, col in range (matrix assignment will catch this - extra work)
    if (row < 0 || row > numberDOF || col < 0 || col > R->noCols())
    {
        cerr << "Node:setR() - row, col index out of range\n";
        return -1;
    }

    // do the assignment
    (*R)(row, col) = Value;
    return 0;
}



const Vector &
Node::getRV(const Vector &V)
{
    // we store the product of RV in unbalLoadWithInertia

    // make sure unbalLoadWithInertia was created, if not create it
    if (unbalLoadWithInertia == 0)
    {
        unbalLoadWithInertia = new Vector(numberDOF);

        if (unbalLoadWithInertia == 0)
        {
            cerr << "Node::getunbalLoadWithInertia -- ran out of memory\n";
            exit(-1);
        }
    }

    // see if quick return , i.e. R == 0
    if (R == 0)
    {
        unbalLoadWithInertia->Zero();
        return *unbalLoadWithInertia;
    }

    // check dimesions of R and V
    if (R->noCols() != V.Size())
    {
        cerr << "WARNING Node::getRV() - R and V of incompatable dimesions\n";
        //  cerr << "R: " << *R << "V: " << V;
        unbalLoadWithInertia->Zero();
        return *unbalLoadWithInertia;
    }

    // determine the product
    unbalLoadWithInertia->addMatrixVector(0.0, *R, V, 1.0);
    return *unbalLoadWithInertia;
}


int
Node::setNumEigenvectors(int numVectorsToStore)
{
    // ensure a positive number of vectors
    if (numVectorsToStore <= 0)
    {
        cerr << "Node::setNumEigenvectors() - " << numVectorsToStore << " < 0\n";
        return -1;
    }

    // if matrix not yet assigned or not of correct size delete old and create new
    if (theEigenvectors == 0 || theEigenvectors->noCols() != numVectorsToStore)
    {
        if (theEigenvectors != 0)
        {
            delete theEigenvectors;
        }


        theEigenvectors = new Matrix(numberDOF, numVectorsToStore);

        if (theEigenvectors == 0 || theEigenvectors->noCols() != numVectorsToStore)
        {
            cerr << "Node::setNumEigenvectors() - out of memory\n";
            return -2;
        }
    }
    else
        // zero the eigenvector matrix
    {
        theEigenvectors->Zero();
    }

    return 0;
}
int
Node::setEigenvector(int mode, const Vector &eigenVector)
{
    if (theEigenvectors == 0 || theEigenvectors->noCols() < mode)
    {
        cerr << "Node::setEigenvectors() - mode " << mode << " invalid\n";
        return -1;
    }

    if (eigenVector.Size() != numberDOF)
    {
        cerr << "Node::setEigenvectors() - eigenvector of incorrect size\n";
        return -2;
    }

    // set the values
    for (int i = 0; i < numberDOF; i++)
    {
        (*theEigenvectors)(i, mode - 1) = eigenVector(i);
    }

    return 0;
}



const Matrix &
Node::getEigenvectors(void)
{
    // check the eigen vectors have been set
    if (theEigenvectors == 0)
    {
        cerr << "Node::getEigenvectors() - eigenvectors have not been set\n";
    }

    return *theEigenvectors;
}


// int Node::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
// {
//     theHDF5_Channel.beginNodeDescription(this->getTag());
//     theHDF5_Channel.addField("data"                       , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
//     theHDF5_Channel.addField("coordinates"                , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_BASIC);
//     if (commitDisp != 0)
//     {
//         theHDF5_Channel.addField("displacements"              , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);
//     }
//     if (commitVel != 0)
//     {
//         theHDF5_Channel.addField("velocity"                   , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);
//     }
//     if (commitAccel != 0)
//     {
//         theHDF5_Channel.addField("acceleration"               , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);
//     }
//     if (mass != 0)
//     {
//         theHDF5_Channel.addField("mass_matrix"                , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
//     }
//     if (R != 0)
//     {
//         theHDF5_Channel.addField("nodal_participation_matrix" , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED);
//     }
//     if (unbalLoad  != 0)
//     {
//         theHDF5_Channel.addField("applied_forces"             , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED);
//     }
//     if (reaction  != 0)
//     {
//         theHDF5_Channel.addField("reactions"                  , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);
//     }
//     if ( theDOF_GroupPtr != 0 )
//     {
//         theHDF5_Channel.addField("DOF_group"                  , ESSI_OUTPUT_TIME_INDEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED);
//     }
//     theHDF5_Channel.endNodeDescription();

//     return 0;
// }


int
Node::sendSelf(int cTag, Channel &theChannel)
{
    int dataTag = this->getDbTag();

    ID data(18);
    data(0) = this->getTag();
    data(1) = numberDOF;

    // indicate whether vector quantaties have been formed
    if (disp == 0)
    {
        data(2) = 1;
    }
    else
    {
        data(2) = 0;
    }

    if (vel == 0)
    {
        data(3) = 1;
    }
    else
    {
        data(3) = 0;
    }

    if (accel == 0)
    {
        data(4) = 1;
    }
    else
    {
        data(4) = 0;
    }

    if (mass == 0)
    {
        data(5) = 1;
    }
    else
    {
        data(5) = 0;
    }

    if (unbalLoad  == 0)
    {
        data(6) = 1;
    }
    else
    {
        data(6) = 0;
    }

    if (R == 0)
    {
        data(12) = 1;
    }
    else
    {
        data(12) = 0;
        data(13) = R->noCols();
    }

    data(7) = Crd->Size();


    if ( theDOF_GroupPtr != 0 )
    {
        data(14) = 1;
        data(15) =  theDOF_GroupPtr->getTag();
        ID temp = theDOF_GroupPtr->getID();
        data(16) = temp.Size();
    }
    else
    {
        data(14) = 0;
    }

    if (reaction != 0)
    {
        data(17) = 1;
    }
    else
    {
        data(17) = 0;
    }

    int res = 0;

    res = theChannel.sendID(dataTag, cTag, data);

    if (res < 0)
    {
        cerr << " Node::sendSelf() - failed to send ID data\n";
        return res;
    }

    res = theChannel.sendVector(dataTag, cTag, *Crd);

    if (res < 0)
    {
        cerr << " Node::sendSelf() - failed to send Vecor data\n";
        return res;
    }

    if (commitDisp != 0)
    {
        res = theChannel.sendVector(dataTag, cTag, *commitDisp);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send Disp data\n";
            return res;
        }
    }

    if (commitVel != 0)
    {
        res = theChannel.sendVector(dataTag, cTag, *commitVel);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send Vel data\n";
            return res;
        }
    }

    if (commitAccel != 0)
    {
        res = theChannel.sendVector(dataTag, cTag, *commitAccel);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send Accel data\n";
            return res;
        }
    }

    if (mass != 0)
    {
        res = theChannel.sendMatrix(dataTag, cTag, *mass);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send Mass data\n";
            return res;
        }
    }

    if (R != 0)
    {
        res = theChannel.sendMatrix(dataTag, cTag, *R);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send R data\n";
            return res;
        }
    }

    if (unbalLoad  != 0)
    {
        res = theChannel.sendVector(dataTag, cTag, *unbalLoad);

        if (res < 0)
        {
            cerr << " Node::sendSelf() - failed to send Load data\n";
            return res;
        }
    }

    //Jose Added
    if ( reaction != 0 )
    {
        if ( theChannel.sendVector(dataTag, cTag, *reaction) < 0)
        {
            cerr << "Node::sendSelf() - failed to send reactions vector!\n";
            return -1;
        }
    }

    //Guanzhou added
    if ( theDOF_GroupPtr != 0 )
    {
        ID theID = theDOF_GroupPtr->getID();

        if ( theChannel.sendID(dataTag, cTag, theID) < 0)
        {
            cerr << "Node::sendSelf() - failed to send DOF_GroupPtr ID!\n";
            return -1;
        }
    }

    // if get here succesfull
    return 0;
}

int
Node::receiveSelf(int cTag, Channel &theChannel,
                  FEM_ObjectBroker &theBroker)
{
    int res = 0;
    int dataTag = this->getDbTag();


    ID data(18);
    res = theChannel.receiveID(dataTag, cTag, data);

    if (res < 0)
    {
        cerr << "Node::receiveSelf() - failed to receive ID data\n";
        return res;
    }

    this->setTag(data(0));
    numberDOF = data(1);
    int numberCrd = data(7);

    penalty_load = new Vector(numberDOF); //Added by Babak 6/25/13


    // create a Vector to hold coordinates IF one needed
    if (Crd == 0)
    {
        Crd = new Vector(numberCrd);
    }

    // check we did not run out of memory
    if (Crd == 0)
    {
        cerr << "Node::receiveSelf() - out of memory creating Coordinate vector\n";
        return -1;
    }

    if (theChannel.receiveVector(dataTag, cTag, *Crd) < 0)
    {
        cerr << "Node::receiveSelf() - failed to receive the Coordinate vector\n";
        return -2;
    }

    if (data(2) == 0)
    {
        // create the disp vectors if node is a total blank
        if (commitDisp == 0)
        {
            this->createDisp();
        }

        // recv the committed disp
        if (theChannel.receiveVector(dataTag, cTag, *commitDisp) < 0)
        {
            cerr << "Node::receiveSelf - failed to receive Disp data\n";
            return res;
        }

        // set the trial quantities equal to committed
        for (int i = 0; i < numberDOF; i++)
        {
            disp[i] = disp[i + numberDOF];    // set trial equal commited
        }


    }
    else if (commitDisp != 0)
    {
        // if going back to initial we will just zero the vectors
        commitDisp->Zero();
        trialDisp->Zero();
    }


    if (data(3) == 0)
    {
        // create the vel vectors if node is a total blank
        if (commitVel == 0)
        {
            this->createVel();
        }

        // recv the committed vel
        if (theChannel.receiveVector(dataTag, cTag, *commitVel) < 0)
        {
            cerr << "Node::receiveSelf - failed to receive Velocity data\n";
            return -3;
        }

        // set the trial quantity
        for (int i = 0; i < numberDOF; i++)
        {
            vel[i] = vel[i + numberDOF];    // set trial equal commited
        }
    }

    if (data(4) == 0)
    {
        // create the vel vectors if node is a total blank
        if (commitAccel == 0)
        {
            this->createAccel();
        }

        // recv the committed accel
        if (theChannel.receiveVector(dataTag, cTag, *commitAccel) < 0)
        {
            cerr << "Node::receiveSelf - failed to receive Acceleration data\n";
            return -4;
        }

        // set the trial values
        for (int i = 0; i < numberDOF; i++)
        {
            accel[i] = accel[i + numberDOF];    // set trial equal commited
        }
    }

    if (data(5) == 0)
    {
        // make some room and read in the vector
        if (mass == 0)
        {
            mass = new Matrix(numberDOF, numberDOF);

            if (mass == 0)
            {
                cerr << "Node::recvData -- ran out of memory\n";
                return -5;
            }
        }

        if (theChannel.receiveMatrix(dataTag, cTag, *mass) < 0)
        {
            cerr << "Node::receiveSelf() - failed to receive Mass data\n";
            return -6;
        }
    }

    if (data(12) == 0)
    {
        // create a matrix for R
        int noCols = data(13);

        if (R == 0)
        {
            R = new Matrix(numberDOF, noCols);

            if (R == 0)
            {
                cerr << "Node::recvData -- ran out of memory\n";
                return -1;
            }
        }

        // now recv the R matrix
        if (theChannel.receiveMatrix(dataTag, cTag, *R) < 0)
        {
            cerr << "Node::receiveSelf() - failed to receive R data\n";
            return res;
        }
    }


    if (data(6) == 0)
    {
        // create a vector for the load
        if (unbalLoad == 0)
        {
            unbalLoad = new Vector(numberDOF);

            if (unbalLoad == 0)
            {
                cerr << "Node::recvData -- ran out of memory\n";
                return -10;
            }
        }

        if (theChannel.receiveVector(dataTag, cTag, *unbalLoad) < 0)
        {
            cerr << "Node::receiveSelf() - failed to receive Load data\n";
            return res;
        }
    }

    //Jose added
    if ( data(17) == 1)
    {
        if ( reaction != 0 )
        {
            delete reaction;
        }

        Vector newreaction;// = new Vector(data(15), this);
        // ID newID(data(16));

        if ( theChannel.receiveVector(dataTag, cTag, newreaction) < 0)
        {
            cerr << "Node::receiveSelf() - reaction failed to recv self!\n";
            return -1;
        }

        *reaction = newreaction;
    }

    //Guanzhou added
    if ( data(14) == 1)
    {
        if ( theDOF_GroupPtr != 0 )
        {
            delete theDOF_GroupPtr;
        }

        theDOF_GroupPtr = new DOF_Group(data(15), this);
        ID newID(data(16));

        if ( theChannel.receiveID(dataTag, cTag, newID) < 0)
        {
            cerr << "Node::receiveSelf() - DOF_Group failed to recv self!\n";
            return -1;
        }

        theDOF_GroupPtr->setID(newID);
    }


    sizedMatrix = new Matrix(numberDOF, numberDOF);

    penalty_load = new Vector(numberDOF);


    return 0;
}



void
Node::Print(ostream &s, int flag)
{
    if (flag == 0)   // print out everything
    {
        s << "\n Node: " << this->getTag() << endln;
        s << "\tCoordinates  : " << *Crd;

        if (commitDisp != 0)
        {
            s << "Displacements     : " << *trialDisp;
        }

        if (commitVel != 0)
        {
            s << "\tVelocities      : " << *trialVel;
        }

        if (commitAccel != 0)
        {
            s << "\tAcceleration    : " << *trialAccel;
        }

        if (unbalLoad != 0)
        {
            s << "\tUnbalanced Load : " << *unbalLoad;
        }

        //     if (mass != 0)
        //  s << "\tMass : " << *mass;
        if (theEigenvectors != 0)
        {
            s << "\tEigenvectors    : " << *theEigenvectors;
        }

        if (reaction != 0)
        {
            s << "\tReactions       : " << *reaction;
        }

        if (theDOF_GroupPtr != 0)
        {
            s << "\tDOF Groups      : " << theDOF_GroupPtr->getID();
        }

        s << "\n";
    }
    else if (flag == 1)   // print out: nodeId displacements
    {
        s << this->getTag() << "  " << *commitDisp << "\n";
        //s << this->getTag() << "  " << *trialDisp << "\n";
    }
}

// createDisp(), createVel() and createAccel():
// private methods to create the arrays to hold the disp, vel and acceleration
// values and the Vector objects for the committed and trial quantaties.

int
Node::createDisp(void)
{
    // trial , committed, incr = (committed-trial)

    if (numberDOF < 1)
    {
        cerr << "WARNING - Node::createDisp() numberDOF = " <<  numberDOF << endl;

        return -1;
    }

    disp = new double[4 * numberDOF];

    if (disp == 0)
    {
        cerr << "WARNING - Node::createDisp() ran out of memory for array of size " << 2 * numberDOF << endl;

        return -1;
    }

    for (int i = 0; i < 4 * numberDOF; i++)
    {
        disp[i] = 0.0;
    }

    commitDisp = new Vector(&disp[numberDOF], numberDOF);
    trialDisp = new Vector(disp, numberDOF);
    incrDisp = new Vector(&disp[2 * numberDOF], numberDOF);
    incrDeltaDisp = new Vector(&disp[3 * numberDOF], numberDOF);

    if (commitDisp == 0 || trialDisp == 0 || incrDisp == 0 || incrDeltaDisp == 0)
    {
        cerr << "WARNING - Node::createDisp() " <<
             "ran out of memory creating Vectors(double *,int)";
        return -2;
    }

    return 0;
}


int
Node::createVel(void)
{
    vel = new double[2 * numberDOF];

    if (vel == 0)
    {
        cerr << "WARNING - Node::createVel() ran out of memory for array of size " << 2 * numberDOF << endln;
        return -1;
    }

    for (int i = 0; i < 2 * numberDOF; i++)
    {
        vel[i] = 0.0;
    }

    commitVel = new Vector(&vel[numberDOF], numberDOF);
    trialVel = new Vector(vel, numberDOF);

    if (commitVel == 0 || trialVel == 0)
    {
        cerr << "WARNING - Node::createVel() %s" <<
             "ran out of memory creating Vectors(double *,int) \n";
        return -2;
    }

    return 0;
}

int
Node::createAccel(void)
{
    accel = new double[2 * numberDOF];

    if (accel == 0)
    {
        cerr << "WARNING - Node::createAccel() ran out of memory for array of size " << 2 * numberDOF << endln;
        return -1;
    }

    for (int i = 0; i < 2 * numberDOF; i++)
    {
        accel[i] = 0.0;
    }

    commitAccel = new Vector(&accel[numberDOF], numberDOF);
    trialAccel = new Vector(accel, numberDOF);

    if (commitAccel == 0 || trialAccel == 0)
    {
        cerr << "WARNING - Node::createAccel() ran out of memory creating Vectors(double *,int)\n";
        return -2;
    }

    return 0;
}



const Vector &
Node::getReaction()
{
    if (reaction == 0)
    {
        reaction = new Vector(numberDOF);

        if (reaction == 0)
        {
            cerr << "FATAL Node::getReaction() - out of memory\n";
            exit(-1);
        }
    }



    return *reaction;
}



int
Node::addReactionForce(const Vector &add, double factor)
{

    // create rection vector if have not done so already
    if (reaction == 0)
    {
        reaction = new Vector(numberDOF);

        if (reaction == 0)
        {
            cerr << "WARNING Node::addReactionForce() - out of memory\n";
            return -1;
        }
    }

    // check vector of appropraie size
    if (add.Size() != numberDOF)
    {
        cerr << "WARNING Node::addReactionForce() - vector not of correct size\n";
        return -1;
    }

    if (factor == 1.0)
    {
        *reaction += add;
    }
    else if (factor == -1.0)
    {
        *reaction -= add;
    }
    else
    {
        *reaction = add * factor;
    }



    return 0;
}



// Nima Tafazzoli (Oct. 2010)
void
Node::setDamping_Coefficient(double A0)
{
    a0 = A0;
}

double
Node::getDamping_Coefficient(void)
{
    return a0;
}



// const Vector *
// Node::getResponse(NodeResponseType responseType)
// {
//   const Vector *result = NULL;
//   if (responseType == Disp)
//     result  = &(this->getDisp());
//   else if (responseType == Vel)
//     return &(this->getVel());
//   else if (responseType == Accel)
//     return &(this->getAccel());
//   else if (responseType == IncrDisp)
//     return &(this->getIncrDisp());
//   else if (responseType == IncrDeltaDisp)
//     return &(this->getIncrDeltaDisp());
//   else if (responseType == Reaction)
//     return &(this->getReaction());
//   else
//     return NULL;
//
//   return result;
// }




int
Node::addUnbalancedLoad(const Vector &add, bool penalty_status)
{



    if ( penalty_status == true)
    {

        penalty_load->Zero();
        penalty_load->addVector(1.0, add, 1.0);


        // check vector arg is of correct size
        if (add.Size() != numberDOF)
        {
            cerr << "Node::addunbalLoad - load to add of incorrect size ";
            cerr << add.Size() << " should be " <<  numberDOF << endln;
            return -1;
        }

        // if no load yet create it and assign
        if (unbalLoad == 0)
        {
            unbalLoad = new Vector(add);

            if (unbalLoad == 0)
            {
                cerr << "FATAL Node::addunbalLoad - ran out of memory\n";
                exit(-1);
            }

            return 0;
        }


        // add fact*add to the unbalanced load
        unbalLoad->addVector(1.0, add, 1.0);



    }

    return 0;
}





int
Node::resetReactionForce(int flag)
{

    // create rection vector if have not done so already
    if (reaction == 0)
    {
        reaction = new Vector(numberDOF);

        if (reaction == 0)
        {
            cerr << "WARNING Node::addReactionForce() - out of memory\n";
            return -1;
        }
    }

    reaction->Zero();

    // add unbalance, the negative of applied forces hence the -=
    if (flag == 0)
    {



        *reaction -= this->getUnbalancedLoad();


        *reaction += *penalty_load;



    }

    if (flag == 1)
    {
        *reaction -= this->getUnbalancedLoadIncInertia();
        *reaction += *penalty_load;
    }
    else
    {
        if (mass != 0 && a0 != 0)
        {
            if (a0 != 0.0)
            {
                const Vector &theVel = this->getTrialVel(); // in case vel not created
                reaction->addMatrixVector(1.0, *mass, theVel, a0);
            }
        }
    }

    return 0;
}




// Nima Tafazzoli added for checking the mesh (September 2012)
int
Node::CheckMesh(ofstream &checkmesh_file)
{
    return 0;
}

// NOTE THE MACRO!
#define SAFE_COMPARE_POINTERS(p,q) if (!(p == NULL) && !( q == NULL) ) \
    { \
        if (!(*p == *q)) \
        { \
            return false; \
        } \
    }

bool
Node::operator==(const Node &rhs) const
{
    if (numberDOF != rhs.numberDOF)
    {
        return false;
    }

    // NOTE THE MACRO! SAFE_COMPARE_POINTERS
    SAFE_COMPARE_POINTERS(theDOF_GroupPtr      , rhs.theDOF_GroupPtr);

    SAFE_COMPARE_POINTERS(Crd                  , rhs.Crd);
    SAFE_COMPARE_POINTERS(commitDisp           , rhs.commitDisp);
    SAFE_COMPARE_POINTERS(commitVel            , rhs.commitVel);
    SAFE_COMPARE_POINTERS(commitAccel          , rhs.commitAccel);
    SAFE_COMPARE_POINTERS(trialDisp            , rhs.trialDisp);
    SAFE_COMPARE_POINTERS(trialVel             , rhs.trialVel);
    SAFE_COMPARE_POINTERS(trialAccel           , rhs.trialAccel);

    SAFE_COMPARE_POINTERS(unbalLoad            , rhs.unbalLoad);
    SAFE_COMPARE_POINTERS(incrDisp             , rhs.incrDisp);
    SAFE_COMPARE_POINTERS(incrDeltaDisp        , rhs.incrDeltaDisp);

    SAFE_COMPARE_POINTERS(disp                 , rhs.disp);
    SAFE_COMPARE_POINTERS(vel                  , rhs.vel);
    SAFE_COMPARE_POINTERS(accel                , rhs.accel);

    SAFE_COMPARE_POINTERS(R                    , rhs.R);
    SAFE_COMPARE_POINTERS(mass                 , rhs.mass);
    SAFE_COMPARE_POINTERS(unbalLoadWithInertia , rhs.unbalLoadWithInertia);

    if ( a0 != rhs.a0)
    {
        return false;
    }

    SAFE_COMPARE_POINTERS(theEigenvectors      , rhs.theEigenvectors);
    SAFE_COMPARE_POINTERS(sizedMatrix          , rhs.sizedMatrix);
    SAFE_COMPARE_POINTERS(reaction             , rhs.reaction);
    SAFE_COMPARE_POINTERS(penalty_load         , rhs.penalty_load);

    return true;
}






int Node::zeroDisplacements()
{
    if (commitDisp != 0)
    {
        // cout << ".";
        commitDisp->Zero();
    }
    if (commitVel != 0)
    {
        commitVel->Zero();
    }
    if (commitAccel != 0)
    {
        commitAccel->Zero();
    }
    if (trialDisp != 0)
    {
        // cout << "+";
        trialDisp->Zero();
    }
    if (trialVel != 0)
    {
        trialVel->Zero();
    }
    if (trialAccel != 0)
    {
        trialAccel->Zero();
    }
    return 0;
}