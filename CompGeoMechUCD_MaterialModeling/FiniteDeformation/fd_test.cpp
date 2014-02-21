//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Finite Deformation Hyper-Elastic classes
//# CLASS:
//#
//# VERSION:           0.6_(1803398874989) (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhao Cheng, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//#
//#
//# DATE:              19AUg2003
//# UPDATE HISTORY:    Sept 2003
//#
//#
//===============================================================================

// fd_test.cpp

#include <string.h>
#include <math.h>

#include <fstream.h>
#include <iostream.h>

#include "NDMaterial.h"

#include "FiniteDeformationElastic3D.h"
#include "FDdecoupledElastic3D.h"
#include "FiniteDeformationElastic3D.h"
#include "W.h"
#include "LogWEnergy.h"
#include "NeoHookeanWEnergy.h"
#include "SimoPisterWEnergy.h"
#include "OgdenWEnergy.h"
#include "MooneyRivlinWEnergy.h"
#include "OgdenSimoWEnergy.h"
#include "MooneyRivlinSimoWEnergy.h"

#include "NeoHookeanCompressible3D.h"
#include "FiniteDeformationEP3D.h"
#include "fdYield/fdYieldVM.h"
#include "fdYield/fdYieldDP.h"
#include "fdFlow/fdFlowVM.h"
#include "fdFlow/fdFlowDP.h"
#include "fdEvolution/fdEvolution_SLS.h"
#include "fdEvolution/fdEvolution_TL.h"

#include <OPS_Globals.h>
#include <G3Globals.h>
#include <ConsoleErrorHandler.h>
#include <ostream.h>

ErrorHandler* g3ErrorHandler = 0;
double        ops_Dt = 0.0;
Domain*       ops_TheActiveDomain = 0;
Element*      ops_TheActiveElement = 0;

ostream* cerrPtr;

int main()
{

    cout << "\n\n*** Finite Deformations: T E S T ***\n\n" << endln;

    ofstream outStress ("Results.txt");

    //********** for Neo-hookean
    //double rho_in = 0.0;
    //double K_in = 1971.67e3;
    //double G_in = 422.5e3;

    ////********** for Odegen
    //int Nogden = 3;
    //double cr[3];
    //double mur[3];
    //cr[0] = 6.3e5;
    //cr[1] = 0.012e5;
    //cr[2] = -0.1e5;
    //mur[0] = 1.3;
    //mur[1] = 5.0;
    //mur[2] = -2.0;

    ////********** for Mooney-Rivlin
    //double c1_in = 1.8484375e5;
    //double c2_in =  0.2640625e5;

    //cout << "rho = " << rho_in << endln;
    //cout << "K = " << K_in << endln;
    //cout << "G = " << G_in << endln;

    //NeoHookeanWEnergy  thisFDW( K_in, G_in );
    //LogWEnergy  thisFDW( K_in, G_in );
    //OgdenWEnergy  thisFDW(Nogden, cr, mur);
    //MooneyRivlinWEnergy  thisFDW( c1_in, c2_in );
    //SimoPisterWEnergy  thisFDW( K_in );
    //OgdenSimoWEnergy  thisFDW( Nogden, cr, mur, K_in);
    //MooneyRivlinSimoWEnergy  thisFDW( c1_in, c2_in, K_in );

    //FDdecoupledElastic3D  thisFDstate( 0, 0, &thisFDW, rho_in);

    //NeoHookeanCompressible3D thisFDstate(0, K_in, G_in, rho_in);
    stresstensor init_stress;
    init_stress.val(1, 1) = 0.0;
    init_stress.val(1, 2) = 0.0;
    init_stress.val(1, 3) = 0.0;
    init_stress.val(2, 1) = 0.0;
    init_stress.val(2, 2) = 0.0;
    init_stress.val(2, 3) = 5.0;
    init_stress.val(3, 1) = 0.0;
    init_stress.val(3, 2) = 5.0;
    init_stress.val(3, 3) = 0.0;

    double rho_in = 0.0;
    double K_in = 1971.67;
    double G_in = 422.5;

    NeoHookeanCompressible3D HE0(0, K_in, G_in, rho_in);
    fdYieldVM fdY0(10, init_stress);
    fdFlowVM fdF0(10, init_stress);
    fdEvolution_TL Hn0(80);
    FiniteDeformationEP3D thisFDstate(1, &HE0, &fdY0, &fdF0, &Hn0);

    double gamma_start = 0.0;
    double delta_gamma = 0.0005;
    int Num_step = 100;

    double gamma = 0.0;
    Tensor aStress(2, def_dim_2, 0.0);

    for ( int i = 0; i <= Num_step; i++)
    {

        gamma = gamma_start + (double)i * delta_gamma;

        cout << "\n***gamma = " << gamma << endln;

        // Simple Shear
        double F11 = 1.0;
        double F12 = 0.0;
        double F13 = gamma;
        double F21 = 0.0;
        double F22 = 1.0;
        double F23 = 0.0;
        double F31 = 0.0;
        double F32 = 0.0;
        double F33 = 1.0;

        double F_values[] = { F11, F12, F13, F21, F22, F23, F31, F32, F33};

        Tensor thisf(2, def_dim_2, F_values);
        //thisf.print("F", "Deformation Gradient:");

        thisFDstate.setTrialF(thisf);

        //straintensor thisC = thisFDstate.getC();
        //thisC.print("C", "C tensor:");

        //stresstensor tStress = thisFDstate.getStressTensor();
        //tStress.print("S","PK2 Stress:");

        straintensor thisb = thisf("iA") * thisf("jA");
        thisb.null_indices();
        //thisb.print("b", "b tensor:");
        tensor EV_b = thisb.eigenvectors();
        EV_b.print("EV_b", "EV_b:");

        stresstensor tStress = thisFDstate.getCauchyStressTensor();
        //tStress.print("Sigma","Cauchy Stress:");
        tensor EV_t = tStress.eigenvectors();
        EV_t.print("EV_t", "EV_t:");

        outStress << F13  <<  "  "  <<  tStress.cval(1, 3) << endln;
    }

    return 0;
}

















