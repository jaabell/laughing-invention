// ===============================================================================
// ESSI Simulator Program - UCD CompGeoMech
//
// COPYRIGHT (C):     ^_^
// PROJECT:           ESSI Simulator
// FILE:              HDF5_Channel.h
// CLASS:             HDF5_Channel
// VERSION:
// LANGUAGE:          C++ /
// TARGET OS:         UNIX || maybe something else
// DESIGNER:          Jose Abell, Babak Kamaranimogghadam, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              Feb 2014
// UPDATE HISTORY:
//
// ===============================================================================



#include <Channel.h>
#include <HDF5_Channel.h>
#include <Message.h>
#include <MovableObject.h>
#include <FEM_ObjectBroker.h>
#include <hdf5.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <Vector.h>
#include <Matrix.h>
#include <ID.h>


int HDF5_Channel::numChannel = 0;



HDF5_Channel::HDF5_Channel ():
    file_is_open(false),
    number_of_time_steps(0),
    filename(""),
    model_name(""),
    stage_name("")
{
    //nuthin
}

// This constructor is used to create a new output file
HDF5_Channel::HDF5_Channel (std::string filename_in,
                            std::string model_name_in,
                            std::string stage_name_in,
                            int nsteps):
    file_is_open(false),
    number_of_time_steps(0),
    filename(""),
    model_name(""),
    stage_name(""),
    subgroupname("")
{
    initialize(filename_in, model_name_in, stage_name_in, nsteps);
}

void HDF5_Channel::initialize(std::string filename_in,
                              std::string model_name_in,
                              std::string stage_name_in,
                              int nsteps)
{

    if (file_is_open)
    {
        H5close();
        file_is_open = false;
    }

    number_of_time_steps = nsteps;
    filename = "";
    filename += filename_in;
    model_name += model_name_in;
    stage_name += stage_name_in;
    subgroupname = "hi";


    numChannel++;
    tag = numChannel;

    current_time = 0.0;
    current_step_number = 0;


    //std::cout << "Opening file" << endl;

    //================================================================================
    //Create the file, overwriting it if it exists
    //================================================================================
    unsigned flags = H5F_ACC_TRUNC;  // Truncate file (ie. if file exists overwrite it)
    hid_t file_creation_plist = H5Pcreate(H5P_FILE_CREATE);
    hid_t file_access_plist = H5Pcreate(H5P_FILE_ACCESS);
    // status = H5Pset_meta_block_size(file_access_plist, HDF5_CHANNEL_META_BLOCK_SIZE);
    // HDF5_CHECK_ERROR;
    // status =  H5Pset_cache(file_access_plist, 0, HDF5_CHANNEL_CHUNK_NSLOTS, HDF5_CHANNEL_CHUNK_NBYTES, 0 );
    // HDF5_CHECK_ERROR;
    status = H5Pset_sieve_buf_size( file_access_plist, HDF5_CHANNEL_SIEVE_BUFFER_SIZE );
    HDF5_CHECK_ERROR;
    id_file = H5Fcreate(filename.c_str(), flags , file_creation_plist, file_access_plist);
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

    stack_length = 0;

    inElementDefinitionMode = false;
    inNodeDefinitionMode = false;

    HDF5_CHANNEL_COUNT_OBJS;
    // cout << "subgroupname" << subgroupname << endl;

}

HDF5_Channel::~HDF5_Channel()
{
    // ssize_t nchars;
    // size_t NMAXCHARS = 80;
    // char *name = 0;
    // nchars = H5Fget_name(id_file, name, NMAXCHARS);

    // std::string namae(name, nchars);

    // cout << "Closing: " << namae << endl;

    // H5Gclose(id_model_group);
    // H5Gclose(id_nodes_group);
    // H5Gclose(id_elements_group);

    // H5Fclose(id_file);
    // H5close();

}

int
HDF5_Channel::isDatastore(void)
{
    return 0;
}

int
HDF5_Channel::getDbTag(void)
{
    return 0;
}

int
HDF5_Channel::getTag(void)
{
    return tag;
}



