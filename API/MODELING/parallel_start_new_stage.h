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

//! Inputs: model name

int parallel_start_new_stage(string CurrentStageName)
{

    //     StageNumber++;

    double newTime = 0;
    theDomain.setCurrentTime(newTime);
    theDomain.setCommittedTime(newTime);


    theDomain.setLoadConstant();
    theDomain.setCurrentTime(newTime);
    theDomain.setCommittedTime(newTime);

    if (theStaticAnalysis != 0)
    {
        theStaticAnalysis->clearAll();
        delete theStaticAnalysis;
    }

    if (theTransientAnalysis != 0)
    {
        theTransientAnalysis->clearAll();
        delete theTransientAnalysis;
    }

    // NOTE : DON'T do the above on theVariableTimeStepAnalysis
    // as it and theTansientAnalysis are one in the same

    DomainDecompositionAnalysis* theSubAnalysis;
    SubdomainIter& theSubdomains = theDomain.getSubdomains();
    ShadowSubdomain* theSub;

    while ((theSub = (ShadowSubdomain*)theSubdomains()) != 0)
    {
        theSub->clearAnalysis();
    }

    OPS_REDEFINE_ANALYSIS = true;


    theAlgorithm = 0;
    theHandler = 0;
    theNumberer = 0;
    theAnalysisModel = 0;
    theSOE = 0;
    theStaticIntegrator = 0;
    theTransientIntegrator = 0;
    theStaticAnalysis = 0;
    theTransientAnalysis = 0;
    theVariableTimeStepTransientAnalysis = 0;
    theConvergenceTest = 0;


    return 0;

};