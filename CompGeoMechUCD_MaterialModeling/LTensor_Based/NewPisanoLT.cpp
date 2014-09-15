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

// #define DEBUG_PISANO 1

#ifndef PisanoLT_CPP
#define PisanoLT_CPP

#include "NewPisanoLT.h"
#include <limits>
#include "LTensorDisplay.h"
#include <Channel.h>
#include <HDF5_Channel.h>
#include <Matrix.h>
#include <Vector.h>

const  DTensor2 NewPisanoLT::ZeroStrain(3, 3, 0.0);
const  DTensor2 NewPisanoLT::ZeroStress(3, 3, 0.0);
const double NewPisanoLT:: check_for_zero = sqrt(std::numeric_limits<double>::epsilon()); // used to check the variable nullity
const double NewPisanoLT::beta_min = 1e-16;
const double NewPisanoLT::beta_max = 1e+16;
const double NewPisanoLT::eplcum_cr = 1e-6;  // Should be always between 1e-5 and 1e-6 (cfr. Mojtaba E-Kan, Hossein A. Taiebat, On implementation of bounding surface plasticity models with no overshooting effect in solving boundary value problems, Computers and Geotechnics, Volume 55, January 2014, Pages 103-116, ISSN 0266-352X, http://dx.doi.org/10.1016/j.compgeo.2013.08.006.(http://www.sciencedirect.com/science/article/pii/S0266352X13001213))
const double NewPisanoLT::patm = 1.013e5;   // 1 Atmosphere of pressure [Pa]

DTensor4 NewPisanoLT::Ee(3, 3, 3, 3, 0.0);


//================================================================================
// Constructors & Destructors
//================================================================================
NewPisanoLT::NewPisanoLT(int tag,
                         double E0_in,
                         double v_in,
                         double M_in,
                         double kd_in,
                         double xi_in,
                         double h_in,
                         double m_in,
                         double rho_in,
                         double initialconfiningstress_in,
                         double n_in,
                         double a_in)
    : NDMaterialLT(tag, ND_TAG_NewPisanoLT),
      E0(E0_in),
      v(v_in), M(M_in),
      kd(kd_in),
      xi(xi_in),
      h(h_in), m(m_in),
      rho(rho_in),
      initialconfiningstress(initialconfiningstress_in),
      n(n_in),
      a(a_in),
      strainplcum(3, 3, 0.0),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      TrialPlastic_Strain( 3, 3, 0.0 ),
      ElasticStateStrain( 3, 3, 0.0 ),
      ElasticStateStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      CommitPlastic_Strain( 3, 3, 0.0 ),
      alpha( 3, 3, 0.0 ),
      alpha0( 3, 3, 0.0 ),
      alpha0mem( 3, 3, 0.0 ),
      strainpl0( 3, 3, 0.0 ),
      Stress_n_minus_2( 3, 3, 0.0 ),
      nij_dev( 3, 3, 0.0 ),
      Stiffness( 3, 3, 3, 3, 0.0 )
{
    DTensor2 initialStress(3, 3, 0.0);
    DTensor2 zeroT(3, 3, 0.0);

    initialStress(0, 0) = -initialconfiningstress;
    initialStress(1, 1) = -initialconfiningstress;
    initialStress(2, 2) = -initialconfiningstress;

#ifdef DEBUG_PISANO
    LTensorDisplay::print(initialStress, "initialStress");
#endif


    ElasticStateStress = DTensor2(initialStress);
    TrialPlastic_Strain(i, j) = 0 * initialStress(i, j);

    Stress_n_minus_2 = DTensor2(initialStress);

    beta = beta_max;

    nij_dev(0, 0) = 0.0;
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


NewPisanoLT::NewPisanoLT()
    : NDMaterialLT(0, ND_TAG_NewPisanoLT)
{
    this->revertToStart();
}

NewPisanoLT::~NewPisanoLT()
{

}

//================================================================================
//  Plasticity related setters and getters
//================================================================================
int NewPisanoLT::setTrialStrain(const DTensor2 &v)
{
    DTensor2 result( 3, 3, 0.0 );
    result( i, j ) = v( i, j ) - TrialStrain( i, j );

    return setTrialStrainIncr( result );
}


int NewPisanoLT::setTrialStrainIncr(const DTensor2 &strain_increment)
{
    return this->Explicit(strain_increment);
}


const DTensor4 &NewPisanoLT::getTangentTensor(void)
{
    return this->Stiffness;
}


const DTensor2  &NewPisanoLT::getStressTensor(void)
{
    return this->TrialStress;
}



const DTensor2 &NewPisanoLT::getStrainTensor(void)
{
    return this->TrialStrain;
}


const DTensor2 &NewPisanoLT::getPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}


