///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               January 20, 2016
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


int add_equaldof_to_two_nodes_two_dofs(int masterNode,
                                       int slaveNode,
                                       int masterDof,
                                       int slaveDof)
{
    int numMPs = theDomain.getNumMPs();

    ID constrainedDOF(1);
    ID retainedDOF (1);
    constrainedDOF (0) = --slaveDof;
    retainedDOF (0) = --masterDof;

    Matrix Ccr (1, 1);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    // Ccr (1, 1) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, masterNode, slaveNode, Ccr, constrainedDOF, retainedDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if ( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};




