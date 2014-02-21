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

// $Revision: 1.8 $
// $Date: 2005-11-07 23:53:45 $
// $Source: /usr/local/cvs/OpenSees/SRC/database/FE_Datastore.cpp,v $

// Written: fmk
// Created: 10/98
// Revision: A
//
// Description: This file contains the class implementation for FE_Datastore.
// FE_Datastore is an abstract base class. An FE_Datastore object is used
// in the program to store/restore the geometry and state information in
// a domain at a particular instance in the analysis.
//
// What: "@(#) FE_Datastore.C, revA"

#include "FE_Datastore.h"
#include <FEM_ObjectBroker.h>
#include <MovableObject.h>
#include <Domain.h>
#include <OPS_Globals.h>
#include <ID.h>
#include <iostream>
using namespace std;


int FE_Datastore::lastDbTag(0);

// FE_Datastore(int tag, int noExtNodes);
//  constructor that takes the FE_Datastore's unique tag and the number
//  of external nodes for the FE_Datastore.

FE_Datastore::FE_Datastore(Domain& thDomain, FEM_ObjectBroker& theBroker)
    : theObjectBroker(&theBroker), theDomain(&thDomain), stepNumber(0)
{

}


FE_Datastore::~FE_Datastore()
{
    // does nothing
}

int
FE_Datastore::isDatastore(void)
{
    return 1;
}

/********************************************************************
 *                   CHANNEL METHODS  THAT DO NOTHING               *
 ********************************************************************/

char*
FE_Datastore::addToProgram(void)
{
    return 0;
}

int
FE_Datastore::setUpConnection(void)
{
    return 0;
}

int
FE_Datastore::setNextAddress(const ChannelAddress& otherChannelAddress)
{
    return 0;
}


ChannelAddress*
FE_Datastore::getLastSendersAddress(void)
{
    return 0;
}


int
FE_Datastore::sendObj(int commitTag,
                      MovableObject& theObject,
                      ChannelAddress* theAddress)
{
    return theObject.sendSelf(commitTag, *this);
}

int
FE_Datastore::recvObj(int commitTag,
                      MovableObject& theObject,
                      FEM_ObjectBroker& theNewBroker,
                      ChannelAddress* theAddress)
{
    return theObject.recvSelf(commitTag, *this, theNewBroker);
}




int
FE_Datastore::commitState(int commitTag)
{
    // invoke sendSelf on the domain object with this as an arg
    int res = 0;

    if (theDomain != 0)
    {
        res = theDomain->sendSelfDatabase(commitTag, *this);

        if (res < 0)
        {
            cerr << "FE_Datastore::commitState - domain failed to sendSelf\n";
            return res;
        }

        ID maxlastDbTag(1);
        maxlastDbTag(0) = lastDbTag;

        if (this->sendID(0, 0, maxlastDbTag) < 0)
        {
            cerr << "FE_Datastore::commitState - failed to get max lastDbTag data from database - problems may ariise\n";
        }
    }

    return res;
}



int
FE_Datastore::restoreState(int commitTag)
{
    // invoke recvSelf on the domain object with this as an arg
    int res = 0;

    if (theDomain != 0)
    {
        res = theDomain->recvSelfDatabase(commitTag, *this, *theObjectBroker);

        if (res < 0)
        {
            cerr << "FE_Datastore::restoreState - domain failed to recvSelf\n";
        }

        ID maxlastDbTag(1);

        if (this->recvID(0, 0, maxlastDbTag) < 0)
        {
            cerr << "FE_Datastore::restoreState - failed to get max lastDbTag data from database - problems may ariise\n";
        }
        else
        {
            lastDbTag = maxlastDbTag(0);
        }

    }

    return res;
}




int
FE_Datastore::createTable(const char* table, int numColumns, char* columns[])
{
    cerr << "FE_Datastore::createTable - not yet implemented\n";
    return -1;
}


int
FE_Datastore::insertData(const char* tableName, char* columns[],
                         int commitTag, const Vector& data)
{
    cerr << "FE_Datastore::insertData - not yet implemented\n";
    return -1;
}


int
FE_Datastore::getData(const char* table, char* column[], int commitTag, Vector& data)
{
    cerr << "FE_Datastore::getData - not yet implemented\n";
    return -1;
}

