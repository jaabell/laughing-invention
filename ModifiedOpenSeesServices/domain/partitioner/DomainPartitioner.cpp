/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is stricADtly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.4 $
// $Date: 2003/10/15 14:21:31 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/partitioner/DomainPartitioner.cpp,v $

// Written: fmk
// Created: Fri Sep 20 15:27:47: 1996
// Revision: A
//
// Description: This file contains the class definition for DomainPartitioner.
// A DomainPartitioner is an object used to partition a PartitionedDomain.
//
// What: "@(#) DomainPartitioner.C, revA"

#include <DomainPartitioner.h>

#include <stdlib.h>
#include <GraphPartitioner.h>
#include <PartitionedDomain.h>
#include <Subdomain.h>
#include <SubdomainIter.h>
#include <Node.h>
#include <Element.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <NodeIter.h>
#include <ElementIter.h>
#include <MP_ConstraintIter.h>
#include <SP_ConstraintIter.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <Graph.h>
#include <Vector.h>
#include <NodalLoad.h>
#include <ElementalLoad.h>
#include <NodalLoadIter.h>
#include <ElementalLoadIter.h>
#include <LoadBalancer.h>
#include <LoadPatternIter.h>
#include <LoadPattern.h>

#include <Timer.h>

# include <ShadowSubdomain.h>

//Guanzhou added for file usage
#include <iostream>
using std::ios;
#include <fstream>
using std::ofstream;
extern ofstream timingfile;


DomainPartitioner::DomainPartitioner(GraphPartitioner &theGraphPartitioner)
    : myDomain(0), thePartitioner(theGraphPartitioner), theBalancer(0),
      theElementGraph(0), theBoundaryElements(0),
      nodePlace(0), elementPlace(0), numPartitions(0), primes(22), partitionFlag(false),
      ChangedNodeList(0)
{
    // set primes to handle up to 8 partitions, can enlarge if needed
    primes(1) = 2;
    primes(2) = 3;
    primes(3) = 5;
    primes(4) = 7;
    primes(5) = 11;
    primes(6) = 13;
    primes(7) = 17;
    primes(8) = 19;
    primes(9) = 23;
    primes(10) = 29;
    primes(11) = 31;
    primes(12) = 37;
    primes(13) = 41;
    primes(14) = 43;
    primes(15) = 47;
    primes(16) = 53;
    primes(17) = 59;
    primes(18) = 61;
    primes(19) = 67;
    primes(20) = 71;
    primes(21) = 73;

    // cerr << "DomainPartitioner::DomainPartitioner - ";
    // cerr << "does not deal with ele loads or mp_constraints yet\n";

    Repartition_Count = 1;
    nodePartition = NULL;
    ParCount = NULL;
    newNodePartition = NULL;
    newParCount = NULL;
    minNodeTag = 0;
    maxNodeTag = 0;
    ele_nodes = NULL;
    data = NULL;

}

DomainPartitioner::DomainPartitioner(GraphPartitioner &theGraphPartitioner,
                                     LoadBalancer &theLoadBalancer)
    : myDomain(0), thePartitioner(theGraphPartitioner), theBalancer(&theLoadBalancer),
      theElementGraph(0), theBoundaryElements(0),
      nodePlace(0), elementPlace(0), numPartitions(0), primes(22), partitionFlag(false),
      ChangedNodeList(0)
{
    // set primes to handle up to 8 partitions, can enlarge if needed
    primes(1) = 2;
    primes(2) = 3;
    primes(3) = 5;
    primes(4) = 7;
    primes(5) = 11;
    primes(6) = 13;
    primes(7) = 17;
    primes(8) = 23;
    primes(9) = 31;
    primes(10) = 29;
    primes(11) = 31;
    primes(12) = 37;
    primes(13) = 41;
    primes(14) = 43;
    primes(15) = 47;
    primes(16) = 53;
    primes(17) = 59;
    primes(18) = 61;
    primes(19) = 67;
    primes(20) = 71;
    primes(21) = 73;

    // set the links the loadBalancer needs
    theLoadBalancer.setLinks(*this);
    Repartition_Count = 1;
    nodePartition = NULL;
    ParCount = NULL;
    newNodePartition = NULL;
    newParCount = NULL;
    minNodeTag = 0;
    maxNodeTag = 0;     //NOTE: minNodeTag will always be zero!! Tags are always positive nonzero -JAbell
    ele_nodes = NULL;
    data = NULL;
}


DomainPartitioner::~DomainPartitioner()
{
    if (theBoundaryElements != 0)
    {
        for (int i = 0; i < numPartitions; i++)
            if (theBoundaryElements[i] != 0)
            {
                delete theBoundaryElements[i];
            }
        delete []theBoundaryElements;
    }

    if (theElementGraph != 0)
    {
        delete theElementGraph;
    }
# ifdef _PDD
    if ( ele_nodes != 0 )
    {
        delete [] ele_nodes;
    }
    if ( nodePartition != NULL )
    {
        delete [] nodePartition;
    }
    if ( ParCount != NULL )
    {
        delete [] ParCount;
    }
# endif


}

void
DomainPartitioner::setPartitionedDomain(PartitionedDomain &theDomain)
{
    myDomain = &theDomain;
}