//
//**************************************************************************************/
//*   Pure Extension                                                                   */
//**************************************************************************************/
//   double F11 = 1.0+gamma;  double F12 = 0.0;        double F13 = 0.0;
//   double F21 = 0.0;        double F22 = 1.0;        double F23 = 0.0;
//   double F31 = 0.0;        double F32 = 0.0;        double F33 = 1.0;

//
//**************************************************************************************/
//*   Simple Extension                                                                   */
//**************************************************************************************/
//   double F11 = 1.0+gamma;  double F12 = 0.0;        double F13 = 0.0;
//   double F21 = 0.0;        double F22 = 1.0/sqrt(1.0+gamma);        double F23 = 0.0;
//   double F31 = 0.0;        double F32 = 0.0;        double F33 = 1.0/sqrt(1.0+gamma);

//
///************************************************************************************/
///*   Pure compression                                                               */
///************************************************************************************/
//  double F11 = 1.0-gamma;     double F12 = 0.0;              double F13 = 0.0;
//  double F21 = 0.0;           double F22 = 0.999999;          double F23 = 0.0;
//  double F31 = 0.0;           double F32 = 0.0;              double F33 = 1.000001;


//
///************************************************************************************/
///*   Triaxial compression                                                           */
///************************************************************************************/
//  double dilatancy = 0.2;
//  double F11 = 1.0-gamma; double F12 = 0.0;                     double F13 = 0.0;
//  double F21 = 0.0;       double F22 = 1.0+gamma*dilatancy; double F23 = 0.0;
//  double F31 = 0.0;       double F32 = 0.0;                     double F33 = 1.0+gamma*dilatancy;


///************************************************************************************/
///*   Pure Volumn Change                                                           */
///************************************************************************************/
//  double F11 = 1.0+gamma;  double F12 = 0.0;                   double F13 = 0.0;
//  double F21 = 0.0;        double F22 = 1.0+gamma;             double F23 = 0.0;
//  double F31 = 0.0;        double F32 = 0.0;                   double F33 = 1.0+gamma;
