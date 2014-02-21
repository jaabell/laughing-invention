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
//#                    This file contains the class definition for Domain_Reduction_Method_Save_Forces.
//#                    Domain_Reduction_Method_Save_Forces is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# CLASS:             Domain_Reduction_Method_Save_Forces
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


// Purpose: This file contains the class definition for Domain_Reduction_Method_Save_Forces.
// Domain_Reduction_Method_Save_Forces is an subclass of loadPattern.

#include <Domain_Reduction_Method_Save_Forces.h>
#include <math.h>

#define numNodeDOF 3


#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

#include <iostream>
using namespace std;


Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()
    : LoadPattern(0, PATTERN_TAG_Domain_Reduction_Method_Save_Forces),
      PBowlElements(0),
      ExteriorNodes(0),
      BoundaryNodes(0),
      NodeID(0),
      PBowlLoads(0),
      U(0),
      Udd(0)
{
    LoadComputed = false;
}

Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces(int tag,
        const char* PBEfName,
        const char* PBNfName,
        const char* DispfName,
        const char* AccefName,
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
                  PATTERN_TAG_Domain_Reduction_Method_Save_Forces),
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


    ifstream theFileACC;
    ifstream theFileDIS;
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
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
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



    //--------------------------------------
    // Beginning of Input displacement data
    //--------------------------------------

    int num_nodes;
    numDataPoints = 0;
    theFileDIS.open(DispfName);

    if (!theFileDIS)
    {
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << DispfName << endln;
        exit(1);
    }

    else
    {


        //        fprintf(stderr, "Number of Input Time Steps= %d", thetimeSteps);
        cerr << "Number of Input Time Steps: " << thetimeSteps << endln;


        if ( thetimeSteps == 0)
        {
            cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
            cerr << " - Time steps for displacement equal to zero... " << DispfName << endln;
            exit(0);
        }


        cerr << "\nNum nodes of disp: " << number_of_DRM_nodes << endln;

        int RequiredNumDataPoints = number_of_DRM_nodes * numNodeDOF * thetimeSteps;


        // create a vector and read in the data
        // first open the file
        // now create the vector


        int cols = RequiredNumDataPoints / thetimeSteps;
        U = new Matrix(cols, thetimeSteps);

        // ensure we did not run out of memory
        if (U == 0 || U->noRows() == 0 || U->noCols() == 0)
        {
            cerr << "Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces() - ran out of memory constructing";
            cerr << " a Matrix of size (cols*rows): " << cols << " * " << thetimeSteps << endln;

            if (U != 0)
            {
                delete U;
            }

            U = 0;
        }
        // read the data from the file
        else
        {


            for  (int j = 0; j < cols; j++)
            {
                for (int t = 0; t < thetimeSteps; t++)
                {
                    theFileDIS >> dataPoint;
                    (*U)(j, t) = dataPoint;
                    //               fprintf(stdout, "U ( %d, %d ) = %12.6e \n", j, t, (*U)(j, t));
                    numDataPoints++;
                }
            }

        }


        cerr << "Number of Displacement Data Points: " << RequiredNumDataPoints << endln;



        //--------------------------------
        // Nima added (June 2nd 2010)
        // Printing the Displacement Matrix
        //  cerr << "- Domain_Reduction_Method_Save_Forces:: Displacement Matrix:" << endln;;
        //  cerr << (*U) << endln;
        //--------------------------------


        UnumDataPoints = numDataPoints;

        if (RequiredNumDataPoints != numDataPoints)
        {
            cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()" << endln;;
            cerr << " - Input data of displacement file are not sufficient ... " << DispfName << endln;
            cerr << "Num of required data points: " << RequiredNumDataPoints <<  "Num of current data points: " << numDataPoints << DispfName << endln;
            exit(1);
        }


        // finally close the file
        theFileDIS.close();
    }

    //--------------------------------
    // End of Input displacement data
    //--------------------------------


    //--------------------------------------
    // Beginning of Input acceleration data
    //--------------------------------------

    numDataPoints = 0;
    theFileACC.open(AccefName);

    if (!theFileACC)
    {
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << AccefName << endln;
        exit(1);
    }

    else
    {





        if ( thetimeSteps == 0)
        {
            cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
            cerr << " - Time steps for acceleration equal to zero... " << AccefName << endln;
            exit(0);
        }



        // create a vector and read in the data
        // first open the file
        // now create the vector

        int RequiredNumDataPoints = number_of_DRM_nodes * numNodeDOF * thetimeSteps;
        int cols = RequiredNumDataPoints / thetimeSteps;
        Udd = new Matrix(cols, thetimeSteps);

        // ensure we did not run out of memory
        if (Udd == 0 || Udd->noRows() == 0 || Udd->noCols() == 0)
        {
            cerr << "Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces() - ran out of memory constructing";
            cerr << " a Matrix of size (cols*rows): " << cols << " * " << thetimeSteps << endln;

            if (Udd != 0)
            {
                delete Udd;
            }

            Udd = 0;
        }
        // read the data from the file
        else
        {

            for  (int j = 0; j < cols; j++)
            {
                for (int t = 0; t < thetimeSteps; t++)
                {
                    theFileACC >> dataPoint;
                    (*Udd)(j, t) = dataPoint;
                    //                        fprintf(stdout, "Udd ( %d, %d ) = %12.6e \n", j, t, (*Udd)(j, t));

                    numDataPoints++;
                }
            }

        }


        cerr << "Number of Acceleration Data Points: " << RequiredNumDataPoints << endln;



        UddnumDataPoints = numDataPoints;

        if (RequiredNumDataPoints != numDataPoints)
        {
            cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()" << endln;;
            cerr << " - Input data of acceleration file are not sufficient ... " << DispfName << endln;
            cerr << "Num of required data points: " << RequiredNumDataPoints <<  "Num of current data points: " << numDataPoints << AccefName << endln;
            exit(1);
        }


        // finally close the file
        theFileACC.close();
    }

    //--------------------------------------
    // End of Input acceleration data
    //--------------------------------------


    LoadComputed = false;

    cerr << "Finished creating Domain_Reduction_Method_Save_Forces!\n" ;

}



