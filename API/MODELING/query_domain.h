///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
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

int query_domain_is_parallel()
{
#ifdef  _PARALLEL_PROCESSING
	return true;
#else
	return false;
#endif
}


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