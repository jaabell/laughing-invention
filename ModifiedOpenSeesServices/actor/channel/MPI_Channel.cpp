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
// $Date: 2003/10/15 00:26:32 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/channel/MPI_Channel.cpp,v $


// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the implementation of the methods needed
// to define the MPI_Channel class interface.

#include <MPI_Channel.h>
#include <Matrix.h>
#include <ID.h>
#include <Vector.h>
#include <Message.h>
#include <MPI_ChannelAddress.h>
#include <MovableObject.h>
#include <nDarray.h>//Guanzhou added
#include <iostream>
#include <LTensor.h>

using namespace std;


// MPI_Channel(unsigned int other_Port, char *other_InetAddr):
//  constructor to open a socket with my inet_addr and with a port number
//  given by the OS.

#define GLOBAL_MSG_TAG 99 //Guanzhou added for point-to-point msg communication


MPI_Channel::MPI_Channel(int other)
    : otherTag(other), otherComm(MPI_COMM_WORLD)
{

}

// ~MPI_Channel():
//  destructor

MPI_Channel::~MPI_Channel()
{

}


int
MPI_Channel::setUpConnection(void)
{
    return 0;
}


ChannelAddress *
MPI_Channel::getLastSendersAddress(void)
{
    // cerr << "MPI_Channel::getLastSendersAddress(void) - ";
    // cerr << " this should not be called - need MPI-2.0 to use\n";

    // Not really used apparently

    return 0;
}


int
MPI_Channel::setNextAddress(const ChannelAddress &theAddress)
{
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress.getType() == MPI_TYPE)
    {
        theMPI_ChannelAddress = (MPI_ChannelAddress *)(&theAddress);
        otherTag = theMPI_ChannelAddress->otherTag;
        otherComm = theMPI_ChannelAddress->otherComm;
    }
    else
    {
        cerr << "MPI_Channel::setNextAddress() - an MPI_Channel ";
        cerr << "can only communicate with an MPI_Channel";
        cerr << " address given is not of type MPI_ChannelAddress\n";
        return -1;
    }

    return 0;
}



int
MPI_Channel::sendObj(int commitTag,
                     MovableObject &theObject,
                     ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendObj() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }


    return theObject.sendSelf(commitTag, *this);
}

int
MPI_Channel::receiveObj(int commitTag,
                        MovableObject &theObject,
                        FEM_ObjectBroker &theBroker,
                        ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveObj() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    return theObject.receiveSelf(commitTag, *this, theBroker);
}


// void Recv(Message &):
//  Method to receive a message, also sets other_Addr to that of sender

