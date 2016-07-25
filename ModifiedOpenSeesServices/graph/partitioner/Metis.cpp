///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         .
// DATE:               .
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////



// Original file, from OpenSees project, see ORIGINAL header below,
// was significantly changed, improved, massacred and massaged, so that
// this implementation/file now constitutes quite a novel development.
// In addition, ORIGINAL OpenSees license (see below) assigned
// (wrongly) copyright to the
// Regents of the University of California.
// ORIGINAL license also gave a number of permissions (see below)
// that we used.
//
//--------------------------------------------------------------------------
// Original OpenSees License:
// Copyright @ 1999,2000  The Regents of the University of California.
// All Rights Reserved.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for educational, research and non-profit purposes, without
// fee, and without a written agreement is hereby granted, provided that the
// above copyright notice, this paragraph and the following three paragraphs
// appear in all copies.
//
// Permission to incorporate this software into commercial products may be
// obtained by contacting the University of California.
// [Office of Technology Licensing,
// 2150 Shattuck Avenue #150
// Berkeley, CA 94720-1620,
// (510) 643-7201]
//
// This software program and documentation are copyrighted by The Regents of
// the University of California. The software program and documentation are
// supplied "as is", without any accompanying services from The Regents. The
// Regents does not warrant that the operation of the program will be
// uninterrupted or error-free. The end-user understands that the program was
// developed for research purposes and is advised not to rely exclusively on
// the program for any reason.
//
// IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
// DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
// LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
// DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE. THE UNIVERSITY OF CALIFORNIA SPECIFICALLY
// DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
// SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
// CALIFORNIA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
//--------------------------------------------------------------------------
//
//
// Original header for this file:




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
// $Date: 2003/02/14 23:01:24 $
// $Source: /usr/local/cvs/OpenSees/SRC/graph/partitioner/Metis.cpp,v $


// Written: fmk
// Created: Sun Sept 15 11:47:47: 1996
// Revision: A
//
// Description: This file contains the class definition for Metis.
// Metis is a type of GraphPartitioner which uses 'METIS - Unstructured
// Graph Partitioning And Sparse Matrix Ordering System', developed by
// G. Karypis and V. Kumar at the University of Minnesota. The metis
// files are found in metis-2.0 which were downloaded.
//     This class provides the C++ interface for metis which will allow
// it to fit seamlessly into our system.
//
// What: "@(#) Metis.C, revA"

#include <Metis.h>
#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <iostream>
using namespace std;


//# include <parmetis.h> //Guanzhou

/* stuff needed to get the program working on the clump & NOW machines*/
#include <bool.h>
#ifdef _LINUX
extern "C" {
    //#include <GKlib.h>
}


//int IsWeighted;

//timer TotalTmr;         /* Times the entire algorithm */
//timer CoarsenTmr;       /* Times total coarsening time */
//timer GreedyTmr;        /* Times Total Greedy Time */
//timer GreedyInitTmr;    /* Times initialization cost of Greedy */
//timer GreedyIterTmr;    /* Times Iterative cost of Greedy */
//timer GreedyWrapUpTmr;  /* Times the wrap up phase of Greedy */
//timer MlevelTmr;        /* Times the entire multilevel algorithm */
//timer InitPartTmr;      /* Times the initial partition phase */
//timer ProjectTmr;       /* Times the projection of the partition */
//timer SplitTmr;         /* Times the boundary creation */
//timer BalanceTmr;       /* Times the time required for balancing */
//timer IOTmr;            /* Times the file input time */
//timer UncrsTmr;         /* Times the file input time */

#endif

#define PMETIS METIS_PartGraphRecursive
#define KMETIS METIS_PartGraphKway

