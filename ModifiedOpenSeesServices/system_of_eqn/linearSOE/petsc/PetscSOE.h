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
// $Date: 2005/05/18 19:24:49 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/petsc/PetscSOE.h,v $


// Written: fmk & om
// Created: 7/98
// Revision: A
//
// Description: This file contains the class definition for PetscSOE
// PetscSOE is a subclass of LinearSOE. It uses the LAPACK storage
// scheme to store the components of the A matrix, which is a full matrix.


// What: "@(#) PetscSOE.h, revA"

#ifndef PetscSOE_h
#define PetscSOE_h

#include <LinearSOE.h>
#include <Vector.h>

//
#include <petscksp.h>
// }

class PetscSolver;

class PetscSOE : public LinearSOE
{
    public:
        PetscSOE(PetscSolver &theSolver, MatType matType = MATMPIAIJ, int blockSize = 1);

        ~PetscSOE();

        int getNumEqn(void) const;
        //     int setSize(Graph &theGraph); //Out by Babak Babak 06/04/13
        int setSize(int MaxDOFtag); //Added by Babak 06/04/13
        int setEigenSize(Graph &theGraph); //Added by Babak to add mass Matrices ... 1/15/13


        int addA(const Matrix &, const ID &, double fact = 1.0);
        int addB(const Vector &, const ID &, double fact = 1.0);
        int addM(const Matrix &, const ID &, double fact = 1.0); //Added by Babak to add mass Matrices ... 1/14/13
        int addK(const Matrix &, const ID &, double fact = 1.0); //Added by Babak to add stiffness Matrices ... 1/15/13

        int setB(const Vector &, double fact = 1.0);

        void zeroA(void);
        void zeroB(void);
        void zeroM(void);//Added by Babak to add mass Matrices ... 1/14/13
        void zeroK(void);//Added by Babak to add stiffness Matrices ... 1/15/13

        const Vector &getX(void);
        const Vector &getB(void);
        double normRHS(void);

        void setX(int loc, double value);
        void setX(const Vector &x);

        int setSolver(PetscSolver &newSolver);

        int sendSelf(int commitTag, Channel &theChannel);
        int recvSelf(int commitTag, Channel &theChannel,
                     FEM_ObjectBroker &theBroker);

        //Addec by Babak Kamrani for dumping B vector (RHS) in a file on 03/30/2012:
        void DumpB(void );
        void DumpVectB(void);

        friend class PetscSolver;
        friend class ActorPetscSOE;
        friend class ShadowPetscSOE;

    protected:
        int setChannels(int nChannels, Channel **theChannels);

    private:
        int isFactored;
        int size;
        int processID;
        int numProcesses;

        bool init_done;//GZ_added

        double *B, *X;
        int *indices;
        Vector *vectX;
        Vector *vectB;
        Mat A;
        Mat M;//Added by Babak to add mass Matrices ... 1/14/13
        Mat K;//Added by Babak to add stiffness Matrices ... 1/15/13
        Vec x, b;
        int blockSize;
        //     PetscBool flg;

        int numChannels;
        Channel **theChannels;
        ID **localCol;

        MatType mType;//Guanzhou

        int startRow, endRow;
};


#endif