// methods to send/receive messages and objects on HDF5_channels.
int HDF5_Channel::sendObj(int commitTag,
                          MovableObject &theObject,
                          ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::recvObj(int commitTag,
                          MovableObject &theObject,
                          FEM_ObjectBroker &theBroker,
                          ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::sendMsg(int dbTag, int commitTag,
                          const Message &theMessage,
                          ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::recvMsg(int dbTag, int commitTag,
                          Message &theMessage,
                          ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::sendMatrix(int dbTag, int commitTag,
                             const Matrix &theMatrix,
                             ChannelAddress *theAddress )
{
    HDF5_CHANNEL_CHECK_DEFINITION_MODE;
    // This function first checks if the field has to be created in the HDF5 file.
    // (1) if the field has to be created
    //      (i) If the field is time varying, an (extendible) 2D array will be created with time on the
    //      second index.
    //      (ii) If it is not time varying only a 1D array is created.
    // (2) if the field is already created
    //      (i) if it is time varying, write out the new data
    //      (ii) if it is not time-varying, then do nothing the data has already been written out.
    //
    // The decision to create the field is based on searching the database.
    // Get pointer to the data from the vector
    //--------------------------------------------------------------------------------------

    int size_of_matrix_data, numRows, numCols;
    double *data = theMatrix.data;
    size_of_matrix_data =  theMatrix.dataSize;
    numRows =  theMatrix.numRows;
    numCols =  theMatrix.numCols;
    hid_t id_dataset;    //Points to the object in the file that is the data

    if (stack_length > 0)  // There is info available to write out data
    {
        //Get the field info
        //--------------------------------------------------------------------------------------
        std::string field_name;
        bool is_time_dependent;
        std::string units;
        getNextField(field_name, is_time_dependent, units);

        // Find out whether the field exists
        int exists = H5Lexists_safe(id_current_object, field_name, H5P_DEFAULT);
        int rank;            // Rank of array to be created (2 if time varying and 1 if not)
        hsize_t dims[3], maxdims[3];
        hsize_t data_dims[2];
        hsize_t data_maxdims[2];
        data_dims[0] = numRows;
        data_dims[1] = numCols;
        data_maxdims[0] = numRows;
        data_maxdims[1] = numCols;



        // Field does not exist
        // --------------------------------
        if (!exists) // The field does not exist -> create it and write out
        {
            //Create the field
            //--------------------------------------------------------------------------------------
            // cout << "Adding field: " << subgroupname << field_name << endl;

            if (is_time_dependent) // If it is time dependent
            {
                rank = 3;
                dims[0] = numRows;
                dims[1] = numCols;
                dims[2] = number_of_time_steps;
                maxdims[0] = numRows;
                maxdims[1] = numCols;
                maxdims[2] = H5S_UNLIMITED;

                id_dataset = createVariableLengthDoubleArray(id_current_object,
                             rank,
                             dims,
                             maxdims,
                             field_name,
                             units);

                //Write out the data to the dataset
                hsize_t offset[3] = {       0,  0, 0 };
                hsize_t stride[3] = {       1,  1, 1 };
                hsize_t count[3]  = {       1,  1, 1 };
                hsize_t block[3]  = { dims[0],  dims[1], 1 };
                id_dataset = writeVariableLengthDoubleArray(id_dataset,
                             2, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);

            }
            else  // If it is not time dependent
            {
                rank = 2;
                dims[0] = numRows;        // Starting array dims
                dims[1] = numCols;        // Starting array dims
                maxdims[0] = numRows;     // Max array dims
                maxdims[1] = numCols;     // Max array dims

                hid_t id_dataset = createConstantLengthDoubleArray(id_current_object,
                                   rank,
                                   dims,
                                   maxdims,
                                   field_name,
                                   units);

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  0 };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  dims[1] };

                id_dataset = writeConstantLengthDoubleArray(id_dataset,
                             2, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
        }

        // Field exists .....
        // --------------------------------
        else if (is_time_dependent) // .... and is time dependent
        {

            // cout << "Writing field: " << subgroupname << field_name << endl;
            //First get information about object
            id_dataset = H5Oopen(id_current_object, field_name.c_str(), H5P_DEFAULT);
            if (id_dataset > 0)
            {
                rank = 3;
                dims[0] = numRows;
                dims[1] = numCols;
                dims[2] = number_of_time_steps;

                //Write out the data to the dataset
                hsize_t offset[3] = {       0,      0,  current_step_number - 1  };
                hsize_t stride[3] = {       1,      1,  1 };
                hsize_t count[3]  = {       1,      1,  1 };
                hsize_t block[3]  = { dims[0], dims[1],  1 };

                id_dataset = writeVariableLengthDoubleArray(id_dataset,
                             2,// datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
            else
            {
                cerr << "HDF5_Channel::sendVector() -> ";
                cerr << "Group named " << field_name << " does not exist." << endl; // This should not happen...... ever
            }
        } // The field either does not exist (do nothing), or is not time dependent (do nothing)

    } // if(stack_length > 0 ) -> cannot do anything without a stack

    HDF5_CHANNEL_CLEAN;
    HDF5_CHANNEL_COUNT_OBJS;
    return 0;
}

int HDF5_Channel::recvMatrix(int dbTag, int commitTag,
                             Matrix &theMatrix,
                             ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::sendVector(int id_object, int commitTag,
                             const Vector &theVector,
                             ChannelAddress *theAddress )
{
    HDF5_CHANNEL_CHECK_DEFINITION_MODE;
    // This function first checks if the field has to be created in the HDF5 file.
    // (1) if the field has to be created
    //      (i) If the field is time varying, an (extendible) 2D array will be created with time on the
    //      second index.
    //      (ii) If it is not time varying only a 1D array is created.
    // (2) if the field is already created
    //      (i) if it is time varying, write out the new data
    //      (ii) if it is not time-varying, then do nothing the data has already been written out.
    //
    // The decision to create the field is based on searching the database.
    // Get pointer to the data from the vector
    //--------------------------------------------------------------------------------------

    int length_of_vector_data;
    double *data = theVector.theData;
    length_of_vector_data =  theVector.sz;
    hid_t id_dataset;    //Points to the object in the file that is the data

    if (stack_length > 0)  // There is info available to write out data
    {
        //Get the field info
        //--------------------------------------------------------------------------------------
        std::string field_name;
        bool is_time_dependent;
        std::string units;
        getNextField(field_name, is_time_dependent, units);

        // Find out whether the field exists
        int exists = H5Lexists_safe(id_current_object, field_name, H5P_DEFAULT);
        int rank;            // Rank of array to be created (2 if time varying and 1 if not)
        hsize_t dims[2], maxdims[2];
        hsize_t data_dims[1];
        hsize_t data_maxdims[1];
        data_dims[0] = length_of_vector_data;
        data_maxdims[0] = length_of_vector_data;



        // Field does not exist
        // --------------------------------
        if (!exists) // The field does not exist -> create it and write out
        {
            //Create the field
            //--------------------------------------------------------------------------------------
            // cout << "Adding field: " << subgroupname << field_name << endl;

            if (is_time_dependent) // If it is time dependent
            {
                rank = 2;
                dims[0] = length_of_vector_data;
                dims[1] = number_of_time_steps;
                maxdims[0] = length_of_vector_data;
                maxdims[1] = H5S_UNLIMITED;

                id_dataset = createVariableLengthDoubleArray(id_current_object,
                             rank,
                             dims,
                             maxdims,
                             field_name,
                             units);

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  0 };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };
                id_dataset = writeVariableLengthDoubleArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);

            }
            else  // If it is not time dependent
            {
                rank = 1;
                dims[0] = length_of_vector_data;        // Starting array dims
                maxdims[0] = length_of_vector_data;     // Max array dims

                hid_t id_dataset = createConstantLengthDoubleArray(id_current_object,
                                   rank,
                                   dims,
                                   maxdims,
                                   field_name,
                                   units);

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  0 };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };

                id_dataset = writeConstantLengthDoubleArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
        }

        // Field exists .....
        // --------------------------------
        else if (is_time_dependent) // .... and is time dependent
        {

            // cout << "Writing field: " << subgroupname << field_name << endl;
            //First get information about object
            id_dataset = H5Oopen(id_current_object, field_name.c_str(), H5P_DEFAULT);
            if (id_dataset > 0)
            {
                rank = 2;
                dims[0] = length_of_vector_data;
                dims[1] = number_of_time_steps;

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  current_step_number - 1  };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };

                id_dataset = writeVariableLengthDoubleArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
            else
            {
                cerr << "HDF5_Channel::sendVector() -> ";
                cerr << "Group named " << field_name << " does not exist." << endl; // This should not happen...... ever
            }
        } // The field either does not exist (do nothing), or is not time dependent (do nothing)

    } // if(stack_length > 0 ) -> cannot do anything without a stack

    HDF5_CHANNEL_CLEAN;
    HDF5_CHANNEL_COUNT_OBJS;
    return 0;
}

int HDF5_Channel::recvVector(int dbTag, int commitTag,
                             Vector &theVector,
                             ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::sendID(int dbTag, int commitTag,
                         const ID &theID,
                         ChannelAddress *theAddress )
{
    HDF5_CHANNEL_CHECK_DEFINITION_MODE;
    // This function first checks if the field has to be created in the HDF5 file.
    // (1) if the field has to be created
    //      (i) If the field is time varying, an (extendible) 2D array will be created with time on the
    //      second index.
    //      (ii) If it is not time varying only a 1D array is created.
    // (2) if the field is already created
    //      (i) if it is time varying, write out the new data
    //      (ii) if it is not time-varying, then do nothing the data has already been written out.
    //
    // The decision to create the field is based on searching the database.
    // Get pointer to the data from the vector
    //--------------------------------------------------------------------------------------


    int length_of_vector_data;
    int *data = theID.data;
    length_of_vector_data =  theID.sz;
    hid_t id_dataset;    //Points to the object in the file that is the data

    if (stack_length > 0)  // There is info available to write out data
    {
        //Get the field info
        //--------------------------------------------------------------------------------------
        std::string field_name;
        bool is_time_dependent;
        std::string units;
        getNextField(field_name, is_time_dependent, units);

        // Find out whether the field exists
        int exists = H5Lexists_safe(id_current_object, field_name, H5P_DEFAULT);
        int rank;            // Rank of array to be created (2 if time varying and 1 if not)
        hsize_t dims[2], maxdims[2];
        hsize_t data_dims[1];
        hsize_t data_maxdims[1];
        data_dims[0] = length_of_vector_data;
        data_maxdims[0] = length_of_vector_data;



        // Field does not exist
        // --------------------------------
        if (!exists) // The field does not exist -> create it and write out
        {
            //Create the field
            //--------------------------------------------------------------------------------------
            // cout << "Adding field: " << subgroupname << field_name << endl;

            if (is_time_dependent) // If it is time dependent
            {
                rank = 2;
                dims[0] = length_of_vector_data;
                dims[1] = number_of_time_steps;
                maxdims[0] = length_of_vector_data;
                maxdims[1] = H5S_UNLIMITED;

                id_dataset = createVariableLengthIntegerArray(id_current_object,
                             rank,
                             dims,
                             maxdims,
                             field_name,
                             units);

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  0 };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };
                id_dataset = writeVariableLengthIntegerArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);

            }
            else  // If it is not time dependent
            {
                rank = 1;
                dims[0] = length_of_vector_data;        // Starting array dims
                maxdims[0] = length_of_vector_data;     // Max array dims

                hid_t id_dataset = createConstantLengthIntegerArray(id_current_object,
                                   rank,
                                   dims,
                                   maxdims,
                                   field_name,
                                   units);

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  0 };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };

                id_dataset = writeConstantLengthIntegerArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
        }

        // Field exists .....
        // --------------------------------
        else if (is_time_dependent) // .... and is time dependent
        {

            // cout << "Writing field: " << subgroupname << field_name << endl;
            //First get information about object
            id_dataset = H5Oopen(id_current_object, field_name.c_str(), H5P_DEFAULT);
            if (id_dataset > 0)
            {
                rank = 2;
                dims[0] = length_of_vector_data;
                dims[1] = number_of_time_steps;

                //Write out the data to the dataset
                hsize_t offset[2] = {       0,  current_step_number - 1  };
                hsize_t stride[2] = {       1,  1 };
                hsize_t count[2]  = {       1,  1 };
                hsize_t block[2]  = { dims[0],  1 };

                id_dataset = writeVariableLengthIntegerArray(id_dataset,
                             1, //datarank
                             dims,
                             data_dims,
                             offset,
                             stride,
                             count,
                             block,
                             data);
                H5Oclose(id_dataset);
            }
            else
            {
                cerr << "HDF5_Channel::sendVector() -> ";
                cerr << "Group named " << field_name << " does not exist." << endl; // This should not happen...... ever
            }
        } // The field either does not exist (do nothing), or is not time dependent (do nothing)

    } // if(stack_length > 0 ) -> cannot do anything without a stack

    HDF5_CHANNEL_CLEAN;
    HDF5_CHANNEL_COUNT_OBJS;

    return 0;
}

