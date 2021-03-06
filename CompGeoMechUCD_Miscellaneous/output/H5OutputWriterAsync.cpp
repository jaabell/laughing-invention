// ///////////////////////////////////////////////////////////////////////////////
// //
// // COPYLEFT (C):     :-))
// //``This  source code is Copyrighted in U.S., by the The Regents of the University
// //of California, for an indefinite period, and anybody caught using it without our
// //permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
// //Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
// //all we wanted to do.'' bj
// // PROJECT:           Object Oriented Finite Element Program
// // FILE:              H5Output:ter.cpp
// // CLASS:             H5OutputWriter
// // MEMBER FUNCTIONS:
// //
// // MEMBER VARIABLES
// //
// // PURPOSE:           Is an OutputWriter that writes into HDF5 format
// // RETURN:
// // VERSION:
// // LANGUAGE:          C++
// // TARGET OS:         DOS || UNIX || . . .
// // DESIGNER:          Jose Abell, Boris Jeremic
// // PROGRAMMER:        Jose Abell
// // DATE:              Mon 05 May 2014 10:09:35 AM PDT
// // UPDATE HISTORY:  Some changes made by Babak 5/31/14 to make it work with parallel ...
// //
// //
// ///////////////////////////////////////////////////////////////////////////////

// #include <iostream>
// #include <string>
// #include <sstream>
// #include <limits>
// #include <stacktrace.h>

// #include <ID.h>
// #include <Vector.h>
// #include <H5OutputWriterAsync.h>
// #include <hdf5.h>
// #include <time.h>

// #ifdef _PARALLEL_PROCESSING
// #include <mpi.h>
// #endif

// # include <ESSITimer.h>

// // Yuan. Tue Jul 19 16:14:23 PDT 2016
// #include <thread>
// #include "tbb/tbb.h"
// #include <atomic>
// bool H5OutputWriter::call_hdf5_flush = false;

// H5OutputWriter::H5OutputWriter():
//     OutputWriter(),
//     file_is_open(false),
//     number_of_nodes(0),
//     number_of_elements(0),
//     number_of_time_steps(0),
//     max_node_tag(0),
//     max_element_tag(0),
//     number_of_dofs(0),
//     number_of_outputs(0),
//     file_name(""),
//     model_name(""),
//     stage_name(""),
//     numof_NO_COLLECTIVE_calls(0),
//     numof_CHUNK_INDEPENDENT_calls(0),
//     numof_CHUNK_COLLECTIVE_calls(0),
//     numof_CHUNK_MIXED_calls(0)
// {
//     current_time                         = 0.0;
//     current_time_step                    = 0;
//     number_of_nodes                      = 0;
//     number_of_elements                   = 0;
//     max_node_tag                         = 0;
//     max_element_tag                      = 0;

//     length_nodes_displacements_output    = 0;
//     length_nodes_velocities_output       = 0;
//     length_nodes_accelerations_output    = 0;
//     length_nodes_reaction_forcess_output = 0;
//     length_element_output                = 0;
//     pos_nodes_outputs                    = 0;
//     pos_nodes_coordinates                = 0;
//     pos_elements_outputs                 = 0;
//     pos_elements_gausscoords             = 0;
//     pos_elements_connectivity            = 0;

//     create_nodeMeshData_arrays           = true;
//     create_nodeDisplacements_arrays      = true;
//     create_nodeVelocities_arrays         = true;
//     create_nodeAccelerations_arrays      = true;
//     create_nodeReactionForces_arrays     = true;
//     create_elementMeshData_arrays        = true;
//     create_elementOutput_arrays          = true;

//     zlib_compression_level = 5;
//     flag_write_element_output            = 1;

// #ifndef _PARALLEL_PROCESSING_COLLECTIVE_IO
//     dataset_xfer_plist = H5Pcreate(H5P_DATASET_XFER);  //Otherwise created in syncWriters in the parallel case
// #endif

//     // ===============================
//     // Yuan
//     // number_of_elements and number_of_nodes
//     // After the two variables are assigned, we can start the thread to write to disk . 
//     // Be careful: In multi-threading, we will have two instance of H5OutputWriter. So the desructor will be called twice. Keep this in mind.
//     buffer_ready.store(false);
//     async_writer = thread([=]{
//         while(true){
//             if(buffer_ready.load()){
//                 writeDisplacements2disk();
//                 writeElementOutput2disk();
//                 break;
//             }
//         }
//     });
//     // async_writer.detach();
// }


// H5OutputWriter::H5OutputWriter(std::string filename_in,
//                                std::string model_name_in,
//                                std::string stage_name_in,
//                                int nsteps):
//     OutputWriter(),
//     file_is_open(false),
//     number_of_nodes(0),
//     number_of_elements(0),
//     number_of_time_steps(0),
//     max_node_tag(0),
//     max_element_tag(0),
//     number_of_dofs(0),
//     number_of_outputs(0),
//     file_name(""),
//     model_name(""),
//     stage_name(""),
//     numof_NO_COLLECTIVE_calls(0),
//     numof_CHUNK_INDEPENDENT_calls(0),
//     numof_CHUNK_COLLECTIVE_calls(0),
//     numof_CHUNK_MIXED_calls(0)
// {
//     current_time                         = 0.0;
//     current_time_step                    = 0;
//     number_of_nodes                      = 0;
//     number_of_elements                   = 0;
//     max_node_tag                         = 0;
//     max_element_tag                      = 0;

//     length_nodes_displacements_output    = 0;
//     length_nodes_velocities_output       = 0;
//     length_nodes_accelerations_output    = 0;
//     length_nodes_reaction_forcess_output = 0;
//     length_element_output                = 0;
//     pos_nodes_outputs                    = 0;
//     pos_nodes_coordinates                = 0;
//     pos_elements_outputs                 = 0;
//     pos_elements_gausscoords             = 0;
//     pos_elements_connectivity            = 0;

//     current_time = 0.0;

//     create_nodeMeshData_arrays           = true;
//     create_nodeDisplacements_arrays      = true;
//     create_nodeVelocities_arrays         = true;
//     create_nodeAccelerations_arrays      = true;
//     create_nodeReactionForces_arrays     = true;
//     create_elementMeshData_arrays        = true;
//     create_elementOutput_arrays          = true;
//     initialize(filename_in,
//                model_name_in,
//                stage_name_in,
//                nsteps);

//     zlib_compression_level = 5;
//     flag_write_element_output = 1;

// #ifndef _PARALLEL_PROCESSING_COLLECTIVE_IO
//     dataset_xfer_plist = H5Pcreate(H5P_DATASET_XFER); //Otherwise created in syncWriters in the parallel case
// #endif
// }



// H5OutputWriter::~H5OutputWriter()
// {
//     finalize();
// }


// void H5OutputWriter::finalize()
// {
//     // cout << "H5OutputWriter::finalize() - ";
//     if (file_is_open)
//     {    // // // Yuan: wait for the thread async_writer to finish
//         // fprintf(stderr, "wait for the thread async_writer to finish\n" );
//         async_writer.join();
//         // fprintf(stderr, "after wait for the thread async_writer to finish\n" );
//         // cout << "endingtime \n\n";
//         time_t current ;
//         time(&current);
//         char *timestring;
//         timestring = ctime(&current);
//         timestring[strlen(timestring) - 1] = '\0';
//         write_string(id_file, "Date_and_Time_End", timestring);


//         //Close all objects
//         // H5F_OBJ_FILE    Files only
//         // H5F_OBJ_DATASET Datasets only
//         // H5F_OBJ_GROUP   Groups only
//         // H5F_OBJ_DATATYPE    Named datatypes only
//         // H5F_OBJ_ATTR    Attributes only
//         // H5F_OBJ_ALL All of the above
//         // (That is, H5F_OBJ_FILE | H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_DATATYPE | H5F_OBJ_ATTR)
//         // H5F_OBJ_LOCAL   Restrict search to objects opened through current file identifier.
//         // Note: H5F_OBJ_LOCAL does not stand alone; it is effective only when used in combination with one or more of the preceding types. For example,
//         //     H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_LOCAL
//         // would count all datasets and groups opened through the current file identifier.

//         hid_t obj_id_list[H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS];
//         hsize_t n_obj_open = 10;
//         while (n_obj_open > 0)
//         {
//             n_obj_open = H5Fget_obj_count(id_file, H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_ATTR | H5F_OBJ_LOCAL );
//             // cout << "H5OutputWriter -- N of HDF5 objects open = " << n_obj_open << endl;


//             if (n_obj_open <= 0)
//             {
//                 break;
//             }


//             int number_of_open_objects;

//             //Close datasets
//             number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_DATASET | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
//             for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
//             {
//                 H5Dclose(obj_id_list[i]);
//             }

//             //Close groups
//             number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_GROUP | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
//             for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
//             {
//                 H5Gclose(obj_id_list[i]);
//             }

//             //Close groups
//             number_of_open_objects = H5Fget_obj_ids( id_file, H5F_OBJ_ATTR | H5F_OBJ_LOCAL, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS, obj_id_list );
//             for (int i = 0; i < std::min(number_of_open_objects, H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS) ; i++ )
//             {
//                 H5Aclose(obj_id_list[i]);
//             }

//         }
//         //Finally close file
//         file_is_open = false;
//         H5Fclose(id_file);

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
//         int processID;
//         MPI_Comm_rank(MPI_COMM_WORLD, &processID);


//         int total_write_calls = numof_NO_COLLECTIVE_calls +
//                                 numof_CHUNK_INDEPENDENT_calls +
//                                 numof_CHUNK_COLLECTIVE_calls +
//                                 numof_CHUNK_MIXED_calls;

//         if (total_write_calls > 0)
//         {
//             cout << "Finalized HDF5 writer on process " << processID << " \n";
//             cout << " Summary of HDF5 I/O mode statistics:\n";
//             cout << "   NO_COLLECTIVE     = " << numof_NO_COLLECTIVE_calls     << " (" << 100 * double(numof_NO_COLLECTIVE_calls) / double(total_write_calls)     << "%%)\n";
//             cout << "   CHUNK_INDEPENDENT = " << numof_CHUNK_INDEPENDENT_calls << " (" << 100 * double(numof_CHUNK_INDEPENDENT_calls) / double(total_write_calls) << "%%)\n";
//             cout << "   CHUNK_COLLECTIVE  = " << numof_CHUNK_COLLECTIVE_calls  << " (" << 100 * double(numof_CHUNK_COLLECTIVE_calls) / double(total_write_calls)  << "%%)\n";
//             cout << "   CHUNK_MIXED       = " << numof_CHUNK_MIXED_calls       << " (" << 100 * double(numof_CHUNK_MIXED_calls) / double(total_write_calls)       << "%%)\n";
//             cout << "   TOTAL             = " << total_write_calls             << endl;
//         }
// #endif

//     }
//     else
//     {
//         // cout << "nothing to do... " << endl;
//     }

// }


// void H5OutputWriter::initialize(std::string filename_in,
//                                 std::string model_name_in,
//                                 std::string stage_name_in,
//                                 int nsteps)
// {


//     if (previous_stage_name.compare("!!none") == 0)
//     {
//         cout << "changing previous_stage_name from " << previous_stage_name << " to " << stage_name
//              << endl;
//         previous_stage_name = stage_name;
//     }
//     file_name = "";
//     model_name = "";
//     stage_name = "";
//     file_name += filename_in;
//     model_name += model_name_in;
//     stage_name += stage_name_in;

