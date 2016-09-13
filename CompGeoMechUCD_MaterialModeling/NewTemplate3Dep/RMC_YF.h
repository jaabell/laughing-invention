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
//                    Nima Tafazzoli updated for API (Feb 2009)
//
///////////////////////////////////////////////////////////////////////////////
//

//!
//! @mainpage
//! \n
//! \n
//! This documentation is the API for Rounded Mohr-Coulomb material model. For more information about theory background
//! see the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/research/index.html#LectureNotes" target="_blank">Lecture Notes
//! \n
//! \n
//!


#ifndef RMC_YF_H
#define RMC_YF_H

#include "YieldFunction.h"
#include <math.h>

class RMC_YF : public YieldFunction
{
    public:
        RMC_YF(int a_which_in = -1, int index_a_in = 0,
               int k_which_in = -1, int index_k_in = 0,
               int r_which_in = -1, int index_r_in = 0);
        ~RMC_YF();

        const char* getYieldFunctionType(void) const
        {
            return "RMC_YF";
        };

        YieldFunction* newObj();

        double YieldFunctionValue(const stresstensor& Stre,
                                  const MaterialParameter& MaterialParameter_in) const;

        const stresstensor& StressDerivative(const stresstensor& Stre,
                                             const MaterialParameter& MaterialParameter_in) const;

        double InScalarDerivative(const stresstensor& Stre,
                                  const MaterialParameter& MaterialParameter_in,
                                  int which) const;

        int getNumInternalScalar() const;
        int getNumInternalTensor() const;
        int getYieldFunctionRank() const;

        //Guanzhou added for parallel
        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    private:
        double geta(const MaterialParameter& MaterialParameter_in) const;
        double getk(const MaterialParameter& MaterialParameter_in) const;
        double getr(const MaterialParameter& MaterialParameter_in) const;

        double RoundedFunctionf1(double s, double r) const;
        double RoundedFunctionf2(double s, double r) const;
        double RoundedFunctiondf1(double s, double r) const;
        double RoundedFunctiondf2(double s, double r) const;
        double RoundedFunction(double s, double r) const;

    private:
        int a_which;
        int index_a;
        int k_which;
        int index_k;
        int r_which;
        int index_r;

        static stresstensor RMCst;
};


#endif

