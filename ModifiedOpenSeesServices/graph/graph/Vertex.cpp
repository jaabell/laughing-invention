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
// $Date: 2003/02/14 23:01:23 $
// $Source: /usr/local/cvs/OpenSees/SRC/graph/graph/Vertex.cpp,v $

// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the class implementation for Vertex.
// Vertex is an element of a graph.
//
// What: "@(#) Vertex.C, revA"

#include <Vertex.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <Vector.h>
#include <iostream>
using namespace std;


Vertex::Vertex()
    : TaggedObject(-1), myRef(-1), myWeight(-1), myColor(-1),
      myDegree(0), myTmp(0), myAdjacency(0, 10)
{

}

Vertex::Vertex(int tag, int ref, double weight, int color, int adj_size)
    : TaggedObject(tag), myRef(ref), myWeight(weight), myColor(color),
      myDegree(0), myTmp(0), myAdjacency(0, adj_size)
      //GZ myAdjacency(0, 8)
{

}

Vertex::Vertex(const Vertex &other)
    : TaggedObject(other.getTag()), myRef(other.getRef()), myWeight(other.getWeight()), myColor(other.getColor()),
      myDegree(other.getDegree()), myTmp(other.getTmp()), myAdjacency(other.getAdjacency())
{

}

Vertex::~Vertex()
{

}

void
Vertex::setWeight(double newWeight)
{
    myWeight = newWeight;
}

void
Vertex::setColor(int newColor)
{
    myColor = newColor;
}

void
Vertex::setTmp(int newTmp)
{
    myTmp = newTmp;
}

int
Vertex::getRef(void) const
{
    return myRef;
}

double
Vertex::getWeight(void) const
{
    return myWeight;
}

int
Vertex::getColor(void) const
{
    return myColor;
}

int
Vertex::getTmp(void) const
{
    return myTmp;
}

int
Vertex::addEdge(int otherTag, bool allowself)
{
    // don't allow itself to be added
    if (otherTag == this->getTag() && not allowself)
    {
        return 0;
    }

    // check the otherVertex has not already been added

    if (myAdjacency.getLocation(otherTag) < 0)
    {
        myAdjacency[myDegree]  = otherTag;
        myDegree++;
        return 0;
    }
    else
    {
        return 1;
    }
}


int
Vertex::getDegree(void) const
{
    return myDegree;
}

const ID &
Vertex::getAdjacency(void) const
{
    return myAdjacency;
}

void
Vertex::Print(ostream &s, int flag)
{
    s << this->getTag() << " " ;
    s << myRef << " ";

    if (flag == 1)
    {
        s << myWeight << " " ;
    }
    else if (flag == 2)
    {
        s << myColor << " " ;
    }
    else if (flag == 3)
    {
        s << myWeight << " " << myColor << " " ;
    }
    else if (flag == 4)
    {
        s << myWeight << " " << myColor << " " << myTmp << " " << vsize << " " << myDegree << " " ;
    }

    s << "ADJACENCY: " << myAdjacency;
}


int
Vertex::sendSelf(int commitTag, Channel &theChannel)
{
    // send the tag/ref/color/degree/tmp, an indication if weighted & size of adjacency
    static ID idData(7);
    idData(0) = this->getTag();
    idData(1) = myRef;
    idData(2) = myColor;
    idData(3) = myDegree;
    idData(4) = myTmp;

    if (myWeight == 0.0)
    {
        idData(5) = 0;
    }
    else
    {
        idData(5) = 1;
    }

    idData(6) = myAdjacency.Size();

    if (theChannel.sendID(0, commitTag, idData) < 0)
    {
        cerr << "Graph::sendSelf() - failed to receive the initial data\n";
        return -1;
    }

    // if weighted, send the weight
    if (myWeight != 0.0)
    {
        static Vector vectData(1);
        vectData(0) = myWeight;

        if (theChannel.sendVector(0, commitTag, vectData) < 0)
        {
            cerr << "Graph::rendSelf() - failed to receive the weight\n";
            return -2;
        }
    }

    // finally send the adjacency
    if (theChannel.sendID(0, commitTag, myAdjacency) < 0)
    {
        cerr << "Graph::sendSelf() - failed to receive the adjacency data\n";
        return -1;
    }

    return 0;
}


int
Vertex::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // recv the tag/ref/color/degree/tmp, an indication if weighted & size of adjacency
    static ID idData(7);

    if (theChannel.receiveID(0, commitTag, idData) < 0)
    {
        cerr << "Graph::receiveSelf() - failed to receive the initial data\n";
        return -1;
    }

    this->setTag(idData(0));
    myRef = idData(1);
    myColor = idData(2);
    myDegree = idData(3);
    myTmp = idData(4);

    // if weighted, receive the weight
    if (idData(5) == 1)
    {
        static Vector vectData(1);

        if (theChannel.receiveVector(0, commitTag, vectData) < 0)
        {
            cerr << "Graph::receiveSelf() - failed to receive the weight\n";
            return -2;
        }

        myWeight = vectData(0);
    }

    // resize the adjacency & receive it
    //  myAdjacency[idData(6)-1] = 0;
    int *adjacencyData;
    adjacencyData = new int[idData[6]];
    myAdjacency.setData(adjacencyData, idData[6], true);

    if (theChannel.receiveID(0, commitTag, myAdjacency) < 0)
    {
        cerr << "Graph::receiveSelf() - failed to receive the initial data\n";
        return -3;
    }

    return 0;
}

// # ifdef _PDD
void
Vertex::setVertexSize(int size)
{
    vsize = size;
}

int Vertex::getVertexSize()
{
    return vsize;
}
// # endif
