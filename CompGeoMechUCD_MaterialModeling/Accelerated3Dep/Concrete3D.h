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
// PROGRAMMER:        Jose Abell, Nima Tafazzoli, Boris Jeremic
// DATE:              June 2013
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

#include <stresst.h>
#include <straint.h>
#include <BJtensor.h>
#include <BJmatrix.h>
#include <BJvector.h>
#include <Matrix.h>
#include <Vector.h>

#include <NDMaterial.h>

#include <G3Globals.h>
#include <iostream>
using namespace std;


#include <Channel.h>


class Concrete3D : public NDMaterial
{

    public:


        Concrete3D(int tag,
                   double E_in,
                   double v_in,
                   double k_in,
                   double rho_in,
                   double initialconfiningstress_in,
                   int maximum_number_of_iterations,
                   double intersection_stress_factor_tolerance,
                   double yield_function_tolerance);


        ~Concrete3D(void);

        const char* getClassType(void) const
        {
            return "Concrete3D";
        };


        // methods to set and retrieve state using the Tensor class
        int setTrialStrain(const Tensor& v);
        int setTrialStrainIncr(const Tensor& v);


        const BJtensor& getTangentTensor(void);
        const stresstensor& getStressTensor(void);
        const straintensor& getStrainTensor(void);
        const straintensor& getPlasticStrainTensor(void);

        const straintensor& PlasticFlowTensor(const stresstensor& Stre) const;
        const stresstensor& StressDerivative(const stresstensor& Stre) const;



        double YieldFunctionValue(const stresstensor& Stre) const;
        double getE();
        double getv();
        double getk();
        double getRho();
        double getInitialConfiningStress();


        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        NDMaterial* getCopy(void);
        NDMaterial* getCopy(const char* code);

        const char* getType(void) const;

        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

    private:

        int Explicit(const straintensor& strain_incr);

        stresstensor yield_surface_cross(const stresstensor& start_stress,
                                         const stresstensor& end_stress,
                                         double a);

        double zbrentstress(const stresstensor& start_stress,
                            const stresstensor& end_stress,
                            double x1, double x2, double tol) const;


    private:


        straintensor TrialStrain;
        stresstensor TrialStress;
        straintensor TrialPlastic_Strain;

        stresstensor CommitStress;
        straintensor CommitStrain;
        straintensor CommitPlastic_Strain;

        straintensor ElasticStateStrain;
        stresstensor ElasticStateStress;

        BJtensor Stiffness;

        // Material parameters
        //double E;
        //double v;
        //double rho;
        //double initialconfiningstress;

        // Parameter var       Name                                                         Used in                                     Units
        double K;           // Bulk Modulus                                                 Elasticity                                  [F ] / [L]^2
        double mu;          // Shear modulus                                                Elasticity                                  [F ] / [L]^2
        double alpha;       //                                                              Shear failure surface Ff                    [F ] / [L]2
        double lambda;      //                                                              Shear failure surface Ff                    [F ] / [L]^2
        double beta;        //                                                              Shear failure surface Ff                    [F ] / [L]^2
        double theta;       //                                                              Shear failure surface Ff                         –
        double kappa0;      // Initial cap position                                         Compression failure surface Fc              [F ] / [L]^2
        double R;           // Cap elipticity ratio                                         Compression failure surface Fc                   –
        double alpha1;      //                                                              Rubin scaling function                      [F ] / [L]
        double lambda1;     //                                                              Rubin scaling function                      [F ] / [L]^2
        double beta1;       //                                                              Rubin scaling function                      [F ] / [L]^2
        double theta1;      //                                                              Rubin scaling function                           –
        double alpha2;      //                                                              Rubin scaling function                      [F ] / [L]^2
        double lambda2;     //                                                              Rubin scaling function                      [F ] / [L]^2
        double beta2;       //                                                              Rubin scaling function                      [F ] / [L]
        double theta2;      //                                                              Rubin scaling function                           –
        double W;           // Maximum plastic volume strain                                Cap hardening rule                               –
        double D1;          // Cap linear hardening parameter                               Cap hardening rule                          [L]^2 / [F ]
        double D2;          // Cap quadratic hardening parameter                            Cap hardening rule                          [L]^4 / [F ]^2
        double X0;          // Location of cap when κ = κ0                                  Cap hardening rule                          [F ] / [L]^2
        double NH;          // Reduction of shear failure surface                           Kinematic hardening                              –
        double CH;          // Kin. hardening in unconfined compresion                      Kinematic hardening                              –
        double rob;         // Brittle damage threshold                                     Damage model                          sqrt( [F ] / [L]^2)
        double rod;         // Ductile damage threshold                                     Damage  model                          sqrt([F ] / [L]^2)
        double dmax;        // Maximum damage                                               Damage  model                                    –
        double A;           //                                                              Damage  model  curve                       [L]^3 / [F ]
        double B;           //                                                              Damage  model  curve                             –
        double C;           //                                                              Damage  model  curve                       [L]^3 / [F ]
        double D;           //                                                              Damage  model  curve                             –
        double Gf_t;        // Fracture energy in tension                                   Damage  model                               [F ] / [L]
        double Gf_s;        // Fracture energy in shear                                     Damage  model                               [F ] / [L]
        double Gf_c;        // Fracture energy in compr.                                    Damage  model                               [F ] / [L]
        double L;           // Element length                                               Damage  model                               [L]
        double pow;         // Introduce rate effects in fracture energy                    Damage  model                                    –
        double eta_ot;      // Fluidity parameter in tension at  epsilon_doṫ = 1.0            Rate effects model                               –
        double eta_oc;      // Fluidity parameter in compression at  epsilon_doṫ = 1.0        Rate effects model                               –
        double Srate;       // Reduction of tension fluidity for shear                      Rate effects model                               –
        double pwrc;        // Exponent controlling change of η with I1 in  compression     Rate effects                                    –
        double pwrt;        // Exponent controlling change of η with I1 in tension          Rate effects                                    –
        double overt;       // Viscoplastic stress limit in tension                                                                          –
        double Gf;          // Fracture energy                                              Damage model                                [F ] / [L]
        double rho;         // Density of concrete                                                                                      [M ] / [L]^3


        double kappa;       // Cap position  (see kappa0)


        static const  straintensor ZeroStrain;
        static const  stresstensor ZeroStress;
        double TOLERANCE1;
        double TOLERANCE2;
        int ISMAX;
        static BJtensor Ee;

};


