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

// $Revision: 1.7 $
// $Date: 2004/04/12 19:11:31 $
// $Source: /usr/local/cvs/OpenSees/SRC/matrix/ID.cpp,v $


// Written: fmk
// Revision: A
//
// Description: This file contains the class implementation for ID.
//
// What: "@(#) ID.C, revA"

#include "ID.h"
#include <stdlib.h>
#include <iostream>
using namespace std;



int ID::ID_NOT_VALID_ENTRY = 0;

// ID():
//  Standard constructor, sets size = 0;

ID::ID()
    : sz(0), data(0), arraySize(0), fromFree(0)
{

}


// ID(int size):
//  Constructor used to allocate a ID of size size.

ID::ID(int size)
    : sz(size), data(0), arraySize(size), fromFree(0)
{

    //#ifdef _G3DEBUG
    if (sz < 0)
    {
        cerr << "ID::ID(int) - size " << size << " specified < 0\n";
        sz = 1;
        arraySize = 1;
    }

    //#endif

    // create the space for the data & check space was available
    //  data = (int *)malloc(size*sizeof(int));
    data = new int[sz];

    if (data == 0)
    {
        cerr << "ID::ID(int): ran out of memory with size " << size << endln;
        exit(-1);
    }

    // zero the data
    for (int i = 0; i < size; i++)
    {
        data[i] = 0;
    }
}


// ID(int size):
//  Fill constructor used to allocate a ID of size size, but available memory arraySz,
// optionally provide a fill value

ID::ID(int size, int arraySz, int fillValue)
    : sz(size), data(0), arraySize(arraySz), fromFree(0)
{
    //#ifdef _G3DEBUG
    if (sz < 0)
    {
        cerr << "ID::ID(size, arraySize) - size " << size << " specified < 0\n";
        sz = 0;
    }

    if (arraySz <= 0)
    {
        cerr << "ID::ID(size, arraySize) - arraySize " << arraySz << " specified < 0\n";

        if (sz != 0)
        {
            arraySz = sz;
        }
        else
        {
            arraySz = 1;
        }
    }

    if (arraySz < sz)
    {
        cerr << "ID::ID(size, arraySize) - arraySize " << arraySz  << " specified < " << size << endln;
        arraySz = sz;
    }

    //#endif

    // create the space
    //  data = (int *)malloc(arraySize*sizeof(int));
    data = new int[arraySize];

    if (data == 0)
    {
        cerr << "ID::ID(int, int): ran out of memory with arraySize: " << arraySize << endln;
        exit(-1);
    }

    // fill the data
    for (int i = 0; i < arraySize; i++)
    {
        data[i] = fillValue;
    }
}

ID::ID(int *d, int size, bool cleanIt)
    : sz(size), data(d), arraySize(size), fromFree(1)
{
    if (d == 0)   // OOPS must have been other constructor we wanted
    {
        sz = 0;
        data = 0;
        arraySize = size;
        fromFree = 0;

        // create the space
        if (arraySize != 0)
        {
            //data = (int *)malloc(arraySize*sizeof(int));
            data = new int[arraySize];

            if (data == 0)
            {
                cerr << "ID::ID(int, int): ran out of memory with arraySize " << arraySize << endln;
                exit(-1);
            }
        }

        // zero the data
        for (int i = 0; i < arraySize; i++)
        {
            data[i] = 0;
        }
    }

    if (cleanIt == true)
    {
        fromFree = 0;
    }
}

// ID(const ID&):
//  Constructor to init a ID from another.

ID::ID(const ID &other)
    : sz(other.sz), data(0), arraySize(other.arraySize), fromFree(0)
{
    // create the space
    //  data = (int *)malloc(arraySize*sizeof(int));
    data = new int[arraySize];

    if (data == 0)
    {
        cerr << "ID::ID(ID): ran out of memory with arraySize " << arraySize << endln,
             exit(-1);
    }

    // copy the data
    for (int i = 0; i < sz; i++)
    {
        data[i] = other.data[i];
    }
}



// ~ID():
//  destructor, deletes the [] data

ID::~ID()
{
    if (data != 0 && fromFree == 0)
        //    free((void *)data);
    {
        delete [] data;
    }
}

int
ID::setData(int *newData, int size, bool cleanIt)
{
    if (data != 0 && fromFree == 0)
        //    free((void *)data);
    {
        delete [] data;
    }

    sz = size;
    data = newData;

    if (cleanIt == false)
    {
        fromFree = 1;
    }
    else
    {
        fromFree = 0;
    }

    if (sz < 0)
    {
        cerr << "ID::ID(int *, size) - size " << size << " specified < 0\n";
        sz = 0;
    }

    return 0;
}


void
ID::Zero(void)
{
    for (int i = 0; i < sz; i++)
    {
        data[i] = 0;
    }
}

int
ID::getLocation(int value) const
{
    // search through ID for the value
    for (int i = 0; i < sz; i++)
        if (data[i] == value)
        {
            return i;
        }

    // if we get here the value is not in the array
    return -1;
}

