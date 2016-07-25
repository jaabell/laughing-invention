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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Jose Abell, Boris Jeremic
//
// DATE:              January 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//! Inputs:
//! - NodeNumber: integer tag identifying the node number
//! - dofNumber: integer tag identifying the dof number



//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n



int remove_support_from_node(int NodeNumber, int dofNumber)
{


    SP_Constraint* theSP = 0;
    bool found = false;
    int spTag = 0;

    dofNumber = dofNumber - 1;


    SP_ConstraintIter& theSPs = theDomain.getSPs();

    while ((found == false) && ((theSP = theSPs()) != 0))
    {
        int nodeTag = theSP->getNodeTag();
        int dof = theSP->getDOF_Number();

        if (nodeTag == NodeNumber && dof == dofNumber)
        {
            spTag = theSP->getTag();
            found = true;
        }
    }

    if (found == true)
    {
        SP_Constraint* theSPconstraint = 0;
        theSPconstraint = theDomain.removeSP_Constraint(spTag);


        if (theSPconstraint == NULL)
        {
            cerr << "Error: (remove_support_from_node) memory allocation problem!" << endl;
            return -1;
        }

        if (theSPconstraint != 0)
        {
            delete theSPconstraint;
        }
    }



    return 0;

};




