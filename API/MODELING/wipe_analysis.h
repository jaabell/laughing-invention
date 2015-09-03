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

//! Inputs: Nothing!


void wipe_analysis();
void wipe_analysis()
{

    if (theStaticAnalysis != 0)
    {
        theStaticAnalysis->clearAll();
        delete theStaticAnalysis;
        cout << "\nAnalysis pointer is deleted!\n\n";

    }

    if (theTransientAnalysis != 0)
    {
        theTransientAnalysis->clearAll();
        delete theTransientAnalysis;
        cout << "\nAnalysis pointer is deleted!\n\n";

    }


    //   theAnalysisModel =0;
    //   theAlgorithm =0;
    //   theConstraintHandler =0;
    //   theNumberer =0;
    //   theSOE =0;
    //   theStaticIntegrator =0;
    //   theTransientIntegrator =0;
    //   theStaticAnalysis =0;
    //   theTransientAnalysis =0;
    //   theVariableTimeStepTransientAnalysis =0;
    //   theConvergenceTest = 0;

};