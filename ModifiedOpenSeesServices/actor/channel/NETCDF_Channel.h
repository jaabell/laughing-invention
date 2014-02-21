// ===============================================================================
// ESSI Simulator Program - UCD CompGeoMech
//
// COPYRIGHT (C):     ^_^
// PROJECT:           ESSI Simulator
// FILE:              NETCDF_Channel.h
// CLASS:             NETCDF_Channel
// VERSION:
// LANGUAGE:          C++ /
// TARGET OS:         UNIX || maybe something else
// DESIGNER:          Jose Abell, Babak Kamaranimogghadam, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              Feb 2014
// UPDATE HISTORY:
//
// ===============================================================================



#ifndef NECDF_Channel_h
#define NECDF_Channel_h


class ChannelAddress;
class Message;
class MovableObject;
class Matrix;
class Vector;
class ID;
class FEM_ObjectBroker;
class nDarray;
class LTensor;

class NETCDF_Channel
{
    public:
        NETCDF_Channel ();
        virtual ~NETCDF_Channel();

        // methods to set up the NETCDF_channel in an actor space
        virtual char *addToProgram(void) = 0;
        virtual int setUpConnection(void) = 0;
        virtual int setNextAddress(const ChannelAddress &theAddress) = 0;
        virtual ChannelAddress *getLastSendersAddress(void) = 0;

        virtual int isDatastore(void);
        virtual int getDbTag(void);
        int getTag(void);

        // methods to send/receive messages and objects on NETCDF_channels.
        virtual int sendObj(int commitTag,
                            MovableObject &theObject,
                            ChannelAddress *theAddress = 0) = 0;

        virtual int recvObj(int commitTag,
                            MovableObject &theObject,
                            FEM_ObjectBroker &theBroker,
                            ChannelAddress *theAddress = 0) = 0;

        virtual int sendMsg(int dbTag, int commitTag,
                            const Message &theMessage,
                            ChannelAddress *theAddress = 0) = 0;

        virtual int recvMsg(int dbTag, int commitTag,
                            Message &theMessage,
                            ChannelAddress *theAddress = 0) = 0;

        virtual int sendMatrix(int dbTag, int commitTag,
                               const Matrix &theMatrix,
                               ChannelAddress *theAddress = 0) = 0;

        virtual int recvMatrix(int dbTag, int commitTag,
                               Matrix &theMatrix,
                               ChannelAddress *theAddress = 0) = 0;

        virtual int sendVector(int dbTag, int commitTag,
                               const Vector &theVector,
                               ChannelAddress *theAddress = 0) = 0;

        virtual int recvVector(int dbTag, int commitTag,
                               Vector &theVector,
                               ChannelAddress *theAddress = 0) = 0;

        virtual int sendID(int dbTag, int commitTag,
                           const ID &theID,
                           ChannelAddress *theAddress = 0) = 0;

        virtual int recvID(int dbTag, int commitTag,
                           ID &theID,
                           ChannelAddress *theAddress = 0) = 0;

        //Guanzhou added
        virtual int sendnDarray(int dbTag, int commitTag,
                                const nDarray &theNDarray,
                                ChannelAddress *theAddress = 0) = 0;

        virtual int recvnDarray(int dbTag, int commitTag,
                                nDarray &theNDarray,
                                ChannelAddress *theAddress = 0) = 0;





    protected:

    private:
        static int numChannel;
        int tag;

};

#endif
