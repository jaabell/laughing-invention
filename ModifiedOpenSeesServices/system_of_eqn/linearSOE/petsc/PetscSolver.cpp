/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.3 $
// $Date: 2005/05/18 19:25:03 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/petsc/PetscSolver.cpp,v $

// Written: fmk & om
// Created: 7/98
// Revision: A
//
// Description: This file contains the class implementation for
// FullGenLinLapackSolver. It solves the FullGenLinSOE object by calling
// Lapack routines.
//
// What: "@(#) FullGenLinLapackSolver.h, revA"

#include <OPS_Globals.h>
#include <PetscSolver.h>
#include <PetscSOE.h>
#include <f2c.h>
#include <math.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <Timer.h>
#include <ID.h>
#include <Vector.h>
#include <string.h>
#include "petscksp.h"

//#include <iostream>


//added by Babak Kamrani 01/24/2012:
//-------------------------------------

// #include <iostream>
// #include <fstream>
// using namespace std;

#include <stdlib.h>
#include <iostream>
#include <fstream>
//#include <FileStream.h>
#include <stdio.h>
//#include <sstream>

using namespace std;
//-------------------------------------

#ifdef _BABAK_DEBUG
extern    int g_Step_Number ;
#endif

PetscSolver::PetscSolver()
    : LinearSOESolver(SOLVER_TAGS_PetscSolver),
      rTol(PETSC_DEFAULT), aTol(PETSC_DEFAULT), dTol(PETSC_DEFAULT), maxIts(PETSC_DEFAULT), matType(MATMPIAIJ)
{

}

PetscSolver::PetscSolver(KSPType meth, PCType pre)
    : LinearSOESolver(SOLVER_TAGS_PetscSolver), method(meth), preconditioner(pre),
      rTol(PETSC_DEFAULT), aTol(PETSC_DEFAULT), dTol(PETSC_DEFAULT), maxIts(PETSC_DEFAULT), matType(MATMPIAIJ)
{

}

PetscSolver::PetscSolver(KSPType meth, PCType pre, double relTol, double absTol, double divTol, int maxIterations, MatType mat)
    : LinearSOESolver(SOLVER_TAGS_PetscSolver), method(meth), preconditioner(pre),
      rTol(relTol), aTol(absTol), dTol(divTol), maxIts(maxIterations), matType(mat)
{

}

PetscSolver::~PetscSolver()
{

}


