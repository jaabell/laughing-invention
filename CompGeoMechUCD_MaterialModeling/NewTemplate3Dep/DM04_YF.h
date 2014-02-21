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
// UPDATE HISTORY:    Guanzhou Jie updated for parallel Dec. 2006
//                    Nima Tafazzoli updated for API (Feb 2009)
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! \n
//! This documentation is the API for Dafalias-Manzari material model. For more information about theory background
//! see the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/LN.pdf#page=30" target="_blank">Lecture Notes
//! \n
//! \n
//!


#ifndef DM04_YF_H
#define DM04_YF_H

#include "YieldFunction.h"
#include <math.h>

class DM04_YF : public YieldFunction
{
    public:

        //! Reference: Dafalias & Manzari (2004)
        //! Yield function for Dafalias-Manzari: f=[(s_ij-p*alpha_ij)(s_ij-p*alpha_ij)]^0.5-sqrt(2/3)*m*p=0
        //! inputs:
        //! - m_which_in: to define the material parameter type (initial back stress),
        //!              - 0 for the (constant) material parameter,
        //!              - 1 for the (initial) internal scalar (N/A), and
        //!              - 2 for the (initial) internal tensor (N/A)
        //! - index_m_in
        //!
        //! - alpha_which_in: to define the material parameter type (initial back stress),
        //!              - 0 for the (constant) material parameter,
        //!              - 1 for the (initial) internal scalar (N/A), and
        //!              - 2 for the (initial) internal tensor (=2, for DM2004 Model)
        //! - index_alpha_in: to locate the position in the defined MaterialParameter command
        //!

        DM04_YF(int m_which_in, int index_m_in,
                int alpha_which_in, int index_alpha_in);

        //! Deconstructor for von-Mises yield function
        ~DM04_YF();

        DM04_YF() : YieldFunction(YF_TAG_DM04) {};

        YieldFunction* newObj();

        const char* getYieldFunctionType(void) const
        {
            return "DM04_YF";
        };


        //! YieldFunctionValue: to define the yield function
        //! inputs:
        //! - stresstensor &Stre: the tensor of stress
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter

        double YieldFunctionValue(const stresstensor& Stre,
                                  const MaterialParameter& MaterialParameter_in) const;

        //! StressDerivative: to obtain the derivative of the yield function respect to the stress
        //! inputs:
        //! - stresstensor &Stre: the tensor of stress
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter

        const stresstensor& StressDerivative(const stresstensor& Stre,
                                             const MaterialParameter& MaterialParameter_in) const;

        //! InTensorDerivative: to obtain the derivative of the yield function respect to the scalar (alpha_ij, if appliable)
        //! inputs:
        //! - stresstensor &Stre: the tensor of stress
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        //! - which: 0 if alpha is a constant material tensor, 1 if alpha is a changing internal tensor variable

        const stresstensor& InTensorDerivative(const stresstensor& Stre,
                                               const MaterialParameter& MaterialParameter_in,
                                               int which) const;
        //! getNumInternalScalar: to get the number of scalar internal varibles
        int getNumInternalScalar() const;

        //! getNumInternalTensor: to get the number of tensor internal varibles
        int getNumInternalTensor() const;

        //! getYieldFunctionRank: to get the rank of the yield function to the unit of stress
        int getYieldFunctionRank() const;

        //Guanzhou added for parallel
        //! sendSelf: for parallel computing
        int sendSelf(int commitTag, Channel& theChannel);

        //! recvSelf: for parallel computing
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    private:

        //!
        //! getm: to get m (constant in yield function)
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        double getm(const MaterialParameter& MaterialParameter_in) const;

        //!
        //! getalpha:to get alpha
        //! inputs:
        //! - MaterialParameter &MaterialParameter_in: the class of material parameter
        const stresstensor& getalpha(const MaterialParameter& MaterialParameter_in) const;

    private:
        int m_which;
        int index_m;
        int alpha_which;
        int index_alpha;

        static stresstensor DM04st;
};


#endif


