// File: ~/database/DB_Datastore.C
//
// Written: Jun Peng  (junpeng@stanford.edu)
// Created: 09/2000
// Revision: 08/2001 Made it more stable
//
// Description: This file contains the class implementation for DB_Datastore.
// DB_Datastore is a concrete subclas of FE_Datastore. A DB_Datastore
// object is used in the program to store/restore the geometry and state
// information in a domain at a particular instance in the analysis. The
// information is stored in text files.
//
// What: "@(#) DB_Datastore.C, revA"


#include "DB_Datastore.h"
#include <iiostream>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bool.h>

#include <MovableObject.h>
#include <FEM_ObjectBroker.h>
#include <Domain.h>
#include <ID.h>
#include <Vector.h>
#include <Matrix.h>
#include <iostream>
using namespace std;


#define IsCreateTable  0

extern "C" void connectToDB(char* theUser, char* thePass);
extern "C" void commitDatabase();
extern "C" void createIDTable(char* tableName);
extern "C" void createVectorTable(char* tableName);
extern "C" void createMatrixTable(char* tableName);
extern "C" void cleanTables();

extern "C" void changeProjTag(int projectID);

// a method to query the project Tag on TagTab.
extern "C" int queryProjTag();
// a method to query the project Tag based on its name.
extern "C" int queryTagOnName(char* projName);

extern "C" void insertIDTable(int idSize, int* tempProjTag, int* tempDataTag,
                              int* tempCommitTag, int* tempPos, int* tempID);

extern "C" void queryIDTable(int idSize, int projTag, int dataTag,
                             int commitTag, int* tempID);

extern "C" void insertVectorTable(int vectSize, int* tempProjTag, int* tempDataTag,
                                  int* tempCommitTag, int* tempPos, double* tempVect);
extern "C" void queryVectorTable(int vectSize, int projTag, int dataTag,
                                 int commitTag, double* tempVect);

extern "C" void insertMatrixTable(int matrixSize, int* tempProjTag, int* tempDataTag,
                                  int* tempCommitTag, int* row, int* col, double* tempMatrix);
extern "C" void queryMatrixTable(int rowSize, int colSize, int projTag, int dataTag,
                                 int commitTag, double* tempMatrix);



DB_Datastore::DB_Datastore(char* dataBaseName,
                           Domain& theDomain,
                           FEM_ObjectBroker& theObjBroker)
    : FE_Datastore(theDomain, theObjBroker), dbTag(0)
{
    connectToDB("junpeng", "g3iscool");

    projTag = this->searchProjTag(dataBaseName);

    if (IsCreateTable)
    {
        createIDTable("ID");
        createVectorTable("VECTOR");
        createMatrixTable("MATRIX");
    }

    //  cleanTables();
}


DB_Datastore::~DB_Datastore()
{
    commitDatabase();
}


int
DB_Datastore::getProjTag(void)
{
    return projTag;
}

/* normally this method will not be directly used for saving data,
 * only used for data query.
 */
void
DB_Datastore::setProjTag(int projectID)
{
    projTag = projectID;
}

/* Search "PROJTAB" to find the projTag based on its name,
 * which is a key.
 */
int
DB_Datastore::searchProjTag(char* projName)
{
    int projTag = queryTagOnName(projName);

    return projTag;
}

int
DB_Datastore::getDbTag(void)
{
    dbTag++;
    return dbTag;
}

/********************************************************************
 *                   CHANNEL METHODS  THAT DO NOTHING               *
 ********************************************************************/

char*
DB_Datastore::addToProgram(void)
{
    return 0;
}

int
DB_Datastore::setUpShadow(void)
{
    return 0;
}

int
DB_Datastore::setUpActor(void)
{
    return 0;
}

int
DB_Datastore::setNextAddress(const ChannelAddress& otherChannelAddress)
{
    return 0;
}


ChannelAddress*
DB_Datastore::getLastSendersAddress(void)
{
    return 0;
}


/********************************************************************
 *                USEFULE METHODS  TO STORE/RETRIEVE DATA           *
 ********************************************************************/

int
DB_Datastore::commitState(int commitTag)
{
    int result = FE_Datastore::commitState(commitTag);

    return result;
}

int
DB_Datastore::sendObj(int commitTag,
                      MovableObject& theObject,
                      ChannelAddress* theAddress)
{
    return theObject.sendSelf(commitTag, *this);
}

int
DB_Datastore::receiveObj(int commitTag,
                      MovableObject& theObject,
                      FEM_ObjectBroker& theNewBroker,
                      ChannelAddress* theAddress)
{
    return theObject.receiveSelf(commitTag, *this, theNewBroker);
}


int
DB_Datastore::sendMsg(int dataTag, int commitTag,
                      const Message&,
                      ChannelAddress* theAddress)
{
    cerr << "DB_Datastore::sendMsg() - not yet implemented\n";
    return -1;
}

