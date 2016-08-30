//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              H5Output:ter.cpp
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
#include <sstream>
#include <limits>
#include <stacktrace.h>
#include <algorithm>

#include <ID.h>
#include <Vector.h>
#include <H5OutputWriter.h>
#include <hdf5.h>
#include <time.h>

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

# include <ESSITimer.h>

#define VERIFY(a) do { if((a)<0) { fprintf(stderr,"Failure line %d.\n",__LINE__); exit(-1);}}while(0)

bool H5OutputWriter::call_hdf5_flush = false;

H5OutputWriter::H5OutputWriter():
    OutputWriter(),
    file_is_open(false),
    number_of_nodes(0),
    number_of_elements(0),
    number_of_time_steps(0),
    max_node_tag(0),
    max_element_tag(0),
    number_of_dofs(0),
    number_of_outputs(0),
    file_name(""),
    model_name(""),
    stage_name(""),
    numof_NO_COLLECTIVE_calls(0),
    numof_CHUNK_INDEPENDENT_calls(0),
    numof_CHUNK_COLLECTIVE_calls(0),
    numof_CHUNK_MIXED_calls(0)
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

    zlib_compression_level = 5;
    flag_write_element_output            = 1;

#ifndef _PARALLEL_PROCESSING_COLLECTIVE_IO
    dataset_xfer_plist = H5Pcreate(H5P_DATASET_XFER);  //Otherwise created in syncWriters in the parallel case
#endif
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
    number_of_dofs(0),
    number_of_outputs(0),
    file_name(""),
    model_name(""),
    stage_name(""),
    numof_NO_COLLECTIVE_calls(0),
    numof_CHUNK_INDEPENDENT_calls(0),
    numof_CHUNK_COLLECTIVE_calls(0),
    numof_CHUNK_MIXED_calls(0)
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

    zlib_compression_level = 5;
    flag_write_element_output = 1;

#ifndef _PARALLEL_PROCESSING_COLLECTIVE_IO
    dataset_xfer_plist = H5Pcreate(H5P_DATASET_XFER); //Otherwise created in syncWriters in the parallel case
#endif
}



H5OutputWriter::~H5OutputWriter()
{
    finalize();
}


void H5OutputWriter::finalize()
{
    // cout << "H5OutputWriter::finalize() - ";
    if (file_is_open)
    {
        // cout << "endingtime \n\n";
        time_t current ;
        time(&current);
        char *timestring;
        timestring = ctime(&current);
        timestring[strlen(timestring) - 1] = '\0';
        write_string(id_file, "Date_and_Time_End", timestring);


        //Close all objects
        // H5F_OBJ_FILE    Files only
        // H5F_OBJ_DATASET Datasets only
        // H5F_OBJ_GROUP   Groups only
        // H5F_OBJ_DATATYPE    Named datatypes only
        // H5F_OBJ_ATTR    Attributes only
        // H5F_OBJ_ALL All of the above
        // (That is, H5F_OBJ_FILE | H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_DATATYPE | H5F_OBJ_ATTR)
        // H5F_OBJ_LOCAL   Restrict search to objects opened through current file identifier.
        // Note: H5F_OBJ_LOCAL does not stand alone; it is effective only when used in combination with one or more of the preceding types. For example,
        //     H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_LOCAL
        // would count all datasets and groups opened through the current file identifier.

        hid_t obj_id_list[H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS];
        hsize_t n_obj_open = 10;
        while (n_obj_open > 0)
        {
            n_obj_open = H5Fget_obj_count(id_file, H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_ATTR | H5F_OBJ_LOCAL );
            // cout << "H5OutputWriter -- N of HDF5 objects open = " << n_obj_open << endl;


            if (n_obj_open <= 0)
            {
                break;
            }


            int number_of_open_objects;

            //Close datasets
            number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_DATASET | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
            for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
            {
                H5Dclose(obj_id_list[i]);
            }

            //Close groups
            number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_GROUP | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
            for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
            {
                H5Gclose(obj_id_list[i]);
            }

            //Close groups
            number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_ATTR | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
            for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
            {
                H5Aclose(obj_id_list[i]);
            }

        }
        //Finally close file
        file_is_open = false;
        H5Fclose(id_file);

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
        int processID;
        MPI_Comm_rank(MPI_COMM_WORLD, &processID);


        int total_write_calls = numof_NO_COLLECTIVE_calls +
                                numof_CHUNK_INDEPENDENT_calls +
                                numof_CHUNK_COLLECTIVE_calls +
                                numof_CHUNK_MIXED_calls;

        if (total_write_calls > 0)
        {
            cout << "Finalized HDF5 writer on process " << processID << " \n";
            cout << " Summary of HDF5 I/O mode statistics:\n";
            cout << "   NO_COLLECTIVE     = " << numof_NO_COLLECTIVE_calls     << " (" << 100 * double(numof_NO_COLLECTIVE_calls) / double(total_write_calls)     << "%%)\n";
            cout << "   CHUNK_INDEPENDENT = " << numof_CHUNK_INDEPENDENT_calls << " (" << 100 * double(numof_CHUNK_INDEPENDENT_calls) / double(total_write_calls) << "%%)\n";
            cout << "   CHUNK_COLLECTIVE  = " << numof_CHUNK_COLLECTIVE_calls  << " (" << 100 * double(numof_CHUNK_COLLECTIVE_calls) / double(total_write_calls)  << "%%)\n";
            cout << "   CHUNK_MIXED       = " << numof_CHUNK_MIXED_calls       << " (" << 100 * double(numof_CHUNK_MIXED_calls) / double(total_write_calls)       << "%%)\n";
            cout << "   TOTAL             = " << total_write_calls             << endl;
        }
#endif

    }
    else
    {
        // cout << "nothing to do... " << endl;
    }

}

void H5OutputWriter::initialize(std::string filename_in,
                                std::string model_name_in,
                                std::string stage_name_in,
                                int nsteps)
{

    file_name = filename_in;
    model_name = model_name_in;
    stage_name = stage_name_in;

    cout << "changing previous_stage_name from " << previous_stage_name << " to " << stage_name << endl;
    previous_stage_name = stage_name;

    current_time                         = 0.0;
    current_time_step                    = 0;
    number_of_nodes                      = 0;
    number_of_elements                   = 0;
    max_node_tag                         = 0;
    max_element_tag                      = 0;

    number_of_eigen_modes                = -1;

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

    number_of_gausspoints                =0;
    number_of_connectivity_nodes         =0;
    number_of_outputs                    =0;

    create_nodeMeshData_arrays           = true;
    create_nodeDisplacements_arrays      = true;
    create_nodeVelocities_arrays         = true;
    create_nodeAccelerations_arrays      = true;
    create_nodeReactionForces_arrays     = true;
    create_elementMeshData_arrays        = true;
    create_elementOutput_arrays          = true;

    number_of_time_steps                 = nsteps;

    numof_NO_COLLECTIVE_calls = 0;
    numof_CHUNK_INDEPENDENT_calls = 0;
    numof_CHUNK_COLLECTIVE_calls = 0;
    numof_CHUNK_MIXED_calls = 0;

}





void H5OutputWriter::set_number_of_time_steps(int nsteps)
{

#ifdef _PARALLEL_PROCESSING
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

    number_of_time_steps = nsteps;
}








