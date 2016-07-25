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
//! - PatternNumber: number assigned to DRM load pettern
//! - dt: time interval of input files for time histories
//! - factor: factor to multiply to the input time history
//! - numberofsteps: Number of the time steps in acceleration/displacement time history
//! - numberofdrmnodes: Number of the nodes in DRM layer
//! - numberofdrmelements: Number of the elements in DRM layer
//! - xpositive, xminus: boundary layer range in x direction
//! - ypositive, yminus: boundary layer range in y direction
//! - zpositive, zminus: boundary layer range in z direction
//! - ElementNumberFile: File including element numbers inside the platic bowl (1 element number per line in the input file)
//! - NodeNumbersFilename: File including node numbers inside the platic bowl (1 node number per line in the input file)
//! - DisplacementTimeHistoryFile: File including displacement time history (in each line write the values of displacement in time
//!   for first degree of freedom of the first node defined in NodeNumbersFilename, next line should have the values for second dof of the first node
//!   and continue for all degrees of freedom. Then move to the second node defined in NodeNumbersFilename and ...
//! - AccelerationTimeHistoryFile: File including acceleration time history (in each line write the values of displacement in time
//!   for first degree of freedom of the first node defined in NodeNumbersFilename, next line should have the values for second dof of the first node
//!   and continue for all degrees of freedom. Then move to the second node defined in NodeNumbersFilename and ...




int add_load_pattern_domain_reduction_method(
    int PatternNumber,
    double dt,
    double factor,
    int numberofsteps,
    int numberofdrmnodes_b,
    int numberofdrmnodes_e,
    int numberofdrmelements,
    string ElementNumbersFilename,
    string NodeNumbersFilename_b,
    string NodeNumbersFilename_e,
    string DisplacementTimeHistoryFilename,
    string AccelerationTimeHistoryFilename)
{

    // const char *ElementNumbers = ElementNumbersFilename.c_str();
    // const char *NodeNumbers = NodeNumbersFilename.c_str();
    // const char *DisplacementTimeHistory = DisplacementTimeHistoryFilename.c_str();
    // const char *AccelerationTimeHistory = AccelerationTimeHistoryFilename.c_str();
    // int tag,
    // const char *ElementsFilename,
    // const char *DispfName,
    // const char *AccefName,
    // const char *boundaryNodesFile,
    // const char *externalNodesFile,
    // double theTimeIncr,
    // double theFactor,
    // int timeSteps,
    // int Number_of_DRM_Elements,
    // int Number_of_DRM_Nodes_b,
    // int Number_of_DRM_Nodes_e);
    LoadPattern *theDRMPattern = 0;
    theDRMPattern = new Domain_Reduction_Method_Modified_Input(
        PatternNumber,
        ElementNumbersFilename.c_str(),
        DisplacementTimeHistoryFilename.c_str(),
        AccelerationTimeHistoryFilename.c_str(),
        NodeNumbersFilename_b.c_str(),
        NodeNumbersFilename_e.c_str(),
        dt,
        factor,
        numberofsteps,
        numberofdrmelements,
        numberofdrmnodes_b,
        numberofdrmnodes_e);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};





















