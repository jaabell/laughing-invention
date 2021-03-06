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

// $Revision$
// $Date$
// $Source$

// Written: fmk
// Revision: A
//
// Description: This file contains the implementation for ParallelNumberer.
//
// What: "@(#) ParallelNumberer.C, revA"

#include <ParallelNumberer.h>
#include <AnalysisModel.h>

#include <Domain.h>
#include <ID.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <Graph.h>
#include <Vertex.h>
#include <VertexIter.h>
#include <DOF_Group.h>
#include <GraphNumberer.h>
#include <FE_Element.h>
#include <FE_EleIter.h>
#include <iostream>
#include <Node.h>
#include <MP_Constraint.h>
#include <MP_ConstraintIter.h>

using namespace std;

ParallelNumberer::ParallelNumberer(int dTag, int numSub, Channel** theC)
    : DOF_Numberer(NUMBERER_TAG_ParallelNumberer),
      processID(dTag), numChannels(numSub)

{
    theChannels = new Channel *[numSub];

    for (int i = 0; i < numSub; i++)
    {
        theChannels[i] = theC[i];
    }
}

ParallelNumberer::ParallelNumberer()
    : DOF_Numberer(NUMBERER_TAG_ParallelNumberer), theNumberer(0),
      processID(0), numChannels(0), theChannels(0)
{

}


ParallelNumberer::ParallelNumberer(GraphNumberer& theGraphNumberer)
    : DOF_Numberer(NUMBERER_TAG_ParallelNumberer), theNumberer(&theGraphNumberer),
      processID(0), numChannels(0), theChannels(0)
{

}


ParallelNumberer::~ParallelNumberer()
{
    if (theChannels != 0)
    {
        delete [] theChannels;
    }

    if (theNumberer != 0)
    {
        delete theNumberer;
    }
}


int
ParallelNumberer::setProcessID(int dTag)
{
    processID = dTag;
    return 0;
}

int
ParallelNumberer::setChannels(int nChannels, Channel** theC)
{
    numChannels = nChannels;

    if (theChannels != 0)
    {
        delete [] theChannels;
    }

    theChannels = new Channel *[numChannels];

    for (int i = 0; i < numChannels; i++)
    {
        theChannels[i] = theC[i];
    }

    return 0;
}


// int numberDOF(void)
// The ParalellNumberer sitting on P0, collects each partition graph from P1 through Pn-1,
// merges them into 1 large graph, & then numbers this graph. The ParallelNumberers sitting
// on P1 through Pn-1 then receive the mapping info for the dof tag and dof numbering from P0.

