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
//! <h1>DSL of Uniaxial Material - Steel02</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of Uniaxial Material - Steel02.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//! \n Reference:
//! \n MENEGOTTO-PINTO STEEL MODEL WITH FILIPPOU ISOTROPIC HARDENING
//! \n written by MOHD YASSIN (1993)
//! \n adapted to FEDEAS material library
//! \n by E. Spacone, G. Monti and F.C. Filippou (1994)
//!
//!


//! Inputs:
//! - MaterialNumber: unique material object tag
//! - fy: yield strength
//! - E0: initial elastic tangent
//! - b: strain-hardening ratio (ratio between post-yield tangent and initial elastic tangent)
//! - R0, cR1, cR2: control the transition from elastic to plastic branches. Recommended values: R0=between 10 and 20, cR1=0.925, cR2=0.15
//! - a1, a2, a3, a4: isotropic hardening parameters
//! - a1: isotropic hardening parameter, increase of compression yield envelope as proportion of yield strength after a plastic strain of a2*(Fy/E).
//! - a2: isotropic hardening parameter (see explanation under a1)
//! - a3: isotropic hardening parameter, increase of tension yield envelope as proportion of yield strength after a plastic strain of a4*(Fy/E)
//! - a4: isotropic hardening parameter (see explanation under a3)




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_constitutive_model_uniaxial_steel02(int MaterialNumber,
        double fy, double E0, double b,
        double R0, double cR1, double cR2,
        double a1, double a2, double a3, double a4)


{
    UniaxialMaterial* theMaterial = 0;
    theMaterial = new Steel02(MaterialNumber, fy, E0, b, R0, cR1, cR2, a1, a2, a3, a4);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_uniaxial_steel02) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addUniaxialMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_uniaxial_steel02) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




