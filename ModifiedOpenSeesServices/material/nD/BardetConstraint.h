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

#ifndef BardetCopnstraint_H
#define BardetCopnstraint_H

#include <stresst.h>
#include <straint.h>
#include <BJtensor.h>
#include <BJmatrix.h>
#include <BJvector.h>
#include <Matrix.h>
#include <Vector.h>

#include <NDMaterial.h>
//  #include <NewTemplate3Dep.h>



//#include "MaterialParameter.h"
//#include "ElasticState.h"
//#include "YieldFunction.h"
//#include "PlasticFlow.h"
//#include "ScalarEvolution.h"
//#include "TensorEvolution.h"
//cout
#include <G3Globals.h>

// Boris Jeremic 17Nov2008
#include <iostream>
using namespace std;

#include <Channel.h>
using namespace std;

straintensor BardetConstraint(int , double , NDMaterial*);

#endif