int
ParallelNumberer::numberDOF(int lastDOF)
{
#ifdef _PARALLEL_PROCESSING
    //Split-off a new communicator
    int numProcesses_world, processID_world;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID_world);
    int *worker_ranks = new int[numProcesses_world - 1];
    if (worker_ranks == NULL)
    {
        cerr << "ParallelNumberer::numberDOF(int lastDOF) - could not allocate new ranks vector of size " << numProcesses_world <<  " \n";
    }

    for (int rank = 1; rank < numProcesses_world ; rank++)
    {
        worker_ranks[rank - 1] = rank;
    }
    MPI_Group worker_group, world_group;
    MPI_Comm worker_comm;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    MPI_Group_incl(world_group, numProcesses_world - 1, worker_ranks, &worker_group);
    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);

    int result = 0;

    // get a pointer to the model & check its not null
    AnalysisModel* theModel = this->getAnalysisModelPtr();
    Domain* theDomain = 0;

    if (theModel != 0)
    {
        theDomain = theModel->getDomainPtr();
    }

    if (theModel == 0 || theDomain == 0)
    {
        cerr << "WARNING ParallelNumberer::numberDOF(int) -";
        cerr << " - no AnalysisModel - has setLinks() been invoked?\n";
        return -1;
    }

    if (lastDOF != -1)
    {
        cerr << "WARNING ParallelNumberer::numberDOF(int lastDOF):";
        cerr << " does not use the lastDOF as requested\n";
    }

    Graph& theGraph = theModel->getDOFGroupGraph();

    // if subdomain, collect graph, send it off, get
    // ID back containing dof tags & start id numbers.
    if (processID != 0)
    {

        Channel* theChannel = theChannels[0];
        int numVertex = theGraph.getNumVertex();

        theGraph.sendSelf(0, *theChannel);

        ID vertexTags(2 * numVertex);
        theChannel->receiveID(0, 0, vertexTags);

        // set vertex numbering based on ID received
        for (int i = 0; i < numVertex; i ++)
        {
            int vertexTag = vertexTags(i);
            int startID = vertexTags(i + numVertex);

            int dofTag = vertexTag;
            DOF_Group* dofPtr;
            dofPtr = theModel->getDOF_GroupPtr(dofTag);

            if (dofPtr == 0)
            {
                cerr << "WARNING ParallelNumberer::numberDOF - ";
                cerr << "DOF_Group " << dofTag << "not in AnalysisModel!\n";
                result = -4;
            }
            else
            {
                const ID& theDOFID = dofPtr->getID();

                int idSize = theDOFID.Size();

                for (int j = 0; j < idSize; j++)
                    if (theDOFID(j) == -2 || theDOFID(j) == -3)
                    {
                        dofPtr->setID(j, startID++);
                    }
            }
        }

        MP_ConstraintIter& theMPs = theDomain->getMPs();
        MP_Constraint* mpPtr = 0;

        const ID retainedDOFIDs_empty(ParallelNumberer_MAXDOFS, ParallelNumberer_MAXDOFS, -2000);
        // ID retainedDOFIDs_array(ParallelNumberer_MAXDOFS, ParallelNumberer_MAXDOFS, -1000);
        int retainedDOFIDs_array[numProcesses_world * ParallelNumberer_MAXDOFS];
        int recvcount[numProcesses_world];
        int displs[numProcesses_world];

        //Used in the MPI allgatherv below
        for (int p = 0; p < numProcesses_world; p++)
        {
            recvcount[p] = ParallelNumberer_MAXDOFS;
            displs[p] = ParallelNumberer_MAXDOFS * p;
        }

        while ((mpPtr = theMPs()) != 0 )
        {
            //Fill retainedDOFIDs_array array with some initial negative values
            for (int i = 0; i < (numProcesses_world * ParallelNumberer_MAXDOFS); i++)
            {
                retainedDOFIDs_array[i] = -1000;
            }

            const ID& constrainedDOFs = mpPtr->getConstrainedDOFs();
            const ID& retainedDOFs = mpPtr->getRetainedDOFs();

            // << endl;
            // note keep looping over all in case multiple constraints
            // are used to constrain a node -- can't assume intelli user

            int nodeRetained = mpPtr->getNodeRetained();
            int nodeConstrained = mpPtr->getNodeConstrained();

            Node* nodeRetainedPtr = theDomain->getNode(nodeRetained);
            Node* nodeConstrainedPtr = theDomain->getNode(nodeConstrained);

            DOF_Group* retainedDOFGrp = (nodeRetainedPtr != 0) ? nodeRetainedPtr->getDOF_GroupPtr() : 0;
            DOF_Group* constrainedDOFGrp = (nodeConstrainedPtr != 0) ? nodeConstrainedPtr->getDOF_GroupPtr() : 0;

            const ID& retainedDOFIDs = (retainedDOFGrp != 0) ? retainedDOFGrp->getID() : retainedDOFIDs_empty;

            for (int i = 0; i < retainedDOFIDs.Size(); i++)
            {
                retainedDOFIDs_array[ParallelNumberer_MAXDOFS * (processID_world - 1) + i] = retainedDOFIDs(i);  // (processID_world - 1) is because process numbers must be within current communicator, that is worker_comm
            }

            /*int status = */MPI_Allgatherv(MPI_IN_PLACE, ParallelNumberer_MAXDOFS,
                                        MPI_INT, retainedDOFIDs_array, recvcount,
                                        displs, MPI_INT, worker_comm);

            // For clarity, this is just a view into the last row of the retainedDOFIDs_array, where the final values will be stored.
            int *retainedDOFIDs_final = &retainedDOFIDs_array[ParallelNumberer_MAXDOFS * (numProcesses_world - 1)];

            for (int p = 0; p < numProcesses_world - 1; p++)
            {
                for (int i = 0; i < ParallelNumberer_MAXDOFS; i++)
                {
                    int dofID = retainedDOFIDs_array[ParallelNumberer_MAXDOFS * p + i];
                    if ( dofID >= 0)
                    {
                        if (retainedDOFIDs_final[i] >= 0 )
                        {
                            if (retainedDOFIDs_final[i] != dofID)
                            {
                                //This error means that the current DOF is shared by more than two processors.
                                // and that the numbering (dofID) is inconsistent across processors. This
                                // should not happen.
                                cout << "!!! ParallelNumberer::numberDOF() -- error: retainedDOFIDs_final[i] != retainedDOFIDs_array[ParallelNumberer_MAXDOFS * p + i]\n";
                            }
                            //The else condition is as it should be: Shared DOFs are numbered equally across processes.
                        }
                        else
                        {
                            retainedDOFIDs_final[i] = dofID;
                        }
                    }
                }
            }

            // //  Debug Parallel EQUAL Dofs
            // //
            // for ( int cur_proc = 1; cur_proc < numProcesses_world; ++cur_proc )
            // {
            //     MPI_Barrier( worker_comm );
            //     if ( cur_proc == processID_world )
            //     {
            //         cout << "(" << processID << ") @ MP # " << mpPtr->getTag()
            //              << " con = " << mpPtr->getNodeConstrained()
            //              << " ret = " << mpPtr->getNodeRetained()
            //              << " dofs = [ ";
            //         for (int i = 0; i < constrainedDOFs.Size(); i++)
            //         {
            //             cout << constrainedDOFs(i) << " ";
            //         }
            //         cout << "]  ";
            //         (nodeRetainedPtr != 0 ) ? cout << "Ret: Y | " : cout << "Ret: N | ";
            //         (nodeConstrainedPtr != 0 ) ? cout << "Con: Y " : cout << "Con: N";
            //         cout << " > retainedDOFIDs.Size() = " << retainedDOFIDs.Size() << endl;
            //         cout << "   MPI_Allgatherv - Exit status = " << status << endl;
            //         cout << "  -- Proc " << cur_proc << " info ---\n";
            //         for (int p = 0; p < numProcesses_world; p++)
            //         {
            //             cout << "   " << p << ": ";
            //             for (int i = 0; i < ParallelNumberer_MAXDOFS; i++)
            //             {
            //                 cout << retainedDOFIDs_array[ParallelNumberer_MAXDOFS * p + i] << " ";
            //             }
            //             cout << endl;
            //         }
            //     }
            // }


            if (constrainedDOFGrp != 0)
            {
                for (int i = 0; i < constrainedDOFs.Size(); i++)
                {
                    int dofC = constrainedDOFs(i);
                    int dofR = retainedDOFs(i);
                    int dofID = retainedDOFIDs_final[dofR];//retainedDOFIDs(dofR);
                    constrainedDOFGrp->setID(dofC, dofID);
                }
            }
        }

        // ID theMPnodeTags(numMP);
        // ID theMPdofCs(numMP);
        // ID theMPdofIDs(numMP);
        // theChannel->receiveID(0, 0, theMPnodeTags);
        // theChannel->receiveID(0, 0, theMPdofCs);
        // theChannel->receiveID(0, 0, theMPdofIDs);

        // for (int pos = 0; pos < theMPnodeTags.Size(); pos++)
        // {
        //     Node* nodePtr = theDomain->getNode(theMPnodeTags[pos]);
        //     if (nodePtr != 0)
        //     {
        //         DOF_Group* dof = nodePtr->getDOF_GroupPtr();
        //         dof->setID(theMPdofCs[pos], theMPdofIDs[pos]);
        //     }
        // }

        // {
        //     // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Sending ID\n";
        // }
        //theChannel->sendID(0, 0, numVertex);  //This creates an ID with size numVertex (ctor. ID::ID(int size) gets called. ) and sends that. It has no use.
    }

    // if main domain, collect graphs from all subdomains,
    // merge into 1, number this one, send to subdomains the
    // id containing dof tags & start id's.
    else
    {

        // for P0 domain determine original vertex and ref tags
        int numVertex = theGraph.getNumVertex();
        int numVertexP0 = numVertex;

        ID vertexTags(numVertex);
        ID vertexRefs(numVertex);
        Vertex* vertexPtr;
        int loc = 0;
        VertexIter& theVertices = theGraph.getVertices();

        while ((vertexPtr = theVertices()) != 0)
        {
            vertexTags[loc] = vertexPtr->getTag();
            vertexRefs[loc] = vertexPtr->getRef();
            loc++;
        }

        ID** theSubdomainIDs = new ID *[numChannels];
        FEM_ObjectBroker theBroker;

        // for each subdomain we receive graph, create an ID (to store
        // subdomain graph to merged graph vertex mapping and the final
        // subdoain graph vertex to startDOF mapping) and finally merge the
        // subdomain graph

        for (int j = 0; j < numChannels; j++)
        {
            Channel* theChannel = theChannels[j];
            Graph* theSubGraph = new Graph();

            /*
            static ID test(2); test(0) = processID; test(1) = 25;
            theChannel->sendID(0, 0, test);
            */

            // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Receiving graph from Channel " << theChannel->getTag() << "\n";
            theSubGraph->receiveSelf(0, *theChannel, theBroker);

            theSubdomainIDs[j] = new ID(theSubGraph->getNumVertex() * 2, theSubGraph->getNumVertex() * 2, -1);

            // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Merging graph received from Channel " << theChannel->getTag() << "\n";
            this->mergeSubGraph(theGraph, *theSubGraph, vertexTags, vertexRefs, *theSubdomainIDs[j]);

            delete theSubGraph;
        }

        // we use graph numberer if one was provided in constructor,
        // otherwise we number based on subdomains (all in subdomain 1 numbered first,
        // then  those in 2 not in 1 and so on till done.
        //    GraphNumberer *theNumberer = this->getGraphNumbererPtr();


        // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Numbering the big merged graph\n";
        ID* theOrderedRefs = new ID(theGraph.getNumVertex());
        ID* theOrderedRefsIndex = new ID(theGraph.getNumVertex(), theGraph.getNumVertex(), -1);  // Use the 'fill' constructor here to set all values to -1. Jose adds this array to speedup searching for the location of element in teh 'theOrderedRefs' array. Used to use the ID::getLocation() function which does linear search and is super slow.

        if (theNumberer != 0)
        {

            // use the supplied graph numberer to number the merged graph
            // cout << "ParallelNumberer::number() - Warning - using user-provided numberer. Untested.\n";

            *theOrderedRefs = theNumberer->number(theGraph, lastDOF);
            // cerr << "Must use the "


        }
        else
        {

            // assign numbers based on the subdomains

            int loc = 0;

            for (int l = 0; l < numChannels; l++)
            {
                const ID& theSubdomain = *theSubdomainIDs[l];
                int numVertexSubdomain = theSubdomain.Size() / 2;

                for (int i = 0; i < numVertexSubdomain; i++)
                {
                    int vertexTagMerged = theSubdomain(i + numVertexSubdomain);

                    //  int refTag = vertexRefs[vertexTags.getLocation(vertexTagMerged)];
                    // if (theOrderedRefs->getLocation(vertexTagMerged) == -1)
                    if ( (*theOrderedRefsIndex)[vertexTagMerged] == -1  )
                    {
                        (*theOrderedRefs)[loc++] = vertexTagMerged;
                        (*theOrderedRefsIndex)[vertexTagMerged] = loc;
                    }
                }
            }

            // now order those not yet ordered in p0
            for (int j = 0; j < numVertexP0; j++)
            {
                int refTagP0 = vertexTags[j];

                // if (theOrderedRefs->getLocation(refTagP0) == -1)
                if ((*theOrderedRefsIndex)[refTagP0] == -1)
                {
                    (*theOrderedRefs)[loc++] = refTagP0;
                    (*theOrderedRefsIndex)[refTagP0] = loc;
                }
            }

        }


        int count = 0;

        for (int i = 0; i < theOrderedRefs->Size(); i++)
        {
            int vertexTag = (*theOrderedRefs)(i);
            //      int vertexTag = vertexTags[vertexRefs.getLocation(tag)];
            Vertex* vertexPtr = theGraph.getVertexPtr(vertexTag);
            int numDOF = vertexPtr->getColor();
            vertexPtr->setTmp(count);
            count += numDOF;
        }

        if (theNumberer == 0)
        {
            delete theOrderedRefs;
            delete theOrderedRefsIndex;
        }

        // number own dof's
        // for (int i = 0; i < numVertexP0; i++  )
        // {
        //     int vertexTag = vertexTags(i);
        //     Vertex* vertexPtr = theGraph.getVertexPtr(vertexTag);

        //     int startID = vertexPtr->getTmp();
        //     int dofTag = vertexTag;
        //     DOF_Group* dofPtr;
        //     dofPtr = theModel->getDOF_GroupPtr(dofTag);

        //     if (dofPtr == 0)
        //     {
        //         cerr << "WARNING ParallelNumberer::numberDOF - 1 ";
        //         cerr << "DOF_Group (P0) " << dofTag << " not in AnalysisModel!\n";
        //         result = -4;
        //     }
        //     else
        //     {
        //         const ID& theDOFID = dofPtr->getID();
        //         int idSize = theDOFID.Size();

        //         for (int j = 0; j < idSize; j++)
        //             if (theDOFID(j) == -2 || theDOFID(j) == -3)
        //             {
        //                 dofPtr->setID(j, startID++);
        //             }
        //     }
        // }

        // For Equal-DOF constraints!
        // cout << "Looking for EQUALDOFS: ";
        // int numMP = 0;// = theDomain->getNumMPs();
        // MP_ConstraintIter& theMPs = theDomain->getMPs();
        // MP_Constraint* mpPtr;
        // while ((mpPtr = theMPs()) != 0 )
        // {
        //     const ID& constrainedDOFs = mpPtr->getConstrainedDOFs();
        //     numMP += constrainedDOFs.Size();
        // }

        // ID theMPnodeTags(numMP);
        // ID theMPdofCs(numMP);
        // ID theMPdofIDs(numMP);
        // int pos = 0;
        // for (int i = 0; i < numVertexP0; i++  )
        // {
        //     int have4s = 0;  //This is set to 1 if we hace some MP constraints


        //     int vertexTag = vertexTags(i);
        //     Vertex* vertexPtr = theGraph.getVertexPtr(vertexTag);

        //     int startID = vertexPtr->getTmp();
        //     int dofTag = vertexTag;
        //     DOF_Group* dofPtr;
        //     dofPtr = theModel->getDOF_GroupPtr(dofTag);

        //     if (dofPtr == 0)
        //     {
        //         cerr << "WARNING ParallelNumberer::numberDOF - 2 ";
        //         cerr << "DOF_Group (P0) " << dofTag << " not in AnalysisModel!\n";
        //         result = -4;
        //     }
        //     else
        //     {
        //         // const ID& theID = dofPtr->getID();
        //         const ID& theDOFID = dofPtr->getID();
        //         int idSize = theDOFID.Size();

        //         //Check whether there are -4s
        //         for (int j = 0; j < theDOFID.Size(); j++)
        //             if (theDOFID(j) == -4)
        //             {
        //                 have4s = 1;
        //                 break;
        //             }
        //         if (have4s == 1)
        //         {
        //             int nodeID = dofPtr->getNodeTag();

        //             // cout << "\nFound EQUALDOF! Node " << nodeID << " is slave where node ";

        //             // loop through the MP_Constraints to see if any of the
        //             // DOFs are constrained, note constraint matrix must be diagonal
        //             // with 1's on the diagonal
        //             MP_ConstraintIter& theMPs = theDomain->getMPs();
        //             MP_Constraint* mpPtr;

        //             while ((mpPtr = theMPs()) != 0 )
        //             {
        //                 // note keep looping over all in case multiple constraints
        //                 // are used to constrain a node -- can't assume intelli user
        //                 if (mpPtr->getNodeConstrained() == nodeID)
        //                 {
        //                     int nodeRetained = mpPtr->getNodeRetained();
        //                     Node* nodeRetainedPtr = theDomain->getNode(nodeRetained);
        //                     // cout << nodeRetainedPtr->getTag() << " is the master! DOF-List = [ ";
        //                     DOF_Group* retainedDOF = nodeRetainedPtr->getDOF_GroupPtr();
        //                     const ID& retainedDOFIDs = retainedDOF->getID();
        //                     const ID& constrainedDOFs = mpPtr->getConstrainedDOFs();
        //                     const ID& retainedDOFs = mpPtr->getRetainedDOFs();

        //                     for (int i = 0; i < constrainedDOFs.Size(); i++)
        //                     {
        //                         int dofC = constrainedDOFs(i);
        //                         int dofR = retainedDOFs(i);
        //                         int dofID = retainedDOFIDs(dofR);
        //                         dofPtr->setID(dofC, dofID);
        //                         theMPnodeTags(pos) = nodeID;
        //                         theMPdofCs(pos) = dofC;
        //                         theMPdofIDs(pos) = dofID;
        //                         pos++;
        //                         // cout << dofC << " ";
        //                     }
        //                     // cout << "]\n";
        //                 }
        //             }
        //             // cout << "\nLooking for more EQUALDOFS: ";
        //         }
        //         // else
        //         // {
        //         // cout << ".";
        //         // }
        //     }
        // }




        // now given the ordered refs we determine the mapping for each subdomain
        // and send the id with the information back to the subdomain, which it uses to order
        // it's own graph
        for (int k = 0; k < numChannels; k++)
        {
            Channel* theChannel = theChannels[k];
            ID& theSubdomain = *theSubdomainIDs[k];
            int numVertexSubdomain = theSubdomain.Size() / 2;

            for (int i = 0; i < numVertexSubdomain; i++)
            {
                int vertexTagMerged = theSubdomain[numVertexSubdomain + i];
                Vertex* vertexPtr = theGraph.getVertexPtr(vertexTagMerged);
                int startDOF = vertexPtr->getTmp();
                theSubdomain[i + numVertexSubdomain] = startDOF;
            }

            // cout << "        + ParallelNumberer::numberDO/F() [" << processID <<  "] : Sending channel " << theChannel->getTag() << " its startdof\n";
            // ID numMP_ID(1);
            // numMP_ID(0) = numMP;
            // theChannel->sendID(0, 0, numMP_ID);
            theChannel->sendID(0, 0, theSubdomain);

            //Also send these for the MP constraints
            // theChannel->sendID(0, 0, theMPnodeTags);
            // theChannel->sendID(0, 0, theMPdofCs);
            // theChannel->sendID(0, 0, theMPdofIDs);
            // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Waiting for channel " << theChannel->getTag() << " to be done.\n";
            // theChannel->receiveID(0, 0, theSubdomain);  // This receive was removed, as well as the corresponding send. They serve no purpose.
            delete theSubdomainIDs[k];
        }

        delete [] theSubdomainIDs;
    }

    // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : Setting FEs\n";
    // iterate through the FE_Element getting them to set their IDs
    FE_EleIter& theEle = theModel->getFEs();
    FE_Element* elePtr;

    while ((elePtr = theEle()) != 0)
    {
        elePtr->setID();
    }

    // cout << "        + ParallelNumberer::numberDOF() [" << processID <<  "] : End numbering!\n";
    return result;
