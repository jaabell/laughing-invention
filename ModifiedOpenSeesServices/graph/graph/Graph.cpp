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
// $Source: /usr/local/cvs/OpenSees/SRC/graph/graph/Graph.cpp,v $


// Written: fmk
// Created: 11/96
// Revision: A
//
// Description: This file contains the class implementation for Graph.
//
// What: "@(#) Graph.C, revA"

#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <ArrayOfTaggedObjects.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

#include <time.h>
#include <NanoTimer.h>

#include <Vector.h>
#include <ID.h>

using namespace std;

Graph::Graph()
    : myVertices(0), theVertexIter(0), numEdge(0), nextFreeTag(START_VERTEX_NUM)
{
#ifdef _BABAK_DEBUG
    int numProcesses, processID;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    cerr << "BABAK @ Graph::Graph() --  PID:" << processID << "---Graph::Graph() called ...:" << endl;
#endif
    myVertices = new ArrayOfTaggedObjects(32);
    theVertexIter = new VertexIter(myVertices);
}


Graph::Graph(int numVertices)
    : myVertices(0), theVertexIter(0), numEdge(0), nextFreeTag(START_VERTEX_NUM)
{
#ifdef _BABAK_DEBUG
    int numProcesses, processID;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    cerr << "BABAK @ Graph(int numVertices) --  PID:" << processID << "---Graph::Graph() called ...:" << endl;
#endif
    myVertices = new ArrayOfTaggedObjects(numVertices);
    theVertexIter = new VertexIter(myVertices);
}


Graph::Graph(TaggedObjectStorage &theVerticesStorage)
    : myVertices(&theVerticesStorage), theVertexIter(0), numEdge(0), nextFreeTag(START_VERTEX_NUM)
{
#ifdef _BABAK_DEBUG
    int numProcesses, processID;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    cerr << "BABAK @ raph(TaggedObjectStorage &theVerticesStorage) --  PID:" << processID << "---Graph::Graph() called ...:" << endl;
#endif
    TaggedObject *theObject;
    TaggedObjectIter &theObjects = theVerticesStorage.getComponents();

    while ((theObject = theObjects()) != 0)
        if (theObject->getTag() > nextFreeTag)
        {
            nextFreeTag = theObject->getTag() + 1;
        }

    theVerticesStorage.clearAll();
    theVertexIter = new VertexIter(myVertices);
}


Graph::Graph(Graph &other)
    : myVertices(0), theVertexIter(0), numEdge(0), nextFreeTag(START_VERTEX_NUM)
{
#ifdef _BABAK_DEBUG
    int numProcesses, processID;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    cerr << "BABAK @ Graph(Graph &other) --  PID:" << processID << "---Graph::Graph() called ...:" << endl;
#endif
    myVertices = new ArrayOfTaggedObjects(other.getNumVertex());
    theVertexIter = new VertexIter(myVertices);

    VertexIter &otherVertices = other.getVertices();
    Vertex *vertexPtr;

    // loop through other creating vertices if tag not the same in this
    while ((vertexPtr = otherVertices()) != 0)
    {
        int vertexTag = vertexPtr->getTag();
        int vertexRef = vertexPtr->getRef();
        vertexPtr = new Vertex(vertexTag, vertexRef);

        if (vertexPtr == 0)
        {
            cerr << "Graph::Graph - out of memory\n";
            return;
        }

        this->addVertex(vertexPtr, false);
    }

    // loop through other adding all the edges that exist in other
    VertexIter &otherVertices2 = other.getVertices();

    while ((vertexPtr = otherVertices2()) != 0)
    {
        int vertexTag = vertexPtr->getTag();
        const ID &adjacency = vertexPtr->getAdjacency();

        for (int i = 0; i < adjacency.Size(); i++)
        {
            if (this->addEdge(vertexTag, adjacency(i)) < 0)
            {
                cerr << "Graph::merge - could not add an edge!\n";
                return;
            }
        }
    }
}

Graph::~Graph()
{
    // invoke delete on the Vertices
    //Out by Babak ... It makes some memory problems ... on September 2012
    // if (myVertices != 0)
    //     myVertices->clearAll();

    if (myVertices != 0)
    {
        delete myVertices;
    }

    if (theVertexIter != 0)
    {
        delete theVertexIter;
    }
}


