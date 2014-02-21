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
// DATE:              October 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Convergence Tests (Norm Unbalance)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of gonvergence test norm unbalance.
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!



//! Inputs:
//! - theTol: convergence tolerance
//! - maxIter: maximum number of iterations that will be performed before "failure to converge" is returned
//! - PrintFlag: flag used to print information on convergence (optional)
//!             - 0: no print output
//!             - 1: print information on each step
//!             - 2: print information when convergence has been achieved
//!             - 4: print norm, dU and dR vectors
//!             - 5: at convergence failure, carry on, print error message, but do not stop analysis



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int define_convergence_test_normunbalance_for_analysis(double theTol, int maxIter, int PrintFlag)
{
    theConvergenceTest = new CTestNormUnbalance(theTol, maxIter, PrintFlag);

    if (theConvergenceTest == NULL)
    {
        cerr << "Error: (define_convergence_test_normunbalance_for_analysis) memory for convergence test can not be allocated!" << endl;
        return -1;
    }

    return 0;

};