#else
    cerr << "ParallelNumberer::numberDOF : This numberer should not be used in sequential case.\n";
    return -1;
#endif

}


int
ParallelNumberer::mergeSubGraph(Graph& theGraph, Graph& theSubGraph, ID& vertexTags, ID& vertexRefs, ID& theSubdomainMap)
{
    // for each vertex in the SubGraph we see if a vertex exists in the Graph which has the same
    // reference tag (Reference tag in the AnalysisModel graph is the node tag) .. if so this will be
    // the new vertex tag for SubGraph vertex in new graph, otherwise we assign it some new vertex tag,
    // create a vertex for this new vertex tag & add it to the graph

    Vertex* subVertexPtr;
    VertexIter& theSubGraphIter1 = theSubGraph.getVertices();
    int count = 0;
    int numVertex = theGraph.getNumVertex();
    int numVertexSub = theSubGraph.getNumVertex();

    //Build temporary indexes into vertexRefs and theSubfomainMaps for faster search
    // This is a linear complexity operation at the extra cost of (temporary) memory usage.
    // This is done only a couple of times (once for each subdomain apparently) as opposed to
    // the linear time search done in ID::getLocation() which is repeated in a loop an raises, uncesessarily, the
    //  complexity of the merging process.

    // cout << "Building vertexRefsIndex" << endl;

    int maxref = 0;
    for (int loc = 0; loc < vertexRefs.Size(); loc++)
    {
        if (maxref < vertexRefs[loc])
        {
            maxref = vertexRefs[loc];
        }
    }

    ID vertexRefsIndex(maxref, maxref, -1);
    for (int loc = 0; loc < vertexRefs.Size(); loc++)
    {
        vertexRefsIndex[vertexRefs[loc]] = loc;
    }

    // cout << "Merging vertices (maxref = " << maxref << ")" <<  endl;

    int max_vertextagsub = 0;
    while ((subVertexPtr = theSubGraphIter1()) != 0)
    {
        int vertexTagSub = subVertexPtr->getTag();
        int vertexTagRef = subVertexPtr->getRef();
        // int loc = vertexRefs.getLocation(vertexTagRef);
        int loc;
        if (vertexTagRef > maxref)
        {
            loc = -1;
        }
        else
        {
            loc = vertexRefsIndex[vertexTagRef];
        }

        int vertexTagMerged;

        // cout << "loc = " << loc << " vertexTagRef = " << vertexTagRef << endl;
        if (loc < 0)
        {
            // if not already in, we will be creating a new vertex
            vertexTagMerged = theGraph.getFreeTag();
            vertexTags[numVertex] = vertexTagMerged;
            vertexRefs[numVertex] = vertexTagRef;
            // vertexRefsIndex[vertexTagRef] = numVertex;  // Dont need to update the index... it wont be used...

            // Vertex* newVertex = new Vertex(vertexTagMerged, vertexTagRef, subVertexPtr->getWeight(), subVertexPtr->getColor());
            Vertex newVertex(vertexTagMerged, vertexTagRef, subVertexPtr->getWeight(), subVertexPtr->getColor());

            bool success = theGraph.addVertex(&newVertex);

            if (!success)
            {
                cerr << "Failed to add vertex \n";
                cerr << "   vertexTagMerged = " << vertexTagMerged << "\n";
                cerr << "      vertexTagRef = " << vertexTagRef << "\n";
                cerr << "            Weight = " << subVertexPtr->getWeight() << "\n";
                cerr << "             Color = " << subVertexPtr->getColor() << endl;

                return -1;

            }

            numVertex++;
        }
        else
        {
            vertexTagMerged = vertexTags[loc];
        }

        // use the subgraphs ID to hold the mapping of vertex numbers between merged and original
        if (max_vertextagsub < vertexTagSub)
        {
            max_vertextagsub = vertexTagSub;
        }

        theSubdomainMap[count] = vertexTagSub;
        theSubdomainMap[count + numVertexSub] = vertexTagMerged;
        count++;
    }

    // cout << "Building theSubdomainMapIndex" << endl;

    int N_theSubdomainMap = theSubdomainMap.Size() / 2;
    ID theSubdomainMapIndex(max_vertextagsub + 1, max_vertextagsub + 1, -1);
    for (int loc = 0; loc < N_theSubdomainMap; loc++)
    {
        theSubdomainMapIndex[theSubdomainMap[loc]] = loc;
    }


    // cout << "Merging edges" << endl;
    // for each vertex in subgraph, we add it's adjacenecy into the merged graph
    VertexIter & theSubGraphIter2 = theSubGraph.getVertices();

    while ((subVertexPtr = theSubGraphIter2()) != 0)
    {
        int vertexTagSub = subVertexPtr->getTag();
        // int loc = theSubdomainMap.getLocation(vertexTagSub);
        int loc;
        if (vertexTagSub <= max_vertextagsub)
        {
            loc = theSubdomainMapIndex[vertexTagSub];
        }
        else
        {
            loc = -1;
            cerr << "vertexTagSub = " << vertexTagSub << " not available!!\n";
            return -1;
        }

        int vertexTagMerged = theSubdomainMap[loc + numVertexSub];

        const ID& adjacency = subVertexPtr->getAdjacency();

        for (int i = 0; i < adjacency.Size(); i++)
        {
            int vertexTagSubAdjacent = adjacency(i);
            // int loc = theSubdomainMap.getLocation(vertexTagSubAdjacent);
            // int loc = theSubdomainMapIndex[vertexTagSubAdjacent];

            int loc;
            if (vertexTagSubAdjacent <= max_vertextagsub)
            {
                loc = theSubdomainMapIndex[vertexTagSubAdjacent];
            }
            else
            {
                loc = -1;
                cerr << "vertexTagSubAdjacent = " << vertexTagSubAdjacent << " not available!!\n";
                return -1;
            }


            int vertexTagMergedAdjacent = theSubdomainMap[loc + numVertexSub];
            theGraph.addEdge(vertexTagMerged, vertexTagMergedAdjacent);
        }
    }


    return 0;
}


