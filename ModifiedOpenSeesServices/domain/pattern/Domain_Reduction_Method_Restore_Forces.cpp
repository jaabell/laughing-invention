//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Plastic Bowl (aka Domain Reduction) implementation:
//#                    This file contains the class definition for Domain_Reduction_Method_Restore_Forces.
//#                    Domain_Reduction_Method_Restore_Forces is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# CLASS:             Domain_Reduction_Method_Restore_Forces
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Nima Tafazzoli , Boris Jeremic
//# PROGRAMMER(S):     Nima Tafazzoli , Boris Jeremic
//# Modified Version
//#              of:   PBowlLoading and Domain_Reduction_Method written by:
//#                    Jinxiu Liao, Zhaohui Yang, Nima Tafazzoli, Boris Jeremic
//#
//#
//# DATE:              April 2013
//# UPDATE HISTORY:
//#
//#
//===============================================================================


// Purpose: This file contains the class definition for Domain_Reduction_Method_Restore_Forces.
// Domain_Reduction_Method_Restore_Forces is an subclass of loadPattern.

#include <Domain_Reduction_Method_Restore_Forces.h>
#include <math.h>

#define numNodeDOF 3
#include <iostream>
using namespace std;


#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

Domain_Reduction_Method_Restore_Forces::Domain_Reduction_Method_Restore_Forces()
    : LoadPattern(0, PATTERN_TAG_Domain_Reduction_Method_Restore_Forces),
      PBowlElements(0),
      ExteriorNodes(0),
      BoundaryNodes(0),
      NodeID(0),
      PBowlLoads(0)
{
    LoadComputed = false;
}

Domain_Reduction_Method_Restore_Forces::Domain_Reduction_Method_Restore_Forces(int tag,
        const char* PBEfName,
        const char* PBNfName,
        const char* ForcefName,
        double theTimeIncr,
        double theFactor,
        int timeSteps,
        int Number_of_DRM_Elements,
        int Number_of_DRM_Nodes,
        double xplus,
        double xminus,
        double yplus,
        double yminus,
        double zplus,
        double zminus)
    : LoadPattern(tag,
                  PATTERN_TAG_Domain_Reduction_Method_Restore_Forces),
    PBTimeIncr(theTimeIncr),
    cFactor(theFactor),
    thetimeSteps(timeSteps),
    number_of_DRM_elements(Number_of_DRM_Elements),
    number_of_DRM_nodes(Number_of_DRM_Nodes),
    xPlus(xplus),
    xMinus(xminus),
    yPlus(yplus),
    yMinus(yminus),
    zPlus(zplus),
    zMinus(zminus),
    DRMForceFile(strdup(ForcefName))
{
    // determine the number of data points .. open file and count num entries

    int numDataPoints = 0;
    double dataPoint;
    int eleID;



    ifstream theFileELE;
    ifstream theFileNOD;

    //--------------------------------
    //Adding plastic bowl elements
    //from file to PBowlElements
    //--------------------------------

    theFileELE.open(PBEfName);
    numDataPoints = 0;

    if (!theFileELE)
    {
        cerr << "\nWARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << PBEfName << endln;
        exit(2);
    }
    else
    {


        // create a vector and read in the data

        // now create the vector
        PBowlElements = new ID(number_of_DRM_elements);


        // ensure we did not run out of memory
        if (PBowlElements == 0 || PBowlElements->Size() == 0 )
        {
            cerr << "Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces() - ran out of memory constructing";
            cerr << " a ID of size: " << PBowlElements->Size() << endln;

            if (PBowlElements != 0)
            {
                delete PBowlElements;
            }

            PBowlElements = 0;
        }

        // read the data from the file
        else
        {
            int i;

            for (i = 0; i < number_of_DRM_elements; i++)
            {
                theFileELE >> eleID;
                (*PBowlElements)(i) = eleID;
                numDataPoints++;
            }
        }

        // finally close the file
        theFileELE.close();


        //Check if read in correctly
        cerr << "Number of DRM Elements: " << number_of_DRM_elements << endln;
        //        cerr << "# of Actual DRM Elements: " << numDataPoints << endln;
        //cerr <<  (*PBowlElements);

    }



    //--------------------------------
    //Adding plastic bowl nodes
    //from file to PBowlElements
    //--------------------------------

    theFileNOD.open(PBNfName);
    numDataPoints = 0;

    if (!theFileNOD)
    {
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << PBNfName << endln;
        exit(2);
    }
    else
    {

        // create a vector and read in the data

        // now create the vector
        NodeID = new ID(number_of_DRM_nodes);

        if ( NodeID == 0 )
        {
            cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
            cerr << " Error allocating memory for NodeID! " << endln;
            exit(0);
        }


        // ensure we did not run out of memory
        if (NodeID == 0 || NodeID->Size() == 0 )
        {
            cerr << "Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces() - ran out of memory constructing";
            cerr << " a ID of size: " << NodeID->Size() << endln;

            if (NodeID != 0)
            {
                delete NodeID;
            }

            NodeID = 0;
        }

        // read the data from the file
        else
        {
            for (int m = 0; m < number_of_DRM_nodes; m++)
            {
                theFileNOD >> (*NodeID)(m);
                numDataPoints++;
            }
        }

        // finally close the file
        theFileNOD.close();


        //Check if read in correctly
        cerr << "Number of DRM Nodes: " << number_of_DRM_nodes << endln;
        //        cerr << "# of Actual DRM Nodes: " << numDataPoints << endln;
        //cerr <<  (*PBowlElements);

    }



    cerr << "Finished creating Domain_Reduction_Method_Restore_Forces!\n" ;

}



