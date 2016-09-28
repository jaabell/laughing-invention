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
// PROGRAMMER:
// DATE:
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include "ClassicElastoplasticityGlobals.h"
Index<'i'> ClassicElastoplasticityGlobals::i;
Index<'j'> ClassicElastoplasticityGlobals::j;
Index<'k'> ClassicElastoplasticityGlobals::k;
Index<'l'> ClassicElastoplasticityGlobals::l;
Index<'m'> ClassicElastoplasticityGlobals::m;
Index<'n'> ClassicElastoplasticityGlobals::n;
Index<'p'> ClassicElastoplasticityGlobals::p;
Index<'q'> ClassicElastoplasticityGlobals::q;
Index<'r'> ClassicElastoplasticityGlobals::r;
Index<'s'> ClassicElastoplasticityGlobals::s;

DTensor2 ClassicElastoplasticityGlobals::kronecker_delta(3, 3, "identity");



void ClassicElastoplasticityGlobals::printTensor(std::string const& name, DTensor2 const& v)
{

    // All 9 elements in one line.
    // std::cout << name << " = [ " ;
    // std::cout << v(0, 0) << " "
    //           << v(1, 1) << " "
    //           << v(2, 2) << " "
    //           << v(0, 1) << " "
    //           << v(0, 2) << " "
    //           << v(1, 2) << " "
    //           << v(1, 0) << " "
    //           << v(2, 0) << " "
    //           << v(2, 1) << " ]" << std::endl;

    // This is in good format but take 3 lines.
    // stderr will print immediately, not like cout (may be reordered by CPU).
    fprintf(stderr, "%s = \n", name.c_str());
    fprintf(stderr, "[%16.8f \t %16.8f \t %16.8f \n",   v(0, 0), v(0, 1), v(0, 2));
    fprintf(stderr, " %16.8f \t %16.8f \t %16.8f \n",   v(1, 0), v(1, 1), v(1, 2));
    fprintf(stderr, " %16.8f \t %16.8f \t %16.8f] \n",  v(2, 0), v(2, 1), v(2, 2));
}

void ClassicElastoplasticityGlobals::printTensor4(std::string const& name, DTensor4 const& v)
{


    std::cout << name << " = [ " ;
    for (int ii = 0; ii < 3; ii++)
        for (int jj = 0; jj < 3; jj++)
            for (int kk = 0; kk < 3; kk++)
                for (int ll = 0; ll < 3; ll++)
                {
                    std::cout << v(ii, jj, kk, ll ) << " ";
                }
    std::cout << "]" << std::endl;
}


std::tuple<double, double, double> ClassicElastoplasticityGlobals::getI1J2J3(const DTensor2 &mystress)
{
    // ------------------------------------------------------------
    // preliminary
    const double I1 = mystress(0, 0) + mystress(1, 1) + mystress(2, 2);
    const double sigma_m = I1 / 3.0;
    DTensor2 s = mystress;
    s(0, 0) -= sigma_m;
    s(1, 1) -= sigma_m;
    s(2, 2) -= sigma_m;
    // J2=0.5*s(i,j)*s(i,j)
    const double J2 = 0.5 * (
                          s(0, 0) * s(0, 0) +  s(0, 1) * s(0, 1)  + s(0, 2) * s(0, 2)
                          +   s(1, 0) * s(1, 0) +  s(1, 1) * s(1, 1)  + s(1, 2) * s(1, 2)
                          +   s(2, 0) * s(2, 0) +  s(2, 1) * s(2, 1)  + s(2, 2) * s(2, 2)   );
    // 3by3 Determinant: Refer to http://www.brown.edu/Departments/Engineering/Courses/En221/Notes/Tensors/Tensors.htm
    const double J3 = s(0, 0) * (s(1, 1) * s(2, 2) - s(1, 2) * s(2, 1))
                      +  s(0, 1) * (s(1, 2) * s(2, 0) - s(1, 0) * s(2, 2))
                      +  s(0, 2) * (s(1, 0) * s(2, 1) - s(2, 0) * s(1, 1));


    return std::make_tuple(I1, J2, J3);
}


// --------------------------------------------
// refer to https://en.wikipedia.org/wiki/Yield_surface
// p = 1/3 * I1
// q = sqrt(3* J2)
// cos(3*theta) = 3/2 * sqrt(3) * J3 / J2^(3/2)
// ------------------------------------------------------------
std::tuple<double, double, double> ClassicElastoplasticityGlobals::getpqtheta(const DTensor2 &sigma)
{
    double I1, J2, J3;

    std::tie(I1, J2, J3) = getI1J2J3(sigma);

    // (1) calculate p
    const double p = -I1 / 3;
    // (2) calculate q
    const double q = sqrt(3 * J2);
    // (3) calculate theta
    const double cos3theta = 1.5 * sqrt(3) * J3 / pow(J2, 1.5);
    // const static double PI = 3.14159265358979323846; //20 digits from Wiki.
    const double theta = acos(cos3theta) / 3.0 * 180 / M_PI; //theta is in degree.

    return std::make_tuple(p, q, theta);
    // return result;
}

