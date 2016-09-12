///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Sumeet, Jose Abell
// DATE:               September, 2016
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


#ifndef TwentyNodeBrick_upU_LT_Elastic_CPP
#define TwentyNodeBrick_upU_LT_Elastic_CPP

#include "TwentyNodeBrick_upU_LT_Elastic.h"


TwentyNodeBrick_upU_LT_Elastic::TwentyNodeBrick_upU_LT_Elastic(int element_number,
        int node_numb_1,
        int node_numb_2,
        int node_numb_3,
        int node_numb_4,
        int node_numb_5,
        int node_numb_6,
        int node_numb_7,
        int node_numb_8,
        int node_numb_9,
        int node_numb_10,
        int node_numb_11,
        int node_numb_12,
        int node_numb_13,
        int node_numb_14,
        int node_numb_15,
        int node_numb_16,
        int node_numb_17,
        int node_numb_18,
        int node_numb_19,
        int node_numb_20,
        NDMaterialLT *Globalmmodel,
        double nn,
        double alf,
        double rs,
        double rf,
        double permb_x,
        double permb_y,
        double permb_z,
        double kks,
        double kkf)

    : TwentyNodeBrick_upU_LT(element_number,
        node_numb_1,node_numb_2,node_numb_3,node_numb_4,
        node_numb_5,node_numb_6,node_numb_7,node_numb_8,
        node_numb_9,node_numb_10,node_numb_11,node_numb_12,
        node_numb_13,node_numb_14,node_numb_15,node_numb_16,
        node_numb_17,node_numb_18,node_numb_19,node_numb_20,
        Globalmmodel,
        nn,alf,rs,rf,permb_x,
        permb_y,permb_z,kks,kkf)
{

}

//====================================================================
TwentyNodeBrick_upU_LT_Elastic::TwentyNodeBrick_upU_LT_Elastic(): TwentyNodeBrick_upU_LT()
{

}

//=============================================================================
const Matrix &TwentyNodeBrick_upU_LT_Elastic::getTangentStiff()
{
    return  TwentyNodeBrick_upU_LT::getConstStiff();
}


#endif

