// ===============================================================================
// ESSI Simulator Program - UCD CompGeoMech
//
// COPYRIGHT (C):     ^_^
// PROJECT:           ESSI Simulator
// FILE:              NETCDF_Channel.h
// CLASS:             NETCDF_Channel
// VERSION:
// LANGUAGE:          C++ /
// TARGET OS:         UNIX || maybe something else
// DESIGNER:          Jose Abell, Babak Kamaranimogghadam, Boris Jeremic
// PROGRAMMER:        Jose Abell
// DATE:              Feb 2014
// UPDATE HISTORY:
//
// ===============================================================================



#ifndef NECDF_Channel_h
#define NECDF_Channel_h

#include <iostream>
#include <string>
#include <netcdf.h>

#define NETCDF_CHECK_ERROR  if(status != NC_NOERR){ cerr << endl << "ERROR! -> " << nc_strerror(status) << endl;  }


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

class NETCDF_Channel
{
    public:
        NETCDF_Channel (string filename_in, int nsteps = -1);

        ~NETCDF_Channel();

        // methods to set up the NETCDF_channel in an actor space
        char *addToProgram(void);
        int setUpConnection(void);
        int setNextAddress(const ChannelAddress &theAddress);
        ChannelAddress *getLastSendersAddress(void);

        int isDatastore(void);
        int getDbTag(void);
        int getTag(void);

        // methods to send/receive messages and objects on NETCDF_channels.
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





    protected:

    private:
        static int numChannel;
        int tag;

        const string dataset_filename;
        const int number_of_time_steps;   // If negative or zero, unlimited number of steps
        int number_of_nodes;
        int number_of_elements;
        // int number_of_materials;
        bool in_dataset_definition_mode;


        // Last NETCDF exit status
        int status;

        // NCIDs
        int ncid_root;            // Root of dataset
        int ncid_grp_elements;    // Elements
        int ncid_grp_nodes;       // Nodes
        int ncid_current;         // Current I/O takes place here
        int ncid_current_parent;  // One level up from current place ... if any
        // int ncid_grp_materials;

        //Basic dimensions
        int dim_time;

        //Basic variables
        int var_time;


        //These are small arrays useful to pass
        int dimentions_vector_1[1];     // To pass a single dimension (e.g in nc_def_var)
        int dimentions_vector_2[2];     // To pass the dimensions
        int dimentions_vector_3[3];     // To pass the dimensions

        //Indexes to pass on stuff when filling arrays
        size_t index1[1];     // For vectors  (e.g. nc_def_var_fill)
        size_t index2[2];     // For matriced
        size_t index3[3];     // For order 3 tensors
        size_t index4[4];     // For order 4 tensors

};

#endif
