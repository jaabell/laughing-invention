///////////////////////////////////////////////////////////////////////////////
// LEGACY/DEFUNCT COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):  Version of Creative Commons (for details contact Boris Jeremic jeremic@ucdavis.edu)
// PROJECT:        Real ESSI Simulator
// PROGRAMMER:     Jose Abell, Federico Pisano', Nima Tafazzoli, Boris Jeremic
// DATE:           December 2013
// UPDATE HISTORY: Full update history in git repository.
//
/////////////////////////////////////////////////////////////////////////////


// #define DEBUG_PISANO 1

#ifndef PisanoLT_CPP
#define PisanoLT_CPP

#include "NewPisanoLT.h"
#include <limits>
#include "LTensorDisplay.h"
#include <Channel.h>
// #include <HDF5_Channel.h>
#include <Matrix.h>
#include <Vector.h>

#include <fstream>


constexpr double sqrt_two_three = sqrt(2.0 / 3.0);

const double NewPisanoLT:: check_for_zero = sqrt(std::numeric_limits<double>::epsilon()); // used to check the variable nullity
const double NewPisanoLT::beta_min = 1e-16;
const double NewPisanoLT::beta_max = 1e+16;
const double NewPisanoLT::patm = 1.013e5;   // 1 Atmosphere of pressure [Pa]
DTensor2 NewPisanoLT::kronecker_delta(3, 3, 0.0);                 // kronecker_delta is the identity matrix


// DTensor4 NewPisanoLT::Ee(3, 3, 3, 3, 0.0);


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
                         double a_in,
                         double eplcum_cr_in)
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
    eplcum_cr(eplcum_cr_in),
    eplcum(0.0),
    strainplcum(3, 3, 0.0),
    TrialStrain( 3, 3, 0.0 ),
    TrialStress( 3, 3, 0.0 ),
    TrialPlastic_Strain( 3, 3, 0.0 ),
    CommitStress( 3, 3, 0.0 ),
    CommitStrain( 3, 3, 0.0 ),
    CommitPlastic_Strain( 3, 3, 0.0 ),
    Trial_alpha( 3, 3, 0.0 ),
    Trial_alpha0( 3, 3, 0.0 ),
    Trial_alpha0mem( 3, 3, 0.0 ),
    Trial_nij_dev( 3, 3, 0.0 ),
    Commit_alpha( 3, 3, 0.0 ),
    Commit_alpha0( 3, 3, 0.0 ),
    Commit_alpha0mem( 3, 3, 0.0 ),
    Commit_nij_dev( 3, 3, 0.0 ),
    Stiffness( 3, 3, 3, 3, 0.0 ),
    Ee(3, 3, 3, 3, 0.0),
    use_alternative_flow_rule(false)
{

  //Initialize identity matrix
  kronecker_delta(0, 0) = 1;
  kronecker_delta(1, 1) = 1;
  kronecker_delta(2, 2) = 1;

  double K0 = v / (1 - v);
  double sigma_z = -3 * initialconfiningstress / (2 * K0 + 1);

  // TrialStress(0, 0) = -initialconfiningstress ;
  // TrialStress(1, 1) = -initialconfiningstress ;
  // TrialStress(2, 2) = -initialconfiningstress ;

  TrialStress(0, 0) = K0 * sigma_z;
  TrialStress(1, 1) = K0 * sigma_z;
  TrialStress(2, 2) = sigma_z;
//
  CommitStress(0, 0) = TrialStress(0, 0);
  CommitStress(1, 1) = TrialStress(1, 1);
  CommitStress(2, 2) = TrialStress(2, 2);

  // Compute alpha consistent with this initial state
  DTensor2 TrialStress_dev(3, 3, 0);
  double p_plastic;
  TrialStress.compute_deviatoric_tensor(TrialStress_dev, p_plastic);
  p_plastic = -TrialStress(i, i) / 3;
  if (p_plastic > 0)
  {
    Trial_alpha(i, j) = TrialStress_dev(i, j);
    Trial_alpha /= p_plastic;
  }
  else
  {
    Trial_alpha *= 0;
  }


  Trial_nij_dev(0, 0) = 0.40825;
  Trial_nij_dev(1, 1) = 0.40825;
  Trial_nij_dev(2, 2) = -0.8165;

  Commit_nij_dev(0, 0) = 0.40825;
  Commit_nij_dev(1, 1) = 0.40825;
  Commit_nij_dev(2, 2) = -0.8165;

  this->revertToStart();

  //A negative kd means use the alternative flow rule. This should remain an undocumented feature
  // since the alternative flow rule is for demonstration purposes only and is not physical.
  if (kd < 0)
  {
    cout << "NewPisanoLT::NewPisanoLT() -- Warning, using alternative flow rule. This is not meant for actual analysis.\n";
    use_alternative_flow_rule = true;
    kd = -kd;
  }

}


NewPisanoLT::NewPisanoLT()
  : NDMaterialLT(0, ND_TAG_NewPisanoLT),
    E0(0.0),
    v(0.0), M(0.0),
    kd(0.0),
    xi(0.0),
    h(0.0), m(0.0),
    rho(0.0),
    initialconfiningstress(0.0),
    n(0.0),
    a(0.0),
    eplcum_cr(0.0),
    eplcum(0.0),
    strainplcum(3, 3, 0.0),
    TrialStrain( 3, 3, 0.0 ),
    TrialStress( 3, 3, 0.0 ),
    TrialPlastic_Strain( 3, 3, 0.0 ),
    CommitStress( 3, 3, 0.0 ),
    CommitStrain( 3, 3, 0.0 ),
    CommitPlastic_Strain( 3, 3, 0.0 ),
    Trial_alpha( 3, 3, 0.0 ),
    Trial_alpha0( 3, 3, 0.0 ),
    Trial_alpha0mem( 3, 3, 0.0 ),
    Trial_nij_dev( 3, 3, 0.0 ),
    Commit_alpha( 3, 3, 0.0 ),
    Commit_alpha0( 3, 3, 0.0 ),
    Commit_alpha0mem( 3, 3, 0.0 ),
    Commit_nij_dev( 3, 3, 0.0 ),
    Stiffness( 3, 3, 3, 3, 0.0 ),
    Ee(3, 3, 3, 3, 0.0),
    use_alternative_flow_rule(false)
{

  //Initialize identity matrix
  kronecker_delta(0, 0) = 1;
  kronecker_delta(1, 1) = 1;
  kronecker_delta(2, 2) = 1;
  // this->revertToStart();
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
  TrialStrain(i, j) = v(i, j);
  result( i, j ) = v( i, j ) - CommitStrain( i, j );

  return setTrialStrainIncr( result );
}