int HDF5_Channel::recvID(int dbTag, int commitTag,
                         ID &theID,
                         ChannelAddress *theAddress )
{
    return 0;
}

//Guanzhou added
int HDF5_Channel::sendnDarray(int dbTag, int commitTag,
                              const nDarray &theNDarray,
                              ChannelAddress *theAddress )
{
    return 0;
}

int HDF5_Channel::recvnDarray(int dbTag, int commitTag,
                              nDarray &theNDarray,
                              ChannelAddress *theAddress )
{
    return 0;
}







//Adding and getting new fields

int HDF5_Channel::addField(std::string field_name,
                           bool is_time_dependent,
                           std::string units)
{
    field_name_stack.push(subgroupname + field_name);
    field_is_time_dependent_stack.push(is_time_dependent);
    field_units_stack.push(units);
    stack_length++;
    return 0;
}


int HDF5_Channel::getNextField(std::string &field_name,
                               bool &is_time_dependent,
                               std::string &units)
{
    if (stack_length > 0)
    {
        field_name = field_name_stack.front();
        is_time_dependent = field_is_time_dependent_stack.front();
        units = field_units_stack.front();
        field_name_stack.pop();
        field_is_time_dependent_stack.pop();
        field_units_stack.pop();
        stack_length--;
        return 0;
    }
    else
    {
        return -1;
    }
}