// bool addVertex(int vertexTag, int vertexRef,
//       int vertexWeight=0, int vertexColor = 0)
// Method to add a vertex to the graph. If the adjacency list
// of the vertex is not empty the graph will first check to see all
// vertices in the the the vertices adjacency list exist in the graph
// before the vertex is added.

bool
Graph::addVertex(Vertex *vertexPtr, bool checkAdjacency)
{
    // check the vertex * and its adjacency list
    if (vertexPtr == 0)
    {
        cerr << "WARNING Graph::addVertex";
        cerr << " - attempting to add a NULL vertex*\n";
        return false;
    }

    if (checkAdjacency == true)
    {
        if (vertexPtr->getDegree() != 0)
        {
            const ID &adjacency = vertexPtr->getAdjacency();
            int size = adjacency.Size();

            for (int i = 0; i < size; i++)
            {
                Vertex *other = this->getVertexPtr(adjacency(i));

                if (other == 0)
                {
                    cerr << "WARNING Graph::addVertex";
                    cerr << " - vertex with adjacent vertex not in graph\n";
                    return false;
                }
            }
        }
    }


    bool result = myVertices->addComponent(vertexPtr); //Out by Babak 6/3/13

    //        bool result = myVertices->addComponent_graph(vertexPtr); //Replaced by Babak 6/3/13
    if (result == false)
    {
        //cerr << *this;
        //cerr << "BAD VERTEX\n: " << *vertexPtr;
        cerr << "WARNING Graph::addVertex";
        cerr << " - vertex could not be stored in TaggedObjectStorage object\n";
        return false;
    }


    // check nextFreeTag
    if (vertexPtr->getTag() >= nextFreeTag)
    {
        nextFreeTag = vertexPtr->getTag() + 1;
    }

    return result;
}


// int addEdge(int vertexTag, int otherVertexTag);
// Causes the Graph to add an edge {\em (vertexTag,otherVertexTag)} to
// the Graph. A check is first made to see if vertices with tags given by
// {\em vertexTag} and {\em otherVertexTag} exist in the graph. If they
// do not exist a $-1$ is returned, otherwise the method invokes {\em
// addEdge()} on each of the corresponding vertices in the
// graph. Returns $0$ if sucessfull, a negative number if not.

int
Graph::addEdge(int vertexTag, int otherVertexTag)
{
    // get pointers to the vertices, if one does not exist return

    Vertex *vertex1 = this->getVertexPtr(vertexTag);
    Vertex *vertex2 = this->getVertexPtr(otherVertexTag);

    if ((vertex1 == 0) || (vertex2 == 0))
    {
        cerr << "WARNING Graph::addEdge() - one or both of the vertices ";
        cerr << vertexTag << " " << otherVertexTag << " not in Graph\n";
        return -1;
    }

    // add an edge to each vertex
    int result;

    if ((result = vertex1->addEdge(otherVertexTag)) == 0)
    {
        if ((result = vertex2->addEdge(vertexTag)) == 0)
        {
            numEdge++;
        }
        else
        {
            cerr << "WARNING Graph::addEdge() - " << vertexTag;
            cerr << " has not been added to " << otherVertexTag;
            cerr << " adjacency - yet vica-versa ok.\n";
            return -2;
        }
    }

    return result;
}


Vertex *
Graph::getVertexPtr(int vertexTag)
{
    TaggedObject *res = myVertices->getComponentPtr(vertexTag);

    if (res == 0)
    {
        return 0;
    }

    Vertex *result = (Vertex *)res;
    return result;
}


VertexIter &
Graph::getVertices(void)
{
    // reset the iter and then return it
    theVertexIter->reset();
    return *theVertexIter;
}


int
Graph::getNumVertex(void) const
{
    return myVertices->getNumComponents();
}

int
Graph::getNumEdge(void) const
{
    return numEdge;
}

int
Graph::getFreeTag(void)
{
    return nextFreeTag;
}

Vertex *
Graph::removeVertex(int tag, bool flag)
{
    TaggedObject *mc = myVertices->removeComponent(tag);

    if (mc == 0)
    {
        return 0;
    }

    Vertex *result = (Vertex *)mc;

    if (flag == true)   // remove all edges associated with the vertex
    {
        cerr << "Graph::removeVertex(int tag, bool flag = true)";
        cerr << " - no code to remove edges yet\n";
        return 0;
    }

    return result;
}


