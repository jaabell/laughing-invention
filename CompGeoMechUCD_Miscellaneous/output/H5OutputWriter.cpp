///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              H5OutputWriter.cpp
// CLASS:             H5OutputWriter
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Is an OutputWriter that writes into HDF5 format
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Jose Abell, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              Mon 05 May 2014 10:09:35 AM PDT
// UPDATE HISTORY:  Some changes made by Babak 5/31/14 to make it work with parallel ...
//
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <ID.h>
#include <Vector.h>
#include <H5OutputWriter.h>
#include <hdf5.h>
#include <time.h>

//Added By Babak 5/31/14
//------------------------
#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif
//------------------------


H5OutputWriter::H5OutputWriter():
    OutputWriter(),
    file_is_open(false),
    number_of_nodes(0),
    number_of_elements(0),
    number_of_time_steps(0),
    max_node_tag(0),
    max_element_tag(0),
    file_name(""),
    model_name(""),
    stage_name("")
{
    current_time                         = 0.0;
    current_time_step                    = 0;
    number_of_nodes                      = 0;
    number_of_elements                   = 0;
    max_node_tag                         = 0;
    max_element_tag                      = 0;

    length_nodes_displacements_output    = 0;
    length_nodes_velocities_output       = 0;
    length_nodes_accelerations_output    = 0;
    length_nodes_reaction_forcess_output = 0;
    length_element_output                = 0;
    pos_nodes_outputs                    = 0;
    pos_nodes_coordinates                = 0;
    pos_elements_outputs                 = 0;
    pos_elements_gausscoords             = 0;
    pos_elements_connectivity            = 0;

    create_nodeMeshData_arrays           = true;
    create_nodeDisplacements_arrays      = true;
    create_nodeVelocities_arrays         = true;
    create_nodeAccelerations_arrays      = true;
    create_nodeReactionForces_arrays     = true;
    create_elementMeshData_arrays        = true;
    create_elementOutput_arrays          = true;
}


H5OutputWriter::H5OutputWriter(std::string filename_in,
                               std::string model_name_in,
                               std::string stage_name_in,
                               int nsteps):
    OutputWriter(),
    file_is_open(false),
    number_of_nodes(0),
    number_of_elements(0),
    number_of_time_steps(0),
    max_node_tag(0),
    max_element_tag(0),
    file_name(""),
    model_name(""),
    stage_name("")
{
    current_time                         = 0.0;
    current_time_step                    = 0;
    number_of_nodes                      = 0;
    number_of_elements                   = 0;
    max_node_tag                         = 0;
    max_element_tag                      = 0;

    length_nodes_displacements_output    = 0;
    length_nodes_velocities_output       = 0;
    length_nodes_accelerations_output    = 0;
    length_nodes_reaction_forcess_output = 0;
    length_element_output                = 0;
    pos_nodes_outputs                    = 0;
    pos_nodes_coordinates                = 0;
    pos_elements_outputs                 = 0;
    pos_elements_gausscoords             = 0;
    pos_elements_connectivity            = 0;

    current_time = 0.0;

    create_nodeMeshData_arrays           = true;
    create_nodeDisplacements_arrays      = true;
    create_nodeVelocities_arrays         = true;
    create_nodeAccelerations_arrays      = true;
    create_nodeReactionForces_arrays     = true;
    create_elementMeshData_arrays        = true;
    create_elementOutput_arrays          = true;
    initialize(filename_in,
               model_name_in,
               stage_name_in,
               nsteps);
}



H5OutputWriter::~H5OutputWriter()
{
    finalize();
}


void H5OutputWriter::finalize()
{
    if (file_is_open)
    {
        // cout << "endingtime \n\n";
        time_t current ;
        time(&current);
        char *timestring;
        timestring = ctime(&current);
        timestring[strlen(timestring) - 1] = '\0';
        write_string(id_file, "Date_and_Time_End", timestring);


        //Close file
        file_is_open = false;
        H5close();
    }
}


void H5OutputWriter::initialize(std::string filename_in,
                                std::string model_name_in,
                                std::string stage_name_in,
                                int nsteps)
{


    if (previous_stage_name.compare("!!none") == 0)
    {
        cout << "changing previous_stage_name from " << previous_stage_name << " to " << stage_name
             << endl;
        previous_stage_name = stage_name;
    }
    file_name = "";
    model_name = "";
    stage_name = "";
    file_name += filename_in;
    model_name += model_name_in;
    stage_name += stage_name_in;

    current_time                         = 0.0;
    current_time_step                    = 0;
    number_of_nodes                      = 0;
    number_of_elements                   = 0;
    max_node_tag                         = 0;
    max_element_tag                      = 0;

    length_nodes_displacements_output    = 0;
    length_nodes_velocities_output       = 0;
    length_nodes_accelerations_output    = 0;
    length_nodes_reaction_forcess_output = 0;
    length_element_output                = 0;

    pos_nodes_outputs                    = 0;
    pos_nodes_coordinates                = 0;
    pos_elements_outputs                 = 0;
    pos_elements_gausscoords             = 0;
    pos_elements_connectivity            = 0;

    create_nodeMeshData_arrays           = true;
    create_nodeDisplacements_arrays      = true;
    create_nodeVelocities_arrays         = true;
    create_nodeAccelerations_arrays      = true;
    create_nodeReactionForces_arrays     = true;
    create_elementMeshData_arrays        = true;
    create_elementOutput_arrays          = true;

    number_of_time_steps                 = nsteps;

}














// Mesh output


int H5OutputWriter::writeNumberOfNodes(unsigned int number_of_nodes_in )
{
    number_of_nodes = number_of_nodes_in;


    return 0;
}

int H5OutputWriter::writeNumberOfElements(unsigned int numberOfElements_ )
{
    number_of_elements = numberOfElements_;



    return 0;
}


