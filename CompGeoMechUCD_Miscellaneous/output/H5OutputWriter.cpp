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
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <ID.h>
#include <Vector.h>
#include <H5OutputWriter.h>
#include <hdf5.h>


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
    initialize(filename_in,
               model_name_in,
               stage_name_in,
               nsteps);
}

H5OutputWriter::~H5OutputWriter()
{

}




void H5OutputWriter::initialize(std::string filename_in,
                                std::string model_name_in,
                                std::string stage_name_in,
                                int nsteps)
{

    if (file_is_open)
    {
        H5close();
        file_is_open = false;
    }

    file_name = "";
    file_name += filename_in;
    model_name += model_name_in;
    stage_name += stage_name_in;


    current_time         = 0.0;
    current_time_step    = 0;
    number_of_nodes                      = 0;
    number_of_elements                   = 0;
    number_of_time_steps                 = nsteps;
    max_node_tag                         = 0;
    max_element_tag                      = 0;
    length_nodes_displacements_output    = 0;
    length_nodes_velocities_output       = 0;
    length_nodes_accelerations_output    = 0;
    length_nodes_reaction_forcess_output = 0;
    length_element_output                = 0;


    //================================================================================
    //Create the file, overwriting it if it exists
    //================================================================================
    unsigned flags = H5F_ACC_TRUNC;  // Truncate file (ie. if file exists overwrite it)
    hid_t file_creation_plist = H5Pcreate(H5P_FILE_CREATE);
    hid_t file_access_plist = H5Pcreate(H5P_FILE_ACCESS);
    // status = H5Pset_meta_block_size(file_access_plist, H5OUTPUTWRITER_META_BLOCK_SIZE);
    // HDF5_CHECK_ERROR;
    // status =  H5Pset_cache(file_access_plist, 0, H5OUTPUTWRITER_CHUNK_NSLOTS, H5OUTPUTWRITER_CHUNK_NBYTES, 0 );
    // HDF5_CHECK_ERROR;
    status = H5Pset_sieve_buf_size( file_access_plist, H5OUTPUTWRITER_SIEVE_BUFFER_SIZE );
    HDF5_CHECK_ERROR;
    id_file = H5Fcreate(file_name.c_str(), flags , file_creation_plist, file_access_plist);
    file_is_open = true;


    //================================================================================
    //Setup the global property lists
    //================================================================================
    group_creation_plist = H5Pcreate(H5P_GROUP_CREATE);
    status = H5Pset_link_creation_order(group_creation_plist, H5P_CRT_ORDER_TRACKED | H5P_CRT_ORDER_INDEXED);
    HDF5_CHECK_ERROR;



    //================================================================================
    // Create basic structure of the file
    //================================================================================
    //Create the model name, stage name and number of steps fields
    write_string(id_file, "Model_Name", model_name);
    write_string(id_file, "Stage_Name", stage_name);


    //Create the group to store the model
    id_model_group = create_group(id_file, "Model");

    //Create the time vector


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


    H5OUTPUTWRITER_COUNT_OBJS;
    // cout << "subgroupname" << subgroupname << endl;

}














// Mesh output


int H5OutputWriter::writeNumberOfNodes(unsigned int number_of_nodes_in )
{
    number_of_nodes = number_of_nodes_in;


    return 0;
}

int H5OutputWriter::writeNumberOfElements(unsigned int numberOfElements_ )
{
    number_of_elements = number_of_elements;
    return 0;
}


