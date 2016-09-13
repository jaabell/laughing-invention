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

//! Inputs: Nothing!

void define_numberer_plain_for_analysis()
{

#ifdef _PARALLEL_PROCESSING

    theNumberer = new ParallelNumberer();

    if (theNumberer == NULL)
    {
        cerr << "Error: (define_numberer_plain_for_analysis:Parallel) memory allocating for theNumberer!" << endl;
        abort();
    }

#else

    theNumberer =  new PlainNumberer();

    if (theNumberer == NULL)
    {
        cerr << "Error: (define_numberer_plain_for_analysis:Sequential) memory allocating for theNumberer!" << endl;
        abort();
    }

    // out NT and BJ looked at commands, cpp    PlainNumberer   *plainnumberer  = new PlainNumberer();
    // out NT and BJ looked at commands, cpp     theNumberer = new DOF_Numberer(*plainnumberer);
#endif
};





