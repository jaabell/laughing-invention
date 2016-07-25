///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               October 2009
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
//! - dT: time-step increment
//! - numSteps: number of iterations performed at each step
//! - dtMin, dtMax: minimum and maximum time steps
//! - Jd: number of iterations performed at each step


int simulate_using_transient_variable_multistep(double dT,
        int numSteps,
        double dtMin,
        double dtMax,
        int Jd)
{

    //output_of_elements_and_nodes();


    //***************************************************************************************************************



    if (theAnalysisModel == 0)
    {
        theAnalysisModel = new AnalysisModel();
    }

    if (theAnalysisModel == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) memory for analysismodel can not be allocated!" << endl;
        return -1;
    }


    if (theHandler == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) theHandler!" << endl;
        return -1;
    }


    if (theNumberer == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) theNumberer!" << endl;
        return -1;
    }



    if (theAlgorithm == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) theAlgorithm!" << endl;
        return -1;
    }


    if (theSOE == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) theSOE!" << endl;
        return -1;
    }


    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) theTransientIntegrator!" << endl;
        return -1;
    }




    //     cout<<"\nTransient Analysis!\n";


    theVariableTimeStepTransientAnalysis = new VariableTimeStepDirectIntegrationAnalysis(theDomain,
            *theHandler,
            *theNumberer,
            *theAnalysisModel,
            *theAlgorithm,
            *theSOE,
            *theTransientIntegrator,
            theConvergenceTest);


    if (theVariableTimeStepTransientAnalysis == NULL)
    {
        cerr << "Error: (analyze_transient_variable_multistep) memory for theVariableTimeStepTransientAnalysis can not be allocated!" << endl;
        return -1;
    }


    theVariableTimeStepTransientAnalysis->analyze(numSteps, dT, dtMin, dtMax, Jd);


    //remove_output_of_elements_and_nodes();


    return 0;

};