Domain_Reduction_Method_Restore_Forces::~Domain_Reduction_Method_Restore_Forces()
{
    // invoke the destructor on all ground motions supplied

    if ( PBowlElements != 0)
    {
        delete PBowlElements;
    }

    if ( ExteriorNodes != 0)
    {
        delete ExteriorNodes;
    }

    if ( BoundaryNodes != 0)
    {
        delete BoundaryNodes;
    }

    if ( PBowlLoads != 0)
    {
        delete PBowlLoads;
    }

    if ( NodeID != 0 )
    {
        delete NodeID;
    }


}


void
Domain_Reduction_Method_Restore_Forces::setDomain(Domain* theDomain)
{
    this->LoadPattern::setDomain(theDomain);


    if ( !LoadComputed )
    {
        this->CompPBLoads();
    }

}


void
Domain_Reduction_Method_Restore_Forces::applyLoad(double time)
{

    Domain* theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }


    // see if quick return, i.e. no plastic bowl nodes or domain set
    int numBnodes = BoundaryNodes->Size();
    int numEnodes = ExteriorNodes->Size();

    if ( (numBnodes + numEnodes) == 0)
    {
        return;
    }

    //Apply loads on each boundary bowl nodes
    Node* theNode;
    Vector* load;
    int i;

    for (i = 0; i < numBnodes; i++)
    {
        # ifdef _PARALLEL_PROCESSING

        if ( !theDomain->hasInternalNode((*BoundaryNodes)[i]) &&
                (theDomain->Domain::getNode( (*BoundaryNodes)[i] ) == 0) )
        {
            continue;
        }

        # endif
        theNode = theDomain->getNode( (*BoundaryNodes)[i] );

        if ( theNode == 0 )
        {
            continue;
        }

        load = this->getNodalLoad((*BoundaryNodes)[i], time);
        //Take care of the minus sign in the effective seismic force for boundary nodes
        (*load) = (*load) * (-1.0);
        theNode->addUnbalancedLoad(*load);
        delete load;
    }

    //Apply loads on each exterior bowl nodes
    for (i = 0; i < numEnodes; i++)
    {
        # ifdef _PARALLEL_PROCESSING

        if ( !theDomain->hasInternalNode((*ExteriorNodes)[i]) &&
                (theDomain->Domain::getNode( (*ExteriorNodes)[i] ) == 0) )
        {
            continue;
        }

        # endif
        theNode = theDomain->getNode( (*ExteriorNodes)[i] );

        if ( theNode == 0 )
        {
            continue;
        }

        load = this->getNodalLoad((*ExteriorNodes)[i], time);
        theNode->addUnbalancedLoad(*load);
        delete load;
    }
}

