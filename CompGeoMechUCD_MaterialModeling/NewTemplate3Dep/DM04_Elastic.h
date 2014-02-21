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
// PROGRAMMER:        Zhao Cheng
// Note:              Helpful discuss with Mahdi Taiebat and Professor Y.F. Dafalias
// DATE:              Fall 2005
// UPDATE HISTORY:    Guanzhou Jie updated for parallel, Dec. 2006
//
///////////////////////////////////////////////////////////////////////////////
//

#ifndef DM04_Elastic_H
#define DM04_Elastic_H

#include "ElasticState.h"

class DM04_Elastic : public ElasticState
{
    public:

        DM04_Elastic() : ElasticState(ES_TAG_DM04) {}; //Guanzhou added for parallel processing

        //! rf: Dafalias-Manzari 2004
        //! Elastic function for Dafalias-Manzari:
        //! inputs:
        //! - G0_in: to locate the position in the defined (constant) MaterialParameter command for G0
        //! - v_in: to locate the position in the defined (constant) MaterialParameter command for v (Poisson's ratio)
        //! - Pat_in: to locate the position in the defined (constant) MaterialParameter command for Pat (atmospheric pressure)
        //! - k_c_in,: to locate the position in the defined (constant) MaterialParameter command for k_c (pressure cut-off ratio)
        //! - e0_in: to locate the position in the defined (constant) MaterialParameter command for e0 (initial viod ratio)
        //! - stresstensor& initialStress: to input the intial stress
        //! - straintensor& initialStrain: to input the intial strain
        //!
        DM04_Elastic(int G0_in,
                     int v_in,
                     int Pat_in,
                     int k_c_in,
                     int e0_in,
                     const stresstensor& initialStress = zerostress,
                     const straintensor& initialStrain = zerostrain);

        ElasticState* newObj();
        ~DM04_Elastic();

        //! rf: Dafalias-Manzari 2004
        //! getElasticStiffness:
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter

        const BJtensor& getElasticStiffness (const MaterialParameter& MaterialParameter_in) const;

        //! rf: Dafalias-Manzari 2004
        //! getStress:
        const stresstensor& getStress() const;

        //Guanzhou added for parallel
        //! sendSelf: for parallel computing
        int sendSelf(int commitTag, Channel& theChannel);
        //! recvSelf: for parallel computing
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    private:

        //! rf: Dafalias-Manzari 2004
        //! getG0: to get
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double getG0(const MaterialParameter& MaterialParameter_in) const;

        //! rf: Dafalias-Manzari 2004
        //! getv: to get
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double getv(const MaterialParameter& MaterialParameter_in) const;

        //! rf: Dafalias-Manzari 2004
        //! getPat: to get
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double getPat(const MaterialParameter& MaterialParameter_in) const;

        //! rf: Dafalias-Manzari 2004
        //! getk_c: to get
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double getk_c(const MaterialParameter& MaterialParameter_in) const;

        //! rf: Dafalias-Manzari 2004
        //! gete0: to get
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double gete0(const MaterialParameter& MaterialParameter_in) const;

    private:

        static BJtensor ElasticStiffness;

        int G0_index;
        int v_index;
        int Pat_index;
        int k_c_index;
        int e0_index;
};

#endif


