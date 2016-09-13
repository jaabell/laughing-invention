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
//! - ElementNumber: tag of the element that selfweight is to be applied




int add_load_selfweight_to_element(int SelfWeightNumber,
                                   int ElementNumber, int AccelerationFieldNumber)
{

    // New load number is the same as pattern number
    int LoadPatternNumber = SelfWeightNumber;

    // Linear time series for the self-weight application on current stage
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


    ///////////////////////////////////////////////////////////////////////////////////


    AccelerationField* theaccelerationfield = 0;
    theaccelerationfield = theDomain.getAccelerationField(AccelerationFieldNumber);

    if (theaccelerationfield == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theaccelerationfield!" << endl;
        return -1;
    }


    Vector accelerationfield(3);
    accelerationfield = theaccelerationfield->getAccelerationFieldVector();

    ///////////////////////////////////////////////////////////////////////////////////


    ID theEleTags (0, 1);
    theEleTags [0] = ElementNumber;
    ElementalLoad* theLoad = 0;
    theLoad = new ElementSelfWeight (SelfWeightNumber , theEleTags, accelerationfield);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};



int add_load_selfweight_to_elements(int SelfWeightNumber,
                                    std::vector<int> &ElementNumbers, int AccelerationFieldNumber)
{

    // New load number is the same as pattern number
    // int LoadPatternNumber = SelfWeightNumber;
    int LoadPatternNumber = SelfWeightNumber;//theDomain.getNumLoadPatterns( ) + 1;

    // Linear time series for the self-weight application on current stage
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


    ///////////////////////////////////////////////////////////////////////////////////


    AccelerationField* theaccelerationfield = 0;
    theaccelerationfield = theDomain.getAccelerationField(AccelerationFieldNumber);

    if (theaccelerationfield == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theaccelerationfield!" << endl;
        return -1;
    }


    Vector accelerationfield(3);
    accelerationfield = theaccelerationfield->getAccelerationFieldVector();

    ///////////////////////////////////////////////////////////////////////////////////

    for (const int& tag : ElementNumbers)
    {
        ID theEleTags (0, 1);
        theEleTags [0] = tag;
        ElementalLoad* theLoad = 0;
        // SelfWeightNumber =  tag;
        theLoad = new ElementSelfWeight (tag , theEleTags, accelerationfield);

        if (theLoad == NULL)
        {
            cerr << "Error: (add_load_selfweight_to_element) memory allocating theLoad!" << endl;
            return -1;
        }


        if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
        {
            cerr << "Error: (add_load_selfweight_to_element) Elemental load could not be added to the domain " << endl;
            return -1;
        }
    }
    return 0;
};

int add_load_selfweight_to_all_elements(int SelfWeightNumber, int AccelerationFieldNumber)
{

    // New load number is the same as pattern number
    // int LoadPatternNumber = SelfWeightNumber;
    int LoadPatternNumber = SelfWeightNumber;//theDomain.getNumLoadPatterns( ) + 1;

    // Linear time series for the self-weight application on current stage
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


    ///////////////////////////////////////////////////////////////////////////////////


    AccelerationField* theaccelerationfield = 0;
    theaccelerationfield = theDomain.getAccelerationField(AccelerationFieldNumber);

    if (theaccelerationfield == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocation problem for theaccelerationfield!" << endl;
        return -1;
    }


    Vector accelerationfield(3);
    accelerationfield = theaccelerationfield->getAccelerationFieldVector();

    ///////////////////////////////////////////////////////////////////////////////////

    // for (const int& tag : ElementNumbers)
    ElementIter &element_iter = theDomain.getElements();
    Element * ele = 0;
    ID theEleTags (theDomain.getNumElements());
    int i = 0;
    while ((ele = element_iter()) != 0)
    {
        int tag = ele->getTag();
        theEleTags [i] = tag;
        i++;
    }

    ElementalLoad* theLoad = new ElementSelfWeight (SelfWeightNumber , theEleTags, accelerationfield);

    if (theLoad == NULL)
    {
        cerr << "Error: (add_load_selfweight_to_element) memory allocating theLoad!" << endl;
        return -1;
    }


    if ( theDomain.addElementalLoad(theLoad, LoadPatternNumber) == false )
    {
        cerr << "Error: (add_load_selfweight_to_element) Elemental load could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};
