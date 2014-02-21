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
// Defining  3D Cam-Clay material model.
//!
//! @param[in]  MaterialNumber number/tag of the material to be used
//! @param[in]  AlgorithmType Explicit (=0) or Implicit (=1)
//! @param[in]  rho density
//! @param[in]  e0 initial void ratio
//! @param[in]  M slope of the critical state line
//! @param[in]  lambda slope of the  Normal Consolidation Line (NCL)
//! @param[in]  kappa slope of the  Unloading-Reloading Line (URL)
//! @param[in]  v
//! @param[in]  Kc
//! @param[in]  initialconfiningstress initial confining stress/pressure (positive for compression)
//!
//! \details Material Modeling, Cam-Clay.
//!
//! \details This DSL command is used to define a Cam-Clay material model
//!
//! \authors  Boris Jeremic (corresponding),  Nima Tafazzoli
//!
//! \date  Oct2009, 10Oct2011
//!
//! \copyright  Boris Jeremic (LGPL)
//!
//! More information about the theory related to this method/function can be found
//! in section \ref{Cam-Clay} on page \pageref{Cam-Clay} in these Notes,
//! as well as in a book: David Muir Wood. Soil Behaviour nad Critical State Soil Mechanics.
//! Cambridge University Press, 1990.
//! \n


int add_constitutive_model_NDMaterial_camclay(int MaterialNumber,
        int Algorithm,
        double rho,
        double e0,
        double M,
        double lambda,
        double kappa,
        double v,
        double Kc,
        double P0,
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

    //isotropic hardening
    double MC[7] = {rho, e0, M, lambda, kappa, v, Kc};
    double IS[1] = {P0};
    MaterialParameter inpar(MC, 7, IS, 1);
    elnp_Elastic le(5, 6, 7, 2, initStress);
    CC_YF dpy(0, 3, 1, 1);
    CC_PF dpf(0, 3, 1, 1);
    CC_Ev Ev(3, 4, 5, 2, 1);
    ScalarEvolution* SE = {&Ev};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &inpar, &le, &dpy, &dpf, &SE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_camclay) memory allocation problem!" << endl;
        return -1;
    }

    if( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_camclay) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