//     current_time                         = 0.0;
//     current_time_step                    = 0;
//     number_of_nodes                      = 0;
//     number_of_elements                   = 0;
//     max_node_tag                         = 0;
//     max_element_tag                      = 0;

//     length_nodes_displacements_output    = 0;
//     length_nodes_velocities_output       = 0;
//     length_nodes_accelerations_output    = 0;
//     length_nodes_reaction_forcess_output = 0;
//     length_element_output                = 0;

//     pos_nodes_outputs                    = 0;
//     pos_nodes_coordinates                = 0;
//     pos_elements_outputs                 = 0;
//     pos_elements_gausscoords             = 0;
//     pos_elements_connectivity            = 0;

//     create_nodeMeshData_arrays           = true;
//     create_nodeDisplacements_arrays      = true;
//     create_nodeVelocities_arrays         = true;
//     create_nodeAccelerations_arrays      = true;
//     create_nodeReactionForces_arrays     = true;
//     create_elementMeshData_arrays        = true;
//     create_elementOutput_arrays          = true;

//     number_of_time_steps                 = nsteps;

//     numof_NO_COLLECTIVE_calls = 0;
//     numof_CHUNK_INDEPENDENT_calls = 0;
//     numof_CHUNK_COLLECTIVE_calls = 0;
//     numof_CHUNK_MIXED_calls = 0;

// }





// void H5OutputWriter::set_number_of_time_steps(int nsteps)
// {
// #ifdef _PARALLEL_PROCESSING
//     int rank = 0;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// #endif

//     number_of_time_steps = nsteps;
// }








// // Mesh output


// int H5OutputWriter::writeGlobalMeshData(unsigned int number_of_nodes_in,
//                                         unsigned int number_of_elements_in,
//                                         unsigned int max_node_tag_in,
//                                         unsigned int max_element_tag_in,
//                                         unsigned int number_of_dofs_in,
//                                         unsigned int number_of_outputs_in)
// {
//     number_of_nodes = number_of_nodes_in;
//     number_of_elements = number_of_elements_in;
//     max_node_tag = max_node_tag_in;
//     max_element_tag = max_element_tag_in;
//     number_of_dofs = number_of_dofs_in;
//     number_of_outputs = number_of_outputs_in;


//     // cout << "number_of_nodes    = " << number_of_nodes    << endl;
//     // cout << "number_of_elements = " << number_of_elements << endl;
//     // cout << "max_node_tag       = " << max_node_tag       << endl;
//     // cout << "max_element_tag    = " << max_element_tag    << endl;
//     // cout << "number_of_dofs     = " << number_of_dofs     << endl;
//     // cout << "number_of_outputs  = " << number_of_outputs  << endl;

//     //Initialize data



//     Coordinates.resize(number_of_nodes * 3);

//     Number_of_DOFs.resize(max_node_tag + 1);
//     Index_to_Coordinates.resize(max_node_tag + 1);
//     Index_to_Generalized_Displacements.resize(max_node_tag + 1);
//     for (int i = 0; i < max_node_tag + 1; i++)
//     {
//         Number_of_DOFs[i] = -1;
//         Index_to_Coordinates[i] = -1;
//         Index_to_Generalized_Displacements[i] = -1;
//     }

//     //For Elements
//     Number_of_Nodes.resize(max_element_tag + 1);
//     Index_to_Connectivity.resize(max_element_tag + 1);
//     Index_to_Outputs.resize(max_element_tag + 1);
//     Number_of_Gauss_Points.resize(max_element_tag + 1);
//     Index_to_Gauss_Point_Coordinates.resize(max_element_tag + 1);


// // Element_types.resize(max_element_tag+1);
//     Class_Tags.resize(max_element_tag + 1);
//     Partition.resize(max_element_tag + 1);
//     Number_of_Output_Fields.resize(max_element_tag + 1);
//     for (int i = 0; i < max_element_tag; i++)
//     {
//         Number_of_Nodes[i] = -1;
//         Index_to_Connectivity[i] = -1;
//         Index_to_Outputs[i] = -1;
//         Number_of_Gauss_Points[i] = -1;
//         Index_to_Gauss_Point_Coordinates[i] = -1;
//         // Element_types[i] = -1;
//         Class_Tags[i] = -1;
//         Partition[i] = -1;
//         Number_of_Output_Fields[i] = -1;
//     }
// //NOTE: This is wasting some memory.
// //FIXME: Make pre-allocation for these arrays be exact
//     Connectivity.resize(27 * number_of_elements);
//     Gauss_Point_Coordinates.resize(27 * number_of_elements * 3);

// // LoadPattern_names.resize();
// // Material_tags.resize();

//     return 0;
// }

// // int H5OutputWriter::writeNumberOfElements(unsigned int numberOfElements_ )
// // {
// //     number_of_elements = numberOfElements_;



// //     return 0;
// // }


// int H5OutputWriter::writeNodeMeshData(int tag     , const Vector &coords   , int ndofs )
// {
//     int ntags = Number_of_DOFs.Size();
//     int addzeros = tag - ntags;


//     //Extend arrays
//     for (int i = 0; i < addzeros; i++)
//     {
//         Number_of_DOFs[ntags + i] = -1;
//         Index_to_Generalized_Displacements[ntags + i] = -1;
//         Index_to_Coordinates[ntags + i] = -1;
//     }



//     //Form Number_of_DOFs
//     Number_of_DOFs[tag] = ndofs;

//     //Form Coordinates
//     // int ncoordinates = Coordinates.Size();
//     // Coordinates.resize(pos_nodes_coordinates + 3);

//     Coordinates[pos_nodes_coordinates] = coords(0);
//     Coordinates[pos_nodes_coordinates + 1] = coords(1);
//     Coordinates[pos_nodes_coordinates + 2] = coords(2);


//     //For Index_to_Coordinates
//     Index_to_Coordinates[tag] = pos_nodes_coordinates;
//     pos_nodes_coordinates += 3;

//     Index_to_Generalized_Displacements[tag] = pos_nodes_outputs;
//     pos_nodes_outputs += ndofs;

//     length_nodes_accelerations_output    = pos_nodes_outputs;
//     length_nodes_velocities_output       = pos_nodes_outputs;
//     length_nodes_displacements_output    = pos_nodes_outputs;
//     length_nodes_reaction_forcess_output = pos_nodes_outputs;
//     return 0;
// }

// int H5OutputWriter::writeElementMeshData(int tag  , std::string type , const ID &connectivity , int materialtag , const Matrix &gausscoordinates,
//         int length_of_output, int class_tag)
// {

//     if (tag < 0)
//     {
//         cerr << "H5OutputWriter::writeElementMeshData - Error: got tag = " << tag << " < 0" << endl;
//     }

//     int nnodes, ntags;
//     ntags = Number_of_Nodes.Size();
//     int addzeros = tag - ntags;

//     //Extend arrays if necessary
//     for (int i = 0; i <= addzeros; i++)
//     {
//         cout << "ntags = " << ntags << " tag = " << tag;
//         cout << " H5OutputWriter::writeElementMeshData() -- Should not happen!!\n\n";
//         Number_of_Nodes[ntags + i]                  = -1;
//         Index_to_Connectivity[ntags + i]            = -1;
//         Index_to_Outputs[ntags + i]                 = -1;
//         Number_of_Gauss_Points[ntags + i]           = -1;
//         Index_to_Gauss_Point_Coordinates[ntags + i] = -1;
//         Material_tags[ntags + i]                    = -1;
//         Element_types.push_back(" not defined ");
//         Number_of_Output_Fields[ntags + i]          = -1;
//         Class_Tags[ntags + i]                       = -1;
//     }
//     //Check if the element has already been added!!
//     // if (Number_of_Nodes[tag] > 0 || Index_to_Connectivity[tag] >= 0 || Index_to_Outputs[tag] >= 0)
//     // {
//     //     cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " already defined in HDF5 database! Something is wrong"
//     //          << endl;
//     // }
//     // Writing Number_of_Nodes;
//     nnodes =  connectivity.Size();
//     Number_of_Nodes[tag] = nnodes;


//     if (Number_of_Nodes[tag] > 0 && nnodes != Number_of_Nodes[tag])
//     {
//         cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " number of nodes changed in database."  << endl;
//     }

//     if (Index_to_Connectivity[tag] > 0 && pos_elements_connectivity != Index_to_Connectivity[tag])
//     {
//         cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " index to connectivity changed in database."  << endl;
//     }

//     if (Index_to_Outputs[tag] > 0 && pos_elements_outputs != Index_to_Outputs[tag])
//     {
//         cerr << "H5OutputWriter::writeElementMeshData() - Element tag " << tag <<  " index to output changed in database."  << endl;
//     }

//     // Writing Connectivity;
//     // int pos_connect = Connectivity.Size();
//     for (int n = 0; n < nnodes; n++)
//     {
//         Connectivity[pos_elements_connectivity + n] = connectivity(n);
//     }


//     // Writing Index_to_Connectivity;
//     Index_to_Connectivity[tag] = pos_elements_connectivity;
//     pos_elements_connectivity +=  nnodes;

//     // Writing Number_of_Output_Fields;
//     Number_of_Output_Fields[tag] = length_of_output;

//     // Writing Class_Tags;
//     Class_Tags[tag] = class_tag;

//     // Writing Index_to_Outputs;
//     Index_to_Outputs[tag] = pos_elements_outputs;
//     pos_elements_outputs += length_of_output;

//     length_element_output = pos_elements_outputs;

//     // Writing Number_of_Gauss_Points;
//     int ngauss = gausscoordinates.noRows();
//     Number_of_Gauss_Points[tag] = ngauss;
//     // Writing Gauss_Point_Coordinates;
//     // int pos_gauss = Gauss_Point_Coordinates.Size();
//     // Gauss_Point_Coordinates.resize(pos_elements_gausscoords + ngauss * 3);


//     int i = 0;
//     for (int n = 0; n < ngauss; n++)
//     {
//         Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 0);
//         Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 1);
//         Gauss_Point_Coordinates[pos_elements_gausscoords + (i++)] = gausscoordinates(n, 2);
//     }

//     // Writing Index_to_Gauss_Point_Coordinates;
//     Index_to_Gauss_Point_Coordinates[tag] = pos_elements_gausscoords;
//     pos_elements_gausscoords += ngauss * 3;

//     // Writing Element_types;
//     // Element_types.push_back(type);

//     // Writing Material_tags;
//     // Material_tags[tag] = 0;
//     return 0;
// }


// int H5OutputWriter::writeLoadPatternData(int tag , std::string name)
// {
//     // int ntags;
//     // ntags = LoadPattern_names.Size();
//     // int addzeros = tag - ntags;

//     // //Extend arrays
//     // for (int i = 0; i <= addzeros; i++)
//     // {
//     //     LoadPattern_names.push_back(" not defined ");
//     // }
//     // LoadPattern_names.push_back(name);
//     return 0;
// }

// int H5OutputWriter::writeSPConstraintsData(int nodetag , int dof)
// {
//     int nsps = SPNodes.Size();

//     // int ntags;
//     // ntags = LoadPattern_names.Size();
//     // int addzeros = tag - ntags;