bool ClassicElastoplasticityGlobals::inverse4thTensor(DTensor4 const& rhs, DTensor4& ret)
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 intermediate_matrix(9, 9, 0.0);
    intermediate_matrix *= 0;
    // static DTensor4 ret(3,3,3,3,0.0);
    int m41 = 0,  m42 = 0;
    // (1). convert 4th order Tensor to matrix
    for ( int c44 = 1 ; c44 <= 3 ; c44++ )
        for ( int c43 = 1 ; c43 <= 3 ; c43++ )
            for ( int c42 = 1 ; c42 <= 3 ; c42++ )
                for ( int c41 = 1 ; c41 <= 3 ; c41++ )
                {
                    m41 = 3 * (c41 - 1) + c42;
                    m42 = 3 * (c43 - 1) + c44;

                    intermediate_matrix( m41 - 1, m42 - 1 ) = rhs( c41 - 1, c42 - 1, c43 - 1, c44 - 1 );

                }
    // (2). Inverse the matrix .
    double det = intermediate_matrix.compute_Determinant();

    // fprintf(stderr, "----> det: %f\n", det);

    if (det < MACHINE_EPSILON)
    {
        std::cout << "ClassicElastoplasticMaterial matrix T is not invertible to get the consistent stiffness tensor. Use the inconsistent stiffness instead! " << std::endl;
        return false;
    }
    else
    {
        static DTensor2 inv_matrix(3, 3, 0.0);
        inv_matrix = intermediate_matrix.Inv();
        // (3). convert Matrix to 4th order tensor
        for ( int c44 = 1 ; c44 <= 3 ; c44++ )
            for ( int c43 = 1 ; c43 <= 3 ; c43++ )
                for ( int c42 = 1 ; c42 <= 3 ; c42++ )
                    for ( int c41 = 1 ; c41 <= 3 ; c41++ )
                    {
                        m41 = 3 * (c41 - 1) + c42;
                        m42 = 3 * (c43 - 1) + c44;

                        ret(c41 - 1, c42 - 1, c43 - 1, c44 - 1) = inv_matrix(m41 - 1, m42 - 1);
                    }
        return true;
    }

}


// Some stress derivatives....
void ClassicElastoplasticityGlobals::dJ2_dsigma_ij(const DTensor2& sigma, DTensor2 &result)   // Stress derivative of second deviatoric stress invariant
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 s(3, 3, 0.0);
    s *= 0;
    double p;
    s.compute_deviatoric_tensor(s, p);
    p = -p;

    result(i, j) = s(i, j);

    return;
}


void ClassicElastoplasticityGlobals::dJ3_dsigma_ij(const DTensor2& sigma, DTensor2 &result)   // Stress derivative of third deviatoric stress invariant
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 s(3, 3, 0.0);
    s *= 0;
    double p;
    s.compute_deviatoric_tensor(s, p);
    p = -p;

    double J2 = (s(i, j) * s(i, j)) / 2;

    result(i, j) = s(i, p) * s(p, j);
    result(0, 0) = s(0, p) * s(p, 0) - (2 * J2) / 3;
    result(1, 1) = s(1, p) * s(p, 1) - (2 * J2) / 3;
    result(2, 2) = s(2, p) * s(p, 2) - (2 * J2) / 3;

    return;
}


void ClassicElastoplasticityGlobals::dq_dsigma_ij(const DTensor2& sigma, DTensor2 &result)   // Stress derivative of deviatoric stress q
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 s(3, 3, 0.0);
    s *= 0;
    double p;
    s.compute_deviatoric_tensor(s, p);
    p = -p;

    double J2 = (s(i, j) * s(i, j)) / 2;
    double q = sqrt(3 * J2);
    result(i, j) = 3 / (2 * q) * s(i, j);

    return;
}


void ClassicElastoplasticityGlobals::dtheta_dsigma_ij(const DTensor2& sigma, DTensor2 &result)   // Stress derivative of Lode angle
{
    using namespace ClassicElastoplasticityGlobals;
    double I1, J2, J3;

    std::tie(I1, J2, J3) = getI1J2J3(sigma);

    static DTensor2 s(3, 3, 0.0);
    static DTensor2 dJ2_ij(3, 3, 0.0);
    static DTensor2 dJ3_ij(3, 3, 0.0);
    s *= 0;
    dJ2_ij *= 0;
    dJ3_ij *= 0;

    double p;
    s.compute_deviatoric_tensor(s, p);
    p = -p;

    dJ2_dsigma_ij(sigma, dJ2_ij);
    dJ3_dsigma_ij(sigma, dJ3_ij);

    double J2cubed = J2 * J2 * J2;
    double sqrtJ2cubed = sqrt(J2cubed);
    double A = (3.*sqrt(3.) ) / ( 2 * J2 * sqrtJ2cubed * sqrt( ( 4.*J2cubed - 27.*J3 * J3 ) / J2cubed ) ) ;

    result(i, j) = A * ( 3 * dJ2_ij(i, j) * J3 - 2 * dJ3_ij(i, j) * J2);

    return;
}