int NewPisanoLT::setTrialStrainIncr(const DTensor2 &strain_increment)
{
  int exitflag = 0;

  //Check for quick return:
  double eps_norm = strain_increment(i, j) * strain_increment(i, j);
  if (eps_norm == 0)
  {
    return exitflag;
  }

  switch (this->constitutive_integration_method)
  {
  case NDMaterialLT_Constitutive_Integration_Method::Not_Set :
    exitflag = -1;
    cerr << "NewPisanoLT::setTrialStrainIncr - Integration method not set!\n" ;
    break;
  case NDMaterialLT_Constitutive_Integration_Method::Euler_One_Step :
    exitflag = this->Euler_One_Step(strain_increment);
    break;
  case NDMaterialLT_Constitutive_Integration_Method::Euler_Multistep :
    // exitflag = this->Euler_Multistep(strain_increment);
    break;
  case NDMaterialLT_Constitutive_Integration_Method::Modified_Euler_Error_Control :
    exitflag = this->Modified_Euler_Error_Control(strain_increment);
    break;
  case NDMaterialLT_Constitutive_Integration_Method::Runge_Kutta_45_Error_Control :
    // exitflag = this->Runge_Kutta_45_Error_Control(strain_increment);;
    break;
  default:
    cerr << "NewPisanoLT::setTrialStrainIncr - Integration method not available!\n" ;
    exitflag = -1;
  }

  return exitflag;
}


const DTensor4 &NewPisanoLT::getTangentTensor(void)
{

  //---------------------------------------------------------------------------------------------
  // Compute Elastic-Plastic stiffness
  //---------------------------------------------------------------------------------------------
  // To obtain Eep, at the last step
  Index<'p'> p;
  Index<'q'> q;
  double E = getE();
  double G = E / (2.0 * (1.0 + v));
  double K = E / (3.0 * (1.0 - 2.0 * v));
  double alpha_nijdev = Trial_alpha(i, j) * Trial_nij_dev(i, j);                          // scalar product alphaij:nij_dev
  double H = getH(Trial_alpha0);
  double D = getD();
  double den =   2.0 * G + (2.0 / 3.0) * H - K * D * alpha_nijdev;

  Stiffness(p, q, i, j) =  Trial_nij_dev(i, j) * Trial_nij_dev(p, q) * (-4.0 * G * G / den) +
                           kronecker_delta(i, j)      * Trial_nij_dev(p, q) * (2.0 * G * K * D / den) +
                           Trial_nij_dev(i, j) * kronecker_delta(p, q)      * (-2.0 * G * K * alpha_nijdev / den) +
                           kronecker_delta(i, j)      * kronecker_delta(p, q)      * (K - (2.0 / 3.0) * G + K * K * D * alpha_nijdev / den);

  Stiffness(0, 0, 0, 0) = Stiffness(0, 0, 0, 0) + (2.0 * G);

  Stiffness(0, 1, 0, 1) = Stiffness(0, 1, 0, 1) + (2.0 * G);

  Stiffness(0, 2, 0, 2) = Stiffness(0, 2, 0, 2) + (2.0 * G);

  Stiffness(1, 0, 1, 0) = Stiffness(1, 0, 1, 0) + (2.0 * G);

  Stiffness(1, 1, 1, 1) = Stiffness(1, 1, 1, 1) + (2.0 * G);

  Stiffness(1, 2, 1, 2) = Stiffness(1, 2, 1, 2) + (2.0 * G);

  Stiffness(2, 0, 2, 0) = Stiffness(2, 0, 2, 0) + (2.0 * G);

  Stiffness(2, 1, 2, 1) = Stiffness(2, 1, 2, 1) + (2.0 * G);

  Stiffness(2, 2, 2, 2) = Stiffness(2, 2, 2, 2) + (2.0 * G);

  return this->Stiffness;
}


const DTensor2  &NewPisanoLT::getStressTensor(void)
{

  return this->TrialStress;
}



const DTensor2 &NewPisanoLT::getStrainTensor(void)
{

  // return this->alpha0;
  return this->TrialStrain;
}


const DTensor2 &NewPisanoLT::getPlasticStrainTensor(void)
{

  // return this->alpha0mem;
  return this->TrialPlastic_Strain;
}

const DTensor2  &NewPisanoLT::getCommittedStressTensor(void)
{

  return this->CommitStress;
}



const DTensor2 &NewPisanoLT::getCommittedStrainTensor(void)
{

  return this->CommitStrain;
}


const DTensor2 &NewPisanoLT::getCommittedPlasticStrainTensor(void)
{

  return this->CommitPlastic_Strain;
}

const DTensor2  &NewPisanoLT::getTrialStressTensor(void)
{

  return this->TrialStress;
}



const DTensor2 &NewPisanoLT::getTrialStrainTensor(void)
{

  return this->TrialStrain;
}


const DTensor2 &NewPisanoLT::getTrialPlasticStrainTensor(void)
{

  return this->TrialPlastic_Strain;
}


//================================================================================
//  Save / Restore
//================================================================================
int NewPisanoLT::commitState(void)
{


  CommitStress(i, j)            = TrialStress(i, j);
  CommitStrain(i, j)            = TrialStrain(i, j);
  CommitPlastic_Strain(i, j)    = TrialPlastic_Strain(i, j);
  Commit_alpha(i, j)            = Trial_alpha(i, j);
  Commit_alpha0(i, j)           = Trial_alpha0(i, j);
  Commit_alpha0mem(i, j)        = Trial_alpha0mem(i, j);
  Commit_nij_dev(i, j)          = Trial_nij_dev(i, j);


  return 0;
}


int NewPisanoLT::revertToLastCommit(void)
{

  TrialStress = CommitStress;
  TrialStrain = CommitStrain;
  TrialPlastic_Strain = CommitPlastic_Strain;

  Trial_alpha = Commit_alpha;
  Trial_alpha0 = Commit_alpha0;
  Trial_alpha0mem = Commit_alpha0mem;
  Trial_nij_dev = Commit_nij_dev;
  return 0;
}


int NewPisanoLT::revertToStart(void)
{

  // CommitStress = ElasticStateStress;
  // CommitStrain = ElasticStateStrain;
  CommitPlastic_Strain *= 0;// ZeroStrain;

  TrialStress = CommitStress;
  TrialStrain = CommitStrain;

  getE();

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
                                      this->geta(),
                                      this->geteplcum_cr());
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
                                        this->geta(),
                                        this->geteplcum_cr());
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

// int NewPisanoLT::describeSelf(int commitTag, HDF5_Channel &theHDF5_Channel)
// {
//     theHDF5_Channel.beginMaterialDescription("NewPisanoLT" , this->getTag());
//     theHDF5_Channel.addField("strain"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 1.
//     theHDF5_Channel.addField("stress"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_BASIC);    // 2.
//     theHDF5_Channel.addField("plastic_strain"              , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DETAILED); // 3.
//     theHDF5_Channel.addField("alpha"                       , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 4.
//     theHDF5_Channel.addField("alpha0"                      , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 5.
//     theHDF5_Channel.addField("stress_n_minus_2"            , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 6.
//     theHDF5_Channel.addField("nij_dev"                     , ESSI_OUTPUT_TIME_DEPENDENT   , ESSI_OUTPUT_LEVEL_DEBUG);    // 7.
//     theHDF5_Channel.addField("model_parameters"            , ESSI_OUTPUT_TIME_INDEPENDENT , ESSI_OUTPUT_LEVEL_BASIC);    // 8.
//     theHDF5_Channel.endMaterialDescription();

