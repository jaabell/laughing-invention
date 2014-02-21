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
// DATE:              May 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Defining a sine wave force time history load pattern</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of a sine wave force time history load pattern.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! -




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





int add_force_time_history_linear(int PatternNumber,
                                  int NodeNumber,
                                  int dof_to_be_shaken,
                                  double final_load_value)
{


    LoadPattern* theLinearPattern = 0;
    theLinearPattern = new Linear_Force_TimeHistory(PatternNumber,
            NodeNumber,
            dof_to_be_shaken,
            final_load_value);

    if (theLinearPattern == 0)
    {
        cerr << "WARNING: (add_force_time_history_linear) ran out of memory creating load pattern ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theLinearPattern) == false)
    {
        cerr << "WARNING: (add_force_time_history_linear) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;

};









