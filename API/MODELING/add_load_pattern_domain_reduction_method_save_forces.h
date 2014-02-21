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
// DATE:              April 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int add_load_pattern_domain_reduction_method_save_forces(int PatternNumber,
        double dt,
        double factor,
        int numberofsteps,
        int numberofdrmnodes,
        int numberofdrmelements,
        double xpositive,
        double xminus,
        double ypositive,
        double yminus,
        double zpositive,
        double zminus,
        string ElementNumbersFilename,
        string NodeNumbersFilename,
        string DisplacementTimeHistoryFilename,
        string AccelerationTimeHistoryFilename,
        string ForceTimeHistoryFilename)
{

    const char* ElementNumbers = ElementNumbersFilename.c_str();
    const char* NodeNumbers = NodeNumbersFilename.c_str();
    const char* DisplacementTimeHistory = DisplacementTimeHistoryFilename.c_str();
    const char* AccelerationTimeHistory = AccelerationTimeHistoryFilename.c_str();
    const char* ForceTimeHistory = ForceTimeHistoryFilename.c_str();

    LoadPattern* theDRMPattern = 0;
    theDRMPattern = new Domain_Reduction_Method_Save_Forces(PatternNumber, ElementNumbers, NodeNumbers, DisplacementTimeHistory, AccelerationTimeHistory, ForceTimeHistory, dt, factor, numberofsteps, numberofdrmelements, numberofdrmnodes, xpositive, xminus, ypositive, yminus, zpositive, zminus);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_save_forces) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_save_forces) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};





















