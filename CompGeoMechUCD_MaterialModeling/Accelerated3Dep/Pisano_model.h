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
// PROGRAMMER:        Federico Pisano', Nima Tafazzoli, Boris Jeremic
// DATE:              October 2012
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

// Pisano_model is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.


#ifndef Pisano_model_H
#define Pisano_model_H

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
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
using namespace std;

#include <Channel.h>



class Pisano_model : public NDMaterial
{

    public:

        Pisano_model( int tag,
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

        Pisano_model(void);

        ~Pisano_model(void);

        const char* getClassType(void) const
        {
            return "Pisano_model";
        };



        // methods to set and retrieve state using the Tensor class
        int setTrialStrain(const Tensor& v);
        int setTrialStrainIncr(const Tensor& v);

        const BJtensor& getTangentTensor(void);
        const stresstensor& getStressTensor(void);
        const straintensor& getStrainTensor(void);
        const straintensor& getPlasticStrainTensor(void);

        double getE();
        double getv();
        double getM();
        double getkd();
        double getxi();
        double geth();
        double getm();
        double getRho();
        double getbeta_min();
        stresstensor&  getInternalTensor();
        double getInitialConfiningStress();

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        NDMaterial* getCopy(void);
        NDMaterial* getCopy(const char* code);

        const char* getType(void) const;

        //Guanzhou
        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

        const Matrix& getTangent (void);


    private:


        int Explicit(const straintensor& strain_incr);


        //  double zbrentstress(const stresstensor& start_stress,
        //                      const stresstensor& end_stress,
        //                      double x1, double x2, double tol) const;

        double get_distance_coeff(stresstensor& start_stress);




    private:

        straintensor TrialStrain;
        stresstensor TrialStress;
        straintensor TrialPlastic_Strain;


        straintensor ElasticStateStrain;
        stresstensor ElasticStateStress;

        stresstensor CommitStress;
        straintensor CommitStrain;
        straintensor CommitPlastic_Strain;


        BJtensor     Stiffness;

        // new variables needed for Pisano's model
        stresstensor alpha; // back stress ratio tensor
        double       beta0; // to be determined after checking the loading/unloading condition
        double       beta; // scalar distance coefficient as internal variable
        stresstensor Stress_n_minus_2; // Stress tensor at step n-2
        stresstensor nij_dev; // direction of deviatoric plastic strain increment(unit tensor)
        stresstensor nij_dev_prev;

        static const  straintensor ZeroStrain;
        static const  stresstensor ZeroStress;
        double beta_min;
        static const double check_for_zero;
        static BJtensor Ee;

        double E;
        double v;
        double M;
        double kd;
        double xi;
        double h;
        double m;
        double rho;
        double initialconfiningstress;

        static Matrix D;


};

#endif
