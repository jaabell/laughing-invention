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
// $Date: 2003/02/14 23:02:10 $
// $Source: /usr/local/cvs/OpenSees/SRC/tagged/storage/ArrayOfTaggedObjects.cpp,v $


// File: ~/tagged/storage/ArrayOfTaggedObjects.C
//
// Written: fmk
// Created: 11/96
// Revision: A
//
// Purpose: This file contains the implementation of the ArrayOfTaggedObjects
// class.
//
// What: "@(#) ArrayOfTaggedObjects.C, revA"

#include <TaggedObject.h>
#include <ArrayOfTaggedObjects.h>

#include <OPS_Globals.h>

#include <math.h> //Added by Babak 6/3/13

#include <iostream>
using namespace std;


ArrayOfTaggedObjects::ArrayOfTaggedObjects(int size)
    : numComponents(0), sizeComponentArray(0), positionLastEntry(0),
      positionLastNoFitEntry(0), fitFlag(true), theComponents(0),
      myIter(*this)
{
    // get the components array from the heap
    theComponents = new TaggedObject *[size];

    // check we obtained enough memory and set the array size
    if (theComponents == 0)
    {
        cerr << "ArrayOfTaggedObjects::ArrayOfTaggedObjects - failed to allocate an array of size " << size << endln;
    }
    else
    {
        sizeComponentArray = size;
    }

    // 0 the array
    for (int i = 0; i < sizeComponentArray; i++)
    {
        theComponents[i] = 0;
    }
}


ArrayOfTaggedObjects::~ArrayOfTaggedObjects()
{
    if (theComponents != 0)
    {
        delete [] theComponents;
    }
}



int
ArrayOfTaggedObjects::setSize(int newSize)
{
    // check a valid size
    if (newSize < 0 && newSize > sizeComponentArray)
    {
        cerr << "ArrayOfTaggedObjects::setSize - invalid size " << newSize << endln;
        return -1;
    }

    if (newSize < 2)
    {
        newSize = 2;    // make 2 the min size;
    }

    // get an array of pointers from the heap
    // NOTE: cannot use TaggedObject **newArray = new (TaggedObject *)[newSize];
    // with digital cxx compiler!!!!
    TaggedObject** newArray = new TaggedObject *[newSize];

    if (newArray == 0)
    {
        // cannot increase size to newSize, keep as is and return error
        cerr << "ArrayOfTaggedObjects::setSize - failed to allocate an array of size " << newSize << endln;
        return -2;
    }

    //
    // we got the space we needed .. now we populate the new array
    // trying to put everything in nicely, i.e. in location given by obj tags.
    //

    // first zero the new array
    for (int i = 0; i < newSize; i++)
    {
        newArray[i] = 0;
    }

    // store the needed info of the old array
    TaggedObject** oldArray = theComponents;
    int oldArrayLastEntry = positionLastEntry;

    // set data for new aray
    theComponents = newArray;
    sizeComponentArray = newSize;

    int error = 0;

    if (fitFlag == true && positionLastEntry <= newSize)
    {
        // we will copy old to new directly
        // as all fitted well last time this is just a straight assignment

        theComponents = newArray;
        sizeComponentArray = newSize;

        for (int i = 0; i <= positionLastEntry; i++)
        {
            theComponents[i] = oldArray[i];
        }
    }
    else
    {
        // we will see if the components fit in well this time

        // reset the parameters for the new array
        numComponents = 0;
        sizeComponentArray = newSize;
        positionLastEntry = 0;
        positionLastNoFitEntry = 0;
        fitFlag = true;

        // now add all the componets of the old array into the new one
        // we add using addComponent() to see if false fitFlag will now be true
        for (int j = 0; j <= oldArrayLastEntry; j++)
            if (oldArray[j] != 0)
                if (this->addComponent(oldArray[j]) == false)
                {
                    // this should never happen - but in case it does
                    cerr << "SERIOUS ERROR: ArrayOfTaggedObjects::setSize() - we have lost a component with tag: " <<
                         oldArray[j]->getTag() << endln;
                    error = -3;
                }
    }

    // delete the old array
    if (oldArray != 0)
    {
        delete [] oldArray;
    }

    return error;
}

