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

#include <iostream>
#include <string>
#include <queue>

#include <hdf5.h>
#include <hdf5_hl.h>


#define HDF5_CHECK_ERROR  {if(status < 0){cout << "status = " << status << endl; }}//return(-1);}}
#define HDF5_CHANNEL_RANDOMSTRNG_LEN 16



using namespace std;

class ChannelAddress;
class Message;
class MovableObject;
class Matrix;
class Vector;
class ID;
class FEM_ObjectBroker;
class nDarray;
class LTensor;

class HDF5_Channel
{
    public:

        ////This constructor is used to open a previous analysis for restoring state
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

        // methods to set up the HDF5_channel in an actor space
        char *addToProgram(void);
        int setUpConnection(void);
        int setNextAddress(const ChannelAddress &theAddress);
        ChannelAddress *getLastSendersAddress(void);

        int isDatastore(void);
        int getDbTag(void);
        int getTag(void);

        // methods to send/receive messages and objects on HDF5_channels.
        int sendObj(int commitTag,
                    MovableObject &theObject,
                    ChannelAddress *theAddress = 0);

        int recvObj(int commitTag,
                    MovableObject &theObject,
                    FEM_ObjectBroker &theBroker,
                    ChannelAddress *theAddress = 0);

        int sendMsg(int dbTag, int commitTag,
                    const Message &theMessage,
                    ChannelAddress *theAddress = 0);

        int recvMsg(int dbTag, int commitTag,
                    Message &theMessage,
                    ChannelAddress *theAddress = 0);

        int sendMatrix(int dbTag, int commitTag,
                       const Matrix &theMatrix,
                       ChannelAddress *theAddress = 0);

        int recvMatrix(int dbTag, int commitTag,
                       Matrix &theMatrix,
                       ChannelAddress *theAddress = 0);

        int sendVector(int dbTag, int commitTag,
                       const Vector &theVector,
                       ChannelAddress *theAddress = 0);

        int recvVector(int dbTag, int commitTag,
                       Vector &theVector,
                       ChannelAddress *theAddress = 0);

        int sendID(int dbTag, int commitTag,
                   const ID &theID,
                   ChannelAddress *theAddress = 0);

        int recvID(int dbTag, int commitTag,
                   ID &theID,
                   ChannelAddress *theAddress = 0);

        int sendnDarray(int dbTag, int commitTag,
                        const nDarray &theNDarray,
                        ChannelAddress *theAddress = 0);

        int recvnDarray(int dbTag, int commitTag,
                        nDarray &theNDarray,
                        ChannelAddress *theAddress = 0);

        int addNewField(std::string field_name,
                        bool is_time_dependent,
                        std::string units);

        int getNextField(std::string &field_name,
                         bool &is_time_dependent,
                         std::string &units);

        int addNewElement(std::string name, int tag);

        int addNewNode(int tag);

        int addNewMaterial(std::string name, int tag);

    private:  // Functions

        int write_string(hid_t here, std::string name, std::string contents);
        int create_group(hid_t here, std::string name);
        std::string generateRandomString();



    protected:

    private:
        static int numChannel;
        int tag;

        int current_step_number;
        double current_time;

        const int number_of_time_steps;
        const std::string filename;          // Name of the HDF5 file
        const std::string model_name;          // Name of the HDF5 file
        const std::string stage_name;          // Name of the HDF5 file

        herr_t status;                  // For error reporting. Each time a HDF5 function that returns herr_t is called, the macro HDF5_CHECK_ERROR should get called.
        hid_t id_file;                  // object id of the file
        hid_t id_model_group;           // object id of the base group for models
        hid_t id_elements_group;        // object id of the base group for models
        hid_t id_nodes_group;           // object id of the base group for models

        hid_t id_current_object;
        hid_t id_current_object_group;
        hid_t id_current_group;
        hid_t id_previous_group;

        //Some property lists
        hid_t group_creation_plist;

        //hid_t id_simulation_settings_group;// object id of the base group for simulation options

        std::queue<string> field_name_stack;               // Contains names of fields for current object
        std::queue<bool>   field_is_time_dependent_stack;  // Contains info on whether each field in "name_stack" is time dependent
        std::queue<string>   field_units_stack;              // Contains the physical units for each field field in "name_stack"

        unsigned int stack_length;
};

#endif
