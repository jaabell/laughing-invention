///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               October 2009
// UPDATE HISTORY:     Full update history in git repository.
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

void define_solver_petsc_for_analysis(KSPType method, PCType preconditioner, double rTol, double aTol, double dTol, int maxIts, MatType matType)
{
    PetscSolver *theSolver = new PetscSolver( method, preconditioner, rTol, aTol, dTol,  maxIts, matType);
    //    PetscSolver *theSolver = new PetscSolver( method, preconditioner);

    theSOE = new PetscSOE(*theSolver, matType);

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