extern "C"
//int
void PMETIS(int *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
//void METIS_PartGraphRecursive(int *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);

extern "C"
//int
void KMETIS(int *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
//void METIS_PartGraphKway(int *, idxtype *, idxtype *, idxtype *, idxtype *, int *, int *, int *, int *, int *, idxtype *);

//typedef int idxtype;
/*

extern "C"
void METIS_PartGraphRecursive(int *n, idxtype *xadj, idxtype *adjacency,
                    idxtype *vertWgt, idxtype *adjWgt,
                    int *wgtFlag, int *numFlag, int *numParts,
                    int *options, int *edgCut, idxtype *part);
extern "C"
void METIS_PartGraphKway(int *n, idxtype *xadj, idxtype *adjacency,
                   idxtype *vertWgt, idxtype *adjWgt,
                   int *wgtFlag, int *numFlag, int *numParts,
                   int *options, int *edgCut, idxtype *part);*/

Metis::Metis(int numParts)
    : GraphNumberer(GraphNUMBERER_TAG_Metis),
      myPtype(0), myMtype(0), myCoarsenTo(0), myRtype(0), myIPtype(0),
      defaultOptions(true), numPartitions(numParts), theRefResult(0)
{

}

Metis::Metis(int Ptype,
             int Mtype,
             int coarsenTo,
             int Rtype,
             int IPtype,
             int numParts)
    : GraphNumberer(GraphNUMBERER_TAG_Metis),
      myPtype(Ptype), myMtype(Mtype), myCoarsenTo(coarsenTo), myRtype(Rtype),
      myIPtype(IPtype), defaultOptions(false), numPartitions(numParts), theRefResult(0)
{
    // check the options are valid
    checkOptions();
}


Metis::~Metis()
{

}

bool
Metis::setOptions(int Ptype,
                  int Mtype,
                  int coarsenTo,
                  int Rtype,
                  int IPtype)
{
    myPtype = Ptype;
    myMtype = Mtype;
    myCoarsenTo = coarsenTo;
    myRtype = Rtype;
    myIPtype = IPtype;

    defaultOptions = false;

    return checkOptions();

}


// bool checkOptions(void) const
//  returns true if options are o.k., false otherwise

bool
Metis::checkOptions(void)
{

    // check default not set

    if (defaultOptions == true)
    {
        return true;
    }


    // otherwise check all options for valid value
    bool okFlag = true;

    if ((myPtype != 1) || (myPtype != 2))
    {
        okFlag = false;
        cerr << "WARNING: Metis::partition ";
        cerr << " - Illegal Ptype " << myPtype << endln;
    }

    if ((myMtype != 1) ||  (myMtype != 2) || (myMtype != 3) ||
            ((myMtype != 4) || (myMtype != 5) || myMtype != 11) ||
            (myMtype != 21) || (myMtype != 51))
    {
        okFlag = false;
        cerr << "WARNING: Metis::partition ";
        cerr << " - Illegal Mtype " << myMtype << endln;
    }

    if (myPtype == 1)
        if ((myRtype != 1) || (myRtype != 2) || (myRtype != 3) ||
                (myRtype != 11) || (myRtype != 12) || (myRtype != 13) ||
                (myRtype != 20))
        {
            okFlag = false;
            cerr << "WARNING: Metis::partition ";
            cerr << " - Illegal Rtype " << myRtype << endln;
            cerr << " for Ptype " << myPtype << endln;
        }
        else if (myPtype == 2)
            if ((myRtype != 11) || (myRtype != 12) || (myRtype != 20))
            {
                okFlag = false;
                cerr << "WARNING: Metis::partition ";
                cerr << " - Illegal Rtype " << myRtype << endln;
                cerr << " for Ptype " << myPtype << endln;
            }

    if ((myIPtype != 1) || (myIPtype != 2) || (myIPtype != 3) ||
            (myIPtype != 4))
    {
        okFlag = false;
        cerr << "WARNING: Metis::partition ";
        cerr << " - Illegal IPtype " << myIPtype << endln;
    }

    if (myCoarsenTo < 0)
    {
        okFlag = false;
        cerr << "WARNING: Metis::partition ";
        cerr << " - Illegal coarsen To " << myCoarsenTo << endln;
    }

    if (okFlag == false)
    {
        defaultOptions = true;
    }

    return okFlag;

}


bool
Metis::setDefaultOptions(void)
{
    defaultOptions = true;
    return true;
}


// int partition(Graph &theGraph, int numPart)
//  Method to partition the graph. It first creates the arrays needed
//  by the metis lib and then invokes a function from the metis lib to
//  partition the graph. The solors of the vertices of the graph are
//  set to colors 0 through numPart-1 to indicate which partition the
//  vrtices are in. Returns -1 if options are not set, -2 if metis failed.

int
// Metis::partition(Graph &theGraph, int numPart)
Metis::partition(Graph *theGraph, int numPart)
{

    cerr << "Metis::partition() -1\n";

    if (theGraph->getNumVertex() == numPart)
    {
        cerr << "METIS\n" << theGraph;
        Vertex *vertex;
        int current = 1;
        VertexIter &theVertices = theGraph->getVertices();

        while ((vertex = theVertices()) != 0)
        {
            vertex->setColor(current++);
        }

        return 0;
    }

    cerr << "Metis::partition() -2\n";

    // first we check that the options are valid
    if (checkOptions() == false)
    {
        return -1;
    }

    // now we get room for the data structures metis needs

    int numVertex = theGraph->getNumVertex();
    int numEdge = theGraph->getNumEdge();
    cerr << "Metis::partition() -3\n";
    int *options = new int [5];
    int *partition = new int [numVertex + 1];
    int *xadj = new int [numVertex + 2];
    int *adjncy = new int [2 * numEdge];
    int *vwgts = 0;
    int *ewgts = 0;
    int numbering = 0;
    int weightflag = 0; // no weights on our graphs yet

    if (START_VERTEX_NUM == 0)
    {
        numbering = 0;
    }
    else if (START_VERTEX_NUM == 1)
    {
        numbering = 1;
    }
    else
    {
        cerr << "WARNING Metis::partition - No partitioning done";
        cerr << " vertex numbering must start at 0 or 1\n";
        return (-2);
    }

    int edgecut;

    if ((options == 0) || (partition == 0) || (xadj == 0) || (adjncy == 0))
    {
        cerr << "WARNING Metis::partition - No partitioning done";
        cerr << " as ran out of memory\n";
        return (-2);
    }


    // we build these data structures

    int indexEdge = 0;
    xadj[0] = 0;
    cerr << "Metis::partition() -4\n";
    Vertex *vertexPtr;

    for (int vertex = 0; vertex < numVertex; vertex++)
    {
        vertexPtr = theGraph->getVertexPtr(vertex + START_VERTEX_NUM);

        // check we don't have an invalid vertex numbering scheme
        // if so WARNING message, clean up and return -2

        if (vertexPtr == 0)
        {
            cerr << "WARNING Metis::partition - No partitioning done";
            cerr << " Metis requires consequtive Vertex Numbering\n";

            delete [] options;
            delete [] partition;
            delete [] xadj;
            delete [] adjncy;

            return -2;
        }

        const ID &adjacency = vertexPtr->getAdjacency();
        int degree = adjacency.Size();

        for (int i = 0; i < degree; i++)
        {
            adjncy[indexEdge++] = adjacency(i) - START_VERTEX_NUM;
        }

        xadj[vertex + 1] = indexEdge;
    }

    cerr << "Metis::partition() -5\n";

    if (defaultOptions == true)
    {
        options[0] = 0;
    }
    else
    {
        options[0] = 1;
        options[1] = myCoarsenTo;
        options[2] = myMtype;
        options[3] = myIPtype;
        options[4] = myRtype;
    }

    cerr << "Metis::partition() -6\n";
    // we now the metis routines

    if (myPtype == 1)
        //PMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag, &numPart,
        //       options, &numbering, &edgecut, partition);
        PMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag, &numbering,
               &numPart, options, &edgecut, partition);//Guanzhou changed to use new Metis lib
    else
        //KMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag, &numPart,
        //       options, &numbering, &edgecut, partition);
        KMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag, &numbering,
               &numPart, options, &edgecut, partition);//Guanzhou changed to use new Metis lib

    cerr << "Metis::partition() -7\n";

    // we set the vertex colors to correspond to the partitioned scheme
    for (int vert = 0; vert < numVertex; vert++)
    {
        vertexPtr = theGraph->getVertexPtr(vert + START_VERTEX_NUM);
        vertexPtr->setColor(partition[vert] + 1); // start colors at 1
    }

    // clean up the space and return
    cerr << "Metis::partition() -8\n";

    delete [] options;
    delete [] partition;
    delete [] xadj;
    delete [] adjncy;

    cerr << "Metis::partition() -9\n";
    return 0;
}