Domain_Reduction_Method_Save_Forces::~Domain_Reduction_Method_Save_Forces()
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

    //if ( U != 0)
    //  delete U;

    //if ( Udd != 0)
    //  delete Udd;

}


void
Domain_Reduction_Method_Save_Forces::setDomain(Domain* theDomain)
{
    this->LoadPattern::setDomain(theDomain);

    //Guanzhou implemented for parallel analysis
    if ( !LoadComputed )
    {
        this->CompPBLoads();    //have to compute the load before send them over!
    }

}


void
Domain_Reduction_Method_Save_Forces::Print(ostream& s, int flag)
{
    s << "Domain_Reduction_Method_Save_Forces " << this->getTag() << endln;
    s << "thetimeSteps " << thetimeSteps << endln;
    s << "BoundaryNodes " << *BoundaryNodes << endln;
    s << "ExteriorNodes " << *ExteriorNodes << endln;
    s << "PBowlLoads rows " << PBowlLoads->noRows() << endln;
    s << "PBowlLoads cols " << PBowlLoads->noCols() << endln;
    s << "NodeID " << *NodeID << endln;
}



void
Domain_Reduction_Method_Save_Forces::CompPBLoads()
{
    ///////////////////////////////////////
    // Figure out the interior and exterior nodes //Guanzhou added
    ////////////////////////////////

    Domain* theDomain = this->getDomain();

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


    // changes by Boris and Nima 11May2009
    if ( (BoundaryNodes == 0) && ( (xPlus < xMinus) || (yPlus < yMinus) || (zPlus < zMinus) ) )
    {
        cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - Boundary node specification not correct... node corrdinates signes flipped" << endln;
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
        cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - ran out of memory constructing";
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
        cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - ran out of memory constructing";
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


    //=======================================================================
    // Computing the equivalent(effective) forces for all plastic bowl nodes
    //=======================================================================

    //int no_boundarynodes = BoundaryNodes->Size();
    cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - # Boundary Nodes: " << no_boundarynodes << endln;
    cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - # Exterior Nodes: " << no_exteriornodes << endln;

    if ( no_exteriornodes + no_boundarynodes != NodeID->Size())
    {
        cerr << "Domain_Reduction_Method_Save_Forces::CompPBLoads() - Error!\n";
        cerr << "Number of nodes in PBElement does not equal to NodeID size!\n";
        exit(1);
    }

    cerr << "Computing the equivalent(effective) forces for all plastic bowl nodes" << endln;
    int cols = Udd->noRows();
    //Matrix to hold the computed effective nodal forces for all plastic bowl DOFs at each time step
    Matrix* F = new Matrix(cols, thetimeSteps);

    //Assume all plastic bowl nodes have the same number of DOFs
    # ifdef _PARALLEL_PROCESSING
    Node* theNode = theDomain->getOutsideNode((*BoundaryNodes)(0));
    # else
    Node* theNode = theDomain->getNode((*BoundaryNodes)(0));
    # endif

    int NDOF = theNode->getNumberDOF();

    # ifdef _PARALLEL_PROCESSING

    if ( theDomain->Domain::getNode( (*BoundaryNodes)(0) )  == NULL )
    {
        delete theNode;
        theNode = NULL;
    }

    # endif

    Vector* Fm = new Vector(NIE * NDOF);
    Vector* Fk  = new Vector(NIE * NDOF);

    Vector* u_e = new Vector(NIE * NDOF);
    Vector* udd_e = new Vector(NIE * NDOF);


    // intialize the F()
    for (int i = 0; i < cols; i++)
        for (int j = 0; j < thetimeSteps; j++)
        {
            (*F)(i, j) = 0;
        }

    Element* theBowlElements;

    for (int i = 0; i < Bowl_elem_nb; i++)
    {
        // get the Brick;
        theBowlElements = theDomain->getElement( (*PBowlElements)(i) );

        # ifdef _PARALLEL_PROCESSING
        theBowlElements->setDomain(theDomain);
        # endif

        const ID& nd = theBowlElements->getExternalNodes();

        Matrix Me = theBowlElements ->getMass();
        Matrix Ke = theBowlElements ->getTangentStiff();

        //-------------------------------------------------------------------------
        //Zero diagonal block entries of boundary and exterior nodes, respectively
        //-------------------------------------------------------------------------

        //Find out the boundary and exterior nodes in this element
        ID* B_node = new ID(NIE);  //array to store the indices of all boundary nodes
        ID* E_node = new ID(NIE);  //array to store the indices of all exterior nodes
        int nB = 0, nE = 0;
        bool bdnode;

        for ( int ii = 0; ii < NIE; ii++)
        {
            //Check if nd(ii) is boundary node or not
            bdnode = false;

            for (int id = 0; id < no_boundarynodes; id++)
            {
                if ( nd(ii) == (*BoundaryNodes)(id) )
                {
                    bdnode = true;
                    break;
                }
            }

            if ( bdnode )
            {
                (*B_node)(nB) = ii;
                nB ++;
            }
            else
            {
                (*E_node)(nE) = ii;
                nE ++;
            }

        }


        //Zero out the diagonal block of Boundary nodes
        int m;

        for (m = 0; m < nB; m++)
            for (int n = 0; n < nB; n++)
                for (int d = 0; d < NDOF; d++)
                    for (int e = 0; e < NDOF; e++)
                    {
                        Me( (*B_node)(m)*NDOF + d, (*B_node)(n)*NDOF + e ) = 0.0;
                        Ke( (*B_node)(m)*NDOF + d, (*B_node)(n)*NDOF + e ) = 0.0;
                    }


        //Zero out the diagonal block of Exterior nodes
        for (m = 0; m < nE; m++)
            for (int n = 0; n < nE; n++)
                for (int d = 0; d < NDOF; d++)
                    for (int e = 0; e < NDOF; e++)
                    {
                        Me( (*E_node)(m)*NDOF + d, (*E_node)(n)*NDOF + e ) = 0.0;
                        Ke( (*E_node)(m)*NDOF + d, (*E_node)(n)*NDOF + e ) = 0.0;
                    }


        delete B_node;
        delete E_node;


        // get the u and u_dotdot for this element
        for ( int t = 0; t < thetimeSteps; t++)
        {

            //cerr << "element: " << i << "" << " Time step: " << t << endln;
            for (int j = 0; j < NIE; j++) //BJ make it into # of nodes per element (2D or 3D...)
            {

                for (int d = 0; d < NDOF; d++)
                {
                    //Guanzhou introduce new data structure!!!
                    int r;

                    for (r = 0; r < NodeID->Size(); r++)
                    {
                        if ( nd(j) == (*NodeID)(r) )
                        {
                            break;
                        }
                    }

                    (*u_e)(j * NDOF + d)   = (*U)(r * NDOF + d, t);
                    (*udd_e)(j * NDOF + d) = (*Udd)(r * NDOF + d, t);

                }
            }



            Fm->addMatrixVector(0.0, Me, (*udd_e), 1.0);
            Fk->addMatrixVector(0.0, Ke, (*u_e), 1.0);

            for (int k = 0; k < NIE; k++)
                for (int d = 0; d < NDOF; d++)
                {

                    //Guanzhou introduce new data structure!!!
                    int r;

                    for (r = 0; r < NodeID->Size(); r++)
                    {
                        if ( nd(k) == (*NodeID)(r) )
                        {
                            break;
                        }
                    }

                    (*F)( r * NDOF + d, t) = (*F)( r * NDOF + d, t) + (*Fk)(k * NDOF + d) + (*Fm)(k * NDOF + d);
                    //(*F)( nd(k)*NDOF-NDOF+d,t) = (*F)( nd(k)*NDOF-NDOF+d,t) + (*Fk)(k*NDOF+d) + (*Fm)(k*NDOF+d);
                }

        } //end for timestep


        # ifdef _PARALLEL_PROCESSING

        if ( theBowlElements != NULL )
        {
            const int num = theBowlElements->getNumExternalNodes();
            Node** nodes = theBowlElements->getNodePtrs();

            for (int i = 0; i < num; i++)
            {
                int nodeTag = (*(nodes + i))->getTag();

                if ( theDomain->Domain::getNode(nodeTag)  == NULL )
                {
                    delete *(nodes + i);
                    *(nodes + i) = NULL;
                }
            }

            //delete [] nodes;
            delete theBowlElements;
            theBowlElements = NULL;
        }

        # endif

    }  // end for bowl element


    PBowlLoads = new Matrix(*F);

    // ensure we did not run out of memory
    if (PBowlLoads->noRows() == 0 || PBowlLoads->noCols() == 0 )
    {
        cerr << "Domain_Reduction_Method_Save_Forces::PBowlLoads() - ran out of memory";
        cerr << " a Matrix of size: " <<  PBowlLoads->noRows() << " * " << PBowlLoads->noCols() << endln;
    }



    //=================================================
    ofstream theForceFile;
    theForceFile.open(DRMForceFile);

    if (!theForceFile)
    {
        cerr << "WARNING - Domain_Reduction_Method_Save_Forces::Domain_Reduction_Method_Save_Forces()";
        cerr << " - could not open file " << DRMForceFile << endln;
        exit(2);
    }



    for (int rr = 0; rr < cols; rr++)
    {
        for (int cc = 0; cc < thetimeSteps; cc++)
        {
            theForceFile << (*PBowlLoads)(rr, cc) << "    ";
        }

        theForceFile << endl;
    }


    theForceFile.close();
    //=================================================



    //BJ test
    cerr << "\nFinish calculating the forces..." << endln << endln;
    //BJ test

    LoadComputed = true;
    delete Bound_node;
    delete Bowl_node;

    delete Fm;
    delete Fk;
    delete u_e;
    delete udd_e;
    delete F;

    if ( Udd != 0 )
    {
        delete Udd;
    }

    if ( U != 0 )
    {
        delete U;
    }

}

