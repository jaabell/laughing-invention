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



#include <Channel.h>
#include <NETCDF_Channel.h>
#include <Message.h>
#include <MovableObject.h>
#include <FEM_ObjectBroker.h>
#include <netcdf.h>

int NETCDF_Channel::numChannel = 0;





NETCDF_Channel::NETCDF_Channel (string filename_in, int nsteps):
    dataset_filename(filename_in),
    number_of_time_steps(nsteps),
{
    numChannel++;
    tag = numChannel;
    in_dataset_definition_mode = true;

    //Create the dataset
    status = nc_create(dataset_filename.c_str(), NC_NETCDF4, &ncid_root);
    NETCDF_CHECK_ERROR;

    //Define the time variable at the root
    status = nc_def_dim(ncid_root, "dim_time", NC_UNLIMITED, &dim_time);
    NETCDF_CHECK_ERROR;

    //Make a group called Nodes
    status = nc_def_grp(ncid_root, "Nodes", &ncid_grp_nodes);
    NETCDF_CHECK_ERROR;

    //Make a group called Elements
    status = nc_def_grp(ncid_root, "Elements", &ncid_grp_elements);
    NETCDF_CHECK_ERROR;

    // //Make a group called Materials
    // status = nc_def_grp(ncid_root, "Materials", &ncid_grp_materials);
    // NETCDF_CHECK_ERROR;

}

NETCDF_Channel::~NETCDF_Channel()
{
    status = nc_close(ncid_root);
    NETCDF_CHECK_ERROR;
}

int
NETCDF_Channel::isDatastore(void)
{
    return 0;
}

int
NETCDF_Channel::getDbTag(void)
{
    return 0;
}

int
NETCDF_Channel::getTag(void)
{
    return tag;
}



// methods to send/receive messages and objects on NETCDF_channels.
int NETCDF_Channel::sendObj(int commitTag,
                            MovableObject &theObject,
                            ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvObj(int commitTag,
                            MovableObject &theObject,
                            FEM_ObjectBroker &theBroker,
                            ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::sendMsg(int dbTag, int commitTag,
                            const Message &theMessage,
                            ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvMsg(int dbTag, int commitTag,
                            Message &theMessage,
                            ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::sendMatrix(int dbTag, int commitTag,
                               const Matrix &theMatrix,
                               ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvMatrix(int dbTag, int commitTag,
                               Matrix &theMatrix,
                               ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::sendVector(int dbTag, int commitTag,
                               const Vector &theVector,
                               ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvVector(int dbTag, int commitTag,
                               Vector &theVector,
                               ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::sendID(int dbTag, int commitTag,
                           const ID &theID,
                           ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvID(int dbTag, int commitTag,
                           ID &theID,
                           ChannelAddress *theAddress )
{
    return 0;
}

//Guanzhou added
int NETCDF_Channel::sendnDarray(int dbTag, int commitTag,
                                const nDarray &theNDarray,
                                ChannelAddress *theAddress )
{
    return 0;
}

int NETCDF_Channel::recvnDarray(int dbTag, int commitTag,
                                nDarray &theNDarray,
                                ChannelAddress *theAddress )
{
    return 0;
}