int
Graph::merge(Graph &other)
{

    int result = 0;
    VertexIter &otherVertices = other.getVertices();
    Vertex *vertexPtrOther;

    // loop through other creating vertices if tag not the same in this
    while ((vertexPtrOther = otherVertices()) != 0)
    {
        int vertexTag = vertexPtrOther->getTag();
        Vertex *vertexPtr = this->getVertexPtr(vertexTag);

        if (vertexPtr == 0)
        {
            int vertexRef = vertexPtrOther->getRef();
            vertexPtr = new Vertex(vertexTag, vertexRef);

            if (vertexPtr == 0)
            {
                cerr << "Graph::merge - out of memory\n";
                return -1;
            }

            this->addVertex(vertexPtr, false);
        }
    }


    // loop through other adding all the edges that exist in other
    VertexIter &otherVertices2 = other.getVertices();

    while ((vertexPtrOther = otherVertices2()) != 0)
    {
        int vertexTag = vertexPtrOther->getTag();
        const ID &adjacency = vertexPtrOther->getAdjacency();

        for (int i = 0; i < adjacency.Size(); i++)
        {
            if (this->addEdge(vertexTag, adjacency(i)) < 0)
            {
                cerr << "Graph::merge - could not add an edge!\n";
                return -2;
            }
        }
    }

    return result;
}


void
Graph::Print(ostream &s, int flag)
{
    myVertices->Print(s, flag);
}


ostream &operator<<(ostream &s, Graph &M)
{
    M.Print(s);
    return s;
}


int
Graph::sendSelf(int commitTag, Channel &theChannel)
{
    // check not a datastore ..
    if (theChannel.isDatastore() != 0)
    {
        cerr << "Graph::sendSelf() - does not at present send to a database\n";
        return -1;
    }

    // send numEdge & the number of vertices

    int numVertex = this->getNumVertex();

    static ID idData(2);
    idData(0) = numEdge;
    idData(1) = numVertex;

    if (theChannel.sendID(0, commitTag, idData) < 0)
    {
        cerr << "Graph::sendSelf() - failed to send the id\n";
        return -3;
    }

    ID tags(numVertex + 1);  //Last one will be the size of the adjacency vector
    ID refs(numVertex);
    Vector weights(numVertex);
    ID colors(numVertex);
    ID tmps(numVertex);
    // ID myTag(numEdge);
    // ID myRef(numEdge);
    // ID myWeight(numEdge);
    // ID myColor(numEdge);
    // ID myDegree(numEdge);
    // ID myTmp(numEdge);
    // ID vsize(numEdge);

    // Send tags
    VertexIter &theVertices = this->getVertices();
    Vertex *vertexPtr;
    int i = 0;
    int adjsize = 0;
    while ((vertexPtr = theVertices()) != 0)
    {
        tags[i] = vertexPtr->getTag();
        refs[i] = vertexPtr->getRef();
        weights[i] = vertexPtr->getWeight();
        colors[i] = vertexPtr->getColor();
        tmps[i] = vertexPtr->getTmp();
        i++;
        adjsize += 1;
        adjsize += vertexPtr->getDegree();
    }
    tags[i] = adjsize;
    cout << "sending tags to " << theChannel.getTag() << endl;
    theChannel.sendID(0, commitTag, tags);
    cout << "sending refs to " << theChannel.getTag() << endl;
    theChannel.sendID(0, commitTag, refs);
    cout << "sending weights to " << theChannel.getTag() << endl;
    theChannel.sendVector(0, commitTag, weights);
    cout << "sending colors to " << theChannel.getTag() << endl;
    theChannel.sendID(0, commitTag, colors);
    cout << "sending tmps to " << theChannel.getTag() << endl;
    theChannel.sendID(0, commitTag, tmps);

    ID adjacency_vector(adjsize);
    theVertices.reset();
    int pos = 0;
    while ((vertexPtr = theVertices()) != 0)
    {
        ID myadj = vertexPtr->getAdjacency();
        adjacency_vector[pos++] = myadj.Size();
        for (int i = 0; i < myadj.Size(); i++)
        {
            adjacency_vector[pos++] = myadj[i];
        }
    }

    theChannel.sendID(0, commitTag, adjacency_vector);

    // send each vertex
    // VertexIter &theVertices = this->getVertices();
    // Vertex *vertexPtr;
    // while ((vertexPtr = theVertices()) != 0)
    // {
    //     if (vertexPtr->sendSelf(commitTag, theChannel) < 0)
    //     {
    //         cerr << "Graph::sendSelf() - failed to send a vertex ";
    //         //       cerr << "Graph::sendSelf() - failed to send a vertex: " << *vertexPtr;
    //         return -3;
    //     }
    // }

    return 0;
}


