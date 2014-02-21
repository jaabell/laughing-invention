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


// Static and Transient Integrators


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Static Integrators (Load Control)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of static and transient integrators (load control).
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!




//! Inputs:
//! - loadstep: load step of analysis

void define_integrator_loadcontrol_for_static_analysis(double);
void define_integrator_loadcontrol_for_static_analysis(double loadstep)
{
    theStaticIntegrator  = new LoadControl(loadstep, 1, loadstep, loadstep);

    if (theStaticIntegrator == NULL)
    {
        cerr << "Error: (define_integrator_loadcontrol_for_static_analysis) memory for theStaticIntegrator can not be allocated!" << endl;
        abort();
    }

};