//     return 0;
// }


int NewPisanoLT::sendSelf(int commitTag, Channel &theChannel)
{
  constexpr int size = 18;
  static Vector material_parameters(size);

  //Send material parameters
  material_parameters(0) = beta;
  material_parameters(1) = E0;
  material_parameters(2) = v;
  material_parameters(3) = M;
  material_parameters(4) = kd;
  material_parameters(5) = xi;
  material_parameters(6) = h;
  material_parameters(7) = m;
  material_parameters(8) = rho;
  material_parameters(9) = initialconfiningstress;
  material_parameters(10) = n;
  material_parameters(11) = a;
  material_parameters(12) = eplcum_cr;
  material_parameters(13) = eplcum;
  material_parameters(14) = f_relative_tol;
  material_parameters(15) = stress_relative_tol;
  material_parameters(16) = n_max_iterations;
  material_parameters(17) = (double) constitutive_integration_method;


// DTensor2 material_parameters;
  if ( theChannel.sendVector( 0, commitTag, material_parameters ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to receive material_parameters\n";
    return -1;
  }

  // DTensor2 strainplcum;
  if ( theChannel.sendDTensor2( 0, commitTag, strainplcum ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send strainplcum\n";
    return -1;
  }

  // DTensor2 TrialStrain
  if ( theChannel.sendDTensor2( 0, commitTag, TrialStrain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send TrialStrain\n";
    return -1;
  }
  // DTensor2 TrialStress
  if ( theChannel.sendDTensor2( 0, commitTag, TrialStress ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send TrialStress\n";
    return -1;
  }
  // DTensor2 TrialPlastic_Strain
  if ( theChannel.sendDTensor2( 0, commitTag, TrialPlastic_Strain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send TrialPlastic_Strain\n";
    return -1;
  }

  // DTensor2 CommitStress
  if ( theChannel.sendDTensor2( 0, commitTag, CommitStress ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send CommitStress\n";
    return -1;
  }
  // DTensor2 CommitStrain
  if ( theChannel.sendDTensor2( 0, commitTag, CommitStrain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send CommitStrain\n";
    return -1;
  }
  // DTensor2 CommitPlastic_Strain
  if ( theChannel.sendDTensor2( 0, commitTag, CommitPlastic_Strain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send CommitPlastic_Strain\n";
    return -1;
  }

  // DTensor2 Trial_alpha
  if ( theChannel.sendDTensor2( 0, commitTag, Trial_alpha ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Trial_alpha\n";
    return -1;
  }
  // DTensor2 Trial_alpha0
  if ( theChannel.sendDTensor2( 0, commitTag, Trial_alpha0 ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Trial_alpha0\n";
    return -1;
  }
  // DTensor2 Trial_alpha0mem
  if ( theChannel.sendDTensor2( 0, commitTag, Trial_alpha0mem ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Trial_alpha0mem\n";
    return -1;
  }
  // DTensor2 Trial_nij_dev
  if ( theChannel.sendDTensor2( 0, commitTag, Trial_nij_dev ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Trial_nij_dev\n";
    return -1;
  }

  // DTensor2 Commit_alpha
  if ( theChannel.sendDTensor2( 0, commitTag, Commit_alpha ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Commit_alpha\n";
    return -1;
  }
  // DTensor2 Commit_alpha0
  if ( theChannel.sendDTensor2( 0, commitTag, Commit_alpha0 ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Commit_alpha0\n";
    return -1;
  }
  // DTensor2 Commit_alpha0mem
  if ( theChannel.sendDTensor2( 0, commitTag, Commit_alpha0mem) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Commit_alpha0mem\n";
    return -1;
  }
  // DTensor2 Commit_nij_dev
  if ( theChannel.sendDTensor2( 0, commitTag, Commit_nij_dev ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::sendSelf() - " << this->getTag() << " failed to send Commit_nij_dev\n";
    return -1;
  }

  getE();


  return 0;
}


int NewPisanoLT::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
  constexpr int size = 18;
  static Vector material_parameters(size);

  //Receive material parameters


  // DTensor2 material_parameters;
  if ( theChannel.receiveVector( 0, commitTag, material_parameters ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive material_parameters\n";
    return -1;
  }

  beta                   = material_parameters(0);
  E0                     = material_parameters(1);
  v                      = material_parameters(2);
  M                      = material_parameters(3);
  kd                     = material_parameters(4);
  xi                     = material_parameters(5);
  h                      = material_parameters(6);
  m                      = material_parameters(7);
  rho                    = material_parameters(8);
  initialconfiningstress = material_parameters(9);
  n                      = material_parameters(10);
  a                      = material_parameters(11);
  eplcum_cr              = material_parameters(12);
  eplcum                 = material_parameters(13);
  NDMaterialLT::f_relative_tol         = material_parameters(14) ;
  NDMaterialLT::stress_relative_tol    = material_parameters(15) ;
  NDMaterialLT::n_max_iterations       = material_parameters(16) ;
  NDMaterialLT::constitutive_integration_method       = (NDMaterialLT_Constitutive_Integration_Method) material_parameters(17) ;



  // DTensor2 strainplcum;
  if ( theChannel.receiveDTensor2( 0, commitTag, strainplcum ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive strainplcum\n";
    return -1;
  }

  // DTensor2 TrialStrain
  if ( theChannel.receiveDTensor2( 0, commitTag, TrialStrain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive TrialStrain\n";
    return -1;
  }
  // DTensor2 TrialStress
  if ( theChannel.receiveDTensor2( 0, commitTag, TrialStress ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive TrialStress\n";
    return -1;
  }
  // DTensor2 TrialPlastic_Strain
  if ( theChannel.receiveDTensor2( 0, commitTag, TrialPlastic_Strain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive TrialPlastic_Strain\n";
    return -1;
  }

  // DTensor2 CommitStress
  if ( theChannel.receiveDTensor2( 0, commitTag, CommitStress ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive CommitStress\n";
    return -1;
  }
  // DTensor2 CommitStrain
  if ( theChannel.receiveDTensor2( 0, commitTag, CommitStrain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive CommitStrain\n";
    return -1;
  }
  // DTensor2 CommitPlastic_Strain
  if ( theChannel.receiveDTensor2( 0, commitTag, CommitPlastic_Strain ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive CommitPlastic_Strain\n";
    return -1;
  }

  // DTensor2 Trial_alpha
  if ( theChannel.receiveDTensor2( 0, commitTag, Trial_alpha ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Trial_alpha\n";
    return -1;
  }
  // DTensor2 Trial_alpha0
  if ( theChannel.receiveDTensor2( 0, commitTag, Trial_alpha0 ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Trial_alpha0\n";
    return -1;
  }
  // DTensor2 Trial_alpha0mem
  if ( theChannel.receiveDTensor2( 0, commitTag, Trial_alpha0mem ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Trial_alpha0mem\n";
    return -1;
  }
  // DTensor2 Trial_nij_dev
  if ( theChannel.receiveDTensor2( 0, commitTag, Trial_nij_dev ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Trial_nij_dev\n";
    return -1;
  }

  // DTensor2 Commit_alpha
  if ( theChannel.receiveDTensor2( 0, commitTag, Commit_alpha ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Commit_alpha\n";
    return -1;
  }
  // DTensor2 Commit_alpha0
  if ( theChannel.receiveDTensor2( 0, commitTag, Commit_alpha0 ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Commit_alpha0\n";
    return -1;
  }
  // DTensor2 Commit_alpha0mem
  if ( theChannel.receiveDTensor2( 0, commitTag, Commit_alpha0mem) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Commit_alpha0mem\n";
    return -1;
  }
  // DTensor2 Commit_nij_dev
  if ( theChannel.receiveDTensor2( 0, commitTag, Commit_nij_dev ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - " << this->getTag() << " failed to receive Commit_nij_dev\n";
    return -1;
  }

  getE();

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

double NewPisanoLT::geteplcum_cr(void)
{

  return eplcum_cr;
}

DTensor2 &NewPisanoLT::getInternalTensor(void)
{

  return Trial_alpha;
}

//================================================================================
//  Plasticity!
//================================================================================

int NewPisanoLT::compute_stress_increment(const DTensor2 &strain_incr, DTensor2& dsigma, DTensor2& depsilon_plastic)
{

  //=============================================================================================
  // Some local definitions
  //=============================================================================================

  double Delta_lambda           = 0.0;    // Plastic multiplier increment
  double G                      = 0.0;    // Shear mod
  double K                      = 0.0;    // Bulk mod
  double depsilon_vol           = 0.0;    // Volumetric incremental strain
  double unload_prod            = 0.0;    //
  double H                      = 0.0;    // Hardening modulus
  double alpha_nijdev           = 0.0;    //
  double p_plastic              = 0.0;    //

  static DTensor2 mij(3, 3, 0.0);                 // Unit vector normal to potential surface
  static DTensor2 depsilon(3, 3, 0.0);            //
  static DTensor2 sigma(3, 3, 0.0);               // Stress at start of step
  static DTensor2 depsilon_dev(3, 3, 0.0);        // Incremental deviatoric strain
  static DTensor2 alpha_0(3, 3, 0.0);             //
  static DTensor2 TrialStress_dev(3, 3, 0.0);     // Deviatoric elastoplastic stress
  static DTensor2 strainplcum_dev(3, 3, 0.0);


  mij              *= 0;
  depsilon         *= 0;
  dsigma           *= 0;
  depsilon_plastic *= 0;
  sigma            *= 0;
  depsilon_dev     *= 0;
  alpha_0          *= 0;
  TrialStress_dev  *= 0;
  strainplcum_dev  *= 0;

  Index < 'p' > p;                        // Extra index for LTensor computations
  Index < 'q' > q;                        // Extra index for LTensor computations




  //---------------------------------------------------------------------------------------------
  // Get initial state
  //---------------------------------------------------------------------------------------------

  sigma(i, j)  = TrialStress(i, j);

  depsilon(i, j) = strain_incr(i, j); //Its seems superficial to have depsilon and stress_incr, but the reason is that strain_incr is "const" and member function "compute_deviatoric_tensor" does not work with const tensors... which is wrong but it is what we have........ LTensor's fault :( -J.Abell

  double E = getE();

  G = E / (2.0 * (1.0 + v));
  K = E / (3.0 * (1.0 - 2.0 * v));


  depsilon.compute_deviatoric_tensor(depsilon_dev, depsilon_vol);
  depsilon_vol = depsilon(i, i);

  double dp = -K * depsilon_vol;


  int count = 0;
  do
  {

    //---------------------------------------------------------------------------------------------
    // Define normal to yield surface deviatoric strain increment (Hypoplastic model)
    //---------------------------------------------------------------------------------------------

    double norm_nij_dev;

    Trial_nij_dev(i, j) = 2 * G * depsilon_dev(i, j) -  dp * Trial_alpha(i, j);
    norm_nij_dev = sqrt(Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

    if (norm_nij_dev > 0)
    {
      Trial_nij_dev(i, j) = Trial_nij_dev(i, j) / norm_nij_dev;
    }
    else
    {
      Trial_nij_dev *= 0;
    }


    // cout << "nij_dev = " << Trial_nij_dev << endl;

    //---------------------------------------------------------------------------------------------
    // Check for unloading!!! (which alpha0 to use)
    //---------------------------------------------------------------------------------------------

    unload_prod = Trial_nij_dev(i, j) * (Trial_alpha(i, j) -  Trial_alpha0(i, j)) ;

    if (unload_prod < 0.0)
    {
      if (true)//eplcum >= eplcum_cr)
      {
        alpha_0(i, j)    = Trial_alpha(i, j);
      }
      else
      {
        alpha_0(i, j) = Trial_alpha0mem(i, j); // back stress is set as at the previous unloading
      }
    }
    else
    {
      alpha_0(i, j) = Trial_alpha0(i, j);
    }

    H = getH(alpha_0);




    //---------------------------------------------------------------------------------------------
    // Compute plastic flow direction (mij)
    //---------------------------------------------------------------------------------------------

    double D = getD();

    mij(i, j) = Trial_nij_dev(i, j);

    mij(0, 0) = Trial_nij_dev(0, 0) - D / 3;
    mij(1, 1) = Trial_nij_dev(1, 1) - D / 3;
    mij(2, 2) = Trial_nij_dev(2, 2) - D / 3;

    // double norm_mij = sqrt(mij(i, j) * mij(i, j));
    // mij(i, j) = mij(i, j) / norm_mij;

    //---------------------------------------------------------------------------------------------
    // Compute plastic multiplier
    //---------------------------------------------------------------------------------------------


    // scalar product depsilon_dev:nij_dev
    double depsilon_dev_nijdev = depsilon_dev(i, j) * Trial_nij_dev(i, j);

    alpha_nijdev = Trial_alpha(i, j) * Trial_nij_dev(i, j);                          // scalar product alphaij:nij_dev

    // Plastic muliplier;
    double num =  (2.0 * G * depsilon_dev_nijdev + K * alpha_nijdev * depsilon_vol);
    double den =   2.0 * G + (2.0 / 3.0) * H - K * D * alpha_nijdev;

    Delta_lambda = num / den;
    if (Delta_lambda < 0.0)
    {
      Delta_lambda = 0.0;
    }

    // cout3 << "Delta_lambda = " << Delta_lambda << ", dp = " << dp << endl;

    //Check dp and iterate for it if necesary
    double dp_predict = -K * (depsilon_vol + D * Delta_lambda);
    double pp = -sigma(i, i) / 3;
    double dp_error = fabs(dp - dp_predict) / (pp + dp_predict);
    if (dp_error > NDMaterialLT::stress_relative_tol)
    {
      dp = dp_predict;
    }
    else
    {
      break;
    }

    count += 1;
  }
  while ( count < NDMaterialLT::n_max_iterations );

  if (count >= NDMaterialLT::n_max_iterations)
  {
    cerr << "NewPisanoLT :: compute_stress_increment - Exceeded maximum number of iterations (" << NDMaterialLT::n_max_iterations << ")\n";
    return -1;
  }

  //---------------------------------------------------------------------------------------------
  // Update internal variales and increments
  //---------------------------------------------------------------------------------------------

  // Plastic strain increment
  depsilon_plastic(i, j) = mij(i, j) * (Delta_lambda);
  dsigma(i, j)  = Ee(i, j, p, q) * (  depsilon(p, q) - depsilon_plastic(p, q) );

  // cout << "dsigma = " << dsigma << endl;

  //update internal variable
  sigma(i, j)  = TrialStress(i, j) + dsigma(i, j);
  sigma.compute_deviatoric_tensor(TrialStress_dev, p_plastic);
  p_plastic = -sigma(i, i) / 3;
  if (p_plastic > 0)//1000 * check_for_zero)
  {
    Trial_alpha(i, j) = TrialStress_dev(i, j);
    Trial_alpha /= p_plastic;
  }
  else
  {
    Trial_alpha *= 0;
  }

  return 0;
}


//
// if (unload_prod < 0.0)
// {

//   if (eplcum >= eplcum_cr) // the projection center is first memorized and updated
//   {
//     Trial_alpha0mem(i, j) = Trial_alpha0(i, j);
//     Trial_alpha0(i, j)    = Trial_alpha(i, j);
//   }
//   else // the previous projection center is reset
//   {
//     Trial_alpha0(i, j) = Trial_alpha0mem(i, j); // back stress is set as at the previous unloading
//     // unload_prod = nij_dev(i, j) * alpha(i, j) - nij_dev(i, j) * alpha0(i, j) ;
//   }
//   strainplcum *=  0;//ZeroStrain(i, j); //TrialPlastic_Strain(i, j); // the local cumulated plastic strain is restarted
// }


// This part is now done only if the dsigma and depsilon_plastic are accepted
//   TrialStress(i, j)  = sigma(i, j) + dsigma(i, j);

// // Update all variables
//   TrialPlastic_Strain(i, j) = epsilon_plastic(i, j);
//   TrialPlastic_Strain(i, j) += depsilon_plastic(i, j);

// //---------------------------------------------------------------------------------------------
// // Overshooting Remediation
// //---------------------------------------------------------------------------------------------
//   strainplcum(i, j) += depsilon_plastic(i, j);

//   strainplcum.compute_deviatoric_tensor(strainplcum_dev, strainplcum_vol); //check declaration!
//   eplcum = sqrt((0.6666666) * strainplcum_dev(i, j) * strainplcum_dev(i, j));

//   TrialStress.compute_deviatoric_tensor(TrialStress_dev, p_plastic);
//   p_plastic = -TrialStress(i, i) / 3;

// // this avoids weird alpha values...it is difficult to find a general rule for the checking factor..tests needed
// // stress rapporto -> stress ratio

//   if (p_plastic > 1000 * check_for_zero)
//   {
//     Trial_alpha(i, j) = TrialStress_dev(i, j);
//     Trial_alpha /= p_plastic;
//   }
//   else
//   {
//     Trial_alpha *= 0;//ZeroStress(i, j);
//   }


//   return 0;
// }

int NewPisanoLT::Euler_One_Step(const DTensor2& depsilon)
{

  int errorflag = 0;

  static DTensor2 dsigma(3, 3, 0.0);
  static DTensor2 depsilon_pl(3, 3, 0.0);

  errorflag = this->compute_stress_increment(depsilon, dsigma, depsilon_pl);

  TrialStress(i, j) +=  dsigma(i, j);
  TrialPlastic_Strain(i, j) += depsilon_pl(i, j);


  double unload_prod = Trial_nij_dev(i, j) * (Trial_alpha(i, j) -  Trial_alpha0(i, j)) ;
  if (unload_prod < 0.0)
  {

    if (true)//eplcum >= eplcum_cr) // the projection center is first memorized and updated
    {
      Trial_alpha0mem(i, j) = Trial_alpha0(i, j);
      Trial_alpha0(i, j)    = Trial_alpha(i, j);
    }
    else // the previous projection center is reset
    {
      Trial_alpha0(i, j) = Trial_alpha0mem(i, j); // back stress is set as at the previous unloading
    }
    strainplcum *=  0;
  }

  //============================================
  //Recompute nij_dev
  //============================================
  // double E = getE();
  // double G = E / (2.0 * (1.0 + v));

  // static DTensor2 depsilon_dev(3, 3, 0.0);        // Incremental deviatoric strain
  // double depsilon_vol;
  // double norm_nij_dev;
  // depsilon.compute_deviatoric_tensor(depsilon_dev, depsilon_vol);
  // double dp = -(dsigma1(i, i) + dsigma2(i, i)) / 3;

  // Trial_nij_dev(i, j) = 2 * G * depsilon_dev(i, j) -  dp * Trial_alpha(i, j);
  // norm_nij_dev = sqrt(Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

  // if (norm_nij_dev > check_for_zero)
  // {
  //   Trial_nij_dev(i, j) = Trial_nij_dev(i, j) / norm_nij_dev;
  // }
  // else
  // {
  //   Trial_nij_dev *= 0;
  // }
  //============================================

  return errorflag;
}

int NewPisanoLT::Modified_Euler_Error_Control(const DTensor2& strain_increment)
{
  int errorflag;

  double T = 0;
  double dT = 1;
  double q = 0;

  static DTensor2 depsilon(3, 3, 0.0);
  static DTensor2 depsilon_pl(3, 3, 0.0);

  static DTensor2 dsigma1(3, 3, 0.0);
  static DTensor2 dalpha1(3, 3, 0.0);
  static DTensor2 dalpha01(3, 3, 0.0);
  static DTensor2 nijdev1(3, 3, 0.0);

  static DTensor2 dsigma2(3, 3, 0.0);
  static DTensor2 dalpha2(3, 3, 0.0);
  static DTensor2 dalpha02(3, 3, 0.0);
  static DTensor2 nijdev2(3, 3, 0.0);

  static DTensor2 sigma_error(3, 3, 0.0);

  static DTensor2 TrialStress_dev(3, 3, 0.0);

  static DTensor2 start_sigma(3, 3, 0.0);
  static DTensor2 start_alpha(3, 3, 0.0);
  static DTensor2 start_alpha0(3, 3, 0.0);

  // static DTensor2 epsilon_applied(3, 3, 0.0);

  depsilon *= 0;
  dsigma1 *= 0;
  dalpha1 *= 0;
  dalpha01 *= 0;
  depsilon_pl *= 0;
  TrialStress_dev *= 0;
  dsigma2 *= 0;
  dalpha2 *= 0;
  dalpha02 *= 0;
  sigma_error *= 0;
  start_sigma *= 0;
  start_alpha *= 0;
  start_alpha0 *= 0;
  // epsilon_applied *= 0;

  start_sigma(i, j) = TrialStress(i, j);
  start_alpha(i, j) = Trial_alpha(i, j);
  start_alpha0(i, j) = Trial_alpha0(i, j);
  // start_alpha0(i, j) = Commit_alpha0(i, j);

  int count = 0;

  while (T < 1)
  {

    double beta = getBeta(Trial_alpha0);
    double H = getH(Trial_alpha0);

    // cout << "================\n" ;
    // cout << "T = " << T << ", q = " << q << ", dT = " << dT << ", beta = " << beta << ", H = " << H <<  endl;

    depsilon(i, j) = dT * strain_increment(i, j);
    // cout << "depsilon = " << depsilon << endl;

    // cout << "dsigma1 = " << endl;
    //First pass
    errorflag = this->compute_stress_increment( depsilon, dsigma1, depsilon_pl);
    if (errorflag < 0)
    {
      return errorflag;
    }

    TrialStress(i, j)  = start_sigma(i, j) + dsigma1(i, j);
    dalpha1(i, j) = Trial_alpha(i, j) - start_alpha(i, j);
    dalpha01(i, j) = Trial_alpha0(i, j) - start_alpha0(i, j);
    nijdev1(i, j) = Trial_nij_dev(i, j);


    //Second pass
    // cout << "dsigma2 = " << endl;
    errorflag = this->compute_stress_increment(depsilon, dsigma2, depsilon_pl);
    if (errorflag < 0)
    {
      return errorflag;
    }

    TrialStress(i, j)  = start_sigma(i, j) + (dsigma1(i, j) + dsigma2(i, j)) / 2;

    dalpha2(i, j) = Trial_alpha(i, j) - (start_alpha(i, j) + dalpha1(i, j));
    dalpha02(i, j) = Trial_alpha0(i, j) - (start_alpha0(i, j) + dalpha01(i, j));
    nijdev2(i, j) = Trial_nij_dev(i, j);

    // //Compute errors
    sigma_error(i, j) = (dsigma2(i, j) - dsigma1(i, j)) / 2;
    double Relative_Error = sqrt(sigma_error(i, j) * sigma_error(i, j)) / sqrt(TrialStress(i, j) * TrialStress(i, j));

    //Compute the Pisano Error metric
    // static DTensor2 ds1(3, 3, 0.0);
    // static DTensor2 ds2(3, 3, 0.0);
    // static DTensor2 TrialS(3, 3, 0.0);
    // double dp1, dp2, Trial_p;
    // constexpr double a = 0.0;
    // constexpr double b = 1 - a;


    // TrialStress.compute_deviatoric_tensor(TrialS, dp1);
    // Trial_p = -TrialStress(i, i) / 3;

    // dsigma1.compute_deviatoric_tensor(ds1, dp1);
    // dp1 = -dsigma1(i, i) / 3;
    // dsigma2.compute_deviatoric_tensor(ds2, dp2);
    // dp2 = -dsigma2(i, i) / 3;
    // sigma_error(i, j) = (ds2(i, j) - ds1(i, j)) / 2;

    // double p_error = (dp2 - dp1) / 2;

    // //Pisano Error metric
    // double Relative_Error = a * sqrt(sigma_error(i, j) * sigma_error(i, j)) / sqrt(TrialS(i, j) * TrialS(i, j))
    //                         + b * fabs(p_error) / fabs(Trial_p);


    // //Limit the relative error
    // Relative_Error = fmax(Relative_Error, std::numeric_limits<double>::epsilon());


    if (Relative_Error < this-> stress_relative_tol) // Accept this step
    {
      T += dT;
      // cout << "Accept! R = " <<  Relative_Error << " T = " << T << endl;
      // cout << "T = " << T << ", dT = " << dT << ", q = " << q << endl;
      // Trial_alpha(i, j) = start_alpha(i, j) + (dalpha1(i, j) + dalpha2(i, j)) / 2;

      double p_plastic;
      TrialStress.compute_deviatoric_tensor(TrialStress_dev, p_plastic);
      p_plastic = -TrialStress(i, i) / 3;
      if (p_plastic > 0)//1000 * check_for_zero)
      {
        Trial_alpha(i, j) = TrialStress_dev(i, j);
        Trial_alpha /= p_plastic;
      }
      else
      {
        Trial_alpha *= 0;
      }

      // Trial_alpha0(i, j) = start_alpha0(i, j) + (dalpha01(i, j) + dalpha02(i, j)) / 2;
      TrialPlastic_Strain(i, j) += depsilon_pl(i, j);
      // Trial_nij_dev(i, j) = (nijdev1(i, j) + nijdev2(i, j)) / 2;
      start_sigma(i, j) = TrialStress(i, j);
      start_alpha(i, j) = Trial_alpha(i, j);
      // start_alpha0(i, j) = Trial_alpha0(i, j);

      // cout << "Trial_nij_dev = " << Trial_nij_dev << endl;

      q = fmin(0.8 * sqrt(this-> stress_relative_tol / Relative_Error), 2.0);
      dT = q * dT;
      dT = fmin(dT, 1.0 - T);

      // //============================================
      // //Recompute nij_dev
      // //============================================
      double E = getE();
      double G = E / (2.0 * (1.0 + v));

      static DTensor2 depsilon_dev(3, 3, 0.0);        // Incremental deviatoric strain
      double depsilon_vol;
      double norm_nij_dev;
      depsilon.compute_deviatoric_tensor(depsilon_dev, depsilon_vol);
      double dp = -(dsigma1(i, i) + dsigma2(i, i)) / 3;

      Trial_nij_dev(i, j) = 2 * G * depsilon_dev(i, j) -  dp * Trial_alpha(i, j);
      norm_nij_dev = sqrt(Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

      if (norm_nij_dev > 0)
      {
        Trial_nij_dev(i, j) = Trial_nij_dev(i, j) / norm_nij_dev;
      }
      else
      {
        Trial_nij_dev *= 0;
      }
      // //============================================

      double unload_prod = Trial_nij_dev(i, j) * (Trial_alpha(i, j) -  Trial_alpha0(i, j)) ;
      // cout << "unload_prod = " << unload_prod << endl;
      if (unload_prod < 0.0)
      {

        if (true)//eplcum >= eplcum_cr) // the projection center is first memorized and updated
        {
          Trial_alpha0mem(i, j) = Trial_alpha0(i, j);
          Trial_alpha0(i, j)    = Trial_alpha(i, j);
        }
        else // the previous projection center is reset
        {
          Trial_alpha0(i, j) = Trial_alpha0mem(i, j); // back stress is set as at the previous unloading
        }
        strainplcum *=  0;
      }
      start_alpha0(i, j) = Trial_alpha0(i, j);
      // epsilon_applied(i, j) += depsilon(i, j);

    }
    else //Reject step, take a smaller one!
    {
      // cout << "Reject! R = " <<  Relative_Error << endl;
      q = fmax(0.8 * sqrt(this-> stress_relative_tol / Relative_Error), 0.1);
      dT = q * dT;
      TrialStress(i, j) = start_sigma(i, j);
      Trial_alpha(i, j) = start_alpha(i, j);
      Trial_alpha0(i, j) = start_alpha0(i, j);
    }


    count += 1;
    if (count > this-> n_max_iterations)
    {
      cerr << "NewPisanoLT -- Modified euler failed to converge after " << count << " iterations with : \n"
           << "  Relative_Error = " << Relative_Error  << endl
           << "  q = " << q  << endl
           << "  dT = " << dT  << endl
           << "  T = " << T  << endl;
      return -1;
    }
  }

  // cout << "    Number of Euler iterations = " << count << endl;
  // cout << "    this-> stress_relative_tol = " << this-> stress_relative_tol << endl;

  return 0;
}





// Probably should be inlined
double NewPisanoLT::get_distance_coeff(DTensor2 &alpha0)
{

  double nij_dev_norm = sqrt (Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

  if (nij_dev_norm > 0)//1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
  {

    //coefficients 2nd order equation
    double a;
    double b;
    double c;
    double Delta_equation;

    // cout << "Trial_alpha = " << Trial_alpha << endl;
    // cout << "alpha0 = " << alpha0 << endl;

    //// a

    a = (Trial_alpha(i, j) - alpha0(i, j)) * (Trial_alpha(i, j) - alpha0(i, j));

    //// b

    b = 2 * Trial_alpha(i, j) * (Trial_alpha(i, j) - alpha0(i, j));

    //// c

    // c = Trial_alpha(i, j) * Trial_alpha(i, j) - (2.0 / 3.0) * M * M;
    c = Trial_alpha(i, j) * Trial_alpha(i, j) - (0.6666666666666666666666666666666666666) * M * M;

    //// solution

    if ( a > 0)
    {
      Delta_equation = b * b - 4.0 * a * c;

      if (Delta_equation < 0.0)
      {
        beta = beta_max;
      }
      else
      {
        beta = (-b + sqrt(Delta_equation)) / (2.0 * a);
      }



      if (beta < beta_min)
      {
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
    beta = beta_max;
    // cerr << "The norm of nij is not 1! This should never happen. Something wrong... ask Pisano." << endl;
    // cout << "Norm of nij_dev is nil. Might be non-deviatoric loading step... or something is wrong. Ask Pisano." << endl;

  };

  return beta;
}





///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double NewPisanoLT::get_distance_coeff_lode(DTensor2 &alpha0)
{

  // A NEW SOIL PARAMETER n NEEDS TO BE DEFINED FOR THE DEVIATORIC SECTION!!!

  double nij_dev_norm = sqrt (Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

  if (nij_dev_norm > 0)//1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
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

    for (double lode = -M_PI / 6.0; lode <=  M_PI / 6.0; lode += lode_step ) // loop over the Lode angle
    {
      R = pow(((1 - beta_secdev * sin(3.0 * lode)) / (1 - beta_secdev)), n);
      M_lode = M * R; // shear strength at a given (assumed) lode angle

      //coefficients 2nd order equation
      double a;
      double b;
      double c;
      double Delta_equation;

      //// a

      a = (Trial_alpha(i, j) - alpha0(i, j)) * (Trial_alpha(i, j) - alpha0(i, j));

      //// b

      b = 2 * Trial_alpha(i, j) * (Trial_alpha(i, j) - alpha0(i, j));

      //// c

      c = Trial_alpha(i, j) * Trial_alpha(i, j) - (0.6666666666666666666666666666666666666) * M_lode * M_lode;

      //// solution

      if ( a > 0)//check_for_zero)
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

          alphab(i, j) = Trial_alpha(i, j) + beta * (Trial_alpha(i, j) - alpha0(i, j)); // bounding back stress

          lode_alphab = get_lode_angle(alphab); //CHECK THIS

          if (fabs(lode_alphab - lode) < lode_step)
          {
            // cout << "lode   = " << lode << endl;
            // cout << "M_lode = " << M_lode << endl;
            // cout << "lode_alphab = " << lode_alphab << endl;
            // cout << "alphab(0,0) = " << alphab(0, 0) << endl;
            // cout << "alphab(1,1) = " << alphab(1, 1) << endl;
            // cout << "alphab(2,2) = " << alphab(2, 2) << endl;
            // cout << "alphab(0,1) = " << alphab(0, 1) << endl;
            // cout << "alphab(0,2) = " << alphab(0, 2) << endl;
            // cout << "alphab(1,2) = " << alphab(1, 2) << endl;


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
  if (fabs(xi) < check_for_zero)
  {
    return 0.0;
  }
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


  double nij_dev_norm = sqrt (Trial_nij_dev(i, j) * Trial_nij_dev(i, j));

  if (nij_dev_norm > 0)//1.0e3 * check_for_zero)  // FIXME 1.0e3 is arbitrary
  {
    lode    = get_lode_angle(Trial_nij_dev);
    R       = pow(((1 - beta_secdev * sin(3.0 * lode)) / (1 - beta_secdev)), n);
    kd_lode = kd * R; // shear strength at a given (assumed) lode angle
  }
  else
  {
    kd_lode = kd;
  };

  alphad(i, j) = (sqrt(2.0 / 3.0) * kd_lode) * Trial_nij_dev(i, j);

  D = xi * (Trial_nij_dev(i, j) * (alphad(i, j) - Trial_alpha(i, j)));

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

  //The input tensor s must be deviatoric (symmetric, nil trace)

  if (fabs(s(i, i)) > check_for_zero )
  {
    cerr << "NewPisanoLT::lode   Warning! Lode angle is being computed with a non-deviatoric tensor s.\n\n";
  }

  double J2;
  double J3;
  double arg;



  J2 = 0.5 * (s(i, j) * s(i, j));

  J3 = (
         s(0, 0) * s(1, 1) * s(2, 2) +
         s(0, 1) * s(1, 2) * s(2, 0) +
         s(0, 2) * s(1, 0) * s(2, 1) +
         -
         (
           s(0, 2) * s(1, 1) * s(2, 0) +
           s(1, 2) * s(2, 1) * s(0, 0) +
           s(2, 2) * s(0, 1) * s(1, 0)
         )
       ) ;/// 3;
  //s.compute_Determinant(); // I need the determinant of the deviatoric stress tensor!!!

  if (J2 > 0)//check_for_zero)
  {
    // arg = -2.598076211353316 * cbrt(J3) / sqrt(J2); // esta era la ueada!!!
    arg = -2.598076211353316 * J3 / sqrt(J2 * J2 * J2);
  }
  else
  {
    arg = 0.0; // this is for hydrostastic stress states
  };

  if (arg > 1.0)
  {
    // cout << "arg = " << arg  << endl;
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

  double E;

  if (a == 0)
  {
    // return E0;
    E = E0;
  }
  double p = -TrialStress(i, i) / 3.0;
  if (p > patm / 2)
  {
    E = E0 * pow(p / patm, a);
    // return E0 * pow(p / patm, a);
  }
  else
  {
    E = E0 * pow(0.5, a);
    // return E0 * pow(0.5, a);
  }

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

  return E;
}




double   NewPisanoLT::get_eplcum() const
{
  return eplcum;
}

DTensor2 NewPisanoLT::get_strainplcum() const
{
  return strainplcum;
}

DTensor2 NewPisanoLT::get_alpha() const
{
  return Trial_alpha;
}

DTensor2 NewPisanoLT::get_alpha0() const
{
  return Trial_alpha0;
}

DTensor2 NewPisanoLT::get_alpha0mem() const
{
  return Trial_alpha0mem;
}


DTensor2 NewPisanoLT::get_nij_dev() const
{
  return Trial_nij_dev;
}




int NewPisanoLT::getObjectSize()
{
  int size = 0;

  //Model parameters
  //double beta;                // Scalar distance coefficient as internal variable
  size += sizeof(beta);
  //double E0;                  // Young's modulus at p = patm
  size += sizeof(E0);
  //double v;                   // Modulus of poisson
  size += sizeof(v);
  //double M;                   // Critical stress ratio in triax compression
  size += sizeof(M);
  //double kd;                  // Dilatancy parameter (controls the transition from dilative to compactive behavior)
  size += sizeof(kd);
  //double xi;                  // Dilatancy parameter(controls the ammount of volumetric plastic strain developed)
  size += sizeof(xi);
  //double h;                   // Hardening parameter
  size += sizeof(h);
  //double m;                   // Hardening parameter
  size += sizeof(m);
  //double rho;                 //
  size += sizeof(rho);
  //double initialconfiningstress;
  size += sizeof(initialconfiningstress);
  //double n;                   // Controls shape of boundary surface in deviatoric plane (n=-0.229 matches Mohr-Coulomb deviatoric section)
  size += sizeof(n);
  //double a;                   // Exponent for pressure dependent elastic moduli
  size += sizeof(a);
  //double eplcum_cr;           // Critical cumulated plastic strain upon stress reversal (for overshooting) // Should be always between 1e-5 and 1e-6 (cfr. Mojtaba E-Kan, Hossein A. Taiebat, On implementation of bounding surface plasticity models with no overshooting effect in solving boundary value problems, Computers and Geotechnics, Volume 55, January 2014, Pages 103-116, ISSN 0266-352X, http://dx.doi.org/10.1016/j.compgeo.2013.08.006.(http://www.sciencedirect.com/science/article/pii/S0266352X13001213))
  size += sizeof(eplcum_cr);
  // double eplcum;
  size += sizeof(eplcum);


  // DTensor2 strainplcum;         //Cumulated plastic strain upon stress reversal (for overshooting)
  size += sizeof(double) * strainplcum.get_size();
  // DTensor2 TrialStrain;
  size += sizeof(double) * TrialStrain.get_size();
  // DTensor2 TrialStress;
  size += sizeof(double) * TrialStress.get_size();
  // DTensor2 TrialPlastic_Strain;
  size += sizeof(double) * TrialPlastic_Strain.get_size();
  // DTensor2 ElasticStateStrain;
  // size += sizeof(double) * ElasticStateStrain.get_size();
  // DTensor2 ElasticStateStress;
  // size += sizeof(double) * ElasticStateStress.get_size();
  // DTensor2 CommitStress;
  size += sizeof(double) * CommitStress.get_size();
  // DTensor2 CommitStrain;
  size += sizeof(double) * CommitStrain.get_size();
  // DTensor2 CommitPlastic_Strain;
  size += sizeof(double) * CommitPlastic_Strain.get_size();
  // DTensor2 alpha;             // back stress ratio tensor
  size += sizeof(double) * Trial_alpha.get_size();
  // DTensor2 alpha0;            // back stress ratio tensor
  size += sizeof(double) * Trial_alpha0.get_size();
  // DTensor2 alpha0mem;         // back stress ratio tensor at last previous stress reversal
  size += sizeof(double) * Trial_alpha0mem.get_size();
  // DTensor2 strainpl0;         // Plastic strain at stress reversal (for overshooting)
  // size += sizeof(double) * Trial_strainpl0.get_size();
  // DTensor2 Stress_n_minus_2;  // Stress tensor at step n-2
  // DTensor2 nij_dev;           // direction of deviatoric plastic strain increment(unit tensor)
  size += sizeof(double) * Trial_nij_dev.get_size();


  //Fourth order stiffness
  // DTensor4 Stiffness;
  size += sizeof(double) * Stiffness.get_size();
  // DTensor4 Ee;
  size += sizeof(double) * Ee.get_size();


  //Global indexes for LTensor
  // Index < 'i' > i;
  // Index < 'j' > j;
  size += sizeof(i);
  size += sizeof(j);

  return size;
}


int NewPisanoLT::startNewStage()
{
  // cout << "New stage\n";
  // cout << "Trial_alpha0mem = " << Trial_alpha0mem << endl;
  // cout << "Trial_alpha0 = " << Trial_alpha0 << endl;
  // cout << "Trial_alpha = " << Trial_alpha << endl;
  // Trial_alpha0mem(i, j) = Trial_alpha0(i, j) = Trial_alpha(i, j);
  return 0;
}

//Returns hardening modulus for given alpha0
double NewPisanoLT::getH(DTensor2 &alpha0)
{
  double beta = getBeta( alpha0);
  double pp = -(TrialStress(i, i)) / 3;
  if (m == 1.0)
  {
    return  pp * h * beta;
  }
  else
  {
    return pp * h * (pow(beta, m));
  }
}

//Returns beta: the distance coeficient for a given alpha0
double NewPisanoLT::getBeta(DTensor2& alpha0)
{
  if ( fabs(n) < check_for_zero)
  {
    return get_distance_coeff(alpha0);    // this should be able to read alpha and alpha0
  }
  else
  {
    return get_distance_coeff_lode(alpha0); // this should be able to read alpha and alpha0
  }
}

//Returns the D parameter based on the current state
double NewPisanoLT::getD()
{
  if ( fabs(n) < check_for_zero)
  {
    static DTensor2 alpha_d(3, 3, 0);
    alpha_d(i, j) = Trial_nij_dev(i, j) * (kd * sqrt(2.0 / 3.0));

    //Flow rule
    if (use_alternative_flow_rule)
    {
      double xi_star = xi * (1.0 - sqrt(Trial_alpha(i, j) * Trial_alpha(i, j)) / (2.0 / 3.0 * M));  // ... to show the effect of variable dilatancy.
      return xi_star * (alpha_d(i, j) - Trial_alpha(i, j) ) * Trial_nij_dev(i, j);
    }
    else  //Use the default, real flow rule.
    {
      return xi * (alpha_d(i, j) - Trial_alpha(i, j) ) * Trial_nij_dev(i, j);
    }
  }
  else
  {
    return get_dilatancy();
  }
}


#endif

