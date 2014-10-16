///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Mysql data recorder
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              January 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



#ifndef MySqlDataRecorder_h
#define MySqlDataRecorder_h


// Description: This file contains the class definition for MySqlDataRecorder.
// MySqlDataRecorder is a concrete subclas of FE_Datastore. A MySqlDataRecorder
// object is used in the program to store/restore the geometry and state
// information in a domain at a particular instance in the analysis. The
// information is stored in a MySQL Database which can be local or remote.
//
// What: "@(#) MySqlDataRecorder.h, revA"

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



class MySqlDataRecorder: public FE_Datastore
{
    public:


        MySqlDataRecorder(const char* databaseName,
                          const char* host,
                          const char* user,
                          const char* passwd,
                          unsigned int port,
                          const char* socket,
                          Domain& theDomain,
                          FEM_ObjectBroker& theBroker);

        ~MySqlDataRecorder();



        int sendMsg(int dbTag, int commitTag,
                    const Message&,
                    ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int receiveMsg(int dbTag, int commitTag,
                    Message&,
                    ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int sendMatrix(int dbTag, int commitTag,
                       const Matrix& theMatrix,
                       ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int receiveMatrix(int dbTag, int commitTag,
                       Matrix& theMatrix,
                       ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int sendVector(int dbTag, int commitTag,
                       const Vector& theVector,
                       ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int receiveVector(int dbTag, int commitTag,
                       Vector& theVector,
                       ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int sendID(int dbTag, int commitTag,
                   const ID& theID,
                   ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int receiveID(int dbTag, int commitTag,
                   ID& theID,
                   ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int sendnDarray(int dbTag, int commitTag,
                        const nDarray& theNDarray,
                        ChannelAddress* theAddress = 0)
        {
            return 0;
        };

        int receivenDarray(int dbTag, int commitTag,
                        nDarray& theNDarray,
                        ChannelAddress* theAddress = 0)
        {
            return 0;
        };



        int sendquery(int commitTag, char* query,
                      ChannelAddress* theAddress = 0);

        double restoreNodalDisplacement(int NodeNumber, int dof, int stepNumber);


        int createDisplacementTables(int StepNumber);
        int create8GPBrickStressTables(int StepNumber);
        int create27GPBrickStressTables(int StepNumber);
        int createLineElementForceTables(int StepNumber);


    protected:
        int createDatabase(const char* projectName);


    private:

        bool connection;
        MYSQL mysql;

        char* query;
        int sizeQuery;
};

#endif
