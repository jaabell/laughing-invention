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



#ifndef HDF5_Channel_h
#define HDF5_Channel_h

#include <Channel.h>
#include <iostream>
#include <string>
#include <queue>

#include <hdf5.h>
#include <hdf5_hl.h>


#define HDF5_CHECK_ERROR  {if(status < 0){cout << "status = " << status << endl; }}//return(-1);}}
#define HDF5_CHANNEL_RANDOMSTRNG_LEN 16
#define HDF5_CHANNEL_CHECK_DEFINITION_MODE if (inElementDefinitionMode | inNodeDefinitionMode) \
    { \
        std::cerr << "HDF5_Channel::sendID -> still in definition mode. Element or node must end definition mode by calling endElementDescription() or endNodeDescription() before exiting! " << endl;  \
        return -1; \
    }
#ifdef _DEBUG_HDF5_CHANNEL
#define HDF5_CHANNEL_COUNT_OBJS ssize_t nobjects = H5Fget_obj_count( id_file, H5F_OBJ_ALL ); \
    cout << "Objects open = "  << nobjects << endl;
#else
#define HDF5_CHANNEL_COUNT_OBJS
#endif
#define HDF5_CHANNEL_CLEAN if(stack_length == 0) \
    { H5Oclose(id_current_object); }

//These optimize access to HDF5 file (tuninng knobs)
#define HDF5_CHANNEL_CHUNK_NSLOTS 10007                        // Good if it is a prime number
#define HDF5_CHANNEL_CHUNK_NBYTES_OVER_SIZEOF_CHUNK 100       // 
#define HDF5_CHANNEL_CHUNK_TIMEDIM 100                        // Size of chunks in time dimension
#define HDF5_CHANNEL_META_BLOCK_SIZE 2048*16                  // Not used
#define HDF5_CHANNEL_SIEVE_BUFFER_SIZE 1024*1024              // Not used because of chunking



// Some definitions to make writing functions more readable
#define ESSI_OUTPUT_LEVEL_BASIC 0
#define ESSI_OUTPUT_LEVEL_DETAILED 1
#define ESSI_OUTPUT_LEVEL_DEBUG 2
#define ESSI_OUTPUT_LEVEL_COMPLETE 10
#define ESSI_OUTPUT_TIME_DEPENDENT true
#define ESSI_OUTPUT_TIME_INDEPENDENT false

using namespace std;

class ChannelAddress;
class Message;
class MovableObject;
class Matrix;
class Vector;
class ID;
class FEM_ObjectBroker;
class nDarray;
// class LTensor;

class HDF5_Channel: public Channel
{
    public:

        ////This constructor is used to open a previous analysis for restoring state
        HDF5_Channel ();
        HDF5_Channel (std::string filename_in,
                      int step_to_restore);
        HDF5_Channel (std::string filename_in,
                      double time_to_restore);

        //This constructor is used to create a new file
        HDF5_Channel (std::string filename_in,
                      std::string model_name_in,
                      std::string stage_name_in,
                      int nsteps);

        ~HDF5_Channel();

        void initialize(std::string filename_in,
                        std::string model_name_in,
                        std::string stage_name_in,
                        int nsteps);

        // ========================================================================
        // Member functions inherited from Channel
        // ========================================================================

        // methods to set up the HDF5_channel in an actor space
        char *addToProgram(void);
        int setUpConnection(void);
        int setNextAddress(const ChannelAddress &theAddress);
        ChannelAddress *getLastSendersAddress(void);


        int isDatastore(void);
        int getDbTag(void);
        int getTag(void);

        // methods to send/receive messages and objects on HDF5_channels.
        int sendObj(int outputLevel,
                    MovableObject &theObject,
                    ChannelAddress *theAddress = 0);

        int receiveObj(int outputLevel,
                    MovableObject &theObject,
                    FEM_ObjectBroker &theBroker,
                    ChannelAddress *theAddress = 0);

        int sendMsg(int not_used, int outputLevel,
                    const Message &theMessage,
                    ChannelAddress *theAddress = 0);

        int receiveMsg(int not_used, int outputLevel,
                    Message &theMessage,
                    ChannelAddress *theAddress = 0);

        int sendMatrix(int not_used, int outputLevel,
                       const Matrix &theMatrix,
                       ChannelAddress *theAddress = 0);

        int receiveMatrix(int not_used, int outputLevel,
                       Matrix &theMatrix,
                       ChannelAddress *theAddress = 0);

        int sendVector(int not_used, int outputLevel,
                       const Vector &theVector,
                       ChannelAddress *theAddress = 0);

        int receiveVector(int not_used, int outputLevel,
                       Vector &theVector,
                       ChannelAddress *theAddress = 0);

        int sendID(int not_used, int outputLevel,
                   const ID &theID,
                   ChannelAddress *theAddress = 0);

        int receiveID(int not_used, int outputLevel,
                   ID &theID,
                   ChannelAddress *theAddress = 0);

        int sendnDarray(int not_used, int outputLevel,
                        const nDarray &theNDarray,
                        ChannelAddress *theAddress = 0);