// Mesh output


int H5OutputWriter::writeGlobalMeshData(unsigned int number_of_nodes_in,
                                        unsigned int number_of_elements_in,
                                        unsigned int max_node_tag_in,
                                        unsigned int max_element_tag_in,
                                        unsigned int number_of_dofs_in,
                                        unsigned int number_of_outputs_in,
                                        unsigned int Total_Number_of_Gauss_Points,
                                        unsigned int Total_Number_of_Connectivity_Nodes)
{
    number_of_nodes = number_of_nodes_in;
    number_of_elements = number_of_elements_in;
    max_node_tag = max_node_tag_in;
    max_element_tag = max_element_tag_in;

    bool Process_output = false;
#ifdef _PARALLEL_PROCESSING
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank!=0)
        Process_output=true;
#else
    Process_output = true;
#endif

        number_of_dofs = number_of_dofs_in;
        number_of_outputs = number_of_outputs_in;
        number_of_gausspoints = Total_Number_of_Gauss_Points;
        number_of_connectivity_nodes = Total_Number_of_Connectivity_Nodes;


        // cout << "number_of_nodes    = " << number_of_nodes    << endl;
        // cout << "number_of_elements = " << number_of_elements << endl;
        // cout << "max_node_tag       = " << max_node_tag       << endl;
        // cout << "max_element_tag    = " << max_element_tag    << endl;
        // cout << "number_of_dofs     = " << number_of_dofs     << endl;
        // cout << "number_of_outputs  = " << number_of_outputs  << endl;

        //Initialize data



        Coordinates.resize(number_of_nodes * 3);

        Number_of_DOFs.resize(max_node_tag + 1);
        Index_to_Coordinates.resize(max_node_tag + 1);
        Index_to_Generalized_Displacements.resize(max_node_tag + 1);
        for (int i = 0; i < max_node_tag + 1; i++)
        {
            Number_of_DOFs[i] = -1;
            Index_to_Coordinates[i] = -1;
            Index_to_Generalized_Displacements[i] = -1;
        }

        //For Elements
        Number_of_Nodes.resize(max_element_tag + 1);
        Index_to_Connectivity.resize(max_element_tag + 1);
        Index_to_Outputs.resize(max_element_tag + 1);
        Number_of_Gauss_Points.resize(max_element_tag + 1);
        Index_to_Gauss_Point_Coordinates.resize(max_element_tag + 1);

        Class_Tags.resize(max_element_tag + 1);
        Material_tags.resize(max_element_tag+1);
        Number_of_Output_Fields.resize(max_element_tag + 1);

        for (int i = 0; i < max_element_tag; i++)
        {
            Number_of_Nodes[i] = -1;
            Index_to_Connectivity[i] = -1;
            Index_to_Outputs[i] = -1;
            Number_of_Gauss_Points[i] = -1;
            Index_to_Gauss_Point_Coordinates[i] = -1;
            Class_Tags[i] = -1;
            Material_tags[i] = -1;
            Number_of_Output_Fields[i] = -1;
        }

        // Exact pre-allocation for these arrays fixed by sumeet 30th July, 2016

        Connectivity.resize(number_of_connectivity_nodes);
        Gauss_Point_Coordinates.resize(number_of_gausspoints*3);

    return 0;
}

int H5OutputWriter::writeNodeMeshData(int tag     , const Vector &coords   , int ndofs )
{
    int ntags = Number_of_DOFs.Size();
    int addzeros = tag - ntags;

    //Extend arrays
    for (int i = 0; i < addzeros; i++)
    {
        cout << "ntags = " << ntags << " tag = " << tag;
        cout << " H5OutputWriter::writeNodeMeshData() -- Should not happen!!\n\n"; //[sumeet August, 2016]
        Number_of_DOFs[ntags + i] = -1;
        Index_to_Generalized_Displacements[ntags + i] = -1;
        Index_to_Coordinates[ntags + i] = -1;
    }

    //Form Number_of_DOFs
    Number_of_DOFs[tag] = ndofs;

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

    if (tag < 0)
    {
        cerr << "H5OutputWriter::writeElementMeshData - Error: got tag = " << tag << " < 0" << endl;
    }

    int nnodes, ntags;
    ntags = Number_of_Nodes.Size();
    int addzeros = tag - ntags;

    //Extend arrays if necessary
    for (int i = 0; i <= addzeros; i++)
    {
        cout << "ntags = " << ntags << " tag = " << tag;
        cout << " H5OutputWriter::writeElementMeshData() -- Should not happen!!\n\n";
        Number_of_Nodes[ntags + i]                  = -1;
        Index_to_Connectivity[ntags + i]            = -1;
        Index_to_Outputs[ntags + i]                 = -1;
        Number_of_Gauss_Points[ntags + i]           = -1;
        Index_to_Gauss_Point_Coordinates[ntags + i] = -1;
        Material_tags[ntags + i]                    = -1;
        Number_of_Output_Fields[ntags + i]          = -1;
        Class_Tags[ntags + i]                       = -1;
    }

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

    // Writing Material_tags;
    Material_tags[tag] = materialtag;
    return 0;
}


int H5OutputWriter::writeLoadPatternData(int tag , std::string name)
{
    // int ntags;
    // ntags = LoadPattern_names.Size();
    // int addzeros = tag - ntags;

    // //Extend arrays
    // for (int i = 0; i <= addzeros; i++)
    // {
    //     LoadPattern_names.push_back(" not defined ");
    // }
    // LoadPattern_names.push_back(name);
    return 0;
}

int H5OutputWriter::writeSPConstraintsData(int nodetag , int dof)
{
    int nsps = SPNodes.Size();

    // int ntags;
    // ntags = LoadPattern_names.Size();
    // int addzeros = tag - ntags;

    // //Extend arrays
    // for (int i = 0; i <= addzeros; i++)
    // {
    //     LoadPattern_names.push_back(" not defined ");
    // }
    // LoadPattern_names.push_back(name);

    SPNodes[nsps] = nodetag;
    SPDofs[nsps] = dof;

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
    int processID;

    // MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);

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
    MPI_Bcast(&number_of_time_steps, 1, MPI_INT,   root, MPI_COMM_WORLD);
    // cout << "H5OutputWriter::syncWriters (" << rank << ") number_of_time_steps = " << number_of_time_steps << endl;

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
    bool Enable_Process_output = false;

    int numProcesses, processID;
    numProcesses = 1;
    processID = 0;

#ifdef _PARALLEL_PROCESSING
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);

    if(processID!=0)
        Enable_Process_output=true;


    //Higher ranks get an additional extension .pid

    // Determine the number of digits in the total number of processes
    int number = numProcesses;
    int digits = 0;
    if (number < 0)
    {
        digits = 1;    // remove this line if '-' counts as a digit
    }
    while (number)
    {
        number /= 10;
        digits++;
    }
    // return digits;
    if (processID > 0)
    {
        std::stringstream ss;
        ss << ".";
        ss << setfill('0') << setw(digits) << processID;

        size_t f = file_name.find(".feioutput");
        file_name.replace(f, std::string(".feioutput").length(), ss.str());

        // file_name += ss.str();
        file_name += ".feioutput";
    }

#else
    Enable_Process_output = true;
