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
// the functions embeded here for increasing the efficiency and speed.

#ifndef Pisano_model_CPP
#define Pisano_model_CPP

#include "Pisano_model.h"


const  straintensor Pisano_model::ZeroStrain;
const  stresstensor Pisano_model::ZeroStress;

BJtensor Pisano_model::Ee(4, def_dim_4, 0.0);
Matrix Pisano_model::D(6, 6);


// const double Pisano_model::beta_min = 1.0e-16; // a positive very low number
// const double Pisano_model::beta_max = 1.0e16; // a positive very high number
const double Pisano_model:: check_for_zero = sqrt(DBL_EPSILON); // used to check the variable nullity



// Constructor
//================================================================================
Pisano_model::Pisano_model(int tag,
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
    : NDMaterial(tag, ND_TAG_Pisano_model),
      // Stiffness(4,def_dim_4,0.0),
      beta_min(beta_min_in),
      E(E_in),
      v(v_in), M(M_in),
      kd(kd_in),
      xi(xi_in),
      h(h_in), m(m_in),
      rho(rho_in),
      initialconfiningstress(initialconfiningstress_in)
{

    stresstensor initialStress;
    initialStress.val(1, 1) = -initialconfiningstress;
    initialStress.val(2, 2) = -initialconfiningstress;
    initialStress.val(3, 3) = -initialconfiningstress;


    ElasticStateStress.Initialize(initialStress);


    stresstensor zeroT;
    alpha.Initialize(zeroT); // FP
    Stress_n_minus_2.Initialize(initialStress);  //FP
    beta  = M * sqrt(2.0 / 3.0); //FP
    beta0 = beta; //FP

    nij_dev.val(1, 1) = 0.0; //FP
    nij_dev.val(2, 2) = 0.0;
    nij_dev.val(3, 3) = 0.0;
    nij_dev.val(1, 2) = 0.0;
    nij_dev.val(2, 1) = 0.0;
    nij_dev.val(1, 3) = -0.7071;
    nij_dev.val(3, 1) = -0.7071;
    nij_dev.val(2, 3) = 0.0;
    nij_dev.val(3, 2) = 0.0;


    this->revertToStart();
}


// Constructor
//================================================================================
Pisano_model::Pisano_model()
    : NDMaterial(0, ND_TAG_Pisano_model)
{
    this->revertToStart();
}

// Destructor
//================================================================================
Pisano_model::~Pisano_model()
{

}

//================================================================================
int Pisano_model::setTrialStrain(const Tensor& v)
{
    return this->setTrialStrainIncr( v - getStrainTensor() );
}

//================================================================================
int Pisano_model::setTrialStrainIncr(const Tensor& strain_increment)
{
    return this->Explicit(strain_increment);
}

//================================================================================
const BJtensor& Pisano_model::getTangentTensor(void)
{
    return this->Stiffness;
}

//================================================================================
const stresstensor&  Pisano_model::getStressTensor(void)
{
    return this->TrialStress;
}


//================================================================================
const straintensor& Pisano_model::getStrainTensor(void)
{
    return this->TrialStrain;
}

//================================================================================
const straintensor& Pisano_model::getPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}


//================================================================================
int Pisano_model::commitState(void)
{
    Stress_n_minus_2.Initialize(CommitStress); // before updating the CommitStress, we keep track of the previous one
    // Stress_n_minus_2 = CommitStress; // before updating the CommitStress, we keep track of the previous one

    CommitStress.Initialize(TrialStress);
    CommitStrain.Initialize(TrialStrain);
    CommitPlastic_Strain.Initialize(TrialPlastic_Strain);

    //     nij_dev_prev.Initialize(nij_dev);  // keeps track of the previous nij_dev tensor
    //     nij_dev_prev = nij_dev;  // keeps track of the previous nij_dev tensor

    // cout << "commit called  ............." << endl;

    return 0;
}

//=========================================================================================
int Pisano_model::revertToLastCommit(void)
{
    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(CommitPlastic_Strain);

    return 0;
}

