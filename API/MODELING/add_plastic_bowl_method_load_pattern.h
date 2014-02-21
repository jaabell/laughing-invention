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
// DATE:              December 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Defining a Plastic Bowl Method Load Pattern</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of a Plastic Bowl Method (DRM) load pattern.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


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



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




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





















