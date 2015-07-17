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
// PROGRAMMER:
// DATE:
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////



#ifndef ClassicElastoplasticMaterial_H
#define ClassicElastoplasticMaterial_H


#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>



using namespace std;


/*

YieldFunctionType, ElasticityType, PlasticFlowType, HardeningLawType are the user-defined classes
that provide the different components of this elastoplastic model. Called Base Elastoplastic Template
Classes (BET Classes)

* All BET Classes  must provide:

C++ "Rule of 5"
 + An empty constructor (default constructor)
 + A copy constructor
 + A copy assignment operator
 + A move constructor
 + A move assign operator
...and
 + A name member function that returns a std::string. For example.
    SomeYieldFunctionType yf(...parameters...);
    std::string name = yf.name();
 + A getObjectSize() function. Returnig size in bytes of the object in memory.


* YieldFunctionType must additionally provide



* ElasticityType must additionally provide



* PlasticFlowType must additionally provide



* HardeningLawType must additionally provide



*/
template < class ElasticityType, class YieldFunctionType, class PlasticFlowType, class HardeningLawType, int thisClassTag >
class ClassicElastoplasticMaterial : public NDMaterialLT
{

public:



//==================================================================================================
//  Void constructor
//==================================================================================================
//Empty constructor is needed for parallel, sets all the pointers to zero. Resources will be
//    allocated later
    ClassicElastoplasticMaterial( )
        : NDMaterialLT(0, thisClassTag),
          NDMaterialLT(tag, thisClassTag),
          TrialStress(3, 3, 0.0),
          TrialStrain(3, 3, 0.0),
          TrialPlastic_Strain(3, 3, 0.0),
          CommitStress(3, 3, 0.0),
          CommitStrain(3, 3, 0.0),
          CommitPlastic_Strain(3, 3, 0.0),
          Stiffness(3, 3, 3, 30.0),
          yf(),
          et(),
          pf(),
          hl()
    {

    }



//==================================================================================================
//  Main constructor
//==================================================================================================
// Constructor... invokes copy constructor for base elastoplastic template parameters
    ClassicElastoplasticMaterial(int tag,
                                 const YieldFunctionType& yf_in,
                                 const ElasticityType& et_in,
                                 const PlasticFlowType& pf_in,
                                 const HardeningLawType& hl_in
                                )
        : NDMaterialLT(tag, thisClassTag),
          TrialStress(3, 3, 0.0),
          TrialStrain(3, 3, 0.0),
          TrialPlastic_Strain(3, 3, 0.0),
          CommitStress(3, 3, 0.0),
          CommitStrain(3, 3, 0.0),
          CommitPlastic_Strain(3, 3, 0.0),
          Stiffness(3, 3, 3, 30.0),
          yf(yf_in),
          et(et_in),
          pf(pf_in),
          hl(hl_in)
    {

    }



//==================================================================================================
//  Destructor
//==================================================================================================
    ~ClassicElastoplasticMaterial(void)
    {

    }




//==================================================================================================
//  Class type function
//==================================================================================================
// Returns a null-terminated character string with the name of the class
    const char *getClassType(void) const
    {
        std::string name("ClassicElastoplasticMaterial");
        name += "_";
        name += yf.name();
        name += "_";
        name += et.name();
        name += "_";
        name += pf.name();
        name += "_";
        name += hl.name();

        return name.c_str();
    };


//==================================================================================================
//  Setters for components (invoke assign copy on BET Classes)
//==================================================================================================

    void setYieldFunction(YieldFunctionType& yf_)
    {
        yf = std::move(yf_);
    }

    void setElasticity(ElasticityType& et_)
    {
        et = std::move(et_);
    }

    void setPlasticFlow(PlasticFlowType& pf_)
    {
        pf = std::move(pf_);
    }

    void setHardeningLaw(HardeningLawType& hl_)
    {
        hl = std::move(hl_);
    }


//==================================================================================================
//  Set Trial strain and trial strain increment
//==================================================================================================
    // For total strain-based elements.
    // Receives the current total strain at a GP.
    // This function then computes the incremental strain (subtracting from the committed one)
    // and sets the increment.
    // Returns a success flag from the call to setTrialStrainIncr
    int setTrialStrain( const DTensor2 &v )
    {

        DTensor2 result( 3, 3, 0.0 );
        TrialStrain(i, j) = v(i, j);
        result( i, j ) = v( i, j ) - CommitStrain( i, j );

        return setTrialStrainIncr( result );
    }

