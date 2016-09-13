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



int add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic(int MaterialNumber,
        double Ehp,
        double Evp,
        double nuhvp,
        double nuhhp,
        double Ghvp,
        double rhop)
{

    NDMaterial* theMaterial = 0;
    theMaterial = new ElasticCrossAnisotropic(MaterialNumber, Ehp, Evp, nuhvp, nuhhp, Ghvp, rhop);;

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_crossanisotropic) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};




