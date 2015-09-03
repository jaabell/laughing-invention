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
//! - NodeNumber: integer tag identifying the node to be constrained
//! - dof_number: dof to be fixed


int add_support_to_node(int NodeNumber,
                        int dof_number)
{

    int currentSpTag = 0;
    // SP_ConstraintIter &theSPs = theDomain.getSPs();
    // SP_Constraint *theSP;

    // while ((theSP = theSPs()) != 0)
    // {
    //     int spTag = theSP->getTag();

    //     if (spTag >= currentSpTag)
    //     {
    //         currentSpTag = spTag + 1;
    //     }
    // }
    currentSpTag = theDomain.getMaxSPsTag() + 1;

    SP_Constraint *sp  = 0;
    sp = new SP_Constraint(currentSpTag, NodeNumber,
                           dof_number - 1, // for SP dofs start from 0, for us from 1
                           0.0);  // DOF_value

    if (sp == NULL)
    {
        cerr << "Error: (add_support_to_node) memory allocating for SP_Constraint!" << endl;
        return -1;
    }


    if ( theDomain.addSP_Constraint(sp) == false )
    {
        cerr << "Error: (add_support_to_node) SP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};



int add_support_to_node(int NodeNumber,
                        int dof_number, double value)
{

    int currentSpTag = 0;
    // SP_ConstraintIter &theSPs = theDomain.getSPs();
    // SP_Constraint *theSP;

    // while ((theSP = theSPs()) != 0)
    // {
    //     int spTag = theSP->getTag();

    //     if (spTag >= currentSpTag)
    //     {
    //         currentSpTag = spTag + 1;
    //     }
    // }
    currentSpTag = theDomain.getMaxSPsTag() + 1;

    SP_Constraint *sp  = 0;
    sp = new SP_Constraint(currentSpTag, NodeNumber,
                           dof_number - 1, // for SP dofs start from 0, for us from 1
                           value);  // DOF_value

    if (sp == NULL)
    {
        cerr << "Error: (add_support_to_node) memory allocating for SP_Constraint!" << endl;
        return -1;
    }


    if ( theDomain.addSP_Constraint(sp) == false )
    {
        cerr << "Error: (add_support_to_node) SP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};

