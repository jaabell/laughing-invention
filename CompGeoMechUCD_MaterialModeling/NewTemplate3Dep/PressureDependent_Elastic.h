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
// DESIGNER:          Zhao Cheng, Boris Jeremic
// PROGRAMMER:        Zhao Cheng,
// DATE:              Fall 2005
// UPDATE HISTORY:    Guanzhou Jie updated for parallel Dec 2006
//
///////////////////////////////////////////////////////////////////////////////
//

#ifndef PressureDependent_Elastic_H
#define PressureDependent_Elastic_H

#include "ElasticState.h"
#include <math.h>

class PressureDependent_Elastic : public ElasticState
{
    public:

        PressureDependent_Elastic() : ElasticState(ES_TAG_PressureDependentElastic) {}; //Guanzhou added for parallel processing

        PressureDependent_Elastic(int E0_in,
                                  int v_in,
                                  int m_in,
                                  int p_ref_in,
                                  int k_cut_in,
                                  const stresstensor& initialStress = zerostress,
                                  const straintensor& initialStrain = zerostrain);
        ElasticState* newObj();
        ~PressureDependent_Elastic();

        const BJtensor& getElasticStiffness(const MaterialParameter& MaterialParameter_in) const;

        //Guanzhou added for parallel
        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    private:

        double getE0(const MaterialParameter& MaterialParameter_in) const;
        double getv(const MaterialParameter& MaterialParameter_in) const;
        double getm(const MaterialParameter& MaterialParameter_in) const;
        double getp_ref(const MaterialParameter& MaterialParameter_in) const;
        double getk_cut(const MaterialParameter& MaterialParameter_in) const;

    private:

        static BJtensor ElasticStiffness;

        int E0_index;
        int v_index;
        int m_index;
        int p_ref_index;
        int k_cut_index;

};

#endif

