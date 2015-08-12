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

#include "AllCEMModels.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <BardetConstraintLT.h>

using namespace std;

int main (void)
{
	DTensor2 de(3, 3, 0);

	de.Identity();

	de *= -0.01;

	int tag = 1;
	double k0 = 1.0;
	double H_alpha = 10;
	double H_k = 0;//10;
	double E = 1000;
	double nu = 0.25;
	double rho = 0.0;
	DruckerPragerLinearHardening drucker(tag , k0 , H_alpha , H_k, E, nu, rho);

	// (int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu) :
	DTensor2 sigma(3, 3, 0);
	DTensor2 epsilon(3, 3, 0);

	for (int i = 0; i < 5; i++)
	{
		drucker.setTrialStrainIncr(de);
		sigma = drucker.getStressTensor();
		epsilon = drucker.getStrainTensor();
		drucker.commitState();
		cout << "sigma = " << sigma << endl;
		cout << "epsilon = "  << epsilon << endl;
	}

	double p = sigma(i, i) / 3;

	NDMaterialLT* mat = &drucker;
	NDMaterialLT* mat2 = mat->getCopy();   //Test creating a copy


	BardetConstraintLT bard(DRAINED_TRIAXIAL_LOADING_STRAIN_CONTROL, mat2);
	// BardetConstraintLT bard(DRAINED_TRIAXIAL_LOADING_STRESS_CONTROL, mat);


	// return 0;

	cout << "========================================\n";
	cout << "Start Bardet Integration\n";
	cout << "========================================\n";

	fstream fout("out_drucker.txt", ios::out);

	DTensor2 depsilon(3, 3, 0);
	double de1 = 0.001;
	// double de1 = 0.05;
	int Nsteps_start = 170;
	int Nsteps = Nsteps_start;
	DTensor2 s(3, 3, 0);
	DTensor2 e(3, 3, 0);

	fout << "  exx   (0 )   "
	     << "  eyy   (1 )   "
	     << "  ezz   (2 )   "
	     << "  exy   (3 )   "
	     << "  exz   (4 )   "
	     << "  eyz   (5 )   "
	     << "  sxx   (6 )   "
	     << "  syy   (7 )   "
	     << "  szz   (8 )   "
	     << "  sxy   (9 )   "
	     << "  sxz   (10)   "
	     << "  syz   (11)   "
	     << "  p     (12)   "
	     << "  ev    (13)   "
	     << "  q     (14)   "
	     << "  eq    (15)   " << endl;
//           0123456789012345
	int count = 0;
	bool die = false;
	for (int n = 0; n < 3; n++)
	{
		de1 *= -1.0;
		cout << "==============> de = " << de1 << endl;
		for (int ii = 0; ii < Nsteps; ii++)
		{
			bard.applyIncrement(de1);
			// sigma = bard.getStress();
			// depsilon = bard.getStrainIncrement();
			// epsilon(i, j) += depsilon(i, j);
			sigma = mat2->getStressTensor();
			epsilon = mat2->getStrainTensor();
			// cout << "epsilon = " << epsilon << endl;
			// cout << "sigma = " << sigma << endl;

			double p, q, ev, eq;
			p = -sigma(i, i) / 3;
			ev = -epsilon(i, i) / 3;
			q = -sigma(0, 0) + sigma(1, 1);
			eq = -epsilon(0, 0) + epsilon(1, 1);
			// s(i, j) = sigma(i, j) + p * kronecker_delta(i, j);
			// e(i, j) = epsilon(i, j) + ev * kronecker_delta(i, j);
			// sigma.compute_deviatoric_tensor(s, p); // here p is positive if in tension
			// epsilon.compute_deviatoric_tensor(e, ev); // here p is positive if in tension
			// p = -p;
			// ev = -ev;
			// double J2 = s(i, j) * s(i, j);
			// double J2p = e(i, j) * e(i, j);
			// q = (3.0 / 2.0 * J2) / sqrt(3.0 / 2.0 * J2);
			// eq = 2.0 * sqrt(3.0 / 2.0 * J2p);





			fout << setw(14) << scientific << epsilon(0, 0) << " "
			     << setw(14) << scientific << epsilon(1, 1) << " "
			     << setw(14) << scientific << epsilon(2, 2) << " "
			     << setw(14) << scientific << epsilon(0, 1) << " "
			     << setw(14) << scientific << epsilon(0, 2) << " "
			     << setw(14) << scientific << epsilon(1, 2) << " "
			     << setw(14) << scientific << sigma(0, 0) << " "
			     << setw(14) << scientific << sigma(1, 1) << " "
			     << setw(14) << scientific << sigma(2, 2) << " "
			     << setw(14) << scientific << sigma(0, 1) << " "
			     << setw(14) << scientific << sigma(0, 2) << " "
			     << setw(14) << scientific << sigma(1, 2) << " "
			     << setw(14) << scientific << p << " "
			     << setw(14) << scientific << ev << " "
			     << setw(14) << scientific << q << " "
			     << setw(14) << scientific << eq << endl;
			// if (de1 < 0)
			// {
			// 	cout << "Bunkants!\n";
			// 	break;
			// }
			count++;
			if (count == 186)
			{
				cout << "Dieded!\n";
				die = true;
				break;
			}
		}
		if (die)break;
		// if (de1 < 0)
		// {
		// 	cout << "Bunkants!\n";
		// 	break;
		// }
		Nsteps = 2 * Nsteps_start;
	}


	fout.close();


	// fstream fout("out.txt", ios::out);

	// DTensor2 de1(3, 3, 0.0);
	// de1(0, 1) = 0.0001;
	// de1(1, 0) = 0.0001;
	// for (int n = 0; n < 2; n++)
	// {
	// 	de1(0, 0) *= -1.0;
	// 	for (int i = 0; i < 50; i++)
	// 	{
	// 		drucker.setTrialStrainIncr(de1);
	// 		sigma = drucker.getStressTensor();
	// 		epsilon = drucker.getStrainTensor();
	// 		drucker.commitState();
	// 		cout << "epsilon = " << epsilon << endl;
	// 		cout << "sigma = " << sigma << endl;

	// 		fout << epsilon(0, 0) << " "
	// 		     << epsilon(1, 1) << " "
	// 		     << epsilon(2, 2) << " "
	// 		     << epsilon(0, 1) << " "
	// 		     << epsilon(0, 2) << " "
	// 		     << epsilon(1, 2) << " "
	// 		     << sigma(0, 0) << " "
	// 		     << sigma(1, 1) << " "
	// 		     << sigma(2, 2) << " "
	// 		     << sigma(0, 1) << " "
	// 		     << sigma(0, 2) << " "
	// 		     << sigma(1, 2) << endl;
	// 	}
	// }
	// fout.close();

	cout << "C'est fini!" << endl;

	return 0;
}