/* ****************************************************************** **
**    OpenSees - Open System for 1Earthquake Engineering Simulation    **
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
** Guanzhou Jie (gjie@ucdavis.edu)                                    **
** Boris Jeremic (jeremic@ucdavis.edu)                    **
** ****************************************************************** */


// Written: Guanzhou Jie

#include <ParMetis.h>
#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
# include <cstdlib>
# include <mpi.h>
/* stuff needed to get the program working on the clump & NOW machines*/
#include <bool.h>
#include <iostream>
using namespace std;


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

extern "C"
void ParMETIS_V3_PartKway(
    int* vtxdist, int* xadj, int* adjncy, int* vwgt,
    int* adjwgt, int* wgtflag, int* numflag, int* ncon, int* nparts,
    float* tpwgts, float* ubvec, int* options, int* edgecut, int* part,
    MPI_Comm* comm);

extern "C"
void ParMETIS_V3_AdaptiveRepart(
    int* vtxdist, int* xadj, int* adjncy, int* vwgt,
    int* vsize, int* adjwgt, int* wgtflag, int* numflag, int* ncon,
    int* nparts, float* tpwgts, float* ubvec, float* ipc2redist,
    int* options, int* edgecut, int* part, MPI_Comm* comm);

extern "C"
int METIS_PartGraphKway(int* numVertex_METIS, int* ncon_METIS, int* xadj_METIS, int* adjncy_METIS,
                        int* vwgts_METIS, int* vsize_METIS, int* adjwgt_METIS, int* numPart, float* tpwgts_METIS,
                        float* ubvec_METIS, int* options_METIS, int* edgecut_METIS, int* partition_METIS);
#include <iostream>
using std::ios;
#include <fstream>
using std::ofstream;

ParMetis::ParMetis(int numParts)
//:GraphNumberer(GraphNUMBERER_TAG_ParMetis)
{
    ncon = 1;
    nobj = 0;
    //options = 0;
    wgtflag = 2;
    numflag = 0;
    tpwgts = 0;
    ubvec = 0;
    //vtxdist = 0;
    globalXadj = 0;
    globalAdjncy = 0;
    //part = 0;
    //vsize = 0;
    //vwgt = 0;
    //edgecut = 0;
    gnvtxs = 0;
    //nvtxs = 0;
    itr = 0.0;
    //eleList = 0;
    //vertexList = 0;
}

/*ParMetis::ParMetis(int Ptype,
      int Mtype,
      int coarsenTo,
      int Rtype,
          int IPtype,
      int numParts)
:GraphNumberer(GraphNUMBERER_TAG_ParMetis),
 myPtype(Ptype), myMtype(Mtype), myCoarsenTo(coarsenTo), myRtype(Rtype),
 myIPtype(IPtype), defaultOptions(false), numPartitions(numParts), theRefResult(0)
{
    // check the options are valid
    checkOptions();
}*/


ParMetis::~ParMetis()
{
    //if ( options != 0 ) delete [] options;
    if ( tpwgts != 0 )
    {
        delete [] tpwgts;
    }

    if ( ubvec != 0 )
    {
        delete [] ubvec;
    }

    //if ( vtxdist != 0 ) delete [] vtxdist;
    if ( globalXadj != 0 )
    {
        delete [] globalXadj;
    }

    if ( globalAdjncy != 0 )
    {
        delete [] globalAdjncy;
    }

    //if ( part != 0 ) delete [] part;
    //if ( vsize != 0 ) delete [] vsize;
    //if ( vwgt != 0 ) delete [] vwgt;
    //if ( eleList != 0 ) delete [] eleList;
    //if ( vertexList != 0 ) delete [] vertexList;
}

//ParMetis::set

// int partition(Graph &theGraph, int numPart)
//  Method to partition the graph. It first creates the arrays needed
//  by the ParMetis lib and then invokes a function from the ParMetis lib to
//  partition the graph. The colors of the vertices of the graph are
//  set to colors 0 through numPart-1 to indicate which partition the
//  vrtices are in. Returns -1 if options are not set, -2 if ParMetis failed.

