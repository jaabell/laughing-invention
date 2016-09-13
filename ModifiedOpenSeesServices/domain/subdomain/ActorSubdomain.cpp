/* ****************************************************************** **
**    Pence's - Open System for Earthquake Engineering Simulation    **
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

// $Revision: 1.4 $
// $Date: 2003/11/18 01:59:04 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/subdomain/ActorSubdomain.cpp,v $



#include <ActorSubdomain.h>
#include <FEM_ObjectBroker.h>
#include <Element.h>
#include <Node.h>
#include <SP_Constraint.h>
#include <SP_ConstraintIter.h>
#include <MP_Constraint.h>
#include <ElementalLoad.h>
#include <NodalLoad.h>
#include <LoadPattern.h>
#include <NodalLoadIter.h>
#include <LoadPatternIter.h>
#include <ElementalLoadIter.h>
#include <Matrix.h>
#include <Vector.h>
#include <DomainDecompositionAnalysis.h>
#include <ConvergenceTest.h>

#include <EquiSolnAlgo.h>
#include <IncrementalIntegrator.h>
#include <LinearSOE.h>
#include <LinearSOESolver.h>

#include <ArrayOfTaggedObjects.h>
#include <ShadowActorSubdomain.h>

#include <Vertex.h>
#include <VertexIter.h>

#include <MPI_Channel.h>
#include <MPI_ChannelAddress.h>
#include <Channel.h>
#include <ChannelAddress.h>
#include <DOF_Group.h>

#include <iostream>
using namespace std;


ActorSubdomain::ActorSubdomain(Channel &theChannel,
                               FEM_ObjectBroker &theBroker)
    : Subdomain(0), Actor(theChannel, theBroker, 0),
      msgData(4), lastResponse(0)
#ifdef _PDD
    , theGraphPartitioner(0)
#endif
{
    // does nothing
#ifdef _PDD
    myChan = 0;
    ChangedNodes = 0;
#endif
}

ActorSubdomain::~ActorSubdomain()
{
    // does nothing
#ifdef _PDD
    if ( theGraphPartitioner != 0 )
    {
        delete theGraphPartitioner;
    }

    if ( ChangedNodes != 0 )
    {
        delete ChangedNodes;
    }

#endif
    // cerr << "ActorSubdomain DIED: " << endln;
}


int
ActorSubdomain::run(void)
{
    Vector theVect(4);
    bool exitYet = false;
    //Guanzhou out cerr << "ActorSubdomain running: " << endln;

    while (exitYet == false)
    {

        this->receiveID(msgData);
        int action = msgData(0);

        //cerr << "ActorSubdomain running action: " << action << endln; //Guanzhou added more comments

        int theType, theOtherType, tag, dbTag, loadPatternTag;
        Element *theEle;
        Node *theNod;
        //,                         * tmpNode;
        SP_Constraint                *theSP;
        MP_Constraint                *theMP;
        LoadPattern                  *theLoadPattern;
        NodalLoad                    *theNodalLoad;
        ElementalLoad                *theElementalLoad;
        DomainDecompositionAnalysis *theDDAnalysis;
        const Matrix                 *theMatrix;
        const Vector                 *theVector;
        // PartitionedModelBuilder  * theBuilder;
        IncrementalIntegrator        *theIntegrator;
        EquiSolnAlgo                 *theAlgorithm;
        LinearSOE                    *theSOE;
        LinearSOESolver              *theSolver;
        ConvergenceTest              *theTest;
        ID                           options(7);
        Vector                       dbl_options(4);
        int                           algorithm;
        double                        f_relative_tol;
        double                        stress_relative_tol;
        int                           n_max_iterations;
        bool                          res;
        DOF_Group *pDOF_Group;

        const ID *theID;

        switch (action)
        {

        case ShadowActorSubdomain_setTag:
            tag = msgData(1); // subdomain tag
            this->setTag(tag);
            this->Actor::setCommitTag(tag);

            break;

        case ShadowActorSubdomain_newStep:
            this->receiveVector(theVect);
            this->newStep(theVect(0));

            break;

        // case ShadowActorSubdomain_buildSubdomain:
        //     theType = msgData(1);
        //     tag = msgData(3); // subdomain tag
        //     this->setTag(tag);
        //     tag = msgData(2); // numSubdomains
        //     // theBuilder = theBroker->getPtrNewPartitionedModelBuilder(*this,
        //     // theType);
        //     this->receiveObject(*theBuilder);
        //     this->buildSubdomain(tag, *theBuilder);

        //     break;

        case ShadowActorSubdomain_getRemoteData:
            theID = &(this->getExternalNodes());
            msgData(0) = theID->Size();
            msgData(1) = this->getNumDOF();

            this->sendID(msgData);

            if (theID->Size() != 0)
            {
                this->sendID(*theID);
            }

            break;

        case ShadowActorSubdomain_getCost:
            theVect(0) = this->getCost(); // have to use [] for Sun's CC!
            this->sendVector(theVect);
            break;

        case ShadowActorSubdomain_addElement:
            theType = msgData(1);
            dbTag = msgData(2);

            theEle = theBroker->getNewElement(theType);

            if (theEle != 0)
            {
                theEle->setDbTag(dbTag);
                theEle->setMaterialTag(msgData(3));
                this->receiveObject(*theEle);
                bool result = this->addElement(theEle);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            /*
            this->receiveID(msgData);
            cerr << "ActorSubdomain::addElement() : " << msgData;

            msgData(0) = 1;
            msgData(1) = 2;
            msgData(2) = 3;
            msgData(3) = 4;
            this->sendID(msgData);
            */

            break;

        //Guanzhou added
        case ShadowActorSubdomain_getElementPtr:
            theEle = this->getElement(msgData(1));
            msgData(0) = theEle->getClassTag();
            this->sendID(msgData);
            this->sendObject(*theEle);
            break;

        //Guanzhou added
        case ShadowActorSubdomain_getNodePtr:
            theNod = this->getNode(msgData(1));
            //msgData(0) = theNod->getClassTag();
            //this->sendID(msgData);
            this->sendObject(*theNod);
            break;

        case ShadowActorSubdomain_hasNode:
            theType = msgData(1);
            res = this->hasNode(theType);

            if (res == true)
            {
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            break;

        case ShadowActorSubdomain_hasInternalNode:
            theType = msgData(1);
            res = this->hasInternalNode(theType);

            if (res == true)
            {
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            break;


        case ShadowActorSubdomain_hasExternalNode:
            theType = msgData(1);
            res = this->hasExternalNode(theType);

            if (res == true)
            {
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            break;


        case ShadowActorSubdomain_hasElement:
            theType = msgData(1);
            res = this->hasElement(theType);

            if (res == true)
            {
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            break;


        case ShadowActorSubdomain_addNode:
            theType = msgData(1);
            dbTag = msgData(2);
            theNod = theBroker->getNewNode(theType);

            if (theNod != 0)
            {
                theNod->setDbTag(dbTag);
                this->receiveObject(*theNod);
                bool result = this->addNode(theNod);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            //      cerr << "ActorSubdomain::add node: " << *theNod;
            break;





        case ShadowActorSubdomain_addExternalNode:
            theType = msgData(1);
            dbTag = msgData(2);
            theNod = theBroker->getNewNode(theType);

            if (theNod != 0)
            {
                theNod->setDbTag(dbTag);
                this->receiveObject(*theNod);
                bool result = this->addExternalNode(theNod);

                //delete theNod;
                /*
                Node *dummy = new Node(*theNod);
                delete theNod;
                cerr << *dummy;
                cerr << dummy->getMass();
                */

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;


        case ShadowActorSubdomain_addSP_Constraint:
            theType = msgData(1);
            dbTag = msgData(2);

            theSP = theBroker->getNewSP(theType);

            if (theSP != 0)
            {
                theSP->setDbTag(dbTag);
                this->receiveObject(*theSP);
                bool result = this->addSP_Constraint(theSP);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_addMP_Constraint:
            theType = msgData(1);
            dbTag = msgData(2);
            theMP = theBroker->getNewMP(theType);

            if (theMP != 0)
            {
                theMP->setDbTag(dbTag);
                this->receiveObject(*theMP);
                bool result = this->addMP_Constraint(theMP);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;


        case ShadowActorSubdomain_addLoadPattern:
            theType = msgData(1);
            dbTag = msgData(2);

            theLoadPattern = theBroker->getNewLoadPattern(theType);

            if (theLoadPattern != 0)
            {
                theLoadPattern->setDbTag(dbTag);
                this->receiveObject(*theLoadPattern);
                bool result = this->addLoadPattern(theLoadPattern);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_addNodalLoadToPattern:
            theType = msgData(1);
            dbTag = msgData(2);
            loadPatternTag = msgData(3);

            theNodalLoad = theBroker->getNewNodalLoad(theType);

            if (theNodalLoad != 0)
            {
                theNodalLoad->setDbTag(dbTag);
                this->receiveObject(*theNodalLoad);
                bool result = this->addNodalLoad(theNodalLoad, loadPatternTag);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;


        case ShadowActorSubdomain_addElementalLoadToPattern:
            theType = msgData(1);
            dbTag = msgData(2);
            loadPatternTag = msgData(3);

            theElementalLoad = theBroker->getNewElementalLoad(theType);

            if (theElementalLoad != 0)
            {
                theElementalLoad->setDbTag(dbTag);
                this->receiveObject(*theElementalLoad);
                bool result = this->addElementalLoad(theElementalLoad,
                                                     loadPatternTag);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_addSP_ConstraintToPattern:
            theType = msgData(1);
            dbTag = msgData(2);
            loadPatternTag = msgData(3);

            theSP = theBroker->getNewSP(theType);

            if (theSP != 0)
            {
                theSP->setDbTag(dbTag);
                this->receiveObject(*theSP);
                bool result = this->addSP_Constraint(theSP, loadPatternTag);

                if (result == true)
                {
                    msgData(0) = 0;
                }
                else
                {
                    msgData(0) = -1;
                }
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_removeElement:
            tag = msgData(1);

            theEle = this->removeElement(tag);

            if ( theEle != 0 )
            {
                delete theEle;
            }
            else
            {
                cerr << "ActorSubdomain::run() - failed to remove element!\n";
                exit(1);
            }

            //Note, Guanzhou took out these unnecessary communications!!!
            //GZ out if (theEle != 0)
            //GZ out    msgData(0) = theEle->getClassTag();
            //GZ out else
            //GZ out    msgData(0) = -1;
            //GZ out
            //GZ out this->sendID(msgData);
            //GZ out if (theEle != 0) {
            //GZ out    this->sendObject(*theEle);
            //GZ out    delete theEle;
            //GZ out }
            //GZ out
            //GZ out msgData(0) = 0;

            //out again if ( theEle != 0 ) msgData(0) = 0;
            //out again else msgData(0) = -1;
            //out again this->sendID(msgData);

            break;


        case ShadowActorSubdomain_removeNode:
            tag = msgData(1);

            theNod = this->removeNode(tag);

            if ( this->getNode(tag) != 0)
            {
                cerr << "ActorSubdomain::run - removeNode failed to remove node: "
                     << tag << "\n";
                exit(1);
            }

            //Note, Guanzhou took out these unnecessary communications!!!
            if (theNod != 0)
            {
                msgData(0) = 0;
                msgData(1) = theNod->getNumberDOF();
                delete theNod;
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);
            //GZ out if (theNod != 0) {
            //GZ out    this->sendObject(*theNod);
            //GZ out    delete theNod;
            //GZ out }
            //GZ out
            //GZ out msgData(0) = 0;

            break;

        //Guanzhou added
        case ShadowActorSubdomain_removeSP_Constraint:
            tag = msgData(1);

            theSP = this->removeSP_Constraint(tag);

            if (theSP != NULL)
            {
                delete theSP;
            }

            break;

        case ShadowActorSubdomain_removeMP_Constraint:
            tag = msgData(1);

            theMP = this->removeMP_Constraint(tag);

            if ( theMP != NULL )
            {
                delete theMP;
            }

            break;

        case ShadowActorSubdomain_removeLoadPattern:
            tag = msgData(1);

            theLoadPattern = this->removeLoadPattern(tag);

            if ( theLoadPattern != NULL )
            {
                delete theLoadPattern;
            }

            break;

        case ShadowActorSubdomain_removeNodalLoadFromPattern:
            tag = msgData(1);
            theType = msgData(2);

            theNodalLoad = this->removeNodalLoad(tag, theType);

            if ( theNodalLoad != NULL )
            {
                delete theNodalLoad;
            }

            break;

        case ShadowActorSubdomain_removeElementalLoadFromPattern:
            tag = msgData(1);
            theType = msgData(2);

            theElementalLoad = this->removeElementalLoad(tag, theType);

            if ( theElementalLoad != NULL )
            {
                delete theElementalLoad;
            }

            break;

        //Guanzhou added remove, reduced data migration
        case ShadowActorSubdomain_removeSP_ConstraintFromPattern:
            tag = msgData(1);
            theType = msgData(2);

            theSP = this->removeSP_Constraint(tag, theType);

            if ( theSP != NULL )
            {
                delete theSP;
            }

            break;

        case ShadowActorSubdomain_getElement:
            tag = msgData(1);

            theEle = this->getElement(tag);

            if (theEle != 0)
            {
                msgData(0) = theEle->getClassTag();
            }
            else
            {
                msgData(0) = -1;
            }

            this->sendID(msgData);

            if (theEle != 0)
            {
                this->sendObject(*theEle);
            }

            msgData(0) = 0;

            break;


        case ShadowActorSubdomain_Print:
            this->Print(cerr);
            this->sendID(msgData);

            break;

        case ShadowActorSubdomain_applyLoad:
            this->receiveVector(theVect);
            this->applyLoad(theVect(0));
            break;

        case ShadowActorSubdomain_setCommittedTime:
            this->receiveVector(theVect);
            this->setCommittedTime(theVect(0));
            this->setCurrentTime(theVect(0));
            break;

        case ShadowActorSubdomain_setLoadConstant:
            this->setLoadConstant();
            break;

        case ShadowActorSubdomain_update:
            this->update();
            break;

        case ShadowActorSubdomain_updateTimeDt:
            this->updateTimeDt();
            break;

        case ShadowActorSubdomain_computeNodalResponse:
            tag = msgData(1);

            if (lastResponse == 0)
            {
                lastResponse = new Vector(tag);
            }
            else if (lastResponse->Size() != tag)
            {
                delete lastResponse;
                lastResponse = new Vector(tag);
            }

            this->receiveVector(*lastResponse);
            this->computeNodalResponse();


        case ShadowActorSubdomain_commit:
            this->commit();
            break;

        /*************************************************************************************
        * Added by Sumeet 3rd September, 2016, to output converged step 
        * Call this function to output at every converged step and write in HDF5 file
        **************************************************************************************/
        case ShadowActorSubdomain_output_step:
            this->output_step();
            break;

        /*************************************************************************************
        * Added by Sumeet 3rd August, 2016, to output substep iteration steps for debugging 
        * The function commits at every substep i,e the trail displacements and trial element
        * output HDF5 Output file. It does not commit any displacements or element output
        **************************************************************************************/
        case ShadowActorSubdomain_output_iteration:
            this->receiveVector(theVect);
            this->output_iteration(theVect(0));

            break;

        case ShadowActorSubdomain_revertToLastCommit:
            this->revertToLastCommit();
            break;

        case ShadowActorSubdomain_revertToStart:
            this->revertToStart();
            break;

        case ShadowActorSubdomain_addRecorder:
            theType = msgData(1);
            // theRecorder = theBroker->getPtrNewRecorder(theType);

            // if (theRecorder != 0)
            // {
            //     this->receiveObject(*theRecorder);
            //     this->addRecorder(*theRecorder);
            // }

            break;

        case ShadowActorSubdomain_removeRecorders:
            // this->removeRecorders();
            break;


        case ShadowActorSubdomain_setDomainDecompAnalysis:
            theType = msgData(1);
            theDDAnalysis =
                theBroker->getNewDomainDecompAnalysis(theType, *this);

            if (theDDAnalysis != 0)
            {
                this->receiveObject(*theDDAnalysis);
                this->setDomainDecompAnalysis(*theDDAnalysis);
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            have_written_static_mesh_data = false;

            break;

        case ShadowActorSubdomain_setAnalysisAlgorithm:
            theType = msgData(1);
            theAlgorithm = theBroker->getNewEquiSolnAlgo(theType);

            if (theAlgorithm != 0)
            {
                this->receiveObject(*theAlgorithm);
                this->setAnalysisAlgorithm(*theAlgorithm);
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_setAnalysisIntegrator:
            theType = msgData(1);
            theIntegrator = theBroker->getNewIncrementalIntegrator(theType);

            if (theIntegrator != 0)
            {
                this->receiveObject(*theIntegrator);
                this->setAnalysisIntegrator(*theIntegrator);
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_setAnalysisLinearSOE:
            theType = msgData(1);
            theOtherType = msgData(2);
            theSOE = theBroker->getNewLinearSOE(theType, theOtherType);

            if (theSOE != 0)
            {
                this->receiveObject(*theSOE);
                theSolver = theSOE->getSolver();
                this->receiveObject(*theSolver);
                this->setAnalysisLinearSOE(*theSOE);
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_setAnalysisConvergenceTest:
            theType = msgData(1);
            theTest = theBroker->getNewConvergenceTest(theType);

            if (theTest != 0)
            {
                this->receiveObject(*theTest);
                this->setAnalysisConvergenceTest(*theTest);
                msgData(0) = 0;
            }
            else
            {
                msgData(0) = -1;
            }

            break;

        case ShadowActorSubdomain_domainChange:
            this->domainChange();

            tag = this->getNumDOF();

            if (lastResponse == 0)
            {
                lastResponse = new Vector(tag);
            }
            else if (lastResponse->Size() != tag)
            {
                delete lastResponse;
                lastResponse = new Vector(tag);
            }

            break;

        case ShadowActorSubdomain_clearAnalysis:
            this->clearAnalysis();
            break;

        /*
        case 50:
          const Matrix *theMatrix1 = &(this->getStiff());
          this->sendMatrix(*theMatrix1);
          break;

        case 51:
          const Matrix *theMatrix2 = &(this->getDamp());
          this->sendMatrix(*theMatrix2);
          break;

        case 52:
          const Matrix *theMatrix3 = &(this->getMass());
          this->sendMatrix(*theMatrix3);
          break;
          */
        case  ShadowActorSubdomain_getTang:
            theMatrix = &(this->getTang());
            this->sendMatrix(*theMatrix);
            break;

        case ShadowActorSubdomain_getResistingForce:
            theVector = &(this->getResistingForce());
            this->sendVector(*theVector);
            break;

        case ShadowActorSubdomain_computeTang:
            tag = msgData(1);
            this->setTag(tag);
            this->computeTang();
            break;


        case ShadowActorSubdomain_computeResidual:
            this->computeResidual();
            break;

        case ShadowActorSubdomain_DIE:
            exitYet = true;
            break;


        case ShadowActorSubdomain_resetSubMultipleSupport:
            loadPatternTag = msgData(1);
            this->resetSubMultipleSupport(loadPatternTag);
            break;

        case ShadowActorSubdomain_swapNodeFromInternalToExternal:
            theNod = this->removeNode(msgData(1));

            if ( this->getNode(theNod->getTag()) != 0)
            {
                cerr << "ActorSubdomain::run() - swapNodeFromInternalToExternal failed to remove node "
                     << theNod->getTag() << "\n";
                exit(1);
            }

            pDOF_Group = theNod->getDOF_GroupPtr();

            if (pDOF_Group != 0)
            {
                pDOF_Group->unSetMyNode();
            }

            this->addExternalNode(theNod);

            //tmpNode =
            //this->Subdomain::addExternalNode(theNod);

            if ( this->getNode(theNod->getTag()) == 0)
            {
                cerr << "ActorSubdomain::run() - swapNodeFromInternalToExternal failed to add external node "
                     << theNod->getTag() << "\n";
                exit(1);
            }

            //delete theNod;

            //send it back to master!
            msgData(1) = theNod->getClassTag();
            msgData(2) = theNod->getDbTag();
            this->sendID(msgData);
            this->sendObject(*theNod);
            //delete theNod;
            break;


        case ShadowActorSubdomain_Partition: //Guanzhou added
            this->partition(msgData(1));
            break;

        case ShadowActorSubdomain_BuildElementGraph:
            this->getElementGraph();
            break;

        case ShadowActorSubdomain_Repartition:
            repartition(msgData(1));
            break;

        case ShadowActorSubdomain_reDistributeData:
            reDistributeData(msgData(1));
            break;

        case ShadowActorSubdomain_recvChangedNodeList:
            if ( ChangedNodes != 0 )
            {
                delete ChangedNodes;
            }

            ChangedNodes = new ID(msgData(1));
            this->receiveID(*ChangedNodes);
            break;

        case ShadowActorSubdomain_ChangeMPIChannel:
            setNextChannelAddress(msgData(1));
            break;

        case ShadowActorSubdomain_restoreChannel:
            setNextChannelAddress(0);
            break;


        case ShadowActorSubdomain_swapNodeFromExternalToInternal:
            theNod = this->removeNode(msgData(1));

            if ( this->getNode(theNod->getTag()) != 0)
            {
                cerr << "ActorSubdomain::run() - swapNodeFromExternalToInternal failed to remove node "
                     << theNod->getTag() << "\n";
                exit(1);
            }

            //this->Subdomain::addInternalNode(theNod);
            //delete theNod;

            pDOF_Group = theNod->getDOF_GroupPtr();

            if (pDOF_Group != 0)
            {
                pDOF_Group->unSetMyNode();
            }

            this->addNode(theNod);

            break;

        case ShadowActorSubdomain_exportInternalNode:
            exportInternalNode(msgData(1), msgData(2));
            break;

        case ShadowActorSubdomain_resetRecorders:
            // resetRecorders3();
            break;

        case ShadowActorSubdomain_sendOutputOptions:
            this->receiveID(options);
            output_is_enabled             = options(0) ;
            element_output_is_enabled     = options(1) ;
            have_written_static_mesh_data = options(2) ;
            output_every_nsteps           = options(3) ;
            countdown_til_output          = options(4) ;

            this->enableOutput(output_is_enabled);
            this->enableElementOutput(element_output_is_enabled);
            this->setOutputEveryNsteps(output_every_nsteps);
            this->setNumberOfOutputSteps(options(5));
            this->setOutputCompressionLevel(options(6));

            break;

        case ShadowActorSubdomain_setConstitutiveIntegrationMethod:
            // dbl_options(4);

            this->receiveVector(dbl_options);
            algorithm             = dbl_options(0) ;
            f_relative_tol     = dbl_options(1) ;
            stress_relative_tol = dbl_options(2) ;
            n_max_iterations           = dbl_options(3) ;
            // cout << "  + ActorSubdomain # " << this->getTag() << " got\n";
            // cout << "    algorithm = " << algorithm << endl;
            // cout << "    f_relative_tol = " << f_relative_tol << endl;
            // cout << "    stress_relative_tol = " << stress_relative_tol << endl;
            // cout << "    n_max_iterations = " << n_max_iterations << endl;

            Domain::setConstitutiveIntegrationMethod( algorithm,
                    f_relative_tol,  stress_relative_tol,  n_max_iterations);

            break;

        default:
            cerr << "ActorSubdomain::invalid action " << action << "received\n";
            msgData(0) = -1;
        }

        //  cerr << "ACTORSUBDOMAIN: FINISHED ACTION" << action << endln;
    }

    return 0;
}



const Vector &
ActorSubdomain::getLastExternalSysResponse(void)
{
    int numDOF = this->getNumDOF();
    numDOF = this->getNumDOF();

    if (lastResponse == 0)
    {
        lastResponse = new Vector(numDOF);
    }
    else if (lastResponse->Size() != numDOF)
    {
        delete lastResponse;
        lastResponse = new Vector(numDOF);
    }

    if (mapBuilt == false)
    {
        this->buildMap();
    }

    ID &theMap = *map;
    Vector &localResponse = *lastResponse;
    int numberDOF = this->getNumDOF();

    for (int i = 0; i < numberDOF; i++)
    {
        (*mappedVect)(theMap(i)) = localResponse(i);
    }

    return *mappedVect;

}

int
ActorSubdomain::update(void)
{
    int res = this->Domain::update();

    this->barrierCheck(res);

    return res;
}

int
ActorSubdomain::updateTimeDt(void)
{
    static Vector data(2);

    this->receiveVector(data);

    double newTime = data(0);
    double dT = data(1);
    int res = this->Domain::update(newTime, dT);
    return this->barrierCheck(res);
}

int
ActorSubdomain::barrierCheck(int myResult)
{
    static ID data(1);
    data(0) = myResult;
    this->sendID(data);
    this->receiveID(data);

    return data(0);
}

#ifdef _PDD

int
ActorSubdomain::setNextChannelAddress(const int other)
{
    //ChannelAddress *theChannelAddress = this->getShadowsAddressPtr();
    //MPI_ChannelAddress * newAddress = new MPI_ChannelAddress(other);
    MPI_ChannelAddress newAddress(other);
    //*theChannelAddress = newAddress;
    myChan = (MPI_Channel *)this->getChannelPtr();
    myChan->setNextAddress(newAddress);
    //delete newAddress;
    return 0;
}

int
ActorSubdomain::partition(int numParts)
{
    //return 0; //shadow subdomain's responsibility
    theGraphPartitioner = new ParMetis;
    //DomainPartitioner theDomainPartitioner = new DomainPartitioner(*theGraphPartitioner);
    //this->setPartitioner(theDomainPartitioner);

    Graph *myEleGraph = this->getElementGraph();

    //theDomainPartitioner->setPartitionedDomain(*this);
    return theGraphPartitioner->partition(myEleGraph, numParts);

}

int
ActorSubdomain::repartition(int numParts)
{
    myEleGraph = this->getElementGraph();

    if ( &myEleGraph == 0 )
    {
        cerr << "Subdomain::repartition, failed to get eleGraph!\n";
        return -1;
    }

    int Error = theGraphPartitioner->repartition(myEleGraph, numParts);

    if ( Error < 0 )
    {
        cerr << "Subdomain::repartition, failed to repartition the eleGraph!\n";
        return -1;
    }

    return 0;
}

int
ActorSubdomain::reDistributeData(int numParts)
{
    const int mype = this->getTag();

    LoadPatternIter &LPs = this->getLoadPatterns();
    LoadPattern *LP;

    while ((LP = LPs()) != 0)
    {

        int loadPatternTag = LP->getTag();

        NodalLoadIter &theNodalLoads = LP->getNodalLoads();
        NodalLoad *theNodalLoad;

        while ((theNodalLoad = theNodalLoads()) != 0)
        {
            const int loadedNodeTag = theNodalLoad->getNodeTag();
            //if ( (*nodePlace(loadedNodeTag)) ) == (*newNodePlace)(loadedNodeTag)) continue;
            bool changed = false;
            const int numChangedNodes = ChangedNodes->Size();

            for (int i = 0; i < numChangedNodes; i++)
            {
                if ( loadedNodeTag == (*ChangedNodes)(i))
                {
                    changed = true;
                    break;
                }
            }

            if ( !changed )
            {
                continue;
            }

            if ( this->hasNode(loadedNodeTag) )
            {
                if ( !this->hasInternalNode(loadedNodeTag) )   //external
                {
                    LP->removeNodalLoad(theNodalLoad->getTag());
                    msgData(0) = Master_addNodalLoadToPattern;
                    msgData(1) = theNodalLoad->getClassTag();
                    msgData(2) = theNodalLoad->getDbTag();
                    msgData(3) = loadPatternTag;
                    this->sendID(msgData);
                    this->sendObject(*theNodalLoad);
                    //this->receiveID(msgData);
                    //if ( msgData(0) != 0) {
                    //  cerr << "ActorSubdomain::reDistributeData(), master failed to add nodal load into load pattern!\n";
                    //  return -1;
                    //}
                    //} else {//internal
                    //  break;
                }
            }
            else    //not in this subdomain anymore, must be internal to one other subdomain, or external to some other subdomains
            {
                bool finished = false;
                LP->removeNodalLoad(theNodalLoad->getTag());

                //bool exported = false;
                for (int i = 1; i <= numParts; i++)
                {
                    //const int mype = this->getTag();
                    if ( i != mype )
                    {
                        MPI_ChannelAddress other(i);
                        msgData(0) = ShadowActorSubdomain_hasInternalNode;
                        msgData(1) = loadedNodeTag;
                        this->sendID(msgData, &other);
                        this->receiveID(msgData, &other);
                        setNextChannelAddress(0);//backup master channel

                        if (msgData(0) == 0)  //internal!
                        {
                            msgData(0) = ShadowActorSubdomain_addNodalLoadToPattern;
                            msgData(1) = theNodalLoad->getClassTag();
                            msgData(2) = theNodalLoad->getDbTag();
                            msgData(3) = loadPatternTag;
                            this->sendID(msgData, &other);
                            this->sendObject(*theNodalLoad, &other);
                            setNextChannelAddress(0);//backup master channel
                            finished = true;
                        }
                        else
                        {
                            msgData(0) = ShadowActorSubdomain_hasExternalNode;
                            msgData(1) = loadedNodeTag;
                            this->sendID(msgData, &other);
                            this->receiveID(msgData, &other);
                            setNextChannelAddress(0);//backup master channel

                            if (msgData(0) == 0)  //external!
                            {
                                msgData(0) = Master_addNodalLoadToPattern;
                                msgData(1) = theNodalLoad->getClassTag();
                                msgData(2) = theNodalLoad->getDbTag();
                                msgData(3) = loadPatternTag;
                                //setNextChannelAddress(0);//backup master channel
                                this->sendID(msgData);
                                this->sendObject(*theNodalLoad);
                                //this->receiveID(msgData);
                                //if ( msgData(0) != 0) {
                                //  cerr << "ActorSubdomain::reDistributeData(), master failed to add nodal load into load pattern!\n";
                                //  return -1;
                                //}
                                finished = true;
                            }
                        }
                    }
                }

                if ( !finished )
                {
                    cerr << "AvtorSubdomain::reDistributeData, failed moving nodal loads in load patterns!\n";
                    return -1;
                }
            }
        }

        SP_ConstraintIter &theSPs = LP->getSPs();
        SP_Constraint *spPtr;

        while ((spPtr = theSPs()) != 0)
        {
            const int constrainedNodeTag = spPtr->getNodeTag();

            if ( this->hasNode(constrainedNodeTag) )
            {
                if ( !this->hasInternalNode(constrainedNodeTag) )   //external
                {
                    LP->removeSP_Constraint(spPtr->getTag());
                    msgData(0) = Master_addSP_ConstraintToPattern;
                    msgData(1) = spPtr->getClassTag();
                    msgData(2) = spPtr->getDbTag();
                    msgData(3) = loadPatternTag;
                    this->sendID(msgData);
                    this->sendObject(*spPtr);
                    //this->receiveID(msgData);
                    //if ( msgData(0) != 0) {
                    //  cerr << "ActorSubdomain::reDistributeData(), master failed to add SP into load pattern!\n";
                    //  return -1;
                    //}
                    //} else {//internal
                    //  break;
                }
            }
            else    //not in this subdomain anymore, must be internal to one other subdomain, or external to some other subdomains
            {
                bool finished = false;
                LP->removeSP_Constraint(spPtr->getTag());

                //bool exported = false;
                for (int i = 1; i <= numParts; i++)
                {
                    //const int mype = this->getTag();
                    if ( i != mype )
                    {
                        MPI_ChannelAddress other(i);
                        msgData(0) = ShadowActorSubdomain_hasInternalNode;
                        msgData(1) = constrainedNodeTag;
                        this->sendID(msgData, &other);
                        this->receiveID(msgData, &other);
                        setNextChannelAddress(0);//backup master channel

                        if (msgData(0) == 0)  //internal!
                        {
                            msgData(0) = ShadowActorSubdomain_addSP_ConstraintToPattern;
                            msgData(1) = spPtr->getClassTag();
                            msgData(2) = spPtr->getDbTag();
                            msgData(3) = loadPatternTag;
                            this->sendID(msgData, &other);
                            this->sendObject(*spPtr, &other);
                            setNextChannelAddress(0);//backup master channel
                            finished = true;
                        }
                        else
                        {
                            msgData(0) = ShadowActorSubdomain_hasExternalNode;
                            msgData(1) = constrainedNodeTag;
                            this->sendID(msgData, &other);
                            this->receiveID(msgData, &other);
                            setNextChannelAddress(0);//backup master channel

                            if (msgData(0) == 0)  //external!
                            {
                                msgData(0) = Master_addSP_ConstraintToPattern;
                                msgData(1) = spPtr->getClassTag();
                                msgData(2) = spPtr->getDbTag();
                                msgData(3) = loadPatternTag;
                                this->sendID(msgData);
                                this->sendObject(*spPtr);
                                //this->receiveID(msgData);
                                //if ( msgData(0) != 0) {
                                //  cerr << "ActorSubdomain::reDistributeData(), master failed to add nodal load into load pattern!\n";
                                //  return -1;
                                //}
                                finished = true;
                            }
                        }
                    }
                }

                if ( !finished )
                {
                    cerr << "AvtorSubdomain::reDistributeData, failed moving SPs in load patterns!\n";
                    return -1;
                }
            }
        }
    }


    VertexIter &theVertices = myEleGraph->getVertices();
    Vertex *vertexPtr;
    Element *elePtr;

    while ((vertexPtr = theVertices()) != 0)
    {
        // move the element
        int partition = vertexPtr->getColor();
        int eleTag = vertexPtr->getRef();

        if ( partition == mype )
        {
            continue;    //quick return
        }

        //if ( elePtr != 0 ) delete elePtr;
        elePtr = this->removeElement(eleTag);

        if ( elePtr == 0 )
        {
            cerr << "Actorsubdomain::reDistributeData, error when remove element from subdomain!\n";
            return -1;
        }

        MPI_ChannelAddress other(partition);
        msgData(0) = ShadowActorSubdomain_addElement;
        msgData(1) = elePtr->getClassTag();
        msgData(2) = elePtr->getDbTag();
        this->sendID(msgData, &other);
        this->sendObject(*elePtr, &other);
        //this->receiveID(msgData, &other);
        setNextChannelAddress(0);//backup master channel
        //if ( msgData(0) != 0) {
        //  cerr << "Actorsubdomain::reDistributeData, failed to add element to subdomain!\n";
        //  return -1;
        //}

        /*********** ELEMENT LOADS ***************************************/
        // if any corresponding elemental loads in the load patterns .. move the load as well
        LoadPatternIter &theLoadPatterns = this->getLoadPatterns();
        LoadPattern *theLoadPattern;

        //ElementalLoad *theLoad;
        while ((theLoadPattern = theLoadPatterns()) != 0)
        {
            int loadPatternTag = theLoadPattern->getTag();
            ElementalLoadIter &theLoads = theLoadPattern->getElementalLoads();
            ElementalLoad *theLoad;

            while ((theLoad = theLoads()) != 0)
            {
                //cerr << "DomainPartitioner::partition - REMOVE ELEMENTAL LOADS\n";
                //if (theLoad->getElementTag() == eleTag)
                //  theLoadPattern->removeElementalLoad(theLoad->getTag());
                //theSubdomain->addElementalLoad(theLoad, loadPatternTag);
                if ( theLoad->hasElement(eleTag) )
                {
                    theLoadPattern->removeElementalLoad(theLoad->getTag());
                    MPI_ChannelAddress other(partition);
                    msgData(0) = ShadowActorSubdomain_addElementalLoadToPattern;
                    msgData(1) = theLoad->getClassTag();
                    msgData(2) = theLoad->getDbTag();
                    msgData(3) = loadPatternTag;
                    this->sendID(msgData, &other);
                    this->sendObject(*theLoad, &other);
                    setNextChannelAddress(0);
                }
            }
        }

    }


    SP_ConstraintIter &theSP1s = this->getSPs();
    SP_Constraint *spPtr1;

    while ((spPtr1 = theSP1s()) != 0)
    {
        const int constrainedNodeTag = spPtr1->getNodeTag();

        if ( !this->hasNode(constrainedNodeTag) )
        {
            this->removeSP_Constraint(spPtr1->getTag());
        }

        for (int i = 1; i <= numParts; i++)
        {
            if ( i != mype )
            {
                MPI_ChannelAddress other(i);
                msgData(0) = ShadowActorSubdomain_hasNode;
                msgData(1) = constrainedNodeTag;
                this->sendID(msgData, &other);
                this->receiveID(msgData, &other);

                if (msgData(0) == 0)  //has node!
                {
                    msgData(0) = ShadowActorSubdomain_addSP_Constraint;
                    msgData(1) = spPtr1->getClassTag();
                    msgData(2) = spPtr1->getDbTag();
                    this->sendID(msgData, &other);
                    this->sendObject(*spPtr1, &other);
                }
            }
        }

        setNextChannelAddress(0);//backup master channel
        msgData(0) = Master_hasNode;
        msgData(1) = constrainedNodeTag;
        this->sendID(msgData);
        this->receiveID(msgData);

        if (msgData(0) == 0)  //has node!
        {
            msgData(0) = Master_addSP_Constraint;
            msgData(1) = spPtr1->getClassTag();
            msgData(2) = spPtr1->getDbTag();
            this->sendID(msgData);
            this->sendObject(*spPtr1);
        }
    }

    for (int i = 1; i <= numParts; i++)
    {
        if ( i != mype )
        {
            MPI_ChannelAddress other(i);
            msgData(0) = ShadowActorSubdomain_restoreChannel;//backup master channels on all other subdomains
            this->sendID(msgData, &other);
        }
    }

    setNextChannelAddress(0);
    msgData(0) = Subdomain_Redistribution_DONE;
    this->sendID(msgData);

    //for (int i = 0; i < numRecorders; i++) {
    //  theRecorders[i]->initialize();
    //}

    this->resetDomainElements();//Guanzhou added to reflect changes caused by node movement!

    return 0;

}

int
ActorSubdomain::exportInternalNode(int nodeTag, int destination)
{
    Node *nodePtr = this->removeNode(nodeTag);

    if ( nodePtr == 0 )
    {
        cerr << "ActorSubdomain::exportInternalNode, failed to get the internal node pointer!\n";
        return -1;
    }

    if ( destination == 0 )
    {
        //msgData(0) = ShadowActorSubdomain_addNode;
        msgData(1) = nodePtr->getClassTag();
        msgData(2) = nodePtr->getDbTag();
        this->sendID(msgData);
        this->sendObject(*nodePtr);
        return 0;
    }

    MPI_ChannelAddress dest(destination);
    msgData(0) = ShadowActorSubdomain_addNode;
    msgData(1) = nodePtr->getClassTag();
    msgData(2) = nodePtr->getDbTag();
    this->sendID(msgData, &dest);
    this->sendObject(*nodePtr, &dest);

    msgData(0) = ShadowActorSubdomain_restoreChannel;
    this->sendID(msgData, &dest);//now backup the channel in destination subdomain!
    //this->restoreChannel();
    setNextChannelAddress(0);//backup master channel

    if ( nodePtr != 0 )
    {
        delete nodePtr;
    }

    return 0;
}

// int
// ActorSubdomain::resetRecorders(void)
// {
//     for (int i = 0; i < numRecorders; i++)
//     {
//         theRecorders[i]->initialize();
//     }
// }
#endif