//Implemented by Babak to optimize it to use in the graphs:
bool
ArrayOfTaggedObjects::addComponent_graph(TaggedObject* newComponent)
{
    // check to see that no other component already exists

    TaggedObject* other = this->getComponentPtr(newComponent->getTag());

    if (other != 0)
    {
        cerr << "WARNING ArrayOfTaggedObjects::addComponent_graph() - component with tag already exists, not adding component with tag: " << newComponent->getTag() << endln;
        return false;
    }


    // check to see if size of current array is big enough. if not resize.
    int newComponentTag = newComponent->getTag();
    int maxTemp = newComponentTag;

    if (sizeComponentArray >= newComponentTag)
    {
        maxTemp = sizeComponentArray;
    }

    if (numComponents <= maxTemp )
    {
        if (this->setSize( maxTemp + 1 ) < 0)
        {
            cerr << "ArrayOfTaggedObjects::addComponent_graph()- failed to enlarge the array with size" << (maxTemp + 1) << endln;
            return false;
        }
    }

    // we try to the Component in nicely, i.e. in
    // position given by the newComponents Tag


    if ((newComponentTag >= 0) && (newComponentTag < sizeComponentArray))
    {
        if (theComponents[newComponentTag] == 0)
        {
            // it will go in nicely

            theComponents[newComponentTag] = newComponent;
            numComponents ++;

            if (newComponentTag > positionLastEntry)
            {
                positionLastEntry = newComponentTag;
            }

            return true;
        }
    }


    return true;
}

//-------------------

bool
ArrayOfTaggedObjects::addComponent(TaggedObject* newComponent)
{
    // check to see that no other component already exists
    //    if (allowMultiple == false) {
    TaggedObject* other = this->getComponentPtr(newComponent->getTag());

    if (other != 0)
    {
        cerr << "WARNING ArrayOfTaggedObjects::addComponent() - component" <<
             " with tag already exists, not adding component with tag: " <<
             newComponent->getTag() << endln;
        return false;
    }

    //    }

    // we try to the Component in nicely, i.e. in
    // position given by the newComponents Tag

    int newComponentTag = newComponent->getTag();

    // check to see if size of current array is big enough. if not resize.
    // if (numComponents == sizeComponentArray)
    if ( sizeComponentArray - 1 < newComponentTag)
        if (this->setSize(2 * newComponentTag) < 0)
        {
            cerr << "ArrayOfTaggedObjects::addComponent()- failed to enlarge the array with size" <<
                 2 * numComponents << endln;
            return false;
        }

    if ((newComponentTag >= 0) && (newComponentTag < sizeComponentArray))
    {
        if (theComponents[newComponentTag] == 0)     // it will go in nicely
        {
            theComponents[newComponentTag] = newComponent;
            numComponents ++;

            if (newComponentTag > positionLastEntry)
            {
                positionLastEntry = newComponentTag;
            }

            return true;
        }
    }

    //Disable "fitting" the component. Result is too inefficient.
    return false;



    // // it won't go in nicely, so put wherever we can get it in
    // while (theComponents[positionLastNoFitEntry] != 0 &&
    //         positionLastNoFitEntry < sizeComponentArray )
    // {
    //     positionLastNoFitEntry++;
    // }

    // // just in case we don't get a location ..  though we should!!
    // if (positionLastNoFitEntry == sizeComponentArray)
    // {
    //     cerr << "ArrayOfTaggedObjects::addComponent() - could not - find a vacant spot after enlarging!!\n";
    //     return false;
    // }

    // theComponents[positionLastNoFitEntry] = newComponent;
    // numComponents++;

    // if (positionLastNoFitEntry > positionLastEntry)
    // {
    //     positionLastEntry = positionLastNoFitEntry;
    // }

    // fitFlag = false;

    // return true;  // o.k.
}

