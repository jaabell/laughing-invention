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

// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:19 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/partitioner/DomainPartitioner.h,v $


// Written: fmk
// Created: Fri Sep 20 15:27:47: 1996
// Revision: A
//
// Description: This file contains the class definition for DomainPartitioner.
// A DomainPartitioner is an object used to partition a PartitionedDomain.
//
// What: "@(#) DomainPartitioner.h, revA"

#ifndef DomainPartitioner_h
#define DomainPartitioner_h

#ifndef _bool_h
#include <bool.h>
#endif

#include <ID.h>

class GraphPartitioner;
class LoadBalancer;
class PartitionedDomain;
class Vector;
class Graph;


class DomainPartitioner
{
    public:

        DomainPartitioner(GraphPartitioner& theGraphPartitioner,
                          LoadBalancer& theLoadBalancer);

        DomainPartitioner(GraphPartitioner& theGraphPartitioner);
        virtual  ~DomainPartitioner();

        virtual void setPartitionedDomain(PartitionedDomain& theDomain);
        virtual int partition(int numParts);

        # ifdef _PDD
        virtual int repartition(int numParts);
        # endif

        virtual int balance(Graph& theWeightedSubdomainGraph);

        // public member functions needed by the load balancer
        virtual int getNumPartitions(void) const;
        //GZ virtual Graph &getPartitionGraph(void);
        //GZ virtual Graph &getColoredGraph(void);
        //GZ
        //GZ virtual int  swapVertex(int from,
        //GZ                int to,
        //GZ                int vertexTag,
        //GZ                bool adjacentVertexNotInOther = true);
        //GZ
        //GZ virtual int     swapBoundary(int from,
        //GZ                  int to,
        //GZ                  bool adjacentVertexNotInOther = true);
        //GZ
        //GZ virtual int  releaseVertex(int from,
        //GZ                   int vertexTag,
        //GZ                   Graph &theWeightedPartitionGraph,
        //GZ                   bool mustReleaseToLighter = true,
        //GZ                   double factorGreater = 1.0,
        //GZ                   bool adjacentVertexNotInOther = true);
        //GZ
        //GZ virtual int releaseBoundary(int from,
        //GZ                   Graph &theWeightedPartitionGraph,
        //GZ                   bool mustReleaseToLighter = true,
        //GZ                   double factorGreater = 1.0,
        //GZ                   bool adjacentVertexNotInOther = true);

    protected:

    private:
        PartitionedDomain* myDomain;
        GraphPartitioner&  thePartitioner;
        LoadBalancer*      theBalancer;

        Graph* theElementGraph;
        Graph** theBoundaryElements;

        ID* nodePlace;
        ID* elementPlace;
        int numPartitions;
        ID primes;
        bool partitionFlag;
        int minNodeTag;
        int maxNodeTag;

        //# ifdef _PDD //Guanzhou added to keep track of mesh structures, thus to save communication
        //when repartition
        int* ele_nodes;
        int numNodesPerEle;
        ID* ChangedNodeList;
        int numChangedNodes;
        int* data;
        int Repartition_Count;

        int* nodePartition;
        int* ParCount;
        int* newNodePartition;
        int* newParCount;
        //# endif

};

#endif


