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
// DATE:              August 2009
// UPDATE HISTORY:    October 2009 (API)
//
///////////////////////////////////////////////////////////////////////////////


// ND Material - Elastic Isotropic 3D


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of ND Material - Elastic Isotropic 3D</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of ND material elastic isotropic 3d.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - MaterialNumber: tag of the ND material to be used
//! - ElasticModulus: elastic modulus
//! - nu: Poisson's Ratio
//! - rho: density



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d(int MaterialNumber,
        double ElasticModulus,
        double nu,
        double rho)
{
    NDMaterial* theMaterial = 0;
    theMaterial = new ElasticIsotropic3D(MaterialNumber, ElasticModulus, nu, rho);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};