int
DomainPartitioner::partition(int numParts)
{
    // first we ensure the partitioned domain has numpart subdomains
    // with tags 1 through numparts
    for (int i = 1; i <= numParts; i++)
    {
        Subdomain *subdomainPtr = myDomain->getSubdomainPtr(i);
        if (subdomainPtr == 0)
        {
            cerr << "DomainPartitioner::partition - No Subdomain: ";
            cerr << i << " exists\n";
            return -1;
        }

        subdomainPtr->partition(numParts);

    }

    // cout << "    Creating Element Graph\n";

    // we get the ele graph from the domain and partition it

    // theElementGraph = new Graph(*(myDomain->getElementGraph()));
    theElementGraph = myDomain->getElementGraph();
    int theError = thePartitioner.partition(theElementGraph, numParts);

    if (theError < 0)
    {
        cerr << "DomainPartitioner::partition";
        cerr << " - the graph partioner failed to partition the ";
        cerr << "element graph\n";
        return -10 + theError;
    }
    // cout << "       Done! Creating Element Graph\n";

    // cout << "    Partitioning Domain\n";
    // we create empty graphs for the numParts subdomains,
    // in the graphs we place the vertices for the elements on the boundaries

    // we do not invoke the destructor on the individual graphs as
    // this would invoke the destructor on the individual vertices

    numPartitions = numParts;

    // we now iterate through the nodes to first determine
    // min and max node numbers

    NodeIter &theNodes = myDomain->getNodes();
    Node *nodePtr;

    maxNodeTag = myDomain->maxNodesTag;   // [Sumeet August 2016]
    minNodeTag = 0;                       // minimum node tag will always be zero [Jabell and Sumeet August 2016]

    // we now create an ID which will contain information
    // about which partition the node is located in

    delete [] nodePartition;
    nodePartition = NULL;
    nodePartition = new int[(maxNodeTag + 1) * 8];  //Note:  why *8 here? -JAbell

    delete [] ParCount;
    ParCount = NULL;
    ParCount = new int[maxNodeTag + 1];


    if (nodePartition == 0 || ParCount == 0)
    {
        cerr << "DomainPartitioner::partition(int numParts)";
        cerr << " - ran out of memory\n";
        numPartitions = 0;
        return -1;
    }

    for (int m = minNodeTag; m <= maxNodeTag; m++)
    {
        ParCount[m] = 0;
        for (int i = 0; i < 8; i++)
        {
            nodePartition[m * 8 + i] = 0;
        }
    }

    //
    // we now iterate through the vertices of the element graph
    // to see if the vertex is a boundary vertex or not - if it is
    // we add to the appropriate graph created above. We also set the
    // value the color variable of each of the external nodes connected
    // to the element to a value which will indicate that that node will
    // have to be added to the subdomain.
    //


    // const int eleNum = theElementGraph->getNumVer:)tex();
    const int eleNum =  myDomain->maxElementsTag; // Added by Sumeet August, 2016
    elementPlace = new ID(eleNum);

    if ( elementPlace == NULL)
    {
        cerr << "DomainPartitioner::partition() - can notallocate memory for elementPlace\n";
    }
    int *where = new int[eleNum];

    if ( where == NULL)
    {
        cerr << "DomainPartitioner::partition() - can notallocate memory for where\n";
    }


    //Looping over the element  ...


    //Added by Babak Kamrani on 1/16/13:
    //-----------------------
    int i_elem_numNodes = 0;                    // Number of the Nodes in each Element
    int total_nodes_Num = 0;                    // Total Number of Nodes in the existing elements
    int *ele_node_index = new int[eleNum + 1];  // Will be used to store the first node number of each element
    if ( ele_node_index == NULL)
    {
        cerr << "DomainPartitioner::partition() - cannot allocate memory for ele_node_index\n";
    }

    ele_node_index[0] = 0;
    for (int i_elem = 1; i_elem <= eleNum ; i_elem++ )
    {
        Element *elePtr = myDomain->getElement(i_elem);
        if(elePtr){  // Added by Sumeet August, 2016
            const ID &nodes = elePtr->getExternalNodes();
            i_elem_numNodes = nodes.Size();
            total_nodes_Num = total_nodes_Num + i_elem_numNodes;
            ele_node_index [i_elem] = (int)nodes(0);
        }

    }

    ele_nodes = new int [total_nodes_Num];
    if ( ele_nodes == NULL)
    {
        cerr << "DomainPartitioner::partition() - can not allocate memory for ele_nodes\n";
    }

    VertexIter &theVertexIter = theElementGraph->getVertices();
    Vertex *vertexPtr;

    int i_ele_nodes = 0; //Added by Babak Kamrani on 1/16/13:

    while ((vertexPtr = theVertexIter()) != 0)
    {
        int eleTag = vertexPtr->getRef();
        int vertexColor = vertexPtr->getColor();

        //Guanzhou added here to export to file for PMVIS
        *(where + eleTag - 1) = vertexColor;
        (*elementPlace)(eleTag - 1) = vertexColor;


        Element *elePtr = myDomain->getElement(eleTag);

        const ID &nodes = elePtr->getExternalNodes();
        int size = nodes.Size();


        //Added by Babak Kamrani on 1/16/13:
        //-----------------------
        for (int j = 0; j < size; j++)
        {
            const int node = nodes(j);

            ele_nodes[i_ele_nodes] = node;

            int count = ParCount[node];
            cout << "count " << count << endl;
            bool added = false;
            for (int k = 0; k < count; k++)
            {
                if ( nodePartition[node * 8 + k] == vertexColor)
                {
                    added = true;
                }
            }
            if ( !added )
            {
                nodePartition[node * 8 + count] = vertexColor;
                ParCount[node]++;
            }

            i_ele_nodes++;
        }


    }

    // now go through the MP_Constraints and ensure:
    //  1. if in different partitions both on boundary
    //  2. if constrained on boundary - retained also on boundary
    MP_ConstraintIter &theMPs = myDomain->getMPs();
    MP_Constraint *mpPtr;
    while ((mpPtr = theMPs()) != 0)
    {
        int retained = mpPtr->getNodeRetained();
        int constrained = mpPtr->getNodeConstrained();
        int count = ParCount[retained];

        bool added = false;
        for (int k = 0; k < count; k++)
        {
            if ( nodePartition[retained * 8 + k] == 0)
            {
                added = true;
            }
        }
        if ( !added )
        {
            nodePartition[retained * 8 + count] = 0; //0 means master proc!
            ParCount[retained]++;
        }

        count = ParCount[constrained];
        added = false;
        for (int k = 0; k < count; k++)
        {
            if ( nodePartition[constrained * 8 + k] == 0)
            {
                added = true;
            }
        }
        if ( !added )
        {
            nodePartition[constrained * 8 + count] = 0; //0 means master proc!
            ParCount[constrained]++;
        }
    }

    cout << "       Done! Partitioning\n";

    // now we go through the load patterns and move NodalLoads and MP_Constraints
    // 1) make sure each subdomain has a copy of the partitioneddomains load patterns.
    // 2) move nodal loads
    // 3) move SP_Constraints


    // we now add the nodes, by iterating through the nodes and for
    // each node determining if it is an internal or external node
    // to each subdomain
    cout << "    Sending Nodes.\n";
    NodeIter &theNodes2 = myDomain->getNodes();

    //Some counters of components for reporting
    int nodesOnEachPart[numParts + 1];
    for (int ipart = 0; ipart < numParts + 1; ipart++)
    {
        nodesOnEachPart[ipart] = 0;
    }
    int numExternalNodes = 0;
    int numTotalNodes = 0;
    while ((nodePtr = theNodes2()) != 0)
    {
        int nodeTag = nodePtr->getTag();

        const int count = ParCount[nodeTag];

        if (count == 1)
        {
            // its an internal node
            // remove the node & add it to the subdomain
            // cout << "Sending node " << nodeTag << " internal" << endl;

            const int par = nodePartition[nodeTag * 8 + count - 1];
            if ( par != 0 )
            {
                Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
                nodePtr = myDomain->Domain::removeNode(nodePtr->getTag());
                myDomain->add_Node_Partition_Info(nodeTag,par);  // [Sumeet August,2016]
                theSubdomain->addNode(nodePtr);
                nodesOnEachPart[par]++;
            }
        }
        else
        {
            // its external, don't remove from mydomain, keep it global
            // cout << "Sending node " << nodeTag << " external" << endl;

            for (int l = 0; l < count; l++)
            {
                const int par = nodePartition[nodeTag * 8 + l];
                Subdomain *theSubdomain;
                if ( par != 0 )
                {
                    theSubdomain = myDomain->getSubdomainPtr(par);
                    myDomain->add_Node_Partition_Info(nodeTag,par);  // [Sumeet August,2016]
                }
                else
                {
                    continue;
                }
                theSubdomain->addExternalNode(nodePtr);
                // nodesOnEachPart[par]++;
            }
            numExternalNodes++;
        }
        numTotalNodes++;
    }
    cout << "       Total Number of Nodes          = " << numTotalNodes << "\n";
    for (int ipart = 0; ipart < numParts + 1 ; ipart++)
    {
        cout << "       Nodes on Partition " << ipart <<  "          = " << nodesOnEachPart[ipart] << "\n";
    }
    cout << "       Total Number of External Nodes = " << numExternalNodes << "\n";
    cout << "    Done Sending Nodes.\n";



    //Guanzhou moved codes here after the Subdomain->addExternalNode, which does numDOF increment
    LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
    LoadPattern *theLoadPattern;
    int number_of_nodal_loads = 0;
    int number_of_sps = 0;
    while ((theLoadPattern = theLoadPatterns()) != 0)
    {
        int loadPatternTag = theLoadPattern->getTag();

        // check that each subdomain has a loadPattern with a similar tag and class tag
        //add LoadPattern to all subdomains
        for (int i = 1; i <= numParts; i++)
        {
            Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);
            LoadPattern *loadPatternCopy = theSubdomain->getLoadPattern(loadPatternTag);
            if (loadPatternCopy == 0)
            {
                LoadPattern *newLoadPattern = theLoadPattern->getCopy();
                if (newLoadPattern == 0)
                {
                    cerr << "DomainPartitioner::partition - out of memory creating LoadPatterns\n";
                    return -1;
                }

                theSubdomain->addLoadPattern(newLoadPattern);

            }
        }

        NodalLoadIter &theNodalLoads = theLoadPattern->getNodalLoads();
        NodalLoad *theNodalLoad;
        while ((theNodalLoad = theNodalLoads()) != 0)
        {
            int loadedNodeTag = theNodalLoad->getNodeTag();

            const int count = ParCount[loadedNodeTag];

            if (count == 1)
            {
                // its an internal node
                const int par = nodePartition[loadedNodeTag * 8 + count - 1];
                if ( par != 0 )
                {
                    theLoadPattern->removeNodalLoad(theNodalLoad->getTag());

                    Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
                    if ((theSubdomain->addNodalLoad(theNodalLoad, loadPatternTag)) != true)
                    {
                        cerr << "DomainPartitioner::partition() - failed to add Nodal Load\n";
                    }
                }
                number_of_nodal_loads++;
            }
        }


        SP_ConstraintIter &theSPs = theLoadPattern->getSPs();
        SP_Constraint *spPtr;
        while ((spPtr = theSPs()) != 0)
        {
            int constrainedNodeTag = spPtr->getNodeTag();

            const int count = ParCount[constrainedNodeTag];

            if (count == 1)
            {
                // its an internal node
                const int par = nodePartition[constrainedNodeTag * 8 + count - 1];
                if ( par != 0 )
                {
                    Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
                    theLoadPattern->removeSP_Constraint(spPtr->getTag());
                    theSubdomain->addSP_Constraint(spPtr, loadPatternTag);
                }
                number_of_sps++;
            }
        }
    }

    cout << "   Sent " << number_of_nodal_loads << " nodal loads!\n";
    cout << "   Sent " << number_of_sps << " SP constraints!\n";

    cout << "   Sending Elements!\n";
    // we now move the elements and any elemental Loads in the loadPatterns
    VertexIter &theVertices = theElementGraph->getVertices();
    int elementsOnEachPart[numParts];
    for (int ipart = 0; ipart < numParts + 1; ipart++)
    {
        elementsOnEachPart[ipart] = 0;
    }
    int numTotalElements = 0;
    int numTotalElementalLoads = 0;
    while ((vertexPtr = theVertices()) != 0)
    {
        // move the element
        int partition = vertexPtr->getColor();

        int eleTag = vertexPtr->getRef();
        Element *elePtr = myDomain->removeElement(eleTag);

        myDomain->add_Element_Partition_Info(eleTag,partition);  // [Sumeet August,2016]
        Subdomain *theSubdomain = myDomain->getSubdomainPtr(partition);
        elementsOnEachPart[partition]++;

        theSubdomain->addElement(elePtr);


        /*********** ELEMENT LOADS ***************************************/
        // if any corresponding elemental loads in the load patterns .. move the load as well
        // LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
        // LoadPattern *theLoadPattern;

        // while ((theLoadPattern = theLoadPatterns()) != 0)
        // {
        //     int loadPatternTag = theLoadPattern->getTag();
        //     ElementalLoadIter &theLoads = theLoadPattern->getElementalLoads();
        //     ElementalLoad *theLoad;
        //     while ((theLoad = theLoads()) != 0)
        //     {
        //         // if ( theLoad->hasElement(eleTag) )
        //         // {
        //         // cout << "  --->>> !!!! Moving load # " << theLoad->getTag() << " to domain number " << theSubdomain->getTag() << endl;
        //         theLoadPattern->removeElementalLoad(theLoad->getTag());
        //         theSubdomain->addElementalLoad(theLoad, loadPatternTag);
        //         numTotalElementalLoads++;
        //         // }
        //     }
        // }
        /********************* ELEMENT LOADS ****************************/
        // cout << endl;
        numTotalElements++;
    }
    cout << "       Total Number of Elements          = " << numTotalElements << "\n";
    cout << "       Total Number of Elemental Loads   = " << numTotalElementalLoads << "\n";
    for (int ipart = 0; ipart < numParts + 1 ; ipart++)
    {
        cout << "       Elements on Partition " << ipart <<  "          = " << elementsOnEachPart[ipart] << "\n";
    }


    // add the single point constraints, only added if for an internal node in a subdomain

    cout << "   Done sending elements!\n";

    cout << "   Sending Elemental Loads!\n";
    LoadPatternIter &theLoadPatterns1 = myDomain->getLoadPatterns();

    while ((theLoadPattern = theLoadPatterns1()) != 0)
    {
        ElementalLoadIter &theLoads = theLoadPattern->getElementalLoads();
        ElementalLoad *theLoad;
        int loadPatternTag = theLoadPattern->getTag();
        while ((theLoad = theLoads()) != 0)
        {
            Subdomain *theSubDomain3;
            SubdomainIter &theSubDomains3 = myDomain->getSubdomains();
            while ((theSubDomain3 = theSubDomains3()) != 0)
            {
                theSubDomain3->addElementalLoad(theLoad, loadPatternTag);
            }
            theLoadPattern->removeElementalLoad(theLoad->getTag());
        }
    }

    cout << "   Done sending Elemental Loads!\n";

    cout << "   Sending Single Point (SP) constraints!\n";
    //Guanzhou changed below
    SP_ConstraintIter &theSPs = myDomain->getSPs();
    SP_Constraint *spPtr;
    int numTotalSPs = 0;
    int spsOnEachPart[numParts + 1];
    for (int ipart = 0; ipart < numParts + 1; ipart++)
    {
        spsOnEachPart[ipart] = 0;
    }

    while ((spPtr = theSPs()) != 0)
    {
        int constrainedNodeTag = spPtr->getNodeTag();

        const int count = ParCount[constrainedNodeTag];

        if (count == 1)
        {
            // its an internal node
            const int par = nodePartition[constrainedNodeTag * 8 + count - 1];

            if ( par != 0 )
            {
                Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
                myDomain->removeSP_Constraint(spPtr->getTag());
                theSubdomain->addSP_Constraint(spPtr);
                spsOnEachPart[par]++;
            }
        }
        else
        {
            for (int l = 0; l < count; l++)
            {
                const int par = nodePartition[constrainedNodeTag * 8 + l];

                Subdomain *theSubdomain;
                if ( par != 0 )
                {
                    theSubdomain = myDomain->getSubdomainPtr(par);
                    theSubdomain->addSP_Constraint(spPtr);
                    spsOnEachPart[par]++;
                }
            }
        }

        numTotalSPs++;
    }

    cout << "       Total Number of SP constraints = " << numTotalSPs << "\n";
    for (int ipart = 0; ipart < numParts + 1 ; ipart++)
    {
        cout << "       SP constraints on Partition " << ipart <<  "          = " << spsOnEachPart[ipart] << "\n";
    }
    cout << "   Done sending Single Point constraints!\n";



    cout << "   Sending Multi-Point (MP) constraints!\n";
    SubdomainIter &theSubDomains1 = myDomain->getSubdomains();
    Subdomain *theSubDomain;
    int countSPs = 0;
    while ((theSubDomain = theSubDomains1()) != 0)
    {
        MP_ConstraintIter &theMPs = myDomain->getMPs();
        MP_Constraint *mpPtr;
        while ((mpPtr = theMPs()) != 0)
        {
            theSubDomain->addMP_Constraint(mpPtr);
            countSPs++;
        }
    }
    cout << "      Sent " << countSPs / numParts << " MP constraints. \n";
    cout << "   Done sending Multi-Point constraints!\n";


    cout << "   Updating subdomains!\n";
    // now we go through all the subdomains and tell them to update
    // their analysis for the new layouts
    SubdomainIter &theSubDomains2 = myDomain->getSubdomains();
    while ((theSubDomain = theSubDomains2()) != 0)
    {
        //theSubDomain->resetRecorders();
        theSubDomain->domainChange();
    }
    cout << "   Done updating subdomains!\n";


    myDomain->domainChange();

    partitionFlag = true;

    myDomain->sendOutputOptionsToSubdomains();




    return 0;
}


