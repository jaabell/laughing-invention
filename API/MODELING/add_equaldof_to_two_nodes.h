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
// DATE:              May 2009
// UPDATE HISTORY:    August 2009 (Removing Pointers)
//                    September 2009 (API)
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Equal-DOF</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of equal dof.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1)
{

    int numMPs = theDomain.getNumMPs();

    int numDOF = 1;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};




//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 2;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2,
                              int dofID3)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 3;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;
    rcDOF (2) = --dofID3;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;
    Ccr (2, 2) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};






//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2,
                              int dofID3,
                              int dofID4)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 4;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;
    rcDOF (2) = --dofID3;
    rcDOF (3) = --dofID4;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;
    Ccr (2, 2) = 1.0;
    Ccr (3, 3) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2,
                              int dofID3,
                              int dofID4,
                              int dofID5)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 5;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;
    rcDOF (2) = --dofID3;
    rcDOF (3) = --dofID4;
    rcDOF (4) = --dofID5;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;
    Ccr (2, 2) = 1.0;
    Ccr (3, 3) = 1.0;
    Ccr (4, 4) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};






//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2,
                              int dofID3,
                              int dofID4,
                              int dofID5,
                              int dofID6)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 6;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;
    rcDOF (2) = --dofID3;
    rcDOF (3) = --dofID4;
    rcDOF (4) = --dofID5;
    rcDOF (5) = --dofID6;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;
    Ccr (2, 2) = 1.0;
    Ccr (3, 3) = 1.0;
    Ccr (4, 4) = 1.0;
    Ccr (5, 5) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};






//! Inputs:
//! - nodeRetain: integer tag identifying the retained, or master node (rNode)
//! - nodeConstr: integer tag identifying the constrained, or slave node (cNode)
//! - dofID: nodal degrees-of-freedom that are constrained at the nodeConstr to be the same as those at the nodeRetain Valid range is from 1 to 7.




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_equaldof_to_two_nodes(int nodeRetain,
                              int nodeConstr,
                              int dofID1,
                              int dofID2,
                              int dofID3,
                              int dofID4,
                              int dofID5,
                              int dofID6,
                              int dofID7)
{
    int numMPs = theDomain.getNumMPs();

    int numDOF = 7;
    ID rcDOF (numDOF);
    rcDOF (0) = --dofID1;
    rcDOF (1) = --dofID2;
    rcDOF (2) = --dofID3;
    rcDOF (3) = --dofID4;
    rcDOF (4) = --dofID5;
    rcDOF (5) = --dofID6;
    rcDOF (6) = --dofID7;

    Matrix Ccr (numDOF, numDOF);
    Ccr.Zero();
    Ccr (0, 0) = 1.0;
    Ccr (1, 1) = 1.0;
    Ccr (2, 2) = 1.0;
    Ccr (3, 3) = 1.0;
    Ccr (4, 4) = 1.0;
    Ccr (5, 5) = 1.0;
    Ccr (6, 6) = 1.0;

    MP_Constraint* mp = 0;
    mp = new MP_Constraint (numMPs, nodeRetain, nodeConstr, Ccr, rcDOF, rcDOF);

    if (mp == NULL)
    {
        cerr << "Error: (add_equaldof_to_two_nodes) memory allocating for EqualDOF!" << endl;
        return -1;
    }


    if( theDomain.addMP_Constraint(mp) == false )
    {
        cerr << "Error: (add_equaldof_to_two_nodes) MP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





