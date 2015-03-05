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
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              March 2015
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




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





















