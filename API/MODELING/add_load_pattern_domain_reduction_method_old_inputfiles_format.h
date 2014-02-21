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
// DATE:              June 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Defining a Domain Reduction Method (DRM) Load Pattern</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of a Domain Reduction Method (DRM) load pattern.
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
//! - numdof: number of dofs per node of element to be used for DRM
//! - xpositive, xminus: boundary layer range in x direction
//! - ypositive, yminus: boundary layer range in y direction
//! - zpositive, zminus: boundary layer range in z direction
//! - ElementNumberFile: File including element numbers inside the platic bowl (first line should be the total number
//!     of the elements and from the second line, 1 element number per line in the input file)
//! - DisplacementTimeHistoryFile: File including displacement time history (in each line write the values of displacement in time
//!   for first degree of freedom of the first node defined in NodeNumbersFilename, next line should have the values for second dof of the first node
//!   and continue for all degrees of freedom. Then move to the second node defined in NodeNumbersFilename and ...
//! - AccelerationTimeHistoryFile: File including acceleration time history (in each line write the values of displacement in time
//!   for first degree of freedom of the first node defined in NodeNumbersFilename, next line should have the values for second dof of the first node
//!   and continue for all degrees of freedom. Then move to the second node defined in NodeNumbersFilename and ...





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





int add_load_pattern_domain_reduction_method_old_inputfiles_format(int PatternNumber,
        double dt,
        double factor,
        int numdof,
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
    theDRMPattern = new Domain_Reduction_Method(PatternNumber, ElementNumbers, DisplacementTimeHistory, AccelerationTimeHistory, dt, factor, numdof, xpositive, xminus, ypositive, yminus, zpositive, zminus);

    if (theDRMPattern == 0)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_old_inputfiles_format) ran out of memory creating load pattern - pattern PBowlLoading ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theDRMPattern) == false)
    {
        cerr << "WARNING: (add_load_pattern_domain_reduction_method_old_inputfiles_format) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;
};