int
Domain_Reduction_Method_Restore_Forces::sendSelf(int commitTag, Channel& theChannel)
{
    //Guanzhou implemented for parallel analysis
    //if ( !LoadComputed )
    //   this->CompPBLoads(); //have to compute the load before send them over!

    static ID idData(7);

    int dataTag = this->getDbTag();

    idData.Zero();
    idData(0) = this->getTag();
    idData(1) = thetimeSteps;
    //idData(2) = UnumDataPoints;
    //idData(3) = UddnumDataPoints;
    //idData(4) = PBowlElements->Size();
    idData(2) = BoundaryNodes->Size();
    idData(3) = ExteriorNodes->Size();
    idData(4) = PBowlLoads->noRows();
    idData(5) = PBowlLoads->noCols();
    idData(6) = NodeID->Size();

    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send ID\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *BoundaryNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send BoundaryNodes\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *ExteriorNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send ExteriorNodes\n";
        return -1;
    }

    if (theChannel.sendMatrix(dataTag, commitTag, *PBowlLoads) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send Matrix PBowlLoads\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *NodeID) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send ExteriorNodes\n";
        return -1;
    }

    //if (theChannel.sendMatrix(dataTag, commitTag, U) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send Matrix U\n";
    // return -1;
    //}
    //
    //if (theChannel.sendMatrix(dataTag, commitTag, Udd) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send Matrix Udd\n";
    // return -1;
    //}
    //
    //if (theChannel.sendID(dataTag, commitTag, PBowlElements) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send PBowlElements\n";
    // return -1;
    //}

    static Vector data(2);

    data(0) = PBTimeIncr;
    data(1) = cFactor;
    //data(2) = xPlus;
    //data(3) = xMinus;
    //data(4) = yPlus;
    //data(5) = yMinus;
    //data(6) = zPlus;
    //data(7) = zMinus;

    if (theChannel.sendVector(dataTag, commitTag, data) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::sendSelf -- failed to send data\n";
        return -1;
    }

    return 0;

}

