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


int start_new_stage(string CurrentStageName)
{

    cout << endl << endl << "Starting new stage: " << CurrentStageName << endl << endl;

    StageName = CurrentStageName;

    //Iterate through load patterns and set them all constant
    // LoadPattern *thePattern;
    // LoadPatternIter &thePatterns = theDomain.getLoadPatterns();
    // while ((thePattern = thePatterns()) != 0)
    // {
    //     thePattern->setLoadConstant();
    // }
    theDomain.setLoadConstant();



    //Reset time
    int newTime = 0;
    theDomain.setCurrentTime(newTime);
    theDomain.setCommittedTime(newTime);

    if (theStaticAnalysis )
    {
        theStaticAnalysis->clearAll();
        delete theStaticAnalysis;
    }

    if (theTransientAnalysis )
    {
        theTransientAnalysis->clearAll();
        delete theTransientAnalysis;
    }

    //Set all these pointers to null! (They will have to be set by user.)
    theAnalysisModel                     = 0;
    theAlgorithm                         = 0;
    theHandler                           = 0;
    theNumberer                          = 0;
    theSOE                               = 0;
    theStaticIntegrator                  = 0;
    theTransientIntegrator               = 0;
    theStaticAnalysis                    = 0;
    theTransientAnalysis                 = 0;
    theVariableTimeStepTransientAnalysis = 0;
    theConvergenceTest                   = 0;
    theEigenSOE                          = 0;

    //Set numberer and check that it is correctly set
#ifdef _PARALLEL_PROCESSING
    OPS_REDEFINE_ANALYSIS = true;
    theNumberer = new ParallelNumberer();
#else
    theNumberer = new DOF_Numberer(*theRCM);
#endif

    if (theNumberer == NULL)
    {
        cerr << "Error: (start_new_stage) theNumberer was not correctly set!" << endl;
        return -1;
    }

    //Set handler to plain
    theHandler = new PlainHandler();
    if (theHandler == NULL)
    {
        cerr << "Error: (start_new_stage) theHandler was not correctly set!!" << endl;
        return -1;
    }


    string filename("");
    filename = ModelName + "_" + StageName + ".h5.feioutput";

    int numSteps = 1;
    theDomain.setOutputWriter(filename,
                              ModelName,
                              StageName,
                              numSteps);


    // ElementIter &elements = theDomain.getElements();
    // Element *ele = 0;

    // while ((ele = elements()) != 0)
    // {
    //     ele->startNewStage();
    // }


    return 0;
};

