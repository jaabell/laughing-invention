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
// DATE:              March 2010
// UPDATE HISTORY:
//
/////////////////////////////////////////////////////////////////////////////

// CamClay is another form of NewTemplate3Dep which has all
// the functions embeded here for increasing the efficiency and speed.

#include "CamClay.h"


const  straintensor CamClay::ZeroStrain;
const  stresstensor CamClay::ZeroStress;
BJtensor CamClay::Ee(4, def_dim_4, 0.0);

//================================================================================
CamClay::CamClay(int tag,
                 double e0_in,
                 double M_in,
                 double lambda_in,
                 double kappa_in,
                 double v_in,
                 double Kc_in,
                 double p0_in,
                 double initial_confining_stress,
                 double rho_in,
                 int maximum_number_of_iterations,
                 double tolerance_1,
                 double tolerance_2)
    : NDMaterial(tag, ND_TAG_CamClay),
      e0(e0_in),
      M(M_in),
      lambda(lambda_in),
      kappa(kappa_in),
      v(v_in),
      Kc(Kc_in),
      p0(p0_in),
      rho(rho_in),
      TOLERANCE1(tolerance_1), TOLERANCE2(tolerance_2),
      ISMAX(maximum_number_of_iterations),
      initial_stress(initial_confining_stress)
{

    stresstensor initialStress;
    initialStress.val(1, 1) = initial_stress;
    initialStress.val(2, 2) = initial_stress;
    initialStress.val(3, 3) = initial_stress;


    ElasticStateStress.Initialize(initialStress);

    this->revertToStart();
}


//================================================================================
CamClay::~CamClay()
{
}

//================================================================================
int CamClay::setTrialStrain(const Tensor& v)
{

    return this->setTrialStrainIncr( v - getStrainTensor() );
}

//================================================================================
int CamClay::setTrialStrainIncr(const Tensor& strain_increment)
{
    return this->Explicit(strain_increment);
}

//================================================================================
const BJtensor& CamClay::getTangentTensor(void)
{
    return this->Stiffness;
}

//================================================================================
const stresstensor&  CamClay::getStressTensor(void)
{
    return this->TrialStress;
}

//================================================================================
const straintensor& CamClay::getStrainTensor(void)
{
    return this->TrialStrain;
}

//================================================================================
const straintensor& CamClay::getPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}

//================================================================================
int CamClay::commitState(void)
{
    CommitStress.Initialize(TrialStress);
    CommitStrain.Initialize(TrialStrain);
    CommitPlastic_Strain.Initialize(TrialPlastic_Strain);

    return 0;
}

//================================================================================
int CamClay::revertToLastCommit(void)
{
    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(CommitPlastic_Strain);

    return 0;
}

//================================================================================
int CamClay::revertToStart(void)
{
    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain.Initialize(ZeroStrain);

    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(ZeroStrain);

    Ee = getElasticStiffness();

    return 0;
}

//================================================================================
NDMaterial* CamClay::getCopy(void)
{
    NDMaterial* tmp = new CamClay(this->getTag(),
                                  this->gete0(),
                                  this->getM(),
                                  this->getlambda(),
                                  this->getkappa(),
                                  this->getv(),
                                  this->getKc(),
                                  this->getp0(),
                                  this->getInitialConfiningStress(),
                                  this->getRho(),
                                  this->ISMAX,
                                  this->TOLERANCE1,
                                  this->TOLERANCE2);

    return tmp;
}


//================================================================================
NDMaterial* CamClay::getCopy(const char* code)
{
    if (strcmp(code, "ThreeDimensional") == 0)
    {
        CamClay* tmp = new CamClay( this->getTag(),
                                    this->gete0(),
                                    this->getM(),
                                    this->getlambda(),
                                    this->getkappa(),
                                    this->getv(),
                                    this->getKc(),
                                    this->getp0(),
                                    this->getInitialConfiningStress(),
                                    this->getRho(),
                                    this->ISMAX,
                                    this->TOLERANCE1,
                                    this->TOLERANCE2);

        return tmp;
    }
    else
    {
        cout << "NewTemplate3Dep::getCopy failed to get model: " <<  code << endln;
        exit(1);
    }

    return 0;
}