        int receivenDarray(int not_used, int outputLevel,
                        nDarray &theNDarray,
                        ChannelAddress *theAddress = 0);


        // ========================================================================
        // Member functions specific to HDF5_Channel
        // ========================================================================

        int setTime(double t);

        int beginElementDescription(std::string name, int tag);

        int beginNodeDescription(int tag);

        int beginMaterialDescription(std::string name, int tag);

        int endElementDescription();

        int endNodeDescription();

        int endMaterialDescription();

        int addField(std::string field_name,
                     bool is_time_dependent,
                     int output_level);

        int printStack();

        void done();

        void setMaxOutputLevel(int new_level);

        void useIndex();

        void wipeIndexAndCreateNewOne();



    private:  // Functions

        int getNextField(std::string &field_name,
                         bool &is_time_dependent,
                         int &output_level);

        int write_string(hid_t here, std::string name, std::string contents);
        int create_group(hid_t here, std::string name);
        std::string generateRandomString();
        int setUnit(hid_t object, std::string units);

        hid_t createVariableLengthDoubleArray(hid_t here,
                                              int rank,
                                              hsize_t *dims,
                                              hsize_t *maxdims,
                                              std::string name,
                                              std::string units);
        hid_t createConstantLengthDoubleArray(hid_t here,
                                              int rank,
                                              hsize_t *dims,
                                              hsize_t *maxdims,
                                              std::string name,
                                              std::string units);
        hid_t createVariableLengthIntegerArray(hid_t here,
                                               int rank,
                                               hsize_t *dims,
                                               hsize_t *maxdims,
                                               std::string name,
                                               std::string units);

        hid_t createConstantLengthIntegerArray(hid_t here,
                                               int rank,
                                               hsize_t *dims,
                                               hsize_t *maxdims,
                                               std::string name,
                                               std::string units);

        hid_t createVariableLengthArray(hid_t here,
                                        int rank,
                                        hsize_t *dims,
                                        hsize_t *maxdims,
                                        std::string name,
                                        std::string units,
                                        hid_t type, int sizeof_type);




        hid_t writeVariableLengthDoubleArray(hid_t id_array,
                                             int datarank,
                                             hsize_t *dims,
                                             hsize_t *data_dims,
                                             hsize_t *offset,
                                             hsize_t *stride,
                                             hsize_t *count,
                                             hsize_t *block,
                                             double *data);

        hid_t writeConstantLengthDoubleArray(hid_t id_array,
                                             int datarank,
                                             hsize_t *dims,
                                             hsize_t *data_dims,
                                             hsize_t *offset,
                                             hsize_t *stride,
                                             hsize_t *count,
                                             hsize_t *block,
                                             double *data);

        hid_t writeVariableLengthIntegerArray(hid_t id_array,
                                              int datarank,
                                              hsize_t *dims,
                                              hsize_t *data_dims,
                                              hsize_t *offset,
                                              hsize_t *stride,
                                              hsize_t *count,
                                              hsize_t *block,
                                              int *data);

        hid_t writeConstantLengthIntegerArray(hid_t id_array,
                                              int datarank,
                                              hsize_t *dims,
                                              hsize_t *data_dims,
                                              hsize_t *offset,
                                              hsize_t *stride,
                                              hsize_t *count,
                                              hsize_t *block,
                                              int *data);

    protected:

    private:
        static int numChannel;
        int tag;

        bool file_is_open;

        unsigned int current_step_number;
        double current_time;
        unsigned int number_of_materials_in_current_element;
        unsigned int number_of_time_steps;

        std::string filename;          // Name of the HDF5 file
        std::string model_name;          // Name of the HDF5 file
        std::string stage_name;          // Name of the HDF5 file

        //herr_t, hid_t, and hsize_t are types defined in the HDF5 library
        herr_t status;                  // For error reporting. Each time a HDF5 function that returns herr_t is called, the macro HDF5_CHECK_ERROR should get called.
        hid_t id_file;                  // object id of the file
        hid_t id_model_group;           // object id of the base group for models
        hid_t id_elements_group;        // object id of the base group for models
        hid_t id_nodes_group;           // object id of the base group for models
        hid_t id_time_vector;           // object id of the time vector

        hid_t id_current_object;
        hid_t id_current_object_material;
        hid_t id_current_object_group;

        //Some property lists
        hid_t group_creation_plist;
        hid_t dataset_creation_plist;
        hid_t dataset_access_plist;

        //hid_t id_simulation_settings_group;// object id of the base group for simulation options

        bool inNodeDefinitionMode;
        bool inElementDefinitionMode;

        //Stacks
        std::queue<string> field_name_stack;               // Contains names of fields for current object
        std::queue<bool>   field_is_time_dependent_stack;  // Contains info on whether each field in "name_stack" is time dependent
        std::queue<int>    field_output_level_stack;              // Contains the physical units for each field field in "name_stack"
        unsigned int stack_length;

        //if inside materials, we need this
        std::string subgroupname;

        int maxOutputLevel;


        //index
        bool useIndexDataset;
        bool createIndex;
        std::vector<hid_t> index;
};


int H5Lexists_safe(hid_t base, std::string &path, hid_t lapl_id);



#endif
