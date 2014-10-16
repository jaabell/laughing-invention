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


#ifndef PisanoLT_CPP
#define PisanoLT_CPP

#include "PisanoLT.h"
#include <limits>
#include "LTensorDisplay.h"
#include <Channel.h>
#include <HDF5_Channel.h>
#include <Matrix.h>
#include <Vector.h>

const  DTensor2 PisanoLT::ZeroStrain(3, 3, 0.0);
const  DTensor2 PisanoLT::ZeroStress(3, 3, 0.0);
const double PisanoLT:: check_for_zero = sqrt(std::numeric_limits<double>::epsilon()); // used to check the variable nullity
DTensor4 PisanoLT::Ee(3, 3, 3, 3, 0.0);


//================================================================================
// Constructors & Destructors
//================================================================================
PisanoLT::PisanoLT(int tag,
                   double E_in,
                   double v_in,
                   double M_in,
                   double kd_in,
                   double xi_in,
                   double h_in,
                   double m_in,
                   double rho_in,
                   double initialconfiningstress_in,
                   double beta_min_in)
    : NDMaterialLT(tag, ND_TAG_PisanoLT),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      TrialPlastic_Strain( 3, 3, 0.0 ),
      ElasticStateStrain( 3, 3, 0.0 ),
      ElasticStateStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      CommitPlastic_Strain( 3, 3, 0.0 ),
      alpha( 3, 3, 0.0 ),
      Stress_n_minus_2( 3, 3, 0.0 ),
      nij_dev( 3, 3, 0.0 ),
      nij_dev_prev( 3, 3, 0.0 ),
      Stiffness( 3, 3, 3, 3, 0.0 ),
      beta_min(beta_min_in),
      E(E_in),
      v(v_in), M(M_in),
      kd(kd_in),
      xi(xi_in),
      h(h_in), m(m_in),
      rho(rho_in),
      initialconfiningstress(initialconfiningstress_in)
{
    DTensor2 initialStress(3, 3, 0.0);
    DTensor2 zeroT(3, 3, 0.0);

    initialStress(0, 0) = -initialconfiningstress;
    initialStress(1, 1) = -initialconfiningstress;
    initialStress(2, 2) = -initialconfiningstress;

    ElasticStateStress = DTensor2(initialStress);
    TrialPlastic_Strain(i, j) = 0 * initialStress(i, j);
    nij_dev_prev(i, j) = 0 * initialStress(i, j);

    Stress_n_minus_2 = DTensor2(initialStress); //FP
    beta  = M * sqrt(2.0 / 3.0); //FP
    beta0 = beta; //FP


    // FIXME: Hard-coded n_ij deviatoric!!!!
    nij_dev(0, 0) = 0.0; //FP
    nij_dev(1, 1) = 0.0;
    nij_dev(2, 2) = 0.0;
    nij_dev(0, 1) = 0.0;
    nij_dev(1, 0) = 0.0;
    nij_dev(0, 2) = -0.7071;
    nij_dev(2, 0) = -0.7071;
    nij_dev(1, 2) = 0.0;
    nij_dev(2, 1) = 0.0;

    this->revertToStart();
}


PisanoLT::PisanoLT()
    : NDMaterialLT(0, ND_TAG_PisanoLT)
{
    this->revertToStart();
}

PisanoLT::~PisanoLT()
{

}

//================================================================================
//  Plasticity related setters and getters
//================================================================================
int PisanoLT::setTrialStrain(const DTensor2 &v)
{
    DTensor2 result( 3, 3, 0.0 );
    result( i, j ) = v( i, j ) - TrialStrain( i, j );

    return setTrialStrainIncr( result );
}


int PisanoLT::setTrialStrainIncr(const DTensor2 &strain_increment)
{
    return this->Explicit(strain_increment);
}


const DTensor4 &PisanoLT::getTangentTensor(void)
{
    return this->Stiffness;
}


const DTensor2  &PisanoLT::getStressTensor(void)
{
    return this->TrialStress;
}



const DTensor2 &PisanoLT::getStrainTensor(void)
{
    return this->TrialStrain;
}


const DTensor2 &PisanoLT::getPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}


//================================================================================
//  Save / Restore
//================================================================================
int PisanoLT::commitState(void)
{
    Stress_n_minus_2 = CommitStress; // before updating the CommitStress, we keep track of the previous one
    CommitStress = TrialStress;
    CommitStrain = TrialStrain;
    CommitPlastic_Strain = TrialPlastic_Strain;

    return 0;
}


