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
//! - KSPType: KSPCG KSPGMRES
//! - PCType: PCJACOBI PCILU PCBJACOBI
//! - rTol: Relative Tolerance
//! - aTol: Absolute Tolerance
//! - dTol: Divergence Tolerance
//! -maxIts: Maximum Number of Iterations Allowed
//! -matType: MAT_SOLVER_SUPERLU_DIST, MAT_SOLVER_SPOOLES MAT_SOLVER_MUMPS MATMPIAIJ


#ifdef _PARALLEL_PROCESSING
#include "../ModifiedOpenSeesServices/domain/domain/SubdomainIter.h"

void define_solver_petsc_for_analysis(KSPType method, PCType preconditioner, double rTol, double aTol, double dTol, int maxIts, MatType matType)
{
    PetscSolver *theSolver = new PetscSolver( method, preconditioner, rTol, aTol, dTol,  maxIts, matType);

    if (theSolver == NULL)
    {
        cerr << "Error: (define_solver_petsc_for_analysis) memory for adding theSolver can not be allocated!" << endl;
        abort();
    }

    theSOE = new PetscSOE(*theSolver, matType);

    if (theSOE == NULL)
    {
        cerr << "Error: (define_solver_petsc_for_analysis) memory for adding theSOE can not be allocated!" << endl;
        abort();
    }


    if (theStaticAnalysis != 0 || theTransientAnalysis != 0)
    {
        SubdomainIter &theSubdomains = theDomain.getSubdomains();
        Subdomain *theSub;

        while ((theSub = theSubdomains()) != 0)
        {
            theSub->setAnalysisLinearSOE(*theSOE);
        }
    }

};



#else



void define_solver_petsc_for_analysis()
{
    cerr << "define_solver_petsc_for_analysis(): ERROR - PETSc solver not available in sequential mode. " << endl;
}



#endif



