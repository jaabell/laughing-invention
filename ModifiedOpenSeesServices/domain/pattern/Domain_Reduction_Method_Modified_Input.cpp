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
//#                    This file contains the class definition for Domain_Reduction_Method_Modified_Input.
//#                    Domain_Reduction_Method_Modified_Input is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# CLASS:             Domain_Reduction_Method_Modified_Input
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
//# DATE:              March 2011
//# UPDATE HISTORY:
//#
//#
//===============================================================================


// Purpose: This file contains the class definition for Domain_Reduction_Method_Modified_Input.
// Domain_Reduction_Method_Modified_Input is an subclass of loadPattern.

#include <Domain_Reduction_Method_Modified_Input.h>
#include <math.h>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <time.h>


#define numNodeDOF 3

using namespace std;

Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()
    : LoadPattern(0, PATTERN_TAG_Domain_Reduction_Method_Modified_Input),
      PBowlElements(0),
      ExteriorNodes(0),
      BoundaryNodes(0),
      NodeID(0),
      PBowlLoads(0),
      U(0),
      Udd(0),
      maxnodetag(0)
{
    LoadComputed = false;
}

Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input
(
    int tag,
    const char *ElementsFilename,
    const char *DispfName,
    const char *AccefName,
    const char *boundaryNodesFile,
    const char *externalNodesFile,
    double theTimeIncr,
    double theFactor,
    int timeSteps,
    int Number_of_DRM_Elements,
    int Number_of_DRM_Nodes_b,
    int Number_of_DRM_Nodes_e)
    : LoadPattern(tag, PATTERN_TAG_Domain_Reduction_Method_Modified_Input),
      PBTimeIncr(theTimeIncr),
      cFactor(theFactor),
      number_of_DRM_elements(Number_of_DRM_Elements),
      number_of_DRM_nodes(Number_of_DRM_Nodes_b + Number_of_DRM_Nodes_e),
      number_of_DRM_nodes_e(Number_of_DRM_Nodes_e),
      number_of_DRM_nodes_b(Number_of_DRM_Nodes_b),
      thetimeSteps(timeSteps),
      DRMElementsFile(strdup(ElementsFilename)),
      DRMDisplacementsFile(strdup(DispfName)),
      DRMAccelerationsFile(strdup(AccefName)),
      PBowlElements(0),
      ExteriorNodes(0),
      BoundaryNodes(0),
      NodeID(0),
      PBowlLoads(0),
      U(0),
      Udd(0),
      maxnodetag(0)
{

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

    theFileELE.open(DRMElementsFile);
    numDataPoints = 0;

    if (!theFileELE)
    {
        cerr << "WARNING - Domain_Reduction_Method_Modified_Input()";
        cerr << " - could not open file " << DRMElementsFile << endln;
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
            cerr << "Domain_Reduction_Method_Modified_Input() - ran out of memory constructing";
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
    }



    //--------------------------------
    //Adding plastic bowl nodes
    //from file to PBowlElements
    //--------------------------------

    theFileNOD.open(externalNodesFile);
    numDataPoints = 0;

    if (!theFileNOD)
    {
        cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
        cerr << " - could not open file " << DRMNodesFile << endln;
        exit(2);
    }
    else
    {
        // create a vector and read in the data
        ExteriorNodes = new ID(Number_of_DRM_Nodes_e);

        if ( ExteriorNodes == 0 )
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
            cerr << " Error allocating memory for ExteriorNodes! " << endln;
            exit(0);
        }
        // ensure we did not run out of memory
        if (ExteriorNodes == 0 || ExteriorNodes->Size() == 0 )
        {
            cerr << "Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input() - ran out of memory constructing";
            cerr << " a ID of size: " << ExteriorNodes->Size() << endln;

            if (ExteriorNodes != 0)
            {
                delete ExteriorNodes;
            }

            ExteriorNodes = 0;
        }
        // read the data from the file
        else
        {
            for (int m = 0; m < Number_of_DRM_Nodes_e; m++)
            {
                theFileNOD >> (*ExteriorNodes)(m);
                if ((*ExteriorNodes)(m) > maxnodetag)
                {
                    maxnodetag = (*ExteriorNodes)(m);
                }
                numDataPoints++;
            }
        }

        // finally close the file
        theFileNOD.close();

        //Check if read in correctly
        cerr << "Number of DRM External Nodes: " << Number_of_DRM_Nodes_e << " of " << number_of_DRM_nodes << endln;
    }



    theFileNOD.open(boundaryNodesFile);
    numDataPoints = 0;

    if (!theFileNOD)
    {
        cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
        cerr << " - could not open file " << DRMNodesFile << endln;
        exit(2);
    }
    else
    {
        // create a vector and read in the data
        BoundaryNodes = new ID(Number_of_DRM_Nodes_b);

        if ( BoundaryNodes == 0 )
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
            cerr << " Error allocating memory for BoundaryNodes! " << endln;
            exit(0);
        }
        // ensure we did not run out of memory
        if (BoundaryNodes == 0 || BoundaryNodes->Size() == 0 )
        {
            cerr << "Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input() - ran out of memory constructing";
            cerr << " a ID of size: " << BoundaryNodes->Size() << endln;

            if (BoundaryNodes != 0)
            {
                delete BoundaryNodes;
            }

            BoundaryNodes = 0;
        }
        // read the data from the file
        else
        {
            for (int m = 0; m < Number_of_DRM_Nodes_b; m++)
            {
                theFileNOD >> (*BoundaryNodes)(m);
                if ((*BoundaryNodes)(m) > maxnodetag)
                {
                    maxnodetag = (*BoundaryNodes)(m);
                }
                numDataPoints++;
            }
        }

        // finally close the file
        theFileNOD.close();

        //Check if read in correctly
        cerr << "Number of DRM Boundary Nodes: " << Number_of_DRM_Nodes_b << " of " << number_of_DRM_nodes << endln;
    }

    NodeID = new ID(maxnodetag + 1);
    for (int i = 0; i < NodeID->Size(); i++)
    {
        (*NodeID)(i) = -1;
    }

    if (NodeID == 0)
    {
        cerr << "Ran out of memory creating NodeID\n";
    }
    else
    {
        int i = 0;
        for (; i < Number_of_DRM_Nodes_b; i++)
        {
            (*NodeID)((*BoundaryNodes)(i)) = i;
        }
        int j = 0;
        for (; j < Number_of_DRM_Nodes_e; j++)
        {
            (*NodeID)((*ExteriorNodes)(j)) = i + j;
        }
        cout << "Copied " << i << " out of "  << Number_of_DRM_Nodes_b << " boundary nodes.\n";
        cout << "Copied " << j << " out of "  << Number_of_DRM_Nodes_e << " exterior nodes.\n";
    }

    cout << "NodeID: \n";
    for (int i = 0; i < NodeID->Size(); i++)
    {
        cout << " " << i << " -> " << (*NodeID)(i) << endl;
    }


    //--------------------------------------
    // Beginning of Input displacement data
    //--------------------------------------

    // int num_nodes;
    numDataPoints = 0;
    theFileDIS.open(DRMDisplacementsFile);

    //   if (theFileDIS.bad())
    if (!theFileDIS)
    {
        cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
        cerr << " - could not open file " << DRMDisplacementsFile << endln;
        //         exit(1);
    }

    else
    {


        //        fprintf(stderr, "Number of Input Time Steps= %d", thetimeSteps);
        cerr << "Number of Input Time Steps: " << thetimeSteps << endln;


        if ( thetimeSteps == 0)
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
            cerr << " - Time steps for displacement equal to zero... " << DRMDisplacementsFile << endln;
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
            cerr << "Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input() - ran out of memory constructing";
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
                    numDataPoints++;
                }
            }
        }


        cerr << "Number of Displacement Data Points: " << RequiredNumDataPoints << endln;

        UnumDataPoints = numDataPoints;

        if (RequiredNumDataPoints != numDataPoints)
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()" << endln;;
            cerr << " - Input data of displacement file are not sufficient ... " << DRMDisplacementsFile << endln;
            cerr << "Num of required data points: " << RequiredNumDataPoints <<  "Num of current data points: " << numDataPoints << DRMDisplacementsFile << endln;
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
    theFileACC.open(DRMAccelerationsFile);

    if (!theFileACC)
    {
        cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
        cerr << " - could not open file " << DRMAccelerationsFile << endln;
        //         exit(1);
    }

    else
    {
        if ( thetimeSteps == 0)
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()";
            cerr << " - Time steps for acceleration equal to zero... " << DRMAccelerationsFile << endln;
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
            cerr << "Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input() - ran out of memory constructing";
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
                    numDataPoints++;
                }
            }

        }


        cerr << "Number of Acceleration Data Points: " << RequiredNumDataPoints << endln;


        UddnumDataPoints = numDataPoints;

        if (RequiredNumDataPoints != numDataPoints)
        {
            cerr << "WARNING - Domain_Reduction_Method_Modified_Input::Domain_Reduction_Method_Modified_Input()" << endln;;
            cerr << " - Input data of acceleration file are not sufficient ... " << DRMDisplacementsFile << endln;
            cerr << "Num of required data points: " << RequiredNumDataPoints <<  "Num of current data points: " << numDataPoints << DRMAccelerationsFile << endln;
            exit(1);
        }

        // finally close the file
        theFileACC.close();
    }

    //--------------------------------------
    // End of Input acceleration data
    //--------------------------------------

    LoadComputed = false;

    cerr << "Finished creating Domain_Reduction_Method_Modified_Input!\n" ;

}



