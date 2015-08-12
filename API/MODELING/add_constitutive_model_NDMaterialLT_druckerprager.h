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
// PROGRAMMER:        Jose Abell,  Boris Jeremic
//
// DATE:              August 2015
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening(int MaterialNumber,
        double rho,
        double E,
        double nu,
        double k,
        double H_alpha,
        double H_k)
{

    NDMaterialLT* theMaterial = 0;

    theMaterial = new   DruckerPragerLinearHardening(MaterialNumber,  k,  H_alpha,  H_k,  E,  nu, rho);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterialLT(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_druckerprager_linear_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};
