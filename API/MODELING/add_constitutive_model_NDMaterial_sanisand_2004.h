///////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (C) and COPYLEFT (C): Boris Jeremic.
// LICENSE:
//   Use  of  these  tools,    programs,  sources  for  research, teaching and
//   consulting (professional practice) is encouraged. Copyright and Copyleft is
//   held  by  Boris Jeremic (under LGPL, if not otherwise noted in directories)
//   and also defined (on a nonlegal, funny side) by the following clause:
//   Woody's    (Guthrie)  viral  (GPL-like)  license  (adapted  by BJ): ``This
//   source  code  is  Copyrighted  worldwide  by the Authors, for an indefinite
//   period of time, and anybody caught using it without our permission, will be
//   mighty  good  friends of ourn, cause we don't give a darn. Hack it. Compile
//   it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's all we wanted
//   to do.''
//
// PROJECT:           Object Oriented Finite Element Program
// PURPOSE:           API for the Domain Specific Language (DSL) c binding
// VERSION:           Oct 2011
// LANGUAGE:          C++
// TARGET OS:         Portable, any OS that supports standard C++
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              October 2009
// UPDATE HISTORY:    October 2011 (API-DSL), Doxygen parts rewritten (BJ)
//
///////////////////////////////////////////////////////////////////////////////
// Doxygen documentation, used by the NRC ESSI (Lecture) Notes
// Defining  3D Dafalias-Manzari material model
//!
//! @param[in] MaterialNumber number/tag of the nD material to be used
//! @param[in] AlgorithmType Explicit (=0) or Implicit (=1)
//! @param[in] rho density
//! @param[in] e0 initial viod ratio
//! @param[in] G0 elastic shear modulus (same unit as stress)
//! @param[in] nu Poisson's ratio
//! @param[in] Pat atmospheric pressure
//! @param[in] p_cut pressure cut-off ratio
//! @param[in] Mc
//! @param[in] c tension-compression strength ratio
//! @param[in] lambda_c parameter for critical state line
//! @param[in] xi parameter for critical state line
//! @param[in] ec_ref reference void for critical state line, ec = e_r - lambda*(pc/Pat)^xi
//! @param[in] m opening of the yield surface
//! @param[in] h0 bounding parameter
//! @param[in] ch bounding parameter
//! @param[in] nb bounding parameter
//! @param[in] A0 dilatancy parameter
//! @param[in] nd dilatancy parameter
//! @param[in] z_max fabric parameter
//! @param[in] cz fabric parameter
//! @param[in] initialconfiningstress initial confining pressure (positive for compression)
//!
//! \details Material Modeling, Dafalias Manzari.
//!
//! \details This DSL command is used to define a Manzari-Dafalias material model
//!
//! \authors  Boris Jeremic (corresponding),  Nima Tafazzoli
//!
//! \date  Oct2009, 10Oct2011
//!
//! \copyright  Boris Jeremic (LGPL)
//!
//! More information about the theory related to this method/function can be found
//! <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/NRC_ESSI_Simulator/NRC_ESSI_Notes/" target="_blank">HERE</A></div>,
//! and also in a paper Yannis F. Dafalias and Majid T. Manzari. Simple plasticity sand model accounting for fabric
//! change effects. ASCE Journal of Engineering Mechanics, 130(6):622-634, June 2004.
//! \n



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

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_dafalias_manzari) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