//================================================================================
//  Save / Restore
//================================================================================
int NewPisanoLT::commitState(void)
{
    Stress_n_minus_2 = CommitStress; // before updating the CommitStress, we keep track of the previous one
    CommitStress = TrialStress;
    CommitStrain = TrialStrain;
    CommitPlastic_Strain = TrialPlastic_Strain;

    return 0;
}


int NewPisanoLT::revertToLastCommit(void)
{
    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    TrialPlastic_Strain = CommitPlastic_Strain;
    return 0;
}


int NewPisanoLT::revertToStart(void)
{
    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain = ZeroStrain;

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;

    double E = getE();
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
NDMaterialLT *NewPisanoLT::getCopy(void)
{
    NDMaterialLT *tmp = new NewPisanoLT(this->getTag(),
                                        this->getE0(),
                                        this->getv(),
                                        this->getM(),
                                        this->getkd(),
                                        this->getxi(),
                                        this->geth(),
                                        this->getm(),
                                        this->getRho(),
                                        this->getInitialConfiningStress(),
                                        this->getn(),
                                        this->geta());
    return tmp;
}

NDMaterialLT *NewPisanoLT::getCopy(const char *code)
{
    if (strcmp(code, "ThreeDimensional") == 0)
    {
        NewPisanoLT *tmp = new NewPisanoLT( this->getTag(),
                                            this->getE0(),
                                            this->getv(),
                                            this->getM(),
                                            this->getkd(),
                                            this->getxi(),
                                            this->geth(),
                                            this->getm(),
                                            this->getRho(),
                                            this->getInitialConfiningStress(),
                                            this->getn(),
                                            this->geta());
        return tmp;
    }
    else
    {
        cout.flush() << "NewPisanoLT::getCopy failed to get model: " <<  code << endln;
        exit(1);
    }

    return 0;
}

//================================================================================
//
//================================================================================
const char *NewPisanoLT::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
//  For the parallel / storage
//================================================================================

int NewPisanoLT::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
{
    theHDF5_Channel.beginMaterialDescription("NewPisanoLT" , this->getTag());
    theHDF5_Channel.addField("strain"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 1.
    theHDF5_Channel.addField("stress"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 2.
    theHDF5_Channel.addField("plastic_strain"              , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED); // 3.
    theHDF5_Channel.addField("alpha"                       , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 4.
    theHDF5_Channel.addField("alpha0"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 5.
    theHDF5_Channel.addField("stress_n_minus_2"            , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 6.
    theHDF5_Channel.addField("nij_dev"                     , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 7.
    theHDF5_Channel.addField("model_parameters"            , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_BASIC);    // 8.
    theHDF5_Channel.endMaterialDescription();

    return 0;
}


int NewPisanoLT::sendSelf(int commitTag, Channel &theChannel)
{
    Matrix aux(3, 3);

    //1 . Sending strain
    aux.setData(TrialStrain.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //2. Sending stress
    aux.setData(TrialStress.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //3. Sending plastic_strain
    aux.setData(TrialPlastic_Strain.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //4. Sending alpha
    aux.setData(alpha.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //5. Sending alpha0
    aux.setData(alpha0.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //6. Sending stress_n_minus_2
    aux.setData(Stress_n_minus_2.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);

    //7. Sending nij_dev
    aux.setData(nij_dev.data, 3, 3);
    theChannel.sendMatrix(0, 0, aux);


    //8. Sending model_parameters
    Vector model_parameters(12);
    //model_parameters(0) = beta0; // not needed anymore
    //model_parameters(2) = beta_min;  // this is not parameter
    model_parameters(0) = beta;  // this is not parameter
    model_parameters(1) = E0;
    model_parameters(2) = v;
    model_parameters(3) = M;
    model_parameters(4) = kd;
    model_parameters(5) = xi;
    model_parameters(6) = h;
    model_parameters(7) = m;
    model_parameters(8) = rho;
    model_parameters(9) = initialconfiningstress;
    model_parameters(10) = n;
    model_parameters(11) = a;

    theChannel.sendVector(0, 0, model_parameters);

    return 0;
}


int NewPisanoLT::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    return 0;
}

//================================================================================
//
//================================================================================

void NewPisanoLT::Print(ostream &s, int flag)
{
    s << (*this);
}

//================================================================================
//   Getters!
//================================================================================
double NewPisanoLT::getE0()
{
    return E0;
}

double NewPisanoLT::getv()
{
    return v;
}


double NewPisanoLT::getM()
{
    return M;
}


double NewPisanoLT::getkd()
{
    return kd;
}


double NewPisanoLT::getxi()
{
    return xi;
}


double NewPisanoLT::geth()
{
    return h;
}


double NewPisanoLT::getm()
{
    return m;
}


double NewPisanoLT::getRho(void)
{
    return rho;
}

double NewPisanoLT::getbeta_min(void)
{
    return beta_min;
}

double NewPisanoLT::getInitialConfiningStress(void)
{
    return initialconfiningstress;
}

double NewPisanoLT::getn(void)
{
    return n;
}

double NewPisanoLT::geta(void)
{
    return a;
}

DTensor2 &NewPisanoLT::getInternalTensor(void)
{
    return alpha;
}

//================================================================================
//  Plasticity!
//================================================================================

int NewPisanoLT::Explicit(const DTensor2 &strain_incr)
{
#ifdef DEBUG_PISANO
    cout << endl << endl << "NewPisanoLT::Explicit()" << endl;
#endif
    //=============================================================================================
    // Some local definitions
    //=============================================================================================
    int err = 0;

    // FP: some of these variables will be useless. Check at the end!!!

    double Delta_lambda           = 0.0;    // Plastic multiplier increment
    double G                      = 0.0;    // Shear mod
    double K                      = 0.0;    // Bulk mod
    double pp                     = 0.0;    // Mean pressure current (compression = positive)
    double incr_strain_vol        = 0.0;    // Volumetric incremental strain
    double p_incr_prev            = 0.0;    // Mean pressure previous (compression = positive)
    double pressure_ratio         = 0.0;    //
    double unload_prod            = 0.0;    //
    double H                      = 0.0;    // Hardening modulus
    double D                      = 0.0;    //
    double incr_strain_dev_nijdev = 0.0;    //
    double alpha_nijdev           = 0.0;    //
    double ep_stress_p            = 0.0;    //
    double eplcum_cr              = 0.0;    // overshooting parameter
    double strainplcum_vol        = 0.0;

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
    DTensor2 strainplcum_dev(3, 3, 0.0);
    DTensor4 Ep(3, 3, 3, 3, 0.0);           // Elastoplastic tangent modulus tensor

    Index < 'p' > p;                        // Extra index for LTensor computations
    Index < 'q' > q;                        // Extra index for LTensor computations

    //---------------------------------------------------------------------------------------------
    // Get initial state
    //---------------------------------------------------------------------------------------------

    //CHECK THIS!!!
    // strainplcum: new internal variable, to be set at zero
    // strainpl0: new internal variable, to be set at zero

    eplcum_cr = 1e-6;

    start_stress  = getStressTensor();
    start_strain  = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();

    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;

    incr_strain = strain_incr;
    double E = getE();

    G = E / (2.0 * (1.0 + v));
    K = E / (3.0 * (1.0 - 2.0 * v));

    start_stress.compute_deviatoric_tensor(s, pp);
    pp = -pp;
    incr_strain.compute_deviatoric_tensor(incr_strain_dev, incr_strain_vol);    // NOTE: Check this might not be needed : about recomputing incr_strain_vol
    incr_strain_vol = incr_strain(i, i); // isn't this redundant?

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

    double norm_nij_dev;

    // NOTE: Not consistent with article! Eqn. 10 FP: it is consistent, it can be demonstrated that the same nij_dev results!!!
    nij_dev(i, j) = 2 * G * incr_strain_dev(i, j) - p_incr_prev * alpha(i, j); // and then normalization
    norm_nij_dev = sqrt(nij_dev(i, j) * nij_dev(i, j));

    if (norm_nij_dev > check_for_zero)
    {
        nij_dev(i, j) = nij_dev(i, j) / norm_nij_dev;
    }
    else
    {
        nij_dev(i, j) = incr_strain_dev(i, j) * 0;
    }

    //---------------------------------------------------------------------------------------------
    // Check for unloading!!!
    //---------------------------------------------------------------------------------------------

    unload_prod = nij_dev(i, j) * alpha(i, j) - nij_dev(i, j) * alpha0(i, j) ;

    if (unload_prod <= 0.0)
    {
        //cout << "unloading condition fulfilled" << endl;

        // overshooting remediation
        double eplcum;

        strainplcum.compute_deviatoric_tensor(strainplcum_dev, strainplcum_vol); //check declaration!
        eplcum = sqrt((0.6666666) * strainplcum_dev(i, j) * strainplcum_dev(i, j));


        if (eplcum >= eplcum_cr) // the projection center is first memorized and updated
        {
            alpha0mem(i, j) = alpha0(i, j);
            alpha0(i, j)    = alpha(i, j);
        }
        else // the previous projection center is reset
        {
            alpha0(i, j) = alpha0mem(i, j); // back stress is set as at the previous unloading
        }

        strainpl0(i, j) = TrialPlastic_Strain(i, j); // the local cumulated plastic strain is restarted
    }


#ifdef DEBUG_PISANO
    //DEBUG
    LTensorDisplay::print(incr_strain, "incr_strain");
    //DEBUG
    LTensorDisplay::print(incr_strain_dev, "incr_strain_dev");
    //DEBUG
    LTensorDisplay::print(nij_dev, "nij_dev");
    //DEBUG
    LTensorDisplay::print(alpha, "alpha");
    //DEBUG
    LTensorDisplay::print(alpha0, "alpha0");

#endif
    //---------------------------------------------------------------------------------------------
    // Compute distance coeff
    //---------------------------------------------------------------------------------------------
    // beta = get_distance_coeff(start_stress); // this should be able to read alpha and alpha0
    beta = get_distance_coeff(start_stress); // this should be able to read alpha and alpha0
    // beta = get_distance_coeff_lode(start_stress); // this should be able to read alpha and alpha0

#ifdef DEBUG_PISANO
    //DEBUG
    cout << "beta = " << beta << endl;
#endif


    //---------------------------------------------------------------------------------------------
    // Compute plastic modulus (H)
    //---------------------------------------------------------------------------------------------

    H = pp * h * (pow(beta, m));


    //---------------------------------------------------------------------------------------------
    // Compute plastic flow direction (mij)
    //---------------------------------------------------------------------------------------------

    ///// D (dilatancy coefficient)
    alpha_d(i, j) = nij_dev(i, j) * (kd * sqrt(2.0 / 3.0));
    D = xi * (alpha_d(i, j) - alpha(i, j) ) * nij_dev(i, j);

    // D = get_dilatancy();

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
    incr_stress(i, j)  = Ee(i, j, p, q) * (  incr_strain(p, q) - incr_Pstrain(p, q) );
    ep_stress(i, j)    = start_stress(i, j) + incr_stress(i, j);

    // Update all variables
    TrialStress(i, j)         = ep_stress(i, j);            // try again Initiailize whenever you have tensor = tensor...
    TrialStrain(i, j)         = start_strain(i, j);
    TrialStrain(i, j)         += incr_strain(i, j);
    TrialPlastic_Strain(i, j) = start_Pstrain(i, j);
    TrialPlastic_Strain(i, j) += incr_Pstrain(i, j);
    // for overshooting remediation
    strainplcum(i, j) = TrialPlastic_Strain(i, j) - strainpl0(i, j);

    ep_stress.compute_deviatoric_tensor(ep_stress_dev, ep_stress_p);
    ep_stress_p = -ep_stress_p;

    // this avoids weird alpha values...it is difficult to find a general rule for the checking factor..tests needed
    // stress rapporto -> stress ratio
    if (ep_stress_p > 1000 * check_for_zero)   // FIXME: the 1000 factor is arbitrary
    {
        alpha(i, j) = ep_stress_dev(i, j);
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

    // NOTE: Problem with the stiffness indexig...... ask Gregor FP: WHY????

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

#ifdef DEBUG_PISANO
    LTensorDisplay::print(Stiffness, "Stiffness", "\n\n\n", 1);
#endif

#ifdef DEBUG_PISANO
    cout << endl << endl << "unload_prod = " << unload_prod <<
         "  nij(1,3) = " <<  nij_dev(0, 2) <<
         "  alpha(1,3) = " <<  alpha(0, 2) <<
         "  alpha0(1,3) = " <<  alpha0(0, 2) <<
         "  beta = " <<  beta << endl;
#endif


    return err;
}





// Probably should be inlined
double NewPisanoLT::get_distance_coeff(DTensor2 &start_stress)
{
    double nij_dev_norm = sqrt (nij_dev(i, j) * nij_dev(i, j));

    if (nij_dev_norm > 1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
    {

        //coefficients 2nd order equation
        double a;
        double b;
        double c;
        double Delta_equation;



        //// a

        a = (alpha(i, j) - alpha0(i, j)) * (alpha(i, j) - alpha0(i, j));

        //// b

        b = 2 * alpha(i, j) * (alpha(i, j) - alpha0(i, j));

        //// c

        // c = alpha(i, j) * alpha(i, j) - (2.0 / 3.0) * M * M;
        c = alpha(i, j) * alpha(i, j) - (0.6666666666666666666666666666666666666) * M * M;

        //// solution

        if ( a > check_for_zero)
        {
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
        else  // a is close to zero
        {
            beta = beta_max;
        }

    }
    else
    {
        // cerr << "The norm of nij is not 1! This should never happen. Something wrong... ask Pisano." << endl;
        // cout << "Norm of nij_dev is nil. Might be non-deviatoric loading step... or something is wrong. Ask Pisano." << endl;

    };

    return beta;
}





///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double NewPisanoLT::get_distance_coeff_lode(DTensor2 &start_stress)
{
    // A NEW SOIL PARAMETER n NEEDS TO BE DEFINED FOR THE DEVIATORIC SECTION!!!

    double nij_dev_norm = sqrt (nij_dev(i, j) * nij_dev(i, j));

    if (nij_dev_norm > 1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
    {
        double sinphi;
        double beta_secdev;
        double arg;
        double lode_step;
        double R;
        double M_lode;

        sinphi       = 3.0 * M / (6.0 + M);
        arg          = (3.0 + sinphi) / (3.0 - sinphi);
        beta_secdev  = (pow(arg, -1.0 / n) - 1.0) / (pow(arg, -1.0 / n) + 1.0);
        lode_step    = M_PI / 300.0; // lode angle step increment

        for (double lode = -M_PI / 6.0; lode <= M_PI / 6.0; lode += lode_step ) // loop over the Lode angle
        {
            R = pow(((1 - beta_secdev * sin(3.0 * lode)) / (1 - beta_secdev)), n);
            M_lode = M * R; // shear strength at a given (assumed) lode angle

            //coefficients 2nd order equation
            double a;
            double b;
            double c;
            double Delta_equation;

            //// a

            a = (alpha(i, j) - alpha0(i, j)) * (alpha(i, j) - alpha0(i, j));

            //// b

            b = 2 * alpha(i, j) * (alpha(i, j) - alpha0(i, j));

            //// c

            c = alpha(i, j) * alpha(i, j) - (0.6666666666666666666666666666666666666) * M_lode * M_lode;

            //// solution

            if ( a > check_for_zero)
            {
                Delta_equation = b * b - 4.0 * a * c;

                if (Delta_equation < 0.0)
                {
                    beta = beta_min;

                    //  cout << "negative Delta in the distance coefficient equation" <<alpha_norm << "e" << alpha_nijdev << endl;
                }
                else
                {
                    beta  = (-b + sqrt(Delta_equation)) / (2.0 * a); // distance coefficient

                    DTensor2 alphab(3, 3, 0.0);
                    double   lode_alphab;

                    alphab(i, j) = alpha(i, j) + beta * (alpha(i, j) - alpha0(i, j)); // bounding back stress

                    lode_alphab = get_lode_angle(alphab); //CHECK THIS

                    if (fabs(lode_alphab - lode) < lode_step)
                    {
                        break; // gets out of the lode angle loop
                    };


                };

                if (beta < beta_min)
                {
                    //   cout << "negative distance coefficient" << endl;
                    beta = beta_min;
                };
            }
            else  // a is close to zero
            {
                beta = beta_max;
            }


        }


    }
    else
    {
        // cout << "Norm of nij_dev is nil. Might be non-deviatoric loading step... or something is wrong. Ask Pisano." << endl;

    };

    return beta;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


double NewPisanoLT::get_dilatancy()
{
    // I am using here input values: kd,xi,n,ndev,alpha

    DTensor2 alphad(3, 3, 0.0);

    double sinphid;
    double arg;
    double R;
    double D;
    double kd_lode;
    double lode;
    double beta_secdev;

    sinphid      = 3.0 * kd / (6.0 + kd);
    arg          = (3.0 + sinphid) / (3.0 - sinphid);
    beta_secdev  = (pow(arg, -1.0 / n) - 1.0) / (pow(arg, -1.0 / n) + 1.0);


    double nij_dev_norm = sqrt (nij_dev(i, j) * nij_dev(i, j));

    if (nij_dev_norm > 1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
    {
        lode    = get_lode_angle(nij_dev);

        R       = pow(((1 - beta_secdev * sin(3.0 * lode)) / (1 - beta_secdev)), n);
        kd_lode = kd * R; // shear strength at a given (assumed) lode angle

    }
    else
    {

        kd_lode = kd;

    };

    alphad(i, j) = (sqrt(2.0 / 3.0) * kd_lode) * nij_dev(i, j);

    D = xi * (nij_dev(i, j) * (alpha(i, j) - alphad(i, j)));

    return D;
}


//Sign function from http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
int sgn(double val)
{
    return (double(0) < val) - (val < double(0));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double NewPisanoLT::get_lode_angle(DTensor2   s) // computes the Lode angle of a stress state
{
    //The input tensor s must be deviatoric

    if (abs(s(i, i)) > check_for_zero )
    {
        cerr << "NewPisanoLT::lode   Warning! Lode angle is being computed with a non-deviatoric tensor s.\n\n";
    }

    double J2;
    double J3;
    double arg;



    J2 = 0.5 * (s(i, j) * s(i, j));

    J3 = 0.3333333 * s.compute_Determinant(); // I need the determinant of the deviatoric stress tensor!!!

    if (J2 > check_for_zero)
    {
        arg = -(3.0 * 1.7321 / 2.0) * (sgn(J3) * fabs((pow(J3, 0.333333333333)))) / sqrt(J2); // check this!
    }
    else
    {
        arg = 0.0; // this is for hydrostastic stress states
    };

    if (arg > 1.0)
    {
        arg = 1.0;
    }
    else if (arg < -1.0)
    {
        arg = -1.0;
    }
    else
    {
        // this would be empty, do we need this?
    };

    //Return the lode angle
    return 0.3333333 * asin(arg);
}

double NewPisanoLT::getE()
{
    double p = -CommitStress(i, i) / 3.0;
    if (p > patm / 2)
    {
        return E0 * pow(p / patm, a);
    }
    else
    {
        return E0 * pow(0.5, a);
    }
}


#endif

