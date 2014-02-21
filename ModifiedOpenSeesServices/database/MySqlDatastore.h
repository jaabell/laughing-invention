/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2002, The Regents of the University of California    **
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


//**********************************************************************
// Major updates by Nima Tafazzoli (September 2012):
// 1. Adding SendnDarray and RecvnDarray functions
// 2. Replacing mysql_escape_string by mysql_real_escape_string
//    (suggested for new version of mysql)
//**********************************************************************


#ifndef MySqlDatastore_h
#define MySqlDatastore_h


// Description: This file contains the class definition for MySqlDatastore.
// MySqlDatastore is a concrete subclas of FE_Datastore. A MySqlDatastore
// object is used in the program to store/restore the geometry and state
// information in a domain at a particular instance in the analysis. The
// information is stored in a MySQL Database which can be local or remote.
//
// What: "@(#) MySqlDatastore.h, revA"

#include <FE_Datastore.h>
#include <Vector.h>
#include <Matrix.h>
#include <ID.h>
#include <nDarray.h> //Nima added
#include <mysql.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <sstream>

using namespace std;



class MySqlDatastore: public FE_Datastore
{
    public:
        MySqlDatastore(const char* databaseName,
                       Domain& theDomain,
                       FEM_ObjectBroker& theBroker,
                       int dbRun = 0);

        MySqlDatastore(int commitTag,
                       const char* databaseName,
                       const char* host,
                       const char* user,
                       const char* passwd,
                       unsigned int port,
                       const char* socket,
                       Domain& theDomain,
                       FEM_ObjectBroker& theBroker,
                       int dbRun = 0);

        ~MySqlDatastore();

        // methods for sending and recieving matrices, vectors and id's
        int sendMsg(int dbTag, int commitTag,
                    const Message&,
                    ChannelAddress* theAddress = 0);
        int recvMsg(int dbTag, int commitTag,
                    Message&,
                    ChannelAddress* theAddress = 0);

        int sendMatrix(int dbTag, int commitTag,
                       const Matrix& theMatrix,
                       ChannelAddress* theAddress = 0);
        int recvMatrix(int dbTag, int commitTag,
                       Matrix& theMatrix,
                       ChannelAddress* theAddress = 0);

        int sendVector(int dbTag, int commitTag,
                       const Vector& theVector,
                       ChannelAddress* theAddress = 0);
        int recvVector(int dbTag, int commitTag,
                       Vector& theVector,
                       ChannelAddress* theAddress = 0);

        int sendID(int dbTag, int commitTag,
                   const ID& theID,
                   ChannelAddress* theAddress = 0);
        int recvID(int dbTag, int commitTag,
                   ID& theID,
                   ChannelAddress* theAddress = 0);


        int sendquery(int commitTag, char* query,
                      ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        // Nima Tafazzoli (Sep. 2012)
        int sendnDarray(int dbTag, int commitTag,
                        const nDarray& theNDarray,
                        ChannelAddress* theAddress = 0);

        int recvnDarray(int dbTag, int commitTag,
                        nDarray& theNDarray,
                        ChannelAddress* theAddress = 0);

        int sendDisplacementsID(int dbTag, int commitTag, int StepNumber,
                                const ID& theID,
                                ChannelAddress* theAddress = 0);

        int sendDisplacementsVector(int commitTag, char* query,
                                    ChannelAddress* theAddress = 0);

        double recvDisplacementsVector(int dbTag, int dof, int commitTag, int stepNumber,
                                       ChannelAddress* theAddress = 0);


        //     double recvDisplacementsVector(int dbTag, int dof, int commitTag, int stepNumber,
        //                                         Vector &theVector,
        //                                         ChannelAddress *theAddress=0);

        int recvDisplacementsID(int dbTag, int commitTag, int stepNumber,
                                ID& theID,
                                ChannelAddress* theAddress = 0);

        int createDisplacementTables(int StepNumber);
        int getCommitTag();



        int createTable(const char* tableName, int numColumns, char* columns[]);
        int insertData(const char* tableName, char* columns[], int commitTag, const Vector& data);
        int getData(const char* tableName, char* columns[], int commitTag, Vector& data);

        int setDbRun(int run);
        int getDbRun(void);

    protected:
        int createDatabase(const char* projectName);
        int execute(const char* query);

    private:
        int dbTag;
        int dbRun;
        int CommitTag;

        bool connection;
        MYSQL mysql;

        char* query;
        int sizeQuery;
        int sizeColumnString;
};

#endif
