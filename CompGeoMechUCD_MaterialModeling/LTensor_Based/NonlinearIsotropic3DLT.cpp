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

#include "NonlinearIsotropic3DLT.h"
#include <Channel.h>
#include <HDF5_Channel.h>
#include <Matrix.h>
#include <ID.h>
#include <Vector.h>

const  DTensor2 NonlinearIsotropic3DLT::ZeroStrain( 3, 3, 0.0 );
const  DTensor2 NonlinearIsotropic3DLT::ZeroStress( 3, 3, 0.0 );
DTensor4 NonlinearIsotropic3DLT::Ee( 3, 3, 3, 3, 0.0 );

void printTensor(string name, const DTensor2 &v)
{


    std::cout << name << " = [ (" ;
    std::cout << v(0, 0) << " "
              << v(1, 1) << " "
              << v(2, 2) << ") ("
              << v(0, 1) << " "
              << v(0, 2) << " "
              << v(1, 2) << ") ("
              << v(1, 0) << " "
              << v(2, 0) << " "
              << v(2, 1) << ") ]" << std::endl;
}


//================================================================================
NonlinearIsotropic3DLT::NonlinearIsotropic3DLT(int tag,
        double rho_, double K_,
        double Kur_, double n_, double c_, double phi0_, double dphi_,
        double Rf_, double K0_, double Kb_, double m_, double pa_,
        double K2_, double B_, double Et_, double Ei_, double Er_)
    :
    NDMaterialLT( tag, ND_TAG_NonlinearIsotropic3DLT ),
    TrialStrain( 3, 3, 0.0 ),
    TrialStress( 3, 3, 0.0 ),
    CommitStress( 3, 3, 0.0 ),
    CommitStrain( 3, 3, 0.0 ),
    rho(rho_),
    K(K_),
    Kur(Kur_),
    n(n_),
    c(c_),
    phi0(phi0_),
    dphi(dphi_),
    Rf(Rf_),
    K0(K0_),
    Kb(Kb_),
    m(m_),
    pa(pa_),
    K2(K2_),
    B(B_),
    Et(Et_),
    Ei(Ei_),
    Er(Er_),
    E(0), nu(0), strlev_max(0), initialize(true)
{
    TrialStress(0, 0) = TrialStress(1, 1) = TrialStress(2, 2) = -pa / 1000;
    CommitStress(0, 0) = CommitStress(1, 1) = CommitStress(2, 2) = -pa / 1000;
}


//================================================================================
NonlinearIsotropic3DLT::NonlinearIsotropic3DLT( )
    : NDMaterialLT( 0, ND_TAG_NonlinearIsotropic3DLT ),
      TrialStrain( 3, 3, 0.0 ),
      TrialStress( 3, 3, 0.0 ),
      CommitStress( 3, 3, 0.0 ),
      CommitStrain( 3, 3, 0.0 ),
      rho(0.0),
      K(0.0),
      Kur(0.0),
      n(0.0),
      c(0.0),
      phi0(0.0),
      dphi(0.0),
      Rf(0.0),
      K0(0.0),
      Kb(0.0),
      m(0.0),
      pa(0.0),
      K2(0.0),
      B(0.0),
      Et(0.0),
      Ei(0.0),
      Er(0.0),
      E(0), nu(0), strlev_max(0), initialize(true)
{

}



// Destructor
//================================================================================
NonlinearIsotropic3DLT::~NonlinearIsotropic3DLT()
{

}

//================================================================================
int NonlinearIsotropic3DLT::setTrialStrain( const DTensor2 &strain )
{
    static DTensor2 strain_increment(3, 3, 0);
    strain_increment *= 0;

    TrialStrain(i, j) = strain(i, j);
    strain_increment(i, j) = strain(i, j) - CommitStrain(i, j);

    return setTrialStrainIncr(strain_increment);
}

//================================================================================
int NonlinearIsotropic3DLT::setTrialStrainIncr( const DTensor2 &strain_increment )
{
    static DTensor2 stress_increment(3, 3, 0);
    stress_increment *= 0;

    compute_tangent_tensor();

    double lambda = ( nu * E ) / ( ( 1 + nu ) * ( 1 - 2 * nu ) );
    double mu = E / ( 2 * ( 1 + nu ) );
    double lambda_eps_ii = lambda * strain_increment(i, i);

    stress_increment(i, j) = 2 * mu * strain_increment(i, j);
    stress_increment(0, 0) += lambda_eps_ii;
    stress_increment(1, 1) += lambda_eps_ii;
    stress_increment(2, 2) += lambda_eps_ii;

    TrialStress(i, j) = CommitStress(i, j);
    TrialStress(i, j) += stress_increment(i, j);

    return 0;
}


