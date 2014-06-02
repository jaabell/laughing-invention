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

  #ifdef _BABAK_DEBUG
	  extern LoadPattern *DRM_LOAD_PATTERN ;
  #endif

DomainPartitioner::DomainPartitioner(GraphPartitioner &theGraphPartitioner)
:myDomain(0),thePartitioner(theGraphPartitioner),theBalancer(0),
 theElementGraph(0), theBoundaryElements(0),
 nodePlace(0),elementPlace(0), numPartitions(0), primes(22), partitionFlag(false),
 ChangedNodeList(0)
{
    // set primes to handle up to 8 partitions, can enlarge if needed
    primes(1) = 2; primes(2) = 3; primes(3) = 5; primes(4) = 7; primes(5) = 11;
    primes(6) = 13; primes(7) = 17; primes(8) = 19; primes(9) = 23;
    primes(10) = 29; primes(11) = 31; primes(12) = 37; primes(13) = 41;
    primes(14) = 43; primes(15) = 47; primes(16) = 53; primes(17) = 59;
    primes(18) = 61; primes(19) = 67; primes(20) = 71; primes(21) = 73;

    cerr << "DomainPartitioner::DomainPartitioner - ";
    cerr << "does not deal with ele loads or mp_constraints yet\n";

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
:myDomain(0),thePartitioner(theGraphPartitioner),theBalancer(&theLoadBalancer),
 theElementGraph(0), theBoundaryElements(0),
 nodePlace(0), elementPlace(0), numPartitions(0),primes(22), partitionFlag(false),
 ChangedNodeList(0)
{
    // set primes to handle up to 8 partitions, can enlarge if needed
    primes(1) = 2; primes(2) = 3; primes(3) = 5; primes(4) = 7; primes(5) = 11;
    primes(6) = 13; primes(7) = 17; primes(8) = 23; primes(9) = 31;
    primes(10) = 29; primes(11) = 31; primes(12) = 37; primes(13) = 41;
    primes(14) = 43; primes(15) = 47; primes(16) = 53; primes(17) = 59;
    primes(18) = 61; primes(19) = 67; primes(20) = 71; primes(21) = 73;
    
    // set the links the loadBalancer needs
    theLoadBalancer.setLinks(*this);
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


DomainPartitioner::~DomainPartitioner()
{
  if (theBoundaryElements != 0) {
    for (int i=0; i<numPartitions; i++)
      if (theBoundaryElements[i] != 0)
	delete theBoundaryElements[i];
    delete []theBoundaryElements;
  }

  if (theElementGraph != 0)
    delete theElementGraph;
# ifdef _PDD
	if ( ele_nodes != 0 ) delete [] ele_nodes;
	//if ( node_eles != 0 ) delete [] node_eles;
	//if ( ChangedNodeList != 0 ) delete ChangedNodeList;
	if ( nodePartition != NULL ) delete [] nodePartition;
	if ( ParCount != NULL ) delete [] ParCount;
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
    for (int i=1; i<=numParts; i++) 
    {
      Subdomain *subdomainPtr = myDomain->getSubdomainPtr(i);
      //Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);
      if (subdomainPtr == 0) 
      {
	cerr << "DomainPartitioner::partition - No Subdomain: ";
	cerr << i << " exists\n";
	return -1;
      }

#ifdef _PDD
//      else { //Guanzhou added for ParMetis
      	subdomainPtr->partition(numParts);
//      }
#endif

    }
    
    // Timer timer;
    // timer.start();
    
    // we get the ele graph from the domain and partition it
    //    Graph &theEleGraph = myDomain->getElementGraph();
    theElementGraph = new Graph(*(myDomain->getElementGraph()));
    
    int theError = thePartitioner.partition(theElementGraph, numParts);

    if (theError < 0) {
      cerr << "DomainPartitioner::partition";
      cerr << " - the graph partioner failed to partition the ";
      cerr << "element graph\n";
      return -10+theError;
    }

    //cerr << "returned!\n";

    //Guanzhou out cerr << "DomainPArtitioner::PARTITION GRAPH:\n"; theElementGraph->Print(cerr, 4);
    
    //    timer.pause(); cerr << "partition:partition " << timer.getReal() << timer.getCPU() << endln;
    
    // we create empty graphs for the numParts subdomains,
    // in the graphs we place the vertices for the elements on the boundaries
    
    // we do not invoke the destructor on the individual graphs as 
    // this would invoke the destructor on the individual vertices
    
    //GZ if (theBoundaryElements != 0)
    //GZ   delete [] theBoundaryElements;
    //GZ 
    //GZ theBoundaryElements = new Graph * [numParts];
    //GZ if (theBoundaryElements == 0) {
    //GZ   cerr << "DomainPartitioner::partition(int numParts)";
    //GZ   cerr << " - ran out of memory\n";
    //GZ   numPartitions = 0;  
    //GZ   return -1;
    //GZ }
    //GZ 
    //GZ for (int l=0; l<numParts; l++) {
    //GZ   theBoundaryElements[l] = new Graph(2048); // graphs can grow larger; just an estimate
    //GZ   
    //GZ   if (theBoundaryElements[l] == 0) {
    //GZ 	cerr << "DomainPartitioner::partition(int numParts)";
    //GZ 	cerr << " - ran out of memory\n";
    //GZ 	numPartitions = 0;
    //GZ 	return -1;
    //GZ   }
    //GZ }
    
    numPartitions = numParts;
    
    // we iterate through the nodes setting all their partition values to 1
    // the partition value will be used to determine what partition the node 
    // will be added to and whether or not the node is a boundary node.
    
    //GZ change data structure if (numParts+1 >= primes.Size()) { // colors 1 through numParts 
    //GZ change data structure   cerr << "DomainPartitioner::partition(int numParts)";
    //GZ change data structure   cerr << " - primes is  not yet big enough and function not yet written\n";
    //GZ change data structure   return -1;
    //GZ change data structure }
    // we now iterate through the nodes to first determine 
    // min and max node numbers
    
    NodeIter &theNodes = myDomain->getNodes();
    Node *nodePtr;
    //cerr << "finished getting nodes ptr!\n";
    //int maxNodeTag =0;
    //int minNodeTag = 0;
    
    /*ofstream nodefile("nodes.xyz", ios::out);//Guanzhou added
    Vector *coord = new Vector()*/
    
    while ((nodePtr = theNodes()) != 0) 
    {
      //if ( nodePtr->getNumberDOF() != 3) continue;//Guanzhou added
      int tag = nodePtr->getTag();
      if (tag > maxNodeTag) maxNodeTag = tag;
      if (tag < minNodeTag) minNodeTag = tag;
/*      nodefile << nodePtr->getCrds()(0) << ' '
      	       << nodePtr->getCrds()(1) << ' '
	       << nodePtr->getCrds()(2) << '\n'*/	
    }
  
    //cerr << "Finished determining min and mx node tags!\n";

    if (minNodeTag < 0) 
    {
      cerr << "DomainPartitioner::partition(int numParts)";
      cerr << " - minNodeTag < 0 \n";
      numPartitions = 0;
      return -1;  
    }
    
    // we now create an ID which will contain information
    // about which partition the node is located in
    
    //GZ changed data structure nodePlace = new ID(maxNodeTag+1);
    //GZ changed data structure if (nodePlace == 0 || nodePlace->Size() < maxNodeTag+1) {
    //GZ changed data structure   cerr << "DomainPartitioner::partition(int numParts)";
    //GZ changed data structure   cerr << " - ran out of memory\n";
    //GZ changed data structure   numPartitions = 0;
    //GZ changed data structure   return -1;
    //GZ changed data structure }
    //GZ changed data structure for (int m=minNodeTag; m<=maxNodeTag; m++)
    //GZ changed data structure   (*nodePlace)(m) = 1;
    
    //int *nodePartition = new int[maxNodeTag+1][numPartitions+1];
    //int nodePartition [maxNodeTag+1][9];
    cerr << "DomainPartitioner::partition - start allocating memory!\n" << maxNodeTag << endln;
    
    if ( nodePartition != NULL ) delete [] nodePartition;
    nodePartition = NULL;
    nodePartition = new int[(maxNodeTag+1)*8];
    
    cerr << "DomainPartitioner::partition - finished allocating memory for nodepartition!\n";
    
    if ( ParCount != NULL ) delete [] ParCount;
    ParCount = NULL;
    ParCount = new int[maxNodeTag+1];
    
    cerr << "DomainPartitioner::partition - finished allocating memory for partition count!\n";
    
    if (nodePartition == 0 || ParCount == 0) 
    {
      cerr << "DomainPartitioner::partition(int numParts)";
      cerr << " - ran out of memory\n";
      numPartitions = 0;
      return -1;
    }

    //GZ changed data structure if (nodePlace == 0 || nodePlace->Size() < maxNodeTag+1) {
    //GZ changed data structure   cerr << "DomainPartitioner::partition(int numParts)";
    //GZ changed data structure   cerr << " - ran out of memory\n";
    //GZ changed data structure   numPartitions = 0;
    //GZ changed data structure   return -1;
    //GZ changed data structure }
    //GZ changed data structure 
    //GZ changed data structure for (int m=minNodeTag; m<=maxNodeTag; m++)
    //GZ changed data structure   (*nodePlace)(m) = 1;
    
    for (int m=minNodeTag; m<=maxNodeTag; m++) 
    {
      ParCount[m] = 0;
      for (int i=0; i<8; i++) 
      {
      	//nodePartition[m][i]=0;
	nodePartition[m*8+i]=0;
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

    //Guanzhou added
    ofstream partfile("part.dat", ios::out);
    const int eleNum = theElementGraph->getNumVertex();
    elementPlace = new ID(eleNum);
    
    if( elementPlace ==NULL)
    {
	cerr << "DomainPartitioner::partition() - can notallocate memory for elementPlace\n"; 
    }
    int *where = new int[eleNum];
    
    if( where ==NULL)
    {
	cerr << "DomainPartitioner::partition() - can notallocate memory for where\n"; 
    }


    //Changing the data Structure to Store the ele_nodes to include different types of elements:
    //Looping over the element  ...
    
    
    //Added by Babak Kamrani on 1/16/13:     
    //-----------------------
    int i_elem_numNodes = 0; //Number of the Nodes in each Element
    int total_nodes_Num = 0;     //Total Number of Nodes in the existing elements
    int *ele_node_index = new int[eleNum+1]; //Will be used to store the first node number of each element 
    if( ele_node_index ==NULL)
    {
	cerr << "DomainPartitioner::partition() - can notallocate memory for ele_node_index\n"; 
    }
    ele_node_index[0] = 0;
    for (int i_elem = 1;i_elem <= eleNum ;i_elem++ ) 
    {
    	Element *elePtr = myDomain->getElement(i_elem);      
	const ID &nodes = elePtr->getExternalNodes();
      	i_elem_numNodes = nodes.Size();
	total_nodes_Num = total_nodes_Num + i_elem_numNodes;
	ele_node_index [i_elem] = (int)nodes(0);

    }
    ele_nodes = new int [total_nodes_Num];
    if( ele_nodes ==NULL)
    {
	cerr << "DomainPartitioner::partition() - can notallocate memory for ele_nodes\n"; 
    }

    //-----------------------

    //Out by Babak Kamrani on 1/16/13:     
    //-----------------------
    /*
    numNodesPerEle = 4;
    ele_nodes = new int [eleNum*numNodesPerEle]; 
    */
    //numNodesPerEle = 4;

    //-----------------------




    //node_eles = new int [maxNodeTag+1][8];

    VertexIter &theVertexIter = theElementGraph->getVertices();
    Vertex *vertexPtr;

    int i_ele_nodes = 0; //Added by Babak Kamrani on 1/16/13:    

    while ((vertexPtr = theVertexIter()) != 0) 
    {
      int eleTag = vertexPtr->getRef();
      int vertexColor = vertexPtr->getColor();
      
      //Guanzhou added here to export to file for PMVIS
      *(where+eleTag-1) = vertexColor;
      (*elementPlace)(eleTag-1) = vertexColor;
            
      //GZ const ID &adjacency = vertexPtr->getAdjacency();
      //GZ int size = adjacency.Size();
      //GZ for (int i=0; i<size; i++) {
      //GZ 	Vertex *otherVertex = theElementGraph->getVertexPtr(adjacency(i));
      //GZ 	if (otherVertex->getColor() != vertexColor) {
      //GZ 	  theBoundaryElements[vertexColor-1]->addVertex(vertexPtr,false);//theBoundaryElements[vertex-1] records boundary ele of partition[vertexColor]
      //GZ 	  i = size;
      //GZ 	}
      //GZ }
      
      Element *elePtr = myDomain->getElement(eleTag);
      
      const ID &nodes = elePtr->getExternalNodes();
      int size = nodes.Size();
      //GZ int primeVertexColor = primes(vertexColor);
      
      
    //Added by Babak Kamrani on 1/16/13:     
    //-----------------------      
      for (int j=0; j<size; j++) 
      {
	const int node = nodes(j);


       /*	if (i_ele_nodes != ((eleTag-1)*numNodesPerEle+j)  ) 
	{
		abort();
	} */


       	ele_nodes[i_ele_nodes] = node;

       	//ele_nodes[(eleTag-1)*numNodesPerEle+j] = node;
      	
	int count = ParCount[node];    
	bool added = false;
	for (int k=0; k<count; k++) 
	{
		if ( nodePartition[node*8+k] == vertexColor) added = true;
	}
	if ( !added ) 
	{
		nodePartition[node*8+count] = vertexColor;
		ParCount[node]++;
	}
	
      i_ele_nodes++;

      }
    //-----------------------     



    //Out by Babak Kamrani on 1/16/13:     
    //-----------------------      
      
/*      for (int j=0; j<size; j++) 
      {
	const int node = nodes(j);
       	ele_nodes[(eleTag-1)*numNodesPerEle+j] = node;
	//GZ if ((*nodePlace)(node) % primeVertexColor != 0)
	//GZ  (*nodePlace)(node) *= primeVertexColor;
	//GZ // (*nodePlace)(nodeTag) % primes(i) == 0 means node(nodeTag) belongs to partition[i]
      	
	int count = ParCount[node];    
	bool added = false;
	for (int k=0; k<count; k++) 
	{
		if ( nodePartition[node*8+k] == vertexColor) added = true;
	}
	if ( !added ) 
	{
		nodePartition[node*8+count] = vertexColor;
		ParCount[node]++;
	}               
      }
*/
    //-----------------------      


    }
    
    //Guanzhou added
    for (int i=1; i<=eleNum; i++)
    {
//     	partfile << *(where+i-1) << '\n';
       	partfile << i << " " <<*(where+i-1) << '\n';
    	cerr << "*(where+i-1)" << i << " " <<*(where+i-1) <<"\n";
    	
    }
    
    delete [] where;
    partfile.close();

    cerr << "DomainPartitioner::partition() - ADD if ((*nodePlace)(node) % primeVertexColor != 0)\n";
    
    // now go through the MP_Constraints and ensure:
    //   1. if in different partitions both on boundary
    //  2. if constrained on boundary - retained also on boundary
    MP_ConstraintIter &theMPs = myDomain->getMPs();
    MP_Constraint *mpPtr;
    while ((mpPtr = theMPs()) != 0) 
    {
      int retained = mpPtr->getNodeRetained();
      int constrained = mpPtr->getNodeConstrained();
      
      //Guanzhou added
      //Node * nod_r = myDomain->Domain::getNode(retained);
      //Node * nod_c = myDomain->Domain::getNode(constrained);
      //
      //if ( nod_r->getNumberDOF() != 3) {
      //	if ( (*nodePlace)(constrained)%primes(numParts+1) != 0 ) (*nodePlace)(constrained) *= primes(numParts+1);
      //	continue;//Guanzhou added
      //}
      //if ( nod_c->getNumberDOF() != 3) continue;//Guanzhou added
    
      //GZ int *proc = new int[numParts+1];
      //if ((*nodePlace)(retained) != (*nodePlace)(constrained)) { 
      //GZ 	// we could have a problem 1. or 2. above
      //GZ 	int p_r = (*nodePlace)(retained);
      //GZ 	int p_c = (*nodePlace)(constrained);
      //GZ 	int count=0;
      //GZ 	for (int i=1; i<=numParts+1; i++) {
      //GZ 		int prime = primes(i);
      //GZ 		if ( p_r%prime == 0 || p_c%prime == 0) {
      //GZ 			proc[count] = prime;
      //GZ 			count++;
      //GZ 		}
      //GZ 	}
      //GZ 	int target=1;
      //GZ 	for (int i=0; i<count; i++) target *= proc[i];
      //GZ 	(*nodePlace)(retained) = target;
      //GZ 	(*nodePlace)(constrained) = target;
      //GZ 	delete [] proc;
      //GZ 	
    	
      //GZ // for now just put both nodes on boundary
      //GZ 	if ( (*nodePlace)(retained)%primes(numParts+1) != 0 ) (*nodePlace)(retained) *= primes(numParts+1);
      //GZ 	if ( (*nodePlace)(constrained)%primes(numParts+1) != 0 ) (*nodePlace)(constrained) *= primes(numParts+1);
      //GZ //}
    
      	int count = ParCount[retained];
      	bool added = false;
      	for (int k=0; k<count; k++) 
	{
    		if ( nodePartition[retained*8+k] == 0) added = true;
      	}
      	if ( !added ) 
	{
    		nodePartition[retained*8+count] = 0; //0 means master proc!
      		ParCount[retained]++;
    	}
    
      	count = ParCount[constrained];
      	added = false;
      	for (int k=0; k<count; k++) 
	{
    		if ( nodePartition[constrained*8+k] == 0) added = true;
      	}
      	if ( !added ) 
	{
    		nodePartition[constrained*8+count] = 0; //0 means master proc!
      		ParCount[constrained]++;
    	}
    }
    
    //    timer.pause(); cerr << "partition:figure out nodes " << timer.getReal() << timer.getCPU() << endln;



    // now we go through the load patterns and move NodalLoads and MP_Constraints
    // 1) make sure each subdomain has a copy of the partitioneddomains load patterns.
    // 2) move nodal loads
    // 3) move SP_Constraints
    
    
    // we now add the nodes, by iterating through the nodes and for
    // each node determining if it is an internal or external node
    // to each subdomain
    NodeIter &theNodes2 = myDomain->getNodes();
    while ((nodePtr = theNodes2()) != 0) 
    {
      int nodeTag = nodePtr->getTag();

      //GZ change data structure //if ( nodePtr->getNumberDOF() != 3) continue;//Guanzhou added
      //GZ change data structure 
      //GZ change data structure int partition = (*nodePlace)(nodeTag);
      //GZ change data structure 
      //GZ change data structure for (int j=1; j<=numParts; j++) {
      //GZ change data structure 	
      //GZ change data structure 	int prime = primes(j);
      //GZ change data structure 	// add the nodes
      //GZ change data structure 	
      //GZ change data structure 	
      //GZ change data structure 	//Guanzhou changed!!!
      //GZ change data structure 	if (partition%prime == 0) { // it belongs to partition j
      //GZ change data structure 
      //GZ change data structure 	    //cerr << "Node Tag: " << nodeTag << " belongs to partition: " << j << "\n";//Guanzhou added
      //GZ change data structure 
      //GZ change data structure 
      //GZ change data structure 
      //GZ change data structure 	  // determine if internal or external and add accordingly
      //GZ change data structure 	  int internal = 1; // assume internal
      //GZ change data structure 	  for (int k=1; k<= numParts+1; k++) {  // numParts+1 for MP_Constraints
      //GZ change data structure 	    if (k != j) {
      //GZ change data structure 	      prime = primes(k);
      //GZ change data structure 	      if (partition%prime == 0) { //the node again belongs to another partition k, so it is external
      //GZ change data structure 		internal = 0; // external!!
      //GZ change data structure 		k = numParts+1; //terminate the loop, as the node is external, don't need to do more.
      //GZ change data structure 	      }
      //GZ change data structure 	    }
      //GZ change data structure 	  }

      //GZ change data structure 	  if (internal == 1) {  // its an internal node
      const int count = ParCount[nodeTag];
      
      if (count == 1) 
      {  // its an internal node
      	    // remove the node & add it to the subdomain

	    //cerr << "Node Tag: " << nodeTag << " is internal to partition: " << j << "\n";//Guanzhou

	    //GZ change data structure Subdomain *theSubdomain = myDomain->getSubdomainPtr(j); 
	    const int par = nodePartition[nodeTag*8+count-1];
	    if ( par != 0 ) 
	    {
	    Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
	    nodePtr = myDomain->Domain::removeNode(nodePtr->getTag());
	    theSubdomain->addNode(nodePtr);
	    }
//Guanzhou out	    // now we go through the load patterns and move NodalLoads and SP_Constraints
//Guanzhou out	    // 1) move nodal loads
//Guanzhou out	    // 2) move SP_Constraints
//Guanzhou out	    LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
//Guanzhou out	    LoadPattern *theLoadPattern;
//Guanzhou out	    while ((theLoadPattern = theLoadPatterns()) != 0) {
//Guanzhou out	      int loadPatternTag = theLoadPattern->getTag();
//Guanzhou out	      // now remove any nodal loads that correspond to internal nodes in a subdomain
//Guanzhou out	      // and add them to the appropriate loadpattern in the subdomain
//Guanzhou out	       
//Guanzhou out	       
//Guanzhou out	       //Guanzhou moved code here to add LoadPattern
//Guanzhou out	       for (int i=1; i<=numParts; i++) {
//Guanzhou out			Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);
//Guanzhou out			LoadPattern *loadPatternCopy = theSubdomain->getLoadPattern(loadPatternTag);
//Guanzhou out			if (loadPatternCopy == 0) {
//Guanzhou out	  	             LoadPattern *newLoadPattern = theLoadPattern->getCopy();
//Guanzhou out	  	             if (newLoadPattern == 0) {
//Guanzhou out	  	               cerr << "DomaiPartitioner::partition - out of memory creating LoadPatterns\n";
//Guanzhou out	  	               return -1;
//Guanzhou out	  	             }
//Guanzhou out	  	             theSubdomain->addLoadPattern(newLoadPattern);
//Guanzhou out		        }
//Guanzhou out      	       }
//Guanzhou out    
//Guanzhou out	      NodalLoadIter &theNodalLoads = theLoadPattern->getNodalLoads();
//Guanzhou out	      NodalLoad *theNodalLoad;
//Guanzhou out	      while ((theNodalLoad = theNodalLoads()) != 0) {
//Guanzhou out		int loadedNodeTag = theNodalLoad->getNodeTag();	      
//Guanzhou out		if (loadedNodeTag == nodeTag) {
//Guanzhou out		  theLoadPattern->removeNodalLoad(theNodalLoad->getTag());
//Guanzhou out		  Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);
//Guanzhou out		  if ((theSubdomain->addNodalLoad(theNodalLoad, loadPatternTag)) != true)
//Guanzhou out		    cerr << "DomainPartitioner::partition() - failed to add Nodal Load\n";
//Guanzhou out		  cerr << "DomainPartitioner - sending load " << *theNodalLoad;
//Guanzhou out		}
//Guanzhou out	      }
//Guanzhou out	      
//Guanzhou out	      
//Guanzhou out	      SP_ConstraintIter &theSPs = theLoadPattern->getSPs();
//Guanzhou out	      SP_Constraint *spPtr;
//Guanzhou out	      while ((spPtr = theSPs()) != 0) {
//Guanzhou out		int constrainedNodeTag = spPtr->getNodeTag();
//Guanzhou out		if (constrainedNodeTag == nodeTag) {
//Guanzhou out		  Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);  
//Guanzhou out		  theLoadPattern->removeSP_Constraint(spPtr->getTag());
//Guanzhou out		  theSubdomain->addSP_Constraint(spPtr, loadPatternTag);
//Guanzhou out		}
//Guanzhou out	      }	    
//Guanzhou out	    }
	    //GZ change data structure j = numParts+1;
	  }
	  else 
	  {  // its external, don't remove from mydomain, keep it global
	    //	    cerr << "EXTERNAL: " << *nodePtr;
	    	for (int l=0; l<count; l++) 
		{
	    		const int par = nodePartition[nodeTag*8+l];
			Subdomain *theSubdomain;
			if ( par != 0 )
				theSubdomain = myDomain->getSubdomainPtr(par);
			else
				continue; 
	    		theSubdomain->addExternalNode(nodePtr);
	    //cerr << "Node Tag: " << nodeTag << " is external to partition: " << j << "\n"; //Guanzhou
	  	}
	}
    }
    //GZ change data structure }
    //GZ change data structure 
    //GZ change data structure }

    
    //Guanzhou moved codes here after the Subdomain->addExternalNode, which does numDOF increment
    LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
    LoadPattern *theLoadPattern;
    while ((theLoadPattern = theLoadPatterns()) != 0) 
    {
      int loadPatternTag = theLoadPattern->getTag();
      
      // check that each subdomain has a loadPattern with a similar tag and class tag
      //add LoadPattern to all subdomains
      for (int i=1; i<=numParts; i++) 
      {
	Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);
	LoadPattern *loadPatternCopy = theSubdomain->getLoadPattern(loadPatternTag);
	if (loadPatternCopy == 0) 
	{
	  LoadPattern *newLoadPattern = theLoadPattern->getCopy();
	  if (newLoadPattern == 0) 
	  {
	    cerr << "DomaiPartitioner::partition - out of memory creating LoadPatterns\n";
	    return -1;
	  }

          #ifdef _BABAK_DEBUG
//		cerr << "DomaiPartitioner::partition--BEFORE theSubdomain->addLoadPattern(newLoadPattern) -- DRM INFORMATION:\n";
//		DRM_LOAD_PATTERN->Print(cerr);
		
//		theLoadPattern->Print(cerr);
          #endif
  	  theSubdomain->addLoadPattern(newLoadPattern);
	  
	  #ifdef _BABAK_DEBUG
//		cerr << "DomaiPartitioner::partition--AFTER theSubdomain->addLoadPattern(newLoadPattern) -- DRM INFORMATION:\n";
//		DRM_LOAD_PATTERN->Print(cerr);
          #endif
	}
      }

 	NodalLoadIter &theNodalLoads = theLoadPattern->getNodalLoads();
        NodalLoad *theNodalLoad;
        while ((theNodalLoad = theNodalLoads()) != 0) 
	{
   		int loadedNodeTag = theNodalLoad->getNodeTag();	      
	      	
		//GZ Node *nodePtr = myDomain->Domain::getNode(loadedNodeTag);
		//GZ if ( nodePtr->getNumberDOF() != 3) continue;//Guanzhou added

		//GZ change data structure int partition = (*nodePlace)(loadedNodeTag);
      		//GZ change data structure for (int j=1; j<=numParts; j++) {
		//GZ change data structure 	int prime = primes(j);
		//GZ change data structure 	// add the nodes
		//GZ change data structure 	if (partition%prime == 0) { // it belongs to partition j
		//GZ change data structure 		int internal = 1; // assume internal
	  	//GZ change data structure 		for (int k=1; k<= numParts+1; k++) {  // numParts+1 for MP_Constraints
	    	//GZ change data structure 			if (k != j) {
	      	//GZ change data structure 				prime = primes(k);
	      	//GZ change data structure 				if (partition%prime == 0) { //the node again belongs to another partition k, so it is external
		//GZ change data structure 					internal = 0; // external!!
		//GZ change data structure 					k = numParts+1; //terminate the loop, as the node is external, don't need to do more.
	      	//GZ change data structure 				}
	    	//GZ change data structure 			}
	  	//GZ change data structure 		}
		//GZ change data structure 		if (internal == 1) {


		const int count = ParCount[loadedNodeTag];
      
      		if (count == 1) 
		{  // its an internal node
			const int par = nodePartition[loadedNodeTag*8+count-1];
			if ( par != 0 ) 
			{
				theLoadPattern->removeNodalLoad(theNodalLoad->getTag());
   	  			//GZ change data structure Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);
   	  			Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
				if ((theSubdomain->addNodalLoad(theNodalLoad, loadPatternTag)) != true)
   	    				cerr << "DomainPartitioner::partition() - failed to add Nodal Load\n";
   	  			cerr << "DomainPartitioner - sending load " << *theNodalLoad;
				//GZ change data structure j = numParts+1;
			}
   		}
        		//GZ change data structure }
	}
	//GZ change data structure }
         
        cerr << "DomainPartitioner::partition - finished sending loads!\n";
 
        SP_ConstraintIter &theSPs = theLoadPattern->getSPs();
        SP_Constraint *spPtr;
        while ((spPtr = theSPs()) != 0) 
	{
   		int constrainedNodeTag = spPtr->getNodeTag();
		
		//GZ Node *nodePtr = myDomain->Domain::getNode(constrainedNodeTag);
		//GZ if ( nodePtr->getNumberDOF() != 3) continue;//Guanzhou added
       		
		//GZ change data structure int partition = (*nodePlace)(constrainedNodeTag);
      		//GZ change data structure for (int j=1; j<=numParts; j++) {
		//GZ change data structure 	int prime = primes(j);
		//GZ change data structure 	// add the nodes
		//GZ change data structure 	if (partition%prime == 0) { // it belongs to partition j
		//GZ change data structure 		int internal = 1; // assume internal
	  	//GZ change data structure 		for (int k=1; k<= numParts+1; k++) {  // numParts+1 for MP_Constraints
	    	//GZ change data structure 			if (k != j) {
	      	//GZ change data structure 				prime = primes(k);
	      	//GZ change data structure 				if (partition%prime == 0) { //the node again belongs to another partition k, so it is external
		//GZ change data structure 					internal = 0; // external!!
		//GZ change data structure 					k = numParts+1; //terminate the loop, as the node is external, don't need to do more.
	      	//GZ change data structure 				}
	    	//GZ change data structure 			}
	  	//GZ change data structure 		}
		//GZ change data structure 		if (internal == 1) {
    	  	//GZ change data structure 			Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);  
   	  	const int count = ParCount[constrainedNodeTag];
      
      		if (count == 1) 
		{  // its an internal node
			const int par = nodePartition[constrainedNodeTag*8+count-1];
			if ( par != 0 ) 
			{
				Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);
				theLoadPattern->removeSP_Constraint(spPtr->getTag());
   	  			theSubdomain->addSP_Constraint(spPtr, loadPatternTag);
				//GZ change data structure j = numParts+1;
			}
		}
		//GZ change data structure }
	}
    	
	cerr << "DomainPartitioner::partition - finished sending sps in load pattern!\n";
    
    }

    // we now move the elements and any elemental Loads in the loadPatterns
    VertexIter &theVertices = theElementGraph->getVertices();
    while ((vertexPtr = theVertices()) != 0) 
    {
      	// move the element
      	int partition = vertexPtr->getColor();
	
	#ifdef _BABAK_DEBUG

	      cerr << "DomainPartitioner::partition - Partition No: " << partition << "*******************************************\n";

	#endif
	    
	    
      	int eleTag = vertexPtr->getRef();
      	Element *elePtr = myDomain->removeElement(eleTag); 
	
	#ifdef _BABAK_DEBUG

	      cerr << "DomainPartitioner::partition - Removing Element No: " << eleTag << "from the Domain\n";
	    
	#endif
	
      	Subdomain *theSubdomain = myDomain->getSubdomainPtr(partition);  

      	theSubdomain->addElement(elePtr);
	


      	/*********** ELEMENT LOADS ***************************************/
      	// if any corresponding elemental loads in the load patterns .. move the load as well
      	LoadPatternIter &theLoadPatterns = myDomain->getLoadPatterns();
      	LoadPattern *theLoadPattern;
	
      	while ((theLoadPattern = theLoadPatterns()) != 0) 
	{
		int loadPatternTag = theLoadPattern->getTag();
		ElementalLoadIter &theLoads = theLoadPattern->getElementalLoads();
		ElementalLoad *theLoad;
		while ((theLoad = theLoads()) != 0) 
		{
	  		//cerr << "DomainPartitioner::partition - REMOVE ELEMENTAL LOADS\n";
	    		if ( theLoad->hasElement(eleTag) ) 
			{
	    			theLoadPattern->removeElementalLoad(theLoad->getTag());
	    			//theLoad->removeElement(eleTag);
				theSubdomain->addElementalLoad(theLoad, loadPatternTag);
			}
		}
      	}
      /********************* ELEMENT LOADS ****************************/
    }


    // timer.pause(); cerr << "partition:add elements " << timer.getReal() << timer.getCPU() << endln;
    
    // add the single point constraints, only added if for an internal node in a subdomain

    cerr << "DomainPartitioner::partition - finished sending elements!\n";

    
    //Guanzhou changed below
    SP_ConstraintIter &theSPs = myDomain->getSPs();
    SP_Constraint *spPtr;
    while ((spPtr = theSPs()) != 0) 
    {
      int constrainedNodeTag = spPtr->getNodeTag();
      //cerr << "constrainedNodeTag: " << constrainedNodeTag << endln;
      //GZ Node *nodePtr = myDomain->Domain::getNode(constrainedNodeTag);
      //GZ 
      //GZ if ( nodePtr != 0)
      //GZ 	if ( nodePtr->getNumberDOF() != 3) continue;//Guanzhou added
 
      //GZ change data structure int partition = (*nodePlace)(constrainedNodeTag);
      //GZ change data structure 
      //GZ change data structure for (int i=1; i<=numParts; i++) {
      //GZ change data structure 	int prime = primes(i);
      //GZ change data structure 	if (partition%prime == 0) { // it belongs to current partition i
      //GZ change data structure 	  
      //GZ change data structure 	  // now test if internal, if so add
      //GZ change data structure 	  int internal = 1; //assume internal!!! Guanzhou changed
      //GZ change data structure 	  for (int j=1; j<=numParts+1; j++) {//GZ numParts+1 for MPs
      //GZ change data structure 	    if (i != j) {
      //GZ change data structure 	      prime = primes(j);
      //GZ change data structure 	      if (partition%prime == 0) { // if the node belongs to more than 1 subdomains, it's external
      //GZ change data structure 		internal = 0;
      //GZ change data structure 		j = numParts+1;
      //GZ change data structure 	      }
      //GZ change data structure 	    }
      //GZ change data structure 	  }
      //GZ change data structure 	  Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);	
      //GZ change data structure     if (internal == 1) { //if the node is internal to the current subdomain, add it
      const int count = ParCount[constrainedNodeTag];
      //cerr << "count: " << count << endln;
      	if (count == 1) 
	{  // its an internal node
      	    	const int par = nodePartition[constrainedNodeTag*8+count-1];
	    	//cerr << "Internal! par: " << par << endln;
		if ( par != 0 ) 
		{
      	    	Subdomain *theSubdomain = myDomain->getSubdomainPtr(par);	
	    	myDomain->removeSP_Constraint(spPtr->getTag());
	    	theSubdomain->addSP_Constraint(spPtr);
	    	//GZ change data structure i = numParts+1; //terminate loop because a node can be only internal to ONE subdomain
	    	}
	  	//GZ change data structure theSubdomain->addSP_Constraint(spPtr);
      	} else 
	{
      		for (int l=0; l<count; l++) 
		{
			const int par = nodePartition[constrainedNodeTag*8+l];
			//cerr << "External! par: " << par << endln;
			Subdomain *theSubdomain;
			if ( par != 0 )
				theSubdomain = myDomain->getSubdomainPtr(par);
			else
				continue;
			theSubdomain->addSP_Constraint(spPtr);
      		}
    	}
    }
    
    cerr << "DomainPartitioner::partition - finished sending sps!\n";
    
    //Guanzhou out cerr << "DONE PARTITIONING NODES, ELEMENTS & SP's\n " << *myDomain;
    //Guanzhou out cerr << "DONE PARTITIONING NODES\n";

    
      
    //GZ //add the MP_Constraints, only added if both internal to a subdomain
    //GZ MP_ConstraintIter &moreMPs = myDomain->getMPs();
    //GZ while ((mpPtr = moreMPs()) != 0) {
    //GZ   int retained = mpPtr->getNodeRetained();
    //GZ   int constrained = mpPtr->getNodeConstrained();
    //GZ   int partRet = (*nodePlace)(retained);
    //GZ   int partCon = (*nodePlace)(constrained);
    //GZ   if (partRet == partCon) {
    //GZ 	for (int i=1; i<=numParts; i++) {
    //GZ 	  int prime = primes(i);
    //GZ 	  if (partRet%prime == 0) { // it belongs to partition
    //GZ 	    
    //GZ 	    // now test if internal, if so add
    //GZ 	    int internal = 1;
    //GZ 	    for (int j=1; j<=numParts+1; j++) 
    //GZ 	      if (i != j) {
    //GZ 		prime = primes(j);
    //GZ 		if (partRet%prime == 0) { // its external
    //GZ 		  internal = 0;
    //GZ 		  j = numParts+1;
    //GZ 		}
    //GZ 	      }
    //GZ 	    Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);
    //GZ 	    if (internal == 1) {    
    //GZ 	      myDomain->removeMP_Constraint(mpPtr->getTag());
    //GZ 	      theSubdomain->addMP_Constraint(mpPtr);
    //GZ 	      i = numParts+1;
    //GZ 	    }
    //GZ 	    //theSubdomain->addMP_Constraint(mpPtr);
    //GZ 	  }
    //GZ 	}  
    //GZ   }
    //GZ }
    
    
    // now we go through all the subdomains and tell them to update
    // their analysis for the new layouts
    SubdomainIter &theSubDomains = myDomain->getSubdomains();
    Subdomain *theSubDomain;
    while ((theSubDomain = theSubDomains()) != 0) 
    {
      //theSubDomain->resetRecorders();
      theSubDomain->domainChange();
    }
    cerr << "DomainPartitioner::partition - finished updating subdomains!\n";

    /*
      if (theSubDomain->hasDomainChanged() == true) 
      theSubDomain->invokeChangeOnAnalysis();
    */

    // we invoke change on the PartitionedDomain
    //myDomain->resetRecorders();

    myDomain->domainChange();
    
    /***** it's up to the analysis object to determine if domain has changed
    if (myDomain->hasDomainChanged() == true) 
    myDomain->invokeChangeOnAnalysis();
    ****************************************************************/

    //    timer.pause(); cerr << "partition:domain change " << timer.getReal() << timer.getCPU() << endln;

    // we are done

    //Guanzhou added debug output
    //myDomain->Print(cerr);
      
    //  for (int j=1; j<=numParts; j++) {
    //	Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);
    //  	theSubdomain->Print(cerr);
    //	}
    //
    // 
    cerr << "DomainPartitioner::partition - finished data distribution!\n";
    partitionFlag = true;

    return 0;
}


int
DomainPartitioner::balance(Graph &theWeightedPGraph)
{
    int res = 0;

    // check that the object did the partitioning
    if (partitionFlag == false) {
  cerr << "DomainPartitioner::balance(const Vector &load)";
  cerr << " - not partitioned or DomainPartitioner did not partition\n";
  return -1;
    }

    if (theBalancer != 0) {

	// call on the LoadBalancer to partition		
	res = theBalancer->balance(theWeightedPGraph);
	    
	// now invoke domainChanged on Subdomains and PartitionedDomain
	SubdomainIter &theSubDomains = myDomain->getSubdomains();
	Subdomain *theSubDomain;

	while ((theSubDomain = theSubDomains()) != 0) 
	  theSubDomain->domainChange();
	
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


/*******************************Guanzhou
Graph &
DomainPartitioner::getPartitionGraph(void)
{
    if (myDomain == 0) {
  cerr << "ERROR: DomainPartitioner::getPartitionGraph(void)";
  cerr << " - No domain has been set";
  exit(-1);
    }
    return myDomain->getSubdomainGraph();
}

Graph &
DomainPartitioner::getColoredGraph(void)
{
    if (myDomain == 0) {
  cerr << "ERROR: DomainPartitioner::getPartitionGraph(void)";
  cerr << " - No domain has been set";
  exit(0);
    }
    
    return myDomain->getElementGraph();
}


int 
DomainPartitioner::swapVertex(int from, int to, int vertexTag,
			      bool adjacentVertexNotInOther)
{
  cerr << "DomainPartitioner::swapVertex() " << from << " " << to << " " << vertexTag << endln;

  // check that the object did the partitioning
  if (partitionFlag == false) {
    cerr << "DomainPartitioner::balance(const Vector &load)";
    cerr << " - not partitioned or DomainPartitioner did not partition\n";
    return -1;
  }
  
  // check that the subdomain exist in partitioned domain
  Subdomain *fromSubdomain = myDomain->getSubdomainPtr(from);
  if (fromSubdomain == 0) {
    cerr << "DomainPartitioner::swapVertex - No from Subdomain: ";
    cerr << from << " exists\n";
    return -2;
  }
  Subdomain *toSubdomain = myDomain->getSubdomainPtr(to);
  if (toSubdomain == 0) {
    cerr << "DomainPartitioner::swapVertex - No to Subdomain: ";
    cerr << to << " exists\n";
    return -3;
  }    
  
  // remove the vertex from the boundary if there
  // if not there we have to get a pointer to it from the ele graph.
  
  //    Graph &theEleGraph = myDomain->getElementGraph();    
  Graph *fromBoundary = theBoundaryElements[from-1];
  Graph *toBoundary = theBoundaryElements[to-1];    
  Vertex *vertexPtr;
  
  // get a pointer to the vertex in the element graph
  if (adjacentVertexNotInOther  == false) {
    vertexPtr = fromBoundary->removeVertex(vertexTag,false);    
    if (vertexPtr == 0) 
      vertexPtr = theElementGraph->getVertexPtr(vertexTag);

    if (vertexPtr == 0)  // if still 0 no vertex given by tag exists
      return -4;
  } else { // get a pointer and check vertex not adjacent to another partition
    vertexPtr = fromBoundary->getVertexPtr(vertexTag);    
    if (vertexPtr == 0) 
      vertexPtr = theElementGraph->getVertexPtr(vertexTag);
    if (vertexPtr == 0)  // if still 0 no vertex given by tag exists
      return -4;	
    const ID &adjacent = vertexPtr->getAdjacency();
    bool inTo = false;
    bool inOther = false;
    int adjacentSize = adjacent.Size();
    for (int i=0; i<adjacentSize; i++) {
      Vertex *other = theElementGraph->getVertexPtr(adjacent(i));
      if (other->getColor() == to) 
	inTo = true;
      else if (other->getColor() != from) {
	inOther = true;
	i = adjacentSize;
      }
    }
    if (inTo != true || inOther == true) // we cannot remove the vertex
      return -5;
    // if can remove the vertex from the fromBoundary
    //Vertex *theVertex = 
    fromBoundary->removeVertex(vertexTag,false);
  }
  
  int eleTag = vertexPtr->getRef();
  
  //
  // in the FROM subdomain we:
  //  1. remove the element
  //  2. remove all nodes connected to the element divide color by prime
  //  3. see if have to add nodes back as external
  //  4. remove from the PartitionedDomain if node was external
  //  5. add new elements to the boundary vertices for from
  
  //  1. remove the element from the fromSubdomain
    //          this gives us a pointer to the element as well.

    Element *elePtr = fromSubdomain->removeElement(eleTag);
    if (elePtr == 0) // if ele not there we can just return but ERROR it should be
  return -6;

    //  2. remove all nodes connected to the element divide color by prime    
    //  3. add back if still in subdomain
    //  4. if node external remove from PartitionedDomains external nodes
    int primeFrom = primes(from);
    const ID  &nodes1 = elePtr->getExternalNodes(); // create a copy, need after send ele
    ID  nodes(nodes1);

    int nodesSize = nodes.Size();
    Node **nodesArray = new Node *[nodesSize];
    for (int i=0; i<nodesSize; i++) {
  int nodeTag = nodes(i);

  // remove the node
  Node *nodePtr = fromSubdomain->removeNode(nodeTag);

  (*nodePlace)(nodeTag) /= primeFrom;
  nodesArray[i] = nodePtr;
        // cerr << "DomainPartitioner::swapVertex -NODE " << *nodePtr;
  // add back as external if still needed
  if ((*nodePlace)(nodeTag)%primeFrom == 0)
      fromSubdomain->addExternalNode(nodePtr);

  // if it was external remove from PartitionedDomains external nodes
  int partition = (*nodePlace)(nodeTag);
  for (int j=1; j<=numPartitions; j++) 
      if (j != from) {
    int prime = primes(j);
    if (partition%prime == 0) {
        myDomain->removeExternalNode(nodeTag);
        j = numPartitions+1;
    }
      }
    }

    // 5. add new elements to boundary vertices of from if connected to
    //    vertex we just removed and of color of from and not already in boundary

    const ID &eleAdjacent = vertexPtr->getAdjacency();
    int eleAdjacentSize = eleAdjacent.Size();

    for (int a=0; a<eleAdjacentSize; a++) {
	int otherEleVertexTag = eleAdjacent(a);
	Vertex *other = fromBoundary->getVertexPtr(otherEleVertexTag);
	if (other == 0) {
	    other = theElementGraph->getVertexPtr(otherEleVertexTag);
	    if (other->getColor() == from)
		fromBoundary->addVertex(other,false);
	}
    }


    
    // in the TO subdomain we:
    //  1. remove all nodes connected to the element, may or may not be there
    //     if there don't divide by primeTo. 
    //  2. mark all connecting nodes of elements as belonging to To
    //  3. add the nodes, checking if external or internal
    //  3. add the element
    //  4. change the vertex color to be to and add vertex to boundary
    //     also see if we can reduce the size of boundary
    //     of boundary of to vertices

    //  1. remove all nodes connected to the element, may or may not be there
    //      if there divide by primeTo, do this as may have been added as extnl
    int primeTo = primes(to);
    for (int l=0; l<nodesSize; l++) {    
  int nodeTag = nodesArray[l]->getTag();
  int nodeColor = (*nodePlace)(nodeTag);
  if (nodeColor % primeTo == 0) {
      Node *nodePtr;
      nodePtr = toSubdomain->removeNode(nodes(l));
  }
    }


    for (int m=0; m<nodesSize; m++) {
  Node *nodePtr = nodesArray[m];
  int nodeTag = nodePtr->getTag();
  
  //  2. mark all connecting nodes of elements as belonging to To      
  (*nodePlace)(nodeTag) *= primeTo;

  //  3. add the nodes, checking if external or internal  
  int internal = 0;
  for (int n=1; n<=numPartitions; n++) 
      if (n != to) {
    int prime = primes(n);
    if ((*nodePlace)(nodeTag)%prime == 0) { // its external
        internal = 1;
        n = numPartitions+1;
    }
      }
  
  if (internal == 0)
      toSubdomain->addNode(nodePtr);
  else {
      toSubdomain->addExternalNode(nodePtr);
      myDomain->addNode(nodePtr);
  }
    }

    delete [] nodesArray; // DELETE nodesArray here should not use nodePtrs
                          // after here as node objects deleted if parallel    
    
    //  3. add the element
    
    toSubdomain->addElement(elePtr);
    
    //  4. change the vertex color to be to and add vertex to boundary
    //     also see if we can reduce the size of to's boundary vertices


    vertexPtr->setColor(to);
    toBoundary->addVertex(vertexPtr,false);

    for (int n=0; n<eleAdjacentSize; n++) {
	int otherEleVertexTag = eleAdjacent(n);
	Vertex *other = toBoundary->removeVertex(otherEleVertexTag,false);
	if (other != 0) {
	    const ID &othersAdjacency = other->getAdjacency();
	    int otherSize = othersAdjacency.Size();
	    for (int b=0; b<otherSize; b++) {
		int anotherEleVertexTag = othersAdjacency(b);
		Vertex *otherOther = theElementGraph->getVertexPtr(anotherEleVertexTag);
		if (otherOther->getColor() != to) {
		    toBoundary->addVertex(other,false);
		    b=otherSize;
		}
	    }
	}
    }


    // now remove any SP_Constraints that may have been external to 
    // PartitionedDomain and are now internal to toSubdomain
    SP_ConstraintIter &theSPs = myDomain->getSPs();
    SP_Constraint *spPtr;
    while ((spPtr = theSPs()) != 0) {
  int nodeTag = spPtr->getNodeTag();
  for (int i=0; i<nodesSize; i++) {
      if (nodeTag == nodes(i)) {
    int internal = 0;
    for (int j=1; j<=numPartitions; j++) 
        if (j != to) {
      int prime = primes(j);
      if ((*nodePlace)(nodeTag)%prime == 0) {
          internal = 1;
          j = numPartitions+1;
      }
        }
    if (internal == 0) { // add to toSubdomain if inteernal
        myDomain->removeSP_Constraint(spPtr->getTag());
        toSubdomain->addSP_Constraint(spPtr);
    }
      }
  }
    }

    
    
    // now remove any SP_Constraints that may have been internal to fromSubdomain
    // and are now external to PartitionedDomain or internal to toSubdomain 
    SP_ConstraintIter &theSPs2 = fromSubdomain->getSPs();
    while ((spPtr = theSPs2()) != 0) {
  int nodeTag = spPtr->getNodeTag();
  for (int i=0; i<nodesSize; i++) {
      if (nodeTag == nodes(i)) {
    fromSubdomain->removeSP_Constraint(spPtr->getTag());
    int internal = 0;
    for (int j=1; j<=numPartitions; j++) 
        if (j != to) {
      int prime = primes(j);
      if ((*nodePlace)(nodeTag)%prime == 0) {
          internal = 1;
          j = numPartitions+1;
      }
        }
    if (internal == 0) 
        toSubdomain->addSP_Constraint(spPtr);
    else
        myDomain->addSP_Constraint(spPtr);
      }
  }
    }

    /*
    // now remove any NodalLoads that may have been external to 
    // PartitionedDomain and are now internal to toSubdomain
    NodalLoadIter &theNodalLoads = myDomain->getNodalLoads();
    NodalLoad *loadPtr;
    while ((loadPtr = theNodalLoads()) != 0) {
  int nodeTag = loadPtr->getNodeTag();
  for (int i=0; i<nodesSize; i++) {
      if (nodeTag == nodes(i)) {
    int internal = 0;
    for (int j=1; j<=numPartitions; j++) 
        if (j != to) {
      int prime = primes(j);
      if ((*nodePlace)(nodeTag)%prime == 0) {
          internal = 1;
          j = numPartitions+1;
      }
        }
    if (internal == 0) { // add to toSubdomain if inteernal
        myDomain->removeNodalLoad(loadPtr->getTag());
        toSubdomain->addNodalLoad(loadPtr);
    }
      }
  }
    }


    // now remove any NodalLoads that may have been internal to fromSubdomain
    // and are now external to PartitionedDomain or internal to toSubdomain 

    NodalLoadIter &theNodalLoads2 = myDomain->getNodalLoads();
    while ((loadPtr = theNodalLoads2()) != 0) {
  int nodeTag = loadPtr->getNodeTag();
  for (int i=0; i<nodesSize; i++) {
      if (nodeTag == nodes(i)) {
    fromSubdomain->removeNodalLoad(loadPtr->getTag());
    int internal = 0;
    for (int j=1; j<=numPartitions; j++) 
        if (j != to) {
      int prime = primes(j);
      if ((*nodePlace)(nodeTag)%prime == 0) {
          internal = 1;
          j = numPartitions+1;
      }
        }
    if (internal == 0) 
        toSubdomain->addNodalLoad(loadPtr);
    else
        myDomain->addNodalLoad(loadPtr);
      }  
  }
    }
    */
/*********************************Guanzhou
    return 0;

}



// method to move from from to to, all elements on the interface of 
// from that are adjacent with to.

int 
DomainPartitioner::swapBoundary(int from, int to, bool adjacentVertexNotInOther)
          
{
  cerr << "DomainPartitioner::swapBoundary: from "  << from << " to " << to << endln;
  /***********************************************************************************
    cerr << "DomainPartitioner::swapBoundary from " << from << "  to " << to << endln;
    Timer timer;
    timer.start();

    // check that the object did the partitioning
    if (partitionFlag == false) {
  cerr << "DomainPartitioner::balance(const Vector &load)";
  cerr << " - not partitioned or DomainPartitioner did not partition\n";
  return -1;
    }

    // check that the subdomains exist in partitioned domain
    Subdomain *fromSubdomain = myDomain->getSubdomainPtr(from);
    if (fromSubdomain == 0) {
  cerr << "DomainPartitioner::swapBoundary - No from Subdomain: ";
  cerr << from << " exists\n";
  return -2;
    }

    Subdomain *toSubdomain = myDomain->getSubdomainPtr(to);
    if (toSubdomain == 0) {
  cerr << "DomainPartitioner::swapBoundary - No to Subdomain: ";
  cerr << to << " exists\n";
  return -3;
    }    

    // get the element graph
    //    Graph &theEleGraph = myDomain->getElementGraph();    
    Graph *fromBoundary = theBoundaryElements[from-1];
    Graph *toBoundary = theBoundaryElements[to-1];

    // into a new graph place the vertices that are to be swapped
    Graph *swapVertices = new Graph(fromBoundary->getNumVertex());

    VertexIter &swappableVertices = fromBoundary->getVertices();
    Vertex *vertexPtr;

    while ((vertexPtr = swappableVertices()) != 0) {
	if (adjacentVertexNotInOther == false) {   
	    const ID &adjacency=vertexPtr->getAdjacency();
	    int size = adjacency.Size();
	    for (int i=0; i<size; i++) {
		int otherTag = adjacency(i);
		Vertex *otherVertex = toBoundary->getVertexPtr(otherTag);
		if (otherVertex != 0) {
		    swapVertices->addVertex(vertexPtr,false);
		    i = size;
		}
	    }
	}
	else {
	    const ID &adjacent = vertexPtr->getAdjacency();
	    bool inTo = false;
	    bool inOther = false;
	    int adjacentSize = adjacent.Size();
	    for (int i=0; i<adjacentSize; i++) {
		Vertex *other = theElementGraph->getVertexPtr(adjacent(i));
		if (other->getColor() == to) 
		    inTo = true;
		else if (other->getColor() != from) {
		    inOther = true;
		    i = adjacentSize;
		}
	    }
	    if (inTo == true && inOther == false) { // we remove the vertex
		swapVertices->addVertex(vertexPtr,false);
		vertexPtr->setColor(to);
	    }
	}
    }

    //
    // in the FROM subdomain we:
    //  1. remove the elements corresponding to the vertices
    //  2. remove all nodes connected to the elements divide color by prime
    //  3. see if have to add nodes back as external
    //  4. remove from the PartitionedDomain if node was external
    //  5. add new elements to the boundary vertices for from

    // 1. first remove all the elements from from subdomain
    VertexIter &verticesToSwap = swapVertices->getVertices();

    int numEleToSwap = swapVertices->getNumVertex();
    Element **elementsArray = new Element *[numEleToSwap];

    int count =0; 
    int numCannotRemove =0;
    for (int i=0; i<numEleToSwap; i++) {
      vertexPtr = verticesToSwap();
  if (vertexPtr == 0) 
      return -1;

  int vertexTag = vertexPtr->getTag();
  int eleTag = vertexPtr->getRef();

  // remove the vertex from fromBoundary, set color to to
  // and add to toBoundary
  bool inFrom = true;
  vertexPtr = fromBoundary->removeVertex(vertexTag,false);


  if (inFrom == true) {
      vertexPtr->setColor(to);
      toBoundary->addVertex(vertexPtr,false);
      Element *elePtr = fromSubdomain->removeElement(eleTag);
      if (elePtr == 0) // if ele not there we can just return IT SHOULD BE
    return -4;
      elementsArray[count] = elePtr;
      count++;
  }
  else  // we cannot remove the element
      numCannotRemove++;
    }
    numEleToSwap -= numCannotRemove;

    //  2. remove all nodes connected to the elements divide color by prime    
    //     for each element and add back if still in subdomain
    //  3. add back if still in subdomain
    //  4. if node external remove from PartitionedDomains external nodes

    // first determine which nodes to remove
    int primeFrom = primes(from);
    int primeTo = primes(to);
    int numNodToSwap = 0;
    ID nodesToRemove(0,128);
    for (int j=0; j<numEleToSwap; j++) {
  Element *elePtr = elementsArray[j];
  const ID  &nodes = elePtr->getExternalNodes(); 
  
  int nodesSize = nodes.Size();

  for (int k=0; k<nodesSize; k++) {
      int nodeTag = nodes(k);
      int loc = nodesToRemove.getLocation(nodeTag);
      if (loc < 0) {
    nodesToRemove[numNodToSwap] = nodeTag;
    numNodToSwap++;
      }
      (*nodePlace)(nodeTag) /= primeFrom;    
      (*nodePlace)(nodeTag) *= primeTo;        
  }
    }

    // remove the nodes
    Node **nodesArray = new Node *[numNodToSwap];  
    for (int k=0; k<numNodToSwap; k++) {
  int nodeTag = nodesToRemove(k); 
  Node *nodePtr = fromSubdomain->removeNode(nodeTag);
  nodesArray[k] = nodePtr;

  // add the node back to from if still external
  if ((*nodePlace)(nodeTag)%primeFrom == 0)
      fromSubdomain->addExternalNode(nodePtr);

  // if it was external remove from PartitionedDomains external nodes
  int partition = (*nodePlace)(nodeTag);
  for (int j=1; j<=numPartitions; j++) 
      if (j != from) {
    int prime = primes(j);
    if (partition%prime == 0) {
        myDomain->removeExternalNode(nodeTag);
        j = numPartitions+1;
    }
      }
    }

    // 5. add new vertices to boundary vertices of from if connected to
    //    vertex we just removed and of color of from and not already in boundary

    VertexIter &verticesToSwap2 = swapVertices->getVertices();
    while ((vertexPtr = verticesToSwap2()) != 0) {
  const ID &vertexAdjacent = vertexPtr->getAdjacency();
  int vertexAdjacentSize = vertexAdjacent.Size();

	for (int a=0; a<vertexAdjacentSize; a++) {
	    int otherEleVertexTag = vertexAdjacent(a);
	    Vertex *other = fromBoundary->getVertexPtr(otherEleVertexTag);
	    if (other == 0) {
		other = theElementGraph->getVertexPtr(otherEleVertexTag);
		if (other->getColor() == from)
		    fromBoundary->addVertex(other,false);
	    }
	}	
    }


    // in the TO subdomain we:
    //  1. remove all nodes connected to the element, may or may not be there
    //     if there don't divide by primeTo. 
    //  2. mark all connecting nodes of elements as belonging to To
    //  3. add the nodes, checking if external or internal
    //  3. add the element
    //  4. change the vertex color to be to and add vertex to boundary
    //     also see if we can reduce the size of boundary
    //     of boundary of to vertices

    //  1. remove all nodes connected to the element, may or may not be there
    //      if there divide by primeTo, do this as may have been added as extnl

    for (int l=0; l<numNodToSwap; l++) {    
  int nodeTag = nodesArray[l]->getTag();
  Node *nodePtr;
  nodePtr = toSubdomain->removeNode(nodesToRemove(l));
    }


    for (int m=0; m<numNodToSwap; m++) {
  Node *nodePtr = nodesArray[m];
  int nodeTag = nodePtr->getTag();
  
  //  3. add the nodes, checking if external or internal  
  int internal = 0;
  for (int n=1; n<=numPartitions; n++) 
      if (n != to) {
    int prime = primes(n);
    if ((*nodePlace)(nodeTag)%prime == 0) { // its external
        internal = 1;
        n = numPartitions+1;
    }
      }
  
  if (internal == 0)
      toSubdomain->addNode(nodePtr);
  else {
      toSubdomain->addExternalNode(nodePtr);
      myDomain->addNode(nodePtr);
  }
    }


    delete [] nodesArray; // DELETE nodesArray here should not use nodePtrs
                          // after here as node objects deleted if parallel    
    
    //  3. add the elements
    for (int a=0; a<numEleToSwap; a++)
  toSubdomain->addElement(elementsArray[a]);


    //  4. change the vertex color to be to and add vertex to boundary
    //     also see if we can reduce the size of to's boundary vertices

    VertexIter &verticesToSwap3 = swapVertices->getVertices();
    while ((vertexPtr = verticesToSwap3()) != 0) {
	const ID &vertexAdjacent = vertexPtr->getAdjacency();
	int vertexAdjacentSize = vertexAdjacent.Size();
	for (int n=0; n<vertexAdjacentSize; n++) {
	    int otherEleVertexTag = vertexAdjacent(n);
	    Vertex *other = toBoundary->removeVertex(otherEleVertexTag,false);
	    if (other != 0) {
		const ID &othersAdjacency = other->getAdjacency();
		int otherSize = othersAdjacency.Size();
		for (int b=0; b<otherSize; b++) {
		    int anotherEleVertexTag = othersAdjacency(b);
		    Vertex *otherOther 
			= theElementGraph->getVertexPtr(anotherEleVertexTag);
		    if (otherOther->getColor() != to) {
			toBoundary->addVertex(other,false);
			b=otherSize;
		    }
		}
	    }
	}
    }

    // now remove any SP_Constraints that may have been external to 
    // PartitionedDomain and are now internal to toSubdomain
    SP_ConstraintIter &theSPs = myDomain->getSPs();
    SP_Constraint *spPtr;
    while ((spPtr = theSPs()) != 0) {
  int nodeTag = spPtr->getNodeTag();
  int loc = nodesToRemove.getLocation(nodeTag);
  if (loc >= 0) {
      int internal = 0;
      for (int j=1; j<=numPartitions; j++) 
    if (j != to) {
        int prime = primes(j);
        if ((*nodePlace)(nodeTag)%prime == 0) {
      internal = 1;
      j = numPartitions+1;
        }
    }
      if (internal == 0) { // add to toSubdomain if inteernal
    myDomain->removeSP_Constraint(spPtr->getTag());
    toSubdomain->addSP_Constraint(spPtr);
      }
  }
    }

    
    // now remove any SP_Constraints that may have been internal to fromSubdomain
    // and are now external to PartitionedDomain or internal to toSubdomain 
    SP_ConstraintIter &theSPs2 = fromSubdomain->getSPs();
    while ((spPtr = theSPs2()) != 0) {
  int nodeTag = spPtr->getNodeTag();
  int loc = nodesToRemove.getLocation(nodeTag);
  if (loc >= 0) {
      fromSubdomain->removeSP_Constraint(spPtr->getTag());
      int internal = 0;
      for (int j=1; j<=numPartitions; j++) 
    if (j != to) {
        int prime = primes(j);
        if ((*nodePlace)(nodeTag)%prime == 0) {
      internal = 1;
      j = numPartitions+1;
        }
    }
      if (internal == 0) 
    toSubdomain->addSP_Constraint(spPtr);
      else
    myDomain->addSP_Constraint(spPtr);
  }  
    }

    // now remove any NodalLoads that may have been external to 
    // PartitionedDomain and are now internal to toSubdomain
    NodalLoadIter &theNodalLoads = myDomain->getNodalLoads();
    NodalLoad *loadPtr;
    while ((loadPtr = theNodalLoads()) != 0) {
  int nodeTag = loadPtr->getNodeTag();
  int loc = nodesToRemove.getLocation(nodeTag);
  if (loc >= 0) {

      int internal = 0;
      for (int j=1; j<=numPartitions; j++) 
    if (j != to) {
        int prime = primes(j);
        if ((*nodePlace)(nodeTag)%prime == 0) {
      internal = 1;
      j = numPartitions+1;
        }
    }
      if (internal == 0) { // add to toSubdomain if inteernal
    myDomain->removeNodalLoad(loadPtr->getTag());
    toSubdomain->addNodalLoad(loadPtr);
      }
  }
    }


    // now remove any NodalLoads that may have been internal to fromSubdomain
    // and are now external to PartitionedDomain or internal to toSubdomain 

    NodalLoadIter &theNodalLoads2 = myDomain->getNodalLoads();
    while ((loadPtr = theNodalLoads2()) != 0) {
  int nodeTag = loadPtr->getNodeTag();
  int loc = nodesToRemove.getLocation(nodeTag);
  if (loc >= 0) {
      fromSubdomain->removeNodalLoad(loadPtr->getTag());
      int internal = 0;
      for (int j=1; j<=numPartitions; j++) 
    if (j != to) {
        int prime = primes(j);
        if ((*nodePlace)(nodeTag)%prime == 0) {
      internal = 1;
      j = numPartitions+1;
        }
    }
      if (internal == 0) 
    toSubdomain->addNodalLoad(loadPtr);
      else
    myDomain->addNodalLoad(loadPtr);
  }  
    }


    delete swapVertices;

    timer.pause();
    cerr << "DomainPartitioner::swapBoundary DONE" << timer.getReal() << endln;

  ***************************************************************************/
  /**********************************************Guanzhou
    return 0;
}


int 
DomainPartitioner::releaseVertex(int from, 
				 int vertexTag, 
				 Graph &theWeightedPartitionGraph,
				 bool mustReleaseToLighter,
				 double factorGreater,
				 bool adjacentVertexNotInOther)
{
  // check that the object did the partitioning
  if (partitionFlag == false) {
    cerr << "DomainPartitioner::balance(const Vector &load)";
    cerr << " - not partitioned or DomainPartitioner did not partition\n";
    return -1;
  }
  
  // we first check the vertex is on the fromBoundary
  Subdomain *fromSubdomain = myDomain->getSubdomainPtr(from);
  if (fromSubdomain == 0) {
    cerr << "DomainPartitioner::swapVertex - No from Subdomain: ";
    cerr << from << " exists\n";
    return -1;
  }

  // get the vertex from the boundary vertices of from
  // Graph &theEleGraph = myDomain->getElementGraph();    
  Graph *fromBoundary = theBoundaryElements[from-1];
    
  Vertex *vertexPtr = fromBoundary->getVertexPtr(vertexTag);
  if (vertexPtr == 0) 
    vertexPtr = theElementGraph->getVertexPtr(vertexTag);
  
  if (vertexPtr == 0)  // if still 0 no vertex given by tag exists
    return -3;

  ID attraction(numPartitions+1);
  attraction.Zero();

  // determine the attraction to the other partitions
  const ID &adjacent = vertexPtr->getAdjacency();
  int numAdjacent = adjacent.Size();
  for (int i=0; i<numAdjacent; i++) {
    int otherTag = adjacent(i);
    Vertex *otherVertex = theElementGraph->getVertexPtr(otherTag);
    int otherPartition = otherVertex->getColor();
    if (otherPartition != from)
      attraction(otherPartition) += 1;
  }
  
  // determine the other partition the vertex is most attracted to
  int partition = 1;
  int maxAttraction = attraction(1);
  for (int j=2; j<=numPartitions; j++)
    if (attraction(j) > maxAttraction) {
      partition = j;
      maxAttraction = attraction(j);
    }

  theWeightedPartitionGraph.Print(cerr,4);
  cerr << "HELLO: " << vertexTag << endln;
  
  // swap the vertex
  if (mustReleaseToLighter == false)
    return swapVertex(from, partition, vertexTag, adjacentVertexNotInOther);
  
  else { // check the other partition has a lighter load
    Vertex *fromVertex = theWeightedPartitionGraph.getVertexPtr(from);
    Vertex *toVertex = theWeightedPartitionGraph.getVertexPtr(partition);	    
    
    double fromWeight = fromVertex->getWeight();
    double toWeight  = toVertex->getWeight();
    
    if (fromWeight == toWeight)
      cerr << "DomainPartitioner::releaseVertex - TO CHANGE >= to >\n";

    if (fromWeight >= toWeight) {
      if (toWeight == 0.0) 
	return swapVertex(from,partition,vertexTag,adjacentVertexNotInOther);	    
      if (fromWeight/toWeight > factorGreater)        
	return swapVertex(from,partition,vertexTag,adjacentVertexNotInOther);	    
    }
  }
  
  return 0;
}



int 
DomainPartitioner::releaseBoundary(int from, 
           Graph &theWeightedPartitionGraph,  
           bool mustReleaseToLighter,
           double factorGreater,
           bool adjacentVertexNotInOther)
{
    // check that the object did the partitioning
    if (partitionFlag == false) {
  cerr << "DomainPartitioner::balance(const Vector &load)";
  cerr << " - not partitioned or DomainPartitioner did not partition\n";
  return -1;
    }

    // we first get a pointer to fromSubdomain & the fromBoundary
    Subdomain *fromSubdomain = myDomain->getSubdomainPtr(from);
    if (fromSubdomain == 0) {
  cerr << "DomainPartitioner::swapVertex - No from Subdomain: ";
  cerr << from << " exists\n";
  return -1;
    }

    //    Graph &theEleGraph = myDomain->getElementGraph();    
    Graph *fromBoundary = theBoundaryElements[from-1];

    // into a new graph place the vertices on the fromBoundary
    // we cannot use fromBoundary as this would empty all the nodes
    // as fromBoundary changes in called methods

    Graph *swapVertices = new Graph(fromBoundary->getNumVertex());

    VertexIter &swappableVertices = fromBoundary->getVertices();
    Vertex *vertexPtr;

    while ((vertexPtr = swappableVertices()) != 0) 
  swapVertices->addVertex(vertexPtr,false);


    cerr << "DomainPartitioner::releaseBoundary\n" << *swapVertices << "END releaseBoundary\n";


    // release all the vertices in the swapVertices
    VertexIter &verticesToSwap = swapVertices->getVertices();
    while ((vertexPtr = verticesToSwap()) != 0)
      releaseVertex(from,
		    vertexPtr->getTag(),
		    theWeightedPartitionGraph,
		    mustReleaseToLighter,
		    factorGreater,
		    adjacentVertexNotInOther);
    
    delete swapVertices;    

    return 0;
}
Guanzhou******************************************************/

# ifdef _PDD  //Guanzhou added for repartitioning
int
DomainPartitioner::repartition(int numParts)
{   
    double start_time = MPI_Wtime();
    // first we ensure the partitioned domain has numpart subdomains
    // with tags 1 through numparts
    Subdomain **subdomainPtrs = new Subdomain * [numParts];
    
    for (int i=1; i<=numParts; i++) {
      //Subdomain *subdomainPtr = 
      subdomainPtrs[i-1] = myDomain->getSubdomainPtr(i);
      //Subdomain *theSubdomain = myDomain->getSubdomainPtr(j);
      if (subdomainPtrs[i-1] == 0) {
	cerr << "DomainPartitioner::repartition - No Subdomain: ";
	cerr << i << " exists\n";
	return -1;
      }
      subdomainPtrs[i-1]->repartition(numParts);
    }
    
    // Timer timer;
    // timer.start();
    
    // we get the ele graph from the domain and partition it
    //    Graph &theEleGraph = myDomain->getElementGraph();
    //Guanzhou: we have to keep the original graph theElementGraph = new Graph(myDomain->getElementGraph());
    
//    int theError = thePartitioner.repartition(theElementGraph, numParts); /*Out  by B. Kamrani September 3dr 2010 */
    int theError = thePartitioner.repartition(theElementGraph, numParts); /*Replaced by B. Kamrani September 3dr 2010 */

    if (theError < 0) {
      cerr << "DomainPartitioner::repartition";
      cerr << " - the graph partioner failed to repartition the ";
      cerr << "element graph\n";
      return -10+theError;
    }
  
    bool repartitioned = false;
    
    VertexIter &VerIter = theElementGraph->getVertices();
    Vertex *verPtr;
    while ((verPtr = VerIter()) != 0) 
    {
      	int eleTag = verPtr->getRef();
      	int vertexColor = verPtr->getColor();
	if ( (*elementPlace)(eleTag-1) != vertexColor) 
	{
		repartitioned = true;
		break;
	}
    }
   
    if ( !repartitioned ) return 0;//quick return
    
    //The domain is repartitioned!!!!
    //record the new element place
    
    //cerr << "\n\n Before Repartitioning: \n" ;
    //myDomain->Print(cerr);
    
    VertexIter &VerIter1 = theElementGraph->getVertices();
    Vertex *verPtr1;
    
    if ( elementPlace != 0 ) delete elementPlace;

    elementPlace = new ID(theElementGraph->getNumVertex());
    
    while ((verPtr1 = VerIter1()) != 0) 
    {
      	int eleTag = verPtr1->getRef();
      	int vertexColor = verPtr1->getColor();
	(*elementPlace)(eleTag-1) = vertexColor;
    }
    
    /*Guanzhou
    if (theBoundaryElements != 0)
      delete [] theBoundaryElements;
    
    theBoundaryElements = new Graph * [numParts];
    if (theBoundaryElements == 0) {
      cerr << "DomainPartitioner::repartition(int numParts)";
      cerr << " - ran out of memory\n";
      numPartitions = 0;  
      return -1;
    }
    
    for (int l=0; l<numParts; l++) {
      theBoundaryElements[l] = new Graph(2048); // graphs can grow larger; just an estimate
      
      if (theBoundaryElements[l] == 0) {
	cerr << "DomainPartitioner::repartition(int numParts)";
	cerr << " - ran out of memory\n";
	numPartitions = 0;
	return -1;
      }
    }*/

    //int NumNodes = (*nodePlace).Size()-1;

    //ID *newNodePlace = new ID(NumNodes+1);

    //GZ new data struc ID *newNodePlace = new ID(maxNodeTag+1);
    //GZ new data struc if (newNodePlace == 0 || newNodePlace->Size() < maxNodeTag+1) {
    //GZ new data struc   cerr << "DomainPartitioner::repartition(int numParts)";
    //GZ new data struc   cerr << " - ran out of memory\n";
    //GZ new data struc   numPartitions = 0;
    //GZ new data struc   return -1;
    //GZ new data struc }
    
    newNodePartition = new int[(maxNodeTag+1)*8];
    newParCount = new int[maxNodeTag+1];
    
    if (newNodePartition == 0 || newParCount == 0) 
    {
      cerr << "DomainPartitioner::partition(int numParts)";
      cerr << " - ran out of memory\n";
      numPartitions = 0;
      return -1;
    }
    
    //GZ new data struc for (int m=minNodeTag; m<=maxNodeTag; m++)
    //GZ new data struc   (*newNodePlace)(m) = 1;
    
    for (int m=minNodeTag; m<=maxNodeTag; m++) 
    {
      newParCount[m] = 0;
      for (int i=0; i<8; i++) 
      {
      	//nodePartition[m][i]=0;
	newNodePartition[m*8+i]=0;
      }
    }
    
    char *theCount = new char [2];
    sprintf(theCount, "%d", Repartition_Count);
    
    char *filename = new char [10];
    strcpy (filename, "repart"); 
    strcat(filename, theCount);

    //ofstream partfile("repart.dat", ios::out);
    ofstream partfile(filename, ios::out);
    const int eleNum = theElementGraph->getNumVertex();
    int *where = new int[eleNum];

    VertexIter &theVertexIter = theElementGraph->getVertices();
    Vertex *vertexPtr;
    while ((vertexPtr = theVertexIter()) != 0) 
    {
      int eleTag = vertexPtr->getRef();
      int vertexColor = vertexPtr->getColor();
      
      //Guanzhou added here to export to file for PMVIS
      *(where+eleTag-1) = vertexColor;
            
      //GZ const ID &adjacency = vertexPtr->getAdjacency();
      //GZ int size = adjacency.Size();
      //GZ for (int i=0; i<size; i++) {
      //GZ 	Vertex *otherVertex = theElementGraph->getVertexPtr(adjacency(i));
      //GZ 	if (otherVertex->getColor() != vertexColor) {
      //GZ 	  theBoundaryElements[vertexColor-1]->addVertex(vertexPtr,false);//theBoundaryElements[vertex-1] records boundary ele of partition[vertexColor]
      //GZ 	  i = size;
      //GZ 	}
      //GZ }
      
      //added by babak at 2/25/13:
      //---
      int numNodesPerEle = myDomain->return_element_numberofNodes(eleTag);
      //---
      
      
      for (int j=0; j<numNodesPerEle; j++) 
      {
	int node = ele_nodes[(eleTag-1)*numNodesPerEle+j];
       	//GZ new data struc if ((*newNodePlace)(node) % primeVertexColor != 0)
	//GZ new data struc   (*newNodePlace)(node) *= primeVertexColor;
	//GZ new data struc   // (*nodePlace)(nodeTag) % primes(i) == 0 means node(nodeTag) belongs to partition[i]
      	int count = newParCount[node];    
	bool added = false;
	for (int k=0; k<count; k++) 
	{
		if ( newNodePartition[node*8+k] == vertexColor) added = true;
	}
	if ( !added ) 
	{
		newNodePartition[node*8+count] = vertexColor;
		newParCount[node]++;
	} 
      }
    }

    //ChangedNodeList = new ID(0, maxNodeTag);
    numChangedNodes = 0;
    if ( ChangedNodeList != 0 ) delete ChangedNodeList;
    if ( data != 0 ) delete [] data;
    
    data = new int [maxNodeTag+1];

    for (int m=minNodeTag; m<=maxNodeTag; m++) 
    {
	if ( newParCount[m] != ParCount[m] ) 
	{
		data[numChangedNodes] = m;
		numChangedNodes++;
		continue;
	}
	for (int i=0;i<newParCount[m];i++) 
	{
		bool changed = true;
		for (int j=0; j<ParCount[m]; j++) 
		{
			if ( newNodePartition[m*8+i] == nodePartition[m*8+j]) 
			{
				changed = false;
				break;
			}
		}
		if ( changed ) {
		//(*ChangedNodeList)(numChangedNodes) = m;
			data[numChangedNodes] = m;
			numChangedNodes++;
			break;
		}
	}
    }

    //if ( ChangedNodeList != 0 ) delete ChangedNodeList;
    cerr << "DomainPartitioner::repartition, numChangedNodes " << numChangedNodes << endln;
    ChangedNodeList = new ID(data, numChangedNodes, false);
    //ChangeNodeList->setData(data, numChangedNodes, true);

    cerr << "DomainPartitioner::repartition, finished creating ChangedNodeList: " << numChangedNodes << endln;

    for (int i=1 ; i<=numParts; i++) {
    	subdomainPtrs[i-1]->recvChangedNodes(*ChangedNodeList, numChangedNodes);
    }

    cerr << "DomainPartitioner::repartition, finished sending ChangedNodeList: " << numChangedNodes << endln;

    //Guanzhou added
    for (int i=1; i<=eleNum; i++)
    	partfile << *(where+i-1) << '\n';
    
    delete [] where;
    partfile.close();

    delete [] filename;
    delete [] theCount;
    Repartition_Count++;

    //cerr << "DomainPartitioner::repartition() - ADD if ((*nodePlace)(node) % primeVertexColor != 0)\n";
    
    //for (int nodeTag = minNodeTag; nodeTag<= maxNodeTag; nodeTag++) {
    
    //GZ cerr << "The changed-node list:...\n";
    //GZ 
    //GZ for (int i = 0; i<numChangedNodes; i++) {
    //GZ 	int nodeTag = (*ChangedNodeList)(i);
    //GZ 	cerr << nodeTag << " Old: " << (*nodePlace)(nodeTag) << " New: " << (*newNodePlace)(nodeTag) << "\n";
    //GZ }
    //GZ 
    cerr << "DomainPartitioner::repartition, starting data redistribution!!!\n";
    
    for (int i = 0; i<numChangedNodes; i++) 
    {
    
	//check for quick return
	//if ( (*nodePlace)(nodeTag) == (*newNodePlace)(nodeTag) ) continue;	
	
 	int nodeTag = (*ChangedNodeList)(i);
	
	//GZ new data struc int partition = (*newNodePlace)(nodeTag);
	//GZ new data struc 
	//GZ new data struc int *others = new int [numParts-1];
	//GZ new data struc 
	//GZ new data struc int NumOfOthers = 0;
      	//GZ new data struc 
      	int dof;
	//GZ new data struc 
	//GZ new data struc for (int j=1; j<=numParts; j++) {
	//GZ new data struc 
	//GZ new data struc 	int prime = primes(j);
	//GZ new data struc 	// add the nodes
	//GZ new data struc 	if (partition%prime == 0) { // it belongs to partition j
	//GZ new data struc 
	//GZ new data struc   		// determine if internal or external and add accordingly
	//GZ new data struc   	        int internal = 1; // assume internal
	//GZ new data struc   	        for (int k=1; k<= numParts; k++) {  // numParts+1 for MP_Constraints
	//GZ new data struc     	        	if (k != j) {
	//GZ new data struc       	        		prime = primes(k);
	//GZ new data struc       	        		if (partition%prime == 0) { //the node again belongs to another partition k, so it is external
	//GZ new data struc 	        			internal = 0; // external!!
	//GZ new data struc 	        			//k = numParts+1; //terminate the loop, as the node is external, don't need to do more.
	//GZ new data struc 	        			others[NumOfOthers] = k;
	//GZ new data struc 	        			NumOfOthers++;
	//GZ new data struc       	        		}
	//GZ new data struc     	        	}
	//GZ new data struc   	        }
      	const int count = newParCount[nodeTag];
	        
	if (count == 1) 
	{  // its an internal node
		const int par = newNodePartition[nodeTag*8+count-1];
		if (myDomain->Domain::getNode(nodeTag) != 0) {//external previously, must search others to remove the node
			Node *nodePtr = myDomain->Domain::removeNode(nodeTag);
			dof = nodePtr->getNumberDOF();
			for (int k=1; k<=numParts; k++) 
			{
				if ( k!=par ) 
				{
					if (subdomainPtrs[k-1]->hasExternalNode(nodeTag)) 
					{
						subdomainPtrs[k-1]->removeNode(nodeTag);
					}
				} else 
				{//external to j, we do swap here!
					if (subdomainPtrs[par-1]->hasExternalNode(nodeTag)) //an external node in j previously
						subdomainPtrs[par-1]->swapNodeFromExternalToInternal(nodeTag, dof);
						//break;
					else //j does not have the node previously
						subdomainPtrs[par-1]->addNode(nodePtr);
				}
			}
		} else {//previously internal to ONE other subdomain, not j
			for (int k=1; k<=numParts; k++) 
			{
				if ( k != par ) 
				{
					if ( (subdomainPtrs[k-1]->hasInternalNode(nodeTag)) ) 
					{
		      				subdomainPtrs[par-1]->ChangeMPIChannel(k);
						subdomainPtrs[k-1]->exportInternalNode(nodeTag, par, dof);
				        	//int *numDOF = subdomainPtrs[j-1]->getPtrToNumDOF();
						//int *numNodes = subdomainPtrs[j-1]->getPtrToNumNodes();
						//ID *theNodes = subdomainPtrs[j-1]->getPtrToNodes();
						//(*theNodes)[*numNodes] = nodeTag;
						//*numDOF = *numDOF + dof;
						//*numNodes++;
						subdomainPtrs[par-1]->addNodeTag(nodeTag, dof);
						//subdomainPtrs[j-1]->restoreChannel();
						break;
					}
				}
			}
		}
		//break;
	} else {  // its external, don't remove from mydomain, keep it global
		//cerr << "EXTERNAL: " << *nodePtr;
		Node *nodePtr = myDomain->Domain::getNode(nodeTag);
		if ( nodePtr != 0) { //previously external, but now external to different subdomains
			for (int l = 0; l<count; l++) 
			{//search all subdomains, if they are in the "others" list, add node, else remove node!
				const int par = newNodePartition[nodeTag*8+l];
				if ( !subdomainPtrs[par-1]->hasExternalNode(nodeTag) ) subdomainPtrs[par-1]->addExternalNode(nodePtr);
			}
			
			for (int k=0; k<ParCount[nodeTag]; k++) 
			{
				const int par = nodePartition[nodeTag*8+k];
				bool OneOfOthers = false;
				for (int n=0; n<count; n++) {
					if ( newNodePartition[nodeTag*8+n] == par ) 
					{
						OneOfOthers = true;
						break;
					}
				}
				if ( OneOfOthers ) {
					if ( !subdomainPtrs[par-1]->hasExternalNode(nodeTag) ) subdomainPtrs[par-1]->addExternalNode(nodePtr);
				} else {
					if ( subdomainPtrs[par-1]->hasExternalNode(nodeTag) ) subdomainPtrs[par-1]->removeNode(nodeTag);
				}
			}
		 	
		} else { //previously internal to ONE specific subdomain (can be j!!!), search for that subdomain and swap that to master and all of the subdomains in the others list!
			int pos = nodePartition[nodeTag*8];
			bool need = false;
			for (int k=0; k<count; k++) {
				const int par = newNodePartition[nodeTag*8+k];
				if ( par == pos ) {
					need = true;
					break;
				}
			}
			
			if ( !need ) subdomainPtrs[pos-1]->exportInternalNode(nodeTag, 0, dof);//0 is the master process!
			else subdomainPtrs[pos-1]->swapNodeFromInternalToExternal(nodeTag);//do swap (in subdomain, will send the node to master)
			
			Node *nodePtr = myDomain->Domain::getNode(nodeTag);
			if ( nodePtr == 0 ) {
				cerr << "DomainPartitioner::repatition, error when retrieve node in master!\n";
				return -1;
			}
			
			for (int k=0; k<count; k++) 
			{
				const int par = newNodePartition[nodeTag*8+k];
				if ( par != pos ) 
				{
				//if ( !subdomainPtrs[j-1]->hasExternalNode(nodeTag) ) subdomainPtrs[j-1]->addExternalNode(nodePtr);
			//for (int k=0; k<NumOfOthers; k++) {
					subdomainPtrs[par-1]->addExternalNode(nodePtr);
				}
			}
		}
		//break;
	}
	//break;
	//
	//	}
      	//}
	//delete [] others;
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
		//if ( (*nodePlace)(loadedNodeTag) == (*newNodePlace)(loadedNodeTag) ) continue;
		bool changed = false;
		if ( newParCount[loadedNodeTag] != ParCount[loadedNodeTag] ) changed = true;
		else {
			for (int i=0;i<newParCount[loadedNodeTag];i++) 
			{
				bool moved = true;
				for (int j=0; j<ParCount[loadedNodeTag]; j++) 
				{
					if ( newNodePartition[loadedNodeTag*8+i] == nodePartition[loadedNodeTag*8+j]) 
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
		if ( !changed ) continue;
		if ( myDomain->Domain::getNode(loadedNodeTag) == 0) 
		{
			theLoadPattern->removeNodalLoad(theNodalLoad->getTag());
			for (int k=1; k<=numParts; k++) {
				if ( subdomainPtrs[k-1]->hasInternalNode(loadedNodeTag) ) 
				{
   	  		   		if ((subdomainPtrs[k-1]->addNodalLoad(theNodalLoad, loadPatternTag)) != true) 
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
		//if ( (*nodePlace)(constrainedNodeTag) == (*newNodePlace)(constrainedNodeTag) ) continue;
		
		bool changed = false;
		if ( newParCount[constrainedNodeTag] != ParCount[constrainedNodeTag] ) changed = true;
		else {
			for (int i=0;i<newParCount[constrainedNodeTag];i++) 
			{
				bool moved = true;
				for (int j=0; j<ParCount[constrainedNodeTag]; j++) 
				{
					if ( newNodePartition[constrainedNodeTag*8+i] == nodePartition[constrainedNodeTag*8+j]) 
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
		if ( !changed ) continue;
		
		
		if ( myDomain->Domain::getNode(constrainedNodeTag) == 0) {
			theLoadPattern->removeSP_Constraint(spPtr->getTag());
			for (int k=1; k<=numParts; k++) {
				if ( subdomainPtrs[k-1]->hasInternalNode(constrainedNodeTag) ) {
   	  		   		if ((subdomainPtrs[k-1]->addSP_Constraint(spPtr, loadPatternTag)) != true) {
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
    while ((spPtr1 = theSP1s()) != 0) {
      	const int constrainedNodeTag = spPtr1->getNodeTag();//all these nodes are external previously
	
	//if ( (*nodePlace)(constrainedNodeTag) == (*newNodePlace)(constrainedNodeTag) ) continue;
	
	bool changed = false;
	if ( newParCount[constrainedNodeTag] != ParCount[constrainedNodeTag] ) changed = true;
	else {
		for (int i=0;i<newParCount[constrainedNodeTag];i++) {
			bool moved = true;
			for (int j=0; j<ParCount[constrainedNodeTag]; j++) {
				if ( newNodePartition[constrainedNodeTag*8+i] == nodePartition[constrainedNodeTag*8+j]) {
					moved = false;
					break;
				}
			}
			if ( moved ) { 
				changed = true;
				break;
			}
		}
	}
	if ( !changed ) continue;
	
	if ( myDomain->Domain::getNode(constrainedNodeTag) == 0 ) {//this node must be internal to ONE specific subdomain now, but previously it was external to SOME subdomains
		myDomain->Domain::removeSP_Constraint(spPtr1->getTag());
		for (int i=1; i<=numParts; i++) {
			if ( subdomainPtrs[i-1]->hasInternalNode(constrainedNodeTag) )
				subdomainPtrs[i-1]->addSP_Constraint(spPtr1);
		}//subdomains will do redistribute data to delete those SPs without an associated node in the subdomain
	} else {// now the node is external to some other subdomains
		for (int i=1; i<=numParts; i++) {
			if ( subdomainPtrs[i-1]->hasExternalNode(constrainedNodeTag) )
				subdomainPtrs[i-1]->addSP_Constraint(spPtr1); //might already existed, have not figured out better way yet! :((
		}
	}
    }

    for (int pe = 1; pe <= numParts; pe++) {
	for (int i = 1; i<= numParts; i++) {
		if ( i != pe ) subdomainPtrs[i-1]->ChangeMPIChannel(pe);
	}
	subdomainPtrs[pe-1]->reDistributeData(numParts);
    }

    for (int pe = 1; pe <= numParts; pe++) {
    	//subdomainPtrs[pe-1]->resetRecorders();
    }
    
    //myDomain->resetRecorders();

    //for (int pe = 1; pe <= numParts; pe++) {
    //	subdomainPtrs[pe-1]->restoreChannel();
    //}
	
/*	LoadPatternIter &LPs = currentSub->getLoadPatterns();
    	LoadPattern *LP;
    	while ((LP = LPs()) != 0) {
	
		int loadPatternTag = LP->getTag();
	
	        NodalLoadIter &theNodalLoads = LP->getNodalLoads();
	        NodalLoad *theNodalLoad;
	        while ((theNodalLoad = theNodalLoads()) != 0) {
	        	const int loadedNodeTag = theNodalLoad->getNodeTag();
	        	if ( (*nodePlace(loadedNodeTag)) ) == (*newNodePlace)(loadedNodeTag)) continue;
			if ( currentSub->hasNode(loadedNodeTag) ) {
	        		if ( !currentSub->hasInternalNode(loadedNodeTag) ) { //external
					LP->removeNodalLoad(theNodalLoad->getTag());
   	          		   	if ((myDomain->addNodalLoad(theNodalLoad, loadPatternTag)) != true) {
   	            					cerr << "DomainPartitioner::repartition, failed to add back Nodal Load to loadPattern in myDomain!\n";
	        					return -1;
	        			}
	        		//} else {//internal
				//	break;	
				}
	        	} else {//not in this subdomain anymore, must be internal to one other subdomain, or external to some other subdomains
				LP->removeNodalLoad(theNodalLoad->getTag());
				//bool exported = false;
				for (int i = 1; i<=numParts; i++) {
					const Subdomain * subPtr = subdomainPtrs[i-1];
					if ( i != pe ) {
						if ( subPtr->hasNode(loadedNodeTag) ) {
							if ( subPtr->hasInternalNode(loadedNodeTag) ) {
								if ( !subPtr->addNodalLoad(theNodalLoad, loadPatternTag) ) {
									cerr << "DomainPartitioner::repartition, failed to add internal nodal load to loadPattern in subdomain!\n";
									return -1;
								}
								break;
							} else {//external
								//if ( !exported ) {
								if ((myDomain->addNodalLoad(theNodalLoad, loadPatternTag)) != true) {
   	            							cerr << "DomainPartitioner::repartition, failed to add back Nodal Load to loadPattern in myDomain!\n";
	        							return -1;
	        						}
								break;	//exported = true;
							}
						}
					}
				}
			}
	        }
	        
                SP_ConstraintIter &theSPs = LP->getSPs();
                SP_Constraint *spPtr;
                while ((spPtr = theSPs()) != 0) {
   	        	const int constrainedNodeTag = spPtr->getNodeTag();
	        	if ( (*nodePlace(constrainedNodeTag)) ) == (*newNodePlace)(constrainedNodeTag)) continue;
			if ( currentSub->hasNode(constrainedNodeTag) ) {
	        		if ( !currentSub->hasInternalNode(constrainedNodeTag) ) { //external
					LP->removeSP_Constraint(spPtr->getTag());
   	          		   	if ((myDomain->addSP_Constraint(spPtr, loadPatternTag)) != true) {
   	            					cerr << "DomainPartitioner::repartition, failed to add back SP to loadPattern in myDomain!\n";
	        					return -1;
	        			}
	        		//} else {//internal
				//	break;	
				}
	        	} else {//not in this subdomain anymore, must be internal to one other subdomain, or external to some other subdomains
				LP->removeSP_Constraint(spPtr->getTag());
				//bool exported = false;
				for (int i = 1; i<=numParts; i++) {
					const Subdomain * subPtr = subdomainPtrs[i-1];
					if ( i != pe ) {
						if ( subPtr->hasNode(constrainedNodeTag) ) {
							if ( subPtr->hasInternalNode(constrainedNodeTag) ) {
								if ( !subPtr->addSP_Constraint(spPtr, loadPatternTag) ) {
									cerr << "DomainPartitioner::repartition, failed to add SP to loadPattern in subdomain!\n";
									return -1;
								}
								break;
							} else {//external
								//if ( !exported ) {
								if ((myDomain->addSP_Constraint(spPtr, loadPatternTag)) != true) {
   	            							cerr << "DomainPartitioner::repartition, failed to add SP back to loadPattern in myDomain!\n";
	        							return -1;
	        						}
								break;	//exported = true;
							}
						}
					}
				}
			}
	        }
    	}

    	
    }


    VertexIter &theVertices = theElementGraph->getVertices();
    while ((vertexPtr = theVertices()) != 0) {
      	// move the element
      	int partition = vertexPtr->getColor();
	int eleTag = vertexPtr->getRef();
      	if ( partition == (*elementPlace)(eleTag-1) ) continue; //quick return
      	Element *elePtr = subdomainPtrs[(*elementPlace)(eleTag-1)-1]->removeElement(eleTag);  
      	//Subdomain *theSubdomain = myDomain->getSubdomainPtr(partition);  
      	if ( elePtr == 0 ) {
		cerr << "DomainPartitioner::repartition, error when remove element from subdomain!\n";
		return -1;
	}
	if ( !subdomainPtrs[partition-1]->addElement(elePtr) ) {
		cerr << "failed to add element to subdomain!\n";
		return -1;
	}
    }
    
    
    for (int pe = 1; pe <= numParts; pe++) {
    	const Subdomain * subPtr = subdomainPtrs[pe-1];

 	SP_ConstraintIter &SPs = subPtr->getSPs();
    	SP_Constraint *sp;
    	while ((sp = SPs()) != 0) {
     		const int constrainedNodeTag = sp->getNodeTag();
		if ( subPtr->hasNode(constrainedNodeTag) ) {
			if ( !subPtr->hasInternalNode(constrainedNodeTag) ) { //external
				myDomain->addSP_Constraint(spPtr);
				
			}
		}
		for (int i = 1; i<= numParts; i++) {
			
		}

    }

      int partition = (*nodePlace)(spPtr->getNodeTag());
      for (int i=1; i<=numParts; i++) {
	int prime = primes(i);
	if (partition%prime == 0) { // it belongs to current partition i
	  
	  // now test if internal, if so add
	  int internal = 1; //assume internal!!! Guanzhou changed
	  for (int j=1; j<=numParts; j++) { 
	    if (i != j) {
	      prime = primes(j);
	      if (partition%prime == 0) { // if the node belongs to more than 1 subdomains, it's external
		internal = 0;
		j = numParts+1;
	      }
	    }
	  }
	  Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);	
	  if (internal == 1) { //if the node is internal to the current subdomain, add it
	    //Subdomain *theSubdomain = myDomain->getSubdomainPtr(i);	
	    myDomain->removeSP_Constraint(spPtr->getTag());
	    //theSubdomain->addSP_Constraint(spPtr);
	    i = numParts+1; //terminate loop because a node can be only internal to ONE subdomain
	  }
	  theSubdomain->addSP_Constraint(spPtr);
	}
      }
    }
*********************************************************/

    cerr << "DomainPartitioner::repartition, finished data redistribution!!!\n";
    
    for (int i=1 ; i<=numParts; i++) {
    	subdomainPtrs[i-1]->domainChange();
    }
    
    myDomain->domainChange();

    //cerr << "\n\n After Repartitioning: \n" ;
    //myDomain->Print(cerr);
    
    partitionFlag = true;
    
    //*nodePlace = *newNodePlace;

    if ( nodePartition != NULL ) delete [] nodePartition;
    nodePartition = newNodePartition;
    newNodePartition = NULL;
    if ( ParCount != NULL ) delete [] ParCount;
    ParCount = newParCount;
    newParCount = NULL;
    //if ( ChangedNodeList != 0 ) delete [] ChangedNodeList;
    
    double end_time = MPI_Wtime();
  
// Out by B. Kamrani July 2011    timingfile << "Timing used by repartition and data migration: " << end_time-start_time << endln;

    return 0;
}

# endif
