Domain_Reduction_Method_Modified_Input::~Domain_Reduction_Method_Modified_Input()
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
}


void
Domain_Reduction_Method_Modified_Input::setDomain(Domain *theDomain)
{
    this->LoadPattern::setDomain(theDomain);

    //Out by Babak Kamrani on 08/16/2012:
    //Guanzhou implemented for parallel analysis
    if ( !LoadComputed )
    {
        this->CompPBLoads();    //have to compute the load before send them over!
    }

# ifdef _BABAK_DEBUG
    cerr << "BABAK@Domain_Reduction_Method_Modified_Input::CompPBLoads() BoundaryNodes are:   " << ((*BoundaryNodes)(0)) << "before coming back to setDomain\n" ;

#endif

}


void
Domain_Reduction_Method_Modified_Input::applyLoad(double time)
{

    Domain *theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }

    //Finding the all the nodes in the plastic bowl and sort it ascendingly
    if ( !LoadComputed )
    {
        this->CompPBLoads();
    }

    // see if quick return, i.e. no plastic bowl nodes or domain set
    int numBnodes = BoundaryNodes->Size();
    int numEnodes = ExteriorNodes->Size();

    if ( (numBnodes + numEnodes) == 0)
    {
        return;
    }

    //Apply loads on each boundary bowl nodes
    Node *theNode;
    Vector *load;
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
Domain_Reduction_Method_Modified_Input::sendSelf(int commitTag, Channel &theChannel)
{
#ifdef _BABAK_DEBUG
    cerr << "DomaiPartitioner::partition--BEFORE theSubdomain->addLoadPattern(newLoadPattern) -- DRM INFORMATION:\n";
#endif
    static ID idData(7);

    int dataTag = this->getDbTag();

    idData.Zero();
    idData(0) = this->getTag();
    idData(1) = thetimeSteps;
    idData(2) = BoundaryNodes->Size();
    idData(3) = ExteriorNodes->Size();
    idData(4) = PBowlLoads->noRows();
    idData(5) = PBowlLoads->noCols();
    idData(6) = NodeID->Size();

    if (theChannel.sendID(dataTag, commitTag, idData) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send ID\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *BoundaryNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send BoundaryNodes\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *ExteriorNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send ExteriorNodes\n";
        return -1;
    }

    if (theChannel.sendMatrix(dataTag, commitTag, *PBowlLoads) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send Matrix PBowlLoads\n";
        return -1;
    }

    if (theChannel.sendID(dataTag, commitTag, *NodeID) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send ExteriorNodes\n";
        return -1;
    }

    static Vector data(2);

    data(0) = PBTimeIncr;
    data(1) = cFactor;

    if (theChannel.sendVector(dataTag, commitTag, data) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::sendSelf -- failed to send data\n";
        return -1;
    }

    return 0;

}