//     // //Extend arrays
//     // for (int i = 0; i <= addzeros; i++)
//     // {
//     //     LoadPattern_names.push_back(" not defined ");
//     // }
//     // LoadPattern_names.push_back(name);

//     SPNodes[nsps] = nodetag;
//     SPDofs[nsps] = dof;

//     return 0;
// }

// void H5OutputWriter::syncWriters()
// {
// #ifdef _PARALLEL_PROCESSING
//     // This function is VERY important.
//     // Output writers are instantiated in every processor (within domain, or childs of domain)
//     // and must contain the SAME information about the sizes of the involved arrays, because
//     // creation of arrays in HDF5 is a collective call, i.e. all processors participate
//     // in it using the same information.
//     //
//     // We need to send the mesh information to all processes before creating arrays

//     int root = 0;
//     // int bcast_return_flag;
//     int length_send;
//     int *int_buffer;
//     int processID;
//     double *double_buffer;

//     // MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
//     MPI_Comm_rank(MPI_COMM_WORLD, &processID);


// #endif

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     // //For nodes ====================================================================================
//     // //ID Number_of_DOFs;
//     // length_send = Number_of_DOFs.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Number_of_DOFs.resize(length_send);
//     // }
//     // int_buffer = Number_of_DOFs.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //Vector Coordinates;
//     // length_send = Coordinates.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Coordinates.resize(length_send);
//     // }
//     // double_buffer = Coordinates.theData;
//     // MPI_Bcast(double_buffer, length_send, MPI_DOUBLE,   root, MPI_COMM_WORLD);


//     // //ID Index_to_Coordinates;
//     // length_send = Index_to_Coordinates.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Index_to_Coordinates.resize(length_send);
//     // }
//     // int_buffer = Index_to_Coordinates.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);

//     // //ID Index_to_Generalized_Displacements;
//     // length_send = Index_to_Generalized_Displacements.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Index_to_Generalized_Displacements.resize(length_send);
//     // }
//     // int_buffer = Index_to_Generalized_Displacements.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);



//     // //For Elements =================================================================================
//     // //ID Number_of_Nodes;
//     // length_send = Number_of_Nodes.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Number_of_Nodes.resize(length_send);
//     // }
//     // int_buffer = Number_of_Nodes.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Connectivity;
//     // length_send = Connectivity.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Connectivity.resize(length_send);
//     // }
//     // int_buffer = Connectivity.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Index_to_Connectivity;
//     // length_send = Index_to_Connectivity.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Index_to_Connectivity.resize(length_send);
//     // }
//     // int_buffer = Index_to_Connectivity.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Number_of_Output_Fields;
//     // length_send = Number_of_Output_Fields.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Number_of_Output_Fields.resize(length_send);
//     // }
//     // int_buffer = Number_of_Output_Fields.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Index_to_Outputs;
//     // length_send = Index_to_Outputs.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Index_to_Outputs.resize(length_send);
//     // }
//     // int_buffer = Index_to_Outputs.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Number_of_Gauss_Points;
//     // length_send = Number_of_Gauss_Points.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Number_of_Gauss_Points.resize(length_send);
//     // }
//     // int_buffer = Number_of_Gauss_Points.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //Vector Gauss_Point_Coordinates;
//     // length_send = Gauss_Point_Coordinates.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Gauss_Point_Coordinates.resize(length_send);
//     // }
//     // double_buffer = Gauss_Point_Coordinates.theData;
//     // MPI_Bcast(double_buffer, length_send, MPI_DOUBLE,   root, MPI_COMM_WORLD);


//     // //ID Index_to_Gauss_Point_Coordinates;
//     // length_send = Index_to_Gauss_Point_Coordinates.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Index_to_Gauss_Point_Coordinates.resize(length_send);
//     // }
//     // int_buffer = Index_to_Gauss_Point_Coordinates.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //std::vector<std::string> Element_types;
//     // // length_send = Index_to_Generalized_Displacements.Size();
//     // // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // // if (processID != 0)
//     // // {
//     // //     Index_to_Generalized_Displacements.resize(length_send);
//     // // }
//     // // int_buffer = Index_to_Generalized_Displacements.data;
//     // // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Material_tags;
//     // length_send = Material_tags.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0 && (length_send > 0))
//     // {
//     //  Material_tags.resize(length_send);
//     // }
//     // int_buffer = Material_tags.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);

//     // //ID Class_Tags;
//     // length_send = Class_Tags.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Class_Tags.resize(length_send);
//     // }
//     // int_buffer = Class_Tags.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);


//     // //ID Partition;
//     // length_send = Partition.Size();
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID != 0)
//     // {
//     //  Partition.resize(length_send);
//     // }
//     // int_buffer = Partition.data;
//     // MPI_Bcast(int_buffer, length_send, MPI_INT,   root, MPI_COMM_WORLD);




//     // // =============================================================================================
//     // // Send the filenames and model names to initialize H5OutputWriter on other processes
//     // // =============================================================================================
//     // char char_buffer[H5OUTPUTWRITER_MAX_STRING_SIZE];//   Preset size for all strings
//     // string file_name_tmp, model_name_tmp, stage_name_tmp, previous_stage_name_tmp;


//     // if (model_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     // {
//     //  cerr << "Model name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     // }
//     // if (file_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     // {
//     //  cerr << "File name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     // }
//     // if (stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     // {
//     //  cerr << "Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     // }
//     // if (previous_stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     // {
//     //  cerr << "Previous_Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     // }

//     // // model_name
//     // length_send  = model_name.size() + 1;
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID == 0)
//     // {
//     //  strcpy(char_buffer, model_name.c_str());
//     //  char_buffer[length_send - 1] = '\0';
//     // }
//     // MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     // model_name = std::string(char_buffer);

//     // // file_name
//     // length_send  = file_name.size() + 1;
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID == 0)
//     // {
//     //  strcpy(char_buffer, file_name.c_str());
//     //  char_buffer[length_send - 1] = '\0';
//     // }
//     // MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     // file_name = std::string(char_buffer);

//     // // stage_name;
//     // length_send  = stage_name.size() + 1;
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID == 0)
//     // {
//     //  strcpy(char_buffer, stage_name.c_str());
//     //  char_buffer[length_send - 1] = '\0';
//     // }
//     // MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     // stage_name = std::string(char_buffer);

//     // // previous_stage_name;
//     // length_send  = previous_stage_name.size() + 1;
//     // MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     // if (processID == 0)
//     // {
//     //  strcpy(char_buffer, previous_stage_name.c_str());
//     //  char_buffer[length_send - 1] = '\0';
//     // }
//     // MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     // previous_stage_name = std::string(char_buffer);


//     // // nsteps;
//     // //char_buffer = previous_stage_name.c_str();
//     // //length_send = previous_stage_name.size();
//     // MPI_Bcast(&number_of_time_steps, 1, MPI_CHAR,   root, MPI_COMM_WORLD);
//     // // previous_stage_name = previous_stage_name_tmp;

//     // MPI_Bcast(&length_nodes_displacements_output    , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&length_nodes_velocities_output       , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&length_nodes_accelerations_output    , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&length_nodes_reaction_forcess_output , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&length_element_output                , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&number_of_nodes                      , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&number_of_elements                   , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&max_node_tag                         , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&max_element_tag                      , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&number_of_dofs                       , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&number_of_outputs                    , 1 , MPI_INT , root , MPI_COMM_WORLD);

//     // MPI_Bcast(&zlib_compression_level               , 1 , MPI_INT , root , MPI_COMM_WORLD);
//     // MPI_Bcast(&flag_write_element_output            , 1 , MPI_INT , root , MPI_COMM_WORLD);


//     // int number_of_load_patterns = LoadPattern_names.size();
//     // // MPI_Bcast(&number_of_load_patterns            , 1 , MPI_INT , root , MPI_COMM_WORLD);

//     // // if (processID == 0)
//     // // {
//     // //     for (auto s : LoadPattern_names)
//     // //     {
//     // //         char_buffer = s.
//     // //     }
//     // // }
//     // // else
//     // // {

//     // // }

//     // //The slave processes must be initialized after all this stuff is transmitted.
//     // if (processID != 0)
//     // {
//     //  // this->initialize(file_name, model_name, stage_name, number_of_time_steps);
//     //  // if (previous_stage_name.compare("!!none") == 0)
//     //  // {
//     //  //     cout << "changing previous_stage_name from " << previous_stage_name << " to " << stage_name
//     //  //          << endl;
//     //  //     previous_stage_name = stage_name;
//     //  // }
//     //  // file_name = "";
//     //  // model_name = "";
//     //  // stage_name = "";
//     //  // file_name += filename_in;
//     //  // model_name += model_name_in;
//     //  // stage_name += stage_name_in;

//     //  current_time                         = 0.0;
//     //  current_time_step                    = 0;

//     //  pos_nodes_outputs                    = 0;
//     //  pos_nodes_coordinates                = 0;
//     //  pos_elements_outputs                 = 0;
//     //  pos_elements_gausscoords             = 0;
//     //  pos_elements_connectivity            = 0;

//     //  create_nodeMeshData_arrays           = true;
//     //  create_nodeDisplacements_arrays      = true;
//     //  create_nodeVelocities_arrays         = true;
//     //  create_nodeAccelerations_arrays      = true;
//     //  create_nodeReactionForces_arrays     = true;
//     //  create_elementMeshData_arrays        = true;
//     //  create_elementOutput_arrays          = true;
//     // }


//     // dataset_xfer_plist = H5Pcreate(H5P_DATASET_XFER);
//     // // H5Pset_dxpl_mpio(dataset_xfer_plist, H5FD_MPIO_INDEPENDENT);
//     // H5Pset_dxpl_mpio(dataset_xfer_plist, H5FD_MPIO_COLLECTIVE);


// #endif



// #ifdef _PARALLEL_PROCESSING


//     // =============================================================================================
//     // Send the filenames and model names to initialize H5OutputWriter on other processes
//     // =============================================================================================
//     char char_buffer[H5OUTPUTWRITER_MAX_STRING_SIZE];//   Preset size for all strings
//     string file_name_tmp, model_name_tmp, stage_name_tmp, previous_stage_name_tmp;


//     if (model_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     {
//         cerr << "Model name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     }
//     if (file_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     {
//         cerr << "File name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     }
//     if (stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     {
//         cerr << "Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     }
//     if (previous_stage_name.size() >  H5OUTPUTWRITER_MAX_STRING_SIZE)
//     {
//         cerr << "Previous_Stage name is larger than " << H5OUTPUTWRITER_MAX_STRING_SIZE << "!!! \n";
//     }

//     // model_name
//     length_send  = model_name.size() + 1;
//     MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     if (processID == 0)
//     {
//         strcpy(char_buffer, model_name.c_str());
//         char_buffer[length_send - 1] = '\0';
//     }
//     MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     model_name = std::string(char_buffer);

//     // file_name
//     length_send  = file_name.size() + 1;
//     MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     if (processID == 0)
//     {
//         strcpy(char_buffer, file_name.c_str());
//         char_buffer[length_send - 1] = '\0';
//     }
//     MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     file_name = std::string(char_buffer);

//     // stage_name;
//     length_send  = stage_name.size() + 1;
//     MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     if (processID == 0)
//     {
//         strcpy(char_buffer, stage_name.c_str());
//         char_buffer[length_send - 1] = '\0';
//     }
//     MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     stage_name = std::string(char_buffer);

