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



int add_load_pattern_domain_reduction_method_hdf5(
    int PatternNumber,
    string hdf5_file)
{


    LoadPattern *theDRMPattern = 0;
    theDRMPattern = new Domain_Reduction_Method_HDF5_input(
        PatternNumber,
        hdf5_file);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_hdf5) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_hdf5) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};





int add_load_pattern_domain_reduction_method_hdf5(
    int PatternNumber,
    string hdf5_file,
    double scale_factor)
{


    LoadPattern *theDRMPattern = 0;
    theDRMPattern = new Domain_Reduction_Method_HDF5_input(
        PatternNumber,
        hdf5_file,
        scale_factor);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_hdf5) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_hdf5) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};




