int H5OutputWriter::writeNodeMeshData(int tag     , const Vector &coords   , int ndofs )
{
    int ntags = Number_of_DOFs.Size();
    int addzeros = tag - ntags;

    //Extend arrays
    for (int i = 0; i < addzeros; i++)
    {
        Number_of_DOFs[ntags + i] = -1;
        Index_to_Generalized_Displacements[ntags + i] = -1;
        Index_to_Coordinates[ntags + i] = -1;
    }


    //Form Number_of_DOFs
    Number_of_DOFs[tag] = ndofs;

    //Form Coordinates
    // int ncoordinates = Coordinates.Size();
    Coordinates.resize(pos_nodes_coordinates + 3);
    Coordinates[pos_nodes_coordinates] = coords(0);
    Coordinates[pos_nodes_coordinates + 1] = coords(1);
    Coordinates[pos_nodes_coordinates + 2] = coords(2);

    //For Index_to_Coordinates
    Index_to_Coordinates[tag] = pos_nodes_coordinates;
    pos_nodes_coordinates += 3;

    Index_to_Generalized_Displacements[tag] = pos_nodes_outputs;
    pos_nodes_outputs += ndofs;

    length_nodes_accelerations_output    = pos_nodes_outputs;
    length_nodes_velocities_output       = pos_nodes_outputs;
    length_nodes_displacements_output    = pos_nodes_outputs;
    length_nodes_reaction_forcess_output = pos_nodes_outputs;

    return 0;
}

int H5OutputWriter::writeElementMeshData(int tag  , std::string type , const ID &connectivity , int materialtag , const Matrix &gausscoordinates,
        int length_of_output, int class_tag)
{
    int nnodes, ntags;
    ntags = Number_of_Nodes.Size();
    int addzeros = tag - ntags;

    //Extend arrays
    for (int i = 0; i <= addzeros; i++)
    {
        Number_of_Nodes[ntags + i]                  = -1;
        Index_to_Connectivity[ntags + i]            = -1;
        Index_to_Outputs[ntags + i]                 = -1;
        Number_of_Gauss_Points[ntags + i]           = -1;
        Index_to_Gauss_Point_Coordinates[ntags + i] = -1;
        Material_tags[ntags + i]                    = -1;
        Element_types.push_back(" not defined ");
        Number_of_Output_Fields[ntags + i]          = -1;
        Class_Tags[ntags + i]                       = -1;
    }

    //Check if the element has already been added!!
    // if (Number_of_Nodes[tag] > 0 || Index_to_Connectivity[tag] >= 0 || Index_to_Outputs[tag] >= 0)
    // {
    //     cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " already defined in HDF5 database! Something is wrong"
    //          << endl;
    // }

    // Writing Number_of_Nodes;
    nnodes =  connectivity.Size();
    Number_of_Nodes[tag] = nnodes;


    if (Number_of_Nodes[tag] > 0 && nnodes != Number_of_Nodes[tag])
    {
        cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " number of nodes changed in database."  << endl;
    }

    if (Index_to_Connectivity[tag] > 0 && pos_elements_connectivity != Index_to_Connectivity[tag])
    {
        cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " index to connectivity changed in database."  << endl;
    }

    if (Index_to_Outputs[tag] > 0 && pos_elements_outputs != Index_to_Outputs[tag])
    {
        cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " index to output changed in database."  << endl;
    }

    // Writing Connectivity;
    // int pos_connect = Connectivity.Size();
    for (int n = 0; n < nnodes; n++)
    {
        Connectivity[pos_elements_connectivity + n] = connectivity(n);
    }

    // Writing Index_to_Connectivity;
    Index_to_Connectivity[tag] = pos_elements_connectivity;
    pos_elements_connectivity +=  nnodes;

    // Writing Number_of_Output_Fields;
    Number_of_Output_Fields[tag] = length_of_output;

    // Writing Class_Tags;
    Class_Tags[tag] = class_tag;

    // Writing Index_to_Outputs;
    Index_to_Outputs[tag] = pos_elements_outputs;
    pos_elements_outputs += length_of_output;

    length_element_output = pos_elements_outputs;

    // Writing Number_of_Gauss_Points;
    int ngauss = gausscoordinates.noRows();
    Number_of_Gauss_Points[tag] = ngauss;

    // Writing Gauss_Point_Coordinates;
    // int pos_gauss = Gauss_Point_Coordinates.Size();
    Gauss_Point_Coordinates.resize(pos_elements_gausscoords + ngauss * 3);
    int i = 0;
    for (int n = 0; n < ngauss; n++)
    {
        Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 0);
        Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 1);
        Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 2);
    }

    // Writing Index_to_Gauss_Point_Coordinates;
    Index_to_Gauss_Point_Coordinates[tag] = pos_elements_gausscoords;
    pos_elements_gausscoords += ngauss * 3;

    // Writing Element_types;
    Element_types.push_back(type);

    // Writing Material_tags;
    Material_tags[tag] = 0;

    return 0;
}