//================================================================================
const DTensor4 &NonlinearIsotropic3DLT::getTangentTensor( void )
{

    compute_tangent_tensor();

    return Ee;
}


void NonlinearIsotropic3DLT::compute_tangent_tensor(void)
{
    Ee *= 0;

    updateModulus();

    double lambda = ( nu * E ) / ( ( 1 + nu ) * ( 1 - 2 * nu ) );
    double mu = E / ( 2 * ( 1 + nu ) );

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
const DTensor2  &NonlinearIsotropic3DLT::getStressTensor( void )
{
    return TrialStress;
}


//================================================================================
const DTensor2 &NonlinearIsotropic3DLT::getStrainTensor( void )
{
    return TrialStrain;
}

//================================================================================
const DTensor2 &NonlinearIsotropic3DLT::getPlasticStrainTensor( void )
{
    return ZeroStrain;
}

//================================================================================
int NonlinearIsotropic3DLT::commitState( void )
{
    CommitStress = TrialStress;
    CommitStrain = TrialStrain;

    return 0;
}

//================================================================================
int NonlinearIsotropic3DLT::revertToLastCommit( void )
{

    TrialStress = CommitStress;
    TrialStrain = CommitStrain;

    return 0;
}

//================================================================================
int NonlinearIsotropic3DLT::revertToStart( void )
{
    CommitStress *= 0;
    CommitStrain *= 0;


    TrialStress *= 0;
    TrialStrain *= 0;

    return 0;
}

//================================================================================
NDMaterialLT *NonlinearIsotropic3DLT::getCopy( void )
{
    NDMaterialLT *tmp = new NonlinearIsotropic3DLT(  this->getTag(),
            rho,  K,
            Kur,  n,  c,  phi0,  dphi,
            Rf,  K0,  Kb,  m,  pa,
            K2,  B,  Et,  Ei,  Er);

    return tmp;
}


//================================================================================
const char *NonlinearIsotropic3DLT::getType( void ) const
{
    return "ThreeDimensional";
}




//================================================================================

int NonlinearIsotropic3DLT::sendSelf( int commitTag, Channel &theChannel )
{

    static ID idData(1);
    static Vector vectorData(17);
    static Matrix a(3, 3);

    idData(0) = this->getTag();

    if (theChannel.sendID(0, commitTag, idData) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send idData\n";
        return -1;
    }

    vectorData(0) = rho;
    vectorData(1) = K;

    vectorData(2) = Kur;
    vectorData(3) = n;
    vectorData(4) = c;
    vectorData(5) = phi0;
    vectorData(6) = dphi;

    vectorData(7) = Rf;
    vectorData(8) = K0;
    vectorData(9) = Kb;
    vectorData(10) = m;
    vectorData(11) = pa;

    vectorData(12) = K2;
    vectorData(13) = B;
    vectorData(14) = Et;
    vectorData(15) = Ei;
    vectorData(16) = Er;

    if (theChannel.sendVector(0, commitTag, vectorData) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send vectorData\n";
        return -1;
    }


    a.setData(CommitStress.data, 3, 3);
    if (theChannel.sendMatrix(0, 0, a) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send Elastic Constant strain\n";
        return -1;
    }

    a.setData(CommitStrain.data, 3, 3);
    if (theChannel.sendMatrix(0, 0, a) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send Elastic Constant Tensor\n";
        return -1;
    }


    return 0;
}

int NonlinearIsotropic3DLT::receiveSelf( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    static ID idData(1);
    static Vector vectorData(17);
    static Matrix a(3, 3);


    if (theChannel.receiveID(0, commitTag, idData) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send idData\n";
        return -1;
    }
    this->setTag(idData(0));


    if (theChannel.receiveVector(0, commitTag, vectorData) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send vectorData\n";
        return -1;
    }
    rho = vectorData(0);
    K = vectorData(1);

    Kur = vectorData(2);
    n = vectorData(3);
    c = vectorData(4);
    phi0 = vectorData(5);
    dphi = vectorData(6);

    Rf = vectorData(7);
    K0 = vectorData(8);
    Kb = vectorData(9);
    m = vectorData(10);
    pa = vectorData(11);

    K2 = vectorData(12);
    B = vectorData(13);
    Et = vectorData(14);
    Ei = vectorData(15);
    Er = vectorData(16);

    TrialStress(0, 0) = TrialStress(1, 1) = TrialStress(2, 2) = -pa;


    if (theChannel.receiveMatrix(0, 0, a) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send Elastic Constant strain\n";
        return -1;
    }
    for (int ii = 0; ii < 3; ii++)
        for (int jj = 0; jj < 3; jj++)

        {
            CommitStress(ii, jj) = a(ii, jj);
        }
    if (theChannel.receiveMatrix(0, 0, a) < 0)
    {
        cerr << "NonlinearIsotropic3DLT::sendSelf -- could not send Elastic Constant Tensor\n";
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
void NonlinearIsotropic3DLT::Print( ostream &s, int flag )
{
    s << "NonlinearIsotropic3DLT::" << endl;
    s << "\tTag: " << this->getTag() << endln;
    s << "\tElastic_Modulus: " << E << endln;
    s << "\tPoissons_Ratio: " << nu << endln;
    s << "\tDensity: " << rho << endln;
    s << "!!Please Implement me!! " << endl;
}

//================================================================================
double NonlinearIsotropic3DLT::getE()
{
    return E;
}

//================================================================================
double NonlinearIsotropic3DLT::getv()
{
    return nu;
}


//================================================================================
double NonlinearIsotropic3DLT::getRho( void )
{
    return rho;
}



int NonlinearIsotropic3DLT::getObjectSize()
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
    size += sizeof(nu);
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



inline void getPrincipalStresses(DTensor2 & sigma, double& sig1, double& sig2, double& sig3)
{
    double sx = sigma(0, 0);
    double sy = sigma(1, 1);
    double sz = sigma(2, 2);
    double sxy = sigma(0, 1);
    double sxz = sigma(0, 2);
    double syz = sigma(1, 2);

    // double p = -(sx + sy + sz) / 3;
    double I1  = sx + sy + sz;
    double I2  =  sx * sy + sy * sz + sz * sx - sxy * sxy - sxz * sxz - syz * syz;
    double I3  = sx * sy * sz -
                 sx * (syz * syz ) -
                 sy * (sxz * sxz) -
                 sz * (sxy * sxy) + 2 * sxy * sxz * syz;
    double num = ( 2 * I1 * I1 * I1 - 9 * I1 * I2 + 27 * I3);
    double den = (2 * pow(I1 * I1 - 3 * I2 , 1.5));

// # if {den == 0.} {
// #   if {$num == 0.} {
// #       set arg 1.
// #   } else {
// #       set arg 0.
// #   }
// # } else {
// #   set arg [expr  $num/$den ]
// # }

    if (den == 0 || abs(I1) > 0)
    {
        sig1 = sig2 = sig3 = I1 / 3;
        return;
    }


    double arg = num / den;

// # if {-1. <= $arg & $arg <= 1.0} {
// #   set phi [expr acos($arg)/3.]
// # } else {
// #   set phi 0.
// # }x)

    double phi_ = acos(arg) / 3;

// # set prnstr {}

// # lappend prnstr [expr $I1/3. + 2./3.*sqrt($I1**2 - 3*$I2)*cos($phi)]
// # lappend prnstr [expr $I1/3. + 2./3.*sqrt($I1**2 - 3*$I2)*cos($phi + 2./3.*$pi)]
// # lappend prnstr [expr $I1/3. + 2./3.*sqrt($I1**2 - 3*$I2)*cos($phi + 4./3.*$pi)]

// #These are not principal stresses, but I can get away with computing q from here
    double s1 = I1 / 3. + 2. / 3.*sqrt(I1 * I1 - 3 * I2) * cos(phi_);
    double s2 = I1 / 3. + 2. / 3.*sqrt(I1 * I1 - 3 * I2) * cos(phi_ + 2. / 3.*M_PI);
    double s3 = I1 / 3. + 2. / 3.*sqrt(I1 * I1 - 3 * I2) * cos(phi_ + 4. / 3.*M_PI);

    if (s1 != s1 || s2 != s2 || s3 != s3)
    {
        cout << "sx = "  << sx << endl;
        cout << "sy = "  << sy << endl;
        cout << "sz = "  << sz << endl;
        cout << "sxy = "  << sxy << endl;
        cout << "sxz = "  << sxz << endl;
        cout << "syz = "  << syz << endl;
        cout << "I1 = "  << I1 << endl;
        cout << "I2 = "  << I2 << endl;
        cout << "I3 = "  << I3 << endl;
        cout << "num = "  << num << endl;
        cout << "den = "  << den << endl;
        cout << "arg = "  << arg << endl;
        cout << "phi_ = "  << phi_ << endl;
        cout << "M_PI = "  << M_PI << endl;
        cout << "s1 = "  << s1 << endl;
        cout << "s2 = "  << s2 << endl;
        cout << "s3 = "  << s3 << endl;
    }

//     sigma = sp.vstack((s1, s2, s3))
//             sigma = sp.sort(abs(sigma), axis = 0)
// # print 'sigma = ', sigma.shape

//                     sig1 = sigma[2, :]
//                            sig2 = sigma[1, :]
//                                   sig3 = sigma[1, 0]
    double sig[3] = {s1, s2, s3};
    constexpr int n = 3;
    for (int c = 0 ; c < ( n - 1 ); c++)
    {
        for (int d = 0 ; d < n - c - 1; d++)
        {
            if (sig[d] > sig[d + 1]) /* For decreasing order use < */
            {
                double swap = sig[d];
                sig[d] = sig[d + 1];
                sig[d + 1] = swap;
            }
        }
    }
    sig1 = sig[0];
    sig2 = sig[1];
    sig3 = sig[2];
}

// inline void NonlinearIsotropic3DLT::updateModulus()
// {
//     double sig1,  sig3;

//     double s1, s3;  // Average stress between previous step and current... (results in a secant stiffness :P)
//     double desv, devfh, strlev, phi, Bmin, G_mod, Ei_el;

//     if (initialize)// For very first step (initial tangent) set confinement to be atmospheric pressures
//     {
//         // cout << ".";
//         sig1 = pa;
//         sig3 = pa;
//         // sig1p = pa;
//         // sig3p = pa;
//         initialize = false;
//     }
//     else
//     {
//         // cout << "+";
//         double sig1_, sig2_, sig3_;
//         getPrincipalStresses(TrialStress, sig1_, sig2_, sig3_);
//         sig1 = -sig3_;
//         sig3 = -sig1_;

//     }



//     s1 = sig1;//0.5*(sig1 + sig1p);
//     s3 = sig3;//0.5*(sig3 + sig3p);


//     /// TCL

//     desv = s1 - s3;


//     // Calculate Stress-Level
//     if (c > 0.)
//     {
//         devfh = (2 * c * cos(phi0) + 2 * s3 * sin(phi0)) / (1. - sin(phi0));
//     }
//     else
//     {

//         if (s3 <= 0.25 * pa )
//         {
//             phi = phi_calc (phi0, dphi, pa, ( 0.25 * pa));
//         }
//         else
//         {
//             phi = phi_calc (phi0, dphi, pa, s3);
//         }

//         devfh = ( 2 * s3 * sin(phi) / (1. - sin(phi)));

//         if (devfh < 0.)
//         {
//             devfh = -devfh;
//         }
//     }

//     if ( devfh == 0 )
//     {
//         if (desv == 0)
//         {
//             strlev  = 0.;
//         }
//         else
//         {
//             strlev = ( 0.25 * pa);
//         }
//     }
//     else
//     {
//         strlev = ( desv / devfh);
//     }

//     if (strlev > 10)
//     {
//         strlev = 10;
//     }

//     Bmin = B_calc ( Kb, pa, n, ( 0.25 * pa) );

//     double B_el, Et_el, nu_el;

//     //Tension Failure
//     if (s3 < -0.1 * pa)
//     {

//         B_el = B_calc  (Kb, pa, n, ( 0.25 * pa) );

//         if (B_el < Bmin)
//         {
//             B_el = Bmin;
//         }

//         // if (stage == 0)       // NOTE: If this produces problems think about it better and compare with TCL stuff.
//         // {
//         //     B_el = ( B_el / 100.);
//         // }

//         G_mod = ( B_el / 10.);
//         Et_el = ( ( 9.*B_el * G_mod ) / ( 3 * B_el + G_mod ));
//         nu_el  = ( ( 3.*B_el - 2.*G_mod ) / ( 6.*B_el + 2.*G_mod ));

//         //If not tension failure check other stages
//     }
//     else
//     {

//         if (strlev > 0.95 )
//         {

//             // Shear failure
//             Ei_el = Ei_calc(K, pa, n, s3);
//             B_el  =  B_calc(Kb, pa, n, s3);
//             Et_el = ( Ei_el * pow(1 - Rf * 0.95, 2));
//             nu_el = ( (3 * B_el - Et_el) / (6 * B_el));
//             //puts -nonewline fid_log "Shear   ,  " ;//Et = (format %8.3f Et_el) nu = (format %5.3f nu_el)"

//         }
//         else // If not shear failure
//         {

//             //Elastic Unloading
//             if (strlev < ( 0.95 * strlev_max))
//             {

//                 Ei_el = Eur_calc(Kur, pa, n, s3);
//                 B_el = B_calc(Kb, pa, n, s3);

//                 if (B_el < Bmin)
//                 {
//                     B_el = Bmin;
//                 }

//                 Et_el = Ei_el;
//                 nu_el = ( (3 * B_el - Et_el) / (6 * B_el));

//             }
//             else
//             {

//                 //Primary loading
//                 // if ( s3 > 0. )
//                 // {
//                 if ( (c <= 0.) & (s3 < ( 0.25 * pa)))
//                 {
//                     Ei_el = Ei_calc(K, pa, n, 0.25 * pa);
//                     B_el  = B_calc(Kb, pa, n, 0.25 * pa);
//                 }
//                 else
//                 {
//                     Ei_el = Ei_calc (K, pa, n, s3);
//                     B_el = B_calc  (Kb, pa, n, s3);
//                 }

//                 Et_el = ( Ei_el * pow(1. - Rf * strlev, 2));
//                 nu_el = ( (3 * B_el - Et_el) / (6 * B_el));
//                 // }
//                 // else
//                 // {
//                 //     cerr << "NonlinearIsotropic3DLT::updateModulus() - This should not happen. Abort!!" << endl;
//                 //     cerr << "                                        - s1 = " << s1 << endl;
//                 //     cerr << "                                        - s3 = " << s3 << endl;
//                 //     cerr << "                                        - ";
//                 //     printTensor("TrialStress", TrialStress);

//                 //     exit(0);
//                 // }
//             } ;// Not shear failure
//         } ;// Not tension failure
//     }

//     E = Et_el;
//     nu = nu_el;

//     if (strlev > strlev_max)
//     {
//         strlev_max = strlev;
//     }

// }


inline void NonlinearIsotropic3DLT::updateModulus()
{
    double sig1,  sig3;

    double s1, s3;  // Average stress between previous step and current... (results in a secant stiffness :P)
    double desv, devfh, strlev, phi, Bmin, G_mod, Ei_el, B_el;

    double sig1_, sig2_, sig3_;
    getPrincipalStresses(TrialStress, sig1_, sig2_, sig3_);
    sig1 = -sig3_;
    sig3 = -sig1_;

    s1 = sig1;//0.5*(sig1 + sig1p);
    s3 = sig3;//0.5*(sig3 + sig3p);


    if ( (c <= 0.) & (s3 < ( 0.25 * pa)))
    {
        Ei_el = Ei_calc(K, pa, n, 0.25 * pa);
        B_el  = B_calc(Kb, pa, n, 0.25 * pa);
    }
    else
    {
        Ei_el = Ei_calc (K, pa, n, s3);
        B_el = B_calc  (Kb, pa, n, s3);
    }

    double Et_el = ( Ei_el * pow(1. - Rf * strlev, 2));
    double nu_el = ( (3 * B_el - Et_el) / (6 * B_el));

    E = Et_el;
    nu = nu_el;


}


void NonlinearIsotropic3DLT::zeroStrain()
{
    TrialStrain *= 0;
    CommitStrain *= 0;
}






//Definir funciones para calcular rigideces y amortiguamientos
// ---------------------------------------------------------------

// Elasticidad inicial
inline double Ei_calc (double K, double pa, double n, double sig3)
{
    return K * pa * pow(sig3 / pa, n);
}

//Elasticidad tangente
inline double Et_calc (double K, double pa, double n, double Rf, double c, double phi, double desv, double sig3)
{
    double Ei = K * pa * pow(sig3 / pa, n);
    double corr = Rf * (1 - sin(phi)) * desv / (2 * c * cos(phi) + 2 * sig3 * sin(phi));
    return Ei * pow(1 - corr, 2);
}

// Elasticidad de recarga
inline double Eur_calc (double Kur, double pa, double n, double sig3)
{
    return Kur * pa * pow(sig3 / pa, n);
}

// Angulo de friccion
inline double phi_calc (double phi0, double Dphi, double pa, double sig3)
{
    return phi0 - Dphi * log10(sig3 / pa);
}

// Modulo volumetrico
inline double B_calc (double Kb, double pa, double n, double sig3)
{
    return Kb * pa * pow(sig3 / pa, n);
}

// Modulo Poisson inicial
inline double nui_calc (double G, double F, double pa, double sig3)
{
    return G - F * log10(sig3 / pa);
}






