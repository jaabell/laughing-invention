///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Damping.h
// CLASS:             Damping
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Damping to Domain
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


#ifndef Damping_h
#define Damping_h


#include <ID.h>
#include <DomainComponent.h>

#include <iostream>
#include <string>
using namespace std;


class Damping : public DomainComponent
{
    public:
        Damping(int tag, int classTag);

        virtual ~Damping();

        virtual double return_a0();
        virtual double return_a1();
        virtual double return_a2();
        virtual double return_a3();
        virtual string return_dampingtype();
        virtual string return_stiffnesstype();
        virtual int return_tag();

        // pure virtual functions
        //    virtual void applyDamping(double loadfactor) =0;

        //    virtual void setDampingTag(int loadPaternTag);
        //    virtual int  getDampingTag(void) const;

    protected:

    private:
        int dampingTag;
        double a0;
        double a1;
        double a2;
        double a3;
};

#endif