//     // previous_stage_name;
//     length_send  = previous_stage_name.size() + 1;
//     MPI_Bcast(&length_send, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     if (processID == 0)
//     {
//         strcpy(char_buffer, previous_stage_name.c_str());
//         char_buffer[length_send - 1] = '\0';
//     }
//     MPI_Bcast(char_buffer, length_send, MPI_CHAR,   root, MPI_COMM_WORLD);
//     previous_stage_name = std::string(char_buffer);


//     // nsteps;
//     //char_buffer = previous_stage_name.c_str();
//     //length_send = previous_stage_name.size();
//     MPI_Bcast(&number_of_time_steps, 1, MPI_INT,   root, MPI_COMM_WORLD);
//     int rank = 0;
//     // cout << "H5OutputWriter::syncWriters (" << rank << ") number_of_time_steps = " << number_of_time_steps << endl;

//     current_time                         = 0.0;
//     current_time_step                    = 0;

//     pos_nodes_outputs                    = 0;
//     pos_nodes_coordinates                = 0;
//     pos_elements_outputs                 = 0;
//     pos_elements_gausscoords             = 0;
//     pos_elements_connectivity            = 0;

//     create_nodeMeshData_arrays           = true;
//     create_nodeDisplacements_arrays      = true;
//     create_nodeVelocities_arrays         = true;
//     create_nodeAccelerations_arrays      = true;
//     create_nodeReactionForces_arrays     = true;
//     create_elementMeshData_arrays        = true;
//     create_elementOutput_arrays          = true;

// #endif
// }


// void H5OutputWriter::writeMesh()
// {
//     // =============================================================================================
//     //   Open HDF5 file for writing and create basic structure
//     // =============================================================================================




//     //Check if this is a new file!
//     if (file_is_open)
//     {
//         finalize();
//         // previous_stage_name  = stage_name;
//     }
//     else
//     {
//         previous_stage_name = "!!none";
//     }

//     //Initialize datamembers
//     file_is_open = false;

//     int numProcesses, processID;
//     numProcesses = 1;
//     processID = 0;
// #ifdef _PARALLEL_PROCESSING
//     MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
//     MPI_Comm_rank(MPI_COMM_WORLD, &processID);


//     //Higher ranks get an additional extension .pid

//     // Determine the number of digits in the total number of processes
//     int number = numProcesses;
//     int digits = 0;
//     if (number < 0)
//     {
//         digits = 1;    // remove this line if '-' counts as a digit
//     }
//     while (number)
//     {
//         number /= 10;
//         digits++;
//     }
//     // return digits;
//     if (processID > 0)
//     {
//         std::stringstream ss;
//         ss << ".";
//         ss << setfill('0') << setw(digits) << processID;

//         size_t f = file_name.find(".feioutput");
//         file_name.replace(f, std::string(".feioutput").length(), ss.str());

//         // file_name += ss.str();
//         file_name += ".feioutput";
//     }

//     // cout << "Process " << processID << " filename = " << file_name << endl;

// #endif

//     //================================================================================
//     //Create the file, overwriting it if it exists
//     //================================================================================
//     unsigned flags = H5F_ACC_TRUNC;  // Truncate file (ie. if file exists overwrite it)
//     hid_t file_creation_plist = H5Pcreate(H5P_FILE_CREATE);
//     hid_t file_access_plist = H5Pcreate(H5P_FILE_ACCESS);
//     // status = H5Pset_meta_block_size(file_access_plist, H5OUTPUTWRITER_META_BLOCK_SIZE);
//     // hdf5_check_error(status);
//     // status =  H5Pset_cache(file_access_plist, 0, H5OUTPUTWRITER_CHUNK_NSLOTS, H5OUTPUTWRITER_CHUNK_NBYTES, 0 );
//     // hdf5_check_error(status);
//     // status = H5Pset_sieve_buf_size( file_access_plist, H5OUTPUTWRITER_SIEVE_BUFFER_SIZE );
//     // hdf5_check_error(status);
//     id_file = H5Fcreate(file_name.c_str(), flags , file_creation_plist, file_access_plist);


//     if (id_file > 0)
//     {
//         file_is_open = true;


//         //================================================================================
//         //Setup the global property lists
//         //================================================================================
//         group_creation_plist = H5Pcreate(H5P_GROUP_CREATE);//H5P_GROUP_CREATE);
//         // status = H5Pset_link_creation_order(group_creation_plist, H5P_CRT_ORDER_TRACKED | H5P_CRT_ORDER_INDEXED);
//         // hdf5_check_error(status);



//         //================================================================================
//         // Create basic structure of the file
//         //================================================================================
//         //Create the model name, stage name and previous stage fields
//         write_string(id_file, "Model_Name", model_name);
//         write_string(id_file, "Stage_Name", stage_name);
//         write_string(id_file, "Previous_Stage", previous_stage_name);


//         //Create the group to store the model
//         id_model_group = create_group(id_file, "Model");


//         //Create elements and nodes groups
//         id_elements_group = create_group(id_model_group, "Elements");
//         id_nodes_group    = create_group(id_model_group, "Nodes");

//         //================================================================================
//         // Create time array
//         //================================================================================

//         hsize_t dims[1], maxdims[1];
//         int rank = 1;
//         dims[0] = number_of_time_steps;
//         maxdims[0] = H5S_UNLIMITED;

//         id_time_vector = createVariableLengthDoubleArray(id_file, rank, dims, maxdims, "time", "s");

//         // cout << "id_time_vector = " << id_time_vector << endl;



//         //================================================================================
//         // Domain metadata
//         //================================================================================
//         rank = 1;
//         dims[0] = 1;
//         maxdims[0] = 1;
//         id_number_of_elements   = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Elements", " ");
//         id_number_of_nodes      = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Nodes", " ");
//         id_number_of_time_steps = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Time_Steps", " ");

//         hsize_t id_my_proc_rank = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Process_Number", " ");
//         hsize_t id_number_of_procs = createConstantLengthIntegerArray(id_file, rank, dims, maxdims, "Number_of_Processes_Used", " ");


//         //================================================================================
//         // Model bounds
//         //================================================================================
//         rank = 1;
//         dims[0] = 6;
//         maxdims[0] = 6;
//         hsize_t id_model_bounds = createConstantLengthDoubleArray(id_model_group, rank, dims, maxdims, "Model_Bounds", " ");
//         dims[0] = 1;
//         maxdims[0] = 1;



//         //================================================================================
//         //Write time of creation
//         time_t current;
//         time(&current);
//         char *timestring;
//         timestring = ctime(&current);
//         timestring[strlen(timestring) - 1] = '\0';
//         write_string(id_file, "Date_and_Time_Start", timestring);


//         createVariableLengthStringArray(id_file, "Analysis_Options", " ");


//         H5OUTPUTWRITER_COUNT_OBJS;


//         // =============================================================================================
//         //   Write node mesh data
//         // =============================================================================================


//         rank = 1;
//         int datarank         = 1;
//         int *int_data_buffer = 0;
//         double *double_data_buffer = 0;
//         //hsize_t dims[1];
//         //hsize_t maxdims[1];
//         hsize_t data_dims[1];
//         hsize_t offset[1];
//         hsize_t stride[1];
//         hsize_t count[1];
//         hsize_t block[1];

//         //Create arrays
//         dims[0] = 1;
//         maxdims[0] = H5S_UNLIMITED;
//         id_nodes_ndofs                = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Number_of_DOFs"                     , " ");
//         id_nodes_coordinates          = createVariableLengthDoubleArray(id_nodes_group  , rank , dims , maxdims , "Coordinates"                        , " ");
//         id_index_to_nodes_coordinates = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Coordinates"               , " ");
//         id_index_to_nodes_outputs     = createVariableLengthIntegerArray(id_nodes_group , rank , dims , maxdims , "Index_to_Generalized_Displacements" , " ");

//         // #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
//         //     // int processID;
//         //     // MPI_Comm_rank(MPI_COMM_WORLD, &processID);

//         //     if (processID == 0)
//         //     {
//         //         //
//         // #endif
//         // Write a vector with the number of DOFS for node at a given tag
//         dims[0]      = (hsize_t) Number_of_DOFs.Size();
//         data_dims[0] = (hsize_t) Number_of_DOFs.Size();
//         offset[0]    = 0;
//         stride[0]    = 1;
//         count[0]     = dims[0];
//         block[0]     = 1;
//         int_data_buffer = Number_of_DOFs.data;
//         writeVariableLengthIntegerArray(id_nodes_ndofs,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         dims[0]      = (hsize_t) Coordinates.Size();
//         data_dims[0] = (hsize_t) Coordinates.Size();
//         count[0]     = dims[0];
//         double_data_buffer = Coordinates.theData;
//         writeVariableLengthDoubleArray(id_nodes_coordinates,
//                                        datarank,
//                                        dims,
//                                        data_dims,
//                                        offset,
//                                        stride,
//                                        count,
//                                        block,
//                                        double_data_buffer);

//         dims[0]      = (hsize_t) Index_to_Coordinates.Size();
//         data_dims[0] = (hsize_t) Index_to_Coordinates.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Index_to_Coordinates.data;
//         writeVariableLengthIntegerArray(id_index_to_nodes_coordinates,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         dims[0]      = (hsize_t) Index_to_Generalized_Displacements.Size();
//         data_dims[0] = (hsize_t) Index_to_Generalized_Displacements.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Index_to_Generalized_Displacements.data;
//         writeVariableLengthIntegerArray(id_index_to_nodes_outputs,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);




//         //================================================================================
//         // Create number of elements and nodes
//         //================================================================================

//         rank        = 1;
//         dims[0]     = 1;
//         data_dims[0] = 1;
//         offset[0]   = 0;
//         stride[0]   = 1;
//         count[0]    = 1;
//         block[0]    = 1;

//         writeConstantLengthIntegerArray(id_number_of_nodes, rank, dims, data_dims, offset, stride, count, block,
//                                         &number_of_nodes);

//         writeConstantLengthIntegerArray(id_number_of_elements, rank, dims, data_dims, offset, stride, count, block,
//                                         &number_of_elements);


//         rank        = 1;
//         dims[0]     = 1;
//         data_dims[0] = 1;
//         offset[0]   = 0;
//         stride[0]   = 1;
//         count[0]    = 1;
//         block[0]    = 1;

//         writeConstantLengthIntegerArray(id_my_proc_rank, rank, dims, data_dims, offset, stride, count, block,
//                                         &processID);

//         rank        = 1;
//         dims[0]     = 1;
//         data_dims[0] = 1;
//         offset[0]   = 0;
//         stride[0]   = 1;
//         count[0]    = 1;
//         block[0]    = 1;

//         writeConstantLengthIntegerArray(id_number_of_procs, rank, dims, data_dims, offset, stride, count, block,
//                                         &numProcesses);

//         H5Dclose(id_my_proc_rank);
//         H5Dclose(id_number_of_procs);



