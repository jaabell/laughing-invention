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
// #include <HDF5_Channel.h>
#include <Matrix.h>
#include <Vector.h>

const  DTensor2 NewPisanoLT::ZeroStrain(3, 3, 0.0);
const  DTensor2 NewPisanoLT::ZeroStress(3, 3, 0.0);
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
    Stiffness( 3, 3, 3, 3, 0.0 ),
    Ee(3, 3, 3, 3, 0.0)
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
  TrialPlastic_Strain(i, j) = ZeroStrain(i, j);

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


  //Initialize identity matrix
  kronecker_delta(0, 0) = 1;
  kronecker_delta(1, 1) = 1;
  kronecker_delta(2, 2) = 1;

  this->revertToStart();


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
    Stiffness( 3, 3, 3, 3, 0.0 ),
    Ee(3, 3, 3, 3, 0.0)
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


int NewPisanoLT::setTrialStrainIncr
(const DTensor2 &strain_increment)
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

  // Sending model_parameters (double data)
  Vector model_parameters(14);
  model_parameters(0) = beta;
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
  model_parameters(12) = eplcum_cr;
  model_parameters(13) = eplcum;

  theChannel.sendVector(0, 0, model_parameters);


  //Tensor data

  Matrix aux(3, 3);
  // 1. Sending strainplcum
  aux.setData(strainplcum.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 1. Sending    strainplcum
  aux.setData(strainplcum.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 2. Sending TrialStrain
  aux.setData(TrialStrain.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 3. Sending TrialStress
  aux.setData(TrialStress.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 4. Sending TrialPlastic_Strain
  aux.setData(TrialPlastic_Strain.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 5. Sending ElasticStateStrain
  aux.setData(ElasticStateStrain.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 6. Sending ElasticStateStress
  aux.setData(ElasticStateStress.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 7. Sending CommitStress
  aux.setData(CommitStress.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 8. Sending CommitStrain
  aux.setData(CommitStrain.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 9. Sending CommitPlastic_Strain
  aux.setData(CommitPlastic_Strain.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 10. Sending alpha
  aux.setData(alpha.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 11. Sending alpha0
  aux.setData(alpha0.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 12. Sending alpha0mem
  aux.setData(alpha0mem.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 13. Sending strainpl0
  aux.setData(strainpl0.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 14. Sending Stress_n_minus_2
  aux.setData(Stress_n_minus_2.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);
  // 15. Sending nij_dev
  aux.setData(nij_dev.data, 3, 3);
  theChannel.sendMatrix(0, 0, aux);

  //Send the stiffness tensors

  // here we unwrap the stiffness tensors into a vector of 81 components to be sent to remotes
  // remotes (receiveSelf) will have to re-wrap
  Vector aux_stifftensor(81);
  int comp = 0;
  for (int ii = 0; ii < 3; ii++)
    for (int jj = 0; jj < 3; jj++)
      for (int kk = 0; kk < 3; kk++)
        for (int ll = 0; ll < 3; ll++)
        {
          aux_stifftensor(comp++) = Stiffness(ii, jj, kk, ll);
        }
  theChannel.sendVector(0, 0, aux_stifftensor);

  // Ee
  comp = 0;
  for (int ii = 0; ii < 3; ii++)
    for (int jj = 0; jj < 3; jj++)
      for (int kk = 0; kk < 3; kk++)
        for (int ll = 0; ll < 3; ll++)
        {
          aux_stifftensor(comp++) = Ee(ii, jj, kk, ll);
        }
  theChannel.sendVector(0, 0, aux_stifftensor);




  return 0;
}


int NewPisanoLT::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
  Vector model_parameters(14);

  // Receive model_parameters (double data)


  if ( theChannel.receiveVector( 0, commitTag, model_parameters ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive model_parameters\n";
    return -1;
  }

  beta                   = model_parameters(0);
  E0                     = model_parameters(1);
  v                      = model_parameters(2);
  M                      = model_parameters(3);
  kd                     = model_parameters(4);
  xi                     = model_parameters(5);
  h                      = model_parameters(6);
  m                      = model_parameters(7);
  rho                    = model_parameters(8);
  initialconfiningstress = model_parameters(9);
  n                      = model_parameters(10);
  a                      = model_parameters(11);
  eplcum_cr              = model_parameters(12);
  eplcum                 = model_parameters(13);

  //Receive tensor data
  Matrix aux(3, 3);


  // 1. Receiving strainplcum
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive strainplcum\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      strainplcum(ii, jj) = aux(ii, jj);
    }
  }


  // 2. Receiving strainplcum
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive strainplcum\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      strainplcum(ii, jj) = aux(ii, jj);
    }
  }


  // 3. Receiving TrialStrain
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive TrialStrain\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      TrialStrain(ii, jj) = aux(ii, jj);
    }
  }


  // 4. Receiving TrialStress
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive TrialStress\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      TrialStress(ii, jj) = aux(ii, jj);
    }
  }


  // 5. Receiving TrialPlastic_Strain
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive TrialPlastic_Strain\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      TrialPlastic_Strain(ii, jj) = aux(ii, jj);
    }
  }


  // 6. Receiving ElasticStateStrain
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive ElasticStateStrain\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      ElasticStateStrain(ii, jj) = aux(ii, jj);
    }
  }


  // 7. Receiving ElasticStateStress
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive ElasticStateStress\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      ElasticStateStress(ii, jj) = aux(ii, jj);
    }
  }


  // 8. Receiving CommitStress
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive CommitStress\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      CommitStress(ii, jj) = aux(ii, jj);
    }
  }


  // 9. Receiving CommitStrain
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive CommitStrain\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      CommitStrain(ii, jj) = aux(ii, jj);
    }
  }


  // 10. Receiving CommitPlastic_Strain
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive CommitPlastic_Strain\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      CommitPlastic_Strain(ii, jj) = aux(ii, jj);
    }
  }


  // 11. Receiving alpha
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive alpha\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      alpha(ii, jj) = aux(ii, jj);
    }
  }


  // 12. Receiving alpha0
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive alpha0\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      alpha0(ii, jj) = aux(ii, jj);
    }
  }


  // 13. Receiving alpha0mem
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive alpha0mem\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      alpha0mem(ii, jj) = aux(ii, jj);
    }
  }


  // 14. Receiving strainpl0
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive strainpl0\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      strainpl0(ii, jj) = aux(ii, jj);
    }
  }


  // 15. Receiving Stress_n_minus_2
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive Stress_n_minus_2\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      Stress_n_minus_2(ii, jj) = aux(ii, jj);
    }
  }


  // 16. Receiving nij_dev
  if ( theChannel.receiveMatrix( 0, commitTag, aux ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive nij_dev\n";
    return -1;
  }
  for (int ii = 0; ii < 3; ii++)
  {
    for (int jj = 0; jj < 3; jj++)
    {
      nij_dev(ii, jj) = aux(ii, jj);
    }
  }


  //Receive the stiffness tensors (through a vector and wrap it)
  Vector aux_stifftensor(81);
  if ( theChannel.receiveVector( 0, commitTag, aux_stifftensor ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive Stiffness\n";
    return -1;
  }
  int comp = 0;
  for (int ii = 0; ii < 3; ii++)
    for (int jj = 0; jj < 3; jj++)
      for (int kk = 0; kk < 3; kk++)
        for (int ll = 0; ll < 3; ll++)
        {
          Stiffness(ii, jj, kk, ll) = aux_stifftensor(comp++);
        }

  if ( theChannel.receiveVector( 0, commitTag, aux_stifftensor ) < 0 )
  {
    cerr << "WARNING NewPisanoLT::receiveSelf() - failed to receive Stiffness\n";
    return -1;
  }
  comp = 0;
  for (int ii = 0; ii < 3; ii++)
    for (int jj = 0; jj < 3; jj++)
      for (int kk = 0; kk < 3; kk++)
        for (int ll = 0; ll < 3; ll++)
        {
          Ee(ii, jj, kk, ll) = aux_stifftensor(comp++);
        }



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

  return alpha;
}

//================================================================================
//  Plasticity!
//================================================================================

int NewPisanoLT::Explicit(const DTensor2 &strain_incr)
{
  //=============================================================================================
  // Some local definitions
  //=============================================================================================
  int err = 0;

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
  DTensor2 strainplcum_dev(3, 3, 0.0);
  DTensor2 alpha_trial(3, 3, 0.0);
  DTensor4 Ep(3, 3, 3, 3, 0.0);           // Elastoplastic tangent modulus tensor

  Index < 'p' > p;                        // Extra index for LTensor computations
  Index < 'q' > q;                        // Extra index for LTensor computations




  //---------------------------------------------------------------------------------------------
  // Get initial state
  //---------------------------------------------------------------------------------------------

  start_stress  = getCommittedStressTensor();
  start_strain  = getCommittedStrainTensor();
  start_Pstrain = getCommittedPlasticStrainTensor();

  incr_strain(i, j) = strain_incr(i, j); //Its seems superficial to have incr_strain and stress_incr, but the reason is that strain_incr is "const" and member function "compute_deviatoric_tensor" does not work with const tensors... which is wrong but it is what we have........ LTensor's fault :( -J.Abell

  double E = getE();

  G = E / (2.0 * (1.0 + v));
  K = E / (3.0 * (1.0 - 2.0 * v));

  start_stress.compute_deviatoric_tensor(s, pp);
  pp = -start_stress(i, i) / 3;
  incr_strain.compute_deviatoric_tensor(incr_strain_dev, incr_strain_vol);
  incr_strain_vol = incr_strain(i, i);

  stress_incr_prev(i, j) = start_stress(i, j) - Stress_n_minus_2(i, j);
  p_incr_prev = -stress_incr_prev(i, i) / 3;

  pressure_ratio = fabs(p_incr_prev / pp);




  //---------------------------------------------------------------------------------------------
  // Some healthy checkups
  //---------------------------------------------------------------------------------------------
  if ( (pressure_ratio < 1.0e-3) || (fabs(pp) < 1000) ) // this avoids the influence of very small pressure fluctuations on nij_dev
  {
    p_incr_prev = 0.0;
  }


  //---------------------------------------------------------------------------------------------
  // Define tangent to yield surface with the sign of the deviatoric strain increment
  //---------------------------------------------------------------------------------------------

  double norm_nij_dev;

  nij_dev(i, j) = 2 * G * incr_strain_dev(i, j) -  p_incr_prev * alpha(i, j); // and then normalization
  norm_nij_dev = sqrt(nij_dev(i, j) * nij_dev(i, j));

  if (norm_nij_dev > check_for_zero)
  {
    nij_dev(i, j) = nij_dev(i, j) / norm_nij_dev;
  }
  else
  {
    nij_dev(i, j) = ZeroStrain(i, j) ;
  }

  // if (fabs(incr_strain_dev(0, 2)) > check_for_zero)
  // {
  //     sign = incr_strain_dev(0, 2) / fabs(incr_strain_dev(0, 2));
  // }
  // else
  // {
  //     sign = 1.0;
  // }

  // nij_dev(0, 2) = sign * 0.7071;
  // nij_dev(2, 0) = sign * 0.7071;


  //---------------------------------------------------------------------------------------------
  // Check for unloading!!!
  //---------------------------------------------------------------------------------------------

  unload_prod = nij_dev(i, j) * alpha(i, j) - nij_dev(i, j) * alpha0(i, j) ;

  if (unload_prod <= 0.0)
  {

    if (eplcum >= eplcum_cr) // the projection center is first memorized and updated
    {
      alpha0mem(i, j) = alpha0(i, j);
      alpha0(i, j)    = alpha(i, j);
    }
    else // the previous projection center is reset
    {
      alpha0(i, j) = alpha0mem(i, j); // back stress is set as at the previous unloading
    }

    strainplcum(i, j) =  ZeroStrain(i, j); //TrialPlastic_Strain(i, j); // the local cumulated plastic strain is restarted
  }



  //---------------------------------------------------------------------------------------------
  // Compute distance and dilatancy coefficients
  //---------------------------------------------------------------------------------------------

  if ( abs(n) < check_for_zero)
  {
    beta = get_distance_coeff(start_stress);    // this should be able to read alpha and alpha0
    alpha_d(i, j) = nij_dev(i, j) * (kd * sqrt(2.0 / 3.0));
    D = xi * (alpha_d(i, j) - alpha(i, j) ) * nij_dev(i, j);
  }
  else
  {
    beta = get_distance_coeff_lode(start_stress); // this should be able to read alpha and alpha0
    D = get_dilatancy();
  }



  //---------------------------------------------------------------------------------------------
  // Compute plastic modulus (H)
  //---------------------------------------------------------------------------------------------

  if (m == 1.0)
  {
    H = pp * h * beta;
  }
  else
  {
    H = pp * h * (pow(beta, m));
  }

  //---------------------------------------------------------------------------------------------
  // Compute plastic flow direction (mij)
  //---------------------------------------------------------------------------------------------

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
  TrialPlastic_Strain(i, j) = start_Pstrain(i, j);
  TrialPlastic_Strain(i, j) += incr_Pstrain(i, j);

  //---------------------------------------------------------------------------------------------
  // Overshooting Remediation
  //---------------------------------------------------------------------------------------------
  strainplcum(i, j) += incr_Pstrain(i, j);

  strainplcum.compute_deviatoric_tensor(strainplcum_dev, strainplcum_vol); //check declaration!
  eplcum = sqrt((0.6666666) * strainplcum_dev(i, j) * strainplcum_dev(i, j));

  ep_stress.compute_deviatoric_tensor(ep_stress_dev, ep_stress_p);
  ep_stress_p = -ep_stress(i, i) / 3;

  // this avoids weird alpha values...it is difficult to find a general rule for the checking factor..tests needed
  // stress rapporto -> stress ratio

  if (ep_stress_p > 1000 * check_for_zero)
  {
    alpha(i, j) = ep_stress_dev(i, j);
    alpha /= ep_stress_p;
  }
  else
  {
    alpha(i, j) = ZeroStress(i, j);
  };

  //---------------------------------------------------------------------------------------------
  // Compute Elastic-Plastic stiffness
  //---------------------------------------------------------------------------------------------
  // To obtain Eep, at the last step

  Stiffness(p, q, i, j) =  nij_dev(i, j) * nij_dev(p, q) * (-4.0 * G * G / den) +
                           kronecker_delta(i, j)      * nij_dev(p, q) * (2.0 * G * K * D / den) +
                           nij_dev(i, j) * kronecker_delta(p, q)      * (-2.0 * G * K * alpha_nijdev / den) +
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

#ifdef DEBUG_PISANO
  LTensorDisplay::print(Stiffness, "Stiffness", "\n\n\n", 1);
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
  if (abs(xi) < check_for_zero)
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

  D = xi * (nij_dev(i, j) * (alphad(i, j) - alpha(i, j)));

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

  if (abs(s(i, i)) > check_for_zero )
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

  if (J2 > check_for_zero)
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

  if (a == 0)
  {
    return E0;
  }
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
  return alpha;
}
DTensor2 NewPisanoLT::get_alpha0() const
{
  return alpha0;
}
DTensor2 NewPisanoLT::get_alpha0mem() const
{
  return alpha0mem;
}
DTensor2 NewPisanoLT::get_strainpl0() const
{
  return strainpl0;
}
DTensor2 NewPisanoLT::get_Stress_n_minus_2() const
{
  return Stress_n_minus_2;
}
DTensor2 NewPisanoLT::get_nij_dev() const
{
  return nij_dev;
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
  size += sizeof(double) * ElasticStateStrain.get_size();
  // DTensor2 ElasticStateStress;
  size += sizeof(double) * ElasticStateStress.get_size();
  // DTensor2 CommitStress;
  size += sizeof(double) * CommitStress.get_size();
  // DTensor2 CommitStrain;
  size += sizeof(double) * CommitStrain.get_size();
  // DTensor2 CommitPlastic_Strain;
  size += sizeof(double) * CommitPlastic_Strain.get_size();
  // DTensor2 alpha;             // back stress ratio tensor
  size += sizeof(double) * alpha.get_size();
  // DTensor2 alpha0;            // back stress ratio tensor
  size += sizeof(double) * alpha0.get_size();
  // DTensor2 alpha0mem;         // back stress ratio tensor at last previous stress reversal
  size += sizeof(double) * alpha0mem.get_size();
  // DTensor2 strainpl0;         // Plastic strain at stress reversal (for overshooting)
  size += sizeof(double) * strainpl0.get_size();
  // DTensor2 Stress_n_minus_2;  // Stress tensor at step n-2
  size += sizeof(double) * Stress_n_minus_2.get_size();
  // DTensor2 nij_dev;           // direction of deviatoric plastic strain increment(unit tensor)
  size += sizeof(double) * nij_dev.get_size();


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

#endif

