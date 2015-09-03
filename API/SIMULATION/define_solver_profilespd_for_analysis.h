///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               Jan  2010
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


int define_solver_profilespd_for_analysis()
{
#ifdef _PARALLEL_PROCESSING
    cerr << "ProfileSPD solver not available in parallel processing mode. " << endl;
    return -1;
#else
    ProfileSPDLinSolver *theSolver = 0;
    theSolver = new ProfileSPDLinDirectSolver();

    if (theSolver == NULL)
    {
        cerr << "Error: (define_solver_profilnespd_for_analysis) memory allocation problem for theSolver!" << endl;
        return -1;
    }

    theSOE = new ProfileSPDLinSOE(*theSolver);

    if (theSOE == NULL)
    {
        cerr << "Error: (define_solver_profilespd_for_analysis) memory allocation problem for theSOE!" << endl;
        return -1;
    }

    return 0;
#endif
};






