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
// PROGRAMMER:        Mario Martinelli, Nima Tafazzoli, Boris Jeremic
//
// DATE:              December 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_element_contact_nonlinear_3dof_to_7dof(int ElementNumber,
        int jNode_7dof,
        int iNode_3dof,
        double Knormal,
        double frictionRatio,
        double maximum_gap,
        double x_local_1,
        double x_local_2,
        double x_local_3)
{
    Element* theElement = 0;
    theElement = new ContactElement_Nonlinear_3DOF_7DOF(ElementNumber, jNode_7dof, iNode_3dof, Knormal, frictionRatio, maximum_gap, x_local_1, x_local_2, x_local_3);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_contact_3dof_to_7dof) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING (add_element_contact_3dof_to_7dof) could not add element to the domain\n";
        cerr << "Element Number: " << ElementNumber << endln;
        return -1;
    }




    //==================================================================================

    int numMPs = theDomain.getNumMPs();

    //     int numDOF = 3;
    //
    //     ID retainedDOF (numDOF);
    //     retainedDOF(0) = 4;
    //     retainedDOF(1) = 5;
    //     retainedDOF(2) = 6;
    //
    //     ID constrainedDOF (numDOF);
    //     constrainedDOF(0) = 0;
    //     constrainedDOF(1) = 1;
    //     constrainedDOF(2) = 2;
    //
    //     Matrix Ccr (numDOF,numDOF);
    //     Ccr.Zero();
    //     Ccr (0,0) = 1.0;
    //     Ccr (1,1) = 1.0;
    //     Ccr (2,2) = 1.0;


    int numDOF = 1;

    ID retainedDOF (numDOF);
    retainedDOF(0) = 6;

    ID constrainedDOF (numDOF);
    constrainedDOF(0) = 2;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;



    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, jNode_7dof, iNode_3dof, Ccr, constrainedDOF, retainedDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_element_contact_3dof_to_7dof) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_element_contact_3dof_to_7dof) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    //==================================================================================


    return 0;

};







