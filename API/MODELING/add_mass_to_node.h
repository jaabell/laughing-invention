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
//! - NodeNumber: integer tag of the node that mass would be applied to
//! - massvalue(#): the amount of mass assigned to each degree of freedom



int add_mass_to_node(int NodeNumber,
                     double massvalue1,
                     double massvalue2,
                     double massvalue3)
{
    int ndof = 3;
    Matrix mass(ndof, ndof);

    double themass[3] = {massvalue1, massvalue2, massvalue3};

    for (int i = 0; i < ndof; i++)
    {
        mass(i, i) = themass[i];
    }

    //      theDomain.getNode(NodeNumber)->setDamping_Coefficient(alpha);

    if ( theDomain.getNode(NodeNumber)->setMass(mass) != 0 )
    {
        cerr << "Error: (add_mass_to_node) Mass could not be added to the node " << endl;
        return -1;
    }

    return 0;
};





//! Inputs:
//! - NodeNumber: integer tag of the node that mass would be applied to
//! - massvalue(#): the amount of mass assigned to each degree of freedom


int add_mass_to_node(int NodeNumber,
                     double massvalue1,
                     double massvalue2,
                     double massvalue3,
                     double massvalue4,
                     double massvalue5,
                     double massvalue6)
{
    int ndof = 6;
    Matrix mass(ndof, ndof);

    double themass[6] = {massvalue1, massvalue2, massvalue3, massvalue4, massvalue5, massvalue6};

    for (int i = 0; i < ndof; i++)
    {
        mass(i, i) = themass[i];
    }

    //      theDomain.getNode(NodeNumber)->setDamping_Coefficient(alpha);

    if ( theDomain.getNode(NodeNumber)->setMass(mass) != 0 )
    {
        cerr << "Error: (add_mass_to_node) Mass could not be added to the node " << endl;
        return -1;
    }

    return 0;

};





//! Inputs:
//! - NodeNumber: integer tag of the node that mass would be applied to
//! - massvalue(#): the amount of mass assigned to each degree of freedom


int add_mass_to_node(int NodeNumber,
                     double massvalue1,
                     double massvalue2,
                     double massvalue3,
                     double massvalue4,
                     double massvalue5,
                     double massvalue6,
                     double massvalue7)
{
    int ndof = 7;
    Matrix mass(ndof, ndof);

    double themass[7] = {massvalue1, massvalue2, massvalue3, massvalue4, massvalue5, massvalue6, massvalue7};

    for (int i = 0; i < ndof; i++)
    {
        mass(i, i) = themass[i];
    }

    //      theDomain.getNode(NodeNumber)->setDamping_Coefficient(alpha);

    if ( theDomain.getNode(NodeNumber)->setMass(mass) != 0 )
    {
        cerr << "Error: (add_mass_to_node) Mass could not be added to the node " << endl;
        return -1;
    }

    return 0;

};