//================================================================================
const char* CamClay::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
int CamClay::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int CamClay::recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}

//================================================================================
void CamClay::Print(ostream& s, int flag)
{
    s << (*this);
}

//================================================================================
double CamClay::gete0()
{
    return e0;
}

//================================================================================
double CamClay::getM()
{
    return M;
}

//================================================================================
double CamClay::getv()
{
    return v;
}

//================================================================================
double CamClay::getlambda()
{
    return lambda;
}

//================================================================================
double CamClay::getkappa()
{
    return kappa;
}

//================================================================================
double CamClay::getKc()
{
    return Kc;
}

//================================================================================
double CamClay::getRho(void)
{
    return rho;
}

//================================================================================
double CamClay::getp0()
{
    return p0;
}

//================================================================================
double CamClay::getInitialConfiningStress(void)
{
    return initial_stress;
}

//================================================================================
double CamClay::YieldFunctionValue( const stresstensor& Stre) const
{

    double p = Stre.p_hydrostatic();
    double q = Stre.q_deviatoric();

    return q * q - M * M * p * (p0 - p);
}


//================================================================================
const BJtensor& CamClay::getElasticStiffness()
{
    BJtensor I_ijkl = I_ijkl.I_1234();
    BJtensor I_1324plusI_1423 = I_1324plusI_1423.I_1324plusI_1423();


    double p = ElasticStateStress.p_hydrostatic();

    double epsilon_v = ElasticStateStrain.Iinvariant1();

    double e = e0 + (1 + e0) * epsilon_v;
    double Kk = (1.0 + e) * p / kappa;

    double K = (Kk > Kc) ? Kk : Kc ;
    double G = K * 1.5 * (1.0 - 2.0 * v) / (1.0 + v);

    // To avoid numerical problems
    //if (G < (1.0e-3) *K)  G = 1.0e-3 *K;


    // Building elasticity tensor
    Ee = I_ijkl * (K - 2.0 * G / 3.0) + I_1324plusI_1423 * G;

    return Ee;
}

