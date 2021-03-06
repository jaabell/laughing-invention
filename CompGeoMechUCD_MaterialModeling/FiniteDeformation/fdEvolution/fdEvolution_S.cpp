//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Finite Deformation Hyper-Elastic classes
//# CLASS:
//#
//# VERSION:           0.6_(1803398874989) (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhao Cheng, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//#
//#
//# DATE:              July 2004
//# UPDATE HISTORY:
//#
//===============================================================================

#ifndef fdEvolution_S_CPP
#define fdEvolution_S_CPP

#include "fdEvolution_S.h"

fdEvolution_S* fdEvolution_S::newObj()
{
    fdEvolution_S* newEL = new fdEvolution_S( *this );
    return newEL;
}

double fdEvolution_S::HModulus(const stresstensor& sts, const FDEPState& fdepstate) const
{
    return 0.0;
}

// void fdEvolution_S::print()
// {
//     cerr << (*this);
// }
//
// ostream& operator<< (ostream& os, const fdEvolution_S & ev)
// {
//    os << "Scalar Evolution Law's Parameters: " << "\n";
//    return os;
// }
//
#endif