int
Domain_Reduction_Method_Modified_Input::receiveSelf(int commitTag, Channel &theChannel,
        FEM_ObjectBroker &theBroker)
{
    //Guanzhou implemented for parallel processing
    int dataTag = this->getDbTag();
    static ID idData(7);

    if (theChannel.receiveID(dataTag, commitTag, idData) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv ID\n";
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
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf() - ran out of memory constructing";
        cerr << " a ID of size: " <<  BoundaryNodes->Size() << endln;
        exit(1);
    }

    if (theChannel.receiveID(dataTag, commitTag, *BoundaryNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv BoundaryNodes\n";
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
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf() - ran out of memory constructing";
        cerr << " a ID of size: " << ExteriorNodes->Size() << endln;
        exit(1);
    }

    if (theChannel.receiveID(dataTag, commitTag, *ExteriorNodes) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv ExteriorNodes\n";
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
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf() - ran out of memory constructing";
        cerr << " a Matrix of size (cols*rows): " << col << " * " << row << endln;
        exit(1);
    }

    if (theChannel.receiveMatrix(dataTag, commitTag, *PBowlLoads) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv Matrix PBowlLoads\n";
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
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf() - ran out of memory constructing";
        cerr << " a ID of size: " << NodeID->Size() << endln;
        exit(1);
    }

    if (theChannel.receiveID(dataTag, commitTag, *NodeID) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv NodeID\n";
        return -1;
    }

    static Vector data(2);

    if (theChannel.receiveVector(dataTag, commitTag, data) < 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::receiveSelf -- failed to recv data\n";
        return -1;
    }

    PBTimeIncr =   data(0);
    cFactor    =   data(1);

    LoadComputed = true;


    return 0;

}

void
Domain_Reduction_Method_Modified_Input::Print(ostream &s, int flag)
{
    s << "Domain_Reduction_Method_Modified_Input " << this->getTag() << endln;
    s << "thetimeSteps " << thetimeSteps << endln;
    s << "BoundaryNodes " << *BoundaryNodes << endln;
    s << "ExteriorNodes " << *ExteriorNodes << endln;
    s << "PBowlLoads rows " << PBowlLoads->noRows() << endln;
    s << "PBowlLoads cols " << PBowlLoads->noCols() << endln;
    s << "NodeID " << *NodeID << endln;
}



// method to obtain a blank copy of the LoadPattern
LoadPattern *
Domain_Reduction_Method_Modified_Input::getCopy(void)
{
    // cout.flush() << "getcopy DRM" << endl;

    // Domain_Reduction_Method_Modified_Input *theCopy = new Domain_Reduction_Method_Modified_Input(this->getTag(),
    //         DRMElementsFile,
    //         DRMNodesFile,
    //         DRMDisplacementsFile,
    //         DRMAccelerationsFile,
    //         PBTimeIncr,
    //         cFactor,
    //         thetimeSteps,
    //         number_of_DRM_elements,
    //         number_of_DRM_nodes,
    //         xPlus,
    //         xMinus,
    //         yPlus,
    //         yMinus,
    //         zPlus,
    //         zMinus);

    // if (theCopy == 0)
    // {
    //     cerr << "omain_Reduction_Method_Modified_Input::getCopy() - ran out of memory\n";
    //     return theCopy; // in case fatal() does not exit
    // }


    // theCopy->BoundaryNodes = BoundaryNodes;
    // theCopy->ExteriorNodes = ExteriorNodes;
    // theCopy->PBowlLoads = PBowlLoads;
    // theCopy->NodeID = NodeID;

    return 0;
}

void
Domain_Reduction_Method_Modified_Input::CompPBLoads()
{
    ///////////////////////////////////////
    // Figure out the interior and exterior nodes //Guanzhou added
    ////////////////////////////////

    cout << "Domain_Reduction_Method_Modified_Input::CompPBLoads()\n";

    clock_t init, final;

    init = clock();

    Domain *theDomain = this->getDomain();

    //Assume all the plastic bowl elements have the same number of nodes
    // Also the same number of DOFS
    Element *theElement = theDomain->getElement( (*PBowlElements)(0) );
    if (theElement == 0)
    {
        cerr << "Element number " << (*PBowlElements)(0) << " not found!!\n";
    }
    int NIE = theElement->getNumExternalNodes();

    int max_bnode = PBowlElements->Size() * NIE;
    ID *Bowl_node = new ID(max_bnode);
    ID NodesinFirstEle = theElement->getExternalNodes();

    int i, j, k;

    //Inital node list from the first plastic bowl element
    for (i = 0; i < NIE; i++)
    {
        (*Bowl_node)(i) = NodesinFirstEle(i);
    }

    int no_bnode = NIE;
    int no_boundarynodes = number_of_DRM_nodes_b;
    int no_exteriornodes = number_of_DRM_nodes_e;
    int Bowl_elem_nb = PBowlElements->Size();
    ID Temp;

# ifdef _PARALLEL_PROCESSING

    if ( theElement != NULL )
    {
        //    delete theElement;
        theElement = NULL;
    }

# endif

    cout << "Domain_Reduction_Method_Modified_Input::CompPBLoads() - Bowl nodes: \n";
    for ( i = 1; i < Bowl_elem_nb; i++)
    {
        cout << i << "(" << (*PBowlElements)(i) << ") ";
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
            //     delete theElement;
            theElement = NULL;
        }

# endif
    }



    //=======================================================================
    // Computing the equivalent(effective) forces for all plastic bowl nodes
    //=======================================================================

    //int no_boundarynodes = BoundaryNodes->Size();
    cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads() - # Boundary Nodes: " << no_boundarynodes << endln;
    cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads() - # Exterior Nodes: " << no_exteriornodes << endln;

    // if ( no_exteriornodes + no_boundarynodes != num)
    // {
    //     cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads() - Error!\n";
    //     cerr << "Number of nodes in PBElement does not equal to NodeID size!\n";
    //     exit(1);
    // }

    cerr << "Computing the equivalent(effective) forces for all plastic bowl nodes" << endln;
    int cols = Udd->noRows();
    //Matrix to hold the computed effective nodal forces for all plastic bowl DOFs at each time step
    Matrix *F = new Matrix(cols, thetimeSteps);

    //Assume all plastic bowl nodes have the same number of DOFs
# ifdef _PARALLEL_PROCESSING
    Node *theNode = theDomain->getOutsideNode((*BoundaryNodes)(0));
# else
    Node *theNode = theDomain->getNode((*BoundaryNodes)(0));
# endif

    int NDOF = theNode->getNumberDOF();

# ifdef _PARALLEL_PROCESSING

    if ( theDomain->Domain::getNode( (*BoundaryNodes)(0) )  == NULL )
    {
        theNode = NULL;
    }

# endif

    Vector *Fm = new Vector(NIE * NDOF);
    Vector *Fk  = new Vector(NIE * NDOF);

    Vector *u_e = new Vector(NIE * NDOF);
    Vector *udd_e = new Vector(NIE * NDOF);

    init = clock();

    // intialize the F()
    for (int i = 0; i < cols; i++)
        for (int j = 0; j < thetimeSteps; j++)
        {
            (*F)(i, j) = 0;
        }

    cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): F initialization is done ..." << endln;
    final = clock() - init;
    cout << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): time spent for initialization is ..." << (double)final / ((double)CLOCKS_PER_SEC) << " seconds \n";

    Element *theBowlElements;
    ofstream fout("loads.txt", ios::out);
    for (int i = 0; i < Bowl_elem_nb; i++)
    {


        init = clock();
        // get the Brick;
        theBowlElements = theDomain->getElement( (*PBowlElements)(i) );

# ifdef _PARALLEL_PROCESSING
        theBowlElements->setDomain(theDomain);
# endif

        const ID &nd = theBowlElements->getExternalNodes();

        Matrix Me = theBowlElements ->getMass();
        Matrix Ke = theBowlElements ->getTangentStiff();



        // cout << "Element " << (*PBowlElements)(i) << " matrices:\n";
        // cout << "  M = " << Me << endl;
        // cout << "  K = " << Ke << endl;

        //-------------------------------------------------------------------------
        //Zero diagonal block entries of boundary and exterior nodes, respectively
        //-------------------------------------------------------------------------

        //Find out the boundary and exterior nodes in this element
        ID *B_node = new ID(NIE);  //array to store the indices of all boundary nodes
        ID *E_node = new ID(NIE);  //array to store the indices of all exterior nodes
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

        // fout << "Element # " << (*PBowlElements)(i) << endl;
        // fout << "    nB = " << nB << endl;
        // fout << "    nE = " << nE << endl;


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

        // cout << "      Element " << (*PBowlElements)(i) << " matrices (w/o diagonal):\n";
        // cout << "        M = " << Me << endl;
        // cout << "        K = " << Ke << endl;


        // get the u and u_dotdot for this element
        fout << "====== Element " << (*PBowlElements)(i) << " accelerations and displs:\n";
        fout << "     thetimeSteps = " << thetimeSteps << endl;
        double accnorm = 0;
        double disnorm = 0;
        for ( int t = 0; t < thetimeSteps; t++)
        {


            for (int j = 0; j < NIE; j++)
            {

                // int mid_index = 0;
                // int max_index = 0;
                // int min_index = 0;
                // max_index = NodeID->Size() - 1 ;
                // mid_index = (max_index / 2 );

                // for()
                // while ((nd(j) != (*NodeID)(mid_index)) && (min_index != mid_index) )
                // {
                //     // cout << " nd(j)                     = " << nd(j) << "\n";
                //     // cout << "  mid_index                = " << mid_index << "\n";
                //     // cout << "  max_index                = " << max_index << "\n";
                //     // cout << "  min_index                = " << min_index << "\n";
                //     // cout << "  (*NodeID)(mid_index)     = " << (*NodeID)(mid_index) << "\n";
                //     if ( nd(j) > (*NodeID)(mid_index) )
                //     {
                //         min_index = mid_index;
                //         mid_index = (min_index + max_index) / 2;

                //     }
                //     else
                //     {
                //         max_index = mid_index;
                //         mid_index = (min_index + max_index) / 2;
                //     }
                // }

                //check for the array with the size 2:
                // if (nd(j) == (*NodeID)(max_index))
                // {
                //     mid_index = max_index;
                // }

                // if ( min_index == max_index )
                // {
                //     std::cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): can not find the node # " << nd(j) << " in the DRM nodes list\n";
                //     exit(1);
                // }

                int r;
                // r = mid_index;
                r = (*NodeID)(nd(j));

                for (int d = 0; d < NDOF; d++)
                {
                    // cout << "Doing this " << NDOF << " " << d << " " <<  j << " " <<  nd(j) << " " <<  r << " " <<  endl;
                    (*u_e)(j * NDOF + d)   = (*U)(r * NDOF + d, t);
                    (*udd_e)(j * NDOF + d) = (*Udd)(r * NDOF + d, t);
                    accnorm += (*udd_e)(j * NDOF + d) * (*udd_e)(j * NDOF + d);
                    disnorm += (*u_e)(j * NDOF + d) * (*u_e)(j * NDOF + d);
                }
            }

            //-----------------------------------------------------------


            Fm->addMatrixVector(0.0, Me, (*udd_e), 1.0);
            Fk->addMatrixVector(0.0, Ke, (*u_e), 1.0);



            for (int k = 0; k < NIE; k++)
            {
                // int mid_index = 0;
                // int max_index = 0;
                // int min_index = 0;
                // max_index = NodeID->Size() - 1 ;
                // mid_index = (max_index / 2 );


                // while ((nd(k) != (*NodeID)(mid_index)) && (min_index != mid_index) )
                // {
                //     if ( nd(k) > (*NodeID)(mid_index) )
                //     {
                //         min_index = mid_index;
                //         mid_index = (min_index + max_index) / 2;

                //     }
                //     else
                //     {
                //         max_index = mid_index;
                //         mid_index = (min_index + max_index) / 2;
                //     }

                // }

                // //check for the array with the size 2:
                // if  (nd(k) == (*NodeID)(max_index))
                // {
                //     mid_index = max_index;
                // }

                // if ( min_index == max_index )
                // {
                //     std::cerr << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): can not find the node # " << nd(k) << " in the DRM nodes list\n";
                //     exit(1);
                // }

                int r;
                // r = mid_index;
                r = (*NodeID)(nd(k));

                for (int d = 0; d < NDOF; d++)
                {
                    (*F)( r * NDOF + d, t) = (*F)( r * NDOF + d, t) + (*Fk)(k * NDOF + d) + (*Fm)(k * NDOF + d);
                }
            }

        } //end for timestep
        fout << accnorm << endl;
        fout << disnorm << endl;
        final = clock() - init;
        cout << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): time spent for calculating the forces for element # " << i <<  " is " << (double)final / ((double)CLOCKS_PER_SEC) << " seconds \n";