//         // =============================================================================================
//         //Determine the bounds
//         // =============================================================================================
//         double bounds[6];//, xmax, ymin, ymax, zmin, zmax;
//         bounds[0] = -std::numeric_limits<double>::infinity();
//         bounds[1] = std::numeric_limits<double>::infinity();
//         bounds[2] = -std::numeric_limits<double>::infinity();
//         bounds[3] = std::numeric_limits<double>::infinity();
//         bounds[4] = -std::numeric_limits<double>::infinity();
//         bounds[5] = std::numeric_limits<double>::infinity();
//         for (int n = 0; n < number_of_nodes; n++)
//         {
//             double x = Coordinates[3 * n];
//             double y = Coordinates[3 * n + 1];
//             double z = Coordinates[3 * n + 2];
//             if (x > bounds[0])
//             {
//                 bounds[0] = x;
//             }
//             if (x < bounds[1])
//             {
//                 bounds[1] = x;
//             }
//             if (y > bounds[2])
//             {
//                 bounds[2] = y;
//             }
//             if (y < bounds[3])
//             {
//                 bounds[3] = y;
//             }
//             if (z > bounds[4])
//             {
//                 bounds[4] = z;
//             }
//             if (z < bounds[5])
//             {
//                 bounds[5] = z;
//             }
//         }
//         // cout << processID <<  ": Model bounds ("
//         //      << bounds[0] << ", "
//         //      << bounds[1] << ", "
//         //      << bounds[2] << ", "
//         //      << bounds[3] << ", "
//         //      << bounds[4] << ", "
//         //      << bounds[5] << ")\n";
//         //Write them
//         rank        = 1;
//         dims[0]     = 6;
//         data_dims[0] = 6;
//         offset[0]   = 0;
//         stride[0]   = 1;
//         count[0]    = 6;
//         block[0]    = 1;

//         writeConstantLengthDoubleArray(id_model_bounds, rank, dims, data_dims, offset, stride, count, block,
//                                        bounds);
//         // cout << processID <<  "Wrote bounds!\n";
//         H5Dclose(id_model_bounds);
//         H5OUTPUTWRITER_COUNT_OBJS;

//         // =============================================================================================
//         //   Write element mesh data
//         // =============================================================================================


//         rank = 1;
//         dims[0] = 1;
//         maxdims[0] = H5S_UNLIMITED;

//         id_elements_nnodes                = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Nodes", " ");
//         id_elements_connectivity          = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Connectivity", " ");
//         id_index_to_elements_connectivity = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Connectivity", " ");
//         id_elements_noutputs              = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Output_Fields", " ");
//         id_index_to_elements_output       = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Outputs", " ");
//         id_elements_ngauss                = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Number_of_Gauss_Points", " ");
//         id_elements_gausscoords           = createVariableLengthDoubleArray (id_elements_group, rank, dims, maxdims, "Gauss_Point_Coordinates", " ");
//         id_index_to_elements_gausscoords  = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Index_to_Gauss_Point_Coordinates", " ");
//         id_elements_type                  = createVariableLengthStringArray (id_elements_group,  "Element_types", " ");
//         id_elements_materialtag           = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Material_tags", " ");
//         id_elements_classtag              = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Class_Tags", " ");

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO // Only write partition if in parallel mode
//         id_elements_partition             = createVariableLengthIntegerArray(id_elements_group, rank, dims, maxdims, "Partition", " ");
// #endif

//         // Write a vector with the number of nodes at a given elements tag
//         datarank     = 1;
//         dims[0]      = (hsize_t) Number_of_Nodes.Size();
//         data_dims[0] = (hsize_t) Number_of_Nodes.Size();
//         offset[0]    = 0;
//         stride[0]    = 1;
//         count[0]     = dims[0];
//         block[0]     = 1;
//         int_data_buffer = Number_of_Nodes.data;
//         writeVariableLengthIntegerArray(id_elements_nnodes,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);


//         //Write index to connectivity
//         dims[0]      = (hsize_t) Index_to_Connectivity.Size();
//         data_dims[0] = (hsize_t) Index_to_Connectivity.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Index_to_Connectivity.data;
//         writeVariableLengthIntegerArray(id_index_to_elements_connectivity,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);


//         //Write element connectivity
//         dims[0]      = (hsize_t) Connectivity.Size();
//         data_dims[0] = (hsize_t) Connectivity.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Connectivity.data;
//         writeVariableLengthIntegerArray(id_elements_connectivity,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         //Write index to number of outputs
//         dims[0]      = (hsize_t) Number_of_Output_Fields.Size();
//         data_dims[0] = (hsize_t) Number_of_Output_Fields.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Number_of_Output_Fields.data;
//         writeVariableLengthIntegerArray(id_elements_noutputs,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         //Write index to outputs
//         dims[0]      = (hsize_t) Index_to_Outputs.Size();
//         data_dims[0] = (hsize_t) Index_to_Outputs.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Index_to_Outputs.data;
//         writeVariableLengthIntegerArray(id_index_to_elements_output,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         //Write material tags
//         dims[0]      = (hsize_t) Material_tags.Size();
//         data_dims[0] = (hsize_t) Material_tags.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Material_tags.data;
//         // writeVariableLengthIntegerArray(id_elements_materialtag,
//         //                                 datarank,
//         //                                 dims,
//         //                                 data_dims,
//         //                                 offset,
//         //                                 stride,
//         //                                 count,
//         //                                 block,
//         //                                 int_data_buffer);

//         dims[0]      = (hsize_t) Class_Tags.Size();
//         data_dims[0] = (hsize_t) Class_Tags.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Class_Tags.data;
//         writeVariableLengthIntegerArray(id_elements_classtag,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);


// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
//         // if (processID == 0) // Only main processor knows about the global partition
//         // {

//         // cout << "Processor " << processID << " writing partition data \n";
//         // cout << "     Partition.Size() = " << Partition.Size() << " \n";
//         // cout << "     dims[0]          = " << dims[0] << " \n";
//         dims[0]      = (hsize_t) Partition.Size();
//         data_dims[0] = (hsize_t) Partition.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Partition.data;
//         writeVariableLengthIntegerArray(id_elements_partition,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);
//         // }
//         // cout << "Processor " << processID << " went through. \n";
// #endif

//         // TODO: Bring back element types
//         // //Write material tags
//         // for (int tag = 0; tag < (int) Element_types.size(); tag++)
//         // {
//         //     std::string type = Element_types[tag];
//         //     writeVariableLengthStringArray(id_elements_type,
//         //                                    tag,
//         //                                    type.size(),
//         //                                    type);
//         // }


//         //Write index to gauss coordinates (if any)
//         dims[0]      = (hsize_t) Number_of_Gauss_Points.Size();
//         data_dims[0] = (hsize_t) Number_of_Gauss_Points.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Number_of_Gauss_Points.data;
//         writeVariableLengthIntegerArray(id_elements_ngauss,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);


//         dims[0]      = (hsize_t) Index_to_Gauss_Point_Coordinates.Size();
//         data_dims[0] = (hsize_t) Index_to_Gauss_Point_Coordinates.Size();
//         count[0]     = dims[0];
//         int_data_buffer = Index_to_Gauss_Point_Coordinates.data;
//         writeVariableLengthIntegerArray(id_index_to_elements_gausscoords,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);

//         //Write gauss coordinate values
//         dims[0]      = (hsize_t) Gauss_Point_Coordinates.Size();
//         data_dims[0] = (hsize_t) Gauss_Point_Coordinates.Size();
//         count[0]     = dims[0];
//         double_data_buffer = Gauss_Point_Coordinates.theData;
//         writeVariableLengthDoubleArray(id_elements_gausscoords,
//                                        datarank,
//                                        dims,
//                                        data_dims,
//                                        offset,
//                                        stride,
//                                        count,
//                                        block,
//                                        double_data_buffer);
//         H5OUTPUTWRITER_COUNT_OBJS;






//         // =============================================================================================
//         //   CREATE OUTPUT ARRAYS!
//         // =============================================================================================

//         {
//             int rank = 2;
//             hsize_t dims[2];
//             hsize_t maxdims[2];
//             dims[0] = (hsize_t) length_nodes_displacements_output;
//             dims[1] = number_of_time_steps + 1;
//             maxdims[0] = (hsize_t)  length_nodes_displacements_output;
//             maxdims[1] = number_of_time_steps + 1;

//             id_nodes_displacements = createVariableLengthDoubleArray(id_nodes_group, rank, dims, maxdims, "Generalized_Displacements", " ", 1);
//             // id_nodes_reaction_forces = createVariableLengthDoubleArray(id_nodes_group, rank, dims, maxdims, "Generalized_Forces", " ", 1);
//         }

//         // if (processID > 0)
//         // if (length_element_output <= 0)
//         // {
//         //  length_element_output = 1;
//         // }


//         if (length_element_output > 0)
//         {
//             int rank = 2;
//             hsize_t dims[2];
//             hsize_t maxdims[2];
//             dims[0] = (hsize_t) length_element_output;
//             dims[1] = number_of_time_steps + 1;
//             maxdims[0] = (hsize_t)  length_element_output;
//             maxdims[1] = number_of_time_steps + 1;

//             if (flag_write_element_output == 1  )
//             {
//                 // dims[1] = 1;
//                 // maxdims[1] = 1;
//                 id_elements_output = createVariableLengthDoubleArray(id_elements_group, rank, dims, maxdims, "Outputs", " ", 1);
//             }

//         }//    create_elementOutput_arrays = false;


//         //For SP Constraints
//         rank = 1;
//         dims[0] = 1;
//         maxdims[0] = H5S_UNLIMITED;
//         id_Constrained_Nodes              = createVariableLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Constrained_Nodes", " ");
//         id_Constrained_DOFs               = createVariableLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Constrained_DOFs", " ");

//         //Write gauss coordinate values
//         dims[0]      = (hsize_t) SPNodes.Size();
//         data_dims[0] = (hsize_t) SPNodes.Size();
//         count[0]     = dims[0];
//         int_data_buffer = SPNodes.data;
//         writeVariableLengthIntegerArray(id_Constrained_Nodes,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);
//         H5OUTPUTWRITER_COUNT_OBJS;

//         //Write gauss coordinate values
//         dims[0]      = (hsize_t) SPDofs.Size();
//         data_dims[0] = (hsize_t) SPDofs.Size();
//         count[0]     = dims[0];
//         int_data_buffer = SPDofs.data;
//         writeVariableLengthIntegerArray(id_Constrained_DOFs,
//                                         datarank,
//                                         dims,
//                                         data_dims,
//                                         offset,
//                                         stride,
//                                         count,
//                                         block,
//                                         int_data_buffer);
//     }
//     else  //Could not open file
//     {
//         cerr << "H5OutputWriter::writeMesh() -> could not open file: " << file_name << "!\n\n";
//     }


//     H5OUTPUTWRITER_COUNT_OBJS;
// }

// int H5OutputWriter::writeElementPartitionData(int tag  , int partition)
// {
// #ifdef _PARALLEL_PROCESSING
//     int nnodes, ntags;
//     ntags = Partition.Size();
//     int addzeros = tag - ntags;

//     //Extend arrays
//     for (int i = 0; i <= addzeros; i++)
//     {
//         Partition[ntags + i]                       = -1;
//     }
//     Partition[tag] = partition;
// #else
//     cout << "H5OutputWriter::writeElementPartitionData() -- Not available in sequential mode\n";
// #endif
//     return 0;
// }




// int H5OutputWriter::writeMaterialMeshData(int tag , std::string type , Vector &parameters)
// {
//     return 0;
// }

// // Results for Nodes
// int H5OutputWriter::writeDisplacements(  int nodeTag, const Vector &displacements)
// {

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
//     int processID;
//     MPI_Comm_rank(MPI_COMM_WORLD, &processID);

