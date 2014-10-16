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
//
/////////////////////////////////////////////////////////////////////////////

// vonMisesIH is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.

#ifndef vonMisesIH_CPP
#define vonMisesIH_CPP

#include "vonMisesIH.h"


const  straintensor vonMisesIH::ZeroStrain;
const  stresstensor vonMisesIH::ZeroStress;
BJtensor vonMisesIH::Ee(4, def_dim_4, 0.0);

// Constructor
//================================================================================
vonMisesIH::vonMisesIH(int tag,
                       double E_in,
                       double v_in,
                       double k_in,
                       double H_in,
                       double rho_in,
                       double initialconfiningstress_in,
                       int maximum_number_of_iterations,
                       double tolerance_1,
                       double tolerance_2)
    : NDMaterial(tag, ND_TAG_vonMisesIH),
      E(E_in),
      v(v_in),
      k(k_in),
      H(H_in),
      rho(rho_in),
      initialconfiningstress(initialconfiningstress_in),
      TOLERANCE1(tolerance_1), TOLERANCE2(tolerance_2),
      ISMAX(maximum_number_of_iterations)
{

    stresstensor initialStress;
    initialStress.val(1, 1) = -initialconfiningstress;
    initialStress.val(2, 2) = -initialconfiningstress;
    initialStress.val(3, 3) = -initialconfiningstress;

    ElasticStateStress.Initialize(initialStress);


    this->revertToStart();
}


// Constructor
//================================================================================
vonMisesIH::vonMisesIH()
    : NDMaterial(0, ND_TAG_vonMisesIH)
{
    this->revertToStart();
}

// Destructor
//================================================================================
vonMisesIH::~vonMisesIH()
{
}

//================================================================================
int vonMisesIH::setTrialStrain(const Tensor& v)
{
    return this->setTrialStrainIncr( v - getStrainTensor() );
}

//================================================================================
int vonMisesIH::setTrialStrainIncr(const Tensor& strain_increment)
{
    return this->Explicit(strain_increment);
}

//================================================================================
const BJtensor& vonMisesIH::getTangentTensor(void)
{
    return this->Stiffness;
}

//================================================================================
const stresstensor&  vonMisesIH::getStressTensor(void)
{
    return this->TrialStress;
}


//================================================================================
const straintensor& vonMisesIH::getStrainTensor(void)
{
    return this->TrialStrain;
}

//================================================================================
const straintensor& vonMisesIH::getPlasticStrainTensor(void)
{
    return this->TrialPlastic_Strain;
}


//================================================================================
int vonMisesIH::commitState(void)
{
    CommitStress.Initialize(TrialStress);
    CommitStrain.Initialize(TrialStrain);
    CommitPlastic_Strain.Initialize(TrialPlastic_Strain);

    return 0;
}

//================================================================================
int vonMisesIH::revertToLastCommit(void)
{
    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(CommitPlastic_Strain);

    return 0;
}

//================================================================================
int vonMisesIH::revertToStart(void)
{

    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain.Initialize(ZeroStrain);

    TrialStress.Initialize(CommitStress);
    TrialStrain.Initialize(CommitStrain);
    TrialPlastic_Strain.Initialize(ZeroStrain);


    double lambda = (v * E) / ((1 + v) * (1 - 2 * v));
    double mu = E / (2 * (1 + v));

    Ee.cval(1, 1, 1, 1) = lambda + 2 * mu;
    Ee.cval(1, 1, 2, 2) = lambda;
    Ee.cval(1, 1, 3, 3) = lambda;
    Ee.cval(1, 2, 1, 2) = mu;
    Ee.cval(1, 2, 2, 1) = mu;
    Ee.cval(1, 3, 1, 3) = mu;
    Ee.cval(1, 3, 3, 1) = mu;
    Ee.cval(2, 1, 1, 2) = mu;
    Ee.cval(2, 1, 2, 1) = mu;
    Ee.cval(2, 2, 1, 1) = lambda;
    Ee.cval(2, 2, 2, 2) = lambda + 2 * mu;
    Ee.cval(2, 2, 3, 3) = lambda;
    Ee.cval(2, 3, 2, 3) = mu;
    Ee.cval(2, 3, 3, 2) = mu;
    Ee.cval(3, 1, 1, 3) = mu;
    Ee.cval(3, 1, 3, 1) = mu;
    Ee.cval(3, 2, 2, 3) = mu;
    Ee.cval(3, 2, 3, 2) = mu;
    Ee.cval(3, 3, 1, 1) = lambda;
    Ee.cval(3, 3, 2, 2) = lambda;
    Ee.cval(3, 3, 3, 3) = lambda + 2 * mu;

    Stiffness = Ee;

    return 0;
}

//================================================================================
NDMaterial* vonMisesIH::getCopy(void)
{
    NDMaterial* tmp = new vonMisesIH(this->getTag(),
                                     this->getE(),
                                     this->getv(),
                                     this->getk(),
                                     this->getH(),
                                     this->getRho(),
                                     this->getInitialConfiningStress(),
                                     this->ISMAX,
                                     this->TOLERANCE1,
                                     this->TOLERANCE2);

    return tmp;
}


