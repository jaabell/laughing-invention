///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// DESIGNER:          Nima Tafazzoli and  Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli and  Boris Jeremic
// DATE:              January 2009
// UPDATE HISTORY:
/////////////////////////////////////////////////////////////////////////////
//

#ifndef BardetCopnstraintLT_H
#define BardetCopnstraintLT_H

//#include <iostream>

#include "../ltensor/LTensor.h"

#include <NDMaterialLT.h>
#include <G3Globals.h>

#include <Channel.h>


#define CONSTANT_P_TRIAXIAL_LOADING_STRAIN_CONTROL         1
#define DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL            2
#define UNDRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL          3
#define UNDRAINED_CYCLIC_TRIAXIAL_LOADING_STRESS_CONTROL   4
#define UNDRAINED_SIMPLE_SHEAR_LOADING_STRAIN_CONTROL      5


DTensor2 BardetConstraintLT(int , double , NDMaterialLT *);

#endif