int PisanoLT::revertToLastCommit(void)
{
    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    TrialPlastic_Strain = CommitPlastic_Strain;
    return 0;
}


int PisanoLT::revertToStart(void)
{
    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain = ZeroStrain;

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    // TrialPlastic_Strain = ZeroStrain;

    double lambda = ( v * E ) / ( ( 1 + v ) * ( 1 - 2 * v ) );
    double mu = E / ( 2 * ( 1 + v ) );

    Ee( 0, 0, 0, 0 ) = lambda + 2 * mu;
    Ee( 0, 0, 1, 1 ) = lambda;
    Ee( 0, 0, 2, 2 ) = lambda;
    Ee( 0, 1, 0, 1 ) = mu;
    Ee( 0, 1, 1, 0 ) = mu;
    Ee( 0, 2, 0, 2 ) = mu;
    Ee( 0, 2, 2, 0 ) = mu;
    Ee( 1, 0, 0, 1 ) = mu;
    Ee( 1, 0, 1, 0 ) = mu;
    Ee( 1, 1, 0, 0 ) = lambda;
    Ee( 1, 1, 1, 1 ) = lambda + 2 * mu;
    Ee( 1, 1, 2, 2 ) = lambda;
    Ee( 1, 2, 1, 2 ) = mu;
    Ee( 1, 2, 2, 1 ) = mu;
    Ee( 2, 0, 0, 2 ) = mu;
    Ee( 2, 0, 2, 0 ) = mu;
    Ee( 2, 1, 1, 2 ) = mu;
    Ee( 2, 1, 2, 1 ) = mu;
    Ee( 2, 2, 0, 0 ) = lambda;
    Ee( 2, 2, 1, 1 ) = lambda;
    Ee( 2, 2, 2, 2 ) = lambda + 2 * mu;

    Stiffness = Ee;

    return 0;
}

//================================================================================
//  Deep Copy
//================================================================================
NDMaterialLT *PisanoLT::getCopy(void)
{
    NDMaterialLT *tmp = new PisanoLT(this->getTag(),
                                     this->getE(),
                                     this->getv(),
                                     this->getM(),
                                     this->getkd(),
                                     this->getxi(),
                                     this->geth(),
                                     this->getm(),
                                     this->getRho(),
                                     this->getInitialConfiningStress(),
                                     this->getbeta_min());
    return tmp;
}

NDMaterialLT *PisanoLT::getCopy(const char *code)
{
    if (strcmp(code, "ThreeDimensional") == 0)
    {
        PisanoLT *tmp = new PisanoLT( this->getTag(),
                                      this->getE(),
                                      this->getv(),
                                      this->getM(),
                                      this->getkd(),
                                      this->getxi(),
                                      this->geth(),
                                      this->getm(),
                                      this->getRho(),
                                      this->getInitialConfiningStress(),
                                      this->getbeta_min());
        return tmp;
    }
    else
    {
        cout.flush() << "PisanoLT::getCopy failed to get model: " <<  code << endln;
        exit(1);
    }

    return 0;
}

//================================================================================
//
//================================================================================
const char *PisanoLT::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
//  For the parallel / storage
//================================================================================