//================================================================================
int CamClay::Explicit(const straintensor& strain_incr)
{

    int err = 0;
    double f_start = 0.0;
    double f_pred  = 0.0;

    BJtensor Ee(4, def_dim_4, 0.0);
    BJtensor Ep(4, def_dim_4, 0.0);

    straintensor intersection_strain;
    stresstensor intersection_stress;
    double intersection_factor = 0.0;


    double nominator = 0.0;
    double denominator = 0.0;
    double Delta_lambda = 0.0;
    double h_s = 0.0;
    double xi_s = 0.0;
    stresstensor dFods;
    BJtensor Hq(2, def_dim_2, 0.0);
    BJtensor Hf(2, def_dim_2, 0.0);


    stresstensor h_t;
    stresstensor xi_t;

    straintensor incr_strain;
    stresstensor incr_stress;
    straintensor incr_Pstrain;
    stresstensor ep_stress;
    stresstensor predicted_stress;

    straintensor start_stress;
    stresstensor start_strain;
    straintensor start_Pstrain;

    start_stress = getStressTensor();
    start_strain = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();


    intersection_stress.Initialize(start_stress);

    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;
    Ee = getElasticStiffness();

    incr_strain = strain_incr;

    /////////////////////////////////////////////////////////////////////////
    //     incr_stress = Ee("ijpq") * incr_strain("pq");
    //     incr_stress.null_indices();

    incr_stress.val(1, 1) = Ee.cval(1, 1, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(1, 1, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(1, 1, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(1, 1, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(1, 1, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(1, 1, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(1, 1, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(1, 1, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(1, 1, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(1, 2) = Ee.cval(1, 2, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(1, 2, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(1, 2, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(1, 2, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(1, 2, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(1, 2, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(1, 2, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(1, 2, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(1, 2, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(1, 3) = Ee.cval(1, 3, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(1, 3, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(1, 3, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(1, 3, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(1, 3, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(1, 3, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(1, 3, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(1, 3, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(1, 3, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(2, 1) = Ee.cval(2, 1, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(2, 1, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(2, 1, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(2, 1, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(2, 1, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(2, 1, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(2, 1, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(2, 1, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(2, 1, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(2, 2) = Ee.cval(2, 2, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(2, 2, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(2, 2, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(2, 2, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(2, 2, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(2, 2, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(2, 2, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(2, 2, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(2, 2, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(2, 3) = Ee.cval(2, 3, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(2, 3, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(2, 3, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(2, 3, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(2, 3, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(2, 3, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(2, 3, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(2, 3, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(2, 3, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(3, 1) = Ee.cval(3, 1, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(3, 1, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(3, 1, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(3, 1, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(3, 1, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(3, 1, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(3, 1, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(3, 1, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(3, 1, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(3, 2) = Ee.cval(3, 2, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(3, 2, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(3, 2, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(3, 2, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(3, 2, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(3, 2, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(3, 2, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(3, 2, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(3, 2, 3, 3) * incr_strain.cval(3, 3);


    incr_stress.val(3, 3) = Ee.cval(3, 3, 1, 1) * incr_strain.cval(1, 1) +
                            Ee.cval(3, 3, 1, 2) * incr_strain.cval(1, 2) +
                            Ee.cval(3, 3, 1, 3) * incr_strain.cval(1, 3) +
                            Ee.cval(3, 3, 2, 1) * incr_strain.cval(2, 1) +
                            Ee.cval(3, 3, 2, 2) * incr_strain.cval(2, 2) +
                            Ee.cval(3, 3, 2, 3) * incr_strain.cval(2, 3) +
                            Ee.cval(3, 3, 3, 1) * incr_strain.cval(3, 1) +
                            Ee.cval(3, 3, 3, 2) * incr_strain.cval(3, 2) +
                            Ee.cval(3, 3, 3, 3) * incr_strain.cval(3, 3);
    /////////////////////////////////////////////////////////////////////////

    incr_stress.null_indices();

    predicted_stress = start_stress + incr_stress;


    f_start = YieldFunctionValue(start_stress);
    f_pred =  YieldFunctionValue(predicted_stress);


    // If Elastic
    if ( (f_start < 0.0 && f_pred <= TOLERANCE2) || f_start > f_pred )
    {
        TrialStrain = start_strain + incr_strain;
        TrialStress.Initialize(predicted_stress);


        Stiffness = Ee;
    }
    else
    {
        // If Elastic and then Elastic-Plastic
        if ( f_start < 0.0 )
        {
            intersection_factor = zbrentstress( start_stress, predicted_stress, 0.0, 1.0, TOLERANCE1 );
            intersection_stress = yield_surface_cross( start_stress, predicted_stress, intersection_factor );
            intersection_strain = start_strain + (incr_strain * intersection_factor);


            incr_stress = predicted_stress - intersection_stress;


            ElasticStateStrain = intersection_strain;
            ElasticStateStress = intersection_stress;
            Ee = getElasticStiffness();
        }


        // If E-P Response,
        Delta_lambda = 0.0;



        ////////////////////////////////////////////////////////////////////
        tensor I2("I", 2, def_dim_2);
        stresstensor sij = intersection_stress.deviator();
        double p = intersection_stress.p_hydrostatic();

        double scalar1 = M * M * (p0 - 2.0 * p) * (1.0 / 3.0);

        //    dFods = (sij * 3.0) + (I2 * scalar1);
        dFods.val(1, 1) = sij.cval(1, 1) * 3 + I2.cval(1, 1) * scalar1;
        dFods.val(1, 2) = sij.cval(1, 2) * 3;
        dFods.val(1, 3) = sij.cval(1, 3) * 3;
        dFods.val(2, 1) = sij.cval(2, 1) * 3;
        dFods.val(2, 2) = sij.cval(2, 2) * 3 + I2.cval(2, 2) * scalar1;
        dFods.val(2, 3) = sij.cval(2, 3) * 3;
        dFods.val(3, 1) = sij.cval(3, 1) * 3;
        dFods.val(3, 2) = sij.cval(3, 2) * 3;
        dFods.val(3, 3) = sij.cval(3, 3) * 3 + I2.cval(3, 3) * scalar1;

        ////////////////////////////////////////////////////////////////////
        //       Hq = Ee("ijkl") * dFods("kl");
        double e = e0 + (1.0 + e0) * intersection_strain.Iinvariant1();

        double Kk = (1.0 + e) * p / kappa;

        double K = (Kk > Kc) ? Kk : Kc ;
        double G = K * 1.5 * (1.0 - 2.0 * v) / (1.0 + v);

        double A = K - G * (2.0 / 3.0);
        double B = M * M * (p0 - 2.0 * p) * (1.0 / 3.0);

        //    double sijI2 = sij.cval(1,1) + sij.cval(2,2) + sij.cval(3,3);

        //    double D = 3*A*sijI2 + 3*A*B + 2*B*G;
        double D = 3 * A * B + 2 * B * G;

        Hq = I2 * D + sij * (6 * G);

        Hq.null_indices();


        Hf = dFods("ij") * Ee("ijkl");
        Hf.null_indices();

        nominator = ( dFods("ij") * incr_stress("ij") ).trace();

        denominator = ( Hf("ij") * dFods("ij") ).trace();

        //     double sijsijtrace = sij.cval(1,1) * sij.cval(1,1) +
        //                          sij.cval(1,2) * sij.cval(1,2) +
        //                          sij.cval(1,3) * sij.cval(1,3) +
        //                          sij.cval(2,1) * sij.cval(2,1) +
        //                          sij.cval(2,2) * sij.cval(2,2) +
        //                          sij.cval(2,3) * sij.cval(2,3) +
        //                          sij.cval(3,1) * sij.cval(3,1) +
        //                          sij.cval(3,2) * sij.cval(3,2) +
        //                          sij.cval(3,3) * sij.cval(3,3);
        //
        // //      denominator = (3*D + 6*B*G)*sijI2 + 18*G*sijsijtrace + 3*B*D;
        //       denominator = 18*G*sijsijtrace + 3*B*D;

        ///////////////////////////////////////////////////////////////////////////////
        // Evolution of scalar (isotropic) internal variables in yield function

        //     double e = e0 + (1.0 + e0) *intersection_strain.Iinvariant1();
        double d_Ev = M * M * (2.0 * p - p0);

        h_s = (1.0 + e) * p0 * d_Ev / (lambda - kappa);
        xi_s = -M * M * p;

        denominator -= h_s * xi_s;

        if (denominator != 0)
        {
            Delta_lambda = nominator / denominator;
        }


        if (Delta_lambda < 0.0)
        {
            Delta_lambda = 0.0;
        }

        // Plastic strain increment
        //     incr_Pstrain = dFods * Delta_lambda;
        incr_Pstrain.val(1, 1) = sij.cval(1, 1) * (3.0 * Delta_lambda) + I2.cval(1, 1) * (scalar1 * Delta_lambda);
        incr_Pstrain.val(1, 2) = sij.cval(1, 2) * (3.0 * Delta_lambda);
        incr_Pstrain.val(1, 3) = sij.cval(1, 3) * (3.0 * Delta_lambda);
        incr_Pstrain.val(2, 1) = sij.cval(2, 1) * (3.0 * Delta_lambda);
        incr_Pstrain.val(2, 2) = sij.cval(2, 2) * (3.0 * Delta_lambda) + I2.cval(2, 2) * (scalar1 * Delta_lambda);
        incr_Pstrain.val(2, 3) = sij.cval(2, 3) * (3.0 * Delta_lambda);
        incr_Pstrain.val(3, 1) = sij.cval(3, 1) * (3.0 * Delta_lambda);
        incr_Pstrain.val(3, 2) = sij.cval(3, 2) * (3.0 * Delta_lambda);
        incr_Pstrain.val(3, 3) = sij.cval(3, 3) * (3.0 * Delta_lambda) + I2.cval(3, 3) * (scalar1 * Delta_lambda);



        ep_stress = predicted_stress - (Hq * Delta_lambda);

        TrialStress.Initialize(ep_stress);
        TrialStrain = start_strain + incr_strain;
        TrialPlastic_Strain = start_Pstrain + incr_Pstrain;

        // Update internal scalar variables
        double dp0 = h_s * Delta_lambda;

        p0 += dp0;


        // To obtain Eep, at the last step
        Ep = Hq("pq") * Hf("mn");
        Ep.null_indices();


        if (denominator != 0)
        {
            Ep = Ep * (1.0 / denominator);
        }



        if ( Delta_lambda > 0.0 )
        {
            Stiffness = Ee - Ep;
        }
        else
        {
            Stiffness = Ee;
        }

    }

    return err;
}



// Trying to find intersection point according to M. Crisfield's book
// "Non-linear Finite Element Analysis of Solids and Structures "  Chp 6.6.1 pp168.
//================================================================================
stresstensor CamClay::yield_surface_cross(const stresstensor& start_stress,
        const stresstensor& end_stress, double a)
{
    stresstensor delta_stress = end_stress - start_stress;
    stresstensor intersection_stress = start_stress + delta_stress * a;

    return intersection_stress;
}

// Routine used by yield_surface_cross to find the stresstensor at cross point
//================================================================================
double CamClay::zbrentstress(const stresstensor& start_stress,
                             const stresstensor& end_stress,
                             double x1, double x2, double tol) const
{
    double EPS = d_macheps();

    int iter;
    double a = x1;
    double b = x2;
    double c = 0.0;
    double d = 0.0;
    double e = 0.0;
    double min1 = 0.0;
    double min2 = 0.0;
    double fc = 0.0;
    double p = 0.0;
    double q = 0.0;
    double r = 0.0;
    double s = 0.0;
    double tol1 = 0.0;
    double xm = 0.0;

    stresstensor alfa_stress_1 = ( start_stress * (1.0 - a) ) + ( end_stress * a );
    stresstensor alfa_stress_2 = ( start_stress * (1.0 - b) ) + ( end_stress * b );

    double fa = YieldFunctionValue(alfa_stress_1);
    double fb = YieldFunctionValue(alfa_stress_2);

    if ( (fb * fa) > 0.0)
    {
        cout << "\a\n Root must be bracketed in ZBRENTstress " << endln;
        exit(1);
    }

    fc = fb;

    for ( iter = 1; iter <= ISMAX; iter++ )
    {
        if ( (fb * fc) > 0.0)
        {
            c = a;
            fc = fa;
            e = d = b - a;
        }

        if ( fabs(fc) < fabs(fb) )
        {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }

        tol1 = 2.0 * EPS * fabs(b) + 0.5 * tol;
        xm = 0.5 * (c - b);

        if ( fabs(xm) <= tol1 || fb == 0.0 )
        {
            return b;
        }

        if ( fabs(e) >= tol1 && fabs(fa) > fabs(fb) )
        {
            s = fb / fa;

            if (a == c)
            {
                p = 2.0 * xm * s;
                q = 1.0 - s;
            }
            else
            {
                q = fa / fc;
                r = fb / fc;
                p = s * ( 2.0 * xm * q * (q - r) - (b - a) * (r - 1.0) );
                q = (q - 1.0) * (r - 1.0) * (s - 1.0);
            }

            if (p > 0.0)
            {
                q = -q;
            }

            p = fabs(p);
            min1 = 3.0 * xm * q - fabs(tol1 * q);
            min2 = fabs(e * q);

            if (2.0 * p < (min1 < min2 ? min1 : min2))
            {
                e = d;
                d = p / q;
            }
            else
            {
                d = xm;
                e = d;
            }
        }
        else
        {
            d = xm;
            e = d;
        }

        a = b;
        fa = fb;

        if (fabs(d) > tol1)
        {
            b += d;
        }
        else
        {
            b += (xm > 0.0 ? fabs(tol1) : -fabs(tol1));
        }

        stresstensor alfa_stress_3 = ( start_stress * (1.0 - b) ) + ( end_stress * b );
        fb = YieldFunctionValue(alfa_stress_3);
    }

    return 0.0;
}