TaggedObject*
ArrayOfTaggedObjects::removeComponent(int tag)
{
    TaggedObject* removed;

    // first check to see if object is located at pos given by tag
    if ((tag >= 0) && (tag < sizeComponentArray))
    {

        // if all objects in nicely then has to be at location given by tag
        if (fitFlag == true)
        {
            removed = theComponents[tag];
            theComponents[tag] = 0;

            // if the object is there decrement numComponents added and
            // check if need to reset positionLastEntry marker
            if (removed != 0)
            {
                numComponents--;

                if (positionLastEntry == tag)
                {
                    for (int i = positionLastEntry; i >= 0; i--)
                    {
                        if (theComponents[i] != 0)
                        {
                            positionLastEntry = i;
                            i = -1; // break out of loop
                        }
                    }
                }
            }

            return removed;
        }
        else     // it still may be at nice location so do as above
        {
            if (theComponents[tag] != 0)
            {
                if ((theComponents[tag]->getTag()) == tag)
                {
                    removed = theComponents[tag];
                    theComponents[tag] = 0;

                    if (positionLastEntry == tag)
                    {
                        for (int i = positionLastEntry; i >= 0; i--)
                        {
                            if (theComponents[i] != 0)
                            {
                                positionLastEntry = i;
                                i = -1; // break out of loop
                            }
                        }
                    }

                    positionLastNoFitEntry = 0;
                    numComponents--;
                    return removed;
                }
            }
        }
    }

    // else we have to look through array until we find it or
    // reach lastPosition used
    for (int i = 0; i <= positionLastEntry; i++)
    {
        if (theComponents[i] != 0)
        {
            if (theComponents[i]->getTag() == tag)
            {

                // we found the component so do as above again
                // and then return a pointer to the component
                removed = theComponents[i];
                theComponents[i] = 0;

                if (positionLastEntry == i)
                {
                    for (int j = positionLastEntry; j >= 0; j--)
                    {
                        if (theComponents[j] != 0)
                        {
                            positionLastEntry = j;
                            j = -1; // break out of loop
                        }
                    }
                }

                positionLastNoFitEntry = 0;
                numComponents--;
                return removed;
            }
        }
    }

    // if get here the object is not stored in the array
    return 0;
}

int
ArrayOfTaggedObjects::getNumComponents(void) const
{
    return numComponents;
}

TaggedObject*
ArrayOfTaggedObjects::getComponentPtr(int tag)
{

    // first check it's not where we would like it
    if ((tag >= 0) && (tag < sizeComponentArray))
    {
        if (fitFlag == true) // either its at nice position or not entered
        {
            return theComponents[tag];
        }
        else               // it still may be at nice location
        {
            if (theComponents[tag] != 0)
                if ((theComponents[tag]->getTag()) == tag)
                {
                    return theComponents[tag];
                }
        }
    }

    // else we have to look through array until we find it or
    // reach lastPosition used
    for (int i = 0; i <= positionLastEntry; i++)
        if (theComponents[i] != 0)
            if (theComponents[i]->getTag() == tag)
            {
                return theComponents[i];
            }

    // its not in the array
    return 0;
}


TaggedObjectIter&
ArrayOfTaggedObjects::getComponents()
{
    // reset the iter to point to first component and then return
    // a reference to the iter
    myIter.reset();
    return myIter;
}


ArrayOfTaggedObjectsIter
ArrayOfTaggedObjects::getIter()
{
    // return a new iter to the components, more expensive to use
    // this iter as two memory calls to heap .. needed if user needs
    // to have multiple iters running in same code segment!
    return ArrayOfTaggedObjectsIter(*this);
}


TaggedObjectStorage*
ArrayOfTaggedObjects::getEmptyCopy(void)
{
    ArrayOfTaggedObjects* theCopy = new ArrayOfTaggedObjects(sizeComponentArray);

    if (theCopy == 0)
    {
        cerr << "ArrayOfTaggedObjects::getEmptyCopy - out of memory\n";
    }

    return theCopy;
}

void
ArrayOfTaggedObjects::clearAll(bool invokeDestructors)
{
    if (invokeDestructors == true)
    {
        // go through and invoke the components object destructors
        // and set the array pointers to 0
        for (int i = 0; i <= positionLastEntry; i++)
        {
            if (theComponents[i] != 0)
            {
                delete theComponents[i];
                theComponents[i] = 0;
            }
        }
    }
    else
    {
        // just set the array pointers to 0
        for (int i = 0; i <= positionLastEntry; i++)
        {
            if (theComponents[i] != 0)
            {
                theComponents[i] = 0;
            }
        }
    }

    positionLastEntry = 0;
    positionLastNoFitEntry = 0;
    fitFlag = true;
    numComponents = 0;
}




// void Print(ostream &s, int flag) const
//  method which invokes Print on all components

void
ArrayOfTaggedObjects::Print(ostream& s, int flag)
{
    // go through the array invoking Print on non-zero entries
    for (int i = 0; i <= positionLastEntry; i++)
        if (theComponents[i] != 0)
        {
            theComponents[i]->Print(s, flag);
        }
}



