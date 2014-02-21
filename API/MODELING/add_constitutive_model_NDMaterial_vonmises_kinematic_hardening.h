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
//! <h1>DSL of ND Material - von-Mises Kinematic Hardening</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of ND material von-Mises kinematic hardening.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - MaterialNumber: tag of the ND material to be used
//! - Algorithm: Explicit (=0) or Implicit (=1)
//! - rho: density
//! - E: Elastic modulus
//! - v: Poisson's ratio
//! - k:
//! - ha:
//! - Cr:
//! - initialconfiningstress: initial confining pressure (possitive for compression)



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_constitutive_model_NDMaterial_vonmises_kinematic_hardening(int MaterialNumber,
        int Algorithm,
        double rho,
        double E,
        double v,
        double k,
        double ha,
        double Cr,
        double initialconfiningstress,
        int number_of_subincrements,
        int maximum_number_of_iterations,
        double tolerance_1,
        double tolerance_2)
{
    stresstensor zeroT;
    stresstensor initStress;
    initStress.val(1, 1) = -initialconfiningstress;
    initStress.val(2, 2) = -initialconfiningstress;
    initStress.val(3, 3) = -initialconfiningstress;

    double MC[6] = {rho, E, v, k, ha, Cr};
    stresstensor TS[1] = {zeroT};
    MaterialParameter matpar(MC, 6, TS, 1);
    Isotropic_Elastic le(2, 3, initStress);
    VM_YF dpy(0, 4, 2, 1);
    VM_PF dpf(2, 1);
    //  VM_PF dpf(0,4);
    //VM_PF dpf(-1,4);
    AF_Eij Eij(5, 6, 1);
    TensorEvolution* TE = {&Eij};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, &TE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_kinematic_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_kinematic_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};