int HDF5_Channel::beginElementDescription(std::string name, int tag)
{

    inElementDefinitionMode = true;

    stack_length = 0;

    //Set the current group identifier to the elements group
    // id_previous_group = id_current_group;
    // id_current_group = id_elements_group;

    //Convert integer tag to string
    string stag;
    stag = boost::lexical_cast<std::string>(tag);

    //Concatenate name and tag to create group name
    string group_name;
    group_name = name + "_" + stag;

    //Check if group exists
    int element_exists = H5Lexists_safe(id_elements_group, group_name, H5P_DEFAULT);
    if (element_exists) // If group doesn't exist -> create it
    {
        id_current_object = H5Oopen(id_elements_group,
                                    group_name.c_str(),
                                    H5P_DEFAULT);
        id_current_object_group = id_elements_group;
    }
    else
    {
        //Create a group to hold the data for the element
        id_current_object = create_group(id_elements_group, group_name.c_str());
        id_current_object_group = id_elements_group;
    }

    subgroupname = "";

    // cout << "Adding element: " << group_name << endl;

    number_of_materials_in_current_element = 0;

    //Return the tag of this group
    return id_current_object;
}

int HDF5_Channel::beginNodeDescription(int tag)
{
    inNodeDefinitionMode = true;
    subgroupname = "";

    stack_length = 0;

    //Set the current group identifier to the nodes group
    // id_previous_group = id_current_group;
    // id_current_group = id_nodes_group;

    //Convert integer tag to string
    string stag;
    stag = boost::lexical_cast<std::string>(tag);

    //Check if group exists
    int node_exists = H5Lexists_safe(id_nodes_group, stag, H5P_DEFAULT);
    if (node_exists) // If group doesn't exist -> create it
    {
        id_current_object = H5Oopen(id_nodes_group,
                                    stag.c_str(),
                                    H5P_DEFAULT);
        id_current_object_group = id_nodes_group;
    }
    else
    {
        //Create a group to hold the data for the node
        id_current_object = create_group(id_nodes_group, stag.c_str());
        id_current_object_group = id_nodes_group;

    }
    //Return the tag of this group
    return id_current_object_group;
}

