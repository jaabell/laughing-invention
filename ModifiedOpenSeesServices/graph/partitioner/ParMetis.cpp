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
    int *vtxdist, int *xadj, int *adjncy, int *vwgt,
    int *adjwgt, int *wgtflag, int *numflag, int *ncon, int *nparts,
    float *tpwgts, float *ubvec, int *options, int *edgecut, int *part,
    MPI_Comm *comm);

extern "C"
void ParMETIS_V3_AdaptiveRepart(
    int *vtxdist, int *xadj, int *adjncy, int *vwgt,
    int *vsize, int *adjwgt, int *wgtflag, int *numflag, int *ncon,
    int *nparts, float *tpwgts, float *ubvec, float *ipc2redist,
    int *options, int *edgecut, int *part, MPI_Comm *comm);

extern "C"
int METIS_PartGraphKway(int *numVertex_METIS, int *ncon_METIS, int *xadj_METIS, int *adjncy_METIS,
                        int *vwgts_METIS, int *vsize_METIS, int *adjwgt_METIS, int *numPart, float *tpwgts_METIS,
                        float *ubvec_METIS, int *options_METIS, int *edgecut_METIS, int *partition_METIS);
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
ParMetis::partition(Graph *theGraph, int numPart)
{
# ifdef _PDD
    //--------------------------------------------------------------------------------
    int NUMProcessors;
    int PID;
    MPI_Comm_size(MPI_COMM_WORLD, &NUMProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &PID);

    if (PID == 0)
    {

        //Metis Format:
        //--------------------------------------------------------------------------------

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

        // first we check that the options are valid
        //     if (checkOptions() == false)
        //  return -1;

        // now we get room for the data structures metis needs

        int numVertex_METIS = theGraph->getNumVertex();
        int numEdge_METIS   = theGraph->getNumEdge();
        int *options_METIS = new int [5];
        int *partition_METIS  = new int [numVertex_METIS + 1];
        int *xadj_METIS  = new int [numVertex_METIS + 2];
        int *adjncy_METIS  = new int [2 * numEdge_METIS];
        int *vwgts_METIS  = 0;
        int *ewgts_METIS  = 0;
        int numbering_METIS  = 0;
        int weightflag_METIS  = 0; // no weights on our graphs yet
        int ncon_METIS = 1;
        int *vsize_METIS = NULL;
        int *adjwgt_METIS = NULL;
        int *ubvec_METIS = NULL;
        float *tpwgts_METIS = NULL;

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
        Vertex *vertexPtr_METIS ;

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

            const ID &adjacency_METIS = vertexPtr_METIS->getAdjacency();
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
        // delete [] xadj_METIS;
        // delete [] adjncy_METIS;

        //Store for use with ParMETIS
        gnvtxs = numVertex_METIS;
        gnedge = numEdge_METIS;
        globalXadj = xadj_METIS;
        globalAdjncy = adjncy_METIS;

        //Now share this info with everyone!!!
        MPI_Bcast(&gnvtxs, 1 , MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&gnedge, 1 , MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast((void *)globalXadj, gnvtxs + 2, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast((void *)globalAdjncy, 2 * gnedge, MPI_INT, 0, MPI_COMM_WORLD);



        return 0;
        //exit(1);
        //------------------------------------------------------------------------------------------------------------------------------------------
    }
    else
    {
        //Getting global number of vertices from P0
        MPI_Bcast(&gnvtxs, 1 , MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&gnedge, 1 , MPI_INT, 0, MPI_COMM_WORLD);
        globalXadj = new int[gnvtxs + 2];
        globalAdjncy = new int[ 2 * gnedge];
        MPI_Bcast((void *)globalXadj, gnvtxs + 2, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast((void *)globalAdjncy, 2 * gnedge, MPI_INT, 0, MPI_COMM_WORLD);


        if ( PID != 0 )
        {
            //Initialize these arrays needed later in ParMeTiS
            int edgecut;
            wgtflag = 2;
            numflag = 0;
            ncon = 1;
            nobj = 0;

            tpwgts = new float [ncon * numPart];

            for ( int i = 0; i < ncon * numPart; i++)
            {
                tpwgts[i] = 1.0 / (float)numPart;
            }

            ubvec = new float [ncon];//unbalance vector

            for (int i = 0; i < ncon ; i++)
            {
                ubvec[i] = 1.05;
            }

        }
    }

# endif
}

int
ParMetis::repartition(Graph *theGraph, int numPart)
{
    // return 0;
# ifdef _PDD
    int npes, mype;
    int nvtxs;
    int *vtxdist, *xadj, *adjncy, *vsize, *vwgt;
    int *part;

    //MPI_Request *req;
    // = new MPI_Request [gnvtxs];

    //int *your_xadj, *your_adjncy, *your_vwgt;
    //int ncon, nobj, maxnvtxs = -1;
    //int your_nvtxs;

    int *partitions;

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
        MPI_Send((void *)&nvtxs, 1, MPI_INT, 0, mype, comm);
    }
    else
    {
        for (int i = 1; i <= npes; i++)
        {
            MPI_Recv((void *)(vtxdist + i), 1, MPI_INT, i, i, comm, &stat);
        }
    }
    if ( mype == 0 )
    {
        for (int i = 1; i <= npes; i++)
        {
            vtxdist[i] = vtxdist[i - 1] + vtxdist[i];
        }
    }

    MPI_Bcast((void *)vtxdist, npes + 1, MPI_INT, 0, comm);

    int *OldToNewLabel = new int [gnvtxs];

    if ( mype != 0 )
    {
        xadj[0] = 0;

        MPI_Request *req = new MPI_Request [nvtxs];
        MPI_Status *stats = new MPI_Status [nvtxs];

        int weight_factor = 1;

        if (mype == 1)
        {
            weight_factor = 100;
        }
        for (int i = 0; i < nvtxs; i++)
        {
            Vertex *vertexPtr = theGraph->getVertexPtr(i);
            vwgt[i] = (int)(vertexPtr->getWeight()) * weight_factor;

            if (vwgt[i] < 0)
            {
                cerr << "ParMetis::repartition() -- Found negative weight!! for ";
                cerr << "          i      = " << i << "\n";
                cerr << "          Ref    = " << vertexPtr->getRef() << "\n";
                cerr << "          Weight = " << vertexPtr->getWeight() << "\n";
                cerr << "          Color  = " << vertexPtr->getColor() << "\n";
                cerr << "          Tmp    = " << vertexPtr->getTmp() << endl;

                vwgt[i] = 0;
                // return -1;
            }

            vsize[i] = vertexPtr->getVertexSize();
            const int eleTag = vertexPtr->getRef();
            const int oldVertexTag = eleTag - 1;
            xadj[i + 1] = xadj[i] + globalXadj[oldVertexTag + 1] - globalXadj[oldVertexTag];

            const int newVertexTag = i + vtxdist[mype - 1];
            MPI_Isend((void *)&newVertexTag, 1, MPI_INT, 0, oldVertexTag, comm, req + i);
        }

        MPI_Waitall(nvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }
    else
    {

        MPI_Request *req = new MPI_Request [gnvtxs];
        MPI_Status *stats = new MPI_Status [gnvtxs];

        for (int i = 0; i < gnvtxs; i++)
        {
            MPI_Irecv((void *)(OldToNewLabel + i), 1, MPI_INT, MPI_ANY_SOURCE, i, comm, req + i);
        }

        MPI_Waitall(gnvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }

    MPI_Bcast((void *)(OldToNewLabel), gnvtxs, MPI_INT, 0, comm);

    if ( mype != 0 )
    {
        adjncy = new int [xadj[nvtxs]];
        int count = 0;

        for (int i = 0; i < nvtxs; i++)
        {
            Vertex *vertexPtr = theGraph->getVertexPtr(i + START_VERTEX_NUM);
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

    int max_wgt = 0, min_wgt = 0, sum_wgt = 0;

    int *edge_weight; //test for edge weight

    if ( mype != 0 )
    {
        edge_weight = new int [xadj[nvtxs]];

        for (int i = 0; i < xadj[nvtxs]; i++)
        {
            edge_weight[i] = 1;
        }
    }


    int mypwgt = 0;
    if ( mype != 0 )
    {
        for (int i = 0; i < nvtxs; i++)
        {
            mypwgt += vwgt[i];
        }

        MPI_Allreduce((void *)&mypwgt, (void *)&max_wgt, 1, MPI_INT, MPI_MAX, worker);
        MPI_Allreduce((void *)&mypwgt, (void *)&min_wgt, 1, MPI_INT, MPI_MIN, worker);
        MPI_Allreduce((void *)&mypwgt, (void *)&sum_wgt, 1, MPI_INT, MPI_SUM, worker);

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
        cout << " --- Calling ParMETIS --- " << endl;
        ParMETIS_V3_AdaptiveRepart(vtxdist, xadj, adjncy, vwgt,
                                   vsize, edge_weight, &wgtflag, &numflag, &ncon, &numPart, tpwgts, ubvec, &itr,
                                   options, &edgecut, part, &worker);
        cout << " --- End     ParMETIS --- " << endl;

        //ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, vwgt, NULL, &wgtflag,
        //  &numflag, &ncon, &numPart, tpwgts, ubvec, options, &edgecut, part, &worker);


        cerr << "ParMETIS_V3_AdaptiveRepart reported a cut of " << edgecut << "\n";
    }


    if ( mype != 0 )
    {
        MPI_Request *req = new MPI_Request [nvtxs];
        MPI_Status *stats = new MPI_Status [nvtxs];

        for (int i = 0; i < nvtxs; i++)
        {
            Vertex *vertexPtr = theGraph->getVertexPtr(i + START_VERTEX_NUM);
            const int eleTag = vertexPtr->getRef();
            const int oldVertexTag = eleTag - 1;
            MPI_Isend((void *)(part + i), 1, MPI_INT, 0, oldVertexTag, comm, req + i);
        }

        MPI_Waitall(nvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }
    else
    {
        MPI_Request *req = new MPI_Request [gnvtxs];
        MPI_Status *stats = new MPI_Status [gnvtxs];
        partitions = new int [gnvtxs];

        for (int i = 0; i < gnvtxs; i++)
        {
            MPI_Irecv((void *)(partitions + i), 1, MPI_INT, MPI_ANY_SOURCE, i, comm, req + i);
        }

        MPI_Waitall(gnvtxs, req, stats);
        delete [] req;
        delete [] stats;
    }

    if (mype == 0)
    {
        // This is old... P0 has nothing.... Thu 12 Nov 2015 05:08:38 PM PST - Jaabell
        for (int vert = 0; vert < gnvtxs; vert++)
        {
            Vertex *vertexPtr;
            vertexPtr = theGraph->getVertexPtr(vert + START_VERTEX_NUM);
            vertexPtr->setColor(partitions[vert] + 1); // start colors at 1!!!
        }
    }
    else
    {
        for (int i = 0; i < nvtxs; i++)
        {
            Vertex *vertex;
            vertex = theGraph->getVertexPtr(i + START_VERTEX_NUM);
            vertex->setColor(part[i] + 1);
        }
    }

    if ( mype != 0 )
    {
        for (int i = 0; i < nvtxs; i++)
        {
            Vertex *vertexPtr = theGraph->getVertexPtr(i);
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
ParMetis::sendSelf(int cTag, Channel &theChannel)
{
    return 0;
}

int
ParMetis::receiveSelf(int cTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

