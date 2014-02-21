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
//! <h1>DSL of Transient Integrator of Hilber Hughes Taylor alpha method (HHT)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of transient integrator of Hilber Hughes Taylor alpha method (HHT).
//! \n
//! \n
//! For information about the functions and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!




//! Inputs:
//! - HHT_Alpha: HHT parameter



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double HHT_Alpha)
{
    double alpha = HHT_Alpha;
    theTransientIntegrator  = new HHT_alpha_method(alpha);
    //     theTransientIntegrator  = new HHT(0.9);


    //     double beta = (1.0-alpha)*(1.0-alpha)*0.25;
    //     double gamma = 0.5-alpha;
    //     theTransientIntegrator  = new HHT_alpha_method(alpha, beta, gamma);

    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (define_dynamic_solution_advancement_integrator_hilber_hughes_taylor) memory allocating theTransientIntegrator!" << endl;
        return -1;
    }

    return 0;

};



int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double, double, double);
int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double HHT_Alpha, double HHT_Beta, double HHT_Gamma)
{
    double alpha = HHT_Alpha;
    double beta  = HHT_Beta;
    double gamma = HHT_Gamma;
    theTransientIntegrator  = new HHT_alpha_method(alpha, beta, gamma);

    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (define_dynamic_solution_advancement_integrator_hilber_hughes_taylor) memory allocating theTransientIntegrator!" << endl;
        return -1;
    }

    return 0;

};

