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
// DATE:              July 2013
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

// vonMisesLinearKH is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.


#ifndef vonMisesLinearKH_H
#define vonMisesLinearKH_H

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


class vonMisesLinearKH : public NDMaterial
{

    public:

        vonMisesLinearKH( int tag,
                          double E_in,
                          double v_in,
                          double k_in,
                          double H_in,
                          double rho_in,
                          double initialconfiningstress_in,
                          int maximum_number_of_iterations,
                          double intersection_stress_factor_tolerance,
                          double yield_function_tolerance);


        vonMisesLinearKH(void);

        ~vonMisesLinearKH(void);

        const char* getClassType(void) const
        {
            return "vonMisesLinearKH";
        };


        // methods to set and retrieve state using the Tensor class
        int setTrialStrain(const Tensor& v);
        int setTrialStrainIncr(const Tensor& v);

        const BJtensor&     getTangentTensor(void);
        const stresstensor& getStressTensor(void);
        const straintensor& getStrainTensor(void);
        const straintensor& getPlasticStrainTensor(void);

        double getE();
        double getv();
        double getk();
        double getH();
        double getRho();
        stresstensor& getInternalTensor();
        double getInitialConfiningStress();

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        NDMaterial* getCopy(void);
        NDMaterial* getCopy(const char* code);
        const char* getType(void) const;


        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

        double YieldFunctionValue( const stresstensor& Stre) const;


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

        stresstensor Internal_Tensor;


        straintensor ElasticStateStrain;
        stresstensor ElasticStateStress;

        stresstensor CommitStress;
        straintensor CommitStrain;
        straintensor CommitPlastic_Strain;

        BJtensor Stiffness;

        double E;
        double v;
        double k;
        double H;
        double rho;
        double initialconfiningstress;

        static const  straintensor ZeroStrain;
        static const  stresstensor ZeroStress;
        double TOLERANCE1;
        double TOLERANCE2;
        int ISMAX;
        static BJtensor Ee;


};

#endif