int HDF5_Channel::beginMaterialDescription(std::string name, int tag)
{
    //Materials are defined within elements so their stack extends that of an element
    if (inElementDefinitionMode)
    {
        number_of_materials_in_current_element++;
        //Set the current group identifier to the nodes group
        // id_previous_group = id_current_object;
        //id_current_group = id_nodes_group;

        //Convert integer tag to string
        string stag;
        stag = boost::lexical_cast<std::string>(tag);

        //Concatenate name and tag and a number for current material in element
        string group_name;
        group_name = "material_" +  name + "_" + stag + "_" + boost::lexical_cast<std::string>(number_of_materials_in_current_element);

        //Check if group exists
        int material_exists = H5Lexists_safe(id_current_object, group_name, H5P_DEFAULT);
        if (material_exists)
        {
            // Does nothing
        }
        else
        {
            //Create a group to hold the data for the material and close it right away!
            hid_t temp_ = create_group(id_current_object, group_name.c_str());
            H5Oclose(temp_);

        }

        //Set subgroup name
        subgroupname = group_name + "/";


        // cout << "Adding material: " << group_name << endl;
        //Return the tag of this group
    }
    else  // Not in element definition mode
    {
        std::cerr << "HDF5_Channel::beginMaterialDescription() -> not in element definition mode"
                  << endl;
    }
    return id_current_object_group;
}