//------------------------------out by Babak and replaced 7/9/13---------------------
int
PetscSolver::solve(void)
{




    int size = theSOE->size;
    cerr << "PetscSolver::solve:   --Size of the SOE is: " << size << endln; //added by babak kamrani for debugging purposes


    int factored = theSOE->isFactored;

    int numProcesses = theSOE->numProcesses;
    int processID = theSOE->processID;

    int ierr;

    //MatSetType(theSOE->A, matType);
    std::cerr << "BABAK@PetscSolver::solve MatAssemblyBegin ... \n";
    ierr = MatAssemblyBegin(theSOE->A, MAT_FINAL_ASSEMBLY);
    CHKERRQ(ierr);
    std::cerr << "BABAK@PetscSolver::solve MatAssemblyEnd ... \n";

    ierr = MatAssemblyEnd(theSOE->A, MAT_FINAL_ASSEMBLY);
    CHKERRQ(ierr);

    //    MatSetOption(theSOE->A, MAT_SPD, PETSC_TRUE );

    KSPCreate(PETSC_COMM_WORLD, &ksp);
    KSPSetOperators(ksp, theSOE->A, theSOE->A, DIFFERENT_NONZERO_PATTERN);







    // //--mumps----:
    //
    //    KSPSetType(ksp,KSPPREONLY);
    //    Mat       F;
    //    PetscInt  ival,icntl;
    //    PetscReal val;
    //    KSPGetPC(ksp,&pc);
    //    PCSetType(pc,PCLU);
    // //   MatSetOption(A,MAT_SPD,PETSC_TRUE); /* set MUMPS id%SYM=1 */
    // //   PCSetType(pc,PCCHOLESKY);
    //     PCFactorSetMatSolverPackage(pc,MATSOLVERMUMPS);
    //     PCFactorSetUpMatSolverPackage(pc); /* call MatGetFactor() to create F */
    // //     MatGetFactor(theSOE->A, MATSOLVERMUMPS ,MAT_FACTOR_LU,&F);
    //     PCFactorGetMatrix(pc,&F);
    //     icntl = 7; ival = 2;
    //     MatMumpsSetIcntl(F,icntl,ival);
    //     icntl = 1; val = 0.0;
    // // //     MatMumpsSetCntl(F,icntl,val);
    //     KSPSetFromOptions(ksp);
    // //-----------------------------



    //--SuperLu-dist----:
    //    KSPSetType(ksp,KSPPREONLY);
    //     Mat       F;
    //     KSPGetPC(ksp,&pc);
    //     PCSetType(pc,PCLU);
    //
    //    PCFactorSetMatSolverPackage(pc,MATSOLVERSUPERLU_DIST);
    //    PCFactorSetUpMatSolverPackage(pc); /* call MatGetFactor() to create F */
    //     PCFactorGetMatrix(pc,&F);
    //     MatSuperluSetILUDropTol(F,1.e-1);
    //    KSPSetFromOptions(ksp);
    //    KSPSetOperators(ksp,theSOE->A,theSOE->A,DIFFERENT_NONZERO_PATTERN);

    //-----------------------------


    //--petsc solver----:

    // KSPSetType(ksp,KSPPREONLY);
    // Mat       F;
    // KSPGetPC(ksp,&pc);
    // PCSetType(pc,PCLU);
    // PCFactorSetMatSolverPackage(pc,MATSOLVERPETSC);
    // PCFactorSetUpMatSolverPackage(pc); /* call MatGetFactor() to create F */
    // PCFactorGetMatrix(pc,&F);
    // KSPSetFromOptions(ksp);
    //-----------------------------





    //----- to test spooles -----------------
    KSPSetType(ksp, KSPPREONLY);
    KSPGetPC(ksp, &pc);
    PCSetType(pc, PCLU);
    PCFactorSetMatSolverPackage(pc, MATSOLVERSPOOLES);
    KSPSetFromOptions(ksp);
    KSPSetOperators(ksp, theSOE->A, theSOE->A, DIFFERENT_NONZERO_PATTERN);
    //---------------------------------------




    static Vector recvVector(1);

    if (numProcesses > 1)
    {
        Vector *vectX = theSOE->vectX;
        Vector *vectB = theSOE->vectB;

#ifdef _BABAK_DEBUG
        cerr << "PetscSolver::solve() -- PID# " << processID << "VectB: " << theSOE->vectB << "\n";
#endif

        // zero X
        vectX->Zero();

        //
        // form B on each
        //

        int numChannels = theSOE->numChannels;
        Channel **theChannels = theSOE->theChannels;

        if (processID != 0)
        {
            Channel *theChannel = theChannels[0];

            theChannel->sendVector(0, 0, *vectB);
            theChannel->recvVector(0, 0, *vectB);

        }
        else
        {

            if (recvVector.Size() != size)
            {
                recvVector.resize(size);
            }

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->recvVector(0, 0, recvVector);
                *vectB += recvVector;
            }

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->sendVector(0, 0, *vectB);
            }
        }
    }

    //
    // solve and mark as having been solved
    //

    //GZ added
    //KSPSetOperators(ksp, theSOE->A, theSOE->A, DIFFERENT_NONZERO_PATTERN);
    //KSPView(ksp, PETSC_VIEWER_STDOUT_WORLD);
    //MatView(theSOE->A, PETSC_VIEWER_STDOUT_WORLD);
    //VecView(theSOE->b, PETSC_VIEWER_STDOUT_WORLD);

    //GZ # ifdef _TIMING
    //GZ # ifdef _PARALLEL_PROCESSING //Guanzhou added
    double start_time = MPI_Wtime();
    //GZ # endif
    //GZ # endif
    //Added By Babak KAmrani 04/03/2012:

    //  theSOE->DumpB();
    //

    ierr = KSPSolve(ksp, theSOE->b, theSOE->x);
    CHKERRQ(ierr);

    //Added by Babak Kamrani at 01/24/2012
    //The following lines will dump all nodes displacement data in a file :
    //----------------------------------------------
    // KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD);

