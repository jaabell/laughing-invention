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

    std::cout << "Opening file" << endl;
    //Create the file, overwriting it if it exists
    id_file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    //Setup the property lists
    group_creation_plist = H5Pcreate(H5P_GROUP_CREATE);
    status = H5Pset_link_creation_order(group_creation_plist, H5P_CRT_ORDER_TRACKED | H5P_CRT_ORDER_INDEXED);
    HDF5_CHECK_ERROR;

    //Create the model name, stage name and number of steps fields
    write_string(id_file, "Model_Name", model_name);
    write_string(id_file, "Stage_Name", stage_name);


    //Create the group to store the model
    id_model_group = create_group(id_file, "Model");

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
    if (stack_length > 0)  // This is a new field to be added, need to create it
    {
        std::string field_name;
        bool is_time_dependent;
        std::string units;

        getNextField(field_name, is_time_dependent, units);

        int nleft;
        double *data = theVector.theData;
        nleft =  theVector.sz;


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