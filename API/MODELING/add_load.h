///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              May 2009
// UPDATE HISTORY:    October 2009
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Nodal Load - Linear</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of linear nodal load.
//! \n
//! \n Currently if works for 3 dofs (brick elements), 6 dofs (beam elements) and 7 dofs (multi-phase brick elements).
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!



//! Inputs:
//! - LoadNumber: integer tag identifying the load
//! - NodeNumber: tag of the node that load is to be applied
//! - vectorvalue(s): load values that are to be applied to the node corresponding to the dofs




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
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


    if( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
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


    if( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
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


    if( theDomain.addNodalLoad(theLoad, LoadNumber) == false )
    {
        cerr << "Error: (add_load) Nodal load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};



