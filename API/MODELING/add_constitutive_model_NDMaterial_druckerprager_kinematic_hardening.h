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
// Defining  3D Drucker-Prager Kinematic Hardening material model
//!

//! @param[in] MaterialNumber: number/tag of the ND material to be used
//! @param[in] Algorithm: Explicit (=0) or Implicit (=1)
//! @param[in] rho: density
//! @param[in] E: Elastic modulus
//! @param[in] v: Poisson's ratio
//! @param[in] k:
//! @param[in] ha: Armstrong-Frederick kinematic hardening constant
//! @param[in] Cr:  Armstrong-Frederick kinematic hardening constant
//! @param[in] initialconfiningstress: initial confining pressure (positive for compression)
//!
//! \details Material Modeling, Drucker-Prager Kinematic Hardening .
//!
//! \details This DSL command is used to define a Drucker-Prager Kinematic Hardening  material model
//!
//! \authors  Boris Jeremic (corresponding),  Nima Tafazzoli
//!
//! \date  Oct2009, 10Oct2011
//!
//! \copyright  Boris Jeremic (LGPL)
//!
//! More information about the theory related to this method/function can be found
//! <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/NRC_ESSI_Simulator/NRC_ESSI_Notes/" target="_blank">HERE</A></div>.
//! \n


int add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening(int MaterialNumber,
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
    DP_YF dpy(0, 4, 2, 1);
    DP_PF dpf(0, 4, 2, 1);
    AF_Eij Eij(5, 6, 1);
    TensorEvolution* TE = {&Eij};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, &TE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_druckerprager_kinematic_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};






