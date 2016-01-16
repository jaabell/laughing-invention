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


#include <ESSITimer.h> //Need this up here to avoid macro error with PETSc

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


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
//-------------------------------------

PetscSolver::PetscSolver()
    : LinearSOESolver(SOLVER_TAGS_PetscSolver),
      rTol(PETSC_DEFAULT), aTol(PETSC_DEFAULT), dTol(PETSC_DEFAULT), maxIts(PETSC_DEFAULT), matType(MATMPIAIJ),
      is_KSP_initialized(false)
{

}

PetscSolver::PetscSolver(KSPType meth, PCType pre)
    : LinearSOESolver(SOLVER_TAGS_PetscSolver), method(meth), preconditioner(pre),
      rTol(PETSC_DEFAULT), aTol(PETSC_DEFAULT), dTol(PETSC_DEFAULT), maxIts(PETSC_DEFAULT), matType(MATMPIAIJ),
      is_KSP_initialized(false)
{

}

PetscSolver::PetscSolver(KSPType meth, PCType pre, double relTol, double absTol, double divTol, int maxIterations, MatType mat)
    : LinearSOESolver(SOLVER_TAGS_PetscSolver), method(meth), preconditioner(pre),
      rTol(relTol), aTol(absTol), dTol(divTol), maxIts(maxIterations), matType(mat),
      is_KSP_initialized(false)
{

}

PetscSolver::~PetscSolver()
{
    // KSPDestroy(&ksp);
    // CHKERRQ(ierr);
}


