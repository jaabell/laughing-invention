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
// UPDATE HISTORY:    August 2009
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Fixing Specific DOF of a Node</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of fixing specific DOF of a node.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - NodeNumber: integer tag identifying the node to be constrained
//! - dof_number: dof to be fixed



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_support_to_node(int NodeNumber,
                        int dof_number)
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
    sp = new SP_Constraint(currentSpTag, NodeNumber,
                           dof_number - 1, // for SP dofs start from 0, for us from 1
                           0.0);  // DOF_value

    if (sp == NULL)
    {
        cerr << "Error: (add_support_to_node) memory allocating for SP_Constraint!" << endl;
        return -1;
    }


    if( theDomain.addSP_Constraint(sp) == false )
    {
        cerr << "Error: (add_support_to_node) SP_Constraint could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};


