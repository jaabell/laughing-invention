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
//! - xpositive, xminus: boundary layer range in x direction
//! - ypositive, yminus: boundary layer range in y direction
//! - zpositive, zminus: boundary layer range in z direction
//! - ElementNumberFile: File including element numbers inside the platic bowl (first line should be the total number
//!     of the elements and from the second line, 1 element number per line in the input file)
//! - DisplacementTimeHistoryFile: File including displacement time history (first line is the number of steps in time history,
//!                                second line is total number of nodes which time history is applied,
//!                                from third line write one node number per line,
//!                                then in each line write the values of displacement in all 3 directions at the
//!                                first time step of loading (one node per line in order defined from third line)
//! - AccelerationTimeHistoryFile: same as DisplacementTimeHistoryFile but for acceleration


int add_plastic_bowl_method_load_pattern(int PatternNumber,
        double dt,
        double factor,
        double xpositive,
        double xminus,
        double ypositive,
        double yminus,
        double zpositive,
        double zminus,
        string ElementNumbersFilename,
        string DisplacementTimeHistoryFilename,
        string AccelerationTimeHistoryFilename)
{

    const char* ElementNumbers = ElementNumbersFilename.c_str();
    const char* DisplacementTimeHistory = DisplacementTimeHistoryFilename.c_str();
    const char* AccelerationTimeHistory = AccelerationTimeHistoryFilename.c_str();

    LoadPattern* theDRMPattern = 0;
    theDRMPattern = new PBowlLoading(PatternNumber, ElementNumbers, DisplacementTimeHistory, AccelerationTimeHistory, dt, factor, xpositive, xminus, ypositive, yminus, zpositive, zminus);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_plastic_bowl_method_load_pattern) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_plastic_bowl_method_load_pattern) could not add load pattern to the domain " << endl;
        return -1;
        //     delete theDRMPattern;
    }

    return 0;
};





