int
ID::removeValue(int value)
{
    int place = -1;

    for (int i = 0; i < sz; i++)
        if (data[i] == value)
        {
            place = i;

            // copy the rest of the components down one in ID
            for (int j = i; j < sz - 1; j++)
            {
                data[j] = data[j + 1];
            }

            sz--;
        }

    return place;
}


int &
ID::operator[](int x)
{

    //#ifdef _G3DEBUG
    // check if it is inside range [0,sz-1]
    if (x < 0)
    {
        cerr << "ID::[] - location " << x << " < 0\n";
        return ID_NOT_VALID_ENTRY;
    }

    //#endif

    // see if quick return
    if (x < sz)
    {
        return data[x];
    }

    /*
     * otherwise we have to enlarge the order of the ID
     */

    // see if we can just enlarge the array
    // without having to go get more space

    if (x < arraySize)
    {
        for (int i = sz; i < x; i++)
        {
            data[i] = 0;
        }

        sz = x + 1;
        return data[x];
    }

    // otherwise we go get more space
    if (x >= arraySize)
    {
        // int newArraySize = x + 1 ;
        int newArraySize = 2 * x ; //This amortizes the allocation costs for arrays that grow by one element at a time. At the cost of memory, of course.
        int *newData = new int[newArraySize];

        if (newData != 0)
        {
            for (int i = 0; i < sz; i++)
            {
                newData[i] = data[i];
            }

            for (int j = sz; j < arraySize; j++)
            {
                newData[j] = 0;
            }

            sz = x + 1;
            delete [] data;
            data = newData;
            arraySize = newArraySize;
            return data[x];
        }
        else
        {
            // we could not allocate more mem .. leave the current size
            cerr << "ID::[]): ran out of memory with arraySize " << arraySize << endln;
            return ID_NOT_VALID_ENTRY;
        }
    }

    // we should never get here, but some compilers need this line
    return ID_NOT_VALID_ENTRY;
}



// ID &operator=(const ID  &V):
//  the assignment operator, This is assigned to be a copy of V. if sizes
//  are not compatable this.data [] is deleted. The data pointers will not
//  point to the same area in mem after the assignment.
//


int
ID::resize(int newSize)
{

    // first check that newSize is valid
    if (newSize <= 0)
    {
        cerr << "ID::resize() - size specified " << newSize << " <= 0\n";
        return -1;
    }


    if (sz > newSize)
    {

        // is size smaller than current, simply reset sz
        sz = newSize;

    }
    else if (newSize < arraySize)
    {

        // see if we can just enlarge the array
        // without having to go get more space

        for (int i = sz; i < newSize; i++)
        {
            data[i] = 0;
        }

        sz = newSize;

    }
    else if (newSize > arraySize)
    {

        // otherwise we go get more space

        int *newData = new int[newSize];

        if (newData != 0)
        {
            // copy the old
            for (int i = 0; i < sz; i++)
            {
                newData[i] = data[i];
            }

            // zero the new
            for (int j = sz; j < newSize; j++)
            {
                newData[j] = 0;
            }

            sz = newSize;
            // release the memory held by the old
            //      free((void *)data);
            delete [] data;
            data = newData;
            arraySize = newSize;

        }
        else
        {
            cerr << "ID::resize() - out of memory creating ID of size " << newSize << "\n";
            return -1;
        }
    }

    return 0;
}




ID &
ID::operator=(const ID &V)
{
    // first check we are not trying v = v
    if (this != &V)
    {

        // check size compatability, if different delete
        // old and make room for new.
        if (sz != V.sz)
        {
            if (arraySize < V.sz)
            {
                arraySize = V.sz;

                if (data != 0)
                    //free((void *)data);
                {
                    delete [] data;
                }

                //      data = (int *)malloc(arraySize*sizeof(int));
                data = new int[arraySize];

                // check we got the memory requested
                if (data == 0)
                {
                    cerr << "WARNING ID::=(ID) - ran out of memory ";
                    cerr << "for new array of size" << arraySize << endln;
                    sz = 0;
                    arraySize = 0;
                }
            }

            sz = V.sz;
        }

        // copy the data
        for (int i = 0; i < sz; i++)
        {
            data[i] = V(i);
        }
    }

    return *this;
}





// friend ostream &operator<<(ostream &s, const ID &V)
//  A function is defined to allow user to print the IDs using ostreams.



ostream &operator<<(ostream &s, const ID &V)
{
    for (int i = 0; i < V.Size(); i++)
    {
        s << V(i) << " ";
    }

    return s << endl;
}

// friend istream &operator>>(istream &s, ID &V)
//  A function is defined to allow user to input the data into a ID which has already
//  been constructed with data, i.e. ID(int) or ID(const ID &) constructors.

/*
istream &operator>>(istream &s, ID &V)
{
    for (int i=0; i<V.Size(); i++)
    s >> V(i);

    return s;
}
*/


bool ID::operator==(const ID &rhs) const
{
    if (!(sz == rhs.sz))
    {
        return false;
    }
    for (int i = 0; i < sz; i++)
    {
        if (! (data[i] == rhs.data[i]))
        {
            return false;
        }
    }
    return true;
}