int
Domain_Reduction_Method_Restore_Forces::recvSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    //Guanzhou implemented for parallel processing
    int dataTag = this->getDbTag();
    static ID idData(7);

    if (theChannel.recvID(dataTag, commitTag, idData) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv ID\n";
        return -1;
    }

    this->setTag(idData(0));

    thetimeSteps = idData(1);

    int numBnodes = idData(2);

    if ( BoundaryNodes != 0 )
    {
        delete BoundaryNodes;
    }

    BoundaryNodes = new ID(numBnodes);

    if (BoundaryNodes == 0 || BoundaryNodes->Size() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf() - ran out of memory constructing";
        cerr << " a ID of size: " <<  BoundaryNodes->Size() << endln;
        exit(1);
    }

    if (theChannel.recvID(dataTag, commitTag, *BoundaryNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv BoundaryNodes\n";
        return -1;
    }

    int numEnodes = idData(3);

    if ( ExteriorNodes != 0 )
    {
        delete ExteriorNodes;
    }

    ExteriorNodes = new ID(numEnodes);

    if (ExteriorNodes == 0 || ExteriorNodes->Size() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf() - ran out of memory constructing";
        cerr << " a ID of size: " << ExteriorNodes->Size() << endln;
        exit(1);
    }

    if (theChannel.recvID(dataTag, commitTag, *ExteriorNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv ExteriorNodes\n";
        return -1;
    }

    int row = idData(4);
    int col = idData(5);

    if ( PBowlLoads != 0 )
    {
        delete PBowlLoads;
    }

    PBowlLoads = new Matrix(row, col);

    if (PBowlLoads == 0 || PBowlLoads->noRows() == 0 || PBowlLoads->noCols() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf() - ran out of memory constructing";
        cerr << " a Matrix of size (cols*rows): " << col << " * " << row << endln;
        exit(1);
    }

    if (theChannel.recvMatrix(dataTag, commitTag, *PBowlLoads) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv Matrix PBowlLoads\n";
        return -1;
    }

    int numNodes = idData(6);

    if ( NodeID != 0 )
    {
        delete NodeID;
    }

    NodeID = new ID(numNodes);

    if (NodeID == 0 || NodeID->Size() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf() - ran out of memory constructing";
        cerr << " a ID of size: " << NodeID->Size() << endln;
        exit(1);
    }

    if (theChannel.recvID(dataTag, commitTag, *NodeID) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv NodeID\n";
        return -1;
    }

    //UnumDataPoints = idData(2);
    //int cols = UnumDataPoints/thetimeSteps;
    //if ( U != 0 ) delete U;
    //U = new Matrix(cols, thetimeSteps);
    //if (U == 0 || U->noRows() == 0 || U->noCols() == 0) {
    //     cerr << "Domain_Reduction_Method_Restore_Forces::Domain_Reduction_Method_Restore_Forces() - ran out of memory constructing";
    //     cerr << " a Matrix of size (cols*rows): " << cols << " * " << thetimeSteps << endln;
    //  exit(1);
    //}
    //
    //if (theChannel.recvMatrix(dataTag, commitTag, U) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv Matrix U\n";
    // return -1;
    //}
    //
    //UddnumDataPoints = idData(3);
    //cols = UddnumDataPoints/thetimeSteps;
    //if ( Udd != 0 ) delete Udd;
    //Udd = new Matrix(cols, thetimeSteps);
    //if (Udd == 0 || Udd->noRows() == 0 || Udd->noCols() == 0) {
    //     cerr << "Domain_Reduction_Method_Restore_Forces::Domain_Reduction_Method_Restore_Forces() - ran out of memory constructing";
    //     cerr << " a Matrix of size (cols*rows): " << cols << " * " << thetimeSteps << endln;
    //  exit(1);
    //}
    //
    //if (theChannel.recvMatrix(dataTag, commitTag, Udd) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv Matrix Udd\n";
    // return -1;
    //}
    //
    //int numPBE = idData(4);
    //
    //if ( PBowlElements != 0 ) delete PBowlElements;
    //
    //PBowlElements = new ID(numPBE);
    //if (theChannel.recvID(dataTag, commitTag, PBowlElements) < 0) {
    // cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv PBowlElements\n";
    // return -1;
    //}

    static Vector data(2);

    if (theChannel.recvVector(dataTag, commitTag, data) < 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf -- failed to recv data\n";
        return -1;
    }

    PBTimeIncr =   data(0);
    cFactor    =   data(1);
    //xPlus      =   data(2);
    //xMinus     =   data(3);
    //yPlus      =   data(4);
    //yMinus     =   data(5);
    //zPlus      =   data(6);
    //zMinus     =   data(7);

    //cerr << "Domain_Reduction_Method_Restore_Forces::recvSelf() - the Domain_Reduction_Method_Restore_Forces received: \n";// << *PBowlLoads << endln;

    LoadComputed = true;

    //this->Print(cerr);

    return 0;

}



void
Domain_Reduction_Method_Restore_Forces::Print(ostream& s, int flag)
{
    s << "Domain_Reduction_Method_Restore_Forces " << this->getTag() << endln;
    s << "thetimeSteps " << thetimeSteps << endln;
    s << "BoundaryNodes " << *BoundaryNodes << endln;
    s << "ExteriorNodes " << *ExteriorNodes << endln;
    s << "PBowlLoads rows " << PBowlLoads->noRows() << endln;
    s << "PBowlLoads cols " << PBowlLoads->noCols() << endln;
    s << "NodeID " << *NodeID << endln;
}



// method to obtain a blank copy of the LoadPattern
LoadPattern*
Domain_Reduction_Method_Restore_Forces::getCopy(void)
{

    cerr << "Domain_Reduction_Method_Restore_Forces::getCopy() - not yet implemented\n";
    return 0;
}




void
Domain_Reduction_Method_Restore_Forces::CompPBLoads()
{
    ///////////////////////////////////////
    // Figure out the interior and exterior nodes //Guanzhou added
    ////////////////////////////////

    Domain* theDomain = this->getDomain();


    if (theDomain == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::CompPBLoads() -- domain can not be found." << endl;
        exit(-1);
    }


    //Assume all the plastic bowl elements have the same number of nodes
    Element* theElement = theDomain->getElement( (*PBowlElements)(0) );
    int NIE = theElement->getNumExternalNodes();

    int max_bnode = PBowlElements->Size() * NIE;
    ID* Bowl_node = new ID(max_bnode);
    ID* Bound_node = new ID(max_bnode);
    ID NidesinFirstEle = theElement->getExternalNodes();

    //   int i, j, k, bi;
    int i, j, k;

    //Inital node list from the first plastic bowl element
    for (i = 0; i < NIE; i++)
    {
        (*Bowl_node)(i) = NidesinFirstEle(i);
    }

    int no_bnode = NIE;
    int no_boundarynodes = 0, no_exteriornodes = 0;
    int Bowl_elem_nb = PBowlElements->Size();
    ID Temp;

    # ifdef _PARALLEL_PROCESSING

    if ( theElement != NULL )
    {
        delete theElement;
        theElement = NULL;
    }

    # endif

    for ( i = 1; i < Bowl_elem_nb; i++)
    {
        theElement = theDomain->getElement( (*PBowlElements)(i) );
        Temp = theElement->getExternalNodes();

        for ( j = 0; j < NIE; j++)
        {
            for (k = 0; k < no_bnode; k++)
            {
                if ( Temp(j) == (*Bowl_node)(k) )
                    //cerr << Temp(j) << "  " << Bowl_node(k) << endln;
                {
                    break;
                }
            } //end of for (k=0...)

            // If no duplicate, add new node; otherwise, skip
            if ( k == no_bnode)
            {
                (*Bowl_node)(no_bnode) = Temp(j);
                no_bnode ++;
            } //end of IF

        } //end of for (j=0...)


        # ifdef _PARALLEL_PROCESSING

        if ( theElement != NULL )
        {
            delete theElement;
            theElement = NULL;
        }

        # endif
    }

    //--Joey------------------------------------------------

    //BJ test
    //check the bowl nodes
    //cerr << "\nCheck all plastic bowl nodes...\n";
    //for (bi=0;bi<no_bnode;bi++)
    //   cerr<< (*Bowl_node)(bi) <<"  ";
    //cerr<< endln << "# of pbowl nodes = " << no_bnode<<endln;
    //cerr<<"finish inputting  and organizing the Bowl_node array"<<endln;
    //BJ test


    //  //Find all nodes on the boundary Joey Yang & Boris Jeremic 11/06/02
    //   if ( (BoundaryNodes == 0) && ( (xPlus == xMinus) || (yPlus == yMinus) || (zPlus == zMinus) ) ) {
    //      cerr << "Domain_Reduction_Method_Restore_Forces::CompPBLoads() - Boundary node specification not correct..." << endln;
    //      exit(1);
    //   }


    // changes by Boris and Nima 11May2009
    if ( (BoundaryNodes == 0) && ( (xPlus < xMinus) || (yPlus < yMinus) || (zPlus < zMinus) ) )
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::CompPBLoads() - Boundary node specification not correct... node corrdinates signes flipped" << endln;
        exit(1);
    }

    no_boundarynodes = 0;

    for (i = 0; i < no_bnode; i++)
    {
        # ifdef _PARALLEL_PROCESSING
        Node* theNode = theDomain->getOutsideNode( (*Bowl_node)(i) );
        # else
        Node* theNode = theDomain->getNode( (*Bowl_node)(i) );
        # endif
        const Vector& coor = theNode->getCrds();

        //right face
        if (      (coor(0) == xPlus)  && (coor(1) >= yMinus) && (coor(1) <= yPlus) && (coor(2) >= zMinus) && (coor(2) <= zPlus) )
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }
        //left face
        else if ( (coor(0) == xMinus) && (coor(1) >= yMinus) && (coor(1) <= yPlus) && (coor(2) >= zMinus) && (coor(2) <= zPlus))
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }
        //upper face
        else if ( (coor(1) == yPlus)  && (coor(0) >= xMinus) && (coor(0) <= xPlus) && (coor(2) >= zMinus) && (coor(2) <= zPlus))
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }
        //lower face
        else if ( (coor(1) == yMinus) && (coor(0) >= xMinus) && (coor(0) <= xPlus) && (coor(2) >= zMinus) && (coor(2) <= zPlus))
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }
        //top face
        else if ( (coor(2) == zPlus)  && (coor(0) >= xMinus) && (coor(0) <= xPlus) && (coor(1) >= yMinus) && (coor(1) <= yPlus))
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }
        //bottom face
        else if ( (coor(2) == zMinus) && (coor(0) >= xMinus) && (coor(0) <= xPlus) && (coor(1) >= yMinus) && (coor(1) <= yPlus))
        {
            (*Bound_node)(no_boundarynodes) = (*Bowl_node)(i);
            no_boundarynodes++;
        }

        # ifdef _PARALLEL_PROCESSING

        if ( theDomain->Domain::getNode( (*Bowl_node)(i) )  == NULL )
        {
            delete theNode;
            theNode = NULL;
        }

        # endif
    }

    //Adding all boundary nodes on the plastic bowl
    BoundaryNodes = new ID(no_boundarynodes);

    if (BoundaryNodes == 0 || BoundaryNodes->Size() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::CompPBLoads() - ran out of memory constructing";
        cerr << " a Vector of size: " <<  BoundaryNodes->Size() << endln;

        if (BoundaryNodes != 0)
        {
            delete BoundaryNodes;
        }

        BoundaryNodes = 0;
    }


    no_exteriornodes = no_bnode;

    for (i = 0; i < no_boundarynodes; i++)
    {
        (*BoundaryNodes)(i) = (*Bound_node)(i);

        //Shift out all the boundary nodes from Bowl_node
        for (j = 0; j < no_bnode; j++)
        {
            if ( (*Bowl_node)(j) == (*Bound_node)(i) )
            {
                (*Bowl_node)(j) = 0; //zero this boundary nodes
                no_exteriornodes --;
            }
        } // end of sifting loop
    } // end of adding boundary node loop



    // Adding all exterior nodes on the plastic bowl
    ExteriorNodes = new ID(no_exteriornodes);

    if (ExteriorNodes == 0 || ExteriorNodes->Size() == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::CompPBLoads() - ran out of memory constructing";
        cerr << " a Vector of size: " <<  ExteriorNodes->Size() << endln;

        if (ExteriorNodes != 0)
        {
            delete ExteriorNodes;
        }

        ExteriorNodes = 0;
    }

    j = 0;

    for (i = 0; i < no_bnode; i++)
    {
        if ( (*Bowl_node)(i) != 0 )
        {
            (*ExteriorNodes)(j) = (*Bowl_node)(i);
            j++;
        }
    }




    //=================================================
    ifstream theForceFile;
    theForceFile.open(DRMForceFile);

    if (!theForceFile)
    {
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << DRMForceFile << endln;
        exit(2);
    }



    int ndof = number_of_DRM_nodes * 3;
    PBowlLoads = new Matrix(ndof, thetimeSteps);


    for (int rr = 0; rr < ndof; rr++)
    {
        for (int cc = 0; cc < thetimeSteps; cc++)
        {
            theForceFile >> (*PBowlLoads)(rr, cc);
        }
    }


    theForceFile.close();
    //=================================================


}