int
ParMetis::partition(Graph* theGraph, int numPart)
{
    # ifdef _PDD
    //--------------------------------------------------------------------------------
    int NUMProcessors;
    int PID;
    MPI_Comm_size(MPI_COMM_WORLD, &NUMProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &PID);

    if(PID == 0)
    {

        //Metis Format:
        //--------------------------------------------------------------------------------

        if (theGraph->getNumVertex() == numPart)
        {
            cerr << "METIS\n" << theGraph;
            Vertex* vertex;
            int current = 1;
            VertexIter& theVertices = theGraph->getVertices();

            while ((vertex = theVertices()) != 0)
            {
                vertex->setColor(current++);
            }

            return 0;
        }

        // first we check that the options are valid
        //     if (checkOptions() == false)
        //  return -1;

        // now we get room for the data structures metis needs

        int numVertex_METIS = theGraph->getNumVertex();
        int numEdge_METIS   = theGraph->getNumEdge();
        int* options_METIS = new int [5];
        int* partition_METIS  = new int [numVertex_METIS + 1];
        int* xadj_METIS  = new int [numVertex_METIS + 2];
        int* adjncy_METIS  = new int [2 * numEdge_METIS];
        int* vwgts_METIS  = 0;
        int* ewgts_METIS  = 0;
        int numbering_METIS  = 0;
        int weightflag_METIS  = 0; // no weights on our graphs yet
        int ncon_METIS = 1;
        int* vsize_METIS = NULL;
        int* adjwgt_METIS = NULL;
        int* ubvec_METIS = NULL;
        float* tpwgts_METIS = NULL;

        if (START_VERTEX_NUM == 0)
        {
            numbering_METIS  = 0;
        }
        else if (START_VERTEX_NUM == 1)
        {
            numbering_METIS  = 1;
        }
        else
        {
            cerr << "WARNING Metis::partition - No partitioning done";
            cerr << " vertex numbering must start at 0 or 1\n";
            return (-2);
        }

        int edgecut_METIS ;

        if ((options_METIS  == 0) || (partition_METIS  == 0) || (xadj_METIS  == 0) || (adjncy_METIS  == 0))
        {
            cerr << "WARNING Metis::partition - No partitioning done";
            cerr << " as ran out of memory\n";
            return (-2);
        }


        // we build these data structures

        int indexEdge_METIS  = 0;
        xadj_METIS [0] = 0;
        Vertex* vertexPtr_METIS ;

        for (int vertex = 0; vertex < numVertex_METIS; vertex++)
        {
            vertexPtr_METIS = theGraph->getVertexPtr(vertex + START_VERTEX_NUM);

            // check we don't have an invalid vertex numbering scheme
            // if so WARNING message, clean up and return -2

            if (vertexPtr_METIS == 0)
            {
                cerr << "WARNING Metis::partition - No partitioning done";
                cerr << " Metis requires consequtive Vertex Numbering\n";

                delete [] options_METIS;
                delete [] partition_METIS;
                delete [] xadj_METIS;
                delete [] adjncy_METIS;

                return -2;
            }

            const ID& adjacency_METIS = vertexPtr_METIS->getAdjacency();
            int degree_METIS = adjacency_METIS.Size();

            for (int i = 0; i < degree_METIS; i++)
            {
                adjncy_METIS[indexEdge_METIS++] = adjacency_METIS(i) - START_VERTEX_NUM;
            }

            xadj_METIS[vertex + 1] = indexEdge_METIS;
        }



        options_METIS[0] = 0; //default options

        METIS_PartGraphKway(&numVertex_METIS, &ncon_METIS, xadj_METIS, adjncy_METIS,
                            NULL, NULL, NULL, &numPart, tpwgts_METIS,
                            NULL, NULL, &edgecut_METIS, partition_METIS);

        // we set the vertex colors to correspond to the partitioned scheme
        for (int vert = 0; vert < numVertex_METIS; vert++)
        {
            vertexPtr_METIS = theGraph->getVertexPtr(vert + START_VERTEX_NUM);
            vertexPtr_METIS->setColor(partition_METIS[vert] + 1); // start colors at 1
        }

        // clean up the space and return

        delete [] options_METIS;
        delete [] partition_METIS;
        delete [] xadj_METIS;
        delete [] adjncy_METIS;

        return 0;
        //exit(1);
        //------------------------------------------------------------------------------------------------------------------------------------------
    }

    /*

    std::cerr<< "ParMetis::partition ...Partitioning ....\n";
    int npes, mype;
    int nvtxs;
    int your_nvtxs, your_nedges, gnedges;
    int maxnvtxs = -1, maxnedges = -1;
    int *vtxdist, *xadj, *adjncy, *vwgt;//, *vsize;
    int *your_xadj, *your_adjncy;//, *your_vwgt;//, *your_vsize;
    int ncon, nobj;

    int *part;

    int *partitions;

    MPI_Status stat;

    MPI_Comm worker;
    MPI_Comm comm;

    MPI_Comm_dup(MPI_COMM_WORLD, &comm);

    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &mype);

    int color = (mype == 0);

    MPI_Comm_split(comm, color, mype, &worker);

    npes = npes - 1; //master does not take part in partitioning;

    std::cout << "ParMetis::partition ...allocating memory for vtxdist ....\n";
    vtxdist = new int [npes+1];

    if (vtxdist == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for vtxdist ....\n";
    }

    if ( mype == 0 )
    {
    if (theGraph->getNumVertex() == numPart)
    {
            cerr << "ParMetis\n" << *theGraph;
            Vertex *vertex;
            int current = 1;
            VertexIter &theVertices = theGraph->getVertices();
            while ((vertex = theVertices()) != 0)
            vertex->setColor(current++);

            return 0;
    }

    //wwcerr << "ParMetis::partition() -2\n";

        gnvtxs = theGraph->getNumVertex();
    gnedges = theGraph->getNumEdge();

        vtxdist[0] = 0;
    for (int i=0, k=gnvtxs; i<npes; i++) {
            int l = k/(npes-i);
            vtxdist[i+1] = vtxdist[i]+l;
            k -= l;
        }
    MPI_Bcast((void *)vtxdist, npes+1, MPI_INT, 0, comm);
    MPI_Bcast((void *)&gnedges, 1, MPI_INT, 0, comm);
    }
    else {
    MPI_Bcast((void *)vtxdist, npes+1, MPI_INT, 0, comm);
    MPI_Bcast((void *)&gnedges, 1, MPI_INT, 0, comm);
    }

    ncon = 0;//number of constraints on vertices
    nobj = 0;//number of objectives on edges

    std::cout << "ParMetis::partition ...allocating memory for gnvtxs ....\n";
    gnvtxs = vtxdist[npes];
    if (gnvtxs == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for gnvtxs ....\n";
    }

    std::cout << "ParMetis::partition ...allocating memory for globalXadj ....\n";
    globalXadj = new int [gnvtxs+1];
    if (globalXadj == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for globalXadj ....\n";
    }

    std::cout << "ParMetis::partition ...allocating memory for globalAdjncy ....\n";
    globalAdjncy = new int [2*gnedges];
    if (globalAdjncy == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for globalAdjncy ....\n";
    }

    //if ( eleList != 0 ) delete [] eleList;
    //eleList = new int [gnvtxs+1];
    //if ( vertexList != 0 ) delete [] vertexList;
    //vertexList = new int [gnvtxs+1];
    //
    //if ( mype == 0 ) {
    //    for (int k = 0; k<gnvtxs; k++) {
    //        const int globalVerTag = k+START_VERTEX_NUM;
    //        Vertex *verPtr = theGraph.getVertexPtr(globalVerTag);
    //        const int eleTag = verPtr->getRef();
    //        eleList[globalVerTag] = eleTag;
    //        vertexList[eleTag] = globalVerTag;
    //    }
    //}
    //
    //MPI_Bcast((void*)eleList, gnvtxs+1, MPI_INT, 0, comm);
    //MPI_Bcast((void*)vertexList, gnvtxs+1, MPI_INT, 0, comm);

    if ( mype != 0 ) {
    nvtxs = vtxdist[mype]-vtxdist[mype-1];
    xadj = new int [nvtxs+1];
    if (xadj == NULL)
    {
        std::cerr<< "ParMetis::partition ...can not allocate memory for xadj ....\n";
    }
    //vsize = new int [nvtxs];
    //vwgt = new int [nvtxs*ncon];

    if (START_VERTEX_NUM == 0)
            numflag = 0;
    else if (START_VERTEX_NUM == 1)
            numflag = 1;
    else {
            cerr << "WARNING ParMetis::partition - No partitioning done";
            cerr << " vertex numbering must start at 0 or 1\n";
            return -2;
    }
    }

    if ( mype == 0 ) {
        maxnvtxs = 0;
        for (int i = 0; i<npes; i++) {
            maxnvtxs = (maxnvtxs < vtxdist[i+1] - vtxdist[i]) ?
            vtxdist[i+1]-vtxdist[i] : maxnvtxs;
    }

        your_xadj = new int [maxnvtxs+1];
    if (your_xadj == NULL)
    {
        std::cerr<< "ParMetis::partition ...can not allocate memory for your_xadj ....\n";
    }
        //your_vwgt = new int [maxnvtxs*ncon];
    //your_vsize = new int [maxnvtxs];

        //cerr << "ParMetis::partition() -3\n";
    maxnedges = 0;

    globalXadj[0] = 0;

    for (int pe = 0; pe < npes; pe++) {

        your_xadj[0] = 0;

        Vertex *vertexPtr;

        //for (int i = 0; i < maxnvtxs*ncon; i++) {
            //  your_vwgt[i] = 1;
            //}

        your_nvtxs = vtxdist[pe+1]-vtxdist[pe];

        for (int vertex = 0; vertex < your_nvtxs; vertex++) {

            vertexPtr = theGraph->getVertexPtr(vertex+vtxdist[pe]+START_VERTEX_NUM);

            // check we don't have an invalid vertex numbering scheme
                // if so WARNING message, clean up and return -2

                if (vertexPtr == 0) {
                    cerr << "WARNING ParMetis::partition - No partitioning done";
                    cerr << " Can't get vertexPtr!\n";

                    //delete [] vtxdist;
                    //delete [] vwgt;
                    //delete [] xadj;
                    //delete [] your_vwgt;
                    delete [] your_xadj;
                //delete [] your_vsize;

                    return -2;
                }

                const ID &adjacency = vertexPtr->getAdjacency();

            int degree = adjacency.Size();

            your_xadj[vertex+1] = your_xadj[vertex] + degree;
            globalXadj[vertex+vtxdist[pe]+1] = globalXadj[vertex+vtxdist[pe]] + degree;
            //your_vsize[vertex] = vertexPtr->getVertexSize();

            //Guanzhou out temporarily for (int i = 0; i < ncon ; i++) {
            //Guanzhou out temporarily  your_vwgt[vertex*ncon + i] = (int)vertexPtr->getWeight();
            //Guanzhou out temporarily }
            }

        maxnedges = (maxnedges < your_xadj[your_nvtxs]) ? your_xadj[your_nvtxs] : maxnedges;

            //if ( pe != 0 ) {
            MPI_Send((void *)your_xadj, your_nvtxs+1, MPI_INT, pe+1, 10, comm);
            //MPI_Send((void *)your_vwgt, your_nvtxs*ncon, MPI_INT, pe+1, 11, comm);
            //MPI_Send((void *)your_vsize, your_nvtxs, MPI_INT, pe+1, 12, comm);
            //}
            //else {
            //  for (int i=0; i<your_nvtxs+1; i++)
            //      xadj[i] = your_xadj[i];
            //  for (int i=0; i<your_nvtxs*ncon; i++)
            //      vwgt[i] = your_vwgt[i];
            //  for (int i=0; i<your_nvtxs; i++)
            //      vsize[i] = your_vsize[i];
        //}
    }
        //delete [] your_vsize;
    delete [] your_xadj;
        //delete [] your_vwgt;
    }
    else { //child processes
        MPI_Recv((void *)xadj, nvtxs+1, MPI_INT, 0, 10, comm, &stat);
        //MPI_Recv((void *)vwgt, nvtxs*ncon, MPI_INT, 0, 11, comm, &stat);
        //MPI_Recv((void *)vsize, nvtxs, MPI_INT, 0, 12, comm, &stat);
    }

    //gnedges = xadj[nvtxs];
    if ( mype != 0 ) {
    adjncy = new int [xadj[nvtxs]];
    if (adjncy == NULL)
    {
        std::cerr<< "ParMetis::partition ...can not allocate memory for adjncy ....\n";
    }
    }
    //adjncy = theGraph.New_adjncy(xadj[nvtxs]);

    //adjwgt = NULL;

    if ( mype == 0 ) {

    your_adjncy = new int [maxnedges];
    if (your_adjncy == NULL)
    {
        std::cerr<< "ParMetis::partition ...can not allocate memory for your_adjncy ....\n";
    }

    //cerr << "ParMetis::partition() -4\n";

    int myCount = 0;

    for (int pe = 0; pe < npes; pe++) {

        Vertex *vertexPtr;

        your_nedges = 0;

        your_nvtxs = vtxdist[pe+1]-vtxdist[pe];

        for (int vertex = 0; vertex < your_nvtxs; vertex++) {

            vertexPtr = theGraph->getVertexPtr(vertex+vtxdist[pe]+START_VERTEX_NUM);
                // check we don't have an invalid vertex numbering scheme
                // if so WARNING message, clean up and return -2

                if (vertexPtr == 0) {
                    cerr << "WARNING ParMetis::partition - No partitioning done";
                    cerr << " Can't get vertexPtr!\n";

                    //delete [] vtxdist;
                    //delete [] vwgt;
                    //delete [] xadj;
                    //delete [] adjncy;
                    //delete [] your_vwgt;
                    //delete [] your_xadj;
                    delete [] your_adjncy;

                    return -2;
                }

                const ID &adjacency = vertexPtr->getAdjacency();
                int degree = adjacency.Size();
                for (int i=0; i<degree; i++) {
                    your_adjncy[your_nedges++] = adjacency(i)-START_VERTEX_NUM;
                globalAdjncy[myCount++] = adjacency(i)-START_VERTEX_NUM;
                }
            }

        //if ( pe != 0 ) {
            MPI_Send((void *)your_adjncy, your_nedges, MPI_INT, pe+1, 10, comm);
            //}
            //else {
            //  for (int i=0; i<your_nedges; i++)
            //      adjncy[i] = your_adjncy[i];
        //}
      }

      delete [] your_adjncy;
      //delete [] your_vwgt;
    }
    else { //child processes
        MPI_Recv((void *)adjncy, xadj[nvtxs], MPI_INT, 0, 10, comm, &stat);
        //MPI_Recv((void *)vwgt, nvtxs*ncon, MPI_INT, 0, 1, comm, &stat);
    }

    MPI_Bcast((void *)globalXadj, gnvtxs+1, MPI_INT, 0, comm);
    MPI_Bcast((void *)globalAdjncy, 2*gnedges, MPI_INT, 0, comm);

    //cerr << "ParMetis::partition() -5\n";

    part = new int [nvtxs];
    if (part == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for part ....\n";
    }

    //int *part = theGraph.New_part(nvtxs);
    //if ( ubvec != 0 ) delete [] ubvec;
    //wcerr << "ParMetis::partition() -6\n";
    // we now the ParMetis routines

    //if (mype == 0)
    //    printf("\nTesting ParMETIS_V3_PartKway with options[1-2] = {%d %d}, Ncon: %d, Nparts: %d\n", options[1], options[2], ncon, nparts);

    // if ( mype == 2 ) {
    // ofstream f_xadj("xadj2.dat", ios::out);
    // ofstream f_adjncy("adjncy2.dat", ios::out);
    // ofstream f_vtxdist("vtxdist2.dat", ios::out);
    //
    // for (int i=0; i<nvtxs; i++) {
    //    f_xadj << *(xadj+i) << '\n';
    // }
    //
    // for (int i=0; i<xadj[nvtxs]; i++) {
    //    f_adjncy << *(adjncy+i) << '\n';
    // }
    //
    // for (int i=0; i<npes+1; i++) {
    //    f_vtxdist << *(vtxdist+i) << '\n';
    // }
    //
    // f_xadj.close();
    // f_adjncy.close();
    // f_vtxdist.close();
    //
    // }
    //
    // if ( mype == 1 ) {
    // ofstream f_xadj("xadj1.dat", ios::out);
    // ofstream f_adjncy("adjncy1.dat", ios::out);
    // ofstream f_vtxdist("vtxdist1.dat", ios::out);
    //
    // for (int i=0; i<nvtxs; i++) {
    //    f_xadj << *(xadj+i) << '\n';
    // }
    //
    // for (int i=0; i<xadj[nvtxs]; i++) {
    //    f_adjncy << *(adjncy+i) << '\n';
    // }
    //
    // for (int i=0; i<npes+1; i++) {
    //    f_vtxdist << *(vtxdist+i) << '\n';
    // }
    //
    // f_xadj.close();
    // f_adjncy.close();
    // f_vtxdist.close();
    //
    // }
    //
    //if ( mype == 0 ) {
    //ofstream f_xadj("xadj0.dat", ios::out);
    //ofstream f_adjncy("adjncy0.dat", ios::out);
    //ofstream f_vtxdist("vtxdist0.dat", ios::out);
    //
    //for (int i=0; i<nvtxs; i++) {
    //   f_xadj << *(xadj+i) << '\n';
    //}
    //
    //for (int i=0; i<xadj[nvtxs]-1; i++) {
    //   f_adjncy << *(adjncy+i) << '\n';
    //}
    //
    //for (int i=0; i<npes+1; i++) {
    //   f_vtxdist << *(vtxdist+i) << '\n';
    //}
    //
    //f_xadj.close();
    //f_adjncy.close();
    //f_vtxdist.close();
    //
    //}

    if ( mype != 0 ) {
    int edgecut;
    wgtflag = 2;
    numflag = 0;
    ncon = 1;
    nobj = 0;
    vwgt = new int [nvtxs];
    if (vwgt == NULL)
    {
      std::cerr<< "ParMetis::partition ...can not allocate memory for vwgt ....\n";
    }



    for (int i = 0; i<nvtxs; i++)
    {
        vwgt[i] = 1;
    }

    tpwgts = new float [ncon*numPart];
    if (tpwgts == NULL)
    {
      std::cerr<< "ParMetis::partition ...can not allocate memory for tpwgts ....\n";
    }

    for ( int i = 0; i < ncon*numPart; i++)
    {
        tpwgts[i] = 1.0/(float)numPart;
    }

        ubvec = new float [ncon];//unbalance vector

    if (ubvec == NULL)
    {
      std::cerr<< "ParMetis::partition ...can not allocate memory for ubvec ....\n";
    }


    for (int i = 0; i < ncon ; i++)
    {
        ubvec[i] = 1.02;
    }

        std::cerr << "ParMetis::partition -- calling ParMETIS_V3_PartKway...\n";

    ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, vwgt, NULL, &wgtflag,
            &numflag, &ncon, &numPart, tpwgts, ubvec, options, &edgecut, part, &worker);
    cerr << "ParMETIS_V3_PartKway reported a cut of " << edgecut << "\n";
    delete [] vwgt;
    }

    //cerr << "ParMetis::partition() -7\n";

    partitions = new int [gnvtxs];
    if (partitions == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for partitions ....\n";
    }

    int *rcounts = new int [npes];
    if (rcounts == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for rcounts ....\n";
    }
    int *displs = new int [npes];
    if (displs == NULL)
    {
    std::cerr<< "ParMetis::partition ...can not allocate memory for displs ....\n";
    }

    if ( mype != 0 )
    MPI_Send((void*)&nvtxs, 1, MPI_INT, 0, 10, comm);
    else
    for (int i = 1; i<=npes; i++)
        MPI_Recv((void*)(rcounts+i-1), 1, MPI_INT, i, 10, comm, &stat);

    displs[0] = 0;
    for (int pe = 1; pe < npes; pe++) {
    displs[pe] = displs[pe-1] + rcounts[pe-1];
    }

    if ( mype != 0 )
    MPI_Send((void*)part, nvtxs, MPI_INT, 0, mype, comm);
    else
    for (int i = 1; i<=npes; i++)
        MPI_Recv((void*)(partitions+displs[i-1]), rcounts[i-1], MPI_INT, i, i, comm, &stat);


    // we set the vertex colors to correspond to the partitioned scheme
    if (mype == 0) {
    for (int vert =0; vert<gnvtxs; vert++) {
        Vertex *vertexPtr;
        vertexPtr = theGraph->getVertexPtr(vert+START_VERTEX_NUM);
        vertexPtr->setColor(partitions[vert]+1); // start colors at 1!!!
        }
    }
    Vertex *vertex;
    VertexIter &theVertices = theGraph->getVertices();
    while ((vertex = theVertices()) != 0)
    vertex->setWeight(0.0);

    // clean up the space and return
    //cerr << "ParMetis::partition() -8\n";

    delete [] partitions;
    delete [] rcounts;
    delete [] displs;
    delete [] vtxdist;
    if ( mype != 0 ) {
    delete [] xadj;
    delete [] adjncy;
    }

    delete [] part;

    //cerr << "ParMetis::partition() -9\n";

    MPI_Comm_free(&worker);
    MPI_Comm_free(&comm);

    # endif

    return 0;

    */
    # endif
}

