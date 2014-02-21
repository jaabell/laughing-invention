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
// DATE:              August 2010
// UPDATE HISTORY:    Modified by Babak Kamrani for Petsc
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




#include "../ModifiedOpenSeesServices/domain/domain/SubdomainIter.h"


int define_solver_parallel_for_analysis()
{


    PetscSolver *theSolver = new PetscSolver( "cg", "jacobi", 1.0e-7, 1.0e-50, 10000,  10000, "mpiaij");
    //    PetscSolver *theSolver = new PetscSolver( method, preconditioner);

    theSOE = new PetscSOE(*theSolver, "mpiaij");


    if (theStaticAnalysis != 0 || theTransientAnalysis != 0)
    {
        SubdomainIter &theSubdomains = theDomain.getSubdomains();
        Subdomain *theSub;

        while ((theSub = theSubdomains()) != 0)
        {
            theSub->setAnalysisLinearSOE(*theSOE);
        }
    }

    // define_constraint_handler_plain_for_analysis();
    // define_parallel_numberer_plain_for_analysis();

    return 0;

};



