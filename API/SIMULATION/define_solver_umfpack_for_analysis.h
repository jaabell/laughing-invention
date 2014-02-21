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

// Solver


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Solver (UMFpack</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of solvers (UMFpack).
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!




//! Inputs: Nothing!


int define_solver_umfpack_for_analysis()
{
#ifdef _PARALLEL_PROCESSING
    cerr << "UMFPack solver not available in sequential mode." << endl;
    return -1;
#else
    UmfpackGenLinSolver *theSolver = new UmfpackGenLinSolver();
    theSOE = new UmfpackGenLinSOE(*theSolver);

    if (!theSOE)
    {
        cerr << "define_solver_umfpack_for_analysis() - Could not allocate memory for solver. " << endl;
        return -1;
    }

    return 0;
#endif
};






