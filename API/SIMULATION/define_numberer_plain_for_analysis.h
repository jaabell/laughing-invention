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
// DATE:              October 2009
// UPDATE HISTORY:    10/26/2011 by Babak Kamrani for parallel analysis
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


//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



void define_numberer_plain_for_analysis();
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





