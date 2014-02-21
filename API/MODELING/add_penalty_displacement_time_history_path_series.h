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




int add_penalty_displacement_time_history_path_series(int PatternNumber,
        int PenaltyElementNumber,
        int dof_to_be_shaken,
        double TimeIncrement,
        double LoadFactor,
        string Displacementinputfilename)
{

    double PenaltyStiffness = theDomain.getElement(PenaltyElementNumber)->getPenaltyStiffness();

    int NodeNumber = (theDomain.getElement(PenaltyElementNumber)->getExternalNodes())(0);

    const char* filename = Displacementinputfilename.c_str();

    LoadPattern* thePathSeriesPattern = 0;
    thePathSeriesPattern = new PathSeries_Disp_TimeHistory_Penalty(PatternNumber,
            PenaltyStiffness,
            NodeNumber,
            dof_to_be_shaken,
            TimeIncrement,
            LoadFactor,
            filename);

    if (thePathSeriesPattern == 0)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_path_series) ran out of memory creating load pattern ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(thePathSeriesPattern) == false)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_path_series) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;

};





int add_penalty_displacement_time_history_path_time_series(int PatternNumber,
        int PenaltyElementNumber,
        int dof_to_be_shaken,
        double LoadFactor,
        string Displacementinputfilename)
{

    double PenaltyStiffness = theDomain.getElement(PenaltyElementNumber)->getPenaltyStiffness();

    int NodeNumber = (theDomain.getElement(PenaltyElementNumber)->getExternalNodes())(0);

    const char* filename = Displacementinputfilename.c_str();

    LoadPattern* thePathSeriesPattern = 0;
    thePathSeriesPattern = new PathSeries_Disp_TimeHistory_Penalty(PatternNumber,
            PenaltyStiffness,
            NodeNumber,
            dof_to_be_shaken,
            LoadFactor,
            filename);

    if (thePathSeriesPattern == 0)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_path_series) ran out of memory creating load pattern ";
        cerr << PatternNumber << endln;
        return -1;
    }

    if (theDomain.addLoadPattern(thePathSeriesPattern) == false)
    {
        cerr << "WARNING: (add_penalty_displacement_time_history_path_series) could not add load pattern to the domain " << endl;
        return -1;
    }

    return 0;

};







