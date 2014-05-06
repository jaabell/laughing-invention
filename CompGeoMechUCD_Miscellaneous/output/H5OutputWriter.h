///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              H5OutputWriter.h
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
#include "OutputWriter.h"
#include <hdf5.h>
#include <hdf5_hl.h>

#ifndef H5OutputWriter_H
#define H5OutputWriter_H


#define HDF5_CHECK_ERROR  {if(status < 0){cout << "status = " << status << endl; }}//return(-1);}}
#define H5OUTPUTWRITER_RANDOMSTRNG_LEN 16
#define H5OUTPUTWRITER_CHECK_DEFINITION_MODE if (inElementDefinitionMode | inNodeDefinitionMode) \
    { \
        std::cerr << "H5OutputWriter::send##() -> still in definition mode. Element or node must end definition mode by calling endElementDescription() or endNodeDescription() before exiting! " << endl;  \
        return -1; \
    }

//Comment to untoggle
// #define _DEBUG_H5OUTPUTWRITER 1
#ifdef _DEBUG_H5OUTPUTWRITER
#define H5OUTPUTWRITER_COUNT_OBJS ssize_t nobjects = H5Fget_obj_count( id_file, H5F_OBJ_ALL ); \
    cout << "Objects open = "  << nobjects << endl;
#else
#define H5OUTPUTWRITER_COUNT_OBJS
#endif


#define H5OUTPUTWRITER_CLEAN if(stack_length == 0) \
    { H5Oclose(id_current_object); }

//These optimize access to HDF5 file (tuninng knobs)
#define H5OUTPUTWRITER_CHUNK_NSLOTS 1009                        // Good if it is a prime number 101 1009  10007 (http://primes.utm.edu/lists/small/100000.txt)
#define H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK 10        // 
#define H5OUTPUTWRITER_CHUNK_TIMEDIM 100                        // Size of chunks in time dimension
#define H5OUTPUTWRITER_META_BLOCK_SIZE 2048*16                 // Not used
#define H5OUTPUTWRITER_SIEVE_BUFFER_SIZE 1024*1024             // Not used because of chunking
#define H5OUTPUTWRITER_PREEMPTION_POLICY 0.75




class H5OutputWriter: public OutputWriter
{

    public:  // To meet with OutputWriter interfacec
        H5OutputWriter();
        H5OutputWriter(std::string filename_in,
                       std::string model_name_in,
                       std::string stage_name_in,
                       int nsteps);
        ~H5OutputWriter();

        virtual int setTime(double t);

        // Mesh output
        virtual int writeNumberOfNodes(unsigned int numberOfNodes_ ) ;
        virtual int writeNumberOfElements(unsigned int numberOfElements_ ) ;

        virtual int writeNodeMeshData(int tag     , const Vector &coords   , int ndofs ) ;
        virtual int writeElementMeshData(int tag  , std::string type , ID &connectivity         , int materialtag , Vector &parameters, int length_of_output) ;
        virtual int writeMaterialMeshData(int tag , std::string type , Vector &parameters) ;

        // Results for Nodes
        virtual int writeDisplacements(  int nodeTag, const Vector &displacements) ;
        virtual int writeVelocities(     int nodeTag, const Vector &velocities) ;
        virtual int writeAccelerations(  int nodeTag, const Vector &accelerations) ;
        virtual int writeReactionForces( int nodeTag, const Vector &reactionForces) ;

        // Results for Elements
        virtual int writeElementOutput(int elementTag, Vector &output) ;


    public:  //Additional stuff
        void initialize(std::string filename_in,
                        std::string model_name_in,
                        std::string stage_name_in,
                        int nsteps);


        int write_string(hid_t here, std::string name, std::string contents);
        int create_group(hid_t here, std::string name);
        int setAttribute(hid_t object, std::string attribute);

        hid_t createVariableLengthArray(hid_t here,
                                        int rank,
                                        hsize_t *dims,
                                        hsize_t *maxdims,
                                        std::string name,
                                        std::string attibute,
                                        hid_t type, int sizeof_type, void *fill_value_ptr);

        hid_t createVariableLengthDoubleArray(hid_t here,
                                              int rank,
                                              hsize_t *dims,
                                              hsize_t *maxdims,
                                              std::string name,
                                              std::string attibute);

        hid_t createVariableLengthIntegerArray(hid_t here,
                                               int rank,
                                               hsize_t *dims,
                                               hsize_t *maxdims,
                                               std::string name,
                                               std::string attibute);

        hid_t createConstantLengthDoubleArray(hid_t here,
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



        hid_t writeVariableLengthDoubleArray(hid_t id_array,
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


        hid_t writeConstantLengthDoubleArray(hid_t id_array,
                                             int datarank,
                                             hsize_t *dims,
                                             hsize_t *data_dims,
                                             hsize_t *offset,
                                             hsize_t *stride,
                                             hsize_t *count,
                                             hsize_t *block,
                                             double *data);


        hid_t writeConstantLengthIntegerArray(hid_t id_array,
                                              int datarank,
                                              hsize_t *dims,
                                              hsize_t *data_dims,
                                              hsize_t *offset,
                                              hsize_t *stride,
                                              hsize_t *count,
                                              hsize_t *block,
                                              int *data);






    private:

        bool file_is_open;

        int number_of_nodes;
        int number_of_elements;
        int current_time_step;
        int number_of_time_steps;
        int max_node_tag;
        int max_element_tag;

        int length_nodes_displacements_output;
        int length_nodes_velocities_output;
        int length_nodes_accelerations_output;
        int length_nodes_reaction_forcess_output;
        int length_element_output;

        double current_time;

        std::string file_name;          // Name of the HDF5 file
        std::string model_name;          // Name of the HDF5 file
        std::string stage_name;          // Name of the HDF5 file

        //herr_t, hid_t, and hsize_t are types defined in the HDF5 library
        herr_t status;                  // For error reporting. Each time a HDF5 function that returns herr_t is called, the macro HDF5_CHECK_ERROR should get called.
        hid_t id_file;                  // object id of the file


        // HDF5 "hid_t" handles to objects within file


        hid_t id_model_group;           // object id of the base group for models
        hid_t id_elements_group;        // object id of the base group for models
        hid_t id_nodes_group;           // object id of the base group for models
        hid_t id_time_vector;           // object id of the time vector

        // For Nodes
        hid_t id_nodes_ndofs         , id_index_to_nodes_ndofs;
        hid_t id_nodes_coordinates   , id_index_to_nodes_coordinates;
        hid_t id_nodes_displacements , id_index_to_nodes_outputs;
        hid_t id_nodes_velocities    ;
        hid_t id_nodes_accelerations ;
        hid_t id_nodes_reaction_forces ;

        hid_t id_current_object;
        hid_t id_current_object_material;
        hid_t id_current_object_group;

        //Some property lists
        hid_t group_creation_plist;
        hid_t dataset_creation_plist;
        hid_t dataset_access_plist;

        bool create_nodeMeshData_arrays;
        bool create_nodeDisplacements_arrays;
        bool create_nodeVelocities_arrays;
        bool create_nodeAccelerations_arrays;
        bool create_nodeReactionForces_arrays;
        bool create_elementMeshData_arrays;

        int pos_nodes_outputs;
        int pos_nodes_coordinates;

};



#endif // #ifndef H5OutputWriter_H