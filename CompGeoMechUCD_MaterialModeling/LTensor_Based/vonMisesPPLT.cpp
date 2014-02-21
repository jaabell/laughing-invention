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

// vonMisesPPLT is another form of NewTemplate3Dep which has all
// the functions embeded here for efficiency.

#include "vonMisesPPLT.h"


const  DTensor2 vonMisesPPLT::ZeroStrain( 3, 3, 0.0 );
const  DTensor2 vonMisesPPLT::ZeroStress( 3, 3, 0.0 );
//DTensor4 vonMisesPPLT::Ee( 3, 3, 3, 3, 0.0 );


//================================================================================
vonMisesPPLT::vonMisesPPLT( int tag,
                            double E_in,
                            double v_in,
                            double k_in,
                            double rho_in,
                            double initialconfiningstress_in,
                            int maximum_number_of_iterations,
                            double tolerance_1,
                            double tolerance_2 )
    : NDMaterialLT( tag, ND_TAG_vonMisesPPLT ),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      TrialPlastic_Strain( 3, 3, 0.0 ),
      ElasticStateStrain( 3, 3, 0.0 ),
      ElasticStateStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      CommitPlastic_Strain( 3, 3, 0.0 ),
      Stiffness( 3, 3, 3, 3, 0.0 ),
      E( E_in ),
      v( v_in ),
      k( k_in ),
      rho( rho_in ),
      initialconfiningstress( initialconfiningstress_in ),
      TOLERANCE1( tolerance_1 ), TOLERANCE2( tolerance_2 ),
      ISMAX( maximum_number_of_iterations ),
      Ee(3, 3, 3, 3, 0.0)
{


    ElasticStateStress( 0, 0 ) = -initialconfiningstress_in;
    ElasticStateStress( 1, 1 ) = -initialconfiningstress_in;
    ElasticStateStress( 2, 2 ) = -initialconfiningstress_in;

    revertToStart();
}


// Destructor
//================================================================================
vonMisesPPLT::~vonMisesPPLT()
{

}

//================================================================================
int vonMisesPPLT::setTrialStrain( const DTensor2& v )
{
    DTensor2 result( 3, 3, 0.0 );
    result( i, j ) = v( i, j ) - TrialStrain( i, j );

    return setTrialStrainIncr( result );
}

//================================================================================
int vonMisesPPLT::setTrialStrainIncr( const DTensor2& strain_increment )
{
    Explicit( strain_increment );

    return 0;
}


//================================================================================
const DTensor4& vonMisesPPLT::getTangentTensor( void )
{
    return Stiffness;
}

//================================================================================
const DTensor2&  vonMisesPPLT::getStressTensor( void )
{
    return TrialStress;
}


//================================================================================
const DTensor2& vonMisesPPLT::getStrainTensor( void )
{
    return TrialStrain;
}

//================================================================================
const DTensor2& vonMisesPPLT::getPlasticStrainTensor( void )
{
    return TrialPlastic_Strain;
}

//================================================================================
int vonMisesPPLT::commitState( void )
{
    CommitStress = TrialStress;
    CommitStrain = TrialStrain;
    CommitPlastic_Strain = TrialPlastic_Strain;

    return 0;
}

//================================================================================
int vonMisesPPLT::revertToLastCommit( void )
{

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    TrialPlastic_Strain = CommitPlastic_Strain;

    return 0;
}