# ifdef _PARALLEL_PROCESSING

        if ( theBowlElements != NULL )
        {
            const int num = theBowlElements->getNumExternalNodes();
            Node **nodes = theBowlElements->getNodePtrs();

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

        // final = clock() - init;
        // cout << "Domain_Reduction_Method_Modified_Input::CompPBLoads(): time spent for calculating the forces for element # " << i <<  " is " << (double)final / ((double)CLOCKS_PER_SEC) << " seconds \n";
    }  // end for bowl element


    PBowlLoads = new Matrix(*F);

    // ofstream fout("loads.txt", ios::out);
    // for (int i = 0; i < PBowlLoads->noRows(); i++)
    // {
    //     for (int j = 0; j <  PBowlLoads->noCols(); j++)
    //     {
    //         fout << (*PBowlLoads)(i, j) << " ";
    //     }
    //     fout << "\n";
    // }
    fout.close();

    cout << "PBowlLoads->noRows() == " << PBowlLoads->noRows() <<  " \n PBowlLoads->noCols() = " << PBowlLoads->noCols() << endl;

    // ensure we did not run out of memory
    if (PBowlLoads->noRows() == 0 || PBowlLoads->noCols() == 0 )
    {
        cerr << "Domain_Reduction_Method_Modified_Input::PBowlLoads() - ran out of memory";
        cerr << " a Matrix of size: " <<  PBowlLoads->noRows() << " * " << PBowlLoads->noCols() << endln;
    }

    cerr << "\nFinished calculating the forces..." << endln << endln;

    LoadComputed = true;

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


