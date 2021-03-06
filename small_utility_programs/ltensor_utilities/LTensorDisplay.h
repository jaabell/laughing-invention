///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Jan 2012
// UPDATE HISTORY:     Full update history in git repository.
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


// Purpose: displayage of LTensors


#ifndef LTENSORDISPLAY_H_INCLUDED
#define LTENSORDISPLAY_H_INCLUDED

#include "../../ltensor/LTensor.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


// To print LTensors with a format similar to that of NDarray :)
namespace LTensorDisplay
{
void print(DTensor1 a, string name = "a", string msg = "", int baseindex = 0);
void print(DTensor2 a, string name = "a", string msg = "", int baseindex = 0);
void print(DTensor3 a, string name = "a", string msg = "", int baseindex = 0);
void print(DTensor4 a, string name = "a", string msg = "", int baseindex = 0);
}
#endif // LTENSORDISPLAY_H_INCLUDED
