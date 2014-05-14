///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): LGPL
//                 and
//                 Woody's license (adopted by BJ):
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
//! <h1>DSL of Transient (Dynamic) Analysis</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of transient (dynamic) analysis.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - dT: time-step increment
//! - numSteps: number of iterations performed at each step



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int simulate_using_transient_multistep(double dT,
                                       int numSteps)
{

    //output_of_elements_and_nodes();


    //***************************************************************************************************************



    if (theAnalysisModel == 0)
    {
        theAnalysisModel = new AnalysisModel();
    }


    if (theAnalysisModel == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) memory for analysismodel can not be allocated!" << endl;
        return -1;
    }



    if (theHandler == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) theHandler!" << endl;
        return -1;
    }


    if (theNumberer == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) theNumberer!" << endl;
        return -1;
    }



    if (theAlgorithm == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) theAlgorithm!" << endl;
        return -1;
    }


    if (theSOE == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) theSOE!" << endl;
        return -1;
    }


    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) theTransientIntegrator!" << endl;
        return -1;
    }



    //     cout<<"\nTransient Analysis!\n";



    theTransientAnalysis = new DirectIntegrationAnalysis(theDomain,
            *theHandler,
            *theNumberer,
            *theAnalysisModel,
            *theAlgorithm,
            *theSOE,
            *theTransientIntegrator,
            theConvergenceTest);

    if (theTransientAnalysis == NULL)
    {
        cerr << "Error: (analyze_transient_multistep) memory for theTransientAnalysis can not be allocated!" << endl;
        return -1;
    }


    theTransientAnalysis->analyze(numSteps, dT);

    //remove_output_of_elements_and_nodes();


    return 0;

};
