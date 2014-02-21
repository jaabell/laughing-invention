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
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Static Integrators (Displacement Control)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of static integrators (displacement control).
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!




//! Inputs:
//! - dispincrement: increment of displacement in each step of analysis
//! - node_number: node whose response controls the solution
//! - doftomove: degree-of-freedom whose response controls the solution. Valid range is from 1 through the number of nodal degrees-of-freedom.




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int define_static_solution_advancement_integrator_displacement_control(int node_number, int doftomove, double dispincrement)
{
    theStaticIntegrator  = new DisplacementControl(node_number, doftomove - 1, dispincrement, 1, dispincrement, dispincrement);

    if (theStaticIntegrator == NULL)
    {
        cerr << "Error: (define_static_solution_advancement_integrator_displacement_control) memory allocating theStaticIntegrator!" << endl;
        return -1;
    }

    return 0;

};


