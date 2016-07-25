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

//! Inputs:





int add_element_shell_NewMITC4(int ElementNumber,
                               int node_numb_1,
                               int node_numb_2,
                               int node_numb_3,
                               int node_numb_4,
                               double thickness,
                               int MaterialNumber)
{
    Element*                 theElement = 0;
    SectionForceDeformation* theSection = 0;
    NDMaterial*            material_ref = 0;
    int                   SectionNumber = ElementNumber;


    // Check the existence of the material and get a pointer
    material_ref = theDomain.getNDMaterial(MaterialNumber);

    if (material_ref == 0)
    {
        cerr << "WARNING: (add_section_membrane_plate_fiber_to_shell) material not found!\n";
        cerr << "add_section_membrane_plate_fiber_to_shell material: " << MaterialNumber << endln;
        return -1;
    }

    ElasticIsotropicMaterial* elastic_material;
    elastic_material = dynamic_cast<ElasticIsotropicMaterial*>(material_ref);

    if (elastic_material == 0)
    {
        cerr << "WARNING: (add_element_shell_NewMITC4) material is not Elastic Isotropic 3D!\n";
        cerr << "add_element_shell_NewMITC4 material: " << MaterialNumber << endln;
        return -1;
    }

    double E = elastic_material->getE();
    double nu = elastic_material->getnu();
    double rho_ = elastic_material->getRho();

    //Create the section for this element
    // theSection = new MembranePlateFiberSection(SectionNumber, thickness, *material_ref);
    theSection = new ElasticMembranePlateSection(SectionNumber, E, nu , thickness, rho_);


    if (theDomain.addSection(*theSection) < 0)
    {
        cerr << "WARNING could not add section to the domain\n";
        cerr << "Section number: " << SectionNumber << endln;
        delete theSection; // invoke the material objects destructor, otherwise mem leak
        return -1;
    }


    //Add the element
    theElement = new NewShellMITC4(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, *theSection);

    if (theElement == 0)
    {
        cerr << "WARNING: (add_element_shell_NewMITC4) ran out of memory creating element\n";
        cerr << "add_element_shell_NewMITC4 element: " << ElementNumber << endln;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_shell_NewMITC4) could not add element to the domain\n";
        cerr << "add_element_shell_NewMITC4 element: " << ElementNumber << endln;
        return -1;
        //      delete theElement;
    }

    return 0;

};