int HDF5_Channel::endElementDescription()
{
    // H5Oclose(id_current_object);
    inElementDefinitionMode = false;
    return 0;
}

int HDF5_Channel::endNodeDescription()
{
    // H5Oclose(id_current_object);
    inNodeDefinitionMode = false;
    return 0;
}

int HDF5_Channel::endMaterialDescription()
{
    subgroupname = "";
    return 0;
}





int HDF5_Channel::setTime(double t)
{
    current_time = t;

    //Extending past initial data allocation
    if (current_step_number >= number_of_time_steps)
    {
        number_of_time_steps = current_step_number + 1;
    }

    hsize_t dims[1]      = {number_of_time_steps};
    hsize_t data_dims[1] = {1};
    hsize_t offset[1]    = {current_step_number };
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

    // cout << "HDF5_Channel::setTime -> time set to t = " << current_time
    //      <<  "s @ step number "
    //      << current_step_number
    //      << ", number_of_time_steps = "
    //      << number_of_time_steps
    //      << endl;

    current_step_number++;
    return 0;
}






// ================================================================================
// Private functions
// ================================================================================



// For Managing HDF5


int HDF5_Channel::write_string(hid_t here, std::string name, std::string contents)
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

int HDF5_Channel::create_group(hid_t here, std::string name)
{

    // cout << "Creating group : " << name << endl;
    hid_t id_new_group;
    id_new_group = H5Gcreate(here, name.c_str(), H5P_DEFAULT, group_creation_plist , H5P_DEFAULT);
    return id_new_group;
}




std::string HDF5_Channel::generateRandomString()
{
    //Initialize string to all "a" with length set by HDF5_CHANNEL_RANDOMSTRNG_LEN
    std::string s(HDF5_CHANNEL_RANDOMSTRNG_LEN, 'a');

    //Randomly generate within these characters
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    //Populate the string
    for (int i = 0; i < HDF5_CHANNEL_RANDOMSTRNG_LEN; ++i)
    {
        int randomint = (int)((double)rand() / ((double)RAND_MAX + 1) * HDF5_CHANNEL_RANDOMSTRNG_LEN);
        s[i] = alphanum[randomint];
    }

    return s;
}



int HDF5_Channel::setUnit(hid_t object, std::string units)
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






hid_t HDF5_Channel::createVariableLengthDoubleArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string units)
{
    double fill_value = 0.0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    hsize_t chunk_dims[rank];
    for (int i = 0; i < rank; i++)
    {
        if (maxdims[i] == H5S_UNLIMITED)
        {
            chunk_dims[i] = HDF5_CHANNEL_CHUNK_TIMEDIM;
        }
        else
        {
            chunk_dims[i] = maxdims[i];
        }
    }


    status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk_cache(dataset_access_plist, HDF5_CHANNEL_CHUNK_NSLOTS, HDF5_CHANNEL_CHUNK_NBYTES, 0 );
    HDF5_CHECK_ERROR;

    status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_DOUBLE, &fill_value);
    HDF5_CHECK_ERROR;


    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                H5T_NATIVE_DOUBLE,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                dataset_access_plist);
    setUnit(id_array, units);
    H5Sclose(id_dataspace);

    return id_array;
}

hid_t HDF5_Channel::createConstantLengthDoubleArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string units)
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
    setUnit(id_array, units);
    H5Sclose(id_dataspace);

    return id_array;
}