int H5OutputWriter::writeNodeData(int tag     , const Vector &coords   , int ndofs )
{

    static bool first_time_running_this_function = true;

    if (first_time_running_this_function)
    {

        int rank = 1;
        hsize_t dims[2];
        hsize_t maxdims[2];
        dims[0] = 1;
        maxdims[0] = H5S_UNLIMITED;

        id_nodes_ndofs          = createVariableLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Number_of_DOFs", " ");

        dims[0] = 1;
        maxdims[0] = H5S_UNLIMITED;
        id_nodes_coordinates = createVariableLengthDoubleArray(id_nodes_group, rank, dims, maxdims, "Coordinates", " ");
        id_index_to_nodes_coordinates = createVariableLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Coordinates_index", " ");
        id_index_to_nodes_outputs = createVariableLengthIntegerArray(id_nodes_group, rank, dims, maxdims, "Nodes_Output_Index", " ");

        first_time_running_this_function = false;
    }

    //Update tag
    if (tag > max_node_tag)
    {
        max_node_tag = tag;
    }



    static  int pos_nodes_ouputs = 0;
    static  int pos_nodes_coordinates = 0;

    // Write a vector with the number of DOFS for node at a given tag
    int datarank         = 1;
    hsize_t dims[1]      = {(hsize_t) max_node_tag };
    hsize_t data_dims[1] = {1};
    hsize_t offset[1]    = {(hsize_t)tag - 1};
    hsize_t stride[1]    = {1};
    hsize_t count[1]     = {1};
    hsize_t block[1]     = {1};
    int *data = &ndofs;
    writeVariableLengthIntegerArray(id_nodes_ndofs,
                                    datarank,
                                    dims,
                                    data_dims,
                                    offset,
                                    stride,
                                    count,
                                    block,
                                    data);


    //Write index to outputs
    dims[0]      = max_node_tag;
    offset[0]    = tag - 1;
    data = &pos_nodes_ouputs;
    writeVariableLengthIntegerArray(id_index_to_nodes_outputs,
                                    datarank,
                                    dims,
                                    data_dims,
                                    offset,
                                    stride,
                                    count,
                                    block,
                                    data);
    pos_nodes_ouputs += ndofs;



    //Write index to coordinates
    dims[0]      = max_node_tag;
    offset[0]    = tag - 1;
    data = &pos_nodes_coordinates;
    writeVariableLengthIntegerArray(id_index_to_nodes_coordinates,
                                    datarank,
                                    dims,
                                    data_dims,
                                    offset,
                                    stride,
                                    count,
                                    block,
                                    data);


    //Write coordinate values
    dims[0]      = pos_nodes_coordinates + 3;
    data_dims[0] = 3;
    offset[0]    = pos_nodes_coordinates;
    count[0]     = 3;
    double *coorddata = coords.theData;
    writeVariableLengthDoubleArray(id_nodes_coordinates,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   coorddata);

    pos_nodes_coordinates += 3;
    number_of_nodes++;


    length_nodes_accelerations_output    = pos_nodes_ouputs;
    length_nodes_velocities_output       = pos_nodes_ouputs;
    length_nodes_accelerations_output    = pos_nodes_ouputs;
    length_nodes_reaction_forcess_output = pos_nodes_ouputs;

    return 0;
}

int H5OutputWriter::writeElementData(int tag  , std::string type , ID &connectivity , int materialtag , Vector &parameters,
                                     int length_of_output)
{
    return 0;
}
int H5OutputWriter::writeMaterialData(int tag , std::string type , Vector &parameters)
{
    return 0;
}

// Results for Nodes
int H5OutputWriter::writeDisplacements(  int nodeTag, const Vector &displacements)
{
    static bool first_time_running_this_function = true;

    if (first_time_running_this_function)
    {
        int rank = 2;
        hsize_t dims[2];
        hsize_t maxdims[2];
        dims[0] = length_nodes_displacements_output;
        dims[1] = 1;
        maxdims[0] = length_nodes_displacements_output;
        maxdims[1] = H5S_UNLIMITED;

        id_nodes_displacements = createVariableLengthDoubleArray(id_nodes_group, rank, dims, maxdims, "Displacements", " ");

        first_time_running_this_function = false;
    }

    int pos, ndofs;

    // Read NDOFS from HDF5 file
    int datarank         = 1;
    hsize_t data_dims[1] = {1};
    hsize_t offset[2]    = {nodeTag - 1, 0};
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
    HDF5_CHECK_ERROR;
    H5Dread(id_nodes_ndofs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &ndofs);
    H5Dread(id_index_to_nodes_outputs, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, &pos);

    hsize_t dims[2]      =  { length_nodes_displacements_output, current_time_step};
    data_dims[0] = ndofs;

    offset[0]    = pos;
    offset[1]    = current_time_step;
    stride[1]    = 1;
    count[1]     = ndofs;
    block[1]     = 1;
    double *data = displacements.theData;
    writeVariableLengthDoubleArray(id_nodes_ndofs,
                                   datarank,
                                   dims,
                                   data_dims,
                                   offset,
                                   stride,
                                   count,
                                   block,
                                   data);

    return 0;
}