int
Graph::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // check not from a datastore
    if (theChannel.isDatastore() != 0)
    {
        cerr << "Graph::receiveSelf() - at present does not receive from a database\n";
        return -1;
    }

    // check blank
    if (this->getNumVertex() != 0)
    {
        cerr << "Graph::receiveSelf() - can only receive to an empty graph at present\n";

        numEdge = 0;
        myVertices->clearAll();
    }

    // recv numEdge & numVertices
    static ID idData(2);

    if (theChannel.receiveID(0, commitTag, idData) < 0)
    {
        cerr << "Graph::receiveSelf() - failed to receive the id\n";
        return -3;
    }

    numEdge = idData(0);
    int numVertex = idData(1);

    cout << "Preparing for numVertex = " << numVertex << endl;

    ID tags(numVertex + 1); //Last is the size of the adjacency vector
    ID refs(numVertex);
    Vector weights(numVertex);
    ID colors(numVertex);
    ID tmps(numVertex);


    cout << "receiving tags from " << theChannel.getTag() << endl;
    theChannel.receiveID(0, commitTag, tags);

    cout << "receiving refs from " << theChannel.getTag() << endl;
    theChannel.receiveID(0, commitTag, refs);

    cout << "receiving weights from " << theChannel.getTag() << endl;
    theChannel.receiveVector(0, commitTag, weights);

    cout << "receiving colors from " << theChannel.getTag() << endl;
    theChannel.receiveID(0, commitTag, colors);

    cout << "receiving tmps from " << theChannel.getTag() << endl;
    theChannel.receiveID(0, commitTag, tmps);

    // for each vertex to be received, create it, receive it and then add it to the graph
    for (int i = 0; i < numVertex; i++)
    {
        int tag = tags[i];
        int ref = refs[i];
        int weight = weights[i];
        int color = colors[i];
        int tmp = tmps[i];
        Vertex *theVertex = new Vertex( tag,  ref,  weight ,  color );
        theVertex->setTmp(tmp);

        if (theVertex == 0)
        {
            cerr << "Graph::receiveSelf() - out of memory\n";
            return -4;
        }

        // if (theVertex->receiveSelf(commitTag, theChannel, theBroker) < 0)
        // {
        //     cerr << "Graph::receiveSelf() - vertex failed to receive itself\n";
        //     return -5;
        // }

        this->addVertex(theVertex, false);
    }

    int adjsize = tags[numVertex];

    ID adjacency_vector(adjsize);

    theChannel.receiveID(0, commitTag, adjacency_vector);

    VertexIter &theVertices = this->getVertices();
    Vertex *vertexPtr;
    int pos = 0;
    while ((vertexPtr = theVertices()) != 0)
    {
        int size = adjacency_vector[pos++];
        for (int i = 0; i < size; i++)
        {
            vertexPtr->addEdge(adjacency_vector[pos++]);
        }
    }


    // // for each vertex to be received, create it, receive it and then add it to the graph
    // for (int i = 0; i < numVertex; i++)
    // {
    //     Vertex *theVertex = new Vertex(0, 0);

    //     if (theVertex == 0)
    //     {
    //         cerr << "Graph::receiveSelf() - out of memory\n";
    //         return -4;
    //     }

    //     if (theVertex->receiveSelf(commitTag, theChannel, theBroker) < 0)
    //     {
    //         cerr << "Graph::receiveSelf() - vertex failed to receive itself\n";
    //         return -5;
    //     }

    //     this->addVertex(theVertex, false);
    // }

    return 0;
}