const ID &
Metis::number(Graph &theGraph, int lastVertex)
{
    // first we check that the options are valid
    // first check our size, if not same make new
    int numVertex = theGraph.getNumVertex();

    // delete the old
    if (theRefResult != 0)
    {
        delete theRefResult;
    }

    theRefResult = new ID(numVertex);

    if (theRefResult == 0)
    {
        cerr << "ERROR:  Metis::number - Out of Memory\n";
        theRefResult = new ID(0);
        return *theRefResult;
    }

    if (checkOptions() == false)
    {
        cerr << "ERROR:  Metis::number - chek options failed\n";
        return *theRefResult;
    }

    // now we get room for the data structures metis needs
    int numEdge = theGraph.getNumEdge();

    int *options = new int [5];
    int *partition = new int [numVertex + 1];
    int *xadj = new int [numVertex + 2];
    int *adjncy = new int [2 * numEdge];
    int *vwgts = 0;
    int *ewgts = 0;
    int numbering = 0;
    int weightflag = 0; // no weights on our graphs yet

    if (START_VERTEX_NUM == 0)
    {
        numbering = 0;
    }
    else if (START_VERTEX_NUM == 1)
    {
        numbering = 1;
    }
    else
    {
        cerr << "WARNING Metis::partition - No partitioning done";
        cerr << " vertex numbering must start at 0 or 1\n";
        return *theRefResult;
    }

    int edgecut;

    if ((options == 0) || (partition == 0) || (xadj == 0) || (adjncy == 0))
    {
        cerr << "WARNING Metis::partition - No partitioning done";
        cerr << " as ran out of memory\n";
        return *theRefResult;
    }


    // we build these data structures

    int indexEdge = 0;
    xadj[0] = 0;

    Vertex *vertexPtr;

    for (int vertex = 0; vertex < numVertex; vertex++)
    {
        vertexPtr = theGraph.getVertexPtr(vertex + START_VERTEX_NUM);

        // check we don't have an invalid vertex numbering scheme
        // if so WARNING message, clean up and return -2

        if (vertexPtr == 0)
        {
            cerr << "WARNING Metis::partition - No partitioning done";
            cerr << " Metis requires consequtive Vertex Numbering\n";

            delete [] options;
            delete [] partition;
            delete [] xadj;
            delete [] adjncy;

            return *theRefResult;
        }

        const ID &adjacency = vertexPtr->getAdjacency();
        int degree = adjacency.Size();

        for (int i = 0; i < degree; i++)
        {
            adjncy[indexEdge++] = adjacency(i) - START_VERTEX_NUM;
        }

        xadj[vertex + 1] = indexEdge;
    }


    if (defaultOptions == true)
    {
        options[0] = 0;
    }
    else
    {
        options[0] = 1;
        options[1] = myCoarsenTo;
        options[2] = myMtype;
        options[3] = myIPtype;
        options[4] = myRtype;
    }


    // we now the metis routines

    if (myPtype == 1)

        PMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag,
               &numPartitions, options, &numbering, &edgecut, partition);
    else
        KMETIS(&numVertex, xadj, adjncy, vwgts, ewgts, &weightflag,
               &numPartitions, options, &numbering, &edgecut, partition);

    cerr << "Metis::number -2\n";
    // we assign numbers now based on the partitions returned.
    // each vertex in partion i is assigned a number less than
    // thos in partion i+1: NOTE WE DON'T CARE WHAT THE NUMBERING IS
    // WITHIN A PARTITION
    int count = 0;

    for (int i = 0; i < numPartitions; i++)
    {
        for (int vert = 0; vert < numVertex; vert++)
        {
            if (partition[vert] == i)
            {
                vertexPtr = theGraph.getVertexPtr(vert + START_VERTEX_NUM);
                vertexPtr->setTmp(count + 1);
                (*theRefResult)(count) = vertexPtr->getRef();
                count++;
            }
        }
    }

    cerr << "Metis::number -3\n";
    // clean up the space and return
    delete [] options;
    delete [] partition;
    delete [] xadj;
    delete [] adjncy;
    cerr << "Metis::number -4\n";
    return *theRefResult;
}


const ID &
Metis::number(Graph &theGraph, const ID &lastVertices)
{
    return this->number(theGraph);
}

int
Metis::sendSelf(int cTag, Channel &theChannel)
{
    return 0;
}

int
Metis::receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

