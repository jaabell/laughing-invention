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
//! - GroundMotionNumber: tag of ground motion
//! - ExcitationNumber: tag of excitation type
//! - timestep: time interval of the data points
//! - scale: load-factor coefficient
//! - inputfilename: name of the input file of time history
//! - dataToApply: type of data to be applied to the system (displacement/velocity/acceleration)



int add_imposed_motion(int GroundMotionNumber,
                       int NodeNumber,
                       int degree_of_freedom,
                       double timestep,
                       double displacement_scale,
                       string displacementfilename,
                       double velocity_scale,
                       string velocityfilename,
                       double acceleration_scale,
                       string accelerationfilename)
{



    LoadPattern* thePatternforMultiSupport = 0;
    MultiSupportPattern* theMultiSupportPattern = 0;
    theMultiSupportPattern = new MultiSupportPattern(GroundMotionNumber);

    if (theMultiSupportPattern == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for theMultiSupportPattern!" << endl;
        return -1;
    }


    if ( theDomain.addMultipleSupport(*theMultiSupportPattern) != 0 )
    {
        cerr << "Error: (add_imposed_motion) theMultiSupportPattern could not be added to the domain " << endl;
        return -1;
    }

    thePatternforMultiSupport = theMultiSupportPattern;

    if (thePatternforMultiSupport == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for thePatternforMultiSupport!" << endl;
        return -1;
    }

    if ( theDomain.addLoadPattern(thePatternforMultiSupport) == false )
    {
        cerr << "Error: (add_imposed_motion) thePatternforMultiSupport could not be added to the domain " << endl;
        return -1;
    }




    TimeSeries* accelSeries = 0;
    TimeSeries* velSeries = 0;
    TimeSeries* dispSeries = 0;
    TimeSeriesIntegrator* seriesIntegrator = 0;


    const char* displacementinputfilename = displacementfilename.c_str();
    const char* velocityinputfilename     = velocityfilename.c_str();
    const char* accelerationinputfilename = accelerationfilename.c_str();



    dispSeries =  new PathSeries (displacementinputfilename, timestep, displacement_scale);
    velSeries =   new PathSeries (velocityinputfilename, timestep, velocity_scale);
    accelSeries = new PathSeries (accelerationinputfilename, timestep, acceleration_scale);



    if ( (dispSeries == NULL) || (velSeries == NULL) || (accelSeries == NULL) )
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }


    GroundMotion* theMotion = 0;
    theMotion = new GroundMotion(dispSeries, velSeries, accelSeries, seriesIntegrator);

    if (theMotion == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }


    MultiSupportPattern* theMultiSupportget = 0;
    theMultiSupportget = theDomain.getMultipleSupport(GroundMotionNumber);

    if (theMultiSupportget == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }

    if ( theMultiSupportget->addMotion(*theMotion, GroundMotionNumber) != 0 )
    {
        cerr << "Error: (add_imposed_motion) LoadPattern could not be added to multiple support pattern " << endl;
        return -1;
    }



    //====================================================================================
    SP_ConstraintIter& theSPs = thePatternforMultiSupport->getSPs();
    int numSPs = 0;
    SP_Constraint* theSP2;

    while ((theSP2 = theSPs()) != 0)
    {
        numSPs++;
    }

    SP_Constraint* theSPMotion = 0;
    theSPMotion = new ImposedMotionSP (numSPs, NodeNumber, degree_of_freedom - 1, *theMotion, GroundMotionNumber, false);

    if (theSPMotion == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for theSPMotion!" << endl;
        return -1;
    }

    if (thePatternforMultiSupport->addSP_Constraint(theSPMotion) == false )
    {
        cerr << "Error: (add_imposed_motion) thePattern->addSP_Constraint " << endl;
        return -1;
    }

    //====================================================================================


    return 0;

};






int add_imposed_motion(int GroundMotionNumber,
                       int NodeNumber,
                       int degree_of_freedom,
                       double displacement_scale,
                       string displacementfilename,
                       double velocity_scale,
                       string velocityfilename,
                       double acceleration_scale,
                       string accelerationfilename)
{



    LoadPattern* thePatternforMultiSupport = 0;
    MultiSupportPattern* theMultiSupportPattern = 0;
    theMultiSupportPattern = new MultiSupportPattern(GroundMotionNumber);

    if (theMultiSupportPattern == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for theMultiSupportPattern!" << endl;
        return -1;
    }


    if ( theDomain.addMultipleSupport(*theMultiSupportPattern) != 0 )
    {
        cerr << "Error: (add_imposed_motion) theMultiSupportPattern could not be added to the domain " << endl;
        return -1;
    }

    thePatternforMultiSupport = theMultiSupportPattern;

    if (thePatternforMultiSupport == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for thePatternforMultiSupport!" << endl;
        return -1;
    }

    if ( theDomain.addLoadPattern(thePatternforMultiSupport) == false )
    {
        cerr << "Error: (add_imposed_motion) thePatternforMultiSupport could not be added to the domain " << endl;
        return -1;
    }




    TimeSeries* accelSeries = 0;
    TimeSeries* velSeries = 0;
    TimeSeries* dispSeries = 0;
    TimeSeriesIntegrator* seriesIntegrator = 0;


    const char* displacementinputfilename = displacementfilename.c_str();
    const char* velocityinputfilename     = velocityfilename.c_str();
    const char* accelerationinputfilename = accelerationfilename.c_str();



    dispSeries =  new PathTimeSeries (displacementinputfilename, displacement_scale);
    velSeries =   new PathTimeSeries (velocityinputfilename, velocity_scale);
    accelSeries = new PathTimeSeries (accelerationinputfilename, acceleration_scale);



    if ( (dispSeries == NULL) || (velSeries == NULL) || (accelSeries == NULL) )
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }


    GroundMotion* theMotion = 0;
    theMotion = new GroundMotion(dispSeries, velSeries, accelSeries, seriesIntegrator);

    if (theMotion == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }


    MultiSupportPattern* theMultiSupportget = 0;
    theMultiSupportget = theDomain.getMultipleSupport(GroundMotionNumber);

    if (theMultiSupportget == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating!" << endl;
        return -1;
    }

    if ( theMultiSupportget->addMotion(*theMotion, GroundMotionNumber) != 0 )
    {
        cerr << "Error: (add_imposed_motion) LoadPattern could not be added to multiple support pattern " << endl;
        return -1;
    }



    //====================================================================================
    SP_ConstraintIter& theSPs = thePatternforMultiSupport->getSPs();
    int numSPs = 0;
    SP_Constraint* theSP2;

    while ((theSP2 = theSPs()) != 0)
    {
        numSPs++;
    }

    SP_Constraint* theSPMotion = 0;
    theSPMotion = new ImposedMotionSP (numSPs, NodeNumber, degree_of_freedom - 1, *theMotion, GroundMotionNumber, false);

    if (theSPMotion == NULL)
    {
        cerr << "Error: (add_imposed_motion) memory allocating for theSPMotion!" << endl;
        return -1;
    }

    if (thePatternforMultiSupport->addSP_Constraint(theSPMotion) == false )
    {
        cerr << "Error: (add_imposed_motion) thePattern->addSP_Constraint " << endl;
        return -1;
    }

    //====================================================================================


    return 0;

};


