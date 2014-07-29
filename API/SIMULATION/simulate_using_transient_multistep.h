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
    int result = 0;

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



    //=====================================================================================
    // Parallel Dynamic Analysis
    //=====================================================================================
#ifdef _PARALLEL_PROCESSING // Parallel processing


    if (OPS_PARTITIONED == true && OPS_NUM_SUBDOMAINS > 1)
    {
        DomainDecompositionAnalysis *theSubAnalysis;
        SubdomainIter &theSubdomains = theDomain.getSubdomains();
        Subdomain *theSub = 0;
        // create the appropriate domain decomposition analysis
        while ((theSub = theSubdomains()) != 0)
        {
            theSubAnalysis = new TransientDomainDecompositionAnalysis(*theSub,
                    *theHandler,
                    *theNumberer,
                    *theAnalysisModel,
                    *theAlgorithm,
                    *theSOE,
                    *theTransientIntegrator,
                    theConvergenceTest,
                    false);

            theSub->setDomainDecompAnalysis(*theSubAnalysis);
        }
    }


    if (OPS_PARTITIONED == false && OPS_NUM_SUBDOMAINS > 1)
    {
        if (OPS_theChannels != 0)
        {
            delete [] OPS_theChannels;
        }

        OPS_theChannels = new Channel *[OPS_NUM_SUBDOMAINS];

        // create some subdomains
        for (int i = 1; i <= OPS_NUM_SUBDOMAINS; i++)
        {
            ShadowSubdomain *theSubdomain = new ShadowSubdomain(i, *OPS_MACHINE, *OPS_OBJECT_BROKER);
            theDomain.addSubdomain(theSubdomain);
            OPS_theChannels[i - 1] = theSubdomain->getChannelPtr();
        }

        // create a partitioner & partition the domain
        if (OPS_DOMAIN_PARTITIONER == 0)
        {
            OPS_GRAPH_PARTITIONER  = new ParMetis;
            OPS_DOMAIN_PARTITIONER = new DomainPartitioner(*OPS_GRAPH_PARTITIONER);
            theDomain.setPartitioner(OPS_DOMAIN_PARTITIONER);
        }
        theDomain.partition(OPS_NUM_SUBDOMAINS);
        OPS_PARTITIONED = true;
    }


    for (int i = 1; i <= numSteps; i++)
    {
        if ( OPS_REDEFINE_ANALYSIS == true )
        {
            DomainDecompositionAnalysis *theSubAnalysis;
            SubdomainIter &theSubdomains = theDomain.getSubdomains();
            Subdomain *theSub = 0;

            while ((theSub = theSubdomains()) != 0)
            {

                // create the appropriate domain decomposition analysis

                theSubAnalysis = new TransientDomainDecompositionAnalysis(*theSub,
                        *theHandler,
                        *theNumberer,
                        *theAnalysisModel,
                        *theAlgorithm,
                        *theSOE,
                        *theTransientIntegrator,
                        theConvergenceTest,
                        false);

                theSub->setDomainDecompAnalysis(*theSubAnalysis);
            }
            OPS_REDEFINE_ANALYSIS = false;
        }

        int numIncr = 1;
        result = theTransientAnalysis->analyze(numIncr, dT);

        cout << "\nAnalysis step " << i << " finished!\n";
    }

    //=====================================================================================
    // Sequential Dynamic Analysis
    //=====================================================================================
#else // Analysis is sequential

    theTransientAnalysis->analyze(numSteps, dT);
#endif
    //=====================================================================================

    return result;

};
