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
// Internal_Scalar:         to store initial evolution scalar variables first time,
//                          and scalar variables thereafter;
// Num_Internal_Scalar:     the number of Internal_Scalar;


#ifndef ConstitutiveMaterialParameter_CPP
#define ConstitutiveMaterialParameter_CPP

#include "ConstitutiveMaterialParameter.h"

//================================================================================
ConstitutiveMaterialParameter::ConstitutiveMaterialParameter(const stresstensor* Internal_Tensor_in,
        int Num_Internal_Tensor_in)
    : MovableObject(Constitutive_MP_TAG), Internal_Scalar(NULL), Num_Internal_Scalar(0)
{
    // Tensor (Kinematic) Internal Variables
    Internal_Tensor = new stresstensor[Num_Internal_Tensor = Num_Internal_Tensor_in];

    if (!Internal_Tensor)
    {
        cerr << "ConstitutiveMaterialParameter::Insufficient memory! " << endln;
        exit (1);
    }

    Internal_Tensor[0] = Internal_Tensor_in[0];
}

//================================================================================
ConstitutiveMaterialParameter::ConstitutiveMaterialParameter(const double* Internal_Scalar_in,
        int Num_Internal_Scalar_in)
    : MovableObject(Constitutive_MP_TAG), Internal_Tensor(NULL), Num_Internal_Tensor(0)
{
    // Tensor (Kinematic) Internal Variables
    Internal_Scalar = new double[Num_Internal_Scalar = Num_Internal_Scalar_in];

    if (!Internal_Scalar)
    {
        cerr << "ConstitutiveMaterialParameter::Insufficient memory! " << endln;
        exit (1);
    }

    Internal_Scalar[0] = Internal_Scalar_in[0];
}

//================================================================================
// Destructor
ConstitutiveMaterialParameter::~ConstitutiveMaterialParameter( )
{
    if (Internal_Tensor)
    {
        delete [] Internal_Tensor;
    }

    if (Internal_Scalar)
    {
        delete [] Internal_Scalar;
    }
}

//================================================================================
// Copy constructor
ConstitutiveMaterialParameter::ConstitutiveMaterialParameter(const ConstitutiveMaterialParameter& refer_ConstitutiveMaterialParameter)
    : MovableObject(MP_TAG)
{
    // Tensor (Kinematic) Internal Variables
    if (refer_ConstitutiveMaterialParameter.getNum_Internal_Tensor() > 0)
    {
        Internal_Tensor = new stresstensor[Num_Internal_Tensor = refer_ConstitutiveMaterialParameter.getNum_Internal_Tensor()];

        if (!Internal_Tensor)
        {
            cerr << "ConstitutiveMaterialParameter::Insufficient memory! " << endln;
            exit (1);
        }

        Internal_Tensor[0] = refer_ConstitutiveMaterialParameter.getInternal_Tensor();
    }
    else
    {
        Num_Internal_Tensor = 0;
        Internal_Tensor = NULL;
    }

}

//================================================================================
// Create a new class pointer
ConstitutiveMaterialParameter* ConstitutiveMaterialParameter::newObj(const char* hardening)
{
    ConstitutiveMaterialParameter* ptr_ConstitutiveMaterialParameter;

    if ((strcmp(hardening, "Tensor") == 0) || (strcmp(hardening, "tensor") == 0))
    {
        ptr_ConstitutiveMaterialParameter = new  ConstitutiveMaterialParameter (this->Internal_Tensor,
                this->Num_Internal_Tensor);
    }
    else if ((strcmp(hardening, "Scalar") == 0) || (strcmp(hardening, "scalar") == 0))
    {
        ptr_ConstitutiveMaterialParameter = new  ConstitutiveMaterialParameter (this->Internal_Scalar,
                this->Num_Internal_Scalar);
    }

    return ptr_ConstitutiveMaterialParameter;

}

//================================================================================
// getNum_Internal_Scalar
int ConstitutiveMaterialParameter::getNum_Internal_Scalar() const
{
    return Num_Internal_Scalar;
}

//================================================================================
// getNum_Internal_Tensor
int ConstitutiveMaterialParameter::getNum_Internal_Tensor() const
{
    return Num_Internal_Tensor;
}

//================================================================================
// getInternal_Scalar
const double ConstitutiveMaterialParameter::getInternal_Scalar() const
{
    return Internal_Scalar[0];
}

//================================================================================
// getInternal_Tensor
const stresstensor& ConstitutiveMaterialParameter::getInternal_Tensor() const
{
    return Internal_Tensor[0];
}

//================================================================================
// setInternal_Scalar
int ConstitutiveMaterialParameter::setInternal_Scalar(double newInternal_Scalar)
{
    Internal_Scalar[0] = newInternal_Scalar;
    return 0;
}

//================================================================================
// setInternal_Tensor
int ConstitutiveMaterialParameter::setInternal_Tensor(const stresstensor& newInternal_Tensor)
{
    Internal_Tensor[0] = newInternal_Tensor;
    return 0;
}

//================================================================================
int ConstitutiveMaterialParameter::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int ConstitutiveMaterialParameter::recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}


#endif

