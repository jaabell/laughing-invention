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
// $Date: 2003/02/14 23:00:55 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/constraints/ImposedMotionSP.cpp,v $


// File: ~/domain/constraints/ImposedMotionSP.C
//
// Written: fmk
// Created: 11/00
// Revision: A
//
// Purpose: This file contains the implementation of class ImposedMotionSP.

#include <ImposedMotionSP.h>
#include <classTags.h>
#include <Vector.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <GroundMotion.h>
#include <Node.h>
#include <Domain.h>
# include <ID.h>
# include <MultiSupportPattern.h>

#include <iostream>
using namespace std;


// constructor for FEM_ObjectBroker
ImposedMotionSP::ImposedMotionSP()
    : SP_Constraint(CNSTRNT_TAG_ImposedMotionSP),
      theGroundMotion(0), theNode(0), theNodeResponse(0),
      theGroundMotionResponse(3), destroyMotion(0)
{


}

// constructor for a subclass to use
ImposedMotionSP::ImposedMotionSP(int tag, int node, int ndof,
                                 GroundMotion& theMotion, int gMotionID, bool killMotion)
    : SP_Constraint(tag, node, ndof, CNSTRNT_TAG_ImposedMotionSP),
      theNode(0), theNodeResponse(0), theGroundMotionResponse(3), destroyMotion(0)
{
    theGroundMotion = &theMotion;
    theGroundMotionID = gMotionID; //Guanzhou

    if (killMotion == true)
    {
        destroyMotion = 1;
    }


}


ImposedMotionSP::~ImposedMotionSP()
{

    // Nima Tafazzoli (April 2013)
    // we will take care of this in applyConstraint function since
    // a new pointer is created at each time step. It can not be
    // created at the constructor because the domain components is
    // empty.

    //   if (theNodeResponse != 0)
    //     delete theNodeResponse;

    if (destroyMotion == 1)
    {
        delete theGroundMotion;
    }
}

int//Guanzhou added
ImposedMotionSP::setGroundMotion(GroundMotion* theMotion)
{
    if (theMotion != 0)
    {
        theGroundMotion = theMotion;
    }
    else
    {
        cerr << "ImposedMotionSP::setGroundMotion - can not set null pointer!\n";
    }

    return 0;
}

double
ImposedMotionSP::getValue(void)
{
    // no longer return 0 for TransformationConstraints also set response at nodes
    return theGroundMotionResponse(0);
}


int
ImposedMotionSP::applyConstraint(double time)
{


    if (theNode == 0 || theNodeResponse)
    {
        Domain* theDomain = this->getDomain();


        if (theDomain == 0)
        {
            cerr << "ImposedMotionSP::ImposedMotionSP() domain can not be found" << endl;
            exit(-1);
        }


        theNode = theDomain->getNode(nodeTag);

        if (theNode == 0)
        {
            cerr << "ImposedMotionSP::ImposedMotionSP() node can not be found" << endl;
            exit(-1);
        }
    }


    int numNodeDOF = theNode->getNumberDOF();

    theNodeResponse = new Vector(numNodeDOF);


    if (theNodeResponse == 0)
    {
        cerr << "ImposedMotionSP::ImposedMotionSP() theNodeResponse can not be set" << endl;
        exit(-1);
    }





    // now get the response from the ground motion
    theGroundMotionResponse = theGroundMotion->getDispVelAccel(time);

    // now set the responses at the node
    *theNodeResponse = theNode->getTrialDisp();
    (*theNodeResponse)(dofNumber) = theGroundMotionResponse(0);
    theNode->setTrialDisp(*theNodeResponse);

    *theNodeResponse = theNode->getTrialVel();
    (*theNodeResponse)(dofNumber) = theGroundMotionResponse(1);
    theNode->setTrialVel(*theNodeResponse);

    *theNodeResponse = theNode->getTrialAccel();
    (*theNodeResponse)(dofNumber) = theGroundMotionResponse(2);
    theNode->setTrialAccel(*theNodeResponse);


    delete theNodeResponse;

    return 0;
}


bool
ImposedMotionSP::isHomogeneous(void) const
{
    return true;
}


int //Guanzhou implemented
ImposedMotionSP::sendSelf(int cTag, Channel& theChannel)
{
    static ID data(4);

    data(0) = this->getTag();
    data(1) = nodeTag;
    data(2) = dofNumber;
    data(3) = theGroundMotionID;
    int result = theChannel.sendID(this->getDbTag(), cTag, data);

    if (result < 0)
    {
        cerr << "WARNING ImposedMotionSP::sendSelf - error sending data\n";
    }

    return result;
}

int  //Guanzhou implemented
ImposedMotionSP::recvSelf(int cTag, Channel& theChannel,
                          FEM_ObjectBroker& theBroker)
{
    static ID data(4);

    int result = theChannel.recvID(this->getDbTag(), cTag, data);

    if (result < 0)
    {
        cerr << "WARNING ImposedMotionSP::recvSelf - error receiving data\n";
        return result;
    }

    // if o.k. set the data
    this->setTag(data(0));
    nodeTag = data(1);
    dofNumber = data(2);
    theGroundMotionID = data(3);

    //Domain *theDomain = this->getDomain();
    //const int LoadPatternTag = this->getLoadPatternTag();
    //MultiSupportPattern *theLP = (MultiSupportPattern *)theDomain->getLoadPattern(LoadPatternTag);
    //theGroundMotion = theLP->getMotion(theGroundMotionID);
    return 0;
}

void
ImposedMotionSP::Print(ostream& s, int flag)
{
    s << "ImposedMotionSP: " << this->getTag();
    s << "\t Node: " << this->getNodeTag();
    s << " DOF: " << this->getDOF_Number();
    s << " GroundMotion ID: " <<  theGroundMotionID << endln;
}








