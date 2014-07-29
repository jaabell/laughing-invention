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
// PROGRAMMER:        Babak Kamrani, Boris Jeremic
//
// DATE:              September 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

// Numberer


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of DOF Numberer (Plain)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of dof numberer (plain).
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!




//! Inputs: Nothing!


int define_parallel_numberer_plain_for_analysis();
int define_parallel_numberer_plain_for_analysis()
{
#ifdef _PARALLEL_PROCESSING
    //PlainNumberer   *plainnumberer  = new ParallelNumberer();
    theNumberer = new ParallelNumberer();

    if (theNumberer == NULL)
    {
        cerr << "Error: (define_parallel_numberer_plain_for_analysis) memory for theNumberer  can not be allocated!" << endl;
        abort();
    }
#else
    cerr << "Error: (define_parallel_numberer_plain_for_analysis) Not available in sequential mode." << endl;
    abort();
#endif

    return 0;
};