
#ifndef OracleDatastore_h
#define OracleDatastore_h

// $Revision: 1.1 $
// $Date: 2003/07/15 21:20:28 $
// $Source: /usr/local/cvs/OpenSees/SRC/database/OracleDatastore.h,v $

// Written:  Jun Peng
//
// Description: This file contains the class definition for OracleDatastore.
// OracleDatastore is a concrete subclas of FE_Datastore. A OracleDatastore
// object is used in the program to store/restore the geometry and state
// information in a domain at a particular instance in the analysis. The
// information is stored in ORACLE8i Database.
//
// What: "@(#) OracleDatastore.h, revA"

#include <FE_Datastore.h>
#include <fstream>

class FEM_ObjectBroker;


class OracleDatastore: public FE_Datastore
{
    public:
        OracleDatastore(char* dataBase,
                        Domain& theDomain,
                        FEM_ObjectBroker& theBroker);

        ~OracleDatastore();

        // methods to set and get a project tag
        int getProjTag();
        void setProjTag(int projectID);

        // method to search for project Tag
        int searchProjTag(char* projName);

        // method to get a database tag
        int getDbTag(void);

        // methods defined in the Channel class interface which do nothing
        char* addToProgram(void);
        int setUpShadow(void);
        int setUpActor(void);
        int setNextAddress(const ChannelAddress& otherChannelAddress);
        ChannelAddress* getLastSendersAddress(void);

        int commitState(int commitTag);

        // methods for sending and receiving the data
        int sendObj(int commitTag,
                    MovableObject& theObject,
                    ChannelAddress* theAddress = 0);
        int receiveObj(int commitTag,
                    MovableObject& theObject,
                    FEM_ObjectBroker& theBroker,
                    ChannelAddress* theAddress = 0);

        int sendMsg(int dbTag, int commitTag,
                    const Message&,
                    ChannelAddress* theAddress = 0);
        int receiveMsg(int dbTag, int commitTag,
                    Message&,
                    ChannelAddress* theAddress = 0);

        int sendMatrix(int dbTag, int commitTag,
                       const Matrix& theMatrix,
                       ChannelAddress* theAddress = 0);
        int receiveMatrix(int dbTag, int commitTag,
                       Matrix& theMatrix,
                       ChannelAddress* theAddress = 0);

        int sendVector(int dbTag, int commitTag,
                       const Vector& theVector, ChannelAddress* theAddress = 0);
        int receiveVector(int dbTag, int commitTag,
                       Vector& theVector,
                       ChannelAddress* theAddress = 0);

        int sendID(int dbTag, int commitTag,
                   const ID& theID,
                   ChannelAddress* theAddress = 0);
        int receiveID(int dbTag, int commitTag,
                   ID& theID,
                   ChannelAddress* theAddress = 0);

    protected:

    private:

        // private attributes
        int dbTag;
        int projTag;
        char dataBase[50];

        int lastDomainChangeStamp;

};


#endif

