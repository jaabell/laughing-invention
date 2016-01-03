///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Dec 2015
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
// Adapted by Jose Abell, from EightNodeBrick to use the LTensor library for
// tensorial representation.
/////////////////////////////////////////////////////////////////////////////////////


#ifndef EightNodeBrickLT_NoOutputLT_H
#define EightNodeBrickLT_NoOutputLT_H

#include "EightNodeBrickLT.h"


// Output is 6 components of strain 6 components of plastic strain and 6 of stress per gauss point + 1 for update time
#define EightNodeBrickLT_NoOutput_NUMBER_OF_GAUSSPOINTS 8
#define EightNodeBrickLT_NoOutput_OUTPUT_SIZE 0

class Node;

class EightNodeBrickLT_NoOutput: public EightNodeBrickLT
{

public:
    EightNodeBrickLT_NoOutput( int element_number,
                               int node_numb_1, int node_numb_2, int node_numb_3, int node_numb_4,
                               int node_numb_5, int node_numb_6, int node_numb_7, int node_numb_8,
                               NDMaterialLT *Globalmmodel);

    EightNodeBrickLT_NoOutput ();

    virtual int getOutputSize() const;
    virtual const Vector &getOutput() ;

    std::string getElementName() const
    {
        return "EightNodeBrickLT_NoOutput";
    }
};


#endif
