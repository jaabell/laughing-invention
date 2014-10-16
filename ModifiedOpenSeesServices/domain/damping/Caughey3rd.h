///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Caughey3rd.h
// CLASS:             Caughey3rd
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Caughey Damping (3rd order) to Domain
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              Nov. 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

#ifndef Caughey3rd_h
#define Caughey3rd_h

#include <iostream>
#include <Damping.h>
#include <string>
using namespace std;


class Caughey3rd : public Damping
{
    public:
        Caughey3rd(int tag, double a0, double a1, double a2, string stiffness_type);

        double return_a0();
        double return_a1();
        double return_a2();
        double return_a3();
        string return_dampingtype()
        {
            return "Caughey3rd";
        };
        string return_stiffnesstype();
        int return_tag();


        int sendSelf (int commitTag, Channel& theChannel);
        int receiveSelf (int commitTag, Channel& theChannel, FEM_ObjectBroker
                      &theBroker);
        void Print(ostream& s, int flag);

    protected:

    private:
        int damptag;
        double a0;
        double a1;
        double a2;
        double a3;
        string damping_type;
        string stiffness_type;
};

#endif


