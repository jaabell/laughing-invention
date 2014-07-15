///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              ElementSelfWeight
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Jose Abell
// PROGRAMMER:        Jose Abell
// DATE:              February 2013
// UPDATE HISTORY:    Novermber 2013, Nima Tafazzoli moving acceleration field
//                    from brick elements to here
//
//
///////////////////////////////////////////////////////////////////////////////


#ifndef ElementSelfWeight_CPP
#define ElementSelfWeight_CPP


#include <ElementSelfWeight.h>
#include <Vector.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


// Vector ElementSelfWeight::data(3);

ElementSelfWeight::ElementSelfWeight(int tag, const ID &theElementTags, const Vector &accelerationfield)
    : ElementalLoad(tag, LOAD_TAG_ElementSelfWeight, theElementTags), data(3)
{
    data(0) = accelerationfield(0);
    data(1) = accelerationfield(1);
    data(2) = accelerationfield(2);
}

ElementSelfWeight::ElementSelfWeight()
    : ElementalLoad(LOAD_TAG_ElementSelfWeight), data(3)
{

}

ElementSelfWeight::~ElementSelfWeight()
{

}

const Vector &
ElementSelfWeight::getData(int &type, double loadFactor)
{
    type = LOAD_TAG_ElementSelfWeight;

    return data;
}

int
ElementSelfWeight::sendSelf(int commitTag, Channel &theChannel)
{
    int dbTag = this->getDbTag();
    int myTag = this->getTag();
    const ID &theElements = this->getElementTags();

    static ID idData(2);
    idData(0) = theElements.Size();
    idData(1) = myTag;

    int result = theChannel.sendID(dbTag, commitTag, idData);

    if (result < 0)
    {
        cerr << "ElementSelfWeight::sendSelf() - failed to send data\n";
        return result;
    }

    result = theChannel.sendID(dbTag, commitTag, theElements);

    if (result < 0)
    {
        cerr << "ElementSelfWeight::sendSelf() - failed to send element tags\n";
        return result;
    }

    result = theChannel.sendVector(dbTag, commitTag, data);
    if (result < 0)
    {
        cerr << "ElementSelfWeight::sendSelf() - failed to data (value of self weight acceleration field.)\n";
        return result;
    }


    return 0;
}

int
ElementSelfWeight::recvSelf(int commitTag, Channel &theChannel,  FEM_ObjectBroker &theBroker)
{
    int dbTag = this->getDbTag();
    static ID idData(2);

    int result = theChannel.recvID(dbTag, commitTag, idData);

    if (result < 0)
    {
        cerr << "ElementSelfWeight::recvSelf() - failed to recv data\n";
        return result;
    }

    int numEle = idData(0);
    int myTag = idData(1);
    ID *NewTags = new ID(numEle);

    result = theChannel.recvID(dbTag, commitTag, *NewTags);

    if (result < 0)
    {
        cerr << "ElementSelfWeight::recvSelf() - failed to recv element tags\n";
        return result;
    }

    // Vector *newdata;
    result = theChannel.recvVector(dbTag, commitTag, data);

    if (result < 0)
    {
        cerr << "ElementSelfWeight::recvSelf() - failed to recv data (value of self weight acceleration field.)\n";
        return result;
    }

    // data = *newdata;


    this->setTag(myTag);
    this->setElementTags(*NewTags);
    delete NewTags;

    return 0;
}

void
ElementSelfWeight::Print(ostream &s, int flag)
{
    s << "ElementSelfWeight...";
    s << "  elements acted on: " << this->getElementTags();
}

#endif