void H5OutputWriter::syncWriters()
{
#ifdef _PARALLEL_PROCESSING
    // This function is VERY important.
    // Output writers are instantiated in every processor (within domain, or childs of domain)
    // and must contain the SAME information about the sizes of the involved arrays, because
    // creation of arrays in HDF5 is a collective call, i.e. all processors participate
    // in it using the same information.
    //
    // We need to send the mesh information to all processes before creating arrays

    int root = 0;
    // int bcast_return_flag;
    int length_send;
    int *int_buffer;
    int processID;
    double *double_buffer;

    // MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);


    //For nodes ====================================================================================
    //ID Number_of_DOFs;
    length_send = Number_of_DOFs.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Number_of_DOFs.resize(length_send);
    }
    int_buffer = Number_of_DOFs.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //Vector Coordinates;
    length_send = Coordinates.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Coordinates.resize(length_send);
    }
    double_buffer = Coordinates.theData;
    MPI_Bcast(double_buffer, length_send, MPI_DOUBLE,   root, MPI_COMM_WORLD);


    //ID Index_to_Coordinates;
    length_send = Index_to_Coordinates.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Index_to_Coordinates.resize(length_send);
    }
    int_buffer = Index_to_Coordinates.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);

    //ID Index_to_Generalized_Displacements;
    length_send = Index_to_Generalized_Displacements.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Index_to_Generalized_Displacements.resize(length_send);
    }
    int_buffer = Index_to_Generalized_Displacements.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);



    //For Elements =================================================================================
    //ID Number_of_Nodes;
    length_send = Number_of_Nodes.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Number_of_Nodes.resize(length_send);
    }
    int_buffer = Number_of_Nodes.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Connectivity;
    length_send = Connectivity.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Connectivity.resize(length_send);
    }
    int_buffer = Connectivity.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Index_to_Connectivity;
    length_send = Index_to_Connectivity.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Index_to_Connectivity.resize(length_send);
    }
    int_buffer = Index_to_Connectivity.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Number_of_Output_Fields;
    length_send = Number_of_Output_Fields.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Number_of_Output_Fields.resize(length_send);
    }
    int_buffer = Number_of_Output_Fields.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Index_to_Outputs;
    length_send = Index_to_Outputs.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Index_to_Outputs.resize(length_send);
    }
    int_buffer = Index_to_Outputs.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Number_of_Gauss_Points;
    length_send = Number_of_Gauss_Points.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Number_of_Gauss_Points.resize(length_send);
    }
    int_buffer = Number_of_Gauss_Points.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //Vector Gauss_Point_Coordinates;
    length_send = Gauss_Point_Coordinates.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Gauss_Point_Coordinates.resize(length_send);
    }
    double_buffer = Gauss_Point_Coordinates.theData;
    MPI_Bcast(double_buffer, length_send, MPI_DOUBLE,   root, MPI_COMM_WORLD);


    //ID Index_to_Gauss_Point_Coordinates;
    length_send = Index_to_Gauss_Point_Coordinates.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Index_to_Gauss_Point_Coordinates.resize(length_send);
    }
    int_buffer = Index_to_Gauss_Point_Coordinates.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //std::vector<std::string> Element_types;
    // length_send = Index_to_Generalized_Displacements.Size();
    // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    // if (processID != 0)
    // {
    //     Index_to_Generalized_Displacements.resize(length_send);
    // }
    // int_buffer = Index_to_Generalized_Displacements.data;
    // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


    //ID Material_tags;
    length_send = Material_tags.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Material_tags.resize(length_send);
    }
    int_buffer = Material_tags.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);

    //ID Class_Tags;
    length_send = Class_Tags.Size();
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID != 0)
    {
        Class_Tags.resize(length_send);
    }
    int_buffer = Class_Tags.data;
    MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);





    // =============================================================================================
    // Send the filenames and model names to initialize H5OutputWriter on other processes
    // =============================================================================================
    char char_buffer[H5OUTPUTWRITER_MAX_STRING_SIZE];//   Preset size for all strings
    string file_name_tmp, model_name_tmp, stage_name_tmp, previous_stage_name_tmp;


    if (model_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
    {
        cerr << "Model name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
    }
    if (file_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
    {
        cerr << "File name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
    }
    if (stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
    {
        cerr << "Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
    }
    if (previous_stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
    {
        cerr << "Previous_Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
    }

    // model_name
    length_send  = model_name.size() + 1;
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID == 0)
    {
        strcpy(char_buffer, model_name.c_str());
        char_buffer[length_send - 1] = '\0';
    }
    MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
    model_name = std::string(char_buffer);

    // file_name
    length_send  = file_name.size() + 1;
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID == 0)
    {
        strcpy(char_buffer, file_name.c_str());
        char_buffer[length_send - 1] = '\0';
    }
    MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
    file_name = std::string(char_buffer);

    // stage_name;
    length_send  = stage_name.size() + 1;
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID == 0)
    {
        strcpy(char_buffer, stage_name.c_str());
        char_buffer[length_send - 1] = '\0';
    }
    MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
    stage_name = std::string(char_buffer);

    // previous_stage_name;
    length_send  = previous_stage_name.size() + 1;
    MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
    if (processID == 0)
    {
        strcpy(char_buffer, previous_stage_name.c_str());
        char_buffer[length_send - 1] = '\0';
    }
    MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
    previous_stage_name = std::string(char_buffer);


    // nsteps;
    //char_buffer = previous_stage_name.c_str();
    //length_send = previous_stage_name.size();
    MPI_Bcast(&number_of_time_steps, 1, MPI_CHAR,   root, MPI_COMM_WORLD);
    // previous_stage_name = previous_stage_name_tmp;

    MPI_Bcast(&length_nodes_displacements_output    , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&length_nodes_velocities_output       , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&length_nodes_accelerations_output    , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&length_nodes_reaction_forcess_output , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&length_element_output                , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&number_of_nodes                      , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&number_of_elements                   , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&max_node_tag                         , 1 , MPI_INT , root , MPI_COMM_WORLD);
    MPI_Bcast(&max_element_tag                        , 1 , MPI_INT , root , MPI_COMM_WORLD);

    //The slave processes must be initialized after all this stuff is transmitted.
    if (processID != 0)
    {
        // this->initialize(file_name, model_name, stage_name, number_of_time_steps);
        // if (previous_stage_name.compare("!!none") == 0)
        // {
        //     cout << "changing previous_stage_name from " << previous_stage_name << " to " << stage_name
        //          << endl;
        //     previous_stage_name = stage_name;
        // }
        // file_name = "";
        // model_name = "";
        // stage_name = "";
        // file_name += filename_in;
        // model_name += model_name_in;
        // stage_name += stage_name_in;

        current_time                         = 0.0;
        current_time_step                    = 0;

        pos_nodes_outputs                    = 0;
        pos_nodes_coordinates                = 0;
        pos_elements_outputs                 = 0;
        pos_elements_gausscoords             = 0;
        pos_elements_connectivity            = 0;

        create_nodeMeshData_arrays           = true;
        create_nodeDisplacements_arrays      = true;
        create_nodeVelocities_arrays         = true;
        create_nodeAccelerations_arrays      = true;
        create_nodeReactionForces_arrays     = true;
        create_elementMeshData_arrays        = true;
        create_elementOutput_arrays          = true;
    }

#endif
}


void H5OutputWriter::writeMesh()
{
    // =============================================================================================
    //   Open HDF5 file for writing and create basic structure
    // =============================================================================================




    //Check if this is a new file!
    if (file_is_open)
    {
        finalize();
        // previous_stage_name  = stage_name;
    }
    else
    {
        previous_stage_name = "!!none";
    }

    //Initialize datamembers
    file_is_open = false;


    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    int numProcesses, processID;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);


    hid_t file_access_plist   = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(file_access_plist, MPI_COMM_WORLD, MPI_INFO_NULL);

    id_file = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, file_access_plist);
    H5Pclose(file_access_plist);
    //------------------------
