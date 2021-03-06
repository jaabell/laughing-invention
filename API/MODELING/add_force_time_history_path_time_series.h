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
//! -



int add_force_time_history_path_time_series(int PatternNumber,
        int NodeNumber,
        int dof_to_be_shaken,
        double LoadFactor,
        string Forceinputfilename)
{

    const char* filename = Forceinputfilename.c_str();


    LoadPattern* thePathSeriesPattern = 0;
    thePathSeriesPattern = new PathTimeSeries_Force_TimeHistory(PatternNumber,
            NodeNumber,
            dof_to_be_shaken,
            LoadFactor,
            filename);

    if (thePathSeriesPattern == 0)
    {
        cerr << "WARNING: (add_force_time_history_ormsby_wavelet) ran out of memory creating load pattern ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(thePathSeriesPattern) == false)
    {
        cerr << "WARNING: (add_force_time_history_ormsby_wavelet) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;

};