int
DB_Datastore::receiveMsg(int dataTag, int commitTag,
                      Message&,
                      ChannelAddress* theAddress)
{
    cerr << "DB_Datastore::receiveMsg() - not yet implemented\n";
    return -1;
}


int
DB_Datastore::sendMatrix(int dataTag, int commitTag,
                         const Matrix& theMatrix,
                         ChannelAddress* theAddress)
{
    int numRows = theMatrix.noRows();
    int numCols = theMatrix.noCols();
    int matrixSize = numRows * numCols;
    int pt;

    int* tempProjTag = new int[matrixSize];
    int* tempDataTag = new int[matrixSize];
    int* tempCommitTag = new int[matrixSize];
    int* row = new int[matrixSize];
    int* col = new int[matrixSize];
    double* tempMatrix = new double[matrixSize];

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            pt = i * numCols + j;
            tempProjTag[pt] = projTag;
            tempDataTag[pt] = dataTag;
            tempCommitTag[pt] = commitTag;
            row[pt] = i;
            col[pt] = j;
            tempMatrix[pt] = theMatrix(i, j);
        }
    }

    insertMatrixTable(matrixSize, tempProjTag, tempDataTag, tempCommitTag, row, col, tempMatrix);

    delete [] tempDataTag;
    delete [] tempCommitTag;
    delete [] row;
    delete [] col;
    delete [] tempMatrix;

    return 0;
}


int
DB_Datastore::receiveMatrix(int dataTag, int commitTag,
                         Matrix& theMatrix,
                         ChannelAddress* theAddress)
{
    int numRows = theMatrix.noRows();
    int numCols = theMatrix.noCols();
    int matrixSize = numRows * numCols;

    double* tempMatrix = new double[matrixSize];

    queryMatrixTable(numRows, numCols, projTag, dataTag, commitTag, tempMatrix);

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            theMatrix(i, j) = tempMatrix[i * numCols + j];
        }
    }

    delete [] tempMatrix;

    return 0;
}


int
DB_Datastore::sendVector(int dataTag, int commitTag,
                         const Vector& theVector,
                         ChannelAddress* theAddress)
{
    int vectSize = theVector.Size();
    int* tempProjTag = new int[vectSize];
    int* tempDataTag = new int[vectSize];
    int* tempCommitTag = new int[vectSize];
    int* tempPos = new int[vectSize];
    double* tempVect = new double[vectSize];

    for (int i = 0; i < vectSize; i++)
    {
        tempProjTag[i] = projTag;
        tempDataTag[i] = dataTag;
        tempCommitTag[i] = commitTag;
        tempPos[i] = i;
        tempVect[i] = theVector(i);
    }

    insertVectorTable(vectSize, tempProjTag, tempDataTag, tempCommitTag, tempPos, tempVect);

    delete [] tempProjTag;
    delete [] tempDataTag;
    delete [] tempCommitTag;
    delete [] tempPos;
    delete [] tempVect;

    return 0;
}

int
DB_Datastore::receiveVector(int dataTag, int commitTag,
                         Vector& theVector,
                         ChannelAddress* theAddress)
{
    int vectSize = theVector.Size();
    double* tempVect = new double[vectSize];

    queryVectorTable(vectSize, projTag, dataTag, commitTag, tempVect);

    for (int i = 0; i < vectSize; i++)
    {
        theVector(i) = tempVect[i];
    }

    delete [] tempVect;

    return 0;
}



int
DB_Datastore::sendID(int dataTag, int commitTag,
                     const ID& theID,
                     ChannelAddress* theAddress)
{
    int idSize = theID.Size();
    int* tempProjTag = new int[idSize];
    int* tempDataTag = new int[idSize];
    int* tempCommitTag = new int[idSize];
    int* tempPos = new int[idSize];
    int* tempID = new int[idSize];

    for (int i = 0; i < idSize; i++)
    {
        tempProjTag[i] = projTag;
        tempDataTag[i] = dataTag;
        tempCommitTag[i] = commitTag;
        tempPos[i] = i;
        tempID[i] = theID(i);
    }

    insertIDTable(idSize, tempProjTag, tempDataTag, tempCommitTag, tempPos, tempID);

    delete [] tempDataTag;
    delete [] tempCommitTag;
    delete [] tempPos;
    delete [] tempID;

    return 0;
}


int
DB_Datastore::receiveID(int dataTag, int commitTag,
                     ID& theID,
                     ChannelAddress* theAddress)
{
    int idSize = theID.Size();
    int* tempID = new int[idSize];

    queryIDTable(idSize, projTag, dataTag, commitTag, tempID);

    for (int i = 0; i < idSize; i++)
    {
        theID(i) = tempID[i];
    }

    delete [] tempID;
    return 0;
}

