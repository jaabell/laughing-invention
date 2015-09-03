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

//! Inputs:
//! - dampingNumber: damping tag number to be used in element definition
//! - a0, a1, a2: Caughey 3rd order damping coefficients
//! - which_stiffness_to_use: Initial_Stiffness/Current_Stiffness/Last_Commited_Stiffness



int add_damping_caughey3rd(int dampingNumber, double a0, double a1, double a2, string which_stiffness_to_use)
{

    Damping* theDamping = 0;

    theDamping = new Caughey3rd(dampingNumber, a0, a1, a2, which_stiffness_to_use);

    if (theDamping == NULL)
    {
        cerr << "Error: (add_damping_caughey3rd) memory for theDamping can not be allocated!" << endl;
        return -1;
    }

    if ( theDomain.addDamping(theDamping) != 0 )
    {
        cerr << "Error: (add_damping_caughey3rd) Caughey3rd " << dampingNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};



