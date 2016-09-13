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


#ifndef add_element_shell_andes_4node_H_INCLUDED
#define add_element_shell_andes_4node_H_INCLUDED


int add_element_shell_andes_4node(int ElementNumber,
                                  int node_numb_1,
                                  int node_numb_2,
                                  int node_numb_3,
                                  int node_numb_4,
                                  double thickness,
                                  int MaterialNumber)
{
  NDMaterial* ndmaterial = 0;
  ndmaterial = theDomain.getNDMaterial(MaterialNumber);

  if (ndmaterial == NULL)
  {
    cerr << "Error: (add_element_shell_andes_4node) memory allocation problem for ndmaterial!" << endl;
    return -1;
  }


  Element* theElement = 0;
  theElement = new FourNodeAndesShell(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, thickness, ndmaterial);

  if (theElement == NULL)
  {
    cerr << "Error: (add_element_shell_andes_4node) memory allocation problem for theElement!" << endl;
    return -1;
  }

  if (theDomain.addElement(theElement) == false)
  {
    cerr << "WARNING: (add_element_shell_andes_4node) could not add element to the domain\n";
    cerr << "Element number: " << ElementNumber << endln;
    return -1;
  }

  return 0;

};



#endif // add_element_shell_andes_4node_H_INCLUDED
