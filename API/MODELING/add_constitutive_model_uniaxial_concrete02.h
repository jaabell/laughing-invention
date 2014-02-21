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
// DATE:              September 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Uniaxial Material - Concrete02</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of Uniaxial Material - Concrete02.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//! \n Reference:
//! \n concrete model with damage modulus
//! \n by MOHD YASSIN (1993)
//! \n adapted to FEDEAS material library
//! \n by D. Sze and Filip C. Filippou in 1994
//!
//!


//! Inputs:
//! - MaterialNumber: unique material object tag
//! - fpc: compressive strength
//! - epsc0: strain at compressive strength
//! - fpcu: crushing strength
//! - epsU: strain at crushing strength
//! - lambda: ratio between unloading slope at epscu and initial slope
//! - ft: tensile strength
//! - Ets: tension softening stiffness (absolute value) (slope of the linear tension softening branch)




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_constitutive_model_uniaxial_concrete02(int MaterialNumber,
        double fpc, double epsc0, double fpcu,
        double epscu, double rat, double ft,
        double Ets)
{
    UniaxialMaterial* theMaterial = 0;
    theMaterial = new Concrete02(MaterialNumber, fpc, epsc0, fpcu, epscu, rat, ft, Ets);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_uniaxial_concrete02) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addUniaxialMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_uniaxial_concrete02) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