Vector *
Domain_Reduction_Method_Modified_Input::getNodalLoad(int nodeTag, double time)
{
    //Get the node
    Domain *theDomain = this->getDomain();
    Node *theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        cerr << "Domain_Reduction_Method_Modified_Input::getNodalLoad() - no nodes associtated to the nodeTag " << nodeTag << "\n";
        exit(1);
    }


    //Create the nodal load vector accoding to the DOFs the node has
    int numDOF = theNode->getNumberDOF();
    Vector *nodalLoad = new Vector(numDOF);


    //Get the nodal loads associated to the nodeTag
    // check for a quick return
    if (time < 0.0 || PBowlLoads == 0)
    {
        cout << "Domain_Reduction_Method_Modified_Input::getNodalLoad()  - Warning, returning zero load! \n";
        return nodalLoad;
    }

    // determine indexes into the data array whose boundary holds the time
    double incr = time / PBTimeIncr;
    int incr1 = (int)floor(incr);
    int incr2 = incr1 + 1;

    double value1 = 0;
    double value2 = 0;


    if (incr2 > thetimeSteps )
    {
        return nodalLoad;
    }

    //If within time step, return interpolated values
    else
    {
        for (int i = 0; i < numDOF; i++)
        {
            //Guanzhou introduce new data structure!!!
            int r;

            // for (r = 0; r < NodeID->Size(); r++)
            // {
            //     if ( nodeTag == (*NodeID)(r) )
            //     {
            //         break;
            //     }
            // }

            r = (*NodeID)(nodeTag);

            value1 = (*PBowlLoads)(numDOF * r + i, incr1);
            value2 = (*PBowlLoads)(numDOF * r + i, incr2);

            (*nodalLoad)(i) = cFactor * (value1 + (value2 - value1) * (time / PBTimeIncr - incr1));

            // cout <<   "       r          = " << r      << endl;
            // cout <<   "       i          = " << i      << endl;
            // cout <<   "       value1     = " << value1 << endl;
            // cout <<   "       value2     = " << value2 << endl;
            // cout <<   "       cFactor    = " << cFactor << endl;
            // cout <<   "       time       = " << time << endl;
            // cout <<   "       PBTimeIncr = " << PBTimeIncr << endl;
            // cout <<   "       incr1      = " << incr1 << endl;
        }
    }

    // cout << "Nodal load for node tag " << nodeTag << " at time " << time << " is :";
    // cout << *nodalLoad << endl;

    return nodalLoad;
}

