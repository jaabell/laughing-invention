///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Rayleigh.h
// CLASS:             Rayleigh
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Rayleigh Damping to Domain
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              Oct. 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

#ifndef Rayleigh_h
#define Rayleigh_h

#include <iostream>
#include <Damping.h>
#include <string>
using namespace std;


class Rayleigh : public Damping
{
    public:
        Rayleigh(int tag, double a0, double a1, string stiffness_type);

        double return_a0();
        double return_a1();
        virtual double return_a2();
        virtual double return_a3();
        string return_dampingtype()
        {
            return "Rayleigh";
        };
        string return_stiffnesstype();
        int return_tag();


        int sendSelf (int commitTag, Channel& theChannel);
        int receiveSelf (int commitTag, Channel& theChannel, FEM_ObjectBroker
                      &theBroker);
        void Print(ostream& s, int flag);

    protected:

    private:
        double a0;
        double a1;
        int damptag;
        string damping_type;
        string stiffness_type;
};

#endif