int H5OutputWriter::writeVelocities(     int nodeTag, const Vector &velocities)
{
    return 0;
}


int H5OutputWriter::writeAccelerations(  int nodeTag, const Vector &accelerations)
{
    return 0;
}


int H5OutputWriter::writeReactionForces( int nodeTag, const Vector &reactionForces)
{
    return 0;
}



// Results for Elements
int H5OutputWriter::writeElementOutput(int elementTag, Vector &output)
{
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

    current_time_step++;
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
    HDF5_CHECK_ERROR;
    dims_string[0] = 1;//contents.size();
    id_dataspace_string = H5Screate_simple(1, dims_string, NULL);
    id_data_string = H5Dcreate2(here,
                                name.c_str(),
                                id_type_string,
                                id_dataspace_string,
                                H5P_DEFAULT,
                                H5P_DEFAULT,
                                H5P_DEFAULT);
    status = H5Dwrite(id_data_string,
                      id_type_string,
                      H5S_ALL,
                      H5S_ALL,
                      H5P_DEFAULT,
                      contents.c_str());

    H5Sclose(id_dataspace_string);
    H5Dclose(id_data_string);

    HDF5_CHECK_ERROR;

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


    status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk_cache(dataset_access_plist, H5OUTPUTWRITER_CHUNK_NSLOTS, nbytes_one_chunk * H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK, 1 );
    HDF5_CHECK_ERROR;
    status = H5Pset_fill_value(dataset_creation_plist, type, fill_value_ptr);
    HDF5_CHECK_ERROR;
    status = H5Pset_fill_time(dataset_creation_plist, H5D_FILL_TIME_ALLOC);// H5D_FILL_TIME_NEVER);
    HDF5_CHECK_ERROR;
    // status = H5Pset_deflate (dataset_creation_plist, 6);
    // HDF5_CHECK_ERROR;



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
    // H5Sclose(id_dataspace);

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
    HDF5_CHECK_ERROR;


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
    // H5Sclose(id_dataspace);

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
    HDF5_CHECK_ERROR;


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
    // H5Sclose(id_dataspace);

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
    HDF5_CHECK_ERROR;

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
    HDF5_CHECK_ERROR;

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
    HDF5_CHECK_ERROR;

    //Close stuff
    // H5Sclose(id_dataspace);
    // H5Sclose(id_memspace);

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
    HDF5_CHECK_ERROR;

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
    HDF5_CHECK_ERROR;

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
    HDF5_CHECK_ERROR;

    //Close stuff
    // H5Sclose(id_dataspace);
    // H5Sclose(id_memspace);

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
    HDF5_CHECK_ERROR;

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_DOUBLE,     // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
    HDF5_CHECK_ERROR;

    //Close stuff
    // H5Sclose(id_dataspace);
    // H5Sclose(id_memspace);

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
    HDF5_CHECK_ERROR;

    //Write data!
    status = H5Dwrite(
                 id_array,              // Dataset to write to
                 H5T_NATIVE_INT,        // Format of data in memory
                 id_memspace,           // Description of data in memory
                 id_dataspace,          // Description of data in storage (including selection)
                 H5P_DEFAULT,           // Form of writing
                 data                   // The actual data
             );
    HDF5_CHECK_ERROR;

    //Close stuff
    // H5Sclose(id_dataspace);
    // H5Sclose(id_memspace);

    return id_array;
}