//================================================================================
NDMaterial* vonMisesIH::getCopy(const char* code)
{
    if (strcmp(code, "ThreeDimensional") == 0)
    {
        vonMisesIH* tmp = new vonMisesIH( this->getTag(),
                                          this->getE(),
                                          this->getv(),
                                          this->getk(),
                                          this->getH(),
                                          this->getRho(),
                                          this->getInitialConfiningStress(),
                                          this->ISMAX,
                                          this->TOLERANCE1,
                                          this->TOLERANCE2);

        return tmp;
    }
    else
    {
        cout << "vonMisesIH::getCopy failed to get model: " <<  code << endln;
        exit(1);
    }

    return 0;
}

//================================================================================
const char* vonMisesIH::getType(void) const
{
    return "ThreeDimensional";
}

//================================================================================
int vonMisesIH::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

//================================================================================
int vonMisesIH::receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    return 0;
}

//================================================================================
void vonMisesIH::Print(ostream& s, int flag)
{
    s << (*this);
}

//================================================================================
double vonMisesIH::getE()
{
    return E;
}

//================================================================================
double vonMisesIH::getv()
{
    return v;
}

//================================================================================
double vonMisesIH::getH()
{
    return H;
}

//================================================================================
double vonMisesIH::getRho(void)
{
    return rho;
}

//================================================================================
double vonMisesIH::getInitialConfiningStress(void)
{
    return initialconfiningstress;
}

//================================================================================
double vonMisesIH::getk(void)
{
    return k;
}

//================================================================================
double vonMisesIH::YieldFunctionValue( const stresstensor& Stre) const
{

    double sqrt23 = sqrt(2.0 / 3.0);

    stresstensor s_bar = Stre;
    s_bar = s_bar.deviator();
    //  double s_norm  = sqrt( (s_bar("ij")*s_bar("ij")).trace() );

    double sijsijtrace = s_bar.cval(1, 1) * s_bar.cval(1, 1) +
                         s_bar.cval(1, 2) * s_bar.cval(1, 2) +
                         s_bar.cval(1, 3) * s_bar.cval(1, 3) +
                         s_bar.cval(2, 1) * s_bar.cval(2, 1) +
                         s_bar.cval(2, 2) * s_bar.cval(2, 2) +
                         s_bar.cval(2, 3) * s_bar.cval(2, 3) +
                         s_bar.cval(3, 1) * s_bar.cval(3, 1) +
                         s_bar.cval(3, 2) * s_bar.cval(3, 2) +
                         s_bar.cval(3, 3) * s_bar.cval(3, 3);

    double s_norm  = sqrt( sijsijtrace );

    double YF = s_norm - sqrt23 * k;
    return YF;
}