int
DomainPartitioner::balance(Graph &theWeightedPGraph)
{
    int res = 0;

    // check that the object did the partitioning
    if (partitionFlag == false)
    {
        cerr << "DomainPartitioner::balance(const Vector &load)";
        cerr << " - not partitioned or DomainPartitioner did not partition\n";
        return -1;
    }

    if (theBalancer != 0)
    {

        // call on the LoadBalancer to partition
        res = theBalancer->balance(theWeightedPGraph);

        // now invoke domainChanged on Subdomains and PartitionedDomain
        SubdomainIter &theSubDomains = myDomain->getSubdomains();
        Subdomain *theSubDomain;

        while ((theSubDomain = theSubDomains()) != 0)
        {
            theSubDomain->domainChange();
        }

        // we invoke change on the PartitionedDomain
        myDomain->domainChange();
    }

    return res;
}



int
DomainPartitioner::getNumPartitions(void) const
{
    return numPartitions;
}


# ifdef _PDD  //Guanzhou added for repartitioning
int
DomainPartitioner::repartition(int numParts)
{
    // cout << "DomainPartitioner::repartition( " << numParts << ")" << endl;
    // double start_time = MPI_Wtime();
    // first we ensure the partitioned domain has numpart subdomains
    // with tags 1 through numparts
    return 0;

    Subdomain **subdomainPtrs = new Subdomain * [numParts];

    for (int i = 1; i <= numParts; i++)
    {
        subdomainPtrs[i - 1] = myDomain->getSubdomainPtr(i);

        if (subdomainPtrs[i - 1] == 0)
        {
            cerr << "DomainPartitioner::repartition - No Subdomain: ";
            cerr << i << " exists\n";
            return -1;
        }
        subdomainPtrs[i - 1]->repartition(numParts);
    }
    // cout << "DomainPartitioner::repartition( " << numParts << ")  ---  1" << endl;
    // we get the ele graph from the domain and partition it
    Graph *theEleGraph = myDomain->getElementGraph();
    //Guanzhou: we have to keep the original graph theElementGraph = new Graph(myDomain->getElementGraph());

    //    int theError = thePartitioner.repartition(theElementGraph, numParts); /*Out  by B. Kamrani September 3dr 2010 */
    int theError = thePartitioner.repartition(theEleGraph, numParts); /*Replaced by B. Kamrani September 3dr 2010 */
    // int theError = thePartitioner.repartition(theElementGraph, numParts); /*Replaced by B. Kamrani September 3dr 2010 */

    if (theError < 0)
    {
        cerr << "DomainPartitioner::repartition";
        cerr << " - the graph partioner failed to repartition the ";
        cerr << "element graph\n";
        return -10 + theError;
    }

    bool repartitioned = false;

    VertexIter &VerIter = theElementGraph->getVertices();
    Vertex *verPtr;
    while ((verPtr = VerIter()) != 0)
    {
        int eleTag = verPtr->getRef();
        int vertexColor = verPtr->getColor();
        if ( (*elementPlace)(eleTag - 1) != vertexColor)
        {
            repartitioned = true;
            break;
        }
    }

    // cout << "DomainPartitioner::repartition( " << numParts << ")  ---  2" << endl;
    if ( !repartitioned )
    {
        // cout << "DomainPartitioner::repartition( " << numParts << ")  - It was not necessary to repartition this step. " << endl;
        return 0;    //quick return
    }
    else
    {
        // cout << "DomainPartitioner::repartition( " << numParts << ")  - Rebalancing domain. " << endl;
    }

    //The domain is repartitioned!!!!
    //record the new element place

    VertexIter &VerIter1 = theElementGraph->getVertices();
    Vertex *verPtr1;

    if ( elementPlace != 0 )
    {
        delete elementPlace;
    }

    cout << "DomainPartitioner::repartition( " << numParts << ")  ---  3" << endl;
    elementPlace = new ID(theElementGraph->getNumVertex());

    while ((verPtr1 = VerIter1()) != 0)
    {
        int eleTag = verPtr1->getRef();
        int vertexColor = verPtr1->getColor();
        (*elementPlace)(eleTag - 1) = vertexColor;
    }


    newNodePartition = new int[(maxNodeTag + 1) * 8];
    newParCount = new int[maxNodeTag + 1];

    if (newNodePartition == 0 || newParCount == 0)
    {
        cerr << "DomainPartitioner::partition(int numParts)";
        cerr << " - ran out of memory\n";
        numPartitions = 0;
        return -1;
    }

    cout << "DomainPartitioner::repartition( " << numParts << ")  ---  4" << endl;

    for (int m = minNodeTag; m <= maxNodeTag; m++)
    {
        newParCount[m] = 0;
        for (int i = 0; i < 8; i++)
        {
            newNodePartition[m * 8 + i] = 0;
        }
    }

    // char *theCount = new char [2];
    // sprintf(theCount, "%d", Repartition_Count);
    cout << " Number of repartitions = " << Repartition_Count << endl;

    // char *filename = new char [10];
    // strcpy (filename, "repart");
    // strcat(filename, theCount);
    string filename("repart_");
    stringstream ss;
    ss << filename;
    ss << Repartition_Count;
    filename = ss.str();
    cout << "DomainPartitioner::repartition( " << numParts << ")  ---  5" << endl;

    ofstream partfile(filename.c_str(), ios::out);
    const int eleNum = theElementGraph->getNumVertex();
    int *where = new int[eleNum];

    VertexIter &theVertexIter = theElementGraph->getVertices();
    Vertex *vertexPtr;
    while ((vertexPtr = theVertexIter()) != 0)
    {
        int eleTag = vertexPtr->getRef();
        int vertexColor = vertexPtr->getColor();

        *(where + eleTag - 1) = vertexColor;

        int numNodesPerEle = myDomain->return_element_numberofNodes(eleTag);

        for (int j = 0; j < numNodesPerEle; j++)
        {
            int node = ele_nodes[(eleTag - 1) * numNodesPerEle + j];

            int count = newParCount[node];
            bool added = false;
            for (int k = 0; k < count; k++)
            {
                if ( newNodePartition[node * 8 + k] == vertexColor)
                {
                    added = true;
                }
            }
            if ( !added )
            {
                newNodePartition[node * 8 + count] = vertexColor;
                newParCount[node]++;
            }
        }
    }

    numChangedNodes = 0;
    if ( ChangedNodeList != 0 )
    {
        delete ChangedNodeList;
    }
    if ( data != 0 )
    {
        delete [] data;
    }

    data = new int [maxNodeTag + 1];

    for (int m = minNodeTag; m <= maxNodeTag; m++)
    {
        if ( newParCount[m] != ParCount[m] )
        {
            data[numChangedNodes] = m;
            numChangedNodes++;
            continue;
        }
        for (int i = 0; i < newParCount[m]; i++)
        {
            bool changed = true;
            for (int j = 0; j < ParCount[m]; j++)
            {
                if ( newNodePartition[m * 8 + i] == nodePartition[m * 8 + j])
                {
                    changed = false;
                    break;
                }
            }
            if ( changed )
            {
                //(*ChangedNodeList)(numChangedNodes) = m;
                data[numChangedNodes] = m;
                numChangedNodes++;
                break;
            }
        }
    }

    cout << "DomainPartitioner::repartition, numChangedNodes " << numChangedNodes << endln;
    ChangedNodeList = new ID(data, numChangedNodes, false);
    //ChangeNodeList->setData(data, numChangedNodes, true);

    cout << "DomainPartitioner::repartition, finished creating ChangedNodeList: " << numChangedNodes << endln;

    for (int i = 1 ; i <= numParts; i++)
    {
        subdomainPtrs[i - 1]->recvChangedNodes(*ChangedNodeList, numChangedNodes);
    }

    cout << "DomainPartitioner::repartition, finished sending ChangedNodeList: " << numChangedNodes << endln;

    for (int i = 1; i <= eleNum; i++)
    {
        partfile << *(where + i - 1) << '\n';
    }

    delete [] where;
    partfile.close();

    // delete [] filename;
    // delete [] theCount;
    Repartition_Count++;

    cout << "DomainPartitioner::repartition, starting data redistribution!!!\n";

    for (int i = 0; i < numChangedNodes; i++)
    {
        int nodeTag = (*ChangedNodeList)(i);
        int dof = -1;
        const int count = newParCount[nodeTag];

        if (count == 1)
        {
            // its an internal node
            const int par = newNodePartition[nodeTag * 8 + count - 1];
            if (myDomain->Domain::getNode(nodeTag) != 0)  //external previously, must search others to remove the node
            {
                Node *nodePtr = myDomain->Domain::removeNode(nodeTag);
                dof = nodePtr->getNumberDOF();
                for (int k = 1; k <= numParts; k++)
                {
                    if ( k != par )
                    {
                        if (subdomainPtrs[k - 1]->hasExternalNode(nodeTag))
                        {
                            subdomainPtrs[k - 1]->removeNode(nodeTag);
                        }
                    }
                    else
                    {
                        //external to j, we do swap here!
                        if (subdomainPtrs[par - 1]->hasExternalNode(nodeTag)) //an external node in j previously
                        {
                            subdomainPtrs[par - 1]->swapNodeFromExternalToInternal(nodeTag, dof);
                        }
                        //break;
                        else //j does not have the node previously
                        {
                            subdomainPtrs[par - 1]->addNode(nodePtr);
                        }
                    }
                }
            }
            else    //previously internal to ONE other subdomain, not j
            {
                for (int k = 1; k <= numParts; k++)
                {
                    if ( k != par )
                    {
                        if ( (subdomainPtrs[k - 1]->hasInternalNode(nodeTag)) )
                        {
                            subdomainPtrs[par - 1]->ChangeMPIChannel(k);
                            subdomainPtrs[k - 1]->exportInternalNode(nodeTag, par, dof);
                            subdomainPtrs[par - 1]->addNodeTag(nodeTag, dof);
                            break;
                        }
                    }
                }
            }
        }
        else      // its external, don't remove from mydomain, keep it global
        {
            Node *nodePtr = myDomain->Domain::getNode(nodeTag);
            if ( nodePtr != 0)   //previously external, but now external to different subdomains
            {
                for (int l = 0; l < count; l++)
                {
                    //search all subdomains, if they are in the "others" list, add node, else remove node!
                    const int par = newNodePartition[nodeTag * 8 + l];
                    if ( !subdomainPtrs[par - 1]->hasExternalNode(nodeTag) )
                    {
                        subdomainPtrs[par - 1]->addExternalNode(nodePtr);
                    }
                }

                for (int k = 0; k < ParCount[nodeTag]; k++)
                {
                    const int par = nodePartition[nodeTag * 8 + k];
                    bool OneOfOthers = false;
                    for (int n = 0; n < count; n++)
                    {
                        if ( newNodePartition[nodeTag * 8 + n] == par )
                        {
                            OneOfOthers = true;
                            break;
                        }
                    }
                    if ( OneOfOthers )
                    {
                        if ( !subdomainPtrs[par - 1]->hasExternalNode(nodeTag) )
                        {
                            subdomainPtrs[par - 1]->addExternalNode(nodePtr);
                        }
                    }
                    else
                    {
                        if ( subdomainPtrs[par - 1]->hasExternalNode(nodeTag) )
                        {
                            subdomainPtrs[par - 1]->removeNode(nodeTag);
                        }
                    }
                }

            }
            else     //previously internal to ONE specific subdomain (can be j!!!), search for that subdomain and swap that to master and all of the subdomains in the others list!
            {
                int pos = nodePartition[nodeTag * 8];
                bool need = false;
                for (int k = 0; k < count; k++)
                {
                    const int par = newNodePartition[nodeTag * 8 + k];
                    if ( par == pos )
                    {
                        need = true;
                        break;
                    }
                }

                if ( !need )
                {
                    subdomainPtrs[pos - 1]->exportInternalNode(nodeTag, 0, dof);    //0 is the master process!
                }
                else
                {
                    subdomainPtrs[pos - 1]->swapNodeFromInternalToExternal(nodeTag);    //do swap (in subdomain, will send the node to master)
                }

                Node *nodePtr = myDomain->Domain::getNode(nodeTag);
                if ( nodePtr == 0 )
                {
                    cerr << "DomainPartitioner::repatition, error when retrieve node in master!\n";
                    return -1;
                }

                for (int k = 0; k < count; k++)
                {
                    const int par = newNodePartition[nodeTag * 8 + k];
                    if ( par != pos )
                    {
                        subdomainPtrs[par - 1]->addExternalNode(nodePtr);
                    }
                }
            }
        }
    }

    LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
    LoadPattern *theLoadPattern;
    while ((theLoadPattern = theLoadPatterns()) != 0)
    {

        int loadPatternTag = theLoadPattern->getTag();

        NodalLoadIter &theNodalLoads = theLoadPattern->getNodalLoads();
        NodalLoad *theNodalLoad;
        while ((theNodalLoad = theNodalLoads()) != 0)
        {
            int loadedNodeTag = theNodalLoad->getNodeTag();

            bool changed = false;
            if ( newParCount[loadedNodeTag] != ParCount[loadedNodeTag] )
            {
                changed = true;
            }
            else
            {
                for (int i = 0; i < newParCount[loadedNodeTag]; i++)
                {
                    bool moved = true;
                    for (int j = 0; j < ParCount[loadedNodeTag]; j++)
                    {
                        if ( newNodePartition[loadedNodeTag * 8 + i] == nodePartition[loadedNodeTag * 8 + j])
                        {
                            moved = false;
                            break;
                        }
                    }
                    if ( moved )
                    {
                        changed = true;
                        break;
                    }
                }
            }
            if ( !changed )
            {
                continue;
            }
            if ( myDomain->Domain::getNode(loadedNodeTag) == 0)
            {
                theLoadPattern->removeNodalLoad(theNodalLoad->getTag());
                for (int k = 1; k <= numParts; k++)
                {
                    if ( subdomainPtrs[k - 1]->hasInternalNode(loadedNodeTag) )
                    {
                        if ((subdomainPtrs[k - 1]->addNodalLoad(theNodalLoad, loadPatternTag)) != true)
                        {
                            cerr << "DomainPartitioner::repartition, failed to add Nodal Load to loadPattern\n";
                            return -1;
                        }
                        break;
                    }
                }
            }
        }

        SP_ConstraintIter &theSPs = theLoadPattern->getSPs();
        SP_Constraint *spPtr;
        while ((spPtr = theSPs()) != 0)
        {
            int constrainedNodeTag = spPtr->getNodeTag();

            bool changed = false;
            if ( newParCount[constrainedNodeTag] != ParCount[constrainedNodeTag] )
            {
                changed = true;
            }
            else
            {
                for (int i = 0; i < newParCount[constrainedNodeTag]; i++)
                {
                    bool moved = true;
                    for (int j = 0; j < ParCount[constrainedNodeTag]; j++)
                    {
                        if ( newNodePartition[constrainedNodeTag * 8 + i] == nodePartition[constrainedNodeTag * 8 + j])
                        {
                            moved = false;
                            break;
                        }
                    }
                    if ( moved )
                    {
                        changed = true;
                        break;
                    }
                }
            }
            if ( !changed )
            {
                continue;
            }


            if ( myDomain->Domain::getNode(constrainedNodeTag) == 0)
            {
                theLoadPattern->removeSP_Constraint(spPtr->getTag());
                for (int k = 1; k <= numParts; k++)
                {
                    if ( subdomainPtrs[k - 1]->hasInternalNode(constrainedNodeTag) )
                    {
                        if ((subdomainPtrs[k - 1]->addSP_Constraint(spPtr, loadPatternTag)) != true)
                        {
                            cerr << "DomainPartitioner::repartition, failed to add SP to loadPattern\n";
                            return -1;
                        }
                        break;
                    }
                }
            }
        }
    }

    SP_ConstraintIter &theSP1s = myDomain->getSPs();
    SP_Constraint *spPtr1;
    while ((spPtr1 = theSP1s()) != 0)
    {
        const int constrainedNodeTag = spPtr1->getNodeTag();//all these nodes are external previously

        bool changed = false;
        if ( newParCount[constrainedNodeTag] != ParCount[constrainedNodeTag] )
        {
            changed = true;
        }
        else
        {
            for (int i = 0; i < newParCount[constrainedNodeTag]; i++)
            {
                bool moved = true;
                for (int j = 0; j < ParCount[constrainedNodeTag]; j++)
                {
                    if ( newNodePartition[constrainedNodeTag * 8 + i] == nodePartition[constrainedNodeTag * 8 + j])
                    {
                        moved = false;
                        break;
                    }
                }
                if ( moved )
                {
                    changed = true;
                    break;
                }
            }
        }
        if ( !changed )
        {
            continue;
        }

        if ( myDomain->Domain::getNode(constrainedNodeTag) == 0 )  //this node must be internal to ONE specific subdomain now, but previously it was external to SOME subdomains
        {
            myDomain->Domain::removeSP_Constraint(spPtr1->getTag());
            for (int i = 1; i <= numParts; i++)
            {
                if ( subdomainPtrs[i - 1]->hasInternalNode(constrainedNodeTag) )
                {
                    subdomainPtrs[i - 1]->addSP_Constraint(spPtr1);
                }
            }//subdomains will do redistribute data to delete those SPs without an associated node in the subdomain
        }
        else    // now the node is external to some other subdomains
        {
            for (int i = 1; i <= numParts; i++)
            {
                if ( subdomainPtrs[i - 1]->hasExternalNode(constrainedNodeTag) )
                {
                    subdomainPtrs[i - 1]->addSP_Constraint(spPtr1);    //might already existed, have not figured out better way yet! :((
                }
            }
        }
    }

    for (int pe = 1; pe <= numParts; pe++)
    {
        for (int i = 1; i <= numParts; i++)
        {
            if ( i != pe )
            {
                subdomainPtrs[i - 1]->ChangeMPIChannel(pe);
            }
        }
        subdomainPtrs[pe - 1]->reDistributeData(numParts);
    }


    cout << "DomainPartitioner::repartition, finished data redistribution!!!\n";

    for (int i = 1 ; i <= numParts; i++)
    {
        subdomainPtrs[i - 1]->domainChange();
    }

    myDomain->domainChange();

    partitionFlag = true;



    if ( nodePartition != NULL )
    {
        delete [] nodePartition;
    }
    nodePartition = newNodePartition;
    newNodePartition = NULL;
    if ( ParCount != NULL )
    {
        delete [] ParCount;
    }
    ParCount = newParCount;
    newParCount = NULL;

    delete subdomainPtrs;
    // double end_time = MPI_Wtime();


    return 0;
}


Graph *DomainPartitioner::getElementGraph() const
{
    return theElementGraph;
}

#endif