int
ParallelNumberer::sendSelf(int cTag, Channel& theChannel)
{
    int sendID = 0;

    // if P0 check if already sent. If already sent use old processID; if not allocate a new process
    // id for remote part of object, enlarge channel * to hold a channel * for this remote object.

    // if not P0, send current processID

    if (processID == 0)
    {

        // check if already using this object
        bool found = false;

        for (int i = 0; i < numChannels; i++)
            if (theChannels[i] == &theChannel)
            {
                sendID = i + 1;
                found = true;
            }

        // if new object, enlarge Channel pointers to hold new channel * & allocate new ID
        if (found == false)
        {
            int nextNumChannels = numChannels + 1;
            Channel** nextChannels = new Channel *[nextNumChannels];

            if (nextNumChannels == 0)
            {
                cerr << "ParalellNumberer::sendSelf() - failed to allocate channel array of size: " <<
                     nextNumChannels << endln;
                return -1;
            }

            for (int i = 0; i < numChannels; i++)
            {
                nextChannels[i] = theChannels[i];
            }

            nextChannels[numChannels] = &theChannel;

            numChannels = nextNumChannels;

            if (theChannels != 0)
            {
                delete [] theChannels;
            }

            theChannels = nextChannels;

            // allocate new processID for remote object
            sendID = numChannels;
        }

    }
    else
    {
        sendID = processID;
    }


    // send remotes processID
    ID idData(1);
    idData(0) = sendID;

    int res = theChannel.sendID(0, cTag, idData);

    if (res < 0)
    {
        cerr << "WARNING DistributedSparseGenColLinSOE::sendSelf() - failed to send data\n";
        return -1;
    }

    return 0;
}

int
ParallelNumberer::receiveSelf(int cTag,
                              Channel& theChannel,
                              FEM_ObjectBroker& theBroker)
{
    ID idData(1);
    int res = theChannel.receiveID(0, cTag, idData);

    if (res < 0)
    {
        cerr << "WARNING Parallel::receiveSelf() - failed to send data\n";
        return -1;
    }

    processID = idData(0);

    numChannels = 1;
    theChannels = new Channel *[1];
    theChannels[0] = &theChannel;

    return 0;
}


int
ParallelNumberer::numberDOF(ID& lastDOFs)
{

    int result = 0;

    // get a pointer to the model & check its not null
    AnalysisModel* theModel = this->getAnalysisModelPtr();
    Domain* theDomain = 0;

    if (theModel != 0)
    {
        theDomain = theModel->getDomainPtr();
    }

    if (theModel == 0 || theDomain == 0)
    {
        cerr << "WARNING ParallelNumberer::numberDOF(int) -";
        cerr << " - no AnalysisModel - has setLinks() been invoked?\n";
        return -1;
    }

    Graph& theGraph = theModel->getDOFGroupGraph();

    // if subdomain, collect graph, send it off, get
    // ID back containing dof tags & start id numbers.
    if (processID != 0)
    {
        Channel* theChannel = theChannels[0];
        int numVertex = theGraph.getNumVertex();
        theGraph.sendSelf(0, *theChannel);
        ID theID(2 * numVertex);
        theChannel->receiveID(0, 0, theID);

        for (int i = 0; i < numVertex; i += 2)
        {
            int dofTag = theID(i);
            int startID = theID(i + 1);
            DOF_Group* dofPtr;
            dofPtr = theModel->getDOF_GroupPtr(dofTag);

            if (dofPtr == 0)
            {
                cerr << "WARNING ParallelNumberer::numberDOF - ";
                cerr << "DOF_Group " << dofTag << "not in AnalysisModel!\n";
                result = -4;
            }
            else
            {
                const ID& theID = dofPtr->getID();
                int idSize = theID.Size();

                for (int j = 0; j < idSize; j++)
                    if (theID(j) == -2)
                    {
                        dofPtr->setID(j, startID++);
                    }
            }
        }
    }

    // if main domain, collect graphs from all subdomains,
    // merge into 1, number this one, send to subdomains the
    // id containing dof tags & start id's.
    else
    {

        // determine original vertex and ref tags
        int numVertex = theGraph.getNumVertex();
        ID vertexTags(numVertex);
        ID vertexRefs(numVertex);
        Vertex* vertexPtr;
        int loc = 0;
        VertexIter& theVertices = theGraph.getVertices();

        while ((vertexPtr = theVertices()) != 0)
        {
            vertexTags[loc] = vertexPtr->getTag();
            vertexRefs[loc] = vertexPtr->getRef();
            loc++;
        }

        ID** theSubdomainIDs = new ID *[numChannels];
        FEM_ObjectBroker theBroker;

        // merge all subdomain graphs
        for (int j = 0; j < numChannels; j++)
        {
            Channel* theChannel = theChannels[j];
            Graph theSubGraph;
            theSubGraph.receiveSelf(0, *theChannel, theBroker);
            theSubdomainIDs[j] = new ID(theSubGraph.getNumVertex() * 2);
            this->mergeSubGraph(theGraph, theSubGraph, vertexTags, vertexRefs, *theSubdomainIDs[j]);
        }

        // number the merged graph
        //    result =  this->DOF_Numberer::number(theGraph);

        // send results of numbered back to subdomains
        for (int k = 0; k < numChannels; k++)
        {
            Channel* theChannel = theChannels[k];
            // this->determineSubIDs
            theChannel->sendID(0, 0, *theSubdomainIDs[k]);
            delete theSubdomainIDs[k];
        }

        delete [] theSubdomainIDs;

        // number own dof's
    }

    return result;
}