#else

    //================================================================================
    //Create the file, overwriting it if it exists
    //================================================================================
    unsigned flags = H5F_ACC_TRUNC;  // Truncate file (ie. if file exists overwrite it)
    hid_t file_creation_plist = H5Pcreate(H5P_FILE_CREATE);
    hid_t file_access_plist = H5Pcreate(H5P_FILE_ACCESS);
    // status = H5Pset_meta_block_size(file_access_plist, H5OUTPUTWRITER_META_BLOCK_SIZE);
    // hdf5_check_error(status);
    // status =  H5Pset_cache(file_access_plist, 0, H5OUTPUTWRITER_CHUNK_NSLOTS, H5OUTPUTWRITER_CHUNK_NBYTES, 0 );
    // hdf5_check_error(status);
    // status = H5Pset_sieve_buf_size( file_access_plist, H5OUTPUTWRITER_SIEVE_BUFFER_SIZE );
    // hdf5_check_error(status);
    id_file = H5Fcreate(file_name.c_str(), flags , file_creation_plist, file_access_plist);
#endif
    if (id_file > 0)
    {
        file_is_open = true;


        //================================================================================
        //Setup the global property lists
        //================================================================================
        // group_creation_plist = H5Pcreate(H5P_GROUP_CREATE);
        // status = H5Pset_link_creation_order(group_creation_plist, H5P_CRT_ORDER_TRACKED | H5P_CRT_ORDER_INDEXED);
        // hdf5_check_error(status);



        //================================================================================
        // Create basic structure of the file
        //================================================================================
        //Create the model name, stage name and previous stage fields
        write_string(id_file, "Model_Name", model_name);
        write_string(id_file, "Stage_Name", stage_name);
        write_string(id_file, "Previous_Stage", previous_stage_name);


        //Create the group to store the model
        id_model_group = create_group(id_file, "Model");


        //Create elements and nodes groups
        id_elements_group = create_group(id_model_group, "Elements");
        id_nodes_group    = create_group(id_model_group, "Nodes");

        //================================================================================
        // Create time array
        //================================================================================

        hsize_t dims[1], maxdims[1];
        int rank = 1;
        dims[0] = number_of_time_steps;
        maxdims[0] = H5S_UNLIMITED;
        id_time_vector = createVariableLengthDoubleArray(id_file, rank, dims, maxdims, "time", "s");




        //================================================================================
        // Domain metadata
        //================================================================================
        rank = 1;
        dims[0] = 1;
        maxdims[0] = 1;
        id_number_of_elements   = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Elements", " ");
        id_number_of_nodes      = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Nodes", " ");
        id_number_of_time_steps = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Time_Steps", " ");

        //Write time of creation
        time_t current;
        time(&current);
        char *timestring;
        timestring = ctime(&current);
        timestring[strlen(timestring) - 1] = '\0';
        write_string(id_file, "Date_and_Time_Start", timestring);


        createVariableLengthStringArray(id_file, "Analysis_Options", " ")


        H5OUTPUTWRITER_COUNT_OBJS;


        // =============================================================================================
        //   Write node mesh data
        // =============================================================================================


        rank = 1;
        int datarank         = 1;
        int *int_data_buffer = 0;
        double *double_data_buffer = 0;
        //hsize_t dims[1];
        //hsize_t maxdims[1];
        hsize_t data_dims[1];
        hsize_t offset[1];
        hsize_t stride[1];
        hsize_t count[1];
        hsize_t block[1];

        //Create arrays
        dims[0] = 1;
        maxdims[0] = H5S_UNLIMITED;
        id_nodes_ndofs                = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Number_of_DOFs"                     , " ");
        id_nodes_coordinates          = createVariableLengthDoubleArray(id_nodes_group  , rank , dims , maxdims , "Coordinates"                        , " ");
        id_index_to_nodes_coordinates = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Coordinates"               , " ");
        id_index_to_nodes_outputs     = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Generalized_Displacements" , " ");

        // #ifdef _PARALLEL_PROCESSING
        //     // int processID;
        //     // MPI_Comm_rank(MPI_COMM_WORLD, &processID);

        //     if (processID == 0)
        //     {
        //         //
        // #endif
        // Write a vector with the number of DOFS for node at a given tag
        dims[0]      = (hsize_t) Number_of_DOFs.Size();
        data_dims[0] = (hsize_t) Number_of_DOFs.Size();
        offset[0]    = 0;
        stride[0]    = 1;
        count[0]     = dims[0];
        block[0]     = 1;
        int_data_buffer = Number_of_DOFs.data;
        writeVariableLengthIntegerArray(id_nodes_ndofs,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        dims[0]      = (hsize_t) Coordinates.Size();
        data_dims[0] = (hsize_t) Coordinates.Size();
        count[0]     = dims[0];
        double_data_buffer = Coordinates.theData;
        writeVariableLengthDoubleArray(id_nodes_coordinates,
                                       datarank,
                                       dims,
                                       data_dims,
                                       offset,
                                       stride,
                                       count,
                                       block,
                                       double_data_buffer);

        dims[0]      = (hsize_t) Index_to_Coordinates.Size();
        data_dims[0] = (hsize_t) Index_to_Coordinates.Size();
        count[0]     = dims[0];
        int_data_buffer = Index_to_Coordinates.data;
        writeVariableLengthIntegerArray(id_index_to_nodes_coordinates,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        dims[0]      = (hsize_t) Index_to_Generalized_Displacements.Size();
        data_dims[0] = (hsize_t) Index_to_Generalized_Displacements.Size();
        count[0]     = dims[0];
        int_data_buffer = Index_to_Generalized_Displacements.data;
        writeVariableLengthIntegerArray(id_index_to_nodes_outputs,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);




        //================================================================================
        // Create number of elements and nodes
        //================================================================================

        rank        = 1;
        dims[0]     = 1;
        data_dims[0] = 1;
        offset[0]   = 0;
        stride[0]   = 1;
        count[0]    = 1;
        block[0]    = 1;

        writeConstantLengthIntegerArray(id_number_of_nodes, rank, dims, data_dims, offset, stride, count, block,
                                        &number_of_nodes);

        writeConstantLengthIntegerArray(id_number_of_elements, rank, dims, data_dims, offset, stride, count, block,
                                        &number_of_elements);



        // #ifdef _PARALLEL_PROCESSING
        //     }
        // #endif
        H5OUTPUTWRITER_COUNT_OBJS;

        // =============================================================================================
        //   Write element mesh data
        // =============================================================================================


        rank = 1;
        dims[0] = 1;
        maxdims[0] = H5S_UNLIMITED;

        id_elements_nnodes                = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Nodes", " ");
        id_elements_connectivity          = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Connectivity", " ");
        id_index_to_elements_connectivity = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Connectivity", " ");
        id_elements_noutputs              = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Output_Fields", " ");
        id_index_to_elements_output       = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Outputs", " ");
        id_elements_ngauss                = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Gauss_Points", " ");
        id_elements_gausscoords           = createVariableLengthDoubleArray (id_elements_group, rank, dims, maxdims, "Gauss_Point_Coordinates", " ");
        id_index_to_elements_gausscoords  = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Gauss_Point_Coordinates", " ");
        id_elements_type                  = createVariableLengthStringArray (id_elements_group,  "Element_types", " ");
        id_elements_materialtag           = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Material_tags", " ");
        id_elements_classtag              = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Class_Tags", " ");

#ifdef _PARALLEL_PROCESSING // Only write partition if in parallel mode
        id_elements_partition             = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Partition", " ");
#endif

        // Write a vector with the number of nodes at a given elements tag
        datarank     = 1;
        dims[0]      = (hsize_t) Number_of_Nodes.Size();
        data_dims[0] = (hsize_t) Number_of_Nodes.Size();
        offset[0]    = 0;
        stride[0]    = 1;
        count[0]     = dims[0];
        block[0]     = 1;
        int_data_buffer = Number_of_Nodes.data;
        writeVariableLengthIntegerArray(id_elements_nnodes,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);


        //Write index to connectivity
        dims[0]      = (hsize_t) Index_to_Connectivity.Size();
        data_dims[0] = (hsize_t) Index_to_Connectivity.Size();
        count[0]     = dims[0];
        int_data_buffer = Index_to_Connectivity.data;
        writeVariableLengthIntegerArray(id_index_to_elements_connectivity,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);


        //Write element connectivity
        dims[0]      = (hsize_t) Connectivity.Size();
        data_dims[0] = (hsize_t) Connectivity.Size();
        count[0]     = dims[0];
        int_data_buffer = Connectivity.data;
        writeVariableLengthIntegerArray(id_elements_connectivity,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        //Write index to number of outputs
        dims[0]      = (hsize_t) Number_of_Output_Fields.Size();
        data_dims[0] = (hsize_t) Number_of_Output_Fields.Size();
        count[0]     = dims[0];
        int_data_buffer = Number_of_Output_Fields.data;
        writeVariableLengthIntegerArray(id_elements_noutputs,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        //Write index to outputs
        dims[0]      = (hsize_t) Index_to_Outputs.Size();
        data_dims[0] = (hsize_t) Index_to_Outputs.Size();
        count[0]     = dims[0];
        int_data_buffer = Index_to_Outputs.data;
        writeVariableLengthIntegerArray(id_index_to_elements_output,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        //Write material tags
        dims[0]      = (hsize_t) Material_tags.Size();
        data_dims[0] = (hsize_t) Material_tags.Size();
        count[0]     = dims[0];
        int_data_buffer = Material_tags.data;
        writeVariableLengthIntegerArray(id_elements_materialtag,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        dims[0]      = (hsize_t) Class_Tags.Size();
        data_dims[0] = (hsize_t) Class_Tags.Size();
        count[0]     = dims[0];
        int_data_buffer = Class_Tags.data;
        writeVariableLengthIntegerArray(id_elements_classtag,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);


#ifdef _PARALLEL_PROCESSING
        dims[0]      = (hsize_t) Class_Tags.Size();
        data_dims[0] = (hsize_t) Class_Tags.Size();
        count[0]     = dims[0];
        int_data_buffer = Partition.data;
        writeVariableLengthIntegerArray(id_elements_partition,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);
#endif

        // TODO: Bring back element types
        // //Write material tags
        // for (int tag = 0; tag < (int) Element_types.size(); tag++)
        // {
        //     std::string type = Element_types[tag];
        //     writeVariableLengthStringArray(id_elements_type,
        //                                    tag,
        //                                    type.size(),
        //                                    type);
        // }


        //Write index to gauss coordinates (if any)
        dims[0]      = (hsize_t) Number_of_Gauss_Points.Size();
        data_dims[0] = (hsize_t) Number_of_Gauss_Points.Size();
        count[0]     = dims[0];
        int_data_buffer = Number_of_Gauss_Points.data;
        writeVariableLengthIntegerArray(id_elements_ngauss,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);


        dims[0]      = (hsize_t) Index_to_Gauss_Point_Coordinates.Size();
        data_dims[0] = (hsize_t) Index_to_Gauss_Point_Coordinates.Size();
        count[0]     = dims[0];
        int_data_buffer = Index_to_Gauss_Point_Coordinates.data;
        writeVariableLengthIntegerArray(id_index_to_elements_gausscoords,
                                        datarank,
                                        dims,
                                        data_dims,
                                        offset,
                                        stride,
                                        count,
                                        block,
                                        int_data_buffer);

        //Write gauss coordinate values
        dims[0]      = (hsize_t) Gauss_Point_Coordinates.Size();
        data_dims[0] = (hsize_t) Gauss_Point_Coordinates.Size();
        count[0]     = dims[0];
        double_data_buffer = Gauss_Point_Coordinates.theData;
        writeVariableLengthDoubleArray(id_elements_gausscoords,
                                       datarank,
                                       dims,
                                       data_dims,
                                       offset,
                                       stride,
                                       count,
                                       block,
                                       double_data_buffer);
        H5OUTPUTWRITER_COUNT_OBJS;






        // =============================================================================================
        //   CREATE OUTPUT ARRAYS!
        // =============================================================================================

        {
            int rank = 2;
            hsize_t dims[2];
            hsize_t maxdims[2];
            dims[0] = (hsize_t) length_nodes_displacements_output;
            dims[1] = 1;
            maxdims[0] = (hsize_t)  length_nodes_displacements_output;
            maxdims[1] = H5S_UNLIMITED;

            id_nodes_displacements = createVariableLengthDoubleArray(id_nodes_group, rank, dims, maxdims, "Generalized_Displacements", " ");
        }

        {
            int rank = 2;
            hsize_t dims[2];
            hsize_t maxdims[2];
            dims[0] = (hsize_t) length_element_output;
            dims[1] = 1;
            maxdims[0] = (hsize_t)  length_element_output;
            maxdims[1] = H5S_UNLIMITED;

            id_elements_output = createVariableLengthDoubleArray(id_elements_group, rank, dims, maxdims, "Outputs", " ");

        }//    create_elementOutput_arrays = false;

        // cout << "subgroupname" << subgroupname << endl;
    }
    else  //Could not open file
    {
        cerr << "H5OutputWriter::writeMesh() -> could not open file: " << file_name << "!\n\n";
    }


}

int H5OutputWriter::writeElementPartitionData(int tag  , int partition)
{
#ifdef _PARALLEL_PROCESSING
    int nnodes, ntags;
    ntags = Number_of_Nodes.Size();
    int addzeros = tag - ntags;

    //Extend arrays
    for (int i = 0; i <= addzeros; i++)
    {
        Partition[ntags + i]                       = -1;
    }
    Partition[tag] = partition;
#else
    cout << "H5OutputWriter::writeElementPartitionData() -- Not available in sequential mode\n";
#endif
    return 0;
}




int H5OutputWriter::writeMaterialMeshData(int tag , std::string type , Vector &parameters)
{
    return 0;
}

// Results for Nodes
int H5OutputWriter::writeDisplacements(  int nodeTag, const Vector &displacements)
{

    int pos, ndofs;

    // Read NDOFS from HDF5 file
    int datarank         = 1;
    hsize_t data_dims[1] = {1};
    hsize_t offset[2]    = {(hsize_t) nodeTag , 0};
    hsize_t stride[2]    = {1, 0};
    hsize_t count[2]     = {1, 0};
    hsize_t block[2]     = {1, 0};

    hsize_t id_dataspace = H5Dget_space(id_nodes_ndofs);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace
    status = H5Sselect_hyperslab(
                 id_dataspace,          // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );

    hdf5_check_error(status);

    H5Dread(id_nodes_ndofs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &ndofs);
    H5Dread(id_index_to_nodes_outputs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &pos);


    //Write data
    hsize_t dims[2]      =  { (hsize_t)  length_nodes_displacements_output, (hsize_t)  current_time_step};
    data_dims[0] = (hsize_t) ndofs;

    offset[0]    = (hsize_t) pos;
    offset[1]    = (hsize_t) current_time_step - 1;
    stride[0]    = 1;
    stride[1]    = 1;
    count[0]     = (hsize_t) ndofs;
    count[1]     = 1;
    block[0]     = 1;
    block[1]     = 1;
    double *data = displacements.theData;
    writeVariableLengthDoubleArray(id_nodes_displacements,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   data);

    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return 0;
}


int H5OutputWriter::writeVelocities(     int nodeTag, const Vector &velocities)
{
    cout << "H5OutputWriter::writeVelocities()  -- Not available. Implement by copying writeDisplacements.\n ";
    return 0;
}


int H5OutputWriter::writeAccelerations(  int nodeTag, const Vector &accelerations)
{
    cout << "H5OutputWriter::writeAccelerations()  -- Not available. Implement by copying writeDisplacements.\n ";
    return 0;
}


int H5OutputWriter::writeReactionForces( int nodeTag, const Vector &reactionForces)
{
    cout << "H5OutputWriter::writeReactionForces()  -- Not available. Implement by copying writeDisplacements.\n ";
    return 0;
}



// Results for Elements
int H5OutputWriter::writeElementOutput(int elementTag, const  Vector &output)
{

    if (length_element_output > 0) // If there is nothing to output, there is nothing to output
    {
        int pos, noutputs;

        // Read NOUTPUTS from HDF5 file
        int datarank         = 1;
        hsize_t data_dims[1] = {1};
        hsize_t offset[2]    = {(hsize_t) elementTag , 0};
        hsize_t stride[2]    = {1, 0};
        hsize_t count[2]     = {1, 0};
        hsize_t block[2]     = {1, 0};

        hsize_t id_dataspace = H5Dget_space(id_elements_noutputs);
        hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace
        status = H5Sselect_hyperslab(
                     id_dataspace,          // Id of the parent dataspace
                     H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                     offset,                // start of selection
                     stride,                // stride in each dimension, NULL  is select everything
                     count ,                // how many blocks to select in each direction
                     block                  // little block selected per selection
                 );

        hdf5_check_error(status);

        H5Dread(id_elements_noutputs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &noutputs);
        H5Dread(id_index_to_elements_output, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &pos);


        //Write data
        hsize_t dims[2]      =  { (hsize_t)  length_element_output, (hsize_t)  current_time_step};
        data_dims[0] = (hsize_t) noutputs;

        offset[0]    = (hsize_t) pos;
        offset[1]    = (hsize_t) current_time_step - 1;
        stride[0]    = 1;
        stride[1]    = 1;
        count[0]     = (hsize_t) noutputs;
        count[1]     = 1;
        block[0]     = 1;
        block[1]     = 1;
        double *data = output.theData;
        writeVariableLengthDoubleArray(id_elements_output,
                                       datarank,
                                       dims,
                                       data_dims,
                                       offset,
                                       stride,
                                       count,
                                       block,
                                       data);

        H5Sclose(id_dataspace);
        H5Sclose(id_memspace);
        H5OUTPUTWRITER_COUNT_OBJS;
    }
    return 0;
}








int H5OutputWriter::setTime(double t)
{
    current_time = t;

    //Extending past initial data allocation
    if (current_time_step >= number_of_time_steps)
    {
        number_of_time_steps = current_time_step + 1;
    }

    hsize_t dims[1]      = {(hsize_t) number_of_time_steps};
    hsize_t data_dims[1] = {1};
    hsize_t offset[1]    = {(hsize_t) current_time_step };
    hsize_t stride[1]    = {1};
    hsize_t count[1]     = {1};
    hsize_t block[1]     = {1};

    writeVariableLengthDoubleArray(id_time_vector,
                                   1, //datarank
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   &current_time);

    offset[0]   = 0;

    writeConstantLengthIntegerArray(id_number_of_time_steps, 1, dims, data_dims, offset, stride, count, block,
                                    &number_of_time_steps);

    current_time_step++;
    H5OUTPUTWRITER_COUNT_OBJS;


    //Extend objects
    hsize_t dims_new[2]      =  { (hsize_t)  length_nodes_displacements_output, (hsize_t)  current_time_step};

    status =  H5Dset_extent( id_nodes_displacements, dims_new );

    dims_new[0] = length_element_output;
    dims_new[1] = current_time_step;

    status =  H5Dset_extent( id_elements_output, dims_new );


    return 0;
}



int H5OutputWriter::write_string(hid_t here, std::string name, std::string contents)
{
    // std::cout << "Writing string" << endl;
    // Some variables needed
    hid_t id_type_string;           // Id of the string object type
    hid_t id_dataspace_string;      // Id of the string dataspace
    hid_t id_data_string;           // Id of the actual string data
    hsize_t dims_string[1];         // Dimensions of the string array [1: scalar]


    id_type_string = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(id_type_string, contents.size());
    hdf5_check_error(status);
    dims_string[0] = 1;//contents.size();
    id_dataspace_string = H5Screate_simple(1, dims_string, NULL);
    id_data_string = H5Dcreate2(here,
                                name.c_str(),
                                id_type_string,
                                id_dataspace_string,
                                H5P_DEFAULT,
                                H5P_DEFAULT,
                                H5P_DEFAULT);

    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(id_data_string,
                      id_type_string,
                      H5S_ALL,
                      H5S_ALL,
                      file_access_plist,
                      contents.c_str());
#else

    status = H5Dwrite(id_data_string,
                      id_type_string,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      contents.c_str());
#endif
    //------------------------


    //     status = H5Dwrite(id_data_string,
    //                       id_type_string,
    //                       H5S_ALL,
    //                       H5S_ALL,
    //                       H5P_DEFAULT,
    //                       contents.c_str());

    H5Sclose(id_dataspace_string);
    H5Dclose(id_data_string);

    hdf5_check_error(status);

    return 0;
}

int H5OutputWriter::create_group(hid_t here, std::string name)
{

    // cout << "Creating group : " << name << endl;
    hid_t id_new_group;
    id_new_group = H5Gcreate(here, name.c_str(), H5P_DEFAULT, group_creation_plist , H5P_DEFAULT);
    return id_new_group;
}


int H5OutputWriter::setAttribute(hid_t object, std::string units)
{
    // hid_t atype;
    // hid_t attribute_dataspace_id;
    // hid_t attribute;

    // attribute_dataspace_id = H5Screate(H5S_SCALAR);
    // atype = H5Tcopy(H5T_C_S1);
    // H5Tset_size(atype, units.size() );
    // attribute = H5Acreate2(object,
    //                        "units",
    //                        atype,
    //                        attribute_dataspace_id,
    //                        H5P_DEFAULT,
    //                        H5P_DEFAULT);
    // status = H5Awrite(attribute, atype, units.c_str());
    // H5Aclose(attribute);
    // H5Sclose(attribute_dataspace_id);

    return 0;
}




hid_t H5OutputWriter::createVariableLengthDoubleArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{
    hid_t id_array;
    double fill_value = 0.0;
    id_array = createVariableLengthArray(here,
                                         rank,
                                         dims,
                                         maxdims,
                                         name,
                                         attribute,
                                         H5T_NATIVE_DOUBLE,
                                         sizeof(double),
                                         &fill_value);

    return id_array;
}

hid_t H5OutputWriter::createVariableLengthIntegerArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{
    hid_t id_array;
    int fill_value = -1;
    id_array = createVariableLengthArray(here,
                                         rank,
                                         dims,
                                         maxdims,
                                         name,
                                         attribute,
                                         H5T_NATIVE_INT,
                                         sizeof(int),
                                         &fill_value);

    return id_array;
}




hid_t H5OutputWriter::createVariableLengthStringArray(hid_t here,
        std::string name,
        std::string attribute)
{
    int rank = 1;
    hsize_t dims[1] = {1};
    hsize_t maxdims[1] = {H5S_UNLIMITED};
    hid_t id_array;

    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace


    //Use unlimited string type
    hid_t type = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(type, H5T_VARIABLE);
    hdf5_check_error(status);

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    hsize_t chunk_dims[1] = {1};

    // cout << "nbytes_one_chunk = " << nbytes_one_chunk << "\n\n\n";
    // cout << "nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK = " << nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK << "\n\n\n";

    status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
    hdf5_check_error(status);
    status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
    hdf5_check_error(status);


    // Create the dataset
    id_array = H5Dcreate2(here,
                          name.c_str(),
                          type,
                          id_dataspace,
                          H5P_DEFAULT,
                          dataset_creation_plist,
                          dataset_access_plist);
    setAttribute(id_array, attribute);

    H5Sclose(id_dataspace);
    H5OUTPUTWRITER_COUNT_OBJS;



    return id_array;
}



hid_t H5OutputWriter::createVariableLengthArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute,
        hid_t type, int sizeof_type, void *fill_value_ptr)
{
    // int fill_value = 0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    hsize_t chunk_dims[rank];
    int nbytes_one_chunk = sizeof_type;// sizeof(int);
    for (int i = 0; i < rank; i++)
    {
        if (maxdims[i] == H5S_UNLIMITED)
        {
            chunk_dims[i] = H5OUTPUTWRITER_CHUNK_TIMEDIM;
        }
        else
        {
            chunk_dims[i] = maxdims[i];
        }
        nbytes_one_chunk *= chunk_dims[i];
    }

    // cout << "nbytes_one_chunk = " << nbytes_one_chunk << "\n\n\n";
    // cout << "nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK = " << nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK << "\n\n\n";

    status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
    hdf5_check_error(status);
    status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
    hdf5_check_error(status);
    status = H5Pset_chunk_cache(dataset_access_plist, H5OUTPUTWRITER_CHUNK_NSLOTS, nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK, H5OUTPUTWRITER_PREEMPTION_POLICY );
    hdf5_check_error(status);
    status = H5Pset_fill_value(dataset_creation_plist, type, fill_value_ptr);
    hdf5_check_error(status);
    if (type == H5T_NATIVE_INT)
    {
        status = H5Pset_fill_time(dataset_creation_plist, H5D_FILL_TIME_ALLOC);// H5D_FILL_TIME_NEVER);
    }
    else
    {
        status = H5Pset_fill_time(dataset_creation_plist, H5D_FILL_TIME_NEVER);
    }
    hdf5_check_error(status);
    // status = H5Pset_deflate (dataset_creation_plist, 6);
    // hdf5_check_error(status);



    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                type,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                dataset_access_plist);
    setAttribute(id_array, attribute);

    H5Sclose(id_dataspace);
    H5OUTPUTWRITER_COUNT_OBJS;

    return id_array;
}


hid_t H5OutputWriter::createConstantLengthDoubleArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{
    double fill_value = 0.0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);


    status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_DOUBLE, &fill_value);
    hdf5_check_error(status);


    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank , dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                H5T_NATIVE_DOUBLE,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                H5P_DEFAULT);
    setAttribute(id_array, attribute);
    H5Sclose(id_dataspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}




hid_t H5OutputWriter::createConstantLengthIntegerArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{
    int fill_value = 0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);


    status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_INT, &fill_value);
    hdf5_check_error(status);


    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank , dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                H5T_NATIVE_INT,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                H5P_DEFAULT);
    setAttribute(id_array, attribute);
    H5Sclose(id_dataspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}




