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

//! Inputs:
//! - theTol: convergence tolerance
//! - maxIter: maximum number of iterations that will be performed before "failure to converge" is returned
//! - PrintFlag: flag used to print information on convergence (optional)
//!             - 0: no print output
//!             - 1: print information on each step
//!             - 2: print information when convergence has been achieved
//!             - 4: print norm, dU and dR vectors
//!             - 5: at convergence failure, carry on, print error message, but do not stop analysis
int define_convergence_test_RelativeNormdisplacementincrement_for_analysis(double theTol, int maxIter, int PrintFlag)
{
	theConvergenceTest = new CTestRelativeNormDispIncr(theTol, maxIter, PrintFlag);

	if (theConvergenceTest == NULL)
	{
		cerr << "Error: (define_convergence_test_RelativeNormdisplacementincrement_for_analysis) memory for convergence test can not be allocated!" << endl;
		return -1;
	}

	return 0;

};





