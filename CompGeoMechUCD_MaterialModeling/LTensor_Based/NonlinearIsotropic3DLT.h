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

// NonlinearIsotropic3DLT is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.

#ifndef NonlinearIsotropic3DLT_HH
#define NonlinearIsotropic3DLT_HH

//#include <LTensor.h>
#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>
using namespace std;

class NonlinearIsotropic3DLT : public NDMaterialLT
{

public:


    NonlinearIsotropic3DLT(int tag,
                           double rho_, double K_,
                           double Kur_, double n_, double c_, double phi0_, double dphi_,
                           double Rf_, double K0_, double Kb_, double m_, double pa_,
                           double K2_, double B_, double Et_, double Ei_, double Er_);

    NonlinearIsotropic3DLT();

    ~NonlinearIsotropic3DLT( void );

    const char *getClassType( void ) const
    {
        return "NonlinearIsotropic3DLT";
    };


    // methods to set and retrieve state using the Tensor class
    int setTrialStrain( const DTensor2 &v );
    int setTrialStrainIncr( const DTensor2 &v );


    const DTensor4 &getTangentTensor( void );
    const DTensor2 &getStressTensor( void );
    const DTensor2 &getStrainTensor( void );
    const DTensor2 &getPlasticStrainTensor( void );


    double YieldFunctionValue( const DTensor2 &Stre ) const; // Material calls
    double getE();
    double getv();
    double getRho();


    int commitState( void );
    int revertToLastCommit( void );
    int revertToStart( void );

    NDMaterialLT *getCopy( void );

    const char *getType( void ) const; // probably not needed

    int sendSelf( int commitTag, Channel &theChannel );
    int receiveSelf( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker );

    void Print( ostream &s, int flag = 0 ); // used

    int getObjectSize();

    void compute_tangent_tensor(void);

    void updateModulus();

    void zeroStrain();


private:

    DTensor2 TrialStrain;
    DTensor2 TrialStress;

    DTensor2 CommitStress;
    DTensor2 CommitStrain;

    double rho;

    // Parameters for equivalent linear (static) model
    double K;    // For initial elasticity modulus
    double Kur;  // Reloading elasticity modulus
    double n;    // Relates confinement with elastic moduli
    double c;    // Cohesion
    double phi0; // Initial friction angle
    double dphi; // Dariation of friction angle with confinement
    double Rf;   // Relates ultimate deviator stress with Mohr-Coulomb rupture criteria
    double K0;   // Coefficient of earth pressure at rest
    double Kb;   // Relates bulk modulus with confinement
    double m;    // Idem
    double pa;   // Atmospheric pressure

    double K2;   // Relates to dynamic equivalent stiffness

    double B;    // Bulk modulus
    double Et;   // Tangent elastic modulus
    double Ei;   // Initial elastic modulus
    double Er;   // Unload-Reload elastic modulus

    double E, nu; //Computed from above and current stress state

    double strlev_max;

    static const  DTensor2 ZeroStrain;
    static const  DTensor2 ZeroStress;

    static DTensor4 Ee;

    bool initialize;

    Index < 'i' > i;
    Index < 'j' > j;
    Index < 'k' > k;
    Index < 'l' > l;
};

//Definir funciones para calcular rigideces y amortiguamientos
// ---------------------------------------------------------------

double Ei_calc (double K, double pa, double n, double sig3);
double Et_calc (double K, double pa, double n, double Rf, double c, double phi, double desv, double sig3);
double Eur_calc (double Kur, double pa, double n, double sig3);
double phi_calc (double phi0, double Dphi, double pa, double sig3);
double B_calc (double Kb, double pa, double n, double sig3);
double nui_calc (double G, double F, double pa, double sig3);



#endif
