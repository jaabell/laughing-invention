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
// DATE:              May 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////




//! Inputs:
//! -




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n





int add_penalty_displacement_time_history_linear(int PatternNumber,
        int PenaltyElementNumber,
        int dof_to_be_shaken,
        double Final_Displacement_Value)
{

    double PenaltyStiffness = theDomain.getElement(PenaltyElementNumber)->getPenaltyStiffness();

    int NodeNumber = (theDomain.getElement(PenaltyElementNumber)->getExternalNodes())(0);


    LoadPattern* theLinearPattern = 0;
    theLinearPattern = new Linear_Disp_TimeHistory_Penalty(PatternNumber,
            PenaltyStiffness,
            NodeNumber,
            dof_to_be_shaken,
            Final_Displacement_Value);

    if (theLinearPattern == 0)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_linear) ran out of memory creating load pattern ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(theLinearPattern) == false)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_linear) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;

};