int
PetscSolver::solve(void)
{

    int size = theSOE->size;
    // int numProcesses = theSOE->numProcesses;
    int processID_world = theSOE->processID_world;
    int ierr;

    if (processID_world > 0)
    {

        //MatSetType(theSOE->A, matType);
        // cout << "PetscSolver::solve (" << processID_world << ") MatAssemblyBegin\n";
        ierr = MatAssemblyBegin(theSOE->A, MAT_FINAL_ASSEMBLY);
        CHKERRQ(ierr);

        ierr = MatAssemblyEnd(theSOE->A, MAT_FINAL_ASSEMBLY);
        CHKERRQ(ierr);
        // cout << "PetscSolver::solve (" << processID_world << ") MatAssemblyEnd\n";


        if (not is_KSP_initialized)
        {

            KSPCreate(PETSC_COMM_WORLD, &ksp);
            KSPSetFromOptions(ksp);
            KSPSetOperators(ksp, theSOE->A, theSOE->A);

            // KSPSetType(ksp, KSPPREONLY);
            // Mat       &F = theSOE->A;
            // KSPGetPC(ksp, &pc);
            // PCSetType(pc, PCLU);

            // PCFactorSetMatSolverPackage(pc, MATSOLVERSUPERLU_DIST);
            // PCFactorSetUpMatSolverPackage(pc); /* call MatGetFactor() to create F */
            // PCFactorGetMatrix(pc, &F);
            // MatSuperluSetILUDropTol(F, 1.e-1);



            is_KSP_initialized = true;
        }

    }



// Everyone sends the assebled vector B to the rank 0 processor. Which sends it back...
    static Vector receiveVector(1);


    Vector *vectX = theSOE->vectX;
    Vector *vectB = theSOE->vectB;

    // zero X
    vectX->Zero();

    //
    // form B on each
    //

    int numChannels = theSOE->numChannels;
    Channel **theChannels = theSOE->theChannels;

    if (processID_world != 0)
    {
        Channel *theChannel = theChannels[0];

        theChannel->sendVector(0, 0, *vectB);
        theChannel->receiveVector(0, 0, *vectB);

        cout << "norm(vectB) = " << vectB->norm() << endl;
    }
    else //Master process assembles b vector. This is a bottleneck and can be improved with
        //a collective reduction
    {

        if (receiveVector.Size() != size)
        {
            receiveVector.resize(size);
        }

        //Better done with ALLREDUCE
        for (int j = 0; j < numChannels; j++)
        {
            Channel *theChannel = theChannels[j];
            theChannel->receiveVector(0, 0, receiveVector);
            *vectB += receiveVector;
        }

        //Better done with a BCast
        for (int j = 0; j < numChannels; j++)
        {
            Channel *theChannel = theChannels[j];
            theChannel->sendVector(0, 0, *vectB);
        }
    }

    // cout << "Process " << processID_world << " norm(vectB) = " << vectB->Norm() << endl;
    // cout << "Process " << processID_world << " norm(vectX) = " << vectX->Norm() << endl;



    //
    // solve and mark as having been solved
    //



    if (processID_world > 0)
    {
        // cout << "Process " <<  processID_world << " calling KSPSolve()\n";
        ierr = KSPSolve(ksp, theSOE->b, theSOE->x);
        // cout << "        " <<  processID_world << " done calling KSPSolve()\n";
        CHKERRQ(ierr);
        theSOE->isFactored = 1;
    }

    //PETSc Diagnostics
    //----------------------------------------------
    // cout << "PetscSolver::solve()  ---------  Begin PetSC diagnostics\n";
    // KSPView(ksp, PETSC_VIEWER_STDOUT_WORLD);

    // PetscViewer    viewer_A;
    // PetscViewerASCIIOpen(theSOE->petsc_comm, "A.txt", &viewer_A);
    // MatView(theSOE->A, viewer_A);


    // // cout << "PetscSolver::solve() -- PID#    " << processID_world << "     writing right hand side in RHS.txt file ... " << endl;
    // PetscViewer    viewer_RHS;
    // PetscViewerASCIIOpen(theSOE->petsc_comm, "RHS.txt", &viewer_RHS);
    // VecView(theSOE->b, viewer_RHS);



    // // cout << "PetscSolver::solve() -- PID#    " << processID_world << " writing results ... " << endl;
    // PetscViewer    viewer_x;
    // PetscViewerASCIIOpen(theSOE->petsc_comm, "X.txt", &viewer_x);
    // VecView(theSOE->x, viewer_x);
    // cout << "PetscSolver::solve()  ---------  End PetSC diagnostics\n";
    ////////////////////////////////////////



    //
    // if parallel, we must form the total X: each processor has startRow through endRow-1
    //


    // Vector *vectX = theSOE->vectX;
    vectX = theSOE->vectX;

    // int numChannels = theSOE->numChannels;
    numChannels = theSOE->numChannels;
    // Channel **theChannels = theSOE->theChannels;
    theChannels = theSOE->theChannels;

    if (processID_world != 0)
    {
        Channel *theChannel = theChannels[0];

        theChannel->sendVector(0, 0, *vectX);
        theChannel->receiveVector(0, 0, *vectX);

    }
    else //Again, the master process forms the global X vector which is then sent to all processors
    {

        if (receiveVector.Size() != size)
        {
            receiveVector.resize(size);
        }

        //Better done with ALLREDUCE
        for (int j = 0; j < numChannels; j++)
        {
            Channel *theChannel = theChannels[j];
            theChannel->receiveVector(0, 0, receiveVector);
            *vectX += receiveVector;
        }

        //Better done with a BCast
        for (int j = 0; j < numChannels; j++)
        {
            Channel *theChannel = theChannels[j];
            theChannel->sendVector(0, 0, *vectX);
        }

    }

    //Destroy KSP and collect the error at P0
    if (processID_world > 0)
    {
        // ierr = KSPDestroy(&ksp);
        // CHKERRQ(ierr);

        //The first rank in petsc_ranks (rank 0 in petsc_comm and petsc_group) send the exit error to
        // the 0 processor
        if ((theSOE->petsc_ranks)[0] == processID_world)
        {
            MPI_Send(&ierr, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Status stat;
        MPI_Recv(&ierr, 1, MPI_INT, (theSOE->petsc_ranks)[0], 0, MPI_COMM_WORLD, &stat);
    }

    // int exitflag;
    // MPI_Reduce(&ierr, &exitflag, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    // cout << " > Process " << processID_world << " norm(vectB) = " << vectB->Norm() << endl;
    // cout << " > Process " << processID_world << " norm(vectX) = " << vectX->Norm() << endl;

    // cout << " PetscSolver.solve(), exitflag = " << ierr << endl;

    if (processID_world > 0)
    {
        PetscViewer    viewer;
        PetscViewerASCIIOpen(PETSC_COMM_WORLD, "petsc_log.txt" , &viewer);
        // PetscViewerASCIIGetStdout(PETSC_COMM_WORLD, &viewer);
        PetscLogView(viewer);
    }

    return ierr;
}





int
PetscSolver::setSize()
{
    /*
     * Create linear solver context
     */
    if (theSOE->processID_world > 0)
    {
        KSPCreate(PETSC_COMM_WORLD, &ksp);
    }


    return 0;
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
    // cout << "PetscSolver::sendSelf()\n";
    // static ID idData(4);
    // idData(0) = maxIts;
    // //if (method == KSPCG)
    // //  idData(1) = 0;
    // //else if (method == KSPBICG)
    // //  idData(1) = 1;
    // //else if (method == KSPRICHARDSON)
    // //  idData(1) = 2;
    // //else if (method == KSPCHEBYCHEV)
    // //  idData(1) = 3;
    // //else if (method == KSPGMRES)
    // //  idData(1) = 4;
    // //else if (method == KSPPREONLY)
    // //  idData(1) = 5;
    // //else {
    // //  cerr << "PetscSolver::sendSelf() - unknown method set\n";
    // //  return -1;
    // //}
    // //
    // //if (preconditioner == PCJACOBI)
    // //  idData(2) = 0;
    // //else if (preconditioner == PCILU)
    // //  idData(2) = 1;
    // //else if (preconditioner == PCICC)
    // //  idData(2) = 2;
    // //else if (preconditioner == PCBJACOBI)
    // //  idData(2) = 3;
    // //else if (preconditioner == PCNONE)
    // //  idData(2) = 4;
    // //else if (preconditioner == PCLU)
    // //  idData(2) = 5;
    // //else if (preconditioner == PCCHOLESKY)
    // //  idData(2) = 6;
    // //else {
    // //  cerr << "PetscSolver::sendSelf() - unknown preconditioner set\n";
    // //  return -1;
    // //}

    // if (strcmp(method, "cg") == 0)
    // {
    //     idData(1) = 0;
    // }
    // else if (strcmp(method, "bicg") == 0)
    // {
    //     idData(1) = 1;
    // }
    // else if (strcmp(method, "richardson") == 0)
    // {
    //     idData(1) = 2;
    // }
    // else if (strcmp(method, "chebychev") == 0)
    // {
    //     idData(1) = 3;
    // }
    // else if (strcmp(method, "gmres") == 0)
    // {
    //     idData(1) = 4;
    // }
    // else if (strcmp(method, "preonly") == 0)
    // {
    //     idData(1) = 5;
    // }
    // else
    // {
    //     cerr << "PetscSolver::sendSelf() - unknown method set\n";
    //     return -1;
    // }

    // if (strcmp(preconditioner, "jacobi") == 0)
    // {
    //     idData(2) = 0;
    // }
    // else if (strcmp(preconditioner, "ilu") == 0)
    // {
    //     idData(2) = 1;
    // }
    // else if (strcmp(preconditioner, "icc") == 0)
    // {
    //     idData(2) = 2;
    // }
    // else if (strcmp(preconditioner, "bjacobi") == 0)
    // {
    //     idData(2) = 3;
    // }
    // else if (strcmp(preconditioner, "none") == 0)
    // {
    //     idData(2) = 4;
    // }
    // else if (strcmp(preconditioner, "lu") == 0)
    // {
    //     idData(2) = 5;
    // }
    // else if (strcmp(preconditioner, "cholesky") == 0)
    // {
    //     idData(2) = 6;
    // }
    // else
    // {
    //     cerr << "PetscSolver::sendSelf() - unknown preconditioner set\n";
    //     return -1;
    // }

    // //if (matType == MATSUPERLU_DIST)
    // //  idData(3) = 0;
    // //else if (matType == MATMPISBAIJSPOOLES)
    // //  idData(3) = 1;
    // //else if (matType == MATMPIROWBS)
    // //  idData(3) = 2;
    // if (strcmp(matType, "superlu_dist") == 0)
    // {
    //     idData(3) = 0;
    // }
    // else if (strcmp(matType, "mpisbaijspooles") == 0)
    // {
    //     idData(3) = 1;
    // }
    // else if (strcmp(matType, "mpirowbs") == 0)
    // {
    //     idData(3) = 2;
    // }
    // else if (strcmp(matType, "mpiaijspooles") == 0)
    // {
    //     idData(3) = 3;
    // }
    // else if (strcmp(matType, "aijmumps") == 0)
    // {
    //     idData(3) = 4;
    // }
    // else
    // {
    //     idData(3) = 99;
    //     //  cerr << "PetscSolver::sendSelf() - unknown matType set\n";
    //     //  return -1;
    // }



    // theChannel.sendID(0, cTag, idData);

    // static Vector data(3);
    // data(0) = rTol;
    // data(1) = aTol;
    // data(2) = dTol;

    // theChannel.sendVector(0, cTag, data);
    return 0;
}

int
PetscSolver::receiveSelf(int cTag, Channel &theChannel,
                         FEM_ObjectBroker &theBroker)
{
    // cout << "PetscSolver::receiveSelf() \n";
    // static ID idData(4);
    // theChannel.receiveID(0, cTag, idData);
    // maxIts = idData(0);

    // if (idData(1) == 0)
    // {
    //     method = KSPCG;
    // }
    // else if (idData(1) == 1)
    // {
    //     method = KSPBICG;
    // }
    // else if (idData(1) == 2)
    // {
    //     method = KSPRICHARDSON;
    // }
    // else if (idData(1) == 3)
    // {
    //     method = KSPCHEBYSHEV;
    // }
    // else if (idData(1) == 4)
    // {
    //     method = KSPGMRES;
    // }
    // else if (idData(1) == 5)
    // {
    //     method = KSPPREONLY;
    // }
    // else
    // {
    //     cerr << "PetscSolver::receiveSelf() - unknown method recvd\n";
    //     return -1;
    // }

    // if (idData(2) == 0)
    // {
    //     preconditioner = PCJACOBI;
    // }
    // else if (idData(2) == 1)
    // {
    //     preconditioner = PCILU;
    // }
    // else if (idData(2) == 2)
    // {
    //     preconditioner = PCICC;
    // }
    // else if (idData(2) == 3)
    // {
    //     preconditioner = PCBJACOBI;
    // }
    // else if (idData(2) == 4)
    // {
    //     preconditioner = PCNONE;
    // }
    // else if (idData(2) == 5)
    // {
    //     preconditioner = PCLU;
    // }
    // else if (idData(2) == 6)
    // {
    //     preconditioner = PCCHOLESKY;
    // }
    // else
    // {
    //     cerr << "PetscSolver::receiveSelf() - unknown preconditioner set\n";
    //     return -1;
    // }

    // if (idData(3) == 0)
    //     //matType = MATSUPERLU_DIST; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    // {
    //     matType = MATSOLVERSUPERLU_DIST;
    // }
    // else if (idData(3) == 1)
    //     //matType = MATMPISBAIJSPOOLES; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    // {
    //     matType = MATSOLVERSUPERLU_DIST;//MATSOLVERSPOOLES;
    // }
    // //else if (idData(3) == 2)
    // //matType = MATMPIROWBS;  Ignored because this interface has been removed fro petsc 3.1   by B. Kamrani (Aug. 2010)
    // else if (idData(3) == 3)
    //     //matType = MATMPIAIJSPOOLES; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    // {
    //     matType = MATSOLVERSUPERLU_DIST;//MATSOLVERSPOOLES;
    // }
    // else if (idData(3) == 4)
    //     //matType = MATAIJMUMPS; Changed because this interface has been changed in  petsc 3.1   by B. Kamrani (Aug. 2010)
    // {
    //     matType = MATSOLVERMUMPS;
    // }
    // else
    // {
    //     matType = MATMPIAIJ;
    //     //  cerr << "PetscSolver::receiveSelf() - unknown matType set\n";
    //     //  return -1;
    // }

    // static Vector data(3);
    // theChannel.receiveVector(0, cTag, data);
    // rTol = data(0);
    // aTol = data(1);
    // dTol = data(2);

    // theSOE->mType = matType;

    return 0;
}



