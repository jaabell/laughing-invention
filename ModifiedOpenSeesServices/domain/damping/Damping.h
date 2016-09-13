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
//
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
