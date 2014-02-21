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
// LANGUAGE:
// TARGET OS:
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




int add_constitutive_model_NDMaterialLT_pisano(int tag,
        double E_in,
        double v_in,
        double M_in,
        double kd_in,
        double xi_in,
        double h_in,
        double m_in,
        double rho_in,
        double initialconfiningstress_in,
        double beta_min_in)
{
    NDMaterialLT* theMaterial = 0;
    theMaterial = new PisanoLT( tag,
                                E_in,
                                v_in,
                                M_in,
                                kd_in,
                                xi_in,
                                h_in,
                                m_in,
                                rho_in,
                                initialconfiningstress_in,
                                beta_min_in);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_pisano) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterialLT(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_pisano) Material " << tag << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};







