//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:
//# CLASS:             TwoStageEquivalentElasticIsotropic3D
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            J. Abell
//# PROGRAMMER(S):     J. Abell
//#
//#
//# DATE:              July 2013
//# UPDATE HISTORY:
//#
//#
//===============================================================================


#ifndef TwoStageEquivalentElasticIsotropic3D_h
#define TwoStageEquivalentElasticIsotropic3D_h

#include <ElasticIsotropicMaterial.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

#include <straint.h>
#include <stresst.h>
#include <Tensor.h>

#include <Channel.h>

class TwoStageEquivalentElasticIsotropic3D : public ElasticIsotropicMaterial
{
    public:
        TwoStageEquivalentElasticIsotropic3D (int tag, double nu, double rho,
                                              double K, double Kur, double n, double c,
                                              double phi0, double dphi, double Rf, double K0,
                                              double Kb, double m, double pa);
        TwoStageEquivalentElasticIsotropic3D ();
        ~TwoStageEquivalentElasticIsotropic3D ();

        int setTrialStrain (const Vector& v);
        int setTrialStrain (const Vector& v, const Vector& r);
        int setTrialStrainIncr (const Vector& v);
        int setTrialStrainIncr (const Vector& v, const Vector& r);
        const Matrix& getTangent (void);
        const Matrix& getInitialTangent (void);

        const Vector& getStress (void);
        const Vector& getStrain (void);

        int setTrialStrain (const Tensor& v);
        int setTrialStrain (const Tensor& v, const Tensor& r);
        int setTrialStrainIncr (const Tensor& v);
        int setTrialStrainIncr (const Tensor& v, const Tensor& r);
        const Tensor& getTangentTensor (void);
        const stresstensor& getStressTensor (void);
        const straintensor& getStrainTensor (void);
        const straintensor& getPlasticStrainTensor (void);

        int commitState (void);
        int revertToLastCommit (void);
        int revertToStart (void);

        NDMaterial* getCopy (void);
        const char* getType (void) const;
        int getOrder (void) const;

        void Print(ostream& s, int flag = 0);
        void setInitElasticStiffness(void);


        //for PDD
        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int receiveSelf(int commitTag, Channel& theChannel,
                             FEM_ObjectBroker& theBroker);
        int getObjectSize();



        //For this material only
        void nextStage();
        void updateModulus();


    protected:

    private:
        static Vector sigma;        // Stress vector
        static Matrix D;            // Elastic constantsuVector sigma;
        Vector epsilon;     // Strain vector

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

        double G_over_Gmax; // Fraction of G to be used in current dynamic analysis.
        double xsi;  // Damping coefficient in current step

        int stage;   // Number of current stage

        stresstensor Stress;        // Stress tensor
        Tensor Dt;                  // Elastic constants tensor
        straintensor Strain;        // Strain tensor

        // State of material at end of static stage
        stresstensor Stress_st;     // Stress tensor
        Tensor Dt_st;               // Elastic constants tensor
        straintensor Strain_st;     // Strain tensor

        double sig1, sig3, sig1p, sig3p;        // State at previous step, whatever it was
        double gamma_max;           // Not 65% of the strain
        double strlev_max;          // Maximum stress level
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


