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



int add_load_constant_normal_pressure_to_8node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        double SurfaceLoadMagnitude)
{


    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }



    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags, Node_1, Node_2, Node_3, Node_4,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude,
                                   SurfaceLoadMagnitude);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_8node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_8node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




int add_load_different_normal_pressure_to_8node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        double SurfaceLoadMagnitude1,
        double SurfaceLoadMagnitude2,
        double SurfaceLoadMagnitude3,
        double SurfaceLoadMagnitude4)
{
    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags, Node_1, Node_2, Node_3, Node_4,
                                   SurfaceLoadMagnitude1, SurfaceLoadMagnitude2, SurfaceLoadMagnitude3,
                                   SurfaceLoadMagnitude4);



    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_8node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_8node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};








int add_load_constant_normal_pressure_to_20node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        int Node_5,
        int Node_6,
        int Node_7,
        int Node_8,
        double SurfaceLoadMagnitude)
{


    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags,
                                   Node_1, Node_2, Node_3, Node_4,
                                   Node_5, Node_6, Node_7, Node_8,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_20node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_20node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






int add_load_different_normal_pressure_to_20node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        int Node_5,
        int Node_6,
        int Node_7,
        int Node_8,
        double SurfaceLoadMagnitude1,
        double SurfaceLoadMagnitude2,
        double SurfaceLoadMagnitude3,
        double SurfaceLoadMagnitude4,
        double SurfaceLoadMagnitude5,
        double SurfaceLoadMagnitude6,
        double SurfaceLoadMagnitude7,
        double SurfaceLoadMagnitude8)
{



    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags,
                                   Node_1, Node_2, Node_3, Node_4,
                                   Node_5, Node_6, Node_7, Node_8,
                                   SurfaceLoadMagnitude1, SurfaceLoadMagnitude2, SurfaceLoadMagnitude3,
                                   SurfaceLoadMagnitude4, SurfaceLoadMagnitude5, SurfaceLoadMagnitude6,
                                   SurfaceLoadMagnitude7, SurfaceLoadMagnitude8);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_20node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_20node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};







int add_load_constant_normal_pressure_to_27node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        int Node_5,
        int Node_6,
        int Node_7,
        int Node_8,
        int Node_9,
        double SurfaceLoadMagnitude)
{



    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags,
                                   Node_1, Node_2, Node_3, Node_4,
                                   Node_5, Node_6, Node_7, Node_8, Node_9,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude,
                                   SurfaceLoadMagnitude, SurfaceLoadMagnitude, SurfaceLoadMagnitude);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_27node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_constant_normal_pressure_to_27node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};





int add_load_different_normal_pressure_to_27node_brick_surface(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        int Node_5,
        int Node_6,
        int Node_7,
        int Node_8,
        int Node_9,
        double SurfaceLoadMagnitude1,
        double SurfaceLoadMagnitude2,
        double SurfaceLoadMagnitude3,
        double SurfaceLoadMagnitude4,
        double SurfaceLoadMagnitude5,
        double SurfaceLoadMagnitude6,
        double SurfaceLoadMagnitude7,
        double SurfaceLoadMagnitude8,
        double SurfaceLoadMagnitude9)
{



    int LoadPatternNumber = SurfaceLoadNumber;
    TimeSeries* theSeries = 0;
    theSeries =  new LinearSeries();

    if (theSeries == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theSeries!" << endl;
        return -1;
    }


    LoadPattern* theLoadPattern = 0;
    theLoadPattern = new LoadPattern(LoadPatternNumber);

    if (theLoadPattern == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theLoadPattern!" << endl;
        return -1;
    }

    theLoadPattern->setTimeSeries(theSeries);

    if ( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }



    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad(SurfaceLoadNumber , theEleTags,
                                   Node_1, Node_2, Node_3, Node_4,
                                   Node_5, Node_6, Node_7, Node_8, Node_9,
                                   SurfaceLoadMagnitude1, SurfaceLoadMagnitude2, SurfaceLoadMagnitude3,
                                   SurfaceLoadMagnitude4, SurfaceLoadMagnitude5, SurfaceLoadMagnitude6,
                                   SurfaceLoadMagnitude7, SurfaceLoadMagnitude8, SurfaceLoadMagnitude9);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_27node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_27node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