int
FE_Datastore::getDbTag(void)
{
    lastDbTag++;
    return lastDbTag;
}




//*******************************************************************************
// Nima Tafazzoli added for saving results using mySQL, November 2012


int
FE_Datastore::setDbTag(int cTag)
{
    lastDbTag = cTag;

    return 0;
}


// int
// FE_Datastore::saveResults(int commitTag)
// {
//   // invoke saveResults on the domain object with this as an arg
//   int res = 0;
//   if (theDomain != 0)
//   {
//     res = theDomain->saveResults(commitTag, *this);
//     if (res < 0) {
//       cerr << "FE_Datastore::saveResults - domain failed to sendSelf\n";
//       return res;
//     }
//     ID maxlastDbTag(1);
//     maxlastDbTag(0) = lastDbTag;
//     if (this->sendID(0,0,maxlastDbTag) < 0)
//     {
//       cerr << "FE_Datastore::saveResults - failed to get max lastDbTag data from database - problems may ariise\n";
//     }
//   }
//
//   return res;
// }



// int
// FE_Datastore::restoreResults(int commitTag)
// {
//   // invoke restoreResults on the domain object with this as an arg
//   int res = 0;
//   if (theDomain != 0)
//   {
//     res = theDomain->restoreResults(commitTag, *this, *theObjectBroker);
//     if (res < 0) {
//       cerr << "FE_Datastore::restoreResults - domain failed to recvSelf\n";
//     }
//     ID maxlastDbTag(1);
//     if (this->recvID(0,0,maxlastDbTag) < 0)
//     {
//       cerr << "FE_Datastore::restoreResults - failed to get max lastDbTag data from database - problems may ariise\n";
//     } else
//       lastDbTag = maxlastDbTag(0);
//
//   }
//
//   return res;
// }



double
FE_Datastore::restoreNodalDisplacement(int NodeNumber, int dof, int stepNumber)
{

    return this->restoreNodalDisplacement(NodeNumber, dof, stepNumber);

}



int
FE_Datastore::createDisplacementTables(int StepNumber)
{
    return this->createDisplacementTables(StepNumber);
}


int
FE_Datastore::create8GPBrickStressTables(int StepNumber)
{
    return this->create8GPBrickStressTables(StepNumber);
}


int
FE_Datastore::create27GPBrickStressTables(int StepNumber)
{
    return this->create27GPBrickStressTables(StepNumber);
}


int
FE_Datastore::createLineElementForceTables(int StepNumber)
{
    return this->createLineElementForceTables(StepNumber);
}


int
FE_Datastore::saveResultsDatabase()
{
    int res = 0;

    stepNumber++;

    if (theDomain != 0)
    {

        //=============================================================================================
        this->createDisplacementTables(stepNumber);


        res = theDomain->saveNodalDisplacements(stepNumber, *this);

        if (res < 0)
        {
            cerr << "FE_Datastore::saveResultsDatabase - domain failed to save nodal displacements\n";
            exit(-1);
        }


        //=============================================================================================

        if (theDomain->getNumberof8GPBrickElements() != 0)
        {
            this->create8GPBrickStressTables(stepNumber);


            res = theDomain->save8GPBrickStresses(stepNumber, *this);

            if (res < 0)
            {
                cerr << "FE_Datastore::saveResultsDatabase - domain failed to save 8GP brick element stresses\n";
                exit(-1);
            }

        }

        //=============================================================================================


        if (theDomain->getNumberof27GPBrickElements() != 0)
        {
            this->create27GPBrickStressTables(stepNumber);


            res = theDomain->save27GPBrickStresses(stepNumber, *this);

            if (res < 0)
            {
                cerr << "FE_Datastore::saveResultsDatabase - domain failed to save 27GP brick element stresses\n";
                exit(-1);
            }

        }

        //=============================================================================================


        if (theDomain->getNumberofLineElements() != 0)
        {
            this->createLineElementForceTables(stepNumber);


            res = theDomain->saveLineElementForces(stepNumber, *this);

            if (res < 0)
            {
                cerr << "FE_Datastore::saveResultsDatabase - domain failed to save line element forces\n";
                exit(-1);
            }

        }

        //=============================================================================================


    }


    return res;
}





