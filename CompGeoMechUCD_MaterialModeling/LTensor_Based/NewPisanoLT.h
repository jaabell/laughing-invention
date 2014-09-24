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
// PROGRAMMER:        Jose Abell, Federico Pisano', Nima Tafazzoli, Boris Jeremic
// DATE:              December 2013
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

// PisanoLT is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.


#ifndef NewPisanoLT_H
#define NewPisanoLT_H


#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>


#define PISANO_FACTOR 0.9


using namespace std;


class NewPisanoLT : public NDMaterialLT
{

    public:

        NewPisanoLT( int tag,
                     double E0_in,  // Young modulus
                     double v_in,  // Poisson's ratio
                     double M_in,  // strength parameter
                     double kd_in, // flow rule parameter
                     double xi_in, // flow rule parameter
                     double h_in,  // hardening parameter
                     double m_in,  // hardening parameter
                     double rho_in,
                     double initialconfiningstress_in,
                     double n_in,
                     double a_in,
                     double eplcum_cr_in);

        NewPisanoLT(void);

        ~NewPisanoLT(void);

        const char *getClassType(void) const
        {
            return "NewPisanoLT";
        };



        // methods to set and retrieve state using the Tensor class
        int setTrialStrain( const DTensor2 &v );         // Used
        int setTrialStrainIncr( const DTensor2 &v );     // Not used (used by setTrialStrain)


        virtual const DTensor4 &getTangentTensor( void );      // Used by element
        virtual const DTensor2 &getStressTensor( void );   // Used
        virtual const DTensor2 &getStrainTensor( void );    // Used
        virtual const DTensor2 &getPlasticStrainTensor( void ); // Not used

        double getE0();
        double getv();
        double getM();
        double getkd();
        double getxi();
        double geth();
        double getm();
        double getRho();
        double getbeta_min();
        DTensor2  &getInternalTensor();
        double getInitialConfiningStress();
        double getn();
        double geta();
        double geteplcum_cr();

        double getE();

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        NDMaterialLT *getCopy(void);
        NDMaterialLT *getCopy(const char *code);

        const char *getType(void) const;

        int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

        void Print(ostream &s, int flag = 0);


        //To check internal variables (not part of ESSI interface)

        double   get_eplcum() const;
        DTensor2 get_strainplcum() const;
        DTensor2 get_alpha() const;
        DTensor2 get_alpha0() const;
        DTensor2 get_alpha0mem() const;
        DTensor2 get_strainpl0() const;
        DTensor2 get_Stress_n_minus_2() const;
        DTensor2 get_nij_dev() const;


    private:


        int Explicit(const DTensor2 &strain_incr);

        double get_distance_coeff(DTensor2 &start_stress);
        double get_distance_coeff_lode(DTensor2 &start_stress);
        double get_dilatancy();
        double get_lode_angle(DTensor2   s); // computes the Lode angle of a stress state





    private:
        //Model parameters
        double beta;                // Scalar distance coefficient as internal variable
        double E0;                  // Young's modulus at p = patm
        double v;                   // Modulus of poisson
        double M;                   // Critical stress ratio in triax compression
        double kd;                  // Dilatancy parameter (controls the transition from dilative to compactive behavior)
        double xi;                  // Dilatancy parameter(controls the ammount of volumetric plastic strain developed)
        double h;                   // Hardening parameter
        double m;                   // Hardening parameter
        double rho;                 //
        double initialconfiningstress;
        double n;                   // Controls shape of boundary surface in deviatoric plane (n=-0.229 matches Mohr-Coulomb deviatoric section)
        double a;                   // Exponent for pressure dependent elastic moduli
        double eplcum_cr;           // Critical cumulated plastic strain upon stress reversal (for overshooting) // Should be always between 1e-5 and 1e-6 (cfr. Mojtaba E-Kan, Hossein A. Taiebat, On implementation of bounding surface plasticity models with no overshooting effect in solving boundary value problems, Computers and Geotechnics, Volume 55, January 2014, Pages 103-116, ISSN 0266-352X, http://dx.doi.org/10.1016/j.compgeo.2013.08.006.(http://www.sciencedirect.com/science/article/pii/S0266352X13001213))

        //Internal variables
        double eplcum;
        DTensor2 strainplcum;         //Cumulated plastic strain upon stress reversal (for overshooting)

        DTensor2 TrialStrain;
        DTensor2 TrialStress;
        DTensor2 TrialPlastic_Strain;

        DTensor2 ElasticStateStrain;
        DTensor2 ElasticStateStress;

        DTensor2 CommitStress;
        DTensor2 CommitStrain;
        DTensor2 CommitPlastic_Strain;

        DTensor2 alpha;             // back stress ratio tensor
        DTensor2 alpha0;            // back stress ratio tensor
        DTensor2 alpha0mem;         // back stress ratio tensor at last previous stress reversal
        DTensor2 strainpl0;         // Plastic strain at stress reversal (for overshooting)
        DTensor2 Stress_n_minus_2;  // Stress tensor at step n-2
        DTensor2 nij_dev;           // direction of deviatoric plastic strain increment(unit tensor)


        //Fourth order stiffness
        DTensor4 Stiffness;

        //Global constants
        const static double beta_min;
        const static double beta_max;
        const static double patm;                // In basic SI units (Pa = kg / s^-2 / m)

        static const  DTensor2 ZeroStrain;
        static const  DTensor2 ZeroStress;
        static const double check_for_zero;
        DTensor4 Ee;

        static DTensor2 kronecker_delta;

        //Global indexes for LTensor
        Index < 'i' > i;
        Index < 'j' > j;


};

#endif
