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
// DATE:              November 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Adding Caughey Damping (3rd Order) (based on current stiffness)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of adding Caughey damping (3rd order) based on current stiffness.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - dampingNumber: damping tag number to be used in element definition
//! - a0, a1, a2: Caughey 3rd order damping coefficients
//! - which_stiffness_to_use: Initial_Stiffness/Current_Stiffness/Last_Commited_Stiffness




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_damping_caughey3rd(int dampingNumber, double a0, double a1, double a2, string which_stiffness_to_use)
{

    Damping* theDamping = 0;

    theDamping = new Caughey3rd(dampingNumber, a0, a1, a2, which_stiffness_to_use);

    if (theDamping == NULL)
    {
        cerr << "Error: (add_damping_caughey3rd) memory for theDamping can not be allocated!" << endl;
        return -1;
    }

    if( theDomain.addDamping(theDamping) != 0 )
    {
        cerr << "Error: (add_damping_caughey3rd) Caughey3rd " << dampingNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};