//=========================================================================================
int Pisano_model::revertToStart(void)
{
    int err = 0;

    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain.Initialize(ZeroStrain);

    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(ZeroStrain);

    double lambda = (v * E) / ((1 + v) * (1 - 2 * v));
    double mu = E / (2 * (1 + v));

    Ee.val(1, 1, 1, 1) = lambda + 2 * mu;
    Ee.val(1, 1, 2, 2) = lambda;
    Ee.val(1, 1, 3, 3) = lambda;
    Ee.val(1, 2, 1, 2) = mu;
    Ee.val(1, 2, 2, 1) = mu;
    Ee.val(1, 3, 1, 3) = mu;
    Ee.val(1, 3, 3, 1) = mu;
    Ee.val(2, 1, 1, 2) = mu;
    Ee.val(2, 1, 2, 1) = mu;
    Ee.val(2, 2, 1, 1) = lambda;
    Ee.val(2, 2, 2, 2) = lambda + 2 * mu;
    Ee.val(2, 2, 3, 3) = lambda;
    Ee.val(2, 3, 2, 3) = mu;
    Ee.val(2, 3, 3, 2) = mu;
    Ee.val(3, 1, 1, 3) = mu;
    Ee.val(3, 1, 3, 1) = mu;
    Ee.val(3, 2, 2, 3) = mu;
    Ee.val(3, 2, 3, 2) = mu;
    Ee.val(3, 3, 1, 1) = lambda;
    Ee.val(3, 3, 2, 2) = lambda;
    Ee.val(3, 3, 3, 3) = lambda + 2 * mu;

    Stiffness.Initialize(Ee);
    //     Stiffness = Ee;

    return err;
}