//================================================================================
int vonMisesPPLT::revertToStart( void )
{
    CommitStress = ElasticStateStress;
    CommitStrain = ElasticStateStrain;
    CommitPlastic_Strain = ZeroStrain;

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;
    TrialPlastic_Strain = ZeroStrain;

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
NDMaterialLT* vonMisesPPLT::getCopy( void )
{
    NDMaterialLT* tmp = new vonMisesPPLT( this->getTag(),
                                          this->getE(),
                                          this->getv(),
                                          this->getk(),
                                          this->getRho(),
                                          this->getInitialConfiningStress(),
                                          this->ISMAX,
                                          this->TOLERANCE1,
                                          this->TOLERANCE2 );

    return tmp;
}


//================================================================================
NDMaterialLT* vonMisesPPLT::getCopy( const char* code )
{
    if ( strcmp( code, "ThreeDimensional" ) == 0 )
    {
        vonMisesPPLT* tmp = new vonMisesPPLT( this->getTag(),
                                              this->getE(),
                                              this->getv(),
                                              this->getk(),
                                              this->getRho(),
                                              this->getInitialConfiningStress(),
                                              this->ISMAX,
                                              this->TOLERANCE1,
                                              this->TOLERANCE2 );

        return tmp;
    }
    else
    {
        cout << "vonMisesPPLT::getCopy failed to get model: " <<  code << endln;
        exit( 1 );
    }

    return 0;
}

//================================================================================
const char* vonMisesPPLT::getType( void ) const
{
    return "ThreeDimensional";
}

//================================================================================
int vonMisesPPLT::sendSelf( int commitTag, Channel& theChannel )
{
    return 0;
}

int vonMisesPPLT::recvSelf( int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker )
{
    return 0;
}

//================================================================================
void vonMisesPPLT::Print( ostream& s, int flag )
{
    s << ( *this );
}

//================================================================================
double vonMisesPPLT::getE()
{
    return E;
}

//================================================================================
double vonMisesPPLT::getv()
{
    return v;
}

//================================================================================
double vonMisesPPLT::getk()
{
    return k;
}

//================================================================================
double vonMisesPPLT::getRho( void )
{
    return rho;
}

//================================================================================
double vonMisesPPLT::getInitialConfiningStress( void )
{
    return initialconfiningstress;
}


//================================================================================
double vonMisesPPLT::YieldFunctionValue( const DTensor2& Stre ) const
{

    // f2 = [(sij-aij)*(sij-aij)]^0.5 - sqrt(2.0/3)*k

    DTensor2 s_bar( 3, 3, 0.0 );
    double mean_stress;
    DTensor2 tempstress( 3, 3, 0.0 );
    tempstress = Stre;
    tempstress.compute_deviatoric_tensor( s_bar, mean_stress );


    //  double s_norm  = sqrt( (s_bar("ij")*s_bar("ij")).trace() );
    double sijsijtrace = s_bar( i, j ) * s_bar( i, j );
    double s_norm  = sqrt( sijsijtrace );

    double YF = s_norm - sqrt( 2.0 / 3.0 ) * k;
    return YF;

}

//================================================================================
int vonMisesPPLT::Explicit( const DTensor2& strain_incr )
{

    int err = 0;
    double f_start = 0.0;
    double f_pred  = 0.0;

    DTensor4 Ep( 3, 3, 3, 3, 0.0 );

    DTensor2 intersection_strain( 3, 3, 0 );
    DTensor2 intersection_stress( 3, 3, 0 );
    double intersection_factor = 0.0;

    double Delta_lambda = 0.0;

    DTensor2 nij( 3, 3, 0 );
    DTensor2 incr_strain( 3, 3, 0 );
    DTensor2 incr_stress( 3, 3, 0 );
    DTensor2 incr_Pstrain( 3, 3, 0 );
    DTensor2 ep_stress( 3, 3, 0 );
    DTensor2 predicted_stress( 3, 3, 0 );

    DTensor2 start_stress( 3, 3, 0 );
    DTensor2 start_strain( 3, 3, 0 );
    DTensor2 start_Pstrain( 3, 3, 0 );

    start_stress  = getStressTensor();
    start_strain  = getStrainTensor();
    start_Pstrain = getPlasticStrainTensor();

    intersection_stress = start_stress;

    ElasticStateStress = start_stress;
    ElasticStateStrain = start_strain;

    double mu = E / ( 2 * ( 1 + v ) );


    incr_strain = strain_incr;

    Index < 'p' > p;
    Index < 'q' > q;

    incr_stress( i, j ) = Ee( i, j, p, q ) * incr_strain( p, q );


    predicted_stress( i, j ) = start_stress( i, j ) + incr_stress( i, j );


    f_start = YieldFunctionValue( start_stress );
    f_pred  = YieldFunctionValue( predicted_stress );


    // If Elastic
    if ( ( f_start < 0.0 && f_pred <= TOLERANCE2 ) || f_start > f_pred )
    {
        TrialStrain( i, j ) = start_strain( i, j ) + incr_strain( i, j );
        TrialStress =  predicted_stress ;

        Stiffness = Ee;
    }
    else
    {
        // If Elastic and then Elastic-Plastic
        if ( f_start < 0.0 )
        {
            intersection_factor = zbrentstress( start_stress, predicted_stress, 0.0, 1.0, TOLERANCE1 );
            intersection_stress = yield_surface_cross( start_stress, predicted_stress, intersection_factor );
            intersection_strain( i, j ) = start_strain( i, j ) + ( incr_strain ( i, j ) * intersection_factor );

            incr_stress( i, j ) = predicted_stress( i, j ) - intersection_stress( i, j ); // necessary


            ElasticStateStrain = intersection_strain;
            ElasticStateStress = intersection_stress;
        }

        // If E-P Response,
        Delta_lambda = 0.0;


        /////////////////////////////////////////////////////////////////////////
        // nij = mij
        // f2 = [(sij-aij)*(sij-aij)]^0.5 - sqrt(2.0/3)*k
        DTensor2 nij( 3, 3, 0 );

        DTensor2 s_bar( 3, 3, 0 );
        double p0;
        intersection_stress.compute_deviatoric_tensor( s_bar, p0 );



        double sijsijtrace = s_bar( i, j ) * s_bar( i, j );

        double s_norm  = sqrt( sijsijtrace );

        if ( s_norm != 0.0 )
        {
            //        nij = s_bar *(1.0/s_norm);

            double normtemp = 1.0 / s_norm;

            nij( 0, 0 ) = s_bar( 0, 0 ) * normtemp;
            nij( 0, 1 ) = s_bar( 0, 1 ) * normtemp;
            nij( 0, 2 ) = s_bar( 0, 2 ) * normtemp;
            nij( 1, 0 ) = s_bar( 1, 0 ) * normtemp;
            nij( 1, 1 ) = s_bar( 1, 1 ) * normtemp;
            nij( 1, 2 ) = s_bar( 1, 2 ) * normtemp;
            nij( 2, 0 ) = s_bar( 2, 0 ) * normtemp;
            nij( 2, 1 ) = s_bar( 2, 1 ) * normtemp;
            nij( 2, 2 ) = s_bar( 2, 2 ) * normtemp;
        }
        else if ( s_norm < 0 )
        {
            cerr << "ERROR vonMisesPPLT::(s_norm < 0)\n ";
            exit( 1 );
        }


        //  ( nij("ij") * incr_stress("ij") ).trace();
        double nijsijtrace = nij( i, j ) * incr_stress( i, j );


        //      Delta_lambda = ( nij("ij") * incr_stress("ij") ).trace() / (2 * mu);
        Delta_lambda = nijsijtrace / ( 2 * mu );


        if ( Delta_lambda < 0.0 )
        {
            Delta_lambda = 0.0;
        }


        /////////////////////////////////////////////////////////////////////////
        // Plastic strain increment


        //      incr_Pstrain = nij * Delta_lambda;

        incr_Pstrain( i, j ) = nij( i, j ) * Delta_lambda;

        /////////////////////////////////////////////////////////////////////////
        //      ep_stress = predicted_stress - (nij * multiplytemp);

        DTensor2 nijmultipliedby2mu(3, 3, 0.0);

        nijmultipliedby2mu( i, j ) = nij( i, j ) * ( 2 * mu * Delta_lambda );


        ep_stress( i, j ) = predicted_stress( i, j ) - nijmultipliedby2mu( i, j );
        /////////////////////////////////////////////////////////////////////////

        TrialStress = ep_stress ;
        TrialStrain( i, j ) = start_strain( i, j ) + incr_strain( i, j );
        TrialPlastic_Strain( i, j ) = start_Pstrain( i, j ) + incr_Pstrain( i, j );

        Ep( i, j, p, q ) = nij( i, j ) * nij( p, q ) * 2 * mu;

        /////////////////////////////////////////////////////////////////////////

        if ( Delta_lambda > 0.0 )
        {
            Stiffness( i, j, p, q ) = Ee( i, j, p, q ) - Ep( i, j, p, q );
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
DTensor2 vonMisesPPLT::yield_surface_cross( const DTensor2& start_stress,
        const DTensor2& end_stress, double a )
{
    DTensor2 delta_stress( 3, 3, 0 );
    delta_stress( i, j ) = end_stress( i, j ) - start_stress( i, j );
    DTensor2 intersection_stress( 3, 3, 0 );
    intersection_stress( i, j ) = start_stress( i, j ) + delta_stress( i, j ) * a;

    return intersection_stress;
}

// Routine used by yield_surface_cross to find the DTensor2 at cross point
//================================================================================
double vonMisesPPLT::zbrentstress( const DTensor2& start_stress,
                                   const DTensor2& end_stress,
                                   double x1, double x2, double tol ) const
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

    DTensor2 alfa_stress_1( 3, 3, 0 );
    alfa_stress_1( i, j ) = ( start_stress( i, j ) * ( 1.0 - a ) ) + ( end_stress( i, j ) * a );
    DTensor2 alfa_stress_2( 3, 3, 0 );
    alfa_stress_2( i, j ) = alfa_stress_2( i, j ) = ( start_stress( i, j ) * ( 1.0 - b ) ) + ( end_stress( i, j ) * b );

    double fa = YieldFunctionValue( alfa_stress_1 );
    double fb = YieldFunctionValue( alfa_stress_2 );

    if ( ( fb * fa ) > 0.0 )
    {
        cout << "\a\n Root must be bracketed in ZBRENTstress " << endln;
        exit( 1 );
    }

    fc = fb;

    for ( iter = 1; iter <= ISMAX; iter++ )
    {
        if ( ( fb * fc ) > 0.0 )
        {
            c = a;
            fc = fa;
            e = d = b - a;
        }

        if ( fabs( fc ) < fabs( fb ) )
        {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }

        tol1 = 2.0 * EPS * fabs( b ) + 0.5 * tol;
        xm = 0.5 * ( c - b );

        if ( fabs( xm ) <= tol1 || fb == 0.0 )
        {
            return b;
        }

        if ( fabs( e ) >= tol1 && fabs( fa ) > fabs( fb ) )
        {
            s = fb / fa;

            if ( a == c )
            {
                p = 2.0 * xm * s;
                q = 1.0 - s;
            }
            else
            {
                q = fa / fc;
                r = fb / fc;
                p = s * ( 2.0 * xm * q * ( q - r ) - ( b - a ) * ( r - 1.0 ) );
                q = ( q - 1.0 ) * ( r - 1.0 ) * ( s - 1.0 );
            }

            if ( p > 0.0 )
            {
                q = -q;
            }

            p = fabs( p );
            min1 = 3.0 * xm * q - fabs( tol1 * q );
            min2 = fabs( e * q );

            if ( 2.0 * p < ( min1 < min2 ? min1 : min2 ) )
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

        if ( fabs( d ) > tol1 )
        {
            b += d;
        }
        else
        {
            b += ( xm > 0.0 ? fabs( tol1 ) : -fabs( tol1 ) );
        }

        DTensor2 alfa_stress_3( 3, 3, 0 );
        alfa_stress_3( i, j ) = ( start_stress( i, j ) * ( 1.0 - b ) ) + ( end_stress( i, j ) * b );
        fb = YieldFunctionValue( alfa_stress_3 );
    }

    return 0.0;
}