hid_t H5OutputWriter::writeVariableLengthDoubleArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        double *data)
{
    // Extend it if necesary!
    status =  H5Dset_extent( id_array, dims );
    hdf5_check_error(status);

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    //Select the region of data to output to
    status = H5Sselect_hyperslab(
                 id_dataspace,          // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );
    hdf5_check_error(status);




    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 file_access_plist,           // Form of writing
                 data                   // The actual data
             );
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif
    //------------------------

    //Write data!
    //     status = H5Dwrite(
    //                  id_array,              // Dataset to write to
    //                  H5T_NATIVE_DOUBLE,     // Format of data in memory
    //                  id_memspace,           // Description of data in memory
    //                  id_dataspace,          // Description of data in storage (including selection)
    //                  H5P_DEFAULT,           // Form of writing
    //                  data                   // The actual data

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}


hid_t H5OutputWriter::writeVariableLengthIntegerArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        int *data)
{
    // Extend it if necesary!
    status =  H5Dset_extent( id_array, dims );
    hdf5_check_error(status);

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    //Select the region of data to output to
    status = H5Sselect_hyperslab(
                 id_dataspace,              // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );
    hdf5_check_error(status);




    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 file_access_plist,           // Form of writing
                 data                   // The actual data

             );


#else

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );

#endif
    //------------------------

    //Write data!
    //     status = H5Dwrite(
    //                  id_array,              // Dataset to write to
    //                  H5T_NATIVE_INT,        // Format of data in memory
    //                  id_memspace,           // Description of data in memory
    //                  id_dataspace,          // Description of data in storage (including selection)
    //                  H5P_DEFAULT,           // Form of writing
    //                  data                   // The actual data
    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}



hid_t H5OutputWriter::writeVariableLengthStringArray(hid_t id_array,
        hsize_t offset_,
        hsize_t datasize,
        std::string &data)
{
    int datarank = 1;
    hsize_t dims[1] = {offset_ + 1};
    hsize_t data_dims[1] = {1};
    hsize_t stride[1] = {1};
    hsize_t count[1] = {1};
    hsize_t block[1] = {1};
    hsize_t offset[1] = {offset_};



    // Extend it if necesary!
    status =  H5Dset_extent( id_array, dims );
    hdf5_check_error(status);

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    //Select the region of data to output to
    status = H5Sselect_hyperslab(
                 id_dataspace,              // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );
    hdf5_check_error(status);

    //Write data!

    hid_t type = H5Tcopy(H5T_C_S1);
    status = H5Tset_size(type, H5T_VARIABLE);
    hdf5_check_error(status);


    const char *dat = data.c_str();
    char *thedata[1] = {(char *)dat};




    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 type,                  // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 file_access_plist,           // Form of writing
                 thedata// The actual data
             );

#else
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 type,                  // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 thedata// The actual data
             );