Vector*
Domain_Reduction_Method_Restore_Forces::getNodalLoad(int nodeTag, double time)
{
    //Vector *dummy = new Vector(0);
    //Get the node
    Domain* theDomain = this->getDomain();
    Node* theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        cerr << "Domain_Reduction_Method_Restore_Forces::getNodalLoad() - no nodes associtated to the nodeTag " << nodeTag << "\n";
        //return dummy;
        exit(1);
    }

    //delete dummy;

    //Create the nodal load vector accoding to the DOFs the node has
    int numDOF = theNode->getNumberDOF();
    Vector* nodalLoad = new Vector(numDOF);


    //Get the nodal loads associated to the nodeTag
    // check for a quick return
    if (time < 0.0 || PBowlLoads == 0)
    {
        return nodalLoad;
    }

    // determine indexes into the data array whose boundary holds the time
    double incr = time / PBTimeIncr;
    //GZ int incr1 = (int) floor(incr)-1;
    int incr1 = (int)floor(incr);
    int incr2 = incr1 + 1;

    double value1 = 0;
    double value2 = 0;

    //GZ int i;
    //GZ if ( incr2 == thetimeSteps ) {
    //GZ   for (i = 0; i < numDOF; i++)
    //GZ      (*nodalLoad)(i) = (*PBowlLoads)(i, incr1);
    //GZ }
    //GZ
    //GZ //If beyond time step, return 0 loads
    //GZ else


    if (incr2 > thetimeSteps )
    {
        //test if ( nodeTag == 109)
        //test    cerr << "Time = " << time << " Node # " << nodeTag  << " " << (*nodalLoad) << endln;
        return nodalLoad;
    }

    //If within time step, return interpolated values
    else
    {
        for (int i = 0; i < numDOF; i++)
        {
            //Guanzhou introduce new data structure!!!
            int r;

            for (r = 0; r < NodeID->Size(); r++)
            {
                if ( nodeTag == (*NodeID)(r) )
                {
                    break;
                }
            }

            value1 = (*PBowlLoads)(numDOF * r + i, incr1);
            value2 = (*PBowlLoads)(numDOF * r + i, incr2);
            //value1 = (*PBowlLoads)(numDOF*(nodeTag-1)+i, incr1);
            //value2 = (*PBowlLoads)(numDOF*(nodeTag-1)+i, incr2);

            //GZ (*nodalLoad)(i) = cFactor*(value1 + (value2-value1)*(time/PBTimeIncr - incr2));
            (*nodalLoad)(i) = cFactor * (value1 + (value2 - value1) * (time / PBTimeIncr - incr1));

        }
    }


    return nodalLoad;

}