#ifdef _BABAK_DEBUG

    cout << "PetscSolver::solve() -- PID#    " << processID << "     writing right hand side in RHS.txt file ... " << endl;
    PetscViewer    viewer_RHS;
    PetscViewerASCIIOpen(PETSC_COMM_WORLD, "RHS.txt", &viewer_RHS);
    VecView(theSOE->b, viewer_RHS);







    //      if (g_Step_Number == 1)
    //      {
    //
    // //     std::ostringstream Out;
    // //
    // //     Out << "Disp_Step#" << g_Step_Number<< ".txt";
    // //     std::string Outputfilename = Out.str();
    //       //char Outputfilename[20];
    //       //strcpy (Outputfilename,"X_Step");
    //       //strncat (Outputfilename, ::g_Step_Number, 6);
    //     //puts (Outputfilename);

    //
    //
    //
    //


    cout << "PetscSolver::solve() -- PID#    " << processID << " writing results ... " << endl;
    PetscViewer    viewer_x;
    PetscViewerASCIIOpen(PETSC_COMM_WORLD, "X.txt", &viewer_x);
    VecView(theSOE->x, viewer_x);
    //

    //      }

#endif


    ////////////////////////////////////////



    //-----------------------
    //VecView(theSOE->x, PETSC_VIEWER_STDOUT_WORLD);

    //GZ # ifdef _TIMING
    //GZ # ifdef _PARALLEL_RPOCESSING //Guanzhou added
    double end_time = MPI_Wtime();

    if ( processID == 0 )
    {
        cerr << "Timing used by solving: " << end_time - start_time << endln;
    }

    //GZ # endif
    //GZ # endif


    theSOE->isFactored = 1;

    //
    // if parallel, we must form the total X: each processor has startRow through endRow-1
    //

    if (numProcesses > 1)
    {
        Vector *vectX = theSOE->vectX;

        int numChannels = theSOE->numChannels;
        Channel **theChannels = theSOE->theChannels;

        if (processID != 0)
        {
            Channel *theChannel = theChannels[0];

            theChannel->sendVector(0, 0, *vectX);
            theChannel->recvVector(0, 0, *vectX);

        }
        else
        {

            if (recvVector.Size() != size)
            {
                recvVector.resize(size);
            }

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->recvVector(0, 0, recvVector);
                *vectX += recvVector;
            }

            for (int j = 0; j < numChannels; j++)
            {
                Channel *theChannel = theChannels[j];
                theChannel->sendVector(0, 0, *vectX);
            }

            //Added by Babak Kamrani at 01/24/2012
            //The following lines will dump all nodes displacement data in a file :
            //------------------------------------------------------------------------
            //       if (processID == 0)
            //       {
            //        char *fileName = "Disp_Dump.txt";
            //        openMode theOMode = APPEND;
            //        FileStream Dispoutputfile;
            //        Dispoutputfile.setFile(fileName, theOMode);
            //        Dispoutputfile << *vectX ;
            //        Dispoutputfile << "Hi";
            // //        Dispoutputfile.close();
            //       }
            //------------------------------------------------------------------------
        }
    }






    ierr = KSPDestroy(&ksp);
    CHKERRQ(ierr);

    //added by Babak Kamrani for debugging purposes 04/09/2012:
    //   cerr<< "BBBB" <<theSOE->getB();
    //   cerr<< "XXXX" <<theSOE->getX();
    //cerr<< "AAAA" <<theSOE->getA();
    //////////////////////////////////////////////////////////

    return ierr;



}
//---------------------------------------------------------------------------------------------------------------------------