int
MPI_Channel::receiveMsg(int dbTag, int commitTag, Message &msg, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::recvMesg() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the message and
    // place the incoming data there
    int nleft;//, nread;
    char *gMsg;
    gMsg = msg.data;
    nleft = msg.length;

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_CHAR, otherTag, GLOBAL_MSG_TAG, otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_CHAR, &count);

    if (count != nleft)
    {
        cerr << "MPI_Channel::recvMesg() -";
        cerr << " incorrect size of Message received ";
        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(Message &):
//  Method to send a message to an address given by other_Addr.

int
MPI_Channel::sendMsg(int dbTag, int commitTag, const Message &msg, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendMsg() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the message and
    // place the incoming data there
    int  nleft;
    char *gMsg;
    gMsg = msg.data;
    nleft = msg.length;

    MPI_Send((void *)gMsg, nleft, MPI_CHAR, otherTag, GLOBAL_MSG_TAG, otherComm);
    return 0;
}

int
MPI_Channel::receiveMatrix(int dbTag, int commitTag, Matrix &theMatrix, ChannelAddress *theAddress)

{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveMatrix() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the Matrix and
    // place the incoming data there
    int nleft;//, nread;
    double *data = theMatrix.data;
    char *gMsg = (char *)data;;
    nleft =  theMatrix.dataSize;

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG,
             otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_DOUBLE, &count);

    if (count != nleft)
    {
        cerr << "MPI_Channel::receiveMatrix() -";
        cerr << " incorrect number of entries for Matrix received: " << count << "\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(Matrix &):
//  Method to send a Matrix to an address given by other_Addr.

int
MPI_Channel::sendMatrix(int dbTag, int commitTag, const Matrix &theMatrix, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendMatrix() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the Matrix and
    // place the incoming data there
    int  nleft;
    double *data = theMatrix.data;
    char *gMsg = (char *)data;
    nleft =  theMatrix.dataSize;

    MPI_Send((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm);

    return 0;
}








int
MPI_Channel::receiveVector(int dbTag, int commitTag, Vector &theVector, ChannelAddress *theAddress)

{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveVector() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the Vector and
    // place the incoming data there
    int nleft;//, nread;
    double *data = theVector.theData;
    char *gMsg = (char *)data;;
    nleft =  theVector.sz;

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_DOUBLE, &count);

    if (count != nleft)
    {
        cerr << "MPI_Channel::receiveVector() -";
        cerr << " incorrect number of entries for Vector received: " << count <<
             " expected: " << theVector.sz << "\n";

        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(Vector &):
//  Method to send a Vector to an address given by other_Addr.

int
MPI_Channel::sendVector(int dbTag, int commitTag, const Vector &theVector, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendVector() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the Vector and
    // place the incoming data there
    int nleft;
    double *data = theVector.theData;
    char *gMsg = (char *)data;
    nleft =  theVector.sz;

    MPI_Send((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm);

    return 0;
}




int
MPI_Channel::receiveID(int dbTag, int commitTag, ID &theID, ChannelAddress *theAddress)
{

    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveID() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the ID and
    // place the incoming data there
    int nleft;//, nread;
    int *data = theID.data;
    char *gMsg = (char *)data;;
    nleft =  theID.sz;

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_INT, otherTag, GLOBAL_MSG_TAG, otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_INT, &count);

    //    int rank;
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //cerr << "MPI_Channel::receiveID " << rank << " " << otherTag << " " << theID;

    if (count != nleft)
    {
        cerr << "MPI_Channel::receiveID() -";
        cerr << " incorrect number of entries for ID received: " << count << "\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(ID &):
//  Method to send a ID to an address given by other_Addr.

int
MPI_Channel::sendID(int dbTag, int commitTag, const ID &theID, ChannelAddress *theAddress)
{

    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendID() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the ID and
    // place the incoming data there
    int  nleft;
    int *data = theID.data;
    char *gMsg = (char *)data;
    nleft =  theID.sz;

    MPI_Send((void *)gMsg, nleft, MPI_INT, otherTag, GLOBAL_MSG_TAG, otherComm);

    // int rank;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //    cerr << "MPI_Channel::sendID " << rank << " " << otherTag << " " << theID;

    return 0;
}

//Guanzhou added
int
MPI_Channel::sendnDarray(int dbTag, int commitTag, const nDarray &theNDarray, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendnDarray() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    double *data = (theNDarray.pc_nDarray_rep)->pd_nDdata;
    char *gmsg = (char *)data;
    // int *dim =  (theNDarray.pc_nDarray_rep)->dim;
    // int rank =  (theNDarray.pc_nDarray_rep)->nDarray_rank;
    int elem =   (theNDarray.pc_nDarray_rep)->total_numb;
    MPI_Send((void *)gmsg, elem, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm);
    //    gmsg = (char*)dim;
    //    MPI_Send((void *)gmsg, rank, MPI_INT, otherTag, 0, otherComm);

    return 0;
}

//Guanzhou added
int
MPI_Channel::receivenDarray(int dbTag, int commitTag, nDarray &theNDarray, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendnDarray() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    double *data = (theNDarray.pc_nDarray_rep)->pd_nDdata;
    char *gmsg = (char *)data;

    int elem =   (theNDarray.pc_nDarray_rep)->total_numb;

    MPI_Status status;
    MPI_Recv((void *)gmsg, elem, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_DOUBLE, &count);

    if (count != elem)
    {
        cerr << "MPI_Channel::receivenDarray() -";
        cerr << " incorrect number of entries for nDarray recieved ";
        return -1;
    }


    return 0;
}


/*
int
MPI_Channel::getPortNumber(void) const
{
    return otherTag;
}
*/

char *
MPI_Channel::addToProgram(void)
{
    cerr << "MPI_Channel::addToProgram(void) - ";
    cerr << " this should not be called - need MPI-2.0\n";
    char *newStuff = (char *)malloc(10 * sizeof(char));

    for (int i = 0; i < 10; i++)
    {
        newStuff[i] = ' ';
    }

    return newStuff;
}


int MPI_Channel::sendString(int dbTag, int commitTag,
                            const std::string &theString,
                            ChannelAddress *theAddress)
{
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendString() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // double *data = (theNDarray.pc_nDarray_rep)->pd_nDdata;
    char *gmsg = (char *)theString.c_str();
    int nchars =  theString.size() + 1;
    // int rank =  (theNDarray.pc_nDarray_rep)->nDarray_rank;
    // int elem =   (theNDarray.pc_nDarray_rep)->total_numb;

    // cout << "Sending string of size: " << nchars << endl;

    MPI_Send(&nchars, 1, MPI_INT, otherTag, GLOBAL_MSG_TAG, otherComm);


    // cout << "Sending string: " << theString << endl;
    MPI_Send((void *)gmsg, nchars, MPI_CHAR, otherTag, GLOBAL_MSG_TAG, otherComm);

    return 0;
}

int MPI_Channel::receiveString(int dbTag, int commitTag,
                               std::string &theString,
                               ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveString() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    int nchar;

    MPI_Status status;
    MPI_Recv(&nchar, 1, MPI_INT, otherTag, GLOBAL_MSG_TAG, otherComm, &status);

    // cout << "Receiving string of size " << nchar << endl;


    char *gmsg = new char[nchar];

    MPI_Recv((void *)gmsg, nchar, MPI_CHAR, otherTag, GLOBAL_MSG_TAG, otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_CHAR, &count);

    if (count != nchar)
    {
        cerr << "MPI_Channel::receiveString() -";
        cerr << " incorrect number of entries for std::string recieved ";
        return -1;
    }

    // cout << "Got string: " << gmsg << endl;

    theString = std::string(gmsg);

    // cout << "Turned it into string: " << theString << endl;
    return 0;
}




int
MPI_Channel::receiveDTensor2(int dbTag, int commitTag, DTensor2 &theDTensor2, ChannelAddress *theAddress)

{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveDTensor2() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the DTensor2 and
    // place the incoming data there
    int nleft;//, nread;
    double *data = theDTensor2.data;
    char *gMsg = (char *) data;;
    nleft =  theDTensor2.get_size();

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG,
             otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_DOUBLE, &count);

    if (count != nleft)
    {
        cerr << "MPI_Channel::receiveDTensor2() -";
        cerr << " incorrect number of entries for DTensor2 received: " << count << "\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(DTensor2 &):
//  Method to send a DTensor2 to an address given by other_Addr.

int
MPI_Channel::sendDTensor2(int dbTag, int commitTag, const DTensor2 &theDTensor2, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)

        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendDTensor2() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }
    // TinyArray_base* base = static_cast<TinyArray_base*>(&theDTensor2);
    // if o.k. get a ponter to the data in the DTensor2 and
    // place the incoming data there
    int  nleft;
    double *data = theDTensor2.data;
    // double &data = (*base)(0, 0);
    char *gMsg = (char *)data;
    nleft =  theDTensor2.get_size();

    MPI_Send((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm);

    return 0;
}




int
MPI_Channel::receiveDTensor4(int dbTag, int commitTag, DTensor4 &theDTensor4, ChannelAddress *theAddress)

{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)
        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::receiveDTensor4() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }

    // if o.k. get a ponter to the data in the DTensor4 and
    // place the incoming data there
    int nleft;//, nread;
    double *data = theDTensor4.data;
    char *gMsg = (char *) data;;
    nleft =  theDTensor4.get_size();

    MPI_Status status;
    MPI_Recv((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG,
             otherComm, &status);
    int count = 0;
    MPI_Get_count(&status, MPI_DOUBLE, &count);

    if (count != nleft)
    {
        cerr << "MPI_Channel::receiveDTensor4() -";
        cerr << " incorrect number of entries for DTensor4 received: " << count << "\n";
        return -1;
    }
    else
    {
        return 0;
    }
}


// void Send(DTensor4 &):
//  Method to send a DTensor4 to an address given by other_Addr.

int
MPI_Channel::sendDTensor4(int dbTag, int commitTag, const DTensor4 &theDTensor4, ChannelAddress *theAddress)
{
    // first check address is the only address a MPI_Channel can send to
    MPI_ChannelAddress *theMPI_ChannelAddress = 0;

    if (theAddress != 0)
    {
        if (theAddress->getType() == MPI_TYPE)

        {
            theMPI_ChannelAddress = (MPI_ChannelAddress *)theAddress;
            otherTag = theMPI_ChannelAddress->otherTag;
            otherComm = theMPI_ChannelAddress->otherComm;
        }
        else
        {
            cerr << "MPI_Channel::sendDTensor4() - a MPI_Channel ";
            cerr << "can only communicate with a MPI_Channel";
            cerr << " address given is not of type MPI_ChannelAddress\n";
            return -1;
        }
    }
    // TinyArray_base* base = static_cast<TinyArray_base*>(&theDTensor4);
    // if o.k. get a ponter to the data in the DTensor4 and
    // place the incoming data there
    int  nleft;
    double *data = theDTensor4.data;
    // double &data = (*base)(0, 0);
    char *gMsg = (char *)data;
    nleft =  theDTensor4.get_size();

    MPI_Send((void *)gMsg, nleft, MPI_DOUBLE, otherTag, GLOBAL_MSG_TAG, otherComm);

    return 0;
}