//     // cout << setw(5) << nodeTag << " == " << processID << " == " << ": (" << displacements[0] << ", " << displacements[1] << ", " << displacements[2] << ")\n ";
//     // cout << "   pos = " << pos << " step = " << current_time_step << " ndofs = " << ndofs << endl;
// #endif

//     buffer_ready.store(true);
//     // Put data to buffer
//     for (int i =0; i< displacements.Size();++i)
//         displacement_buffer.push(displacements[i]);

//     H5OUTPUTWRITER_COUNT_OBJS;
//     return 0;
// }
// // ==============================================================
// // Yuan testing
// // This method should be called only once. 
// void H5OutputWriter::writeDisplacements2disk() {
//     // Get data from buffer.
//     double item=3.0;
//     vector<double> disp_vec_step(number_of_dofs, 0.0) ;
//     for (int step = 0; step < number_of_time_steps; ++step){
//         for (int d_count=0; d_count<number_of_dofs; ++d_count){
//             // This is a blocking pop.
//             displacement_buffer.pop(item);
//             // fprintf(stderr, "writeDisplacements2disk item: %f\n",item );
//             disp_vec_step[d_count] = item ;
//         }
//         // Write data to HDF5 (hard disk) for this step
//         int datarank         = 1;
//         hsize_t data_dims[1] = {1};
//         hsize_t offset[2]    = {1, 0};
//         hsize_t stride[2]    = {1, 0};
//         hsize_t count[2]     = {1, 0};
//         hsize_t block[2]     = {1, 0};
//         hsize_t *dims;
//         dims = 0;   
//         data_dims[0] = (hsize_t) disp_vec_step.size();
//         offset[0]    = (hsize_t) 0;
//         offset[1]    = (hsize_t) step ;
//         stride[0]    = 1;
//         stride[1]    = 1;
//         count[0]     = (hsize_t) disp_vec_step.size();
//         count[1]     = 1;
//         block[0]     = 1;
//         block[1]     = 1;
//         writeVariableLengthDoubleArray(id_nodes_displacements,
//                                        datarank,
//                                        dims,
//                                        data_dims,
//                                        offset,
//                                        stride,
//                                        count,
//                                        block,
//                                        &disp_vec_step[0]);
//     }    
//     H5OUTPUTWRITER_COUNT_OBJS;
// }

// // Results for Nodes
// int H5OutputWriter::writeDummyDisplacements(  )
// {


//     // Read NDOFS from HDF5 file
//     int datarank         = 1;
//     hsize_t dummy[1] = {1};

//     double *zerodata = 0;
//     writeVariableLengthDoubleArray(id_nodes_displacements,
//                                    datarank,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    zerodata);

//     H5OUTPUTWRITER_COUNT_OBJS;
//     return 0;
// }


// int H5OutputWriter::writeVelocities(     int nodeTag, const Vector &velocities)
// {
//     cout << "H5OutputWriter::writeVelocities()  -- Not available. Implement by copying writeDisplacements.\n ";
//     return 0;
// }


// int H5OutputWriter::writeAccelerations(  int nodeTag, const Vector &accelerations)
// {
//     cout << "H5OutputWriter::writeAccelerations()  -- Not available. Implement by copying writeDisplacements.\n ";
//     return 0;
// }


// int H5OutputWriter::writeReactionForces( int nodeTag, const Vector &reactionForces)
// {

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO
//     int processID;
//     MPI_Comm_rank(MPI_COMM_WORLD, &processID);

//     // cout << setw(5) << nodeTag << " == " << processID << " == " << ": (" << displacements[0] << ", " << displacements[1] << ", " << displacements[2] << ")\n ";
// #endif
//     int pos, ndofs;

//     // Read NDOFS from HDF5 file
//     int datarank         = 1;
//     hsize_t data_dims[1] = {1};
//     hsize_t offset[2]    = {(hsize_t) nodeTag , 0};
//     hsize_t stride[2]    = {1, 0};
//     hsize_t count[2]     = {1, 0};
//     hsize_t block[2]     = {1, 0};

//     hsize_t id_dataspace = H5Dget_space(id_nodes_ndofs);
//     hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace
//     status = H5Sselect_hyperslab(
//                  id_dataspace,          // Id of the parent dataspace
//                  H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                  offset,                // start of selection
//                  stride,                // stride in each dimension, NULL  is select everything
//                  count ,                // how many blocks to select in each direction
//                  block                  // little block selected per selection
//              );

//     hdf5_check_error(status);

//     H5Dread(id_nodes_ndofs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &ndofs);
//     H5Dread(id_index_to_nodes_outputs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &pos);


//     //Write data
//     // hsize_t dims[2]      =  { (hsize_t)  length_nodes_displacements_output, (hsize_t)  current_time_step};
//     hsize_t *dims;
//     dims = 0;   // This is to disable array extension (would not be done collectively)
//     data_dims[0] = (hsize_t) ndofs;

//     offset[0]    = (hsize_t) pos;
//     offset[1]    = (hsize_t) current_time_step - 1;
//     stride[0]    = 1;
//     stride[1]    = 1;
//     count[0]     = (hsize_t) ndofs;
//     count[1]     = 1;
//     block[0]     = 1;
//     block[1]     = 1;

// #if _PARALLEL_PROCESSING_COLLECTIVE_IO
//     // cout << "   pos = " << pos << " step = " << current_time_step << " ndofs = " << ndofs << endl;
// #endif


//     double *data = reactionForces.theData;
//     writeVariableLengthDoubleArray(id_nodes_reaction_forces,
//                                    datarank,
//                                    dims,
//                                    data_dims,
//                                    offset,
//                                    stride,
//                                    count,
//                                    block,
//                                    data);

//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return 0;
//     return 0;
// }



// // Results for Elements
// int H5OutputWriter::writeElementOutput(int elementTag, const  Vector &output)
// {
//     if (length_element_output > 0){
//         // debug
//         // cout<<"writeElementOutput is called"<<endl;
//         // Put data to buffer
//         for (int i =0; i< output.Size();++i)
//             elementOutput_buffer.push(output[i]);

//     }
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return 0;
// }
// // ==============================================================
// // Yuan
// void H5OutputWriter::writeElementOutput2disk(){
//     if (length_element_output > 0) 
//     {
//         // debug
//         // cout<<"writeElementOutput2disk is called"<<endl;
//         // Get the data from buffer. 
//         double item{0.0};
//         vector<double> element_vec_step(number_of_outputs, 0.0) ;
//         for (int step = 0; step < number_of_time_steps; ++step)
//         {
//             for (int c = 0; c < number_of_outputs; ++c)
//             {
//                 elementOutput_buffer.pop(item);
//                 element_vec_step[c] = item;
//             }

//             // Write the data to HDF5 (hard disk).
//             int datarank         = 1;
//             hsize_t data_dims[1] = {1};
//             hsize_t offset[2]    = {1, 0};
//             hsize_t stride[2]    = {1, 0};
//             hsize_t count[2]     = {1, 0};
//             hsize_t block[2]     = {1, 0};
//             hsize_t *dims;
//             dims = 0;
//             data_dims[0] = (hsize_t) element_vec_step.size() ;
//             offset[0]    = (hsize_t) 0;
//             offset[1]    = (hsize_t) step;
//             stride[0]    = 1;
//             stride[1]    = 1;
//             count[0]     = (hsize_t) element_vec_step.size() ;
//             count[1]     = 1;
//             block[0]     = 1;
//             block[1]     = 1;
//             writeVariableLengthDoubleArray(id_elements_output,
//                                            datarank,
//                                            dims,
//                                            data_dims,
//                                            offset,
//                                            stride,
//                                            count,
//                                            block,
//                                            &element_vec_step[0]);
//         }
//     }
//     H5OUTPUTWRITER_COUNT_OBJS;
// }





// // Results for Elements
// int H5OutputWriter::writeDummyElementOutput()
// {
// // Read NDOFS from HDF5 file
//     int datarank         = 1;
//     hsize_t dummy[1] = {1};

//     double *zerodata = 0;//displacements.theData;
//     writeVariableLengthDoubleArray(id_elements_output,
//                                    datarank,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    dummy,
//                                    zerodata);

//     H5OUTPUTWRITER_COUNT_OBJS;
//     return 0;
// }






// int H5OutputWriter::setTime(double t)
// {

//     if (call_hdf5_flush)
//     {
//         H5Fflush(id_file, H5F_SCOPE_GLOBAL);

//         cout << "H5OutputWriter: H5Flush has been called!\n";
//         cout << " *****************************************************************\n";
//         cout << " *****************************************************************\n";
//         cout << " ESSI has stopped simulating and is waiting for you to continue. \n";
//         cout << " You can safely explore the current output file: \n";
//         cout << "   " << file_name << endl;
//         cout << " *****************************************************************\n";
//         cout << " Press any key to continue. \n";
//         std::cin.ignore();

//         call_hdf5_flush = false;
//     }

//     current_time = t;

//     //Extending past initial data allocation
//     if (current_time_step >= number_of_time_steps)
//     {
//         number_of_time_steps = current_time_step + 1;
//     }

//     hsize_t dims[1]      = {(hsize_t) number_of_time_steps};
//     hsize_t data_dims[1] = {1};
//     hsize_t offset[1]    = {(hsize_t) current_time_step };
//     hsize_t stride[1]    = {1};
//     hsize_t count[1]     = {1};
//     hsize_t block[1]     = {1};

//     id_time_vector = H5Dopen( id_file, "/time", H5P_DEFAULT);
//     writeVariableLengthDoubleArray(id_time_vector,
//                                    1, //datarank
//                                    dims,
//                                    data_dims,
//                                    offset,
//                                    stride,
//                                    count,
//                                    block,
//                                    &current_time);

//     offset[0]   = 0;

//     writeConstantLengthIntegerArray(id_number_of_time_steps, 1, dims, data_dims, offset, stride, count, block,
//                                     &number_of_time_steps);

//     current_time_step++;
//     H5OUTPUTWRITER_COUNT_OBJS;


//     //Extend objects
//     hsize_t dims_new[2]      =  { (hsize_t)  length_nodes_displacements_output, (hsize_t)  current_time_step + 1};

//     status =  H5Dset_extent( id_nodes_displacements, dims_new );

//     hdf5_check_error(status);

//     int processID = 1; //

// #ifdef _PARALLEL_PROCESSING
//     MPI_Comm_rank(MPI_COMM_WORLD, &processID);
// #endif

//     if ( flag_write_element_output == 1 && (processID > 0)) //extend element output array depending on whether the flag is enabled.
//     {
//         dims_new[0] = length_element_output;
//         dims_new[1] = current_time_step + 1;
//         status =  H5Dset_extent( id_elements_output, dims_new );
//     }


//     return 0;
// }



// int H5OutputWriter::write_string(hid_t here, std::string name, std::string contents)
// {
//     // std::cout << "Writing string" << endl;
//     // Some variables needed
//     hid_t id_type_string;           // Id of the string object type
//     hid_t id_dataspace_string;      // Id of the string dataspace
//     hid_t id_data_string;           // Id of the actual string data
//     hsize_t dims_string[1];         // Dimensions of the string array [1: scalar]


