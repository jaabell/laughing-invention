///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////



int add_constitutive_model_NDMaterial_sanisand_2004(int MaterialNumber,
        int Algorithm,
        double rho,
        double e0,
        double G0,
        double nu,
        double Pat,
        double p_cut,
        double Mc,
        double c,
        double lambda_c,
        double xi,
        double ec_ref,
        double m,
        double h0,
        double ch,
        double nb,
        double A0,
        double nd,
        double z_max,
        double cz,
        double initialconfiningstress,
        int number_of_subincrements,
        int maximum_number_of_iterations,
        double tolerance_1,
        double tolerance_2)
{
    stresstensor zeroT1;
    stresstensor zeroT2;
    stresstensor initStress;

    initStress.val(1, 1) = -initialconfiningstress;
    initStress.val(2, 2) = -initialconfiningstress;
    initStress.val(3, 3) = -initialconfiningstress;

    double MC[19] = {rho, e0, G0, nu, Pat, p_cut, Mc, c, lambda_c, xi, ec_ref, m, h0, ch, nb, A0, nd, z_max, cz};
    stresstensor TS[2] = {zeroT1, zeroT2};
    MaterialParameter matpar(MC, 19, TS, 2);
    DM04_Elastic le(3, 4, 5, 6, 2, initStress);
    DM04_YF dpy(0, 12, 2, 1);
    DM04_PF dpf(0, 2, 0, 11, 0, 9, 0, 10, 0, 5, 0, 12, 0, 7, 0, 8, 0, 16, 0, 17, 2, 1, 2, 2 );
    DM04_alpha_Eij Eij1(2, 11, 9, 10, 5, 12, 7, 8, 15, 13, 14, 3, 1, 2 );
    DM04_z_Eij Eij2(2, 11, 9, 10, 5, 12, 7, 8, 16, 17, 19, 18, 1, 2 );
    TensorEvolution* TE[2] = {&Eij1, &Eij2};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, TE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_dafalias_manzari) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_dafalias_manzari) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