// int
// PetscSolver::solve(void)
// {
//   //Major changes is done by Babak to make it work with direct solver ...
//   int size = theSOE->size;
//   int factored = theSOE->isFactored;
//
//   int numProcesses = theSOE->numProcesses;
//   int processID = theSOE->processID;
//
//   int ierr;
//
//   //MatSetType(theSOE->A, matType);
//
//   ierr = MatAssemblyBegin(theSOE->A, MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
//   ierr = MatAssemblyEnd(theSOE->A, MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
//
//
//   //Create the linear solver context:
//   //----------------------------------------------------------
//   KSPCreate(PETSC_COMM_WORLD, &ksp);
//   KSPSetOperators(ksp,theSOE->A,theSOE->A,DIFFERENT_NONZERO_PATTERN);
//   //----------------------------------------------------------
//
//
//   KSPSetType(ksp,KSPPREONLY);
//   KSPGetPC(ksp,&pc);
//   PCSetType(pc,PCLU);
//   PCFactorSetMatSolverPackage(pc,MATSOLVERSUPERLU_DIST);
//   PCFactorSetUpMatSolverPackage(pc); /* call MatGetFactor() to create F */
//   Mat       F;
//   PCFactorGetMatrix(pc,&F);
//   MatSuperluSetILUDropTol(F,1.e-8);
//   KSPSetUp(ksp);
//   KSPSetFromOptions(ksp);
//
//
//
//
//
//
//
//
//
//
//
//
//   static Vector recvVector(1);
//
//   if (numProcesses > 1)
//   {
//     Vector *vectX = theSOE->vectX;
//     Vector *vectB = theSOE->vectB;
//
//     // zero X
//     vectX->Zero();
//
//     //
//     // form B on each
//     //
//
//     int numChannels = theSOE->numChannels;
//     Channel **theChannels = theSOE->theChannels;
//
//     if (processID != 0)
//     {
//       Channel *theChannel = theChannels[0];
//
//       theChannel->sendVector(0, 0, *vectB);
//       theChannel->recvVector(0, 0, *vectB);
//
//     } else
//     {
//
//       if (recvVector.Size() != size)
//  recvVector.resize(size);
//       for (int j=0; j<numChannels; j++)
//       {
//  Channel *theChannel = theChannels[j];
//  theChannel->recvVector(0, 0, recvVector);
//  *vectB += recvVector;
//       }
//       for (int j=0; j<numChannels; j++)
//       {
//  Channel *theChannel = theChannels[j];
//  theChannel->sendVector(0, 0, *vectB);
//       }
//     }
//   }
//
//   //
//   // solve and mark as having been solved
//   //
//
//   //GZ added
//   //KSPSetOperators(ksp, theSOE->A, theSOE->A, DIFFERENT_NONZERO_PATTERN);
//   //KSPView(ksp, PETSC_VIEWER_STDOUT_WORLD);
//   //MatView(theSOE->A, PETSC_VIEWER_STDOUT_WORLD);
//   //VecView(theSOE->b, PETSC_VIEWER_STDOUT_WORLD);
//
// //GZ # ifdef _TIMING
// //GZ # ifdef _PARALLEL_PROCESSING //Guanzhou added
//  double start_time = MPI_Wtime();
// //GZ # endif
// //GZ # endif
//   ierr = KSPSolve(ksp, theSOE->b, theSOE->x); CHKERRQ(ierr);
//   //VecView(theSOE->x, PETSC_VIEWER_STDOUT_WORLD);
//
// //GZ # ifdef _TIMING
// //GZ # ifdef _PARALLEL_RPOCESSING //Guanzhou added
//  double end_time = MPI_Wtime();
//  if ( processID == 0 )
//      cerr << "Timing used by solving: " << end_time-start_time << endln;
// //GZ # endif
// //GZ # endif
//
//
//   theSOE->isFactored = 1;
//
//   //
//   // if parallel, we must form the total X: each processor has startRow through endRow-1
//   //
//
//   if (numProcesses > 1) {
//     Vector *vectX = theSOE->vectX;
//
//     int numChannels = theSOE->numChannels;
//     Channel **theChannels = theSOE->theChannels;
//
//     if (processID != 0) {
//       Channel *theChannel = theChannels[0];
//
//       theChannel->sendVector(0, 0, *vectX);
//       theChannel->recvVector(0, 0, *vectX);
//
//     } else {
//
//       if (recvVector.Size() != size)
//  recvVector.resize(size);
//
//       for (int j=0; j<numChannels; j++) {
//  Channel *theChannel = theChannels[j];
//  theChannel->recvVector(0, 0, recvVector);
//  *vectX += recvVector;
//       }
//       for (int j=0; j<numChannels; j++) {
//  Channel *theChannel = theChannels[j];
//  theChannel->sendVector(0, 0, *vectX);
//       }
//     }
//   }
//
//   ierr = KSPDestroy(&ksp);CHKERRQ(ierr);
//
//   return ierr;
// }




