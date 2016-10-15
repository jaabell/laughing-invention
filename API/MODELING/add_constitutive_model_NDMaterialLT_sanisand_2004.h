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



int add_constitutive_model_NDMaterialLT_sanisand_2004(int MaterialNumber, double rho_, double initial_confinement_p0,
        double G0_in, double patm_in, double nu_in,
        double h0_in, double ch_in,
        double c_in, double Mc_in, double ec0_in, double lambda_c_in,
        double xi_in,
        double nb_in,
        double A0_in, double cz_in, double zmax_in, double nd_in,
        double m_in, double e_in)
{

    NDMaterialLT* theMaterial = 0;
    double Me_in = c_in * Mc_in;
    theMaterial = new     sanisand2004( MaterialNumber,  rho_,  initial_confinement_p0,
                                        G0_in,  patm_in,  nu_in,
                                        h0_in,  ch_in,
                                        Me_in,  Mc_in,  ec0_in,  lambda_c_in,
                                        xi_in,
                                        nb_in,
                                        A0_in,  cz_in,  zmax_in,  nd_in,
                                        m_in,  e_in) ;

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_sanisand_2004) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterialLT(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterialLT_sanisand_2004) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};






