///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               Oct 2010
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//`
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////


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

    int damptag;
    double a0;
    double a1;
    string damping_type;
    string stiffness_type;
};

#endif


