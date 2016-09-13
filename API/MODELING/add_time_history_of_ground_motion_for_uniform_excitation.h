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
//! - ExcitationNumber: tag of excitation type
//! - timestep: time interval of the data points
//! - scale: load-factor coefficient
//! - vel0: initial velocity
//! - inputfilename: name of the input file of time history
//! - dataToApply: type of data to be applied to the system (displacement/velocity/acceleration)
//! - direction: direction of the wave to be applied in range of dof of the node

int add_time_history_of_ground_motion_for_uniform_excitation(int ExcitationNumber,
        double timestep,
        double scale,
        double vel0,
        string inputfilename_string,
        int direction)
{

    const char* inputfilename = inputfilename_string.c_str();

    TimeSeries* accelSeries = 0;
    TimeSeries* velSeries = 0;
    TimeSeries* dispSeries = 0;
    TimeSeriesIntegrator* seriesIntegrator = 0;

    accelSeries = new PathSeries (inputfilename, timestep, scale);


    if ( (dispSeries == NULL) && (velSeries == NULL) && (accelSeries == NULL) )
    {
        cerr << "Error: (add_time_history_of_ground_motion_for_uniform_excitation) memory allocating!" << endl;
        abort();
    }


    GroundMotion* theMotion = 0;
    theMotion = new GroundMotion(dispSeries, velSeries, accelSeries, seriesIntegrator);

    if (theMotion == NULL)
    {
        cerr << "Error: (add_time_history_of_ground_motion_for_uniform_excitation) memory allocating!" << endl;
        abort();
    }


    LoadPattern* theUniformPattern = 0;
    theUniformPattern = new UniformExcitation(*theMotion, direction - 1, ExcitationNumber, vel0);

    if (theUniformPattern == NULL)
    {
        cerr << "Error: (add_time_history_of_ground_motion_for_uniform_excitation) memory allocating!" << endl;
        abort();
    }


    if ( theDomain.addLoadPattern(theUniformPattern) == false )
    {
        cerr << "Error: (add_time_history_of_ground_motion_for_uniform_excitation) LoadPattern could not be added to the domain " << endl;
        abort();
    }

    return 0;
};