//     id_type_string = H5Tcopy(H5T_C_S1);
//     status = H5Tset_size(id_type_string, contents.size());
//     hdf5_check_error(status);
//     dims_string[0] = 1;//contents.size();
//     id_dataspace_string = H5Screate_simple(1, dims_string, NULL);
//     id_data_string = H5Dcreate2(here,
//                                 name.c_str(),
//                                 id_type_string,
//                                 id_dataspace_string,
//                                 H5P_DEFAULT,
//                                 H5P_DEFAULT,
//                                 H5P_DEFAULT);

//     //Added By Babak 5/31/14
//     //------------------------
// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     status = H5Dwrite(id_data_string,
//                       id_type_string,
//                       H5S_ALL,
//                       H5S_ALL,
//                       dataset_xfer_plist,
//                       contents.c_str());

// #else

//     status = H5Dwrite(id_data_string,
//                       id_type_string,
//                       H5S_ALL,
//                       H5S_ALL,
//                       H5P_DEFAULT,
//                       contents.c_str());
// #endif
//     //------------------------


//     //     status = H5Dwrite(id_data_string,
//     //                       id_type_string,
//     //                       H5S_ALL,
//     //                       H5S_ALL,
//     //                       H5P_DEFAULT,
//     //                       contents.c_str());

//     H5Sclose(id_dataspace_string);
//     H5Dclose(id_data_string);

//     hdf5_check_error(status);

//     return 0;
// }

// int H5OutputWriter::create_group(hid_t here, std::string name)
// {

//     // cout << "Creating group : " << name << endl;
//     hid_t id_new_group;
//     id_new_group = H5Gcreate(here, name.c_str(), H5P_DEFAULT, group_creation_plist , H5P_DEFAULT);
//     return id_new_group;
// }


// int H5OutputWriter::setAttribute(hid_t object, std::string units)
// {
//     // hid_t atype;
//     // hid_t attribute_dataspace_id;
//     // hid_t attribute;

//     // attribute_dataspace_id = H5Screate(H5S_SCALAR);
//     // atype = H5Tcopy(H5T_C_S1);
//     // H5Tset_size(atype, units.size() );
//     // attribute = H5Acreate2(object,
//     //                        "units",
//     //                        atype,
//     //                        attribute_dataspace_id,
//     //                        H5P_DEFAULT,
//     //                        H5P_DEFAULT);
//     // status = H5Awrite(attribute, atype, units.c_str());
//     // H5Aclose(attribute);
//     // H5Sclose(attribute_dataspace_id);

//     return 0;
// }




// hid_t H5OutputWriter::createVariableLengthDoubleArray(hid_t here,
//         int rank,
//         hsize_t *dims,
//         hsize_t *maxdims,
//         std::string name,
//         std::string attribute,
//         int timedimension)
// {
//     hid_t id_array;
//     double fill_value = 0.0;
//     id_array = createVariableLengthArray(here,
//                                          rank,
//                                          dims,
//                                          maxdims,
//                                          name,
//                                          attribute,
//                                          H5T_NATIVE_DOUBLE,
//                                          sizeof(double),
//                                          &fill_value,
//                                          timedimension);

//     return id_array;
// }

// hid_t H5OutputWriter::createVariableLengthIntegerArray(hid_t here,
//         int rank,
//         hsize_t *dims,
//         hsize_t *maxdims,
//         std::string name,
//         std::string attribute,
//         int timedimension)
// {
//     hid_t id_array;
//     int fill_value = -1;
//     id_array = createVariableLengthArray(here,
//                                          rank,
//                                          dims,
//                                          maxdims,
//                                          name,
//                                          attribute,
//                                          H5T_NATIVE_INT,
//                                          sizeof(int),
//                                          &fill_value,
//                                          timedimension);

//     return id_array;
// }




// hid_t H5OutputWriter::createVariableLengthStringArray(hid_t here,
//         std::string name,
//         std::string attribute)
// {


// #ifdef HDF5_CREATE_CHECK
//     cout << "HDF5 array var-length (string) create: " << name << "\n";
// #endif
//     int rank = 1;
//     hsize_t dims[1] = {1};
//     hsize_t maxdims[1] = {H5S_UNLIMITED};
//     hid_t id_array;

//     //Create the data description both for data in file and in memory
//     hid_t id_dataspace;
//     id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace


//     //Use unlimited string type
//     hid_t type = H5Tcopy(H5T_C_S1);
//     status = H5Tset_size(type, H5T_VARIABLE);
//     hdf5_check_error(status);

//     //Setup the creation property list
//     dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
//     dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

//     // Set the layout to be chunked, the chunk size and the fill value
//     // needs to be chunked because it is extensible
//     // Set the layout to be chunked, the chunk size and the fill value
//     // needs to be chunked because it is extensible
//     hsize_t chunk_dims[1] = {1};

//     // cout << "nbytes_one_chunk = " << nbytes_one_chunk << "\n\n\n";
//     // cout << "nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK = " << nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK << "\n\n\n";

//     status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
//     hdf5_check_error(status);
//     status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
//     hdf5_check_error(status);


//     // Create the dataset
//     id_array = H5Dcreate2(here,
//                           name.c_str(),
//                           type,
//                           id_dataspace,
//                           H5P_DEFAULT,
//                           dataset_creation_plist,
//                           dataset_access_plist);
//     setAttribute(id_array, attribute);

//     H5Sclose(id_dataspace);
//     H5OUTPUTWRITER_COUNT_OBJS;



//     return id_array;
// }



// hid_t H5OutputWriter::createVariableLengthArray(hid_t here,
//         int rank,
//         hsize_t *dims,
//         hsize_t *maxdims,
//         std::string name,
//         std::string attribute,
//         hid_t type, int sizeof_type, void *fill_value_ptr, int timedimension)
// {
//     // int fill_value = 0;

//     //Setup the creation property list
//     dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
//     dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

//     // Set the layout to be chunked, the chunk size and the fill value
//     // needs to be chunked because it is extensible

//     hsize_t chunk_dims[rank];
//     int nbytes_one_chunk = sizeof_type;// sizeof(int);
//     for (int i = 0; i < rank; i++)
//     {

//         chunk_dims[i] = std::min(maxdims[i], static_cast<hsize_t>(H5OUTPUTWRITER_CHUNK_MAXDIMENSION));

//         nbytes_one_chunk *= chunk_dims[i];
//     }

//     // The idea is to chunk less in the time dimension.
//     if (timedimension >= 0)
//     {
//         nbytes_one_chunk /= chunk_dims[timedimension];
//         chunk_dims[timedimension] = std::min(maxdims[timedimension], static_cast<hsize_t>(H5OUTPUTWRITER_CHUNK_TIMEDIM));
//         nbytes_one_chunk *= chunk_dims[timedimension];
//     }


//     // cout << "nbytes_one_chunk = " << nbytes_one_chunk << "\n\n\n";
//     // cout << "nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK = " << nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK << "\n\n\n";

//     status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
//     hdf5_check_error(status);
//     status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
//     hdf5_check_error(status);
//     status = H5Pset_chunk_cache(dataset_access_plist, H5OUTPUTWRITER_CHUNK_NSLOTS, nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK, H5OUTPUTWRITER_PREEMPTION_POLICY );
//     hdf5_check_error(status);
//     status = H5Pset_fill_value(dataset_creation_plist, type, fill_value_ptr);
//     hdf5_check_error(status);
//     if (type == H5T_NATIVE_INT)
//     {
//         status = H5Pset_fill_time(dataset_creation_plist, H5D_FILL_TIME_ALLOC);// H5D_FILL_TIME_NEVER);
//     }
//     else
//     {
//         status = H5Pset_fill_time(dataset_creation_plist, H5D_FILL_TIME_NEVER);
//     }
//     hdf5_check_error(status);

//     if (zlib_compression_level > 0)
//     {
//         status = H5Pset_deflate (dataset_creation_plist, zlib_compression_level );
//         hdf5_check_error(status);
//     }


// #ifdef HDF5_CREATE_CHECK
//     cout << "HDF5 array var-length << ";
//     if (type == H5T_NATIVE_DOUBLE)
//     {
//         cout << "(double)";
//     }
//     else if (type == H5T_NATIVE_INT)
//     {
//         cout << "(int)";
//     }
//     cout << " create: " << name << "\n";
//     cout << "    rank = " << rank << "\n";
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "          dims[" << i << "] = " << dims[i] << "\n";
//     }
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
//     }
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "    chunk_dims[" << i << "] = " << chunk_dims[i] << "\n";
//     }
// #endif

//     //Create the data description both for data in file and in memory
//     hid_t id_dataspace;
//     id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace

//     // Create the dataset
//     hid_t id_array = H5Dcreate2(here,
//                                 name.c_str(),
//                                 type,
//                                 id_dataspace,
//                                 H5P_DEFAULT,
//                                 dataset_creation_plist,
//                                 dataset_access_plist);
//     // hdf5_check_error(status);
//     setAttribute(id_array, attribute);

//     H5Sclose(id_dataspace);
//     H5OUTPUTWRITER_COUNT_OBJS;

//     return id_array;
// }


// hid_t H5OutputWriter::createConstantLengthDoubleArray(hid_t here,
//         int rank,
//         hsize_t *dims,
//         hsize_t *maxdims,
//         std::string name,
//         std::string attribute)
// {

// #ifdef HDF5_CREATE_CHECK
//     cout << "HDF5 array const-length (double) create: " << name << "\n";
//     cout << "    rank = " << rank << "\n";
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "          dims[" << i << "] = " << dims[i] << "\n";
//     }
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
//     }
// #endif
//     double fill_value = 0.0;

//     //Setup the creation property list
//     dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
//     dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);


//     status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_DOUBLE, &fill_value);
//     hdf5_check_error(status);


//     //Create the data description both for data in file and in memory
//     hid_t id_dataspace;
//     id_dataspace = H5Screate_simple(rank , dims     , maxdims     );       // create dataspace

//     // Create the dataset
//     hid_t id_array = H5Dcreate2(here,
//                                 name.c_str(),
//                                 H5T_NATIVE_DOUBLE,
//                                 id_dataspace,
//                                 H5P_DEFAULT,
//                                 dataset_creation_plist,
//                                 H5P_DEFAULT);
//     setAttribute(id_array, attribute);
//     H5Sclose(id_dataspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }




// hid_t H5OutputWriter::createConstantLengthIntegerArray(hid_t here,
//         int rank,
//         hsize_t *dims,
//         hsize_t *maxdims,
//         std::string name,
//         std::string attribute)
// {
// #ifdef HDF5_CREATE_CHECK
//     cout << "HDF5 array const-length (int) create: " << name << "\n";
//     cout << "    rank = " << rank << "\n";
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "          dims[" << i << "] = " << dims[i] << "\n";
//     }
//     for (int i = 0; i < rank; i++)
//     {
//         cout << "       maxdims[" << i << "] = " << maxdims[i] << "\n";
//     }

// #endif
//     int fill_value = 0;

//     //Setup the creation property list
//     dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
//     dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);


//     status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_INT, &fill_value);
//     hdf5_check_error(status);


//     //Create the data description both for data in file and in memory
//     hid_t id_dataspace;
//     id_dataspace = H5Screate_simple(rank , dims     , maxdims     );       // create dataspace

