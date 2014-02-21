///*
//################################################################################
//# COPY-YES  (C):     :-))                                                      #
//# PROJECT:           Object Oriented Finite Element Program                    #
//# PURPOSE:                                                                     #
//# CLASS:                                                                       #
//#                                                                              #
//# VERSION:                                                                     #
//# LANGUAGE:          C++.ver >= 2.0                                            #
//# TARGET OS:         DOS || UNIX || . . .                                      #
//# DESIGNER(S):       Boris Jeremic                                             #
//# PROGRAMMER(S):     Boris Jeremic                                             #
//#                                                                              #
//#                                                                              #
//# DATE:              Aug 2007                                                  #
//# UPDATE HISTORY:                                                              #
//#                                                                              #
//#                                                                              #
//################################################################################
//*/
#include "time.h"
#include "limits.h"
#include "math.h"
#include "float.h"
// nDarray tools
#include "BJtensor.h"
#include "stresst.h"
#include "straint.h"
#include "BJvector.h"
#include "BJmatrix.h"
#include "iostream.h"

int main(int argc, char* argv[])
{
    //  if (argc != 2)
    //    {
    //      puts("\a\n usage: elast_tst test01.std\n");
    //      exit( 1 );
    //    }

    // defining machine epsilon for different built in data types supported by C++
    float        float_eps       = f_macheps();  // or use float.h values FLT_EPSILON
    double       double_eps      = d_macheps();  // or use float.h values DBL_EPSILON
    //long double  long_double_eps = ld_macheps(); // or use float.h values LDBL_EPSILON

    ::printf("\n float macheps = %.20e \n", float_eps);
    ::printf(" double macheps = %.20e \n", double_eps);
    //::printf(" long double macheps = %.20Le \n\n\n",long_double_eps);


    // Usual tolerance is defined as square root of machine epsilon
    float        sqrt_f_eps       = sqrt(f_macheps());
    double       sqrt_d_eps       = sqrt(d_macheps());
    //long double  sqrt_ld_eps      = sqrt(ld_macheps());

    ::printf("\n sqrt float macheps = %.20e \n", sqrt_f_eps);
    ::printf(" sqrt double macheps = %.20e \n", sqrt_d_eps);
    //::printf(" sqrt long double macheps = %.20Le \n",sqrt_ld_eps);



    // Kronecker delta tensor
    tensor I2("I", 2, def_dim_2);
    I2.print("I2", "\ntensor I2 (2nd order unit tensor: Kronecker Delta -> d_ij)");


    tensor Ipmnq = I2("pm") * I2("nq");
    tensor Imnpq = I2("mn") * I2("pq");

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //....................................................................
    // some of the 4. order tensor used in conituum mechanics:
    // the most general representation of the fourth order isotropic tensor
    // includes the following fourth orther unit isotropic tensors:
    tensor I_ijkl( 4, def_dim_4, 0.0 );
    I_ijkl = I2("ij") * I2("kl");
    //  I_ijkl.print("ijkl","\ntensor I_ijkl = d_ij d_kl)");
    tensor I_ikjl( 4, def_dim_4, 0.0 );
    //  I_ikjl = I2("ij")*I2("kl");
    I_ikjl = I_ijkl.transpose0110();
    //  I_ikjl.print("ikjl","\ntensor I_ikjl) ");
    tensor I_ikjl_inv_2 = I_ikjl.inverse_2();

    //  I_ikjl_inv_2.print("I_ikjl","\ntensor I_ikjl_inverse_2)");
    if ( I_ikjl == I_ikjl_inv_2 )
    {
        ::printf("\n\n I_ikjl == I_ikjl_inv_2 !\n");
    }
    else
    {
        ::printf("\a\n\n           I_ikjl != I_ikjl_inv_2 !\n");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    tensor I_iljk( 4, def_dim_4, 0.0 );
    //  I_ikjl = I2("ij")*I2("kl");
    //..  I_iljk = I_ikjl.transpose0101();
    I_iljk = I_ijkl.transpose0111();
    //  I_iljk.print("iljk","\ntensor I_iljk) ");
    // To check out this three fourth-order isotropic tensor please see
    // W.Michael Lai, David Rubin, Erhard Krempl
    // " Introduction to Continuum Mechanics"
    // QA808.2
    // ISBN 0-08-022699-X

    //....................................................................
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // tensor additions ans scalar multiplications
    // symmetric part of fourth oder unit isotropic tensor
    tensor I4s = (I_ikjl + I_iljk) * 0.5;
    //  tensor I4s = 0.5*(I_ikjl+I_iljk);
    I4s.print("I4s", "\n symmetric tensor I4s = (I_ikjl+I_iljk)*0.5 ");
    // skew-symmetric part of fourth oder unit isotropic tensor
    tensor I4sk = (I_ikjl - I_iljk) * 0.5;
    I4sk.print("I4sk", "\n skew-symmetric tensor I4sk = (I_ikjl-I_iljk)*0.5 ");

    // equvalence check ( they should be different )
    if ( I4s == I4sk )
    {
        ::printf("\n\n I4s == I4sk !\n");
    }
    else
    {
        ::printf("\a\n\n           I4s != I4sk !\n");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    //....................................................................
    // Linear Isotropic Elasticity Tensor
    // building elasticity tensor
    double Ey = 20000;
    double nu = 0.2;
    // building stiffness tensor in one command line
    tensor E1 = (I_ijkl * ((Ey * nu * 2.0) / (2.0 * (1.0 + nu) * (1 - 2.0 * nu)))) +
                ( (I_ikjl + I_iljk) * (Ey / (2.0 * (1.0 + nu))));
    // building compliance tensor in one command line
    tensor D1 = (I_ijkl * (-nu / Ey)) + ( (I_ikjl + I_iljk) * ((1.0 + nu) / (2.0 * Ey)));

    // multiplication between them
    tensor test = E1("ijkl") * D1("klpq");
    test.print("t", "\n\n\n test =  E1(\"ijkl\")*D1(\"klpq\") \n");

    // and this should be equal to the symmetric part of 4. order unit tensor
    if ( test == I4s )
    {
        ::printf("\n test == I4s  TRUE  ( up to sqrt(macheps())) \n");
    }
    else
    {
        ::printf("\a\n\n\n   test == I4s    NOTTRUE  ( up to sqrt(macheps())) \n\n\n");
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(stderr, "LINE  --------------------------  9");


    //............Different Way...........................................
    // Linear Isotropic Elasticity Tensor
    double lambda = nu * Ey / (1. + nu) / (1. - 2. * nu);
    double mu = Ey / (2. * (1. + nu));
    ::printf("\n  lambda = %.12e\n", lambda);
    ::printf("      mu = %.12e\n", mu);

    ::printf("\nYoung Modulus = %.4e", Ey);
    ::printf("\nPoisson ratio = %.4e\n", nu);
    ::printf(  "    lambda + 2 mu = %.4e\n", (lambda + 2 * mu));

    // building elasticity tensor
    tensor E2 = (I_ijkl * lambda) + (I4s * (2. * mu));
    E2.print("E2", "tensor E2: elastic moduli Tensor = lambda*I2*I2+2*mu*I4s");


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    tensor E2inv = E2.inverse();
    E2inv.print("E2inv", "tensor E2inv");

    // building compliance tensor
    tensor D2 = (I_ijkl * (-nu / Ey)) + (I4s * (1. / (2.*mu)));
    D2.print("D2", "tensor D2: compliance Tensor (I_ijkl * (-nu/Ey)) + (I4s * (1./2./mu))");

    if ( E2inv == D2 )
    {
        ::printf("\n E2inv == D2   TRUE \n");
    }
    else
    {
        ::printf("\a\n\n\n  E2inv == D2   NOTTRUE \n\n\n");
    }

    if ( E1 == E2 )
    {
        ::printf("\n E1 == E2  TRUE \n");
    }
    else
    {
        ::printf("\a\n\n\n  E1 == E2  NOTTRUE \n\n\n");
    }

    if ( D1 == D2 )
    {
        ::printf("\n D1 == D2  TRUE \n");
    }
    else
    {
        ::printf("\a\n\n\n  D1 == D2  NOTTRUE \n\n\n");
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    fprintf(stderr, "LINE  --------------------------  11\n\n\n\n");


    //::printf("stack length is : %u \n",_stklen);
    double PIo3 = PI / 3.0;
    //::printf("Pi/3.0 = %f \n",PIo3);
    //-------------------------------------------
    double p_start     = 5000.000;
    double q_start     = 2000.0;
    double theta_start = PIo3;
    //..//  double theta_start = 0.4;

    stresstensor stress = stress.pqtheta2stress(p_start, q_start, theta_start);
    //stress.report("stress\n");
    stress.reportshort("stress\n");

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    straintensor strain = D2("ijkl") * stress("kl");
    //strain.report("strain = D2(\"ijkl\") * stress(\"kl\") \n");
    strain.reportshort("strain = D2(\"ijkl\") * stress(\"kl\") \n");
    strain.null_indices();

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////



    tensor stress_new = E2("ijkl") * strain("kl");
    //stress_new.report("stress_new\n");
    stress_new.print("stress_new\n");


    tensor strain_inverse = strain.inverse();
    //strain_inverse.report("Inverse of strain");
    strain_inverse.print("Inverse of strain");

    tensor test2 = strain("ij") * strain_inverse("jk");
    test2.print();




    tensor stress_times_strain_inverse = stress_new("ij") * strain_inverse("kl");
    stress_times_strain_inverse.null_indices();
    E2.print("E2", "tensor E2");
    stress_times_strain_inverse.print("s", "stressstraininv");

    strain.null_indices();
    tensor test1 = stress_times_strain_inverse("ijkl") * strain("kl");
    test1.print();



    exit(1);
    //  return 1;
}
