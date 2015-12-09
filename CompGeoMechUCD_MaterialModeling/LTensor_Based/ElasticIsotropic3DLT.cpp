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
// PROGRAMMER:        Jose Abell, Nima Tafazzoli, Boris Jeremic
// DATE:              January 2010
// UPDATE HISTORY:    Dec 2015, Optimized memory usage.
//
//
/////////////////////////////////////////////////////////////////////////////

#include "ElasticIsotropic3DLT.h"
#include <Channel.h>
#include <HDF5_Channel.h>
#include <Matrix.h>
#include <ID.h>
#include <Vector.h>

const  DTensor2 ElasticIsotropic3DLT::ZeroStrain( 3, 3, 0.0 );
const  DTensor2 ElasticIsotropic3DLT::ZeroStress( 3, 3, 0.0 );
DTensor4 ElasticIsotropic3DLT::Ee( 3, 3, 3, 3, 0.0 );



//================================================================================
ElasticIsotropic3DLT::ElasticIsotropic3DLT( int tag,
        double E_in,
        double v_in,
        double rho_in)
    : NDMaterialLT( tag, ND_TAG_ElasticIsotropic3DLT ),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      E( E_in ),
      v( v_in ),
      rho( rho_in )
{


    revertToStart();
}


//================================================================================
ElasticIsotropic3DLT::ElasticIsotropic3DLT( )
    : NDMaterialLT( 0, ND_TAG_ElasticIsotropic3DLT ),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      E( 0 ),
      v( 0 ),
      rho( 0 )
{


    revertToStart();
}



// Destructor
//================================================================================
ElasticIsotropic3DLT::~ElasticIsotropic3DLT()
{

}

//================================================================================
int ElasticIsotropic3DLT::setTrialStrain( const DTensor2 &v )
{
    compute_tangent_tensor();
    TrialStrain(i, j) = v(i, j);
    TrialStress(i, j) = Ee(i, j, k, l) * TrialStrain(k, l);
    return 0;
}

//================================================================================
int ElasticIsotropic3DLT::setTrialStrainIncr( const DTensor2 &strain_increment )
{
    return 0;
}


//================================================================================
const DTensor4 &ElasticIsotropic3DLT::getTangentTensor( void )
{

    compute_tangent_tensor();

    return Ee;
}


void ElasticIsotropic3DLT::compute_tangent_tensor(void)
{
    Ee *= 0;

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
}


//================================================================================
const DTensor2  &ElasticIsotropic3DLT::getStressTensor( void )
{
    return TrialStress;
}


//================================================================================
const DTensor2 &ElasticIsotropic3DLT::getStrainTensor( void )
{
    return TrialStrain;
}

//================================================================================
const DTensor2 &ElasticIsotropic3DLT::getPlasticStrainTensor( void )
{
    return ZeroStrain;
}

//================================================================================
int ElasticIsotropic3DLT::commitState( void )
{
    CommitStress = TrialStress;
    CommitStrain = TrialStrain;

    return 0;
}

//================================================================================
int ElasticIsotropic3DLT::revertToLastCommit( void )
{

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;

    return 0;
}

//================================================================================
int ElasticIsotropic3DLT::revertToStart( void )
{
    CommitStress *= 0;
    CommitStrain *= 0;


    TrialStress *= 0;
    TrialStrain *= 0;

    return 0;
}

//================================================================================
NDMaterialLT *ElasticIsotropic3DLT::getCopy( void )
{
    NDMaterialLT *tmp = new ElasticIsotropic3DLT( this->getTag(),
            this->getE(),
            this->getv(),
            this->getRho() );

    return tmp;
}


//================================================================================
const char *ElasticIsotropic3DLT::getType( void ) const
{
    return "ThreeDimensional";
}




//================================================================================

int ElasticIsotropic3DLT::sendSelf( int commitTag, Channel &theChannel )
{

    static ID idData(1);
    static Vector vectorData(3);
    static Matrix a(3, 3);

    idData(0) = this->getTag();

    if (theChannel.sendID(0, commitTag, idData) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send idData\n";
        return -1;
    }

    vectorData(0) = E;
    vectorData(1) = v;
    vectorData(2) = rho;

    if (theChannel.sendVector(0, commitTag, vectorData) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send vectorData\n";
        return -1;
    }


    a.setData(CommitStress.data, 3, 3);
    if (theChannel.sendMatrix(0, 0, a) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send Elastic Constant strain\n";
        return -1;
    }

    a.setData(CommitStrain.data, 3, 3);
    if (theChannel.sendMatrix(0, 0, a) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send Elastic Constant Tensor\n";
        return -1;
    }


    return 0;
}

int ElasticIsotropic3DLT::receiveSelf( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    static ID idData(1);
    static Vector vectorData(3);
    static Matrix a(3, 3);


    if (theChannel.receiveID(0, commitTag, idData) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send idData\n";
        return -1;
    }
    this->setTag(idData(0));


    if (theChannel.receiveVector(0, commitTag, vectorData) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send vectorData\n";
        return -1;
    }
    v = vectorData(1);
    rho = vectorData(2);
    E = vectorData(0);


    if (theChannel.receiveMatrix(0, 0, a) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send Elastic Constant strain\n";
        return -1;
    }
    for (int ii = 0; ii < 3; ii++)
        for (int jj = 0; jj < 3; jj++)

        {
            CommitStress(ii, jj) = a(ii, jj);
        }
    if (theChannel.receiveMatrix(0, 0, a) < 0)
    {
        cerr << "ElasticIsotropic3DLT::sendSelf -- could not send Elastic Constant Tensor\n";
        return -1;
    }
    for (int ii = 0; ii < 3; ii++)
        for (int jj = 0; jj < 3; jj++)

        {
            CommitStrain(ii, jj) = a(ii, jj);
        }

    compute_tangent_tensor();


    return 0;
}

//================================================================================
void ElasticIsotropic3DLT::Print( ostream &s, int flag )
{
    s << ( *this );
}

//================================================================================
double ElasticIsotropic3DLT::getE()
{
    return E;
}

//================================================================================
double ElasticIsotropic3DLT::getv()
{
    return v;
}


//================================================================================
double ElasticIsotropic3DLT::getRho( void )
{
    return rho;
}



int ElasticIsotropic3DLT::getObjectSize()
{
    int size = 0;
    // DTensor2 TrialStrain;
    size += sizeof(double) * TrialStrain.get_size();
    // DTensor2 TrialStress;
    size += sizeof(double) * TrialStress.get_size();
    // DTensor2 CommitStress;
    size += sizeof(double) * CommitStress.get_size();
    // DTensor2 CommitStrain;
    size += sizeof(double) * CommitStrain.get_size();
    // double E;
    size += sizeof(E);
    // double v;
    size += sizeof(v);
    // double rho;
    size += sizeof(rho);
    // DTensor4 Ee;
    size += sizeof(double) * Ee.get_size();
    // Index < 'i' > i;
    size += sizeof(i);
    // Index < 'j' > j;
    size += sizeof(j);
    // Index < 'k' > k;
    size += sizeof(k);
    // Index < 'l' > l;
    size += sizeof(l);


    return size;
}
