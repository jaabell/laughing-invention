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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Jose Abell, Boris Jeremic
//
// DATE:              June 2013
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



int output_of_elements_and_nodes()
{


    double dT_recorder = 0;
    int startFlag = 0;
    bool echoTime = true;

    //***************************************************************************************************************
    // Nima Tafazzoli added April-May 2012
    // Having the node and element recorders by default at the beginning of each stage
    // Nodes: recording generalized displacements of all nodes to a file called Nodes_Generalized_Displacements_Stage_....out
    // Elements: recording stresses and forces of all elements to a files called Elements_Stresses_Stage_....out and
    //           Elements_Forces_Stage_....out respectively

    //*********************** Nodes ************************


    const char* dataToStore_node = "disp";

    string filenamestring_node = ModelName + "_" + StageName + "_" + "Nodes_Generalized_Displacements" + ".feioutput";

    const char* filename_node = filenamestring_node.c_str();

    int numberofdofs = 7;
    ID* dofs = 0;
    dofs = new ID( 0, numberofdofs );

    for ( int i = 0; i < numberofdofs; i++ )
    {
        ( *dofs )[i] = i;
    }

    ID* theNodes = 0;
    theNodes = new ID( 0, 16 );

    NodeIter& theNodeIter = theDomain.getNodes();
    Node* theRecordedNode;
    int loc = 0;

    while ( ( theRecordedNode = theNodeIter() ) != 0 )
    {
        int tag = theRecordedNode->getTag();
        ( *theNodes )[loc++] = tag;
    }

    echoMode theEMode = NONE;
    openMode theOMode = APPEND;

    theOutputHandler_node_displacement = new DataOutputFileHandler( filename_node, theEMode, theOMode );

    if ( theOutputHandler_node_displacement == NULL )
    {
        cerr << "Error: (output_of_all_nodes_to_file) memory allocation problem for theOutputHandler_node_displacement!" << endl;
        return -1;
    }

    Recorder* thenodeRecorder = 0;
    thenodeRecorder = new NodeRecorder( *dofs, *theNodes, dataToStore_node, theDomain, *theOutputHandler_node_displacement, dT_recorder, startFlag );

    if ( thenodeRecorder == NULL )
    {
        cerr << "Error: (output_of_all_nodes_to_file) memory allocation problem for thenodeRecorder!" << endl;
        return -1;
    }


    if( theDomain.addRecorder( *thenodeRecorder ) != 0 )
    {
        cerr << "Error: (output_of_all_nodes_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }


    //*********************** Elements Stresses ************************

    int NB = 1 ;
    ID* eleIDsAll = 0;
    eleIDsAll = new ID( 1, 2 );

    loc = 0;
    ElementIter& theEleIterRecorder = theDomain.getElements();
    Element* theEleRecorder;

    while ( ( theEleRecorder = theEleIterRecorder() ) != 0 )
    {
        ( *eleIDsAll )[loc++] = theEleRecorder->getTag();
    }



    const char* dataToStore_element_stress = "stress";

    string filenamestring_element_stresses = ModelName + "_" + StageName + "_" + "Elements_Stresses" + ".feioutput";

    const char* filename_element_stresses = filenamestring_element_stresses.c_str();


    theOutputHandler_element_stress = new DataOutputFileHandler( filename_element_stresses, theEMode, theOMode );

    if ( theOutputHandler_element_stress == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theOutputHandler_element_stress!" << endl;
        return -1;
    }

    Recorder* theelementrecorder_stress = 0;
    theelementrecorder_stress = new ElementRecorder( *eleIDsAll, &dataToStore_element_stress, NB, echoTime, theDomain, *theOutputHandler_element_stress, dT_recorder );

    if ( theelementrecorder_stress == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theelementrecorder_stress!" << endl;
        return -1;
    }

    if( theDomain.addRecorder( *theelementrecorder_stress ) != 0 )
    {
        cerr << "Error: (output_of_all_elements_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }



    //*********************** Elements Strains ************************


    const char* dataToStore_element_strain = "strain";

    string filenamestring_element_strains = ModelName + "_" + StageName + "_" + "Elements_Strains" + ".feioutput";

    const char* filename_element_strains = filenamestring_element_strains.c_str();


    theOutputHandler_element_strain = new DataOutputFileHandler( filename_element_strains, theEMode, theOMode );

    if ( theOutputHandler_element_strain == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theOutputHandler_element_strain!" << endl;
        return -1;
    }

    Recorder* theelementrecorder_strain = 0;
    theelementrecorder_strain = new ElementRecorder( *eleIDsAll, &dataToStore_element_strain, NB, echoTime, theDomain, *theOutputHandler_element_strain, dT_recorder );

    if ( theelementrecorder_strain == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theelementrecorder_strain!" << endl;
        return -1;
    }

    if( theDomain.addRecorder( *theelementrecorder_strain ) != 0 )
    {
        cerr << "Error: (output_of_all_elements_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }


    //*********************** Elements Forces ************************




    const char* dataToStore_element_force  = "force";

    string filenamestring_element_forces = ModelName + "_" + StageName + "_" + "Elements_Forces" + ".feioutput";

    const char* filename_element_forces = filenamestring_element_forces.c_str();


    theOutputHandler_element_force = new DataOutputFileHandler( filename_element_forces, theEMode, theOMode );

    if ( theOutputHandler_element_force == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theOutputHandler_element_force!" << endl;
        return -1;
    }

    Recorder* theelementrecorder_force = 0;
    theelementrecorder_force = new ElementRecorder( *eleIDsAll, &dataToStore_element_force, NB, echoTime, theDomain, *theOutputHandler_element_force, dT_recorder );

    if ( theelementrecorder_force == NULL )
    {
        cerr << "Error: (output_of_all_elements_to_file) memory allocation problem for theelementrecorder_force!" << endl;
        return -1;
    }

    if( theDomain.addRecorder( *theelementrecorder_force ) != 0 )
    {
        cerr << "Error: (output_of_all_elements_to_file) Recorder could not be added to the domain " << endl;
        return -1;
    }

    //     theOutputHandler_element_force = 0;
    //     delete theOutputHandler_element_force;





    if ( eleIDsAll != 0 )
    {
        delete eleIDsAll;
    }

    if ( theNodes != 0 )
    {
        delete theNodes;
    }


    if ( dofs != 0 )
    {
        delete dofs;
    }



    return 0;

};















