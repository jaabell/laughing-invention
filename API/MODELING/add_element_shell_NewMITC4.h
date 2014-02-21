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
// PROGRAMMER:        Jose Abell, Nima Tafazzoli, Boris Jeremic
//
// DATE:              July 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Shell NewMITC4 Element </h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of the NewMITC4 shell element.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




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

    if(elastic_material == 0)
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