int
ParMetis::repartition(Graph* theGraph, int numPart)
{
    return 0;
    # ifdef _PDD
    int npes, mype;
    int nvtxs;
    int* vtxdist, *xadj, *adjncy, *vsize, *vwgt;
    int* part;

    //MPI_Request *req;
    // = new MPI_Request [gnvtxs];

    //int *your_xadj, *your_adjncy, *your_vwgt;
    //int ncon, nobj, maxnvtxs = -1;
    //int your_nvtxs;

    int* partitions;

    //int numbering;

    MPI_Status stat;

    //MPI_Status *status;
    // = new MPI_Status [gnvtxs];

    MPI_Comm worker;
    MPI_Comm comm;

    MPI_Comm_dup(MPI_COMM_WORLD, &comm);

    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &mype);

    int color = (mype == 0);

    MPI_Comm_split(comm, color, mype, &worker);

    npes = npes - 1; //master does not take part in partitioning;

    vtxdist = new int [npes + 1];

    if ( mype != 0 )
    {
        //cerr << "Subdomain " << mype << endln;
        //cerr << *theGraph;
        //cerr << "end!!!!!!!!" << endln;
        nvtxs = theGraph->getNumVertex();
        vwgt = new int [nvtxs];
        xadj = new int [nvtxs + 1];
        vsize = new int [nvtxs];
    }

    vtxdist[0] = 0;

    if ( mype != 0 )
    {
        MPI_Send((void*)&nvtxs, 1, MPI_INT, 0, mype, comm);
    }
    else
        for (int i = 1; i <= npes; i++)
        {
            MPI_Recv((void*)(vtxdist + i), 1, MPI_INT, i, i, comm, &stat);
        }

    if ( mype == 0 )
    {
        for (int i = 1; i <= npes; i++)
        {
            vtxdist[i] = vtxdist[i - 1] + vtxdist[i];
        }
    }

    MPI_Bcast((void*)vtxdist, npes + 1, MPI_INT, 0, comm);

    int* OldToNewLabel = new int [gnvtxs];

    if ( mype != 0 )
    {
        xadj[0] = 0;

        MPI_Request* req = new MPI_Request [nvtxs];
        MPI_Status* stats = new MPI_Status [nvtxs];

        for (int i = 0; i < nvtxs; i++)
        {
            Vertex* vertexPtr = theGraph->getVertexPtr(i);
            vwgt[i] = (int)(vertexPtr->getWeight());
            vsize[i] = vertexPtr->getVertexSize();
            const int eleTag = vertexPtr->getRef();
            const int oldVertexTag = eleTag - 1;
            xadj[i + 1] = xadj[i] + globalXadj[oldVertexTag + 1] - globalXadj[oldVertexTag];

            const int newVertexTag = i + vtxdist[mype - 1];
            MPI_Isend((void*)&newVertexTag, 1, MPI_INT, 0, oldVertexTag, comm, req + i);
        }

        MPI_Waitall(nvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }
    else
    {

        MPI_Request* req = new MPI_Request [gnvtxs];
        MPI_Status* stats = new MPI_Status [gnvtxs];

        for (int i = 0; i < gnvtxs; i++)
        {
            MPI_Irecv((void*)(OldToNewLabel + i), 1, MPI_INT, MPI_ANY_SOURCE, i, comm, req + i);
        }

        MPI_Waitall(gnvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }

    MPI_Bcast((void*)(OldToNewLabel), gnvtxs, MPI_INT, 0, comm);

    if ( mype != 0 )
    {
        adjncy = new int [xadj[nvtxs]];
        int count = 0;

        for (int i = 0; i < nvtxs; i++)
        {
            Vertex* vertexPtr = theGraph->getVertexPtr(i + START_VERTEX_NUM);
            const int eleTag = vertexPtr->getRef();
            const int oldVertexTag = eleTag - 1;

            for (int j = globalXadj[oldVertexTag]; j < globalXadj[oldVertexTag + 1]; j++)
            {
                const int oldVertexTag = globalAdjncy[j];
                adjncy[count++] = OldToNewLabel[globalAdjncy[j]];
            }
        }
    }

    itr = 0.001;

    //for debug purpose;
    //if ( mype == 1 ) {
    //for (int i = 0; i<nvtxs; i++) {
    //        vwgt[0] = 5;
    //}
    //}
    //
    //if ( mype == 2 ) {
    //    for (int i = 0; i<nvtxs; i++) {
    //        vwgt[i] = 1;
    //    }
    //}

    int mypwgt = 0;
    int max_wgt = 0, min_wgt = 0, sum_wgt = 0;

    int* edge_weight; //test for edge weight

    if ( mype != 0 )
    {
        edge_weight = new int [xadj[nvtxs]];

        for (int i = 0; i < xadj[nvtxs]; i++)
        {
            edge_weight[i] = 1;
        }
    }


    if ( mype != 0 )
    {
        for (int i = 0; i < nvtxs; i++)
        {
            mypwgt = mypwgt + vwgt[i];
        }

        MPI_Allreduce((void*)&mypwgt, (void*)&max_wgt, 1, MPI_INT, MPI_MAX, worker);
        MPI_Allreduce((void*)&mypwgt, (void*)&min_wgt, 1, MPI_INT, MPI_MIN, worker);
        MPI_Allreduce((void*)&mypwgt, (void*)&sum_wgt, 1, MPI_INT, MPI_SUM, worker);

        if (mype == 1)
        {
            cerr << "Initial Load Imbalance: " << (1.0 * max_wgt * npes) / (1.0 * sum_wgt) << endln;
            cerr << "MAX: " << max_wgt << " MIN: " << min_wgt << " SUM: " << sum_wgt << endln;
        }
    }

    int edgecut = 0;
    //wgtflag = 2;
    wgtflag = 3; //test for edge weight
    numflag = 0;
    ncon = 1;
    nobj = 0;
    part = new int [nvtxs];
    options[0] = 1;
    options[1] = 2;
    options[2] = 15;
    options[3] = 1;

    //if ( mype == 2 ) vwgt[1] = 30000;
    if ( mype != 0 )
    {
        //test for edge weightParMETIS_V3_AdaptiveRepart(vtxdist, xadj, adjncy, vwgt,
        //test for edge weight  vsize, NULL, &wgtflag, &numflag, &ncon, &numPart, tpwgts, ubvec, &itr,
        //test for edge weight  options, &edgecut, part, &worker);
        ParMETIS_V3_AdaptiveRepart(vtxdist, xadj, adjncy, vwgt,
                                   vsize, edge_weight, &wgtflag, &numflag, &ncon, &numPart, tpwgts, ubvec, &itr,
                                   options, &edgecut, part, &worker);

        //ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, vwgt, NULL, &wgtflag,
        //  &numflag, &ncon, &numPart, tpwgts, ubvec, options, &edgecut, part, &worker);


        cerr << "ParMETIS_V3_AdaptiveRepart reported a cut of " << edgecut << "\n";
    }

    partitions = new int [gnvtxs];

    if ( mype != 0 )
    {
        MPI_Request* req = new MPI_Request [nvtxs];
        MPI_Status* stats = new MPI_Status [nvtxs];

        for (int i = 0; i < nvtxs; i++)
        {
            Vertex* vertexPtr = theGraph->getVertexPtr(i + START_VERTEX_NUM);
            const int eleTag = vertexPtr->getRef();
            const int oldVertexTag = eleTag - 1;
            MPI_Isend((void*)(part + i), 1, MPI_INT, 0, oldVertexTag, comm, req + i);
        }

        MPI_Waitall(nvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }
    else
    {
        MPI_Request* req = new MPI_Request [gnvtxs];
        MPI_Status* stats = new MPI_Status [gnvtxs];

        for (int i = 0; i < gnvtxs; i++)
        {
            MPI_Irecv((void*)(partitions + i), 1, MPI_INT, MPI_ANY_SOURCE, i, comm, req + i);
        }

        MPI_Waitall(gnvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }

    if (mype == 0)
    {
        for (int vert = 0; vert < gnvtxs; vert++)
        {
            Vertex* vertexPtr;
            vertexPtr = theGraph->getVertexPtr(vert + START_VERTEX_NUM);
            vertexPtr->setColor(partitions[vert] + 1); // start colors at 1!!!
        }
    }
    else
    {
        for (int i = 0; i < nvtxs; i++)
        {
            Vertex* vertex;
            vertex = theGraph->getVertexPtr(i + START_VERTEX_NUM);
            vertex->setColor(part[i] + 1);
        }
    }

    if ( mype != 0 )
    {
        for (int i = 0; i < nvtxs; i++)
        {
            Vertex* vertexPtr = theGraph->getVertexPtr(i);
            vertexPtr->setWeight(0.0);
        }
    }

    // clean up the space and return

    delete [] partitions;
    delete [] vtxdist;

    if (mype != 0)
    {
        delete [] xadj;
        delete [] adjncy;
        delete [] vwgt;
        delete [] vsize;
        delete [] edge_weight;
    }

    delete [] part;
    delete [] OldToNewLabel;
    //cerr << "ParMetis::partition() -9\n";

    MPI_Comm_free(&worker);
    MPI_Comm_free(&comm);

    # endif

    return 0;

}

int
ParMetis::sendSelf(int cTag, Channel& theChannel)
{
    return 0;
}

int
ParMetis::recvSelf(int cTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}

