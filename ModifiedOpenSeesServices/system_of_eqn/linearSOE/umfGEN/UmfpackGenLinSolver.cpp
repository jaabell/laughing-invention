///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              UmfpackGenLinSolver.cpp
// CLASS:             UmfpackGenLinSolver
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// PROGRAMMER:        Nima Tafazzoli and Boris Jeremic
// DATE:              April 2011
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////
//
// Writing of this program was initiated by Frank McKenna in September 2000
// and then completed and linked to MOSS for UMFPACK5.5.1
// by Nima Tafazzoli and Boris Jeremic in April 2011
//
// Description: This file contains the class definition for
// UmfpackGenLinSolver. It solves the UmfpackGenLinSOEobject by calling
// UMFPACK5.5.1 routines.
//

//  ================= Uncomment for stiffness matrix output
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <iostream>
using namespace std;


typedef std::numeric_limits< double > dbl;

// ==============================================================

#include <UmfpackGenLinSOE.h>
#include <UmfpackGenLinSolver.h>
#include <f2c.h>
#include <math.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>




extern "C" {
#include <umfpack.h>
}

using namespace std;


UmfpackGenLinSolver::
UmfpackGenLinSolver()
    : LinearSOESolver(SOLVER_TAGS_UmfpackGenLinSolver),
      Control(0), Info(0), Symbolic(0), Numeric(0), theSOE(0)
{
    Control = new double[UMFPACK_CONTROL];
    //   Control = new double[UMFPACK_STRATEGY_UNSYMMETRIC];
    //   Control = new double[UMFPACK_IRSTEP];
    //   double Control [UMFPACK_STRATEGY_UNSYMMETRIC];
    Info = new double[UMFPACK_INFO];
    //  double Info [UMFPACK_INFO];

    umfpack_di_defaults(Control);
}


UmfpackGenLinSolver::~UmfpackGenLinSolver()
{
    if (Symbolic != 0)
    {
        umfpack_di_free_symbolic (&Symbolic) ;
    }

    if (Numeric != 0)
    {
        umfpack_di_free_numeric (&Numeric) ;
    }

    if (Control != 0)
    {
        delete [] Control;
    }

    if (Info != 0)
    {
        delete [] Info;
    }
}

int
UmfpackGenLinSolver::solve(void)
{
    if (theSOE == 0)
    {
        cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
        cerr << " No LinearSOE object has been set\n";
        return -1;
    }

    int n = theSOE->size;

    // check for quick return
    if (n == 0)
    {
        return 0;
    }

    // first copy B into X
    double* Xptr = theSOE->X;
    double* Bptr = theSOE->B;
    double* Aptr = theSOE->A;

    int* Ap = theSOE->rowStartA;
    int* Ai = theSOE->colA;
    int status = 0;

    int Asize2 = theSOE->Asize;
    int Bsize2 = theSOE->Bsize;
    int Apsize2 = Bsize2 + 1;
    int Aisize2 = theSOE->nnz;


    //**************************************************************************
    // Output the stiffness matrix into a different file for each step.
    // Must uncomment some headers above...
    // static unsigned int times_called = 0;
    // std::stringstream ss(std::stringstream::in | std::stringstream::out);
    // ss << "soe_step_" << ++times_called << ".out"; //
    // std::string fname(ss.str());
    // std::fstream fid(fname.c_str(), std::ios::out);

    // fid << "size of Ai is: " << Aisize2 << endl;
    // fid << "size of Ap is: " << Apsize2 << endl;
    // fid << "size of A is: " << Asize2 << endl;
    // fid << "size of B is: " << Bsize2 << endl;

    // cout.precision(dbl::digits10);

    // for (int w = 0; w < Apsize2; w++)
    // {
    //     fid << "Ap[" << w << "]" << "=" << (Ap)[w] << endl;
    // }

    // for (int w = 0; w < Aisize2; w++)
    // {
    //     fid << "Ai[" << w << "]" << "=" << (Ai)[w] << endl;
    // }

    // for (int w = 0; w < Asize2; w++)
    // {
    //     fid << "A[" << w << "]" << "=" << setprecision(dbl::digits10) << fixed <<  (Aptr)[w] << endl;
    // }

    // for (int w = 0; w < Bsize2; w++)
    // {
    //     fid << "B[" << w << "]" << "=" << setprecision(dbl::digits10) << fixed <<  (Bptr)[w] << endl;
    // }

    // fid.close();
    //**************************************************************************



    if (theSOE->factored == false)
    {

        // factor the matrix
        status =  umfpack_di_numeric (Ap, Ai, Aptr, Symbolic, &Numeric, Control, Info) ;

        if (status != UMFPACK_OK)
        {
            cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
            cerr << status << " returned in umfpack_numeric()\n";
            //             • umfpack * report status:
            // Prints the status (return value) of other umfpack * routines.
            // • umfpack * report info:
            cout << "==== From UMFPACK: umfpack_di_report_info() ====\n";
            umfpack_di_report_status(Control, status);
            umfpack_di_report_info(Control, Info);
            cout << "======== END: umfpack_di_report_info() ========\n";
            return -status;
        }

        theSOE->factored = true;
    }




    // do forward and backward substitution
    //     status =  umfpack_di_solve ("Ax=b", Ap, Ai, Aptr, Xptr, Bptr, Numeric, Control, Info) ;
    //     status =  umfpack_di_solve (UMFPACK_A, Ap, Ai, Aptr, Xptr, Bptr, Numeric, Control, Info) ;
    status =  umfpack_di_solve (UMFPACK_At, Ap, Ai, Aptr, Xptr, Bptr, Numeric, Control, Info);

    if (status != UMFPACK_OK)
    {
        cerr << "WARNING UmfpackGenLinSolver::solve(void)- ";
        cerr << status << " returned in umpack_solve()\n";
        cout << "==== From UMFPACK: umfpack_di_report_info() ====\n";
        umfpack_di_report_status(Control, status);
        umfpack_di_report_info(Control, Info);
        cout << "======== END: umfpack_di_report_info() ========\n";
        return -status;
    }


    //     umfpack_di_free_symbolic (&Symbolic);
    umfpack_di_free_numeric(&Numeric);

    return 0;
}


int
UmfpackGenLinSolver::setSize()
{
    int n = theSOE->size;
    int* Ap = theSOE->rowStartA;
    int* Ai = theSOE->colA;
    double* Aptr = theSOE->A;


    if (n > 0)
    {

        // do the symbolic factorization
        int status =  umfpack_di_symbolic (n, n, Ap, Ai, Aptr, &Symbolic, Control, Info) ;

        if (status != status)
        {
            cerr << "WARNING UmfpackGenLinSolver::setSize(void)- ";
            cerr << status << " returned in umfpack_symbolic()\n";
            return -status;
        }
    }

    return 0;
}

int
UmfpackGenLinSolver::setLinearSOE(UmfpackGenLinSOE& theLinearSOE)
{
    theSOE = &theLinearSOE;
    return 0;
}

int
UmfpackGenLinSolver::sendSelf(int cTag, Channel& theChannel)
{
    // nothing to do
    return 0;
}

int
UmfpackGenLinSolver::receiveSelf(int ctag,
                                 Channel& theChannel,
                                 FEM_ObjectBroker& theBroker)
{
    // nothing to do
    return 0;
}

