///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic, Jose Abell
// DATE:               July 2012
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


#include <Vector.h>


Vector calculate_cross_product(const Vector& a, const Vector& b)
{
    Vector c(3); // Store the result here

    if ( (a.Size() != 3) || (b.Size() != 3) )
    {
        cerr << "Error: calculate_cross_product only defined for 3x1 vectors.\n";
        exit(-1);
    }

    c(0) =   a(1) * b(2) - b(1) * a(2);
    c(1) = - a(0) * b(2) + b(0) * a(2);
    c(2) =   a(0) * b(1) - b(0) * a(1);

    return c;
}