int PisanoLT::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
{
    theHDF5_Channel.beginMaterialDescription("PisanoLT" , this->getTag());
    theHDF5_Channel.addField("strain"                   , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 1.
    theHDF5_Channel.addField("stress"                   , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 2.
    theHDF5_Channel.addField("plastic_strain"           , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED); // 3.
    theHDF5_Channel.addField("alpha"                    , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 4.
    theHDF5_Channel.addField("stress_n_minus_2"         , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 5.
    theHDF5_Channel.addField("nij_dev"                  , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 6.
    theHDF5_Channel.addField("nij_dev_prev"             , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 7.
    theHDF5_Channel.addField("model_parameters"         , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_DETAILED); // 8.
    theHDF5_Channel.endMaterialDescription();

    return 0;
}


int PisanoLT::sendSelf(int commitTag, Channel &theChannel)
{
    Matrix a(3, 3);

    //1 . Sending strain
    a.setData(TrialStrain.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //2. Sending stress
    a.setData(TrialStress.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //3. Sending plastic_strain
    a.setData(TrialPlastic_Strain.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //4. Sending alpha
    a.setData(alpha.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //5. Sending stress_n_minus_2
    a.setData(Stress_n_minus_2.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //6. Sending nij_dev
    a.setData(nij_dev.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //7. Sending nij_dev_prev
    a.setData(nij_dev_prev.data, 3, 3);
    theChannel.sendMatrix(0, 0, a);

    //8. Sending model_parameters
    Vector model_parameters(12);
    model_parameters(0) = beta0;
    model_parameters(1) = beta;
    model_parameters(2) = beta_min;
    model_parameters(3) = E;
    model_parameters(4) = v;
    model_parameters(5) = M;
    model_parameters(6) = kd;
    model_parameters(7) = xi;
    model_parameters(8) = h;
    model_parameters(9) = m;
    model_parameters(10) = rho;
    model_parameters(11) = initialconfiningstress;

    theChannel.sendVector(0, 0, model_parameters);

    return 0;
}


int PisanoLT::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

//================================================================================
//
//================================================================================

void PisanoLT::Print(ostream &s, int flag)
{
    s << (*this);
}

//================================================================================
//   Getters!
//================================================================================
double PisanoLT::getE()
{
    return E;
}

double PisanoLT::getv()
{
    return v;
}


double PisanoLT::getM()
{
    return M;
}


double PisanoLT::getkd()
{
    return kd;
}


double PisanoLT::getxi()
{
    return xi;
}


double PisanoLT::geth()
{
    return h;
}


double PisanoLT::getm()
{
    return m;
}


double PisanoLT::getRho(void)
{
    return rho;
}

double PisanoLT::getbeta_min(void)
{
    return beta_min;
}

double PisanoLT::getInitialConfiningStress(void)
{
    return initialconfiningstress;
}

DTensor2 &PisanoLT::getInternalTensor(void)
{
    return alpha;
}

//================================================================================
//  Plasticity!
//================================================================================

int PisanoLT::Explicit(const DTensor2 &strain_incr)
{

    //=============================================================================================
    // Some local definitions
    //=============================================================================================
    int err = 0;

    double Delta_lambda           = 0.0;    // Plastic parameter increment
    double G                      = 0.0;    // Shear mod
    double K                      = 0.0;    // Bulk mod
    double pp                     = 0.0;    // Mean pressure current (compression = positive)
    double incr_strain_vol        = 0.0;    // Volumetric incremental strain
    double p_incr_prev            = 0.0;    // Mean pressure previous (compression = positive)
    double pressure_ratio         = 0.0;    //
    double sign                   = 0.0;    //
    double beta                   = 0.0;    // distance coefficient
    double unload_prod            = 0.0;    //
    double arg_den                = 0.0;    //
    // double arg_den_min            ;//= 0.0;    //
    double H                      = 0.0;    // Hardening parameter
    double alpha_norm             = 0.0;    //
    double xi_var                 = 0.0;    // Damping ratio
    double D                      = 0.0;    //
    double incr_strain_dev_nijdev = 0.0;    //
    double alpha_nijdev           = 0.0;    //
    double ep_stress_p            = 0.0;    //

    DTensor2 mij(3, 3, 0.0);                // Unit vector normal to potential surface
    DTensor2 incr_strain(3, 3, 0.0);        //
    DTensor2 incr_stress(3, 3, 0.0);        //
    DTensor2 incr_Pstrain(3, 3, 0.0);       //
    DTensor2 ep_stress(3, 3, 0.0);          //
    DTensor2 predicted_stress(3, 3, 0.0);   //
    DTensor2 start_stress(3, 3, 0.0);       // Stress at start of step
    DTensor2 start_strain(3, 3, 0.0);       // Strain at start of step
    DTensor2 start_Pstrain(3, 3, 0.0);      // Plastic strain at start of step
    DTensor2 s(3, 3, 0.0);                  // Deviatoric stress tensor (unused?)
    DTensor2 incr_strain_dev(3, 3, 0.0);    // Incremental deviatoric strain
    DTensor2 stress_incr_prev(3, 3, 0.0);   // Previous step stress increment
    DTensor2 alpha_d(3, 3, 0.0);            //
    DTensor2 alphad_alpha(3, 3, 0.0);       //
    DTensor2 ep_stress_dev(3, 3, 0.0);      // Deviatoric elastoplastic stress
    DTensor2 I2(3, 3, 0.0);                 // I2 is the identity matrix

    DTensor4 Ep(3, 3, 3, 3, 0.0);           // Elastoplastic tangent modulus tensor

    Index < 'p' > p;                        // Extra index for LTensor computations
    Index < 'q' > q;                        // Extra index for LTensor computations

    //---------------------------------------------------------------------------------------------
    // Get initial state
    //---------------------------------------------------------------------------------------------

    start_stress  = getStressTensor();
    start_strain  = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();

    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;

    incr_strain = strain_incr;

    G = E / (2.0 * (1.0 + v));
    K = E / (3.0 * (1.0 - 2.0 * v));

    start_stress.compute_deviatoric_tensor(s, pp);
    pp = -pp;
    incr_strain.compute_deviatoric_tensor(incr_strain_dev, incr_strain_vol);
    incr_strain_vol = incr_strain(i, i);

    // LTensorDisplay::print(incr_strain,"a","Hi there!",1);
    // LTensorDisplay::print(incr_strain_dev,"a","Hi there!",1);
    // cout << "incr_strain_vol = " << incr_strain_vol << endl;

    stress_incr_prev(i, j) = start_stress(i, j) - Stress_n_minus_2(i, j);
    p_incr_prev = -stress_incr_prev(i, i) / 3;

    pressure_ratio = fabs(p_incr_prev / pp);


    //---------------------------------------------------------------------------------------------
    // Some healthy checkups
    //---------------------------------------------------------------------------------------------
    if ( (pressure_ratio < 1.0e-3) || (fabs(pp) < 1.0) ) // this avoids the influence of very small pressure fluctuations on nij_dev
    {
        p_incr_prev = 0.0;
    }

    //---------------------------------------------------------------------------------------------
    // Define tangent to yield surface with the sign of the deviatoric strain increment
    //---------------------------------------------------------------------------------------------
    if (fabs(incr_strain_dev(0, 2)) > check_for_zero)
    {
        sign = incr_strain_dev(0, 2) / fabs(incr_strain_dev(0, 2));
    }
    else
    {
        sign = 0.0;
    }

    nij_dev(0, 2) = sign * 0.7071;
    nij_dev(2, 0) = sign * 0.7071;


    //---------------------------------------------------------------------------------------------
    // Compute distance coeff
    //---------------------------------------------------------------------------------------------
    beta = get_distance_coeff(start_stress);


    //---------------------------------------------------------------------------------------------
    // Check for unloading!!!
    //---------------------------------------------------------------------------------------------

    unload_prod = nij_dev(i, j) * nij_dev_prev(i, j);

    if (unload_prod < 0.0)
    {
        beta0 = beta; // stress tensor at last load reversal
    }

    // else
    // {
    //     beta0 = beta0;
    // };


    //---------------------------------------------------------------------------------------------
    // Compute plastic modulus (H)
    //---------------------------------------------------------------------------------------------
    arg_den = fabs(beta - beta0);
    // double arg_den_min =  1.0e3 * check_for_zero;

    if (arg_den > 1e-15)
    {
        H = pp * h * (pow(beta / arg_den, m));
    }
    else
    {
        H = 1.0e20;
    };


    //---------------------------------------------------------------------------------------------
    // Compute plastic flow direction (mij)
    //---------------------------------------------------------------------------------------------

    ///// D (dilatancy coefficient)
    alpha_d(i, j) = nij_dev(i, j) * (kd * sqrt(2.0 / 3.0));

    alphad_alpha(i, j) = alpha_d(i, j) - alpha(i, j);

    alpha_norm = sqrt (alpha(i, j) * alpha(i, j));

    xi_var = xi * (1.0 - 1.0 * alpha_norm / (M * sqrt(2.0 / 3.0)));

    D =  (alphad_alpha(i, j) * nij_dev(i, j)) * xi_var;

    ///// mij (risulting direction)
    mij = nij_dev;

    mij(0, 0) = nij_dev(0, 0) - 1.0 * D / 3.0;

    mij(1, 1) = nij_dev(1, 1) - 1.0 * D / 3.0;

    mij(2, 2) = nij_dev(2, 2) - 1.0 * D / 3.0;


    //---------------------------------------------------------------------------------------------
    // Compute plastic multiplier
    //---------------------------------------------------------------------------------------------
    // scalar product incr_strain_dev:nij_dev
    incr_strain_dev_nijdev = incr_strain_dev(i, j) * nij_dev(i, j);

    alpha_nijdev = alpha(i, j) * nij_dev(i, j);                          // scalar product alphaij:nij_dev

    // Plastic muliplier;
    double num =  (2.0 * G * incr_strain_dev_nijdev + K * alpha_nijdev * incr_strain_vol);

    double den =   2.0 * G + (2.0 / 3.0) * H - K * D * alpha_nijdev;

    Delta_lambda = num / den;

    if (Delta_lambda < 0.0)
    {
        Delta_lambda = 0.0;
    }

    //---------------------------------------------------------------------------------------------
    // Update internal variales and increments
    //---------------------------------------------------------------------------------------------
    // Plastic strain increment

    incr_Pstrain(i, j) = mij(i, j) * (Delta_lambda);
    incr_stress(i, j) = Ee(i, j, p, q) * (  incr_strain(p, q) - incr_Pstrain(p, q) );
    ep_stress(i, j) = start_stress(i, j) + incr_stress(i, j);

    // Update all variables
    TrialStress         = ep_stress;              // try again Initiailize whenever you have tensor = tensor...
    TrialStrain         = start_strain;
    TrialStrain         += incr_strain;
    TrialPlastic_Strain = start_Pstrain;
    TrialPlastic_Strain += incr_Pstrain;

    ep_stress.compute_deviatoric_tensor(ep_stress_dev, ep_stress_p);
    ep_stress_p = -ep_stress_p;

    if (ep_stress_p > 1000 * check_for_zero)   // FIXME: the 1000 factor is arbitrary
    {
        alpha = ep_stress_dev;
        alpha /= ep_stress_p;
    }
    else
    {
        //Set all values in alpha to zero
        for (DTensor2::literator it = alpha.begin(); it != alpha.end(); it++ )
        {
            *it = 0.0;
        }
    };

    //---------------------------------------------------------------------------------------------
    // Compute Elastic-Plastic stiffness
    //---------------------------------------------------------------------------------------------
    // To obtain Eep, at the last step

    //Initialize identity matrix
    I2(0, 0) = 1;

    I2(1, 1) = 1;

    I2(2, 2) = 1;

    Stiffness(p, q, i, j) =  nij_dev(i, j) * nij_dev(p, q) * (-4.0 * G * G / den) +
                             I2(i, j)      * nij_dev(p, q) * (2.0 * G * K * D / den) +
                             nij_dev(i, j) * I2(p, q)      * (-2.0 * G * K * alpha_nijdev / den) +
                             I2(i, j)      * I2(p, q)      * (K - (2.0 / 3.0) * G + K * K * D * alpha_nijdev / den);

    Stiffness(0, 0, 0, 0) = Stiffness(0, 0, 0, 0) + (2.0 * G);

    Stiffness(0, 1, 0, 1) = Stiffness(0, 1, 0, 1) + (2.0 * G);

    Stiffness(0, 2, 0, 2) = Stiffness(0, 2, 0, 2) + (2.0 * G);

    Stiffness(1, 0, 1, 0) = Stiffness(1, 0, 1, 0) + (2.0 * G);

    Stiffness(1, 1, 1, 1) = Stiffness(1, 1, 1, 1) + (2.0 * G);

    Stiffness(1, 2, 1, 2) = Stiffness(1, 2, 1, 2) + (2.0 * G);

    Stiffness(2, 0, 2, 0) = Stiffness(2, 0, 2, 0) + (2.0 * G);

    Stiffness(2, 1, 2, 1) = Stiffness(2, 1, 2, 1) + (2.0 * G);

    Stiffness(2, 2, 2, 2) = Stiffness(2, 2, 2, 2) + (2.0 * G);

    // LTensorDisplay::print(Stiffness,"a","Hi there!",1);

    nij_dev_prev = nij_dev;  // keeps track of the previous nij_dev tensor

    return err;
}

// Probably should be inlined
double PisanoLT::get_distance_coeff(DTensor2 &start_stress)
{

    double nij_dev_norm = sqrt (nij_dev(i, j) * nij_dev(i, j));

    if (nij_dev_norm > 1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
    {
        //      cout << "standard beta calculation. nij_dev_norm=" << nij_dev_norm << endl;

        //coefficients 2nd order equation
        double a;
        double b;
        double c;
        double Delta_equation;



        //// a

        a = 1.0;


        //// b

        double alpha_nijdev = alpha(i, j) * nij_dev(i, j);

        b = 2.0 * alpha_nijdev;

        //// c

        double alpha_norm = sqrt (alpha(i, j) * alpha(i, j));


        c = alpha_norm * alpha_norm  -  (2.0 / 3.0) * (M * M);


        //// solution


        Delta_equation = b * b - 4.0 * a * c;

        if (Delta_equation < 0.0)
        {
            beta = beta_min;

            //             cout << "negative Delta in the distance coefficient equation" <<alpha_norm << "e" << alpha_nijdev << endl;
        }
        else
        {
            beta = (-b + sqrt(Delta_equation)) / (2.0 * a);
        };



        if (beta < beta_min)
        {
            //        cout << "negative distance coefficient" << endl;
            beta = beta_min;
        };


    }
    else
    {
        //     cout << "nil nij_dev...beta not modified" << endl;

    };

    return beta;
}




#endif