//================================================================================
NDMaterial* Pisano_model::getCopy(void)
{
    NDMaterial* tmp = new Pisano_model(this->getTag(),
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

//================================================================================
NDMaterial* Pisano_model::getCopy(const char* code)
{
    if (strcmp(code, "ThreeDimensional") == 0)
    {
        Pisano_model* tmp = new Pisano_model( this->getTag(),
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
        cout.flush() << "Pisano_model::getCopy failed to get model: " <<  code << endln;
        exit(1);
    }

    return 0;
}

//================================================================================
const char* Pisano_model::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
int Pisano_model::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

//================================================================================
int Pisano_model::receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}

//================================================================================
void Pisano_model::Print(ostream& s, int flag)
{
    s << (*this);
}

//================================================================================
double Pisano_model::getE()
{
    return E;
}

//================================================================================
double Pisano_model::getv()
{
    return v;
}

//================================================================================
double Pisano_model::getM()
{
    return M;
}

//================================================================================
double Pisano_model::getkd()
{
    return kd;
}

//================================================================================
double Pisano_model::getxi()
{
    return xi;
}

//================================================================================
double Pisano_model::geth()
{
    return h;
}

//================================================================================
double Pisano_model::getm()
{
    return m;
}

//================================================================================
double Pisano_model::getRho(void)
{
    return rho;
}
//================================================================================
double Pisano_model::getbeta_min(void)
{
    return beta_min;
}
//================================================================================
double Pisano_model::getInitialConfiningStress(void)
{
    return initialconfiningstress;
}
//================================================================================
stresstensor& Pisano_model::getInternalTensor(void)
{
    return alpha;
}

//================================================================================

int Pisano_model::Explicit(const straintensor& strain_incr)
{



    int err = 0;

    BJtensor Ep(4, def_dim_4, 0.0);

    // double lower = 0.0;
    double Delta_lambda = 0.0;

    straintensor mij;

    straintensor incr_strain;
    stresstensor incr_stress;
    straintensor incr_Pstrain;
    stresstensor ep_stress;
    stresstensor predicted_stress;

    stresstensor start_stress;
    stresstensor start_strain;
    straintensor start_Pstrain;

    start_stress  = getStressTensor();
    start_strain  = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();


    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;

    incr_strain = strain_incr;


    ////////////////////////////////////////////
    // Plasticity activated at any load level!!!
    ////////////////////////////////////////////

    double G = E / (2.0 * (1.0 + v));
    double K = E / (3.0 * (1.0 - 2.0 * v));

    double       p = start_stress.p_hydrostatic();
    stresstensor s = start_stress.deviator();



    straintensor incr_strain_dev = incr_strain.deviator();
    double       incr_strain_vol = incr_strain.cval(1, 1) + incr_strain.cval(2, 2) + incr_strain.cval(3, 3);

    // incr_strain.print();
    // incr_strain_dev.print();
    // cout << "incr_strain_vol = " << incr_strain_vol << endl;


    stresstensor stress_incr_prev = start_stress - Stress_n_minus_2;

    //     double stress_incr_prev_norm = sqrt(stress_incr_prev.cval(1,1)*stress_incr_prev.cval(1,1) +
    //                   stress_incr_prev.cval(1,2)*stress_incr_prev.cval(1,2) +
    //                   stress_incr_prev.cval(1,3)*stress_incr_prev.cval(1,3) +
    //                   stress_incr_prev.cval(2,1)*stress_incr_prev.cval(2,1) +
    //                   stress_incr_prev.cval(2,2)*stress_incr_prev.cval(2,2) +
    //                   stress_incr_prev.cval(2,3)*stress_incr_prev.cval(2,3) +
    //                   stress_incr_prev.cval(3,1)*stress_incr_prev.cval(3,1) +
    //                   stress_incr_prev.cval(3,2)*stress_incr_prev.cval(3,2) +
    //                   stress_incr_prev.cval(3,3)*stress_incr_prev.cval(3,3));

    //     double incr_strain_dev_norm = sqrt(incr_strain_dev.cval(1,1)*incr_strain_dev.cval(1,1) +
    //                   incr_strain_dev.cval(1,2)*incr_strain_dev.cval(1,2) +
    //                   incr_strain_dev.cval(1,3)*incr_strain_dev.cval(1,3) +
    //                   incr_strain_dev.cval(2,1)*incr_strain_dev.cval(2,1) +
    //                   incr_strain_dev.cval(2,2)*incr_strain_dev.cval(2,2) +
    //                   incr_strain_dev.cval(2,3)*incr_strain_dev.cval(2,3) +
    //                   incr_strain_dev.cval(3,1)*incr_strain_dev.cval(3,1) +
    //                   incr_strain_dev.cval(3,2)*incr_strain_dev.cval(3,2) +
    //                   incr_strain_dev.cval(3,3)*incr_strain_dev.cval(3,3));

    ///// nij_dev (deviatoric flow)

    double p_incr_prev         = stress_incr_prev.p_hydrostatic(); // incremental pressure at the previous step
    //     stresstensor dev_incr_prev = stress_incr_prev.deviator(); // incremental deviatoric stress at the previous step

    //     double nij_dev_norm;
    //     stresstensor nij_dev_unnorm;



    double pressure_ratio = fabs(p_incr_prev / p);

    if ( (pressure_ratio < 1.0e-3) || (fabs(p) < 1.0) ) // this avoids the influence of very small pressure fluctuations on nij_dev
    {
        p_incr_prev = 0.0;
    }




    //      nij_dev_unnorm = incr_strain_dev*(2.0*G) - alpha*(p_incr_prev*1.0);

    //      nij_dev_norm = sqrt(nij_dev_unnorm.cval(1,1)*nij_dev_unnorm.cval(1,1) +
    //                   nij_dev_unnorm.cval(1,2)*nij_dev_unnorm.cval(1,2) +
    //                   nij_dev_unnorm.cval(1,3)*nij_dev_unnorm.cval(1,3) +
    //                   nij_dev_unnorm.cval(2,1)*nij_dev_unnorm.cval(2,1) +
    //                   nij_dev_unnorm.cval(2,2)*nij_dev_unnorm.cval(2,2) +
    //                   nij_dev_unnorm.cval(2,3)*nij_dev_unnorm.cval(2,3) +
    //                   nij_dev_unnorm.cval(3,1)*nij_dev_unnorm.cval(3,1) +
    //                   nij_dev_unnorm.cval(3,2)*nij_dev_unnorm.cval(3,2) +
    //                   nij_dev_unnorm.cval(3,3)*nij_dev_unnorm.cval(3,3));


    /*
    // && (stress_incr_prev_norm >= 1.0e5*check_for_zero)
      if ((incr_strain_dev_norm >= 1.0e3*check_for_zero) && (nij_dev_norm >= 1.0e3*check_for_zero))
          {
             // cout << "nij ok" <<  endl;
              nij_dev = nij_dev_unnorm*(1.0/nij_dev_norm);
          }

      else

          {
            //   cout << " nij rimedio" <<  endl;
              double sign;

          if (fabs(incr_strain_dev.cval(1,3))>check_for_zero)
             {

               sign = incr_strain_dev.cval(1,3)/fabs(incr_strain_dev.cval(1,3));
             }
          else
             {
              //   cout << "segno imposto" <<  endl;
                sign = 0.0;
             }

               // cout << "incr_strain_dev.cval(1,3)" << incr_strain_dev.cval(1,3) << endl;

                 nij_dev.val(1,1) = 0.0;
                 nij_dev.val(2,2) = 0.0;
                 nij_dev.val(3,3) = 0.0;
                 nij_dev.val(1,2) = 0.0;
                 nij_dev.val(2,1) = 0.0;
                 nij_dev.val(1,3) = sign*0.7071;
                 nij_dev.val(3,1) = sign*0.7071;
                 nij_dev.val(2,3) = 0.0;
                 nij_dev.val(3,2) = 0.0;

      //        nij_dev = nij_dev_prev;
           }

    */

    double sign;

    if (fabs(incr_strain_dev.cval(1, 3)) > check_for_zero)
    {
        sign = incr_strain_dev.cval(1, 3) / fabs(incr_strain_dev.cval(1, 3));
    }
    else
    {
        sign = 0.0;
    }

    //             cout << "sign= " << sign << endl;


    // cout << "incr_strain_dev.cval(1,3)" << incr_strain_dev.cval(1,3) << endl;


    //            static bool firststep=true;

    //     if (!firststep)
    //     {


    nij_dev.val(1, 1) = 0.0;
    nij_dev.val(2, 2) = 0.0;
    nij_dev.val(3, 3) = 0.0;
    nij_dev.val(1, 2) = 0.0;
    nij_dev.val(2, 1) = 0.0;
    nij_dev.val(1, 3) = sign * 0.7071;
    nij_dev.val(3, 1) = sign * 0.7071;
    nij_dev.val(2, 3) = 0.0;
    nij_dev.val(3, 2) = 0.0;

    //     } else
    //     {
    //       firststep = false;
    //     }


    //         cout << "nij_dev: " << nij_dev.val(1,3) << "   " << nij_dev.val(3,1) << endl;

    //         cout << "nij_dev_prev: " << nij_dev_prev.val(1,3) << "   " << nij_dev_prev.val(3,1) << endl;


    /////////////////////////////////////////////////////////////////////////////////////

    // GET HERE THE NEW BETA AND THE PROJECTED BACK STRESS

    double beta = get_distance_coeff(start_stress); // distance coefficient

    // stresstensor alpha_img_dev = alpha + nij_dev*(beta);

    //       cout << "beta: " << beta << endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // UNLOADING CHECK AND UPDATE beta0


    //        double unload_prod = ( nij_dev("ij") * nij_dev_prev("ij") ).trace();

    double unload_prod = nij_dev.cval(1, 1) * nij_dev_prev.cval(1, 1) +
                         nij_dev.cval(1, 2) * nij_dev_prev.cval(1, 2) +
                         nij_dev.cval(1, 3) * nij_dev_prev.cval(1, 3) +
                         nij_dev.cval(2, 1) * nij_dev_prev.cval(2, 1) +
                         nij_dev.cval(2, 2) * nij_dev_prev.cval(2, 2) +
                         nij_dev.cval(2, 3) * nij_dev_prev.cval(2, 3) +
                         nij_dev.cval(3, 1) * nij_dev_prev.cval(3, 1) +
                         nij_dev.cval(3, 2) * nij_dev_prev.cval(3, 2) +
                         nij_dev.cval(3, 3) * nij_dev_prev.cval(3, 3);

    //       cout << "unload prod: " << unload_prod << endl;


    if (unload_prod < 0.0)
    {
        beta0 = beta; // stress tensor at last load reversal
        //        cout << "unloading and beta0 update" << endl;
    }
    else
    {
        beta0 = beta0;
        //  cout << "no unloading and no beta0 update" << beta0 << endl;
    };






    /////////////////////////////////////////////////////////////////////////////////////

    // GET HERE THE PLASTIC MODULUS

    double arg_den = fabs(beta - beta0);

    double arg_den_min =  1.0e3 * check_for_zero;

    double H;



    if (arg_den > 1e-15)
    {
        double arg = beta / arg_den;
        H = p * h * (pow(arg, m));
        //          cout << "H if ... " << H << endl;
        //          cout << "p if ... " << p << endl;
        //          cout << "h if ... " << h << endl;
        //          cout << "m if ... " << m << endl;

    }
    else
    {
        H = 1.0e20;
        //        double arg = beta/arg_den_min;
        //        H = p*h*(pow(arg, m));
        //           cout << "H else ... " << H << endl;

    };





    /////////////////////////////////////////////////////////////////////////////////////

    // GET HERE THE DIRECTION OF THE PLASTIC FLOW



    //nij_dev.print("nij_dev","nij_dev");

    ///// D (dilatancy coefficient)
    stresstensor alpha_d = nij_dev * (kd * sqrt(2.0 / 3.0));

    stresstensor alphad_alpha = alpha_d - alpha;

    //alpha_d.print("alpha_d","alpha_d");

    double alpha_norm = sqrt (alpha.cval(1, 1) * alpha.cval(1, 1) +

                              alpha.cval(1, 2) * alpha.cval(1, 2) +
                              alpha.cval(1, 3) * alpha.cval(1, 3) +
                              alpha.cval(2, 1) * alpha.cval(2, 1) +
                              alpha.cval(2, 2) * alpha.cval(2, 2) +
                              alpha.cval(2, 3) * alpha.cval(2, 3) +
                              alpha.cval(3, 1) * alpha.cval(3, 1) +
                              alpha.cval(3, 2) * alpha.cval(3, 2) +
                              alpha.cval(3, 3) * alpha.cval(3, 3));

    double xi_var = xi * (1.0 - 1.0 * alpha_norm / (M * sqrt(2.0 / 3.0)));

    double D =     (alphad_alpha.cval(1, 1) * nij_dev.cval(1, 1) +
                    alphad_alpha.cval(1, 2) * nij_dev.cval(1, 2) +
                    alphad_alpha.cval(1, 3) * nij_dev.cval(1, 3) +
                    alphad_alpha.cval(2, 1) * nij_dev.cval(2, 1) +
                    alphad_alpha.cval(2, 2) * nij_dev.cval(2, 2) +
                    alphad_alpha.cval(2, 3) * nij_dev.cval(2, 3) +
                    alphad_alpha.cval(3, 1) * nij_dev.cval(3, 1) +
                    alphad_alpha.cval(3, 2) * nij_dev.cval(3, 2) +
                    alphad_alpha.cval(3, 3) * nij_dev.cval(3, 3)) * xi_var;

    //  cout << "D=" << D << endl;

    ///// mij (risulting direction)

    // mij = nij_dev;

    mij.val(1, 1) = nij_dev.cval(1, 1) - 1.0 * D / 3.0;

    mij.val(2, 2) = nij_dev.cval(2, 2) - 1.0 * D / 3.0;

    mij.val(3, 3) = nij_dev.cval(3, 3) - 1.0 * D / 3.0;

    mij.val(1, 2) = nij_dev.cval(1, 2);

    mij.val(1, 3) = nij_dev.cval(1, 3);

    mij.val(2, 1) = nij_dev.cval(2, 1);

    mij.val(2, 3) = nij_dev.cval(2, 3);

    mij.val(3, 1) = nij_dev.cval(3, 1);

    mij.val(3, 2) = nij_dev.cval(3, 2);

    //    mij.print("mij","mij");
    /////////////////////////////////////////////////////////////////////////////////////

    // GET HERE THE PLASTIC MULTIPLIER

    // scalar product incr_strain_dev:nij_dev

    double incr_strain_dev_nijdev = incr_strain_dev.cval(1, 1) * nij_dev.cval(1, 1) +
                                    incr_strain_dev.cval(1, 2) * nij_dev.cval(1, 2) +
                                    incr_strain_dev.cval(1, 3) * nij_dev.cval(1, 3) +
                                    incr_strain_dev.cval(2, 1) * nij_dev.cval(2, 1) +
                                    incr_strain_dev.cval(2, 2) * nij_dev.cval(2, 2) +
                                    incr_strain_dev.cval(2, 3) * nij_dev.cval(2, 3) +
                                    incr_strain_dev.cval(3, 1) * nij_dev.cval(3, 1) +
                                    incr_strain_dev.cval(3, 2) * nij_dev.cval(3, 2) +
                                    incr_strain_dev.cval(3, 3) * nij_dev.cval(3, 3);

    // scalar product alphaij:nij_dev

    double alpha_nijdev = alpha.cval(1, 1) * nij_dev.cval(1, 1) +
                          alpha.cval(1, 2) * nij_dev.cval(1, 2) +
                          alpha.cval(1, 3) * nij_dev.cval(1, 3) +
                          alpha.cval(2, 1) * nij_dev.cval(2, 1) +
                          alpha.cval(2, 2) * nij_dev.cval(2, 2) +
                          alpha.cval(2, 3) * nij_dev.cval(2, 3) +
                          alpha.cval(3, 1) * nij_dev.cval(3, 1) +
                          alpha.cval(3, 2) * nij_dev.cval(3, 2) +
                          alpha.cval(3, 3) * nij_dev.cval(3, 3);

    // Plastic muliplier;
    double num =  (2.0 * G * incr_strain_dev_nijdev + K * alpha_nijdev * incr_strain_vol);

    double den =   2.0 * G + (2.0 / 3.0) * H - K * D * alpha_nijdev;

    //     cout << "den= " <<  den << endl;



    Delta_lambda = num / den;

    if (Delta_lambda < 0.0)
    {
        Delta_lambda = 0.0;
        //cout << "negative plastic multiplier. Delta_lambda=" <<  Delta_lambda << endl;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    // UPDATE HERE ALL VARIABLES

    ///////////////////////////////////////////////////////////////////
    // Plastic strain increment

    //    incr_Pstrain = mij * Delta_lambda;
    incr_Pstrain.val(1, 1) = mij.cval(1, 1) * (Delta_lambda);
    incr_Pstrain.val(1, 2) = mij.cval(1, 2) * (Delta_lambda);
    incr_Pstrain.val(1, 3) = mij.cval(1, 3) * (Delta_lambda);
    incr_Pstrain.val(2, 1) = mij.cval(2, 1) * (Delta_lambda);
    incr_Pstrain.val(2, 2) = mij.cval(2, 2) * (Delta_lambda);
    incr_Pstrain.val(2, 3) = mij.cval(2, 3) * (Delta_lambda);
    incr_Pstrain.val(3, 1) = mij.cval(3, 1) * (Delta_lambda);
    incr_Pstrain.val(3, 2) = mij.cval(3, 2) * (Delta_lambda);
    incr_Pstrain.val(3, 3) = mij.cval(3, 3) * (Delta_lambda);


    /////////////////////////////////////////////////////////////////////////
    //   straintensor  incr_Estrain = incr_strain - incr_Pstrain;

    //   incr_stress = Ee("ijpq") * (incr_Estrain("pq"));
    //    incr_stress.null_indices();

    incr_stress.val(1, 1) = Ee.cval(1, 1, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(1, 1, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(1, 1, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(1, 1, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(1, 1, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(1, 1, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(1, 1, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(1, 1, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(1, 1, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));

    incr_stress.val(1, 2) = Ee.cval(1, 2, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(1, 2, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(1, 2, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(1, 2, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(1, 2, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(1, 2, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(1, 2, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(1, 2, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(1, 2, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(1, 3) = Ee.cval(1, 3, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(1, 3, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(1, 3, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(1, 3, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(1, 3, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(1, 3, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(1, 3, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(1, 3, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(1, 3, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(2, 1) = Ee.cval(2, 1, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(2, 1, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(2, 1, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(2, 1, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(2, 1, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(2, 1, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(2, 1, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(2, 1, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(2, 1, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));



    incr_stress.val(2, 2) = Ee.cval(2, 2, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(2, 2, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(2, 2, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(2, 2, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(2, 2, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(2, 2, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(2, 2, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(2, 2, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(2, 2, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(2, 3) = Ee.cval(2, 3, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(2, 3, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(2, 3, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(2, 3, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(2, 3, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(2, 3, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(2, 3, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(2, 3, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(2, 3, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(3, 1) = Ee.cval(3, 1, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(3, 1, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(3, 1, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(3, 1, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(3, 1, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(3, 1, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(3, 1, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(3, 1, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(3, 1, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(3, 2) =  Ee.cval(3, 2, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                             Ee.cval(3, 2, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                             Ee.cval(3, 2, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                             Ee.cval(3, 2, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                             Ee.cval(3, 2, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                             Ee.cval(3, 2, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                             Ee.cval(3, 2, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                             Ee.cval(3, 2, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                             Ee.cval(3, 2, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));


    incr_stress.val(3, 3) = Ee.cval(3, 3, 1, 1) * (incr_strain.cval(1, 1) - incr_Pstrain.cval(1, 1)) +
                            Ee.cval(3, 3, 1, 2) * (incr_strain.cval(1, 2) - incr_Pstrain.cval(1, 2)) +
                            Ee.cval(3, 3, 1, 3) * (incr_strain.cval(1, 3) - incr_Pstrain.cval(1, 3)) +
                            Ee.cval(3, 3, 2, 1) * (incr_strain.cval(2, 1) - incr_Pstrain.cval(2, 1)) +
                            Ee.cval(3, 3, 2, 2) * (incr_strain.cval(2, 2) - incr_Pstrain.cval(2, 2)) +
                            Ee.cval(3, 3, 2, 3) * (incr_strain.cval(2, 3) - incr_Pstrain.cval(2, 3)) +
                            Ee.cval(3, 3, 3, 1) * (incr_strain.cval(3, 1) - incr_Pstrain.cval(3, 1)) +
                            Ee.cval(3, 3, 3, 2) * (incr_strain.cval(3, 2) - incr_Pstrain.cval(3, 2)) +
                            Ee.cval(3, 3, 3, 3) * (incr_strain.cval(3, 3) - incr_Pstrain.cval(3, 3));

    /////////////////////////////////////////////////////////////////////////

    ep_stress = start_stress + incr_stress;



    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Update all variables

    TrialStress = ep_stress;              // try again Initiailize whenever you have tensor = tensor...
    TrialStrain = start_strain + incr_strain;
    TrialPlastic_Strain = start_Pstrain + incr_Pstrain;

    stresstensor  ep_stress_dev = ep_stress.deviator();
    double        ep_stress_p   = ep_stress.p_hydrostatic();

    if (ep_stress_p > 1000 * check_for_zero)
    {
        alpha = ep_stress_dev * (1.0 / ep_stress_p);

    }
    else
    {
        //      cout << "too small pressure..compute alpha otherwise!" << endl;
        alpha.val(1, 1) = 0.0;
        alpha.val(2, 2) = 0.0;
        alpha.val(3, 3) = 0.0;
        alpha.val(1, 2) = 0.0;
        alpha.val(2, 1) = 0.0;
        alpha.val(1, 3) = 0.0;
        alpha.val(3, 1) = 0.0;
        alpha.val(2, 3) = 0.0;
        alpha.val(3, 2) = 0.0;
        //         alpha.print("alpha","alpha");
    };



    /////////////////////////////////////////////////////////////////////////////////////

    // GET HERE THE ELASTIC-PLASTIC STIFFNESS TENSOR

    // To obtain Eep, at the last step

    tensor I2("I", 2, def_dim_2);


    //     cout << "alpha_nijdev= " << alpha_nijdev << endl;

    //        if ( Delta_lambda > 0.0 )
    //         {


    Stiffness =    nij_dev("pq") * nij_dev("mn") * (-4.0 * G * G / den) +
                   I2("pq") * nij_dev("mn") * (2.0 * G * K * D / den) +
                   nij_dev("pq") * I2("mn") * (-2.0 * G * K * alpha_nijdev / den) +
                   I2("pq") * I2("mn") * (K - (2.0 / 3.0) * G + K * K * D * alpha_nijdev / den);


    Stiffness.val(1, 1, 1, 1) = Stiffness.cval(1, 1, 1, 1) + (2.0 * G);

    Stiffness.val(1, 2, 1, 2) = Stiffness.cval(1, 2, 1, 2) + (2.0 * G);

    Stiffness.val(1, 3, 1, 3) = Stiffness.cval(1, 3, 1, 3) + (2.0 * G);

    Stiffness.val(2, 1, 2, 1) = Stiffness.cval(2, 1, 2, 1) + (2.0 * G);

    Stiffness.val(2, 2, 2, 2) = Stiffness.cval(2, 2, 2, 2) + (2.0 * G);

    Stiffness.val(2, 3, 2, 3) = Stiffness.cval(2, 3, 2, 3) + (2.0 * G);

    Stiffness.val(3, 1, 3, 1) = Stiffness.cval(3, 1, 3, 1) + (2.0 * G);

    Stiffness.val(3, 2, 3, 2) = Stiffness.cval(3, 2, 3, 2) + (2.0 * G);

    Stiffness.val(3, 3, 3, 3) = Stiffness.cval(3, 3, 3, 3) + (2.0 * G);

    // Stiffness.print("");

    nij_dev_prev.Initialize(nij_dev);  // keeps track of the previous nij_dev tensor


    return err;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// AUXILIARY FUNCTIONS

double Pisano_model::get_distance_coeff(stresstensor& start_stress)
{

    double nij_dev_norm = sqrt (nij_dev.cval(1, 1) * nij_dev.cval(1, 1) +
                                nij_dev.cval(1, 2) * nij_dev.cval(1, 2) +
                                nij_dev.cval(1, 3) * nij_dev.cval(1, 3) +
                                nij_dev.cval(2, 1) * nij_dev.cval(2, 1) +
                                nij_dev.cval(2, 2) * nij_dev.cval(2, 2) +
                                nij_dev.cval(2, 3) * nij_dev.cval(2, 3) +
                                nij_dev.cval(3, 1) * nij_dev.cval(3, 1) +
                                nij_dev.cval(3, 2) * nij_dev.cval(3, 2) +
                                nij_dev.cval(3, 3) * nij_dev.cval(3, 3));

    if (nij_dev_norm > 1.0e3 * check_for_zero)
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

        double alpha_nijdev = alpha.cval(1, 1) * nij_dev.cval(1, 1) +
                              alpha.cval(1, 2) * nij_dev.cval(1, 2) +
                              alpha.cval(1, 3) * nij_dev.cval(1, 3) +
                              alpha.cval(2, 1) * nij_dev.cval(2, 1) +
                              alpha.cval(2, 2) * nij_dev.cval(2, 2) +
                              alpha.cval(2, 3) * nij_dev.cval(2, 3) +
                              alpha.cval(3, 1) * nij_dev.cval(3, 1) +
                              alpha.cval(3, 2) * nij_dev.cval(3, 2) +
                              alpha.cval(3, 3) * nij_dev.cval(3, 3);

        b = 2.0 * alpha_nijdev;

        //// c

        double alpha_norm = sqrt (alpha.cval(1, 1) * alpha.cval(1, 1) +
                                  alpha.cval(1, 2) * alpha.cval(1, 2) +
                                  alpha.cval(1, 3) * alpha.cval(1, 3) +
                                  alpha.cval(2, 1) * alpha.cval(2, 1) +
                                  alpha.cval(2, 2) * alpha.cval(2, 2) +
                                  alpha.cval(2, 3) * alpha.cval(2, 3) +
                                  alpha.cval(3, 1) * alpha.cval(3, 1) +
                                  alpha.cval(3, 2) * alpha.cval(3, 2) +
                                  alpha.cval(3, 3) * alpha.cval(3, 3));


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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//=================================================================================
// For Matrix based elements
const Matrix& Pisano_model::getTangent (void)
{
    D(0, 0) = Stiffness.cval(1, 1, 1, 1);
    D(0, 1) = Stiffness.cval(1, 1, 2, 2);
    D(0, 2) = Stiffness.cval(1, 1, 3, 3);
    D(0, 3) = Stiffness.cval(1, 1, 1, 2) * 0.5;
    D(0, 4) = Stiffness.cval(1, 1, 1, 3) * 0.5;
    D(0, 5) = Stiffness.cval(1, 1, 2, 3) * 0.5;

    D(1, 0) = Stiffness.cval(2, 2, 1, 1);
    D(1, 1) = Stiffness.cval(2, 2, 2, 2);
    D(1, 2) = Stiffness.cval(2, 2, 3, 3);
    D(1, 3) = Stiffness.cval(2, 2, 1, 2) * 0.5;
    D(1, 4) = Stiffness.cval(2, 2, 1, 3) * 0.5;
    D(1, 5) = Stiffness.cval(2, 2, 2, 3) * 0.5;

    D(2, 0) = Stiffness.cval(3, 3, 1, 1);
    D(2, 1) = Stiffness.cval(3, 3, 2, 2);
    D(2, 2) = Stiffness.cval(3, 3, 3, 3);
    D(2, 3) = Stiffness.cval(3, 3, 1, 2) * 0.5;
    D(2, 4) = Stiffness.cval(3, 3, 1, 3) * 0.5;
    D(2, 5) = Stiffness.cval(3, 3, 2, 3) * 0.5;

    D(3, 0) = Stiffness.cval(1, 2, 1, 1);
    D(3, 1) = Stiffness.cval(1, 2, 2, 2);
    D(3, 2) = Stiffness.cval(1, 2, 3, 3);
    D(3, 3) = Stiffness.cval(1, 2, 1, 2) * 0.5;
    D(3, 4) = Stiffness.cval(1, 2, 1, 3) * 0.5;
    D(3, 5) = Stiffness.cval(1, 2, 2, 3) * 0.5;

    D(4, 0) = Stiffness.cval(1, 3, 1, 1);
    D(4, 1) = Stiffness.cval(1, 3, 2, 2);
    D(4, 2) = Stiffness.cval(1, 3, 3, 3);
    D(4, 3) = Stiffness.cval(1, 3, 1, 2) * 0.5;
    D(4, 4) = Stiffness.cval(1, 3, 1, 3) * 0.5;
    D(4, 5) = Stiffness.cval(1, 3, 2, 3) * 0.5;

    D(5, 0) = Stiffness.cval(2, 3, 1, 1);
    D(5, 1) = Stiffness.cval(2, 3, 2, 2);
    D(5, 2) = Stiffness.cval(2, 3, 3, 3);
    D(5, 3) = Stiffness.cval(2, 3, 1, 2) * 0.5;
    D(5, 4) = Stiffness.cval(2, 3, 1, 3) * 0.5;
    D(5, 5) = Stiffness.cval(2, 3, 2, 3) * 0.5;

    return D;
}



#endif