//     // Create the dataset
//     hid_t id_array = H5Dcreate2(here,
//                                 name.c_str(),
//                                 H5T_NATIVE_INT,
//                                 id_dataspace,
//                                 H5P_DEFAULT,
//                                 dataset_creation_plist,
//                                 H5P_DEFAULT);
//     setAttribute(id_array, attribute);
//     H5Sclose(id_dataspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }




// hid_t H5OutputWriter::writeVariableLengthDoubleArray(hid_t id_array,
//         int datarank,
//         hsize_t *dims,
//         hsize_t *data_dims,
//         hsize_t *offset,
//         hsize_t *stride,
//         hsize_t *count,
//         hsize_t *block,
//         double *data)
// {
//     // Extend it if necesary!
//     if (dims != 0 && data != 0)
//     {
//         status =  H5Dset_extent( id_array, dims ); // Needs to be avoided for Displacement and Outputs arrays, they are
//         //extended collectively in setTime() function
//         hdf5_check_error(status);
//     }

//     //Get pointer to the dataspace and create the memory space
//     hsize_t id_dataspace = 0;
//     hsize_t id_memspace = 0;

//     id_dataspace = H5Dget_space(id_array);
//     id_memspace = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

//     if (data != 0)
//     {
//         //Select the region of data to output to
//         status = H5Sselect_hyperslab(
//                      id_dataspace,          // Id of the parent dataspace
//                      H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                      offset,                // start of selection
//                      stride,                // stride in each dimension, NULL  is select everything
//                      count ,                // how many blocks to select in each direction
//                      block                  // little block selected per selection
//                  );
//     }
//     else
//     {
//         status = H5Sselect_none(id_dataspace);
//         status = H5Sselect_none(id_memspace);
//     }
//     hdf5_check_error(status);

// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_DOUBLE,     // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  dataset_xfer_plist,           // Form of writing
//                  data                   // The actual data
//              );

//     H5D_mpio_actual_io_mode_t actual_io_mode;
//     H5Pget_mpio_actual_io_mode( dataset_xfer_plist, &actual_io_mode);

//     switch (actual_io_mode)
//     {

//     case H5D_MPIO_NO_COLLECTIVE: //     No collective I/O was performed. Collective I/O was not requested or collective I/O isn't possible on this dataset.
//         numof_NO_COLLECTIVE_calls++;
//         break;
//     case H5D_MPIO_CHUNK_INDEPENDENT:        // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed independently.
//         numof_CHUNK_INDEPENDENT_calls++;
//         break;
//     case H5D_MPIO_CHUNK_COLLECTIVE:     // HDF5 performed one the chunk collective optimization schemes and each chunk was accessed collectively.
//         numof_CHUNK_COLLECTIVE_calls++;
//         break;
//     case H5D_MPIO_CHUNK_MIXED:            // HDF5 performed one the chunk collective optimization schemes and some chunks were accessed independently, some collectively.
//         numof_CHUNK_MIXED_calls++;
//         break;
//     }
// #else

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_DOUBLE,     // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  H5P_DEFAULT,           // Form of writing
//                  data                   // The actual data
//              );
// #endif
// //------------------------

//     hdf5_check_error(status);

//     //Close stuff
//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }


// hid_t H5OutputWriter::writeVariableLengthIntegerArray(hid_t id_array,
//         int datarank,
//         hsize_t *dims,
//         hsize_t *data_dims,
//         hsize_t *offset,
//         hsize_t *stride,
//         hsize_t *count,
//         hsize_t *block,
//         int *data)
// {
//     // Extend it if necesary!
//     status =  H5Dset_extent( id_array, dims );
//     hdf5_check_error(status);

//     //Get pointer to the dataspace and create the memory space
//     hsize_t id_dataspace = H5Dget_space(id_array);
//     hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

//     //Select the region of data to output to
//     status = H5Sselect_hyperslab(
//                  id_dataspace,              // Id of the parent dataspace
//                  H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                  offset,                // start of selection
//                  stride,                // stride in each dimension, NULL  is select everything
//                  count ,                // how many blocks to select in each direction
//                  block                  // little block selected per selection
//              );
//     hdf5_check_error(status);




//     //Added By Babak 5/31/14
//     //------------------------
// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     //Write data!
//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_INT,        // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  dataset_xfer_plist,           // Form of writing
//                  data                   // The actual data

//              );


// #else

//     //Write data!
//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_INT,        // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  H5P_DEFAULT,           // Form of writing
//                  data                   // The actual data
//              );

// #endif
//     //------------------------

//     //Write data!
//     //     status = H5Dwrite(
//     //                  id_array,              // Dataset to write to
//     //                  H5T_NATIVE_INT,        // Format of data in memory
//     //                  id_memspace,           // Description of data in memory
//     //                  id_dataspace,          // Description of data in storage (including selection)
//     //                  H5P_DEFAULT,           // Form of writing
//     //                  data                   // The actual data
//     hdf5_check_error(status);

//     //Close stuff
//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }



// hid_t H5OutputWriter::writeVariableLengthStringArray(hid_t id_array,
//         hsize_t offset_,
//         hsize_t datasize,
//         std::string &data)
// {
//     int datarank = 1;
//     hsize_t dims[1] = {offset_ + 1};
//     hsize_t data_dims[1] = {1};
//     hsize_t stride[1] = {1};
//     hsize_t count[1] = {1};
//     hsize_t block[1] = {1};
//     hsize_t offset[1] = {offset_};



//     // Extend it if necesary!
//     status =  H5Dset_extent( id_array, dims );
//     hdf5_check_error(status);

//     //Get pointer to the dataspace and create the memory space
//     hsize_t id_dataspace = H5Dget_space(id_array);
//     hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

//     //Select the region of data to output to
//     status = H5Sselect_hyperslab(
//                  id_dataspace,              // Id of the parent dataspace
//                  H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                  offset,                // start of selection
//                  stride,                // stride in each dimension, NULL  is select everything
//                  count ,                // how many blocks to select in each direction
//                  block                  // little block selected per selection
//              );
//     hdf5_check_error(status);

//     //Write data!

//     hid_t type = H5Tcopy(H5T_C_S1);
//     status = H5Tset_size(type, H5T_VARIABLE);
//     hdf5_check_error(status);


//     const char *dat = data.c_str();
//     char *thedata[1] = {(char *)dat};




//     //Added By Babak 5/31/14
//     //------------------------
// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  type,                  // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  dataset_xfer_plist,           // Form of writing
//                  thedata// The actual data
//              );

// #else
//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  type,                  // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  H5P_DEFAULT,           // Form of writing
//                  thedata// The actual data
//              );
// #endif
//     //------------------------

//     //     status = H5Dwrite(
//     //                  id_array,              // Dataset to write to
//     //                  type,                  // Format of data in memory
//     //                  id_memspace,           // Description of data in memory
//     //                  id_dataspace,          // Description of data in storage (including selection)
//     //                  H5P_DEFAULT,           // Form of writing
//     //                  thedata// The actual data
//     //              );


//     // cout << "done! \n\n";

//     hdf5_check_error(status);

//     //Close stuff
//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }




// hid_t H5OutputWriter::writeConstantLengthDoubleArray(hid_t id_array,
//         int datarank,
//         hsize_t *dims,
//         hsize_t *data_dims,
//         hsize_t *offset,
//         hsize_t *stride,
//         hsize_t *count,
//         hsize_t *block,
//         double *data)
// {
//     //Get pointer to the dataspace and create the memory space
//     hsize_t id_dataspace = H5Dget_space(id_array);
//     hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

//     //Select the region of data to output to
//     status = H5Sselect_hyperslab(
//                  id_dataspace,          // Id of the parent dataspace
//                  H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                  offset,                // start of selection
//                  stride,                // stride in each dimension, NULL  is select everything
//                  count ,                // how many blocks to select in each direction
//                  block                  // little block selected per selection
//              );
//     hdf5_check_error(status);



//     //Added By Babak 5/31/14
//     //------------------------
// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_DOUBLE,     // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  dataset_xfer_plist,           // Form of writing
//                  data                   // The actual data
//              );
// #else

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_DOUBLE,     // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  H5P_DEFAULT,           // Form of writing
//                  data                   // The actual data
//              );
// #endif
//     //------------------------
//     //Write data!
//     //     status = H5Dwrite(
//     //                  id_array,              // Dataset to write to
//     //                  H5T_NATIVE_DOUBLE,     // Format of data in memory
//     //                  id_memspace,           // Description of data in memory
//     //                  id_dataspace,          // Description of data in storage (including selection)
//     //                  H5P_DEFAULT,           // Form of writing
//     //                  data                   // The actual data
//     //              );
//     hdf5_check_error(status);

//     //Close stuff
//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }






// hid_t H5OutputWriter::writeConstantLengthIntegerArray(hid_t id_array,
//         int datarank,
//         hsize_t *dims,
//         hsize_t *data_dims,
//         hsize_t *offset,
//         hsize_t *stride,
//         hsize_t *count,
//         hsize_t *block,
//         int *data)
// {
//     //Get pointer to the dataspace and create the memory space
//     hsize_t id_dataspace = H5Dget_space(id_array);
//     hsize_t id_memspace  = H5Screate_simple(datarank   , data_dims, data_dims);       // create dataspace

//     //Select the region of data to output to
//     status = H5Sselect_hyperslab(
//                  id_dataspace,          // Id of the parent dataspace
//                  H5S_SELECT_SET,        // Selection operatior H5S_SELECT_<>, where <> = {SET, OR, AND, XOR, NOTB, NOTA}
//                  offset,                // start of selection
//                  stride,                // stride in each dimension, NULL  is select everything
//                  count ,                // how many blocks to select in each direction
//                  block                  // little block selected per selection
//              );
//     hdf5_check_error(status);


//     //Added By Babak 5/31/14
//     //------------------------
// #ifdef _PARALLEL_PROCESSING_COLLECTIVE_IO

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_INT,        // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  dataset_xfer_plist,           // Form of writing
//                  data                   // The actual data
//              );
// #else

//     status = H5Dwrite(
//                  id_array,              // Dataset to write to
//                  H5T_NATIVE_INT,        // Format of data in memory
//                  id_memspace,           // Description of data in memory
//                  id_dataspace,          // Description of data in storage (including selection)
//                  H5P_DEFAULT,           // Form of writing
//                  data                   // The actual data
//              );
// #endif
//     //------------------------


//     //Write data!
//     //     status = H5Dwrite(
//     //                  id_array,              // Dataset to write to
//     //                  H5T_NATIVE_INT,        // Format of data in memory
//     //                  id_memspace,           // Description of data in memory
//     //                  id_dataspace,          // Description of data in storage (including selection)
//     //                  H5P_DEFAULT,           // Form of writing
//     //                  data                   // The actual data
//     //              );
//     hdf5_check_error(status);

//     //Close stuff
//     H5Sclose(id_dataspace);
//     H5Sclose(id_memspace);
//     H5OUTPUTWRITER_COUNT_OBJS;
//     return id_array;
// }

// void H5OutputWriter::set_zlib_compression_level(int level)
// {
//     zlib_compression_level = level;
// }


// void call_hdf5_flush_next_chance()
// {
//     H5OutputWriter::call_hdf5_flush = true;
// }


// inline void hdf5_check_error(herr_t status)
// {
//     if (status < 0)
//     {
//         cout << "status = " << status << endl;
//         print_stacktrace();
//         exit(-1);
//     }
// }
