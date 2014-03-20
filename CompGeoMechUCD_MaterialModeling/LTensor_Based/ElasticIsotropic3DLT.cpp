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
// DATE:              January 2010
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

// ElasticIsotropic3DLT is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.

#include "ElasticIsotropic3DLT.h"


const  DTensor2 ElasticIsotropic3DLT::ZeroStrain( 3, 3, 0.0 );
const  DTensor2 ElasticIsotropic3DLT::ZeroStress( 3, 3, 0.0 );
//DTensor4 ElasticIsotropic3DLT::Ee( 3, 3, 3, 3, 0.0 );


//================================================================================
ElasticIsotropic3DLT::ElasticIsotropic3DLT( int tag,
        double E_in,
        double v_in,
        double rho_in)
    : NDMaterialLT( tag, ND_TAG_ElasticIsotropic3DLT ),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      ElasticStateStrain( 3, 3, 0.0 ),
      ElasticStateStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      Stiffness( 3, 3, 3, 3, 0.0 ),
      E( E_in ),
      v( v_in ),
      rho( rho_in ),
      Ee(3, 3, 3, 3, 0.0)
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
    DTensor2 result( 3, 3, 0.0 );
    result( i, j ) = v( i, j ) - TrialStrain( i, j );

    return setTrialStrainIncr( result );
}

//================================================================================
int ElasticIsotropic3DLT::setTrialStrainIncr( const DTensor2 &strain_increment )
{
    return 0;
}


//================================================================================
const DTensor4 &ElasticIsotropic3DLT::getTangentTensor( void )
{
    return Stiffness;
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
    return TrialStrain;
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
    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;


    TrialStress = CommitStress;
    TrialStrain = CommitStrain;

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
NDMaterialLT *ElasticIsotropic3DLT::getCopy( void )
{
    NDMaterialLT *tmp = new ElasticIsotropic3DLT( this->getTag(),
            this->getE(),
            this->getv(),
            this->getRho() );

    return tmp;
}


//================================================================================
NDMaterialLT *ElasticIsotropic3DLT::getCopy( const char *code )
{
    if ( strcmp( code, "ThreeDimensional" ) == 0 )
    {
        ElasticIsotropic3DLT *tmp = new ElasticIsotropic3DLT( this->getTag(),
                this->getE(),
                this->getv(),
                this->getRho());

        return tmp;
    }
    else
    {
        cout << "ElasticIsotropic3DLT::getCopy failed to get model: " <<  code << endln;
        exit( 1 );
    }

    return 0;
}

//================================================================================
const char *ElasticIsotropic3DLT::getType( void ) const
{
    return "ThreeDimensional";
}

//================================================================================
int ElasticIsotropic3DLT::sendSelf( int commitTag, Channel &theChannel )
{
    return 0;
}

int ElasticIsotropic3DLT::recvSelf( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{
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
