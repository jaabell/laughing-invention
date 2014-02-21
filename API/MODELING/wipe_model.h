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
// DATE:              August 2009
// UPDATE HISTORY:    September 2009 (API)
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Starting New Stage</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of starting new stage which resets the pseudo time to zero and delete the analysis onjects.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs: Nothing!


int wipe_model()
{

    theDomain.clearAll();


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

    // if (theHandler != 0)
    // {
    //     theHandler->clearAll();
    //     delete theHandler;
    // }

    delete theNumberer;

    theAnalysisModel                     = 0;
    theAlgorithm                         = 0;
    theHandler                 = 0;
    theNumberer                          = 0;
    theSOE                               = 0;
    theStaticIntegrator                  = 0;
    theTransientIntegrator               = 0;
    theStaticAnalysis                    = 0;
    theTransientAnalysis                 = 0;
    theVariableTimeStepTransientAnalysis = 0;
    theConvergenceTest                   = 0;
    theEigenSOE                          = 0;


    // if (theOutputHandler_element_stress != 0)
    // {
    //     delete theOutputHandler_element_stress;
    // }

    // theOutputHandler_element_stress = 0;


    // if (theOutputHandler_node_displacement != 0)
    // {
    //     delete theOutputHandler_node_displacement;
    // }

    // theOutputHandler_node_displacement = 0;


    // if (theOutputHandler_element_force != 0)
    // {
    //     delete theOutputHandler_element_force;
    // }

    // theOutputHandler_element_force = 0;


    // #ifdef _PARALLEL_PROCESSING
    //     if (thebodyforce != 0)
    //     {
    //         delete thebodyforce;
    //     }

    //     thebodyforce = 0;
    // #endif

    return 0;

};