hid_t HDF5_Channel::createVariableLengthIntegerArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string units)
{
    int fill_value = 0;

    //Setup the creation property list
    dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
    dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    // Set the layout to be chunked, the chunk size and the fill value
    // needs to be chunked because it is extensible
    hsize_t chunk_dims[rank];
    for (int i = 0; i < rank; i++)
    {
        if (maxdims[i] == H5S_UNLIMITED)
        {
            chunk_dims[i] = HDF5_CHANNEL_CHUNK_TIMEDIM;
        }
        else
        {
            chunk_dims[i] = maxdims[i];
        }
    }


    status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
    HDF5_CHECK_ERROR;
    status = H5Pset_chunk_cache(dataset_access_plist, HDF5_CHANNEL_CHUNK_NSLOTS, HDF5_CHANNEL_CHUNK_NBYTES, 0 );
    HDF5_CHECK_ERROR;
    status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_INT, &fill_value);
    HDF5_CHECK_ERROR;


    //Create the data description both for data in file and in memory
    hid_t id_dataspace;
    id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace

    // Create the dataset
    hid_t id_array = H5Dcreate2(here,
                                name.c_str(),
                                H5T_NATIVE_INT,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                dataset_access_plist);
    setUnit(id_array, units);
    H5Sclose(id_dataspace);

    return id_array;
}

hid_t HDF5_Channel::createConstantLengthIntegerArray(hid_t here,
        int rank,
        hsize_t *dims,
        hsize_t *maxdims,
        std::string name,
        std::string units)
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
    setUnit(id_array, units);
    H5Sclose(id_dataspace);

    return id_array;
}



hid_t HDF5_Channel::writeVariableLengthDoubleArray(hid_t id_array,
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
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);

    return id_array;
}

hid_t HDF5_Channel::writeConstantLengthDoubleArray(hid_t id_array,
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
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);

    return id_array;
}



hid_t HDF5_Channel::writeVariableLengthIntegerArray(hid_t id_array,
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
                 id_array,              // Id of the parent dataspace
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
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);

    return id_array;
}

hid_t HDF5_Channel::writeConstantLengthIntegerArray(hid_t id_array,
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
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);

    return id_array;
}




int HDF5_Channel::printStack()
{
    std::queue<string> temp_field_name_stack;
    std::queue<bool>   temp_field_is_time_dependent_stack;
    std::queue<string>   temp_field_units_stack;
    // cout << "Current stack: " << endl;
    // cout << "======================================================" << endl;
    while ( stack_length > 0)
    {
        std::string name;
        bool td;
        std::string units;

        // cout << "Stack_length =" << stack_length << endl;

        getNextField(name, td, units);
        temp_field_name_stack.push(name);
        temp_field_is_time_dependent_stack.push(td);
        temp_field_units_stack.push(units);

        // cout << name << ", " << td << ", " <<  units << endl;
    }
    field_name_stack = temp_field_units_stack;
    field_is_time_dependent_stack = temp_field_is_time_dependent_stack;
    field_units_stack = temp_field_units_stack;

    return 0;
}

void HDF5_Channel::garbage_collect()
{
    status = H5garbage_collect();
    HDF5_CHECK_ERROR;
}












// This is after: https://gist.github.com/jzrake/3025642
// -----------------------------------------------------------------------------
// The HDF5 specification only allows H5Lexists to be called on an immediate
// child of the current object. However, you may wish to see whether a whole
// relative path exists, returning false if any of the intermediate links are
// not present. This function does that.
// http://www.hdfgroup.org/HDF5/doc/RM/RM_H5L.html#Link-Exists
// -----------------------------------------------------------------------------
int H5Lexists_safe(hid_t base, std::string &path,  hid_t lapl_id )
{
    hid_t last = base, next;
    std::string pathc(path);
    boost::char_separator<char> sep("/");
    boost::tokenizer< boost::char_separator<char> > tokens(pathc, sep);

    BOOST_FOREACH(string pch, tokens)
    {
        int exists = H5Lexists(last, pch.c_str(), lapl_id);
        if (!exists)
        {
            if (last != base)
            {
                H5Oclose(last);
            }
            return 0;
        }
        else
        {
            next = H5Oopen(last, pch.c_str(), lapl_id);
            if (last != base)
            {
                H5Oclose(last);
            }
            last = next;
        }
    }
    if (last != base)
    {
        H5Oclose(last);
    }

    return 1;
}



char *HDF5_Channel::addToProgram(void)
{
    return 0;
}
int HDF5_Channel::setUpConnection(void)
{
    return 0;
}
int HDF5_Channel::setNextAddress(const ChannelAddress &theAddress)
{
    return 0;
}
ChannelAddress *HDF5_Channel::getLastSendersAddress(void)
{
    return 0;
}



