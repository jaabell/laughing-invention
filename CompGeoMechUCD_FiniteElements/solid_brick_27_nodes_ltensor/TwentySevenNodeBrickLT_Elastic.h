///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Zhaohui Yang, Xiaoyan Wu, Nima Tafazzoli, & Jose Abell
// DATE:               NOv. 2013
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

/////////////////////////////////////////////////////////////////////////////////////
// Adapted by Sumeet, from TwentySevenNodeBrick to use the LTensor library for
// tensorial representation. [September, 2016]
/////////////////////////////////////////////////////////////////////////////////////


#ifndef TwentySevenNodeBrickLT_ElasticLT_H
#define TwentySevenNodeBrickLT_ElasticLT_H


#include "TwentySevenNodeBrickLT.h"

class Node;

class TwentySevenNodeBrickLT_Elastic: public TwentySevenNodeBrickLT
{

public:
    TwentySevenNodeBrickLT_Elastic( int element_number,
                      int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                      int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                      NDMaterialLT *Globalmmodel);

    TwentySevenNodeBrickLT_Elastic ();

    const Matrix &getTangentStiff ();
    
    std::string getElementName() const
    {
        return "TwentySevenNodeBrickLT_Elastic";
    }
};


#endif