int
PetscSolver::setSize()
{
    /*
     * Create linear solver context
     */

    KSPCreate(PETSC_COMM_WORLD, &ksp);

    //GZ out   /*
    //GZ out    *  Set operators. NOTE: matrix that defines the linear system
    //GZ out    *  also serves as the preconditioning matrix.
    //GZ out    */
    //GZ out
    //GZ out   //GZ added to use ICC from BlockSolve95
    //GZ out   Mat P;
    //GZ out   MatCreate(PETSC_COMM_WORLD,&P);
    //GZ out   MatDuplicate(theSOE->A,MAT_COPY_VALUES,&P);
    //GZ out   MatSetType(P, MATMPIROWBS);
    //GZ out   MatSetFromOptions(P);
    //GZ out   //KSPSetOperators(ksp, theSOE->A, theSOE->A, DIFFERENT_NONZERO_PATTERN);
    //GZ out   KSPSetOperators(ksp, theSOE->A, P, DIFFERENT_NONZERO_PATTERN);
    //GZ out
    //GZ out   /*
    //GZ out    *  Set solution scheme & tolerances
    //GZ out    */
    //GZ out
    //GZ out   int ierr;
    //GZ out   ierr = KSPSetType(ksp, method); CHKERRQ(ierr);
    //GZ out   ierr = KSPSetTolerances(ksp, rTol, aTol, dTol, maxIts);
    //GZ out
    //GZ out   /*
    //GZ out    *  Set preconditioning scheme
    //GZ out    */
    //GZ out
    //GZ out   KSPGetPC(ksp, &pc);
    //GZ out   ierr = PCSetType(pc,  preconditioner); CHKERRQ(ierr);
    //GZ out
    //GZ out   /*
    //GZ out    *  Finally mark so that uses last solution as initial guess in each solution
    //GZ out    *    NOTE: maybe change this as another user supplied option
    //GZ out    */
    //GZ out
    //GZ out   //   ierr = KSPSetInitialGuessNonzero(ksp, PETSC_TRUE); CHKERRQ(ierr);
    //GZ out   return ierr;
}


int
PetscSolver::setLinearSOE(PetscSOE &theSys)
{
    theSOE = &theSys;
    return 0;
}


int
PetscSolver::sendSelf(int cTag, Channel &theChannel)
{
    static ID idData(4);
    idData(0) = maxIts;
    //if (method == KSPCG)
    //  idData(1) = 0;
    //else if (method == KSPBICG)
    //  idData(1) = 1;
    //else if (method == KSPRICHARDSON)
    //  idData(1) = 2;
    //else if (method == KSPCHEBYCHEV)
    //  idData(1) = 3;
    //else if (method == KSPGMRES)
    //  idData(1) = 4;
    //else if (method == KSPPREONLY)
    //  idData(1) = 5;
    //else {
    //  cerr << "PetscSolver::sendSelf() - unknown method set\n";
    //  return -1;
    //}
    //
    //if (preconditioner == PCJACOBI)
    //  idData(2) = 0;
    //else if (preconditioner == PCILU)
    //  idData(2) = 1;
    //else if (preconditioner == PCICC)
    //  idData(2) = 2;
    //else if (preconditioner == PCBJACOBI)
    //  idData(2) = 3;
    //else if (preconditioner == PCNONE)
    //  idData(2) = 4;
    //else if (preconditioner == PCLU)
    //  idData(2) = 5;
    //else if (preconditioner == PCCHOLESKY)
    //  idData(2) = 6;
    //else {
    //  cerr << "PetscSolver::sendSelf() - unknown preconditioner set\n";
    //  return -1;
    //}

    if (strcmp(method, "cg") == 0)
    {
        idData(1) = 0;
    }
    else if (strcmp(method, "bicg") == 0)
    {
        idData(1) = 1;
    }
    else if (strcmp(method, "richardson") == 0)
    {
        idData(1) = 2;
    }
    else if (strcmp(method, "chebychev") == 0)
    {
        idData(1) = 3;
    }
    else if (strcmp(method, "gmres") == 0)
    {
        idData(1) = 4;
    }
    else if (strcmp(method, "preonly") == 0)
    {
        idData(1) = 5;
    }
    else
    {
        cerr << "PetscSolver::sendSelf() - unknown method set\n";
        return -1;
    }

    if (strcmp(preconditioner, "jacobi") == 0)
    {
        idData(2) = 0;
    }
    else if (strcmp(preconditioner, "ilu") == 0)
    {
        idData(2) = 1;
    }
    else if (strcmp(preconditioner, "icc") == 0)
    {
        idData(2) = 2;
    }
    else if (strcmp(preconditioner, "bjacobi") == 0)
    {
        idData(2) = 3;
    }
    else if (strcmp(preconditioner, "none") == 0)
    {
        idData(2) = 4;
    }
    else if (strcmp(preconditioner, "lu") == 0)
    {
        idData(2) = 5;
    }
    else if (strcmp(preconditioner, "cholesky") == 0)
    {
        idData(2) = 6;
    }
    else
    {
        cerr << "PetscSolver::sendSelf() - unknown preconditioner set\n";
        return -1;
    }

    //if (matType == MATSUPERLU_DIST)
    //  idData(3) = 0;
    //else if (matType == MATMPISBAIJSPOOLES)
    //  idData(3) = 1;
    //else if (matType == MATMPIROWBS)
    //  idData(3) = 2;
    if (strcmp(matType, "superlu_dist") == 0)
    {
        idData(3) = 0;
    }
    else if (strcmp(matType, "mpisbaijspooles") == 0)
    {
        idData(3) = 1;
    }
    else if (strcmp(matType, "mpirowbs") == 0)
    {
        idData(3) = 2;
    }
    else if (strcmp(matType, "mpiaijspooles") == 0)
    {
        idData(3) = 3;
    }
    else if (strcmp(matType, "aijmumps") == 0)
    {
        idData(3) = 4;
    }
    else
    {
        idData(3) = 99;
        //  cerr << "PetscSolver::sendSelf() - unknown matType set\n";
        //  return -1;
    }



    theChannel.sendID(0, cTag, idData);

    static Vector data(3);
    data(0) = rTol;
    data(1) = aTol;
    data(2) = dTol;

    theChannel.sendVector(0, cTag, data);
    return 0;
}