    // Directly sets the trial strain increment and does an explicit or implicit step.
    // Returns a flag depending on the result of the step.
    int setTrialStrainIncr( const DTensor2 &v )
    {
        return this->explicitStep(strain_increment);
        //
        // or
        //
        // return this->implicitStep(strain_increment);
    }

//==================================================================================================
//  Getters
//==================================================================================================

    const DTensor4 &getTangentTensor( void )
    {
        return *Stiffness;
    }
    const DTensor2 &getStressTensor( void )
    {
        return *TrialStress;
    }
    const DTensor2 &getStrainTensor( void )
    {
        return *TrialStrain;
    }
    const DTensor2 &getPlasticStrainTensor( void )
    {
        return *TrialPlastic_Strain;
    }

    const DTensor2  &getCommittedStressTensor(void)
    {
        return *CommitStress;
    }

    const DTensor2 &getCommittedStrainTensor(void)
    {
        return *CommitStrain;
    }

    const DTensor2 &getCommittedPlasticStrainTensor(void)
    {
        return *CommitPlastic_Strain;
    }



//==================================================================================================
//  State commiting and reversion
//==================================================================================================

    // Forwards the trial variables to commited variables and calls commit state on the
    // BET classes
    int commitState(void)
    {
        int errorcode = 0;
        CommitStress(i, j) = TrialStress(i, j);
        CommitStrain(i, j) = TrialStrain(i, j);
        CommitPlastic_Strain(i, j) = TrialPlastic_Strain(i, j);

        if (errorcode = yf.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = et.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = pf.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = hl.commitState() != 0)
        {
            return errorcode;
        }

        return errorcode;
    }

    //Reverts the commited variables to the trials and calls revert on BET Classes.
    int revertToLastCommit(void);
    {
        int errorcode = 0;
        TrialStress(i, j) = CommitStress(i, j);
        TrialStrain(i, j) = CommitStrain(i, j);
        TrialPlastic_Strain(i, j) = CommitPlastic_Strain(i, j);

        if (errorcode = yf.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = et.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = pf.commitState() != 0)
        {
            return errorcode;
        }

        if (errorcode = hl.commitState() != 0)
        {
            return errorcode;
        }

        return errorcode;
    }

    int revertToStart(void);

    // NDMaterialLT *getCopy(void);
    // NDMaterialLT *getCopy(const char *code);
    // const char *getType(void) const;

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    int getObjectSize()
    {
        int size = 0;

        // 6 3x3 DTensor2s and 1 DTensor4 (3x3x3x3)
        size += (3 * 3 * 6 + 3 * 3 * 3 * 3) * sizeof(double);

        //Four pointers
        size += 4 * sizeof(YieldFunctionType*);

        //Whatever the base components size is
        size += yf->getObjectSize();
        size += et->getObjectSize();
        size += pf->getObjectSize();
        size += hl->getObjectSize();

        return size;
    }

private:


    int explicitStep(const DTensor2 &strain_incr)
    {
        int errorcode = 0;



        return errorcode;
    }



    int implicitStep(const DTensor2 &strain_incr);
    {
        int errorcode

        //Magic happens here!

        return errorcode;
    }




private:

    DTensor2 TrialStrain;
    DTensor2 TrialStress;
    DTensor2 TrialPlastic_Strain;

    DTensor2 CommitStress;
    DTensor2 CommitStrain;
    DTensor2 CommitPlastic_Strain;

    DTensor4 Stiffness;

    YieldFunctionType yf;
    ElasticityType    et;
    PlasticFlowType   pf;
    HardeningLawType  hl;

    static int constitutive_integration_method;

};

// int ClassicElastoplasticMaterial< class ElasticityType, class YieldFunctionType, , class PlasticFlowType, class HardeningLawType, int thisClassTag >::constitutive_integration_method = 0;



#endif
