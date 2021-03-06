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

// $Revision: 1.7 $
// $Date: 2003/03/06 20:32:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/integrator/Newmark.h,v $


#ifndef Newmark_h
#define Newmark_h

// File: ~/analysis/integrator/Newmark.h
//
// Written: fmk
// Created: 11/98
// Revision: A
//
// Description: This file contains the class definition for Newmark.
// Newmark is an algorithmic class for performing a transient analysis
// using the Newmark integration scheme.
//
// What: "@(#) Newmark.h, revA"

#include <TransientIntegrator.h>

class DOF_Group;
class FE_Element;
class Vector;

class Newmark : public TransientIntegrator
{
    public:
        Newmark();
        Newmark(double gamma, double beta, bool disp = true);

        // Nima Tafazzoli removed (March 2010)
        //    Newmark(double gamma, double beta, double alphaM, double betaK,
        //      double betaKi, double betaKc, bool disp = true);
        ~Newmark();


        // methods which define what the FE_Element and DOF_Groups add
        // to the system of equation object.
        int formEleTangent(FE_Element* theEle);
        int formNodTangent(DOF_Group* theDof);

        int domainChanged(void);
        int initialize(void);
        int newStep(double deltaT);
        int revertToLastStep(void);
        int update(const Vector& deltaU);

        virtual int sendSelf(int commitTag, Channel& theChannel);
        virtual int receiveSelf(int commitTag, Channel& theChannel,
                             FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);



    protected:

        bool displ;      // a flag indicating whether displ or accel increments
        double gamma;
        double beta;

        // rayleigh factors
        //    double alphaM, betaK, betaKi, betaKc;

        double c1, c2, c3;  // some constants we need to keep
        Vector* Ut, *Utdot, *Utdotdot; // response quantities at time t
        Vector* U, *Udot, *Udotdot; // response quantities at time t+deltat
        bool determiningMass; // flag to check if just want the mass contribution

    private:

};

#endif



