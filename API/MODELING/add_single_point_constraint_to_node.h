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
//! - ConstraintNumber: integer tag identifying the constraint
//! - NodeNumber: integer tag of the node to be constrained
//! - dof_number: dof of the node to be constrained
//! - DOFvalue: initial value of displacement for specified dof



int add_single_point_constraint_to_node(int NodeNumber,
                                        int dof_number,
                                        double DOFvalue) // for prescribed displacement
{
    int currentSpTag = 0;
    SP_ConstraintIter& theSPs = theDomain.getSPs();
    SP_Constraint* theSP;

    while ((theSP = theSPs()) != 0)
    {
        int spTag = theSP->getTag();

        if (spTag >= currentSpTag)
        {
            currentSpTag = spTag + 1;
        }
    }

    SP_Constraint* sp  = 0;
    sp = new SP_Constraint(currentSpTag, NodeNumber, dof_number - 1, DOFvalue);

    if (sp == NULL)
    {
        cerr << "Error: (add_single_point_constraint_to_node) memory allocating for SP_Constraint!" << endl;
        return -1;
    }


    if ( theDomain.addSP_Constraint(sp) == false )
    {
        cerr << "Error: (add_single_point_constraint_to_node) SP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};





