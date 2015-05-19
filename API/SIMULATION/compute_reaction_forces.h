///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): LGPL
//                 and
//                 Woody's license (adopted by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Jose Abell Boris Jeremic
//
// DATE:              Mon 18 May 2015 05:09:28 PM PDT
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int compute_reaction_forces(void)
{
    int errorcode = theDomain.calculateNodalReactions(0);

    if (errorcode == 0)
        cout << "Reaction forces computed successfully.\n";
    else
        cout << "Error computing reaction forces. (code = "  << errorcode << ")\n";

    return errorcode;
};




