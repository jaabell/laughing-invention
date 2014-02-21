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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              July 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening(int MaterialNumber,
        int Algorithm,
        double rho,
        double E,
        double v,
        double k,
        double H,
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

    double MC[5] = {rho, E, v, k, H};
    stresstensor TS[1] = {zeroT};
    MaterialParameter matpar(MC, 5, TS, 1);
    Isotropic_Elastic le(2, 3, initStress);
    VM_YF dpy(0, 4, 2, 1);
    VM_PF dpf(2, 1);
    Linear_Eij Eij(5);
    TensorEvolution* TE = {&Eij};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, &TE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_linear_kinematic_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};







