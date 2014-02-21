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
// DATE:              October 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening(int MaterialNumber,
        double rho,
        double E,
        double v,
        double k,
        double ha,
        double Cr,
        double initialconfiningstress,
        int maximum_number_of_iterations,
        double tolerance_1,
        double tolerance_2)
{

    NDMaterial* theMaterial = 0;
    theMaterial = new vonMisesKH(MaterialNumber, E, v, k, ha, Cr, rho, initialconfiningstress, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_accelerated_vonmises_kinematic_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};







