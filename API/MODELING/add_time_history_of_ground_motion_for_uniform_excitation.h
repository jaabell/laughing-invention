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
// DATE:              September 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Time History for Uniform Excitation</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of time history for uniform excitation.
//! \n
//! \n
//! For more information <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!



//! Inputs:
//! - ExcitationNumber: tag of excitation type
//! - timestep: time interval of the data points
//! - scale: load-factor coefficient
//! - vel0: initial velocity
//! - inputfilename: name of the input file of time history
//! - dataToApply: type of data to be applied to the system (displacement/velocity/acceleration)
//! - direction: direction of the wave to be applied in range of dof of the node




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n


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


    if( theDomain.addLoadPattern(theUniformPattern) == false )
    {
        cerr << "Error: (add_time_history_of_ground_motion_for_uniform_excitation) LoadPattern could not be added to the domain " << endl;
        abort();
    }

    return 0;
};
