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

// $Revision: 1.2 $
// $Date: 2003/10/15 00:26:32 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/channel/MPI_Channel.h,v $


// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class definition for MPI_Channel.
// MPI_Channel is a sub-class of channel. It is implemented with Berkeley
// stream sockets using the TCP protocol. Messages delivery is garaunteed.
// Communication is full-duplex between a pair of connected sockets.

#ifndef MPI_Channel_h
#define MPI_Channel_h

#include <Channel.h>
#include <mpi.h>
#include <string>

class MPI_Channel : public Channel
{
public:
  MPI_Channel(int otherProcess);
  ~MPI_Channel();

  char* addToProgram(void);

  virtual int setUpConnection(void);

  int setNextAddress(const ChannelAddress& otherChannelAddress);
  virtual ChannelAddress* getLastSendersAddress(void);

  int sendObj(int commitTag,
              MovableObject& theObject,
              ChannelAddress* theAddress = 0);

  int receiveObj(int commitTag,
                 MovableObject& theObject,
                 FEM_ObjectBroker& theBroker,
                 ChannelAddress* theAddress = 0);

  int receiveMsg(int dbTag, int commitTag, Message&, ChannelAddress* theAddress = 0);
  int sendMsg(int dbTag, int commitTag, const Message&, ChannelAddress* theAddress = 0);

  int sendMatrix(int dbTag, int commitTag, const Matrix& theMatrix, ChannelAddress* theAddress = 0);
  int receiveMatrix(int dbTag, int commitTag, Matrix& theMatrix, ChannelAddress* theAddress = 0);

  int sendVector(int dbTag, int commitTag, const Vector& theVector, ChannelAddress* theAddress = 0);
  int receiveVector(int dbTag, int commitTag, Vector& theVector, ChannelAddress* theAddress = 0);

  int sendID(int dbTag, int commitTag, const ID& theID, ChannelAddress* theAddress = 0);
  int receiveID(int dbTag, int commitTag, ID& theID, ChannelAddress* theAddress = 0);

  //Guanzhou added
  int sendnDarray(int dbTag, int commitTag, const nDarray& theNDarray, ChannelAddress* theAddress = 0);
  int receivenDarray(int dbTag, int commitTag, nDarray& theNDarray, ChannelAddress* theAddress = 0);

  //Jose Added
  int sendString(int dbTag, int commitTag,
                 const std::string &theString,
                 ChannelAddress *theAddress = 0) ;

  int receiveString(int dbTag, int commitTag,
                    std::string &theString,
                    ChannelAddress *theAddress = 0);
  virtual int sendDTensor2(int dbTag, int commitTag,
                           const DTensor2 &theTensor,
                           ChannelAddress *theAddress = 0) ;

  virtual int receiveDTensor2(int dbTag, int commitTag,
                              DTensor2 &theTensor,
                              ChannelAddress *theAddress = 0) ;
  virtual int sendDTensor4(int dbTag, int commitTag,
                           const DTensor4 &theTensor,
                           ChannelAddress *theAddress = 0) ;

  virtual int receiveDTensor4(int dbTag, int commitTag,
                              DTensor4 &theTensor,
                              ChannelAddress *theAddress = 0) ;





protected:

private:
  int otherTag;
  MPI_Comm otherComm;
};


#endif








