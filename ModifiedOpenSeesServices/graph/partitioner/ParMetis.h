///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Guanzhou Jie & Boris Jeremic
// DATE:               2004
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



#ifndef ParMetis_h
#define ParMetis_h

#include <GraphPartitioner.h>
#include <GraphNumberer.h>
# include <mpi.h>

#ifndef _bool_h
#include <bool.h>
#endif

class ParMetis : public GraphPartitioner
//, public GraphNumberer
{
public:
    ParMetis(int numParts = 1);
    /*ParMetis(int Ptype,
      int Mtype,
      int coarsenTo,
      int Rtype,
      int IPtype,
      int numParts =1);*/
    ~ParMetis();

    /*bool setOptions(int Ptype,
            int Mtype,
            int coarsenTo,
            int Rtype,
            int IPtype);

    bool setDefaultOptions(void);*/

    int partition(Graph* theGraph, int numPart);
    int repartition(Graph* theGraph, int numPart);
    // the follwing methods are if the object is to be used as a numberer
    //const ID &number(Graph &theGraph, int lastVertex = -1);
    //const ID &number(Graph &theGraph, const ID &lastVertices);
    //int Setup();

    int sendSelf(int commitTag,
                 Channel& theChannel);
    int receiveSelf(int commitTag, Channel& theChannel,
                    FEM_ObjectBroker& theBroker);


protected:

private:
    int ncon; //number of constraints on vertices (vertex weight)
    //
    int nobj; //number of objectives (edge weight)
    //
    int options[4]; //options for ParMetis routine
    //
    int wgtflag; //0, no weights
    //       //1, weights on edges only
    //       //2, weights on vertices only
    //       //3, weights on both
    //
    int numflag; //0 or 1 starts

    float* tpwgts; //vertex weight that should be distributed to each subdomain
    //useful for heterogeneous environment

    float* ubvec; //unbalance tolerance for each constraint

    //int *vtxdist; //how the vertices are distributed among processors

    int* globalXadj, *globalAdjncy; //local adjacency structure of graph

    //int *part; //results

    //int *vsize; //vertex size, should be put into vertex class

    //int *vwgt; //vertex weight

    //int edgecut;

    float itr;

    int gnvtxs;//, nvtxs;

    //int *eleList, *vertexList;

    //MPI_Comm comm, worker;
};

#endif

