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


#ifndef PisanoLT_H
#define PisanoLT_H


#include "NDMaterialLT.h"
#include <G3Globals.h>
#include <iostream>
#include "../../ltensor/LTensor.h"
#include <Channel.h>


using namespace std;


class PisanoLT : public NDMaterialLT
{

public:

    PisanoLT( int tag,
              double E_in,  // Young modulus
              double v_in,  // Poisson's ratio
              double M_in,  // strength parameter
              double kd_in, // flow rule parameter
              double xi_in, // flow rule parameter
              double h_in,  // hardening parameter
              double m_in,  // hardening parameter
              double rho_in,
              double initialconfiningstress_in,
              double beta_min_in);

    PisanoLT(void);

    ~PisanoLT(void);

    const char *getClassType(void) const
    {
        return "PisanoLT";
    };



    // methods to set and retrieve state using the Tensor class
    int setTrialStrain( const DTensor2 &v );         // Used
    int setTrialStrainIncr( const DTensor2 &v );     // Not used (used by setTrialStrain)


    const DTensor4 &getTangentTensor( void );      // Used by element
    const DTensor2 &getStressTensor( void );   // Used
    const DTensor2 &getStrainTensor( void );    // Used
    const DTensor2 &getPlasticStrainTensor( void ); // Not used

    double getE();
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

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    NDMaterialLT *getCopy(void);
    NDMaterialLT *getCopy(const char *code);

    const char *getType(void) const;

    // int describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel);
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(ostream &s, int flag = 0);

    //const Matrix &getTangent (void);


private:


    int Explicit(const DTensor2 &strain_incr);


    //  double zbrentstress(const stresstensor& start_stress,
    //                      const stresstensor& end_stress,
    //                      double x1, double x2, double tol) const;

    double get_distance_coeff(DTensor2 &start_stress);




private:

    DTensor2 TrialStrain;
    DTensor2 TrialStress;
    DTensor2 TrialPlastic_Strain;


    DTensor2 ElasticStateStrain;
    DTensor2 ElasticStateStress;

    DTensor2 CommitStress;
    DTensor2 CommitStrain;
    DTensor2 CommitPlastic_Strain;

    DTensor2 alpha;             // back stress ratio tensor
    DTensor2 Stress_n_minus_2;  // Stress tensor at step n-2
    DTensor2 nij_dev;           // direction of deviatoric plastic strain increment(unit tensor)
    DTensor2 nij_dev_prev;
    DTensor4 Stiffness;

    double beta0;               // to be determined after checking the loading/unloading condition
    double beta;                // scalar distance coefficient as internal variable
    double beta_min;
    double E;
    double v;
    double M;
    double kd;
    double xi;
    double h;
    double m;
    double rho;
    double initialconfiningstress;

    static const  DTensor2 ZeroStrain;
    static const  DTensor2 ZeroStress;
    static const double check_for_zero;
    static DTensor4 Ee;
    //static Matrix D;

    Index < 'i' > i;
    Index < 'j' > j;
};

#endif