int
PetscSolver::recvSelf(int cTag, Channel &theChannel,
                      FEM_ObjectBroker &theBroker)
{
    static ID idData(4);
    theChannel.recvID(0, cTag, idData);
    maxIts = idData(0);

    if (idData(1) == 0)
    {
        method = KSPCG;
    }
    else if (idData(1) == 1)
    {
        method = KSPBICG;
    }
    else if (idData(1) == 2)
    {
        method = KSPRICHARDSON;
    }
    else if (idData(1) == 3)
    {
        method = KSPCHEBYSHEV;
    }
    else if (idData(1) == 4)
    {
        method = KSPGMRES;
    }
    else if (idData(1) == 5)
    {
        method = KSPPREONLY;
    }
    else
    {
        cerr << "PetscSolver::recvSelf() - unknown method recvd\n";
        return -1;
    }

    if (idData(2) == 0)
    {
        preconditioner = PCJACOBI;
    }
    else if (idData(2) == 1)
    {
        preconditioner = PCILU;
    }
    else if (idData(2) == 2)
    {
        preconditioner = PCICC;
    }
    else if (idData(2) == 3)
    {
        preconditioner = PCBJACOBI;
    }
    else if (idData(2) == 4)
    {
        preconditioner = PCNONE;
    }
    else if (idData(2) == 5)
    {
        preconditioner = PCLU;
    }
    else if (idData(2) == 6)
    {
        preconditioner = PCCHOLESKY;
    }
    else
    {
        cerr << "PetscSolver::recvSelf() - unknown preconditioner set\n";
        return -1;
    }

    if (idData(3) == 0)
        //matType = MATSUPERLU_DIST; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    {
        matType = MATSOLVERSUPERLU_DIST;
    }
    else if (idData(3) == 1)
        //matType = MATMPISBAIJSPOOLES; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    {
        matType = MATSOLVERSPOOLES;
    }
    //else if (idData(3) == 2)
    //matType = MATMPIROWBS;  Ignored because this interface has been removed fro petsc 3.1   by B. Kamrani (Aug. 2010)
    else if (idData(3) == 3)
        //matType = MATMPIAIJSPOOLES; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    {
        matType = MATSOLVERSPOOLES;
    }
    else if (idData(3) == 4)
        //matType = MATAIJMUMPS; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    {
        matType = MATSOLVERMUMPS;
    }
    else
    {
        matType = MATMPIAIJ;
        //  cerr << "PetscSolver::recvSelf() - unknown matType set\n";
        //  return -1;
    }

    static Vector data(3);
    theChannel.recvVector(0, cTag, data);
    rTol = data(0);
    aTol = data(1);
    dTol = data(2);

    theSOE->mType = matType;

    return 0;
}