#endif
    //------------------------

    //     status = H5Dwrite(
    //                  id_array,              // Dataset to write to
    //                  type,                  // Format of data in memory
    //                  id_memspace,           // Description of data in memory
    //                  id_dataspace,          // Description of data in storage (including selection)
    //                  H5P_DEFAULT,           // Form of writing
    //                  thedata// The actual data
    //              );


    // cout << "done! \n\n";

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}




hid_t H5OutputWriter::writeConstantLengthDoubleArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        double *data)
{
    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    //Select the region of data to output to
    status = H5Sselect_hyperslab(
                 id_dataspace,          // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );
    hdf5_check_error(status);



    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 file_access_plist,           // Form of writing
                 data                   // The actual data
             );
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif
    //------------------------
    //Write data!
    //     status = H5Dwrite(
    //                  id_array,              // Dataset to write to
    //                  H5T_NATIVE_DOUBLE,     // Format of data in memory
    //                  id_memspace,           // Description of data in memory
    //                  id_dataspace,          // Description of data in storage (including selection)
    //                  H5P_DEFAULT,           // Form of writing
    //                  data                   // The actual data
    //              );
    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}





hid_t H5OutputWriter::writeConstantLengthIntegerArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        int *data)
{
    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    //Select the region of data to output to
    status = H5Sselect_hyperslab(
                 id_dataspace,          // Id of the parent dataspace
                 H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                 offset,                // start of selection
                 stride,                // stride in each dimension, NULL  is select everything
                 count ,                // how many blocks to select in each direction
                 block                  // little block selected per selection
             );
    hdf5_check_error(status);


    //Added By Babak 5/31/14
    //------------------------
#ifdef _PARALLEL_PROCESSING
    hid_t file_access_plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(file_access_plist, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 file_access_plist,           // Form of writing
                 data                   // The actual data
             );
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif
    //------------------------


    //Write data!
    //     status = H5Dwrite(
    //                  id_array,              // Dataset to write to
    //                  H5T_NATIVE_INT,        // Format of data in memory
    //                  id_memspace,           // Description of data in memory
    //                  id_dataspace,          // Description of data in storage (including selection)
    //                  H5P_DEFAULT,           // Form of writing
    //                  data                   // The actual data
    //              );
    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}
