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
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              March 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////
//
// Summarized form of MaterialParameter for specific usage of constitutive material models
// This is the container to store material constants:
// Internal_Tensor:         to store initial evolution tensor variables first time (usually zero tensor),
//                          and tensor variables thereafter;
// Num_Internal_Tensor:     the number of Internal_Tensor;


#ifndef ConstitutiveMaterialParameter_H
#define ConstitutiveMaterialParameter_H

#include <stresst.h>
#include <FEM_ObjectBroker.h>
#include <Channel.h>
# include <ID.h>
# include <Vector.h>
#include <MovableObject.h>


class ConstitutiveMaterialParameter : public MovableObject
{
    public:

        ConstitutiveMaterialParameter(const stresstensor* Internal_Tensor_in,
                                      int Num_Internal_Tensor_in = 1);
        ConstitutiveMaterialParameter(const double* Internal_Scalar_in,
                                      int Num_Internal_Scalar_in = 1);

        ~ConstitutiveMaterialParameter( );
        ConstitutiveMaterialParameter(const ConstitutiveMaterialParameter& refer_ConstitutiveMaterialParameter);
        ConstitutiveMaterialParameter* newObj(const char*);

        int getNum_Internal_Tensor() const;
        int getNum_Internal_Scalar() const;

        const double getInternal_Scalar() const;
        const stresstensor& getInternal_Tensor() const;

        int setInternal_Scalar(double newInternal_Scalar) ;
        int setInternal_Tensor(const stresstensor& newInternal_Tensor);

        //Guanzhou added for parallel
        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    private:

        stresstensor* Internal_Tensor;
        int Num_Internal_Tensor;
        double* Internal_Scalar;
        int Num_Internal_Scalar;
};


#endif

