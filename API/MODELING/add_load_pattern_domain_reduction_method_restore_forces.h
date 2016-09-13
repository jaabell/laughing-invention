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


int add_load_pattern_domain_reduction_method_restore_forces(int PatternNumber,
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
        string ForceTimeHistoryFilename)
{

    const char* ElementNumbers = ElementNumbersFilename.c_str();
    const char* NodeNumbers = NodeNumbersFilename.c_str();
    const char* ForceTimeHistory = ForceTimeHistoryFilename.c_str();

    LoadPattern* theDRMPattern = 0;
    theDRMPattern = new Domain_Reduction_Method_Restore_Forces(PatternNumber, ElementNumbers, NodeNumbers, ForceTimeHistory, dt, factor, numberofsteps, numberofdrmelements, numberofdrmnodes, xpositive, xminus, ypositive, yminus, zpositive, zminus);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_restore_forces) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_restore_forces) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};





















