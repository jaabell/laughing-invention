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


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Uniaxial Material - Elastic</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of uniaxial material (elastic).
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - MaterialNumber: unique material object tag
//! - elasticmodulus: elastic modulus of the material
//! - eta: damping tangent




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_constitutive_model_uniaxial_elastic(int MaterialNumber,
        double elasticmodulus,
        double eta)
{
    UniaxialMaterial* theMaterial = 0;
    theMaterial = new ElasticMaterial(MaterialNumber, elasticmodulus, eta);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_uniaxial_elastic) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addUniaxialMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_uniaxial_elastic) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