#endif

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


    if (id_file > 0)
    {
        file_is_open = true;


        //================================================================================
        //Setup the global property lists
        //================================================================================
        group_creation_plist = H5Pcreate(H5P_GROUP_CREATE);//H5P_GROUP_CREATE);
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
        dims[0] = (hsize_t) number_of_time_steps+1;
        maxdims[0] = (hsize_t) number_of_time_steps+1;

        id_time_vector = createConstantLengthFloatArray(id_file, rank, dims, maxdims, "time", "s");


        //================================================================================
        // Domain metadata
        //================================================================================
        rank = 1;
        dims[0] = 1;
        maxdims[0] = 1;
        id_number_of_elements   = createConstantLengthIntegerArray(id_file, rank, dims, maxdims,  "Number_of_Elements", " ");
        id_number_of_nodes      = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Nodes", " ");
        id_number_of_time_steps = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Time_Steps", " ");
        hsize_t id_my_proc_rank = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Process_Number", " ");
        hsize_t id_number_of_procs = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Processes_Used", " ");

        //================================================================================
        // Model bounds
        //================================================================================
        // rank = 1;
        // dims[0] = 6;
        // maxdims[0] = 6;
        // hsize_t id_model_bounds = createConstantLengthFloatArray(id_model_group, rank, dims, maxdims, "Model_Bounds", " ");
        dims[0] = 1;
        maxdims[0] = 1;

        // ================================================================================
        // Time of Creation
        // ================================================================================
        time_t current;
        time(&current);
        char *timestring;
        timestring = ctime(&current);
        timestring[strlen(timestring) - 1] = '\0';
        write_string(id_file, "Date_and_Time_Start", timestring);

        // ================================================================================
        // Analysis Options
        // ================================================================================
        createConstantLengthStringArray(id_file, rank,  dims, maxdims, "Analysis_Options", " ");


        H5OUTPUTWRITER_COUNT_OBJS;


        // =============================================================================================
        //   Write node mesh data
        // =============================================================================================


        rank = 1;
        int datarank         = 1;
        int *int_data_buffer = 0;
        float *float_data_buffer = 0;
        //hsize_t dims[1];
        //hsize_t maxdims[1];
        hsize_t data_dims[1];
        hsize_t offset[1];
        hsize_t stride[1];
        hsize_t count[1];
        hsize_t block[1];

        if(Enable_Process_output)
        {
            //Create arrays
            dims[0]    = (hsize_t) max_node_tag+1;
            maxdims[0] = (hsize_t) max_node_tag+1;
            id_nodes_ndofs                = createConstantLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Number_of_DOFs"                     , " ");
            id_index_to_nodes_coordinates = createConstantLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Coordinates"               , " ");
            id_index_to_nodes_outputs     = createConstantLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Generalized_Displacements" , " ");

            dims[0]    = number_of_nodes*3;
            maxdims[0] = number_of_nodes*3;      
            id_nodes_coordinates          = createConstantLengthFloatArray(id_nodes_group  , rank , dims , maxdims , "Coordinates"                        , " ");
            
            // Write a vector with the number of DOFS for node at a given tag
            dims[0]      = (hsize_t) Number_of_DOFs.Size();
            data_dims[0] = (hsize_t) Number_of_DOFs.Size();
            offset[0]    = 0;
            stride[0]    = 1;
            count[0]     = dims[0];
            block[0]     = 1;
            int_data_buffer = Number_of_DOFs.data;
            writeConstantLengthIntegerArray(id_nodes_ndofs,
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
            float_data_buffer = (float *) Coordinates.theData;
            writeConstantLengthFloatArray(id_nodes_coordinates,
                                           datarank,
                                           dims,
                                           data_dims,
                                           offset,
                                           stride,
                                           count,
                                           block,
                                           float_data_buffer);

            dims[0]      = (hsize_t) Index_to_Coordinates.Size();
            data_dims[0] = (hsize_t) Index_to_Coordinates.Size();
            count[0]     = dims[0];
            int_data_buffer = Index_to_Coordinates.data;
            writeConstantLengthIntegerArray(id_index_to_nodes_coordinates,
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
            writeConstantLengthIntegerArray(id_index_to_nodes_outputs,
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


            rank        = 1;
            dims[0]     = 1;
            data_dims[0] = 1;
            offset[0]   = 0;
            stride[0]   = 1;
            count[0]    = 1;
            block[0]    = 1;

            writeConstantLengthIntegerArray(id_my_proc_rank, rank, dims, data_dims, offset, stride, count, block,
                                            &processID);

            rank        = 1;
            dims[0]     = 1;
            data_dims[0] = 1;
            offset[0]   = 0;
            stride[0]   = 1;
            count[0]    = 1;
            block[0]    = 1;

            writeConstantLengthIntegerArray(id_number_of_procs, rank, dims, data_dims, offset, stride, count, block,
                                            &numProcesses);

            H5Dclose(id_my_proc_rank);
            H5Dclose(id_number_of_procs);


            // // =============================================================================================
            // //Determine the bounds
            // // =============================================================================================
            // float bounds[6];//, xmax, ymin, ymax, zmin, zmax;
            // bounds[0] = -std::numeric_limits<float>::infinity();
            // bounds[1] = std::numeric_limits<float>::infinity();
            // bounds[2] = -std::numeric_limits<float>::infinity();
            // bounds[3] = std::numeric_limits<float>::infinity();
            // bounds[4] = -std::numeric_limits<float>::infinity();
            // bounds[5] = std::numeric_limits<float>::infinity();
            // for (int n = 0; n < number_of_nodes; n++)
            // {
            //     float x = Coordinates[3 * n];
            //     float y = Coordinates[3 * n + 1];
            //     float z = Coordinates[3 * n + 2];
            //     if (x > bounds[0])
            //     {
            //         bounds[0] = x;
            //     }
            //     if (x < bounds[1])
            //     {
            //         bounds[1] = x;
            //     }
            //     if (y > bounds[2])
            //     {
            //         bounds[2] = y;
            //     }
            //     if (y < bounds[3])
            //     {
            //         bounds[3] = y;
            //     }
            //     if (z > bounds[4])
            //     {
            //         bounds[4] = z;
            //     }
            //     if (z < bounds[5])
            //     {
            //         bounds[5] = z;
            //     }
            // }
            // // cout << processID <<  ": Model bounds ("
            // //      << bounds[0] << ", "
            // //      << bounds[1] << ", "
            // //      << bounds[2] << ", "
            // //      << bounds[3] << ", "
            // //      << bounds[4] << ", "
            // //      << bounds[5] << ")\n";
            // //Write them
            // rank        = 1;
            // dims[0]     = 6;
            // data_dims[0] = 6;
            // offset[0]   = 0;
            // stride[0]   = 1;
            // count[0]    = 6;
            // block[0]    = 1;

            // writeConstantLengthFloatArray(id_model_bounds, rank, dims, data_dims, offset, stride, count, block,
            //                                bounds);
            // // cout << processID <<  "Wrote bounds!\n";
            // H5Dclose(id_model_bounds);
            // H5OUTPUTWRITER_COUNT_OBJS;

            // =============================================================================================
            //   Write element mesh data
            // =============================================================================================


            rank = 1;
            dims[0] = max_element_tag+1;
            maxdims[0] = max_element_tag+1;

            id_elements_nnodes                = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Nodes", " ");
            id_index_to_elements_connectivity = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Connectivity", " ");
            id_elements_noutputs              = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Output_Fields", " ");
            id_index_to_elements_output       = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Outputs", " ");
            id_elements_ngauss                = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Gauss_Points", " ");
            id_index_to_elements_gausscoords  = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Gauss_Point_Coordinates", " ");
            id_elements_materialtag           = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Material_tags", " ");
            id_elements_classtag              = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Class_Tags", " ");

            dims[0]    = this->number_of_gausspoints*3;
            maxdims[0] = this->number_of_gausspoints*3;
            id_elements_gausscoords           = createConstantLengthFloatArray (id_elements_group, rank, dims, maxdims, "Gauss_Point_Coordinates", " ");

            dims[0]    = this->number_of_connectivity_nodes;
            maxdims[0] = this->number_of_connectivity_nodes;
            id_elements_connectivity          = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Connectivity", " ");
           
            // Write a vector with the number of nodes at a given elements tag
            datarank     = 1;
            dims[0]      = (hsize_t) max_element_tag + 1;
            data_dims[0] = (hsize_t) max_element_tag + 1;
            offset[0]    = 0;
            stride[0]    = 1;
            count[0]     = dims[0];
            block[0]     = 1;
            int_data_buffer = Number_of_Nodes.data;
            writeConstantLengthIntegerArray(id_elements_nnodes,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);


            //Write index to connectivity
            dims[0]      = (hsize_t) max_element_tag + 1;
            data_dims[0] = (hsize_t) max_element_tag + 1;
            count[0]     = dims[0];
            int_data_buffer = Index_to_Connectivity.data;
            writeConstantLengthIntegerArray(id_index_to_elements_connectivity,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);


            //Write element connectivity
            dims[0]      = (hsize_t) number_of_connectivity_nodes;
            data_dims[0] = (hsize_t) number_of_connectivity_nodes;
            count[0]     = dims[0];
            int_data_buffer = Connectivity.data;
            writeConstantLengthIntegerArray(id_elements_connectivity,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Write index to number of outputs
            dims[0]      = (hsize_t) max_element_tag + 1;
            data_dims[0] = (hsize_t) max_element_tag + 1;
            count[0]     = dims[0];
            int_data_buffer = Number_of_Output_Fields.data;
            writeConstantLengthIntegerArray(id_elements_noutputs,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Write index to outputs
            dims[0]      = (hsize_t) max_element_tag + 1;
            data_dims[0] = (hsize_t) max_element_tag + 1;
            count[0]     = dims[0];
            int_data_buffer = Index_to_Outputs.data;
            writeConstantLengthIntegerArray(id_index_to_elements_output,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Write material tags
            dims[0]      = (hsize_t) max_element_tag + 1;
            data_dims[0] = (hsize_t) max_element_tag + 1;
            count[0]     = dims[0];
            int_data_buffer = Material_tags.data;
            writeConstantLengthIntegerArray(id_elements_materialtag,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Writing Class tags
            dims[0]      = (hsize_t) Class_Tags.Size();
            data_dims[0] = (hsize_t) Class_Tags.Size();
            count[0]     = dims[0];
            int_data_buffer = Class_Tags.data;
            writeConstantLengthIntegerArray(id_elements_classtag,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            // Creating Element_Class_Tag Description [Sumeet, August, 2016]
            ELE_TAG_DESC_ARRAY;
            int ele_class_desc_size = sizeof(ele_tag_desc_array)/sizeof(int);
            dims[0]      = (hsize_t) ele_class_desc_size;
            maxdims[0]   = (hsize_t) ele_class_desc_size;
            count[0]     = dims[0];
            id_elements_class_desc            = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Element_Class_Desc"," ");

            // Writing Element_Class_Tag Description [Sumeet, August, 2016]
            writeConstantLengthIntegerArray(id_elements_class_desc,
                                            datarank,
                                            dims,
                                            maxdims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            ele_tag_desc_array);

            //Write number of gauss points
            dims[0]      = (hsize_t) max_element_tag+1;
            data_dims[0] = (hsize_t) max_element_tag+1;
            count[0]     = dims[0];
            int_data_buffer = Number_of_Gauss_Points.data;
            writeConstantLengthIntegerArray(id_elements_ngauss,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Write index_to_gauss_coordinate
            dims[0]      = (hsize_t) max_element_tag+1;
            data_dims[0] = (hsize_t) max_element_tag+1;
            count[0]     = dims[0];
            int_data_buffer = Index_to_Gauss_Point_Coordinates.data;
            writeConstantLengthIntegerArray(id_index_to_elements_gausscoords,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

            //Write gauss coordinate values
            dims[0]      = (hsize_t) number_of_gausspoints*3;
            data_dims[0] = (hsize_t) number_of_gausspoints*3;
            count[0]     = dims[0];
            float_data_buffer = (float *) Gauss_Point_Coordinates.theData;
            writeConstantLengthFloatArray(id_elements_gausscoords,
                                           datarank,
                                           dims,
                                           data_dims,
                                           offset,
                                           stride,
                                           count,
                                           block,
                                           float_data_buffer);
            H5OUTPUTWRITER_COUNT_OBJS;

            // =============================================================================================
            //   CREATE OUTPUT ARRAYS!
            // =============================================================================================

            {
                int rank = 2;
                hsize_t dims[2];
                hsize_t maxdims[2];
                dims[0] = (hsize_t) length_nodes_displacements_output;
                dims[1] = 0;
                maxdims[0] = (hsize_t)  length_nodes_displacements_output;
                maxdims[1] = number_of_time_steps + 1;

                id_nodes_displacements = createVariableLengthFloatArray(id_nodes_group, rank, dims, maxdims, "Generalized_Displacements", " ", 1);
                id_nodes_reaction_forces = createVariableLengthFloatArray(id_nodes_group, rank, dims, maxdims, "Generalized_Forces", " ", 1);
            }

            if (number_of_outputs > 0)
            {
                int rank = 2;
                hsize_t dims[2];
                hsize_t maxdims[2];
                dims[0] = (hsize_t) number_of_outputs;
                dims[1] = 0;
                maxdims[0] = (hsize_t)  number_of_outputs;
                maxdims[1] = number_of_time_steps + 1;

                if (flag_write_element_output == 1  )
                {
                    id_elements_output = createVariableLengthFloatArray(id_elements_group, rank, dims, maxdims, "Outputs", " ");
                }

            }//    create_elementOutput_arrays = false;

            //For SP Constraints

            int size ;

            size = SPNodes.Size();
            rank = 1;
            dims[0] = size;
            maxdims[0] = size;
            id_Constrained_Nodes              = createConstantLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Constrained_Nodes", " ");

            size = SPDofs.Size();
            rank = 1;
            dims[0] = size;
            maxdims[0] = size;
            id_Constrained_DOFs               = createConstantLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Constrained_DOFs", " ");

            //Write gauss coordinate values
            dims[0]      = (hsize_t) SPNodes.Size();
            data_dims[0] = (hsize_t) SPNodes.Size();
            count[0]     = dims[0];
            int_data_buffer = SPNodes.data;
            writeConstantLengthIntegerArray(id_Constrained_Nodes,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);
            H5OUTPUTWRITER_COUNT_OBJS;

            //Write gauss coordinate values
            dims[0]      = (hsize_t) SPDofs.Size();
            data_dims[0] = (hsize_t) SPDofs.Size();
            count[0]     = dims[0];
            int_data_buffer = SPDofs.data;
            writeConstantLengthIntegerArray(id_Constrained_DOFs,
                                            datarank,
                                            dims,
                                            data_dims,
                                            offset,
                                            stride,
                                            count,
                                            block,
                                            int_data_buffer);

        }

        // Added by [Sumeet August, 2016]
        // OUtput Only Avialable for ProcessId O 
        #ifdef _PARALLEL_PROCESSING

            if (processID == 0)
            {
                
                
                datarank =1;
                stride[0]=1;
                offset[0]=0;
                block[0] =1;

                dims[0] = max_element_tag+1;
                maxdims[0] = max_element_tag+1;
                id_elements_partition             = createConstantLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Partition", " ");
                
                int_data_buffer = Element_Partition.data;
                writeConstantLengthIntegerArray(id_elements_partition,
                                                datarank,
                                                dims,
                                                maxdims,
                                                offset,
                                                stride,
                                                dims,
                                                block,
                                                int_data_buffer);       

                dims[0] = max_node_tag+1;
                maxdims[0] = max_node_tag+1;
                id_nodes_partition                = createConstantLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Partition", " ");
                
                int_data_buffer = Node_Partition.data;
                writeConstantLengthIntegerArray(id_nodes_partition,
                                                datarank,
                                                dims,
                                                maxdims,
                                                offset,
                                                stride,
                                                dims,
                                                block,
                                                int_data_buffer);           
            }

        #endif


        //================================================================================
        // Material Info
        //================================================================================
        dims[0] = Materials.size();
        maxdims[0] = dims[0];
        id_material = createConstantLengthStringArray(id_model_group, datarank,  dims, maxdims, "Materials", " ");

        //Write material info
        for (int tag = 0; tag < (int) Materials.size(); tag++)
        {
            
            std::string mat_info = Materials[tag];

            writeConstantLengthStringArray(id_material,
                                           tag,
                                           mat_info.size(),
                                           mat_info);
        }

    }
    else  //Could not open file
    {
        cerr << "H5OutputWriter::writeMesh() -> could not open file: " << file_name << "!\n\n";
    }

    H5OUTPUTWRITER_COUNT_OBJS;
}

/*************************************************************************************************
* Added by Sumeet 30th August 2016
* Initializes the Partition ID Vector
*************************************************************************************************/
void H5OutputWriter::initializePartitionData(int max_node_id, int max_element_id){

    Node_Partition.resize(max_node_id + 1);
    Element_Partition.resize(max_element_id + 1);
    Node_Partition[0]=-1;
    Element_Partition[0]=-1;
}

/*************************************************************************************************
* Added by Sumeet 30th August 2016
* Adds partition info to Elements
*************************************************************************************************/
void H5OutputWriter::writeElementPartitionData(int tag  , int partition)
{
    Element_Partition[tag] = partition;
}

/*************************************************************************************************
* Added by Sumeet 30th August 2016
* Adds partition info to Nodes
*************************************************************************************************/
void H5OutputWriter::writeNodePartitionData(int tag  , int partition)
{
    Node_Partition[tag] = partition;
}

// Added by Sumeet August 2016
int H5OutputWriter::writeMaterialMeshData(int tag , std::string material_info )
{

    if (tag < 0)
    {
        cerr << "H5OutputWriter::writeElementMeshData - Error: got tag = " << tag << " < 0" << endl;
    }

    int ntags, addzeros;
    ntags = (int) Materials.size();
    addzeros = tag - ntags;

    //Extend arrays if necessary
    for (int i = 0; i <= addzeros; i++)
    {
        cout << "ntags = " << ntags << " tag = " << tag;
        cout << " H5OutputWriter::writeMaterialMeshData() -- Should not happen!!\n\n";
        Materials.push_back("-1");
    }

    Materials[tag] = material_info;

    return 0;
}

// Results for Nodes
int H5OutputWriter::writeDisplacements(  int nodeTag, const Vector &displacements)
{

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
    int processID;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);

    // cout << setw(5) << nodeTag << " == " << processID << " == " << ": (" << displacements[0] << ", " << displacements[1] << ", " << displacements[2] << ")\n ";
#endif
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

    hsize_t dims[0];
    dims[1] = (hsize_t) ndofs;  
    data_dims[0] = (hsize_t) ndofs;
    offset[0]    = (hsize_t) pos;
    offset[1]    = (hsize_t) current_time_step-1;
    stride[0]    = 1;
    stride[1]    = 1;
    count[0]     = (hsize_t) ndofs;
    count[1]     = 1;
    block[0]     = 1;
    block[1]     = 1;

#if _PARALLEL_PROCESSING_COLLECTIVE_IO
    // cout << "   pos = " << pos << " step = " << current_time_step << " ndofs = " << ndofs << endl;
#endif


    float *data = (float *) displacements.theData;
    writeConstantLengthFloatArray(id_nodes_displacements,
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

/************************************************************************************************************
* Added by Sumeet 3rd August, 2016
* It outputs the trial substep iteration results for nodes.
**************************************************************************************************************/

int H5OutputWriter::writeTrialDisplacements(  int nodeTag, const Vector &displacements)
{

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
    int processID;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);

    // cout << setw(5) << nodeTag << " == " << processID << " == " << ": (" << displacements[0] << ", " << displacements[1] << ", " << displacements[2] << ")\n ";
#endif
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


    hsize_t dims[0];
    dims[1] = (hsize_t) ndofs;  
    data_dims[0] = (hsize_t) ndofs;
    offset[0]    = (hsize_t) pos;
    offset[1]    = (hsize_t) current_sub_step - 1;
    stride[0]    = 1;
    stride[1]    = 1;
    count[0]     = (hsize_t) ndofs;
    count[1]     = 1;
    block[0]     = 1;
    block[1]     = 1;

#if _PARALLEL_PROCESSING_COLLECTIVE_IO
    // cout << "   pos = " << pos << " step = " << current_time_step << " ndofs = " << ndofs << endl;
#endif

    // ///////////////////////// Fotr Debugging Sumeet 1st August, 2016 ////////////////////////
    // cout << "offset[0] " << offset[0] <<endl;
    // cout << "offset[1] " << offset[1] << endl;
    // cout << "stride[0] " << stride[0] <<endl;
    // cout << "stride[1] " << stride[1] << endl;
    // cout << " count[0] " <<  count[0] <<endl;
    // cout << " count[1] " <<  count[1] << endl;   
    // cout << "-------------------------------------------------" <<endl; 
    // /////////////////////////////////////////////////////////////////////////////////////////


    float *data = (float *) displacements.theData;
    writeConstantLengthFloatArray(id_trial_nodes_displacements,
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

// Results for Nodes
int H5OutputWriter::writeDummyDisplacements(  )
{


    // Read NDOFS from HDF5 file
    int datarank         = 1;
    hsize_t dummy[1] = {1};

    float *zerodata = 0;
    writeVariableLengthFloatArray(id_nodes_displacements,
                                   datarank,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   zerodata);

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

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
    int processID;
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);

    // cout << setw(5) << nodeTag << " == " << processID << " == " << ": (" << displacements[0] << ", " << displacements[1] << ", " << displacements[2] << ")\n ";
#endif
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
    // hsize_t dims[2]      =  { (hsize_t)  length_nodes_displacements_output, (hsize_t)  current_time_step};
    hsize_t *dims;
    dims = 0;   // This is to disable array extension (would not be done collectively)
    data_dims[0] = (hsize_t) ndofs;

    offset[0]    = (hsize_t) pos;
    offset[1]    = (hsize_t) current_time_step-1;
    stride[0]    = 1;
    stride[1]    = 1;
    count[0]     = (hsize_t) ndofs;
    count[1]     = 1;
    block[0]     = 1;
    block[1]     = 1;

#if _PARALLEL_PROCESSING_COLLECTIVE_IO
    // cout << "   pos = " << pos << " step = " << current_time_step << " ndofs = " << ndofs << endl;
#endif


    float *data = (float *) reactionForces.theData;
    writeVariableLengthFloatArray(id_nodes_reaction_forces,
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
    return 0;
}



// Results for Elements
int H5OutputWriter::writeElementOutput(int elementTag, const  Vector &output)
{

    if (length_element_output > 0) // If there is nothing to output, there is nothing to output
    {
        int pos, noutputs;

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

        hsize_t dims[0];
        dims[1] = (hsize_t) noutputs;  
        data_dims[0] = (hsize_t) noutputs;
        offset[0]    = (hsize_t) pos;
        offset[1]    = (hsize_t) current_time_step - 1;
        stride[0]    = 1;
        stride[1]    = 1;
        count[0]     = (hsize_t) noutputs;
        count[1]     = 1;
        block[0]     = 1;
        block[1]     = 1;
        float *data = (float *) output.theData;
        writeConstantLengthFloatArray(id_elements_output,
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

/************************************************************************************************************
* Added by Sumeet 3rd August, 2016
* It outputs the trial substep iteration results for elements output.
**************************************************************************************************************/
// Results for Elements
int H5OutputWriter::writeTrialElementOutput(int elementTag, const  Vector &output)
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


        hsize_t dims[1];
        dims[0] = (hsize_t) noutputs;  
        data_dims[0] = (hsize_t) noutputs;
        offset[0]    = (hsize_t) pos;
        offset[1]    = (hsize_t) current_sub_step - 1;
        stride[0]    = 1;
        stride[1]    = 1;
        count[0]     = (hsize_t) noutputs;
        count[1]     = 1;
        block[0]     = 1;
        block[1]     = 1;
        float *data = (float *) output.theData;
        writeConstantLengthFloatArray(id_trial_elements_output,
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



// Results for Elements
int H5OutputWriter::writeDummyElementOutput()
{
// Read NDOFS from HDF5 file
    int datarank         = 1;
    hsize_t dummy[1] = {1};

    float *zerodata = 0;//displacements.theData;
    writeVariableLengthFloatArray(id_elements_output,
                                   datarank,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   dummy,
                                   zerodata);

    H5OUTPUTWRITER_COUNT_OBJS;
    return 0;
}






int H5OutputWriter::setTime(float t)
{

    if (call_hdf5_flush)
    {
        H5Fflush(id_file, H5F_SCOPE_GLOBAL);

        cout << "H5OutputWriter: H5Flush has been called!\n";
        cout << " *****************************************************************\n";
        cout << " *****************************************************************\n";
        cout << " ESSI has stopped simulating and is waiting for you to continue. \n";
        cout << " You can safely explore the current output file: \n";
        cout << "   " << file_name << endl;
        cout << " *****************************************************************\n";
        cout << " Press any key to continue. \n";
        std::cin.ignore();

        call_hdf5_flush = false;
    }

    current_time = t;

    hsize_t dims[1]      = {(hsize_t) (number_of_time_steps+1)};
    hsize_t data_dims[1] = {1};
    hsize_t offset[1]    = {(hsize_t) current_time_step };
    hsize_t stride[1]    = {1};
    hsize_t count[1]     = {1};
    hsize_t block[1]     = {1};

    writeConstantLengthFloatArray(id_time_vector,
                                   1, //datarank
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   &current_time);

    current_time_step++;

    //===============================================================================
    // Writing Number of time steps  
    //===============================================================================
    dims[0]=1;
    offset[0]=0;
    writeConstantLengthIntegerArray(id_number_of_time_steps, 1, dims, data_dims, offset, stride, count, block,
                                &current_time_step);

    // ===============================================================================
    // Extending the output dataset (By Sumeet)
    // ===============================================================================

    // cout << "current_time_step " << current_time_step << endl;

    hsize_t      size[2];
    size[0] = (hsize_t) number_of_outputs;
    size[1] = (hsize_t) current_time_step;

    status = H5Dset_extent (id_elements_output, size);


    size[0] = (hsize_t) length_nodes_displacements_output;
    size[1] = (hsize_t) current_time_step;

    status = H5Dset_extent (id_nodes_displacements, size);
    status = H5Dset_extent (id_nodes_reaction_forces, size);

    // /**********************************************************************************/

    H5OUTPUTWRITER_COUNT_OBJS;

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
#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(id_data_string,
                      id_type_string,
                      H5S_ALL,
                      H5S_ALL,
                      dataset_xfer_plist,
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




hid_t H5OutputWriter::createVariableLengthFloatArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute,
        int timedimension)
{
    hid_t id_array;
    float fill_value = 0.0;
    id_array = createVariableLengthArray(here,
                                         rank,
                                         dims,
                                         maxdims,
                                         name,
                                         attribute,
                                         H5T_NATIVE_FLOAT,
                                         sizeof(float),
                                         &fill_value,
                                         timedimension);

    return id_array;
}

hid_t H5OutputWriter::createVariableLengthIntegerArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute,
        int timedimension)
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
                                         &fill_value,
                                         timedimension);

    return id_array;
}




hid_t H5OutputWriter::createConstantLengthStringArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{


#ifdef HDF5_CREATE_CHECK
    cout << "HDF5 array var-length (string) create: " << name << "\n";
#endif

    hid_t id_array;

    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank, dims, maxdims);       // create dataspace

    /* create a datatype for the text  */ 
    hid_t type = H5Tcopy(H5T_C_S1);

    /* set the total size for the datatype*/
    status = H5Tset_size(type, H5T_VARIABLE);

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    hsize_t chunk_dims[1] = {dims[0]};

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

    hdf5_check_error(id_array);

    // setAttribute(id_array, attribute);

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
        hid_t type, int sizeof_type, void *fill_value_ptr, int timedimension)
{
    // int fill_value = 0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible

    hsize_t chunk_dims[rank];
    int nbytes_one_chunk = sizeof_type;// sizeof(int);
    for (int i = 0; i < rank; i++)
    {

        chunk_dims[i] = std::min(maxdims[i], static_cast<hsize_t>(H5OUTPUTWRITER_CHUNK_MAXDIMENSION));

        nbytes_one_chunk *= chunk_dims[i];
    }

    // The idea is to chunk less in the time dimension.
    if (timedimension >= 0)
    {
        nbytes_one_chunk /= chunk_dims[timedimension];
        chunk_dims[timedimension] = std::min(maxdims[timedimension], static_cast<hsize_t>(H5OUTPUTWRITER_CHUNK_TIMEDIM));
        nbytes_one_chunk *= chunk_dims[timedimension];
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

    if (zlib_compression_level > 0)
    {
        status = H5Pset_deflate (dataset_creation_plist, zlib_compression_level );
        hdf5_check_error(status);
    }


#ifdef HDF5_CREATE_CHECK
    cout << "HDF5 array var-length << ";
    if (type == H5T_NATIVE_FLOAT)
    {
        cout << "(float)";
    }
    else if (type == H5T_NATIVE_INT)
    {
        cout << "(int)";
    }
    cout << " create: " << name << "\n";
    cout << "    rank = " << rank << "\n";
    for (int i = 0; i < rank; i++)
    {
        cout << "          dims[" << i << "] = " << dims[i] << "\n";
    }
    for (int i = 0; i < rank; i++)
    {
        cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
    }
    for (int i = 0; i < rank; i++)
    {
        cout << "    chunk_dims[" << i << "] = " << chunk_dims[i] << "\n";
    }
#endif

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
    // hdf5_check_error(status);
    setAttribute(id_array, attribute);

    H5Sclose(id_dataspace);
    H5OUTPUTWRITER_COUNT_OBJS;

    return id_array;
}


hid_t H5OutputWriter::createConstantLengthFloatArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string attribute)
{

#ifdef HDF5_CREATE_CHECK
    cout << "HDF5 array const-length (float) create: " << name << "\n";
    cout << "    rank = " << rank << "\n";
    for (int i = 0; i < rank; i++)
    {
        cout << "          dims[" << i << "] = " << dims[i] << "\n";
    }
    for (int i = 0; i < rank; i++)
    {
        cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
    }
#endif
    float fill_value = 0.0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);


    status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_FLOAT, &fill_value);
    hdf5_check_error(status);


    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank , dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                H5T_NATIVE_FLOAT,
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
#ifdef HDF5_CREATE_CHECK
    cout << "HDF5 array const-length (int) create: " << name << "\n";
    cout << "    rank = " << rank << "\n";
    for (int i = 0; i < rank; i++)
    {
        cout << "          dims[" << i << "] = " << dims[i] << "\n";
    }
    for (int i = 0; i < rank; i++)
    {
        cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
    }

#endif
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

hid_t H5OutputWriter::writeVariableLengthFloatArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        float *data)
{
    // Extend it if necesary!
    if (dims != 0 && data != 0)
    {
        status =  H5Dset_extent( id_array, dims ); // Needs to be avoided all the time [sumeet August, 2016]
        //extended collectively in setTime() function
        cout << "Warning !!! I am in wronh place. Please Fix Me" << endl;
        hdf5_check_error(status);
    }

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = 0;
    hsize_t id_memspace = 0;

    id_dataspace = H5Dget_space(id_array);
    id_memspace = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

    if (data != 0)
    {
        //Select the region of data to output to
        status = H5Sselect_hyperslab(
                     id_dataspace,          // Id of the parent dataspace
                     H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
                     offset,                // start of selection
                     stride,                // stride in each dimension, NULL  is select everything
                     count ,                // how many blocks to select in each direction
                     block                  // little block selected per selection
                 );
    }
    else
    {
        status = H5Sselect_none(id_dataspace);
        status = H5Sselect_none(id_memspace);
    }
    hdf5_check_error(status);

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
                 data                   // The actual data
             );

    H5D_mpio_actual_io_mode_t actual_io_mode;
    H5Pget_mpio_actual_io_mode( dataset_xfer_plist, &actual_io_mode);

    switch (actual_io_mode)
    {

    case H5D_MPIO_NO_COLLECTIVE: //     No collective I/O was performed. Collective I/O was not requested or collective I/O isn't possible on this dataset.
        numof_NO_COLLECTIVE_calls++;
        break;
    case H5D_MPIO_CHUNK_INDEPENDENT:        // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed independently.
        numof_CHUNK_INDEPENDENT_calls++;
        break;
    case H5D_MPIO_CHUNK_COLLECTIVE:     // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed collectively.
        numof_CHUNK_COLLECTIVE_calls++;
        break;
    case H5D_MPIO_CHUNK_MIXED:            // HDF5 performed one the chunk collective optimization schemes and some chunks were accessed independently, some collectively.
        numof_CHUNK_MIXED_calls++;
        break;
    }
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif
//------------------------

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}

/****************************************************************************
* Added by Sumeet 1st Augustt, 2016 
* It takes a matrix in the form of 1-D array and converts in matrix and 
* write it to output file
*****************************************************************************/
hid_t H5OutputWriter::writeConstantLengthFloatMatrix(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        float* matrix_data_in_linear_fashion)
{

    float data[6][3];
    for(int i=0; i < (int) count[1]; i++) {
        for(int j=0; j < (int) count[0]; j++) {
            data[i][j] = matrix_data_in_linear_fashion[j*count[0]+i];
        }
    }

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = 0;
    hsize_t id_memspace = 0;

    id_dataspace = H5Dget_space(id_array); VERIFY(id_dataspace);
    id_memspace = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

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

#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
                 data                   // The actual data
             );

    H5D_mpio_actual_io_mode_t actual_io_mode;
    H5Pget_mpio_actual_io_mode( dataset_xfer_plist, &actual_io_mode);

    switch (actual_io_mode)
    {

    case H5D_MPIO_NO_COLLECTIVE: //     No collective I/O was performed. Collective I/O was not requested or collective I/O isn't possible on this dataset.
        numof_NO_COLLECTIVE_calls++;
        break;
    case H5D_MPIO_CHUNK_INDEPENDENT:        // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed independently.
        numof_CHUNK_INDEPENDENT_calls++;
        break;
    case H5D_MPIO_CHUNK_COLLECTIVE:     // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed collectively.
        numof_CHUNK_COLLECTIVE_calls++;
        break;
    case H5D_MPIO_CHUNK_MIXED:            // HDF5 performed one the chunk collective optimization schemes and some chunks were accessed independently, some collectively.
        numof_CHUNK_MIXED_calls++;
        break;
    }
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif
//------------------------

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
#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
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

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}



hid_t H5OutputWriter::writeConstantLengthStringArray(hid_t id_array,
        hsize_t offset_,
        hsize_t datasize,
        std::string &data)
{

    int datarank = 1;
    hsize_t data_dims[1] = {1};
    hsize_t stride[1] = {1};
    hsize_t count[1] = {1};
    hsize_t block[1] = {1};
    hsize_t offset[1] = {offset_};

    //Get pointer to the dataspace and create the memory space
    hsize_t id_dataspace = H5Dget_space(id_array);
    hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create memspace

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
#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 type,                  // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
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

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}




hid_t H5OutputWriter::writeConstantLengthFloatArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        float *data)
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
#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
                 data                   // The actual data
             );
#else

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_FLOAT,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
#endif

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
#ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 dataset_xfer_plist,           // Form of writing
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

    hdf5_check_error(status);

    //Close stuff
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);
    H5OUTPUTWRITER_COUNT_OBJS;
    return id_array;
}

void H5OutputWriter::set_zlib_compression_level(int level)
{
    zlib_compression_level = level;
}


void call_hdf5_flush_next_chance()
{
    H5OutputWriter::call_hdf5_flush = true;
}


inline void hdf5_check_error(herr_t status)
{
    if (status < 0)
    {
        cout << "status = " << status << endl;
        print_stacktrace();
        exit(-1);
    }
}

/**********************************************************************************
* Added by Sumeet 1st August, 2016
* Can be used to reserve space for other datasets as well
***********************************************************************************/
int H5OutputWriter::reserveSpaceForDatasets(unsigned int number_of_materials){

    Materials.resize(number_of_materials + 1);

    for (unsigned int i = 0; i < number_of_materials+1; i++)
    {
        Materials[i] = "0";
    }

    return 0;


}


/**********************************************************************************
* Added by Sumeet 1st August, 2016
* Creates datasets for eigen analysis outputs 
***********************************************************************************/
int H5OutputWriter::writeEigenMesh (int number_of_modes){

    this->number_of_eigen_modes = number_of_modes;

    if (id_file > 0)
    {

        //================================================================================
        // Creating Eigen Value Analysis Group
        //================================================================================

        id_eigen_analysis_group = create_group(id_file, "Eigen_Mode_Analysis");

        //================================================================================
        // Create Eigen Vaule Analysis Datasets
        //================================================================================

        hsize_t dims[1], maxdims[1];
        int rank = 1;
        dims[0] = number_of_modes;
        maxdims[0] = number_of_modes;

        id_eigen_values      = createConstantLengthFloatArray(id_eigen_analysis_group, rank, dims, maxdims, "values", " ");
        id_eigen_frequencies = createConstantLengthFloatArray(id_eigen_analysis_group, rank, dims, maxdims, "frequencies", " ");
        id_eigen_periods     = createConstantLengthFloatArray(id_eigen_analysis_group, rank, dims, maxdims, "periods", " ");

        {
            int rank = 2;
            hsize_t dims[2];
            hsize_t maxdims[2];
            dims[0] = (hsize_t) number_of_dofs;
            dims[1] = number_of_modes;
            maxdims[0] = (hsize_t)  number_of_dofs;
            maxdims[1] = number_of_modes;

            id_eigen_modes = createConstantLengthFloatArray(id_eigen_analysis_group, rank, dims, maxdims, "modes", " ");
        }

        dims[0] = 1;
        maxdims[0] = 1;
        id_number_of_modes = createConstantLengthIntegerArray(id_eigen_analysis_group, rank, dims, maxdims, "number_of_modes", " ");

        dims[0]     = 1;
        hsize_t data_dims[1] = {1};
        hsize_t offset[1]    = {0};
        hsize_t stride[1]    = {1};
        hsize_t count[1]     = {1};
        hsize_t block[1]     = {1};

        writeConstantLengthIntegerArray(id_number_of_modes, rank, dims, data_dims, offset, stride, count, block,
                                        &number_of_eigen_modes);

    }

    H5OUTPUTWRITER_COUNT_OBJS;  

    return 0;
}

/**********************************************************************************
* Added by Sumeet 1st August, 2016
* Writes eigen modes
***********************************************************************************/
int H5OutputWriter::writeEigenModes( int nodeTag, const Matrix &displacements){

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

    datarank =2;
    hsize_t *dims;
    hsize_t data_2dims[2];
    dims = 0;                           // This is to disable array extension (would not be done collectively)
    data_2dims[0] = (hsize_t) ndofs;
    data_2dims[1] = (hsize_t) this->number_of_eigen_modes;

    offset[0]    = (hsize_t) pos;
    offset[1]    = 0;
    stride[0]    = 1;
    stride[1]    = 1;
    count[0]     = (hsize_t) ndofs;
    count[1]     = (hsize_t) this->number_of_eigen_modes;
    block[0]     = 1;
    block[1]     = 1;

    // ///////////////////////// Fotr Debugging Sumeet 1st August, 2016 ////////////////////////
    // cout << "offset[0] " << offset[0] <<endl;
    // cout << "offset[1] " << offset[1] << endl;
    // cout << "stride[0] " << stride[0] <<endl;
    // cout << "stride[1] " << stride[1] << endl;
    // cout << " count[0] " <<  count[0] <<endl;
    // cout << " count[1] " <<  count[1] << endl;   
    // cout << "-------------------------------------------------" <<endl; 
    // /////////////////////////////////////////////////////////////////////////////////////////

    float *data = (float *) displacements.data;
    writeConstantLengthFloatMatrix(id_eigen_modes,
                                   datarank,
                                   dims,
                                   data_2dims,
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


/**********************************************************************************
* Added by Sumeet 1st August, 2016
* writes eigen period, frequencies and values 
***********************************************************************************/
int H5OutputWriter::writeEigen_Value_Frequency_Period ( const Vector & eigenvalues, Vector periodvalues, Vector frequencyvalues){

    int datarank         = 1;
    hsize_t *dims; dims=0;
    hsize_t data_dims[1] = {(hsize_t) this->number_of_eigen_modes};
    hsize_t offset[2]    = {0, 0};
    hsize_t stride[2]    = {1, 0};
    hsize_t count[2]     = {(hsize_t) this->number_of_eigen_modes, 0};
    hsize_t block[2]     = {1, 0};

    writeConstantLengthFloatArray(id_eigen_values,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   (float *)periodvalues.theData);

    writeConstantLengthFloatArray(id_eigen_frequencies,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   (float *)frequencyvalues.theData);

    writeConstantLengthFloatArray(id_eigen_periods,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   (float *)eigenvalues.theData);

    H5OUTPUTWRITER_COUNT_OBJS;

    return 0; 

}

/********************************************************************************************
* Added by sumeet 3rd August, 2016 
* This function is used to create the substep outputmesh
*********************************************************************************************/
int H5OutputWriter::writeSubstepMesh(int number_of_substeps){
    
    if (id_file > 0)
    {
        int rank = 2;
        hsize_t dims[2];
        hsize_t maxdims[2];
        dims[0] = (hsize_t) number_of_dofs;
        dims[1] = 0;
        maxdims[0] = (hsize_t)  number_of_dofs;
        maxdims[1] = number_of_substeps;

        id_trial_nodes_displacements = createVariableLengthFloatArray(id_nodes_group, rank, dims, maxdims, "Substep_Generalized_Displacements", " ");

        dims[0] = (hsize_t) number_of_outputs;
        maxdims[0] = (hsize_t) number_of_outputs;

        id_trial_elements_output     = createVariableLengthFloatArray(id_elements_group, rank, dims, maxdims, "Substep_Outputs", " ");

        this->current_sub_step = 0;
    }

    H5OUTPUTWRITER_COUNT_OBJS;  

    return 0;

}

/********************************************************************************************
* Added by sumeet 3rd August, 2016 
* Basically saves the current sub_step_number i.e the column index of HDF5 output file.
* and also extends the dimension of the dataset
*********************************************************************************************/
int H5OutputWriter::setSubStep(int substep_no){

    cout << "Writing Substep " <<"[" << substep_no << "]" << " ....................."; 

    this->current_sub_step = substep_no ;

    hsize_t      size[2];
    size[0] = (hsize_t) number_of_outputs;
    size[1] = (hsize_t) current_sub_step;

    status = H5Dset_extent (id_trial_elements_output, size);


    size[0] = (hsize_t) number_of_dofs;
    size[1] = (hsize_t) current_sub_step;

    status = H5Dset_extent (id_trial_nodes_displacements, size);

    return 0;
}

