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
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              January 2010
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

// ElasticIsotropic3DLT is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.

#ifndef ELASTICISOTROPIC3dLT_HH
#define ELASTICISOTROPIC3dLT_HH

//#include <LTensor.h>
#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>
using namespace std;

class ElasticIsotropic3DLT : public NDMaterialLT
{

    public:


        ElasticIsotropic3DLT( int tag,
                              double E_in,
                              double v_in,
                              double rho_in );

        ElasticIsotropic3DLT();

        ~ElasticIsotropic3DLT( void );

        const char *getClassType( void ) const
        {
            return "ElasticIsotropic3DLT";
        };


        // methods to set and retrieve state using the Tensor class
        int setTrialStrain( const DTensor2 &v );         // Used
        int setTrialStrainIncr( const DTensor2 &v );     // Not used (used by setTrialStrain)


        const DTensor4 &getTangentTensor( void );      // Used by element
        const DTensor2 &getStressTensor( void );   // Used
        const DTensor2 &getStrainTensor( void );    // Used
        const DTensor2 &getPlasticStrainTensor( void ); // Not used

        // Need these to make material usable by elements
        //const BJtensor& getTangentTensor( void );      // Used by element
        //const stresstensor& getStressTensor( void );   // Used
        //const straintensor& getStrainTensor( void );    // Used
        //const straintensor& getPlasticStrainTensor( void ); // Not used


        double YieldFunctionValue( const DTensor2 &Stre ) const; // Material calls
        double getE(); // may be used by get copy
        double getv(); // may be used by get copy
        double getRho(); // may be used by get copy


        int commitState( void );             // called by element
        int revertToLastCommit( void );         // called by element
        int revertToStart( void );      // called by element

        NDMaterialLT *getCopy( void ); //might be used in parallel Im not sure about that
        NDMaterialLT *getCopy( const char *code ); //probably not needed

        const char *getType( void ) const; // probably not needed

        int sendSelf( int commitTag, Channel &theChannel );
        int receiveSelf( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker );

        void Print( ostream &s, int flag = 0 ); // used



    private:


        DTensor2 TrialStrain;
        DTensor2 TrialStress;

        DTensor2 ElasticStateStrain;
        DTensor2 ElasticStateStress;

        DTensor2 CommitStress;
        DTensor2 CommitStrain;

        // DTensor4 Stiffness;


        double E;
        double v;
        double rho;



        static const  DTensor2 ZeroStrain;
        static const  DTensor2 ZeroStress;

        DTensor4 Ee;

        Index < 'i' > i;
        Index < 'j' > j;
        Index < 'k' > k;
        Index < 'l' > l;

};


#endif
