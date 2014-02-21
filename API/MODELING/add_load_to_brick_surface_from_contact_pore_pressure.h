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
// DATE:              December 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int add_load_normal_pressure_to_8node_brick_surface_from_coupled_contact_element(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        double CoupledContactElement1,
        double CoupledContactElement2,
        double CoupledContactElement3,
        double CoupledContactElement4)
{


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad_from_Contact_Pore_Pressure(SurfaceLoadNumber , theEleTags, Node_1, Node_2, Node_3, Node_4,
            CoupledContactElement1, CoupledContactElement2, CoupledContactElement3,
            CoupledContactElement4);



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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_8node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_8node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};







int add_load_normal_pressure_to_20node_brick_surface_from_coupled_contact_element(int SurfaceLoadNumber,
        int ElementNumber,
        int Node_1,
        int Node_2,
        int Node_3,
        int Node_4,
        int Node_5,
        int Node_6,
        int Node_7,
        int Node_8,
        double CoupledContactElement1,
        double CoupledContactElement2,
        double CoupledContactElement3,
        double CoupledContactElement4,
        double CoupledContactElement5,
        double CoupledContactElement6,
        double CoupledContactElement7,
        double CoupledContactElement8)
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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad_from_Contact_Pore_Pressure(SurfaceLoadNumber , theEleTags,
            Node_1, Node_2, Node_3, Node_4,
            Node_5, Node_6, Node_7, Node_8,
            CoupledContactElement1, CoupledContactElement2, CoupledContactElement3,
            CoupledContactElement4, CoupledContactElement5, CoupledContactElement6,
            CoupledContactElement7, CoupledContactElement8);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_20node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_20node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






int add_load_normal_pressure_to_27node_brick_surface_from_coupled_contact_element(int SurfaceLoadNumber,
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
        double CoupledContactElement1,
        double CoupledContactElement2,
        double CoupledContactElement3,
        double CoupledContactElement4,
        double CoupledContactElement5,
        double CoupledContactElement6,
        double CoupledContactElement7,
        double CoupledContactElement8,
        double CoupledContactElement9)
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

    if( theDomain.addLoadPattern(theLoadPattern) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) LoadPattern " << LoadPatternNumber << " could not be added to the domain " << endl;
        return -1;
    }



    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new BrickSurfaceLoad_from_Contact_Pore_Pressure(SurfaceLoadNumber , theEleTags,
            Node_1, Node_2, Node_3, Node_4,
            Node_5, Node_6, Node_7, Node_8, Node_9,
            CoupledContactElement1, CoupledContactElement2, CoupledContactElement3,
            CoupledContactElement4, CoupledContactElement5, CoupledContactElement6,
            CoupledContactElement7, CoupledContactElement8, CoupledContactElement9);


    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_different_normal_pressure_to_27node_brick_surface) memory allocating theLoad!" << endl;
        return -1;
    }


    if( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_different_normal_pressure_to_27node_brick_surface) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




