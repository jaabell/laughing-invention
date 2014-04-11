///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
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
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              June 2013

//
///////////////////////////////////////////////////////////////////////////////


int query_domain_number_of_nodes()
{
    return theDomain.getNumNodes();
}


double query_domain_current_time()
{
    return theDomain.getCurrentTime();
}


int query_domain_number_of_elements()
{
    return theDomain.getNumElements();
}

int query_domain_number_of_sp_constraints()
{
    return theDomain.getNumSPs();
}

int query_domain_number_of_mp_constraints()
{
    return theDomain.getNumMPs();
}

int query_domain_number_of_loads()
{
    return theDomain.getNumLoadPatterns();
}



// virtual const Vector &getPhysicalBounds(void);