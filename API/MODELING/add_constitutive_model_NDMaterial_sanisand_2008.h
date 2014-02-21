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
//! <h1>DSL of ND Material - SANISAND</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of ND material SANISAND.
//! \n
//! Reference: Taiebat & Dafalias (2008)
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - MaterialNumber: tag of the ND material to be used
//! - Algorithm: Explicit (=0) or Implicit (=1)
//! - rho: density
//! - e0: initial void ratio at zero strain;
//! - G0: Reference elastic shear modulus (same unit as stress)
//! - K0: Reference elastic bulk modulus (same unit as stress)
//! - Pat: atmospherics pressure for critical state line;
//! - k_c: cut-off factor; for p < k_c*Pat, use p = k_c*Pat for calculation of G; (a default value of k_c = 0.01 should work fine)
//! - alpha_cc: critical state stress ratio;
//! - c: tension-compression strength ratio;
//! - lambda: parameter for critical state line;
//! - xi: parameter for critical state line;
//! - ec_ref: reference void for critical state line, ec = e_r - lambda*(pc/Pat)^xi;
//! - m: opening of the yield surface;
//! - h0: bounding parameter;
//! - ch: bounding parameter;
//! - nb: bounding parameter;
//! - A0: dilatancy parameter;
//! - nd: dilatancy parameter;
//! - p_r: LCC parameter;
//! - rho_c: LCC parameter;
//! - theta_c: LCC parameter;
//! - X: LCC parameter;
//! - z_max: fabric parameter;
//! - cz: fabric parameter;
//! - p0: yield surface size;
//! - p_in:



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_constitutive_model_NDMaterial_sanisand_2008(int MaterialNumber,
        int Algorithm,
        double rho,
        double e0,
        double G0,
        double K0,
        double Pat,
        double k_c,
        double alpha_cc,
        double c,
        double lambda,
        double xi,
        double ec_ref,
        double m,
        double h0,
        double ch,
        double nb,
        double A0,
        double nd,
        double p_r,
        double rho_c,
        double theta_c,
        double X,
        double z_max,
        double cz,
        double p0,
        double p_in,
        int number_of_subincrements,
        int maximum_number_of_iterations,
        double tolerance_1,
        double tolerance_2)
{
    stresstensor zeroT1;
    stresstensor zeroT2;
    stresstensor initStress;
    stresstensor initStrain;

    initStress.val(1, 1) = -p_in;
    initStress.val(2, 2) = -p_in;
    initStress.val(3, 3) = -p_in;

    double MC[23] = {rho, e0, G0, K0, Pat, k_c, alpha_cc, c, lambda, xi, ec_ref, m, h0, ch, nb, A0, nd, p_r, rho_c, theta_c, X,  cz, z_max};
    double IS[1] = {p0};
    stresstensor TS[2] = {zeroT1, zeroT2};
    MaterialParameter matpar(MC, 23, IS, 1, TS, 2);

    //------------------G0  K0  Pat  k_c  e0  sigma_in
    SANISAND_Elastic le(3,  4,  5,   6,   2,  initStress, initStrain);

    //--------------  m     p0    alpha
    SANISAND_YF dpy(0, 12, 1, 1,  2, 1);

    //--------------  e0   ec_ref   lambda   xi    Pat   alpha_cc   c    A0    nd    m     alpha   z    X
    SANISAND_PF dpf(0, 2, 0, 11,    0, 9,     0, 10, 0, 5,  0, 7,       0, 8, 0, 16, 0, 17, 0, 12, 2, 1,    2, 2, 0, 21);

    //--------------------- e0  Pat alpha_cc  p_r  rho_c  theta_c  K0  p0  alpha
    SANISAND_p0_bar p0_bar(2,   5,  7,        18,   19,     20,    4,  1,   1);

    //--------------------- e0  ec_ref  lambda  xi  Pat  alpha_cc  c    nb  h0  ch   G0  m   alpha
    SANISAND_alpha_Eij Eij1(2,  11,     9,      10, 5,   7,        8,   15, 13, 14,  3,  12, 1);

    //------------------e0  ec_ref  lambda  xi  Pat  alpha_cc  c    nb  h0  ch   G0  m   c_z  z_maz alpha  z
    SANISAND_z_Eij Eij2(2,  11,     9,      10, 5,   7,        8,   15, 13, 14,  3,  12, 22,  23,   1,     2);

    ScalarEvolution* SE = {&p0_bar};
    TensorEvolution* TE[2] = {&Eij1, &Eij2};


    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, &SE, TE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_material_NDMaterial_sanisand) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_material_NDMaterial_sanisand) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};







