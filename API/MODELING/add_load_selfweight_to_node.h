///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
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

//! Inputs:
//! - tag: integer number for load tag
//! - LoadPatternNumber: tag of the predefined constant or linear load pattern
//! - NodeNumber: tag of the node that selfweight is to be applied to




int add_load_selfweight_to_node(int SelfWeightNumber,
                                int NodeNumber, int AccelerationFieldNumber)
{

    // New load number is the same as pattern number
    int LoadPatternNumber = SelfWeightNumber;

    // Linear time series for the self-weight application on current stage
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_node) memory allocation problem for theSeries!" << endl;
        return -1;
    }

    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_node) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_node) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    ///////////////////////////////////////////////////////////////////////////////////


    AccelerationField* theaccelerationfield = 0;
    theaccelerationfield = theDomain.getAccelerationField(AccelerationFieldNumber);

    if (theaccelerationfield == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_node) memory allocation problem for theaccelerationfield!" << endl;
        return -1;
    }


    Vector accelerationfield(3);
    accelerationfield = theaccelerationfield->getAccelerationFieldVector();

    ///////////////////////////////////////////////////////////////////////////////////

    Node* theNode = theDomain.getNode(NodeNumber);

    int numDOFs = theNode->getNumberDOF();

    Vector selfweight(numDOFs);
    selfweight.Zero();

    Matrix M = theNode->getMass();

    if (numDOFs == 3)
    {
        selfweight.addMatrixVector(0, M, accelerationfield, -1.0);
    }
    else if (numDOFs == 6)
    {
        Vector acc(6);
        acc.Zero();
        acc(0) = accelerationfield(0);
        acc(1) = accelerationfield(1);
        acc(2) = accelerationfield(2);
        selfweight.addMatrixVector(0, M, acc, 1.0);  //Using convention of weight in same direction as acceleration field :/
        // selfweight.addMatrixVector(0, M, acc, -1.0);
    }
    else
    {
        cerr << "Error: (add_load_selfweight_to_node) Case for numDOFS = " << numDOFs << " not implemented." << endl;
        return -1;
    }



    NodalLoad * theLoad = 0;
    bool is_load_constant = false;
    theLoad = new NodalLoad(SelfWeightNumber, NodeNumber, selfweight, is_load_constant);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_node) memory allocating theLoad!" << endl;
    }


    if ( theDomain.addNodalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_selfweight_to_node) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    // cout << "Added selfweight = " << selfweight << " to node number " << NodeNumber << endl;

    return 0;
};
