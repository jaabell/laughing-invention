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
// $Source: /usr/local/cvs/OpenSees/SRC/graph/graph/Vertex.h,v $


#ifndef Vertex_h
#define Vertex_h

// Written: fmk
// Revision: A
//
// Description: This file contains the class definition for Vertex.
// Vertex is an element of a graph.
//
// What: "@(#) Vertex.h, revA"

#ifndef _bool_h
#include "bool.h"
#endif

#include <TaggedObject.h>
#include <ID.h>

#define START_VERTEX_NUM 0
#define VERTEX_INITIAL_ADJACENCY_SIZE 40
class Channel;
class FEM_ObjectBroker;

class Vertex: public TaggedObject
{
public:
    Vertex(void);
    Vertex(int tag, int ref, double weight = 0, int color = 0, int adj_size = 10);
    Vertex(const Vertex &other);
    // Vertex(Vertex const &other);

    virtual ~Vertex();

    virtual void setWeight(double newWeight);
    virtual void setColor(int newColor);
    virtual void setTmp(int newTmp);

    virtual int getRef(void) const;
    virtual double getWeight(void) const;
    virtual int getColor(void) const;
    virtual int getTmp(void) const;

    virtual int addEdge(int otherTag, bool allowSelf = false);
    virtual int getDegree(void) const;
    virtual const ID &getAdjacency(void) const;

    virtual  void Print(ostream &s, int flag = 0);
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);


    void setVertexSize(int size);// {vsize = size;};
    int getVertexSize();// {return vsize;};

protected:

private:
    int myRef;
    double myWeight;
    int myColor;
    int myDegree;
    int myTmp;
    ID  myAdjacency;
    int vsize;

};

#endif

