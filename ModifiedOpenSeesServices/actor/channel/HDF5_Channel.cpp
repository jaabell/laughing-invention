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
#include <Vector.h>

int HDF5_Channel::numChannel = 0;




// This constructor is used to create a new output file
HDF5_Channel::HDF5_Channel (std::string filename_in,
                            std::string model_name_in,
                            std::string stage_name_in,
                            int nsteps):
    number_of_time_steps(nsteps),
    filename(filename_in),
    model_name(model_name_in),
    stage_name(stage_name_in)
{
    numChannel++;
    tag = numChannel;

    current_time = 0.0;
    current_step_number = 0;

    std::cout << "Opening file" << endl;

    //================================================================================
    //Create the file, overwriting it if it exists
    //================================================================================
    id_file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);



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
}

HDF5_Channel::~HDF5_Channel()
{
    H5Fclose(id_file);
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
    // This function first checks if the field has to be created in the HDF5 file.
    // (1) if the field has to be created
    //      (i) If the field is time varying, an (extendible) 2D array will be created with time on the
    //      second index.
    //      (ii) If it is not time varying only a 1D array is created.
    // (2) if the field is already created
    //      (i) if it is time varying, write out the new data
    //      (ii) if it is not time-varying, then do nothing the data has already been written out.
    //
    // The decision to create the field is based on the stack length. If there are items in the
    // field stacks, then describeSelf has been called and we are trying to add these new fields
    // Otherwise we are just outputting the data to existing fields.

    // Get pointer to the data from the vector
    //--------------------------------------------------------------------------------------

    int fill_value = 0;   // Default value of empty array

    int length_of_vector_data;
    double *data = theVector.theData;
    length_of_vector_data =  theVector.sz;
    hid_t id_dataset;    //Points to the object in the file that is the data

    if (stack_length > 0)  // This is a new field to be added, need to create it
    {
        //Get the field info
        //--------------------------------------------------------------------------------------
        std::string field_name;
        bool is_time_dependent;
        std::string units;
        getNextField(field_name, is_time_dependent, units);

        //Create the field
        //--------------------------------------------------------------------------------------
        int rank;            // Rank of array to be created (2 if time varying and 1 if not)
        hid_t id_dataspace;  // Identifier of the bject that describes data in file
        hid_t id_memspace;   // Identifier of the Object that describes data in memory
        hsize_t dims[2], maxdims[2];
        hsize_t data_dims[1];
        data_dims[0] = length_of_vector_data;
        hsize_t data_maxdims[1];
        data_maxdims[0] = length_of_vector_data;

        if (is_time_dependent) // If it is time dependent
        {
            rank = 2;
            //Starting array dimensions
            dims[0] = length_of_vector_data;
            dims[1] = number_of_time_steps;
            //Max array dimensions
            maxdims[0] = length_of_vector_data;
            maxdims[1] = H5S_UNLIMITED;

            //Setup the creation property list
            dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
            dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

            // Set the layout to be chunked, the chunk size and the fill value
            // needs to be chunked because it is extensible
            hsize_t chunk_dims[2];
            chunk_dims[0] = length_of_vector_data;
            chunk_dims[1] = 1;
            status = H5Pset_layout(dataset_creation_plist, H5D_CHUNKED);
            HDF5_CHECK_ERROR;
            status = H5Pset_chunk(dataset_creation_plist, rank, chunk_dims);
            HDF5_CHECK_ERROR;
            status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_DOUBLE, &fill_value);
            HDF5_CHECK_ERROR;
        }
        else  // If it is not time dependent
        {
            rank = 1;
            dims[0] = length_of_vector_data;        // Starting array dims
            maxdims[0] = length_of_vector_data;     // Max array dims

            //Setup the creation property list (default)
            dataset_creation_plist = H5Pcreate(H5P_DATASET_CREATE);
            dataset_access_plist = H5Pcreate(H5P_DATASET_ACCESS);

            // Set the fill value
            status = H5Pset_fill_value(dataset_creation_plist, H5T_NATIVE_DOUBLE, &fill_value);
            HDF5_CHECK_ERROR;
        }

        //Create the data description both for data in file and in memory
        id_dataspace = H5Screate_simple(rank, dims     , maxdims     );       // create dataspace
        id_memspace  = H5Screate_simple(1   , data_dims, data_maxdims);       // create dataspace

        // Create the dataset
        cout << "!! Creating dataset" << endl;
        id_dataset = H5Dcreate2(id_current_object_group,
                                field_name.c_str(),
                                H5T_NATIVE_DOUBLE,
                                id_dataspace,
                                H5P_DEFAULT,
                                dataset_creation_plist,
                                H5P_DEFAULT);

        //Write out the data to the dataset




    }
    else // This field exists and will be written to, or it does not exist and will not be written to
    {

    }

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





// ================================================================================
// Private functions
// ================================================================================



// For Managing HDF5


int HDF5_Channel::write_string(hid_t here, std::string name, std::string contents)
{
    std::cout << "Writing string" << endl;
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
    HDF5_CHECK_ERROR;

    return 0;
}

int HDF5_Channel::create_group(hid_t here, std::string name)
{
    hid_t id_new_group;
    id_new_group = H5Gcreate(here, name.c_str(), H5P_DEFAULT, group_creation_plist , H5P_DEFAULT);
    return id_new_group;
}





//Adding new fields

int HDF5_Channel::addNewField(std::string field_name,
                              bool is_time_dependent,
                              std::string units)
{
    field_name_stack.push(field_name);
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




int HDF5_Channel::addNewElement(std::string name, int tag)
{
    //Set the current group identifier to the elements group
    id_previous_group = id_current_group;
    id_current_group = id_elements_group;

    //Convert integer tag to string
    string stag;
    stag = boost::lexical_cast<std::string>(tag);

    //Concatenate name and tag to create group name
    string group_name;
    group_name = name + "_" + stag;

    //Create a group to hold the data for the element
    id_current_object_group = create_group(id_current_group, group_name.c_str());

    //Return the tag of this group
    return id_current_object_group;
}

int HDF5_Channel::addNewNode(int tag)
{
    //Set the current group identifier to the nodes group
    id_previous_group = id_current_group;
    id_current_group = id_nodes_group;

    //Convert integer tag to string
    string stag;
    stag = boost::lexical_cast<std::string>(tag);

    //Create a group to hold the data for the node
    id_current_object_group = create_group(id_current_group, stag.c_str());

    //Return the tag of this group
    return id_current_object_group;
}

int HDF5_Channel::addNewMaterial(std::string name, int tag)
{

    //Set the current group identifier to the nodes group
    id_previous_group = id_current_group;
    //id_current_group = id_nodes_group;

    //Convert integer tag to string
    string stag;
    stag = boost::lexical_cast<std::string>(tag);

    //Concatenate name and tag and a random string to create group name
    string group_name;
    group_name = name + "_" + stag + generateRandomString();

    //Create a group to hold the data for the node
    id_current_object_group = create_group(id_current_group, stag.c_str());

    //Return the tag of this group
    return id_current_object_group;
}



//Misc


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



