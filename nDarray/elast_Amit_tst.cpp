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
//# DATE:              April 2001                                                #
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

int main(void)
{
    ::printf("\n\n----------------- TENSOR --------------------\n\n");

    ////////////////////////////////1st type of tensor products//////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    tensor t1;

    static double t2values[] = {  1, 2, 3,
                                  4, 5, 6,
                                  7, 8, 9
                               };
    // tensor constructor with values assignments ( order 2 ; like matrix )
    tensor t2( 2, def_dim_2, t2values);
    tensor tst( 2, def_dim_2, t2values);

    tensor t2xt2 = t2("ij") * t2("jk");
    tensor t2xtst = t2("ij") * tst("jk");
    tensor tst2 = t2("ij") * t2("kj");
    tensor tst3 = t2("ij") * t2("ik");///it should be 1213 or 1214
    tensor tst4 = t2("ij") * t2("ki");  //there is nothing like 1213 or 1241 but 1231 and 1214 is there
    tensor tst5 = t2("ij") * t2("ji");
    tensor tst6 = t2("ij") * t2("ji");
    tensor tst7 = t2("ij") * t2("kl");
    tensor tst8 = t2("ij") * t2("lk"); //1234 is there but 1243 is not there means kl nad lk is equivalent

    ////////////////////////////////2nd type of tensor products//////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    //////////////multiplication of 3rd order and 2nd order tensors////////////////////
    tensor e("e", 3, def_dim_3);
    tensor tst9 = t2("ij") * e("jlm");
    tensor tst10 = t2("ij") * e("jml");
    tensor tst11 = t2("ij") * e("ilm");
    tensor tst12 = t2("ij") * e("iml");
    tensor tst13 = t2("ij") * e("mij");
    tensor tst14 = t2("ij") * e("mji");
    tensor tst15 = t2("ij") * e("lij");
    tensor tst16 = t2("ij") * e("lji");

    tensor tst17 = e("jlm") * t2("ij") ;
    tensor tst18 = e("jml") * t2("ij") ;
    tensor tst19 = e("ilm") * t2("ij") ;
    tensor tst20 = e("iml") * t2("ij") ;
    tensor tst21 = e("mij") * t2("ij") ;
    tensor tst22 = e("mji") * t2("ij") ;
    tensor tst23 = e("lij") * t2("ij") ;
    tensor tst24 = e("lji") * t2("ij") ;



    /////////////////////////////////////////////////////////////////////////////////////////
    //////////////multiplication of 3rd order and 3rd order tensors////////////////////


    tensor ee1 = e("ijm") * e("klm");
    tensor ee2 = e("ijm") * e("kml"); ////ee2 nad ee3 have same number 123436/////
    tensor ee3 = e("ijm") * e("lmk");
    tensor ee4 = e("ijm") * e("lkm");
    tensor ee5 = e("ijm") * e("mkl");
    tensor ee6 = e("ijm") * e("mlk");

    tensor ee7 = e("ilm") * e("jlm");
    tensor ee8 = e("ilm") * e("jml");
    tensor ee9 = e("ilm") * e("ljm");
    tensor ee10 = e("ilm") * e("lmj");
    tensor ee11 = e("ilm") * e("mlj");
    tensor ee12 = e("ilm") * e("mjl");


    tensor ee13 = e("imj") * e("ilm");
    tensor ee14 = e("imj") * e("iml");
    tensor ee15 = e("imj") * e("mli");
    tensor ee16 = e("imj") * e("mil");
    tensor ee17 = e("imj") * e("lmi");
    tensor ee18 = e("imj") * e("lim");


    tensor ee19 = e("ijk") * e("ijk");



    //////////////declaration of t4//////////////////////

    tensor ZERO(4, def_dim_4, 0.0);
    ZERO.print("z", "\ntensor ZERO (4-th order with value assignment)");

    static double t4val[] =
    {
        11.100,  12.300,  13.100,   15.230,  16.450,   14.450,  17.450,  18.657,  19.340,
        110.020, 111.000, 112.030,  114.034, 115.043,  113.450, 116.670, 117.009, 118.060,
        128.400, 129.500, 130.060,  132.560, 133.000,  131.680, 134.100, 135.030, 136.700,
        119.003, 120.400, 121.004,  123.045, 124.023,  122.546, 125.023, 126.043, 127.000,
        137.500, 138.600, 139.000,  141.067, 142.230,  140.120, 143.250, 144.030, 145.900,
        146.060, 147.700, 148.990,  150.870, 151.000,  149.780, 152.310, 153.200, 154.700,
        164.050, 165.900, 166.003,  168.450, 169.023,  167.067, 170.500, 171.567, 172.500,
        155.070, 156.800, 157.067,  159.000, 160.230,  158.234, 161.470, 162.234, 163.800,
        173.090, 174.030, 175.340,  177.060, 178.500,  176.000, 179.070, 180.088, 181.200
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //////////////multiplication of 4th order and 3rd order tensors////////////////////
    tensor t4(4, def_dim_4, t4val);

    tensor test1 = t4("ijkl") *  t4("ijkl");
    tensor test2 = t4("ijkl") *  t4("ijlk");
    tensor test3 = t4("ijkl") *  t4("ikjl");
    tensor test4 = t4("ijkl") *  t4("iklj");
    tensor test5 = t4("ijkl") *  t4("iljk");
    tensor test6 = t4("ijkl") *  t4("ilkj");

    tensor test7 = t4("ijkl") *  t4("jikl");
    tensor test8 = t4("ijkl") *  t4("jilk");
    tensor test9 = t4("ijkl") *  t4("jkij");
    tensor test10 = t4("ijkl") *  t4("jkji");
    tensor test11 = t4("ijkl") *  t4("jlki");
    tensor test12 = t4("ijkl") *  t4("jlik");

    tensor test13 = t4("ijkl") *  t4("kjli");
    tensor test14 = t4("ijkl") *  t4("kjil");
    tensor test15 = t4("ijkl") *  t4("klji");
    tensor test16 = t4("ijkl") *  t4("klij");
    tensor test17 = t4("ijkl") *  t4("kijl");
    tensor test18 = t4("ijkl") *  t4("kilj");


    /////////////////////////////////////////////////////////////////////////////////////////
    //////////////multiplication of 4th order and 2nd order tensors////////////////////

    tensor tes1 = t4("ijkl") * t2("ln");
    tensor tes2 = t4("ijkl") * t2("nl");

    tensor tes3 = t4("ijlk") * t2("ln");
    tensor tes4 = t4("ikjl") * t2("ln");
    tensor tes5 = t4("iklj") * t2("ln");
    tensor tes6 = t4("iljk") * t2("ln");
    tensor tes7 = t4("ilkj") * t2("ln");




    ////////////////////////////////Tensor types from elast_tst.cpp files//////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    //  tensor I2("I", 2, def_dim_2);
    //  tensor Ipmnq = I2("pm") * I2("nq");
    //  tensor Imnpq = I2("mn") * I2("pq");
    //  tensor I2I2 = I2("ij")*I2("ij");
    //  tensor I2PI = I2*PI;

    //  tensor I_ijkl( 4, def_dim_4, 0.0 );
    //  I_ijkl = I2("ij")*I2("kl");
    //  tensor I_ikjl( 4, def_dim_4, 0.0 );
    //  I_ikjl = I_ijkl.transpose0110();
    //  tensor I_ikjl_inv_2 = I_ikjl.inverse_2();
    //  tensor I_iljk( 4, def_dim_4, 0.0 );
    //  I_iljk = I_ijkl.transpose0111();
    // tensor I_ijkl_I_ikjl = I_ijkl("ijkl")*I_ikjl("ijkl");
    //  tensor I_ijkl_I_iljk = I_ijkl("ijkl")*I_iljk("ijkl");
    //  tensor I_ikjl_I_iljk = I_ikjl("ijkl")*I_iljk("ijkl");
    //  tensor I_ijkl_I_ijkl = I_ijkl("ijkl")*I_ijkl("ijkl");
    //  tensor I_ikjl_I_ikjl = I_ikjl("ikjl")*I_ikjl("ikjl");
    //  tensor I_iljk_I_iljk = I_iljk("iljk")*I_iljk("iljk");

    //  tensor I4s = (I_ikjl+I_iljk)*0.5;
    //  tensor I4sk = (I_ikjl-I_iljk)*0.5;

    //  double Ey = 20000;
    //  double nu = 0.2;

    //  tensor E1 = (I_ijkl*((Ey*nu*2.0)/(2.0*(1.0+nu)*(1-2.0*nu)))) +
    //              ( (I_ikjl+I_iljk)*(Ey/(2.0*(1.0+nu))));
    //  tensor D1= (I_ijkl * (-nu/Ey)) + ( (I_ikjl+I_iljk) * ((1.0+nu)/(2.0*Ey)));
    //  tensor test = E1("ijkl")*D1("klpq");

    //  double lambda = nu * Ey / (1. + nu) / (1. - 2. * nu);
    //  double mu = Ey / (2. * (1. + nu));
    //  tensor E2 = (I_ijkl * lambda) + (I4s * (2. * mu));
    //  tensor E2inv = E2.inverse();

    //  double PIo3 = PI/3.0;
    //  double p_start     = 5000.000;
    //  double q_start     = 2000.0;
    //  double theta_start = PIo3;
    //  tensor D2= (I_ijkl * (-nu/Ey)) + (I4s * (1./(2.*mu)));
    //  stresstensor stress = stress.pqtheta2stress(p_start, q_start, theta_start);
    //  straintensor strain = D2("ijkl") * stress("kl");
    //////////////////////////////////cause of bug//////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //  strain.report("strain = D2(\"ijkl\") * stress(\"kl\") \n");

    ///////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //               static double stressvalues[] = {  1.0, 2.0, 3.0,
    //                                    2.0, 2.0, 5.0,
    //                                    3.0, 5.0, 9.0  };
    //
    //  stresstensor sigma(stressvalues);
    //  stresstensor s = sigma.deviator();
    //  stresstensor test01 = s("ij") * s("ij");
    //  stresstensor KWtest01 =  s("ij") * s("jk");
    //  stresstensor KWtest02 =  KWtest01("ik") * I2("ik");
    //
    //       static double stressvector[] = {-3.0, -2.0, 5.0, 2.0*sqrt(2.0), 3.0*sqrt(2.0), 5.0*sqrt(2.0)};
    //
    //  BJvector stressvector01( 6, stressvector);
    //  BJvector product = stressvector01.transpose() * stressvector01;

    //::printf("\n\n\n");



    // --------------
    //::printf("\n\n\n --------------------------TENSOR SCALAR MULTIPLICATION ----\n\n");
    //  tensor EEEE1 = E2;
    //  tensor EEEE2 = EEEE1*2.0;



















    exit(1);
    //  return 1;
}
