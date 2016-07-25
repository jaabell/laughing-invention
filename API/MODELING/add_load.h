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
//! - LoadNumber: integer tag identifying the load
//! - NodeNumber: tag of the node that load is to be applied
//! - vectorvalue(s): load values that are to be applied to the node corresponding to the dofs



int add_load(int LoadNumber,
             int NodeNumber,
             double vectorvalue1, double vectorvalue2, double vectorvalue3)
{


    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theSeries!" << endl;
        return -1;
    }

    //     int LoadNumber = StageNumber * 10 + 1;

    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (start_new_stage) LoadPattern " << LoadNumber << " could not be added to the domain " << endl;
        return -1;
    }





    Vector theLoadValues(3);
    theLoadValues(0) =  vectorvalue1;
    theLoadValues(1) =  vectorvalue2;
    theLoadValues(2) =  vectorvalue3;

    NodalLoad* theLoad = 0;
    theLoad = new NodalLoad(LoadNumber, NodeNumber, theLoadValues);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load) memory allocating for nodal load!" << endl;
        return -1;
    }


    if ( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
    {
        cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};





//! Inputs:
//! - LoadNumber: integer tag identifying the load
//! - NodeNumber: tag of the node that load is to be applied
//! - vectorvalue(s): load values that are to be applied to the node corresponding to the dofs


int add_load(int LoadNumber,
             int NodeNumber,
             double vectorvalue1, double vectorvalue2, double vectorvalue3,
             double vectorvalue4, double vectorvalue5, double vectorvalue6)
{


    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theSeries!" << endl;
        return -1;
    }

    //     int LoadNumber = StageNumber * 10 + 1;

    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (start_new_stage) LoadPattern " << LoadNumber << " could not be added to the domain " << endl;
        return -1;
    }



    Vector theLoadValues(6);
    theLoadValues(0) =  vectorvalue1;
    theLoadValues(1) =  vectorvalue2;
    theLoadValues(2) =  vectorvalue3;
    theLoadValues(3) =  vectorvalue4;
    theLoadValues(4) =  vectorvalue5;
    theLoadValues(5) =  vectorvalue6;

    NodalLoad* theLoad = 0;
    theLoad = new NodalLoad(LoadNumber, NodeNumber, theLoadValues);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load) memory allocating for nodal load!" << endl;
        return -1;
    }


    if ( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
    {
        cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






//! Inputs:
//! - LoadNumber: integer tag identifying the load
//! - NodeNumber: tag of the node that load is to be applied
//! - vectorvalue(s): load values that are to be applied to the node corresponding to the dofs



int add_load(int LoadNumber,
             int NodeNumber,
             double vectorvalue1, double vectorvalue2,
             double vectorvalue3, double vectorvalue4,
             double vectorvalue5, double vectorvalue6,
             double vectorvalue7)
{


    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theSeries!" << endl;
        return -1;
    }

    //     int LoadNumber = StageNumber * 10 + 1;

    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (start_new_stage) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (start_new_stage) LoadPattern " << LoadNumber << " could not be added to the domain " << endl;
        return -1;
    }



    Vector theLoadValues(7);
    theLoadValues(0) =  vectorvalue1;
    theLoadValues(1) =  vectorvalue2;
    theLoadValues(2) =  vectorvalue3;
    theLoadValues(3) =  vectorvalue4;
    theLoadValues(4) =  vectorvalue5;
    theLoadValues(5) =  vectorvalue6;
    theLoadValues(6) =  vectorvalue7;

    NodalLoad* theLoad = 0;
    theLoad = new NodalLoad(LoadNumber, NodeNumber, theLoadValues);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load) memory allocating for nodal load!" << endl;
        return -1;
    }


    if ( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
    {
        cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};