//================================================================================
int vonMisesIH::Explicit(const straintensor& strain_incr)
{
    int err = 0;
    double f_start = 0.0;
    double f_pred  = 0.0;

    BJtensor Ep(4, def_dim_4, 0.0);

    straintensor intersection_strain;
    stresstensor intersection_stress;
    double intersection_factor = 0.0;

    double lower = 0.0;
    double Delta_lambda = 0.0;
    double hardMod  = 0.0;
    stresstensor nij;
    straintensor mij;
    BJtensor Hq(2, def_dim_2, 0.0);

    straintensor incr_strain;
    stresstensor incr_stress;
    straintensor incr_Pstrain;
    stresstensor ep_stress;
    stresstensor predicted_stress;

    stresstensor start_stress;
    stresstensor start_strain;
    straintensor start_Pstrain;

    start_stress = getStressTensor();
    start_strain = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();

    intersection_stress.Initialize(start_stress);

    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;

    incr_strain = strain_incr;

    /////////////////////////////////////////////////////////////////////////
    //    incr_stress = Ee("ijpq") * incr_strain("pq");
    //    incr_stress.null_indices();

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

            incr_stress = predicted_stress - intersection_stress;  // necessary


            ElasticStateStrain = intersection_strain;
            ElasticStateStress = intersection_stress;

        }


        // If E-P Response,
        Delta_lambda = 0.0;


        ///////////////////////////////////////////////////////////////////////////////////
        // f2 = [(sij-aij)*(sij-aij)]^0.5 - sqrt(2.0/3)*k

        stresstensor s_bar = intersection_stress;
        s_bar = s_bar.deviator();

        //  double s_norm  = sqrt( (s_bar("ij")*s_bar("ij")).trace() );
        double sijsijtrace = s_bar.cval(1, 1) * s_bar.cval(1, 1) +
                             s_bar.cval(1, 2) * s_bar.cval(1, 2) +
                             s_bar.cval(1, 3) * s_bar.cval(1, 3) +
                             s_bar.cval(2, 1) * s_bar.cval(2, 1) +
                             s_bar.cval(2, 2) * s_bar.cval(2, 2) +
                             s_bar.cval(2, 3) * s_bar.cval(2, 3) +
                             s_bar.cval(3, 1) * s_bar.cval(3, 1) +
                             s_bar.cval(3, 2) * s_bar.cval(3, 2) +
                             s_bar.cval(3, 3) * s_bar.cval(3, 3);

        double s_norm  = sqrt( sijsijtrace );


        if (s_norm != 0.0)
        {
            double normtemp = 1.0 / s_norm;
            nij.val(1, 1) = s_bar.cval(1, 1) * normtemp;
            nij.val(1, 2) = s_bar.cval(1, 2) * normtemp;
            nij.val(1, 3) = s_bar.cval(1, 3) * normtemp;
            nij.val(2, 1) = s_bar.cval(2, 1) * normtemp;
            nij.val(2, 2) = s_bar.cval(2, 2) * normtemp;
            nij.val(2, 3) = s_bar.cval(2, 3) * normtemp;
            nij.val(3, 1) = s_bar.cval(3, 1) * normtemp;
            nij.val(3, 2) = s_bar.cval(3, 2) * normtemp;
            nij.val(3, 3) = s_bar.cval(3, 3) * normtemp;

        }
        else if (s_norm < 0)
        {
            fprintf(stderr, "ERROR stresstensor& vonMisesIH::StressDerivative(  s_norm < 0 ");
            exit(1);
        }


        ///////////////////////////////////////////////////////////////////////////////////

        mij = nij;

        Hq = Ee("ijkl") * nij("kl");
        Hq.null_indices();


        //      upper = (nij("ij") * incr_stress("ij")).trace();
        double upper = nij.cval(1, 1) * incr_stress.cval(1, 1) +
                       nij.cval(1, 2) * incr_stress.cval(1, 2) +
                       nij.cval(1, 3) * incr_stress.cval(1, 3) +
                       nij.cval(2, 1) * incr_stress.cval(2, 1) +
                       nij.cval(2, 2) * incr_stress.cval(2, 2) +
                       nij.cval(2, 3) * incr_stress.cval(2, 3) +
                       nij.cval(3, 1) * incr_stress.cval(3, 1) +
                       nij.cval(3, 2) * incr_stress.cval(3, 2) +
                       nij.cval(3, 3) * incr_stress.cval(3, 3);


        lower = ( Hq("ij") * mij("ij") ).trace();

        hardMod  = 0.0;

        ///////////////////////////////////////////////////////////////////
        // Evolution of tensor (kinematic) internal variables in yield function
        double h_s = mij.equivalent() * H;
        double xi_s = -sqrt(2.0 / 3.0);
        hardMod = h_s * xi_s;
        ///////////////////////////////////////////////////////////////////

        lower -= hardMod;

        Delta_lambda = upper / lower;

        if (Delta_lambda < 0.0)
        {
            Delta_lambda = 0.0;
        }


        ///////////////////////////////////////////////////////////////////
        // Plastic strain increment
        //      incr_Pstrain = nij * Delta_lambda;
        incr_Pstrain.val(1, 1) = nij.cval(1, 1) * Delta_lambda;
        incr_Pstrain.val(1, 2) = nij.cval(1, 2) * Delta_lambda;
        incr_Pstrain.val(1, 3) = nij.cval(1, 3) * Delta_lambda;
        incr_Pstrain.val(2, 1) = nij.cval(2, 1) * Delta_lambda;
        incr_Pstrain.val(2, 2) = nij.cval(2, 2) * Delta_lambda;
        incr_Pstrain.val(2, 3) = nij.cval(2, 3) * Delta_lambda;
        incr_Pstrain.val(3, 1) = nij.cval(3, 1) * Delta_lambda;
        incr_Pstrain.val(3, 2) = nij.cval(3, 2) * Delta_lambda;
        incr_Pstrain.val(3, 3) = nij.cval(3, 3) * Delta_lambda;


        ep_stress = predicted_stress - (Hq * Delta_lambda);

        TrialStress.Initialize(ep_stress);
        TrialStrain = start_strain + incr_strain;
        TrialPlastic_Strain = start_Pstrain + incr_Pstrain;

        /////////////////////////////////////////////////////
        // Update internal tensor variables
        double dk = h_s * Delta_lambda;
        ///////////////////////////////////////////////////////////////////

        k += dk;


        Ep = Hq("pq") * Hq("mn");
        Ep.null_indices();
        Ep = Ep * (1.0 / lower);


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
stresstensor vonMisesIH::yield_surface_cross(const stresstensor& start_stress,
        const stresstensor& end_stress, double a)
{
    stresstensor delta_stress = end_stress - start_stress;
    stresstensor intersection_stress = start_stress + delta_stress * a;

    return intersection_stress;
}

// Routine used by yield_surface_cross to find the stresstensor at cross point
//================================================================================
double vonMisesIH::zbrentstress(const stresstensor& start_stress,
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



#endif

