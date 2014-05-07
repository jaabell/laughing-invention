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
// $Date: 2004/04/12 19:11:51 $
// $Source: /usr/local/cvs/OpenSees/SRC/matrix/ID.h,v $


// Written: fmk
// Revision: A
//
// Description: This file contains the class definition for ID.
// ID is a concrete class implementing the integer array abstraction.
// ID objects are Vectors of integers which only need a few
// operators defined on them.
//
// What: "@(#) ID.h, revA"


#ifndef ID_h
#define ID_h

#include <OPS_Globals.h>
#include <iostream>


// Boris Jeremic 17Nov2008
#include <iostream>
#ifdef _BABAK_DEBUG
#include <cstdlib>
#include <exception>
#endif
using namespace std;


class ID
{
    public:
        // constructors and destructor
        ID();
        ID(int);
        ID(int size, int arraySize);
        ID(int *data, int size, bool cleanIt = false);
        ID(const ID &);
        ~ID();

        // utility methods
        int Size(void) const;
        void Zero(void);
        int setData(int *newData, int size, bool cleanIt = false);
        int resize(int newSize);

        // overloaded operators
        inline int &operator()(int x);
        inline int operator()(int x) const;
        int &operator[](int);

        ID &operator=(const ID  &V);

        int getLocation(int value) const;
        int removeValue(int value);

        //        friend ostream& operator<<(ostream& s, const ID& V);
        friend ostream &operator<<(ostream &s, const ID &V);
        //    friend istream &operator>>(istream &s, ID &V);

        friend class UDP_Socket;
        friend class TCP_Socket;
        friend class TCP_SocketNoDelay;
        friend class MPI_Channel;
        friend class OutputWriter;
        friend class H5OutputWriter;
        friend class HDF5_Channel;
        friend class MySqlDatastore;
        friend class MySqlDataRecorder;
        friend class BerkeleyDbDatastore;

    private:
        static int ID_NOT_VALID_ENTRY;
        int sz;
        int *data;
        int arraySize;
        int fromFree;
};


inline int
ID::Size(void) const
{
    return sz;
}

inline int &
ID::operator()(int x)
{
#ifdef _G3DEBUG

    // check if it is inside range [0,sz-1]
    if (x < 0 || x >= sz)
    {
        std::cerr << "ID::(loc) - loc " << x << " outside range 0 - " <<  sz - 1 << endln;
#ifdef _BABAK_DEBUG
        abort();
#endif
        return ID_NOT_VALID_ENTRY;
    }

#endif


    return data[x];
}

inline int
ID::operator()(int x) const
{
    //#ifdef _G3DEBUG
    // check if it is inside range [0,sz-1]
    if (x < 0 || x >= sz)
    {
        cerr << "ID::(loc) - loc " << x << " outside range 0 - " <<  sz - 1 << endln;
#ifdef _BABAK_DEBUG
        abort();
#endif
        return ID_NOT_VALID_ENTRY;
    }

    //#endif

    return data[x];
}

#endif


