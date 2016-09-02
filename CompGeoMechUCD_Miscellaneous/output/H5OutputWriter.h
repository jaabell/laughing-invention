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
#include <ctime>
#include <string>
#include <ID.h>
#include <Vector.h>
#include "OutputWriter.h"
#include <hdf5.h>
#include <hdf5_hl.h>
#include <classTags.h>

#include <algorithm>  // For std::min and std::max functions

#ifndef H5OutputWriter_H
#define H5OutputWriter_H

// #define HDF5_CREATE_CHECK
// #define HDF5_CHECK_ERROR  {if(status < 0){cout << "status = " << status << endl; }}//return(-1);}}
inline void hdf5_check_error(herr_t status);
// {
//  if (status < 0)
//  {
//      cout << "status = " << status << endl;
//  }
// }

#define H5OUTPUTWRITER_RANDOMSTRNG_LEN 16
#define H5OUTPUTWRITER_CHECK_DEFINITION_MODE if (inElementDefinitionMode | inNodeDefinitionMode) \
        { \
                std::cerr << "H5OutputWriter::send##() -> still in definition mode. Element or node must end definition mode by calling endElementDescription() or endNodeDescription() before exiting! " << endl;  \
                return -1; \
        }
#define H5OUTPUTWRITER_MAX_STRING_SIZE 1024

//Comment to untoggle
// #define _DEBUG_H5OUTPUTWRITER 1;

#ifdef _DEBUG_H5OUTPUTWRITER
#define H5OUTPUTWRITER_COUNT_OBJS  nobjects = H5Fget_obj_count( id_file, H5F_OBJ_ALL ); \
        cout << "Objects open = "  << nobjects << endl;
#else
#define H5OUTPUTWRITER_COUNT_OBJS
#endif

#ifdef _DEBUG_H5OUTPUTWRITER
ssize_t nobjects;
#endif


#define H5OUTPUTWRITER_CLEAN if(stack_length == 0) \
        { H5Oclose(id_current_object); }

//These control the efficiency access to HDF5 file (tuning knobs)
#define H5OUTPUTWRITER_CHUNK_NSLOTS 10007                        // Good if it is a prime number 101 1009  10007 (http://primes.utm.edu/lists/small/100000.txt)
#define H5OUTPUTWRITER_CHUNK_NBYTES_OVER_SIZEOF_CHUNK 10        // 
#define H5OUTPUTWRITER_CHUNK_TIMEDIM 10                        // Size of chunks in time dimension
#define H5OUTPUTWRITER_CHUNK_MAXDIMENSION 10000
#define H5OUTPUTWRITER_META_BLOCK_SIZE 2048*16                 // Not used
#define H5OUTPUTWRITER_SIEVE_BUFFER_SIZE 1024*1024             // Not used because of chunking
#define H5OUTPUTWRITER_PREEMPTION_POLICY 0.75
#define H5OUTPUTWRITER_TIMESTRING_MAX_SIZE 64

#define H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS 100

class H5OutputWriter: public OutputWriter
{

public:  // To meet with OutputWriter interfacec
    H5OutputWriter();
    H5OutputWriter(std::string filename_in,
                   std::string model_name_in,
                   std::string stage_name_in,
                   int nsteps);

    ~H5OutputWriter();

    virtual int setTime(float t);

    // Mesh output
    int writeGlobalMeshData(unsigned int number_of_nodes_in,
                            unsigned int number_of_elements_in,
                            unsigned int max_node_tag_in,
                            unsigned int max_element_tag_in,
                            unsigned int number_of_dofs_in,
                            unsigned int number_of_element_outputs_in,
                            unsigned int Total_Number_of_Gauss_Points,
                            unsigned int Total_Number_of_Connectivity_Nodes);

    // Added by sumeet on 30th ju;y, 2016 
    // Can be used to reserve space for datasets 
    int reserveSpaceForDatasets(unsigned int number_of_materials);

    virtual int writeNodeMeshData(int tag     , const Vector &coords   , int ndofs ) ;
    virtual int writeElementMeshData(int tag,const ID &connectivity, int  materialtag, const  Matrix &gausscoordinates, int class_tag);
    virtual int writeLoadPatternData(int tag , std::string name) ;
    virtual int writeSPConstraintsData(int nodetag , int dof) ;
    virtual int writeMaterialMeshData(int tag , std::string type ); //[Sumeet August,2016]

    // Results for Nodes
    virtual int writeDisplacements(  int nodeTag, const Vector &displacements) ;
    virtual int writeTrialDisplacements(  int nodeTag, const Vector &displacements) ;
    virtual int writeDummyDisplacements() ;
    virtual int writeUnbalancedForces( int nodeTag, const Vector &reactionForces) ;
    virtual int writeSupportReactions( int number_of_constrained_dofs, const std::vector<float> &reactionForces);

    // Results for Elements
    virtual int writeElementOutput(int elementTag, const vector<float> &outputint , int class_tag) ;
    virtual int writeGaussOutput(int elementTag, const vector<float> &outputint , int class_tag) ;
    virtual int writeTrialElementOutput(int elementTag, const vector<float> &output,  int class_tag);
    virtual int writeDummyElementOutput() ;  //Needed for collective HDF5 calls

    // Results for Eigen Value Analysis   // Sumeet 1st August, 2016
    virtual int writeEigenMesh (  int number_of_modes) ;
    virtual int writeEigenModes(  int nodeTag, const Matrix &eigenvectors) ;
    virtual int writeEigen_Value_Frequency_Period ( const Vector &eigenvalues, Vector periodvalues, Vector frequencyvalues) ;

    // Results of Substeps                // Sumeet 3rd August, 2016 
    virtual int writeSubstepMesh(int);       
    virtual int setSubStep(int substep_no);

    // Partition Info in master file output // [Sumeet August,2016]
    void writeNodePartitionData(int tag  , int partition) ;
    void writeElementPartitionData(int tag  , int partition) ;
    void initializePartitionData(int max_node_id, int max_element_id);

public:  //Additional stuff
    void initialize(std::string filename_in,
                    std::string model_name_in,
                    std::string stage_name_in,
                    int nsteps);
    void set_number_of_time_steps(int nsteps);
    int get_number_of_time_steps()
    {
        return number_of_time_steps;
    };

    void finalize();


    int write_string(hid_t here, std::string name, std::string contents);
    int create_group(hid_t here, std::string name);
    int setAttribute(hid_t object, std::string attribute);

    hid_t createVariableLengthArray(hid_t here,
                                    int rank,
                                    hsize_t *dims,
                                    hsize_t *maxdims,
                                    std::string name,
                                    std::string attibute,
                                    hid_t type,
                                    int sizeof_type,
                                    void *fill_value_ptr,
                                    int timedimension = -1);

    hid_t createVariableLengthFloatArray(hid_t here,
                                          int rank,
                                          hsize_t *dims,
                                          hsize_t *maxdims,
                                          std::string name,
                                          std::string attibute,
                                          int timedimension = -1);

    hid_t createVariableLengthIntegerArray(hid_t here,
                                           int rank,
                                           hsize_t *dims,
                                           hsize_t *maxdims,
                                           std::string name,
                                           std::string attibute,
                                           int timedimension = -1);

    hid_t createConstantLengthStringArray(hid_t here,
                                          int rank,
                                          hsize_t *dims,
                                          hsize_t *maxdims,
                                          std::string name,
                                          std::string attribute);

    hid_t createConstantLengthFloatArray(hid_t here,
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

    hid_t writeVariableLengthFloatArray(hid_t id_array,
                                         int datarank,
                                         hsize_t *dims,
                                         hsize_t *data_dims,
                                         hsize_t *offset,
                                         hsize_t *stride,
                                         hsize_t *count,
                                         hsize_t *block,
                                         float *data);

    hid_t writeConstantLengthFloatMatrix(hid_t id_array,
                                        int datarank,
                                        hsize_t *dims,
                                        hsize_t *data_dims,
                                        hsize_t *offset,
                                        hsize_t *stride,
                                        hsize_t *count,
                                        hsize_t *block,
                                        float *data);

    hid_t writeVariableLengthIntegerArray(hid_t id_array,
                                          int datarank,
                                          hsize_t *dims,
                                          hsize_t *data_dims,
                                          hsize_t *offset,
                                          hsize_t *stride,
                                          hsize_t *count,
                                          hsize_t *block,
                                          int *data);


    hid_t writeConstantLengthStringArray(hid_t id_array,
                                         hsize_t datasize,
                                         hsize_t offset,
                                         std::string &data);


    hid_t writeConstantLengthFloatArray(hid_t id_array,
                                         int datarank,
                                         hsize_t *dims,
                                         hsize_t *data_dims,
                                         hsize_t *offset,
                                         hsize_t *stride,
                                         hsize_t *count,
                                         hsize_t *block,
                                         float *data);

hid_t writeConstantLengthFloatArray(hid_t id_array,
        int datarank,
        hsize_t *dims,
        hsize_t *data_dims,
        hsize_t *offset,
        hsize_t *stride,
        hsize_t *count,
        hsize_t *block,
        const float *data);


    hid_t writeConstantLengthIntegerArray(hid_t id_array,
                                          int datarank,
                                          hsize_t *dims,
                                          hsize_t *data_dims,
                                          hsize_t *offset,
                                          hsize_t *stride,
                                          hsize_t *count,
                                          hsize_t *block,
                                          int *data);

    void writeMesh(void);
    void DeleteMeshArrays(); // [Sumeet August, 2016] 
    void syncWriters(); //Used in parallel


    void set_zlib_compression_level(int level);
    int get_zlib_compression_level()
    {
        return zlib_compression_level;
    }
    void set_flag_write_element_output(bool is_element_output_is_enabled_)
    {
        flag_write_element_output = is_element_output_is_enabled_;
        // cout << "flag_write_element_output = " << flag_write_element_output << endl;
    }


    static bool call_hdf5_flush;


private:

    bool file_is_open;

    int number_of_nodes;
    int number_of_elements;
    int number_of_time_steps;
    int max_node_tag;
    int max_element_tag;
    int number_of_dofs;
    int number_of_element_outputs;
    int number_of_gausspoints;
    int number_of_connectivity_nodes;   // Added by sumeet 
    int current_time_step;              

    int current_sub_step;               // Added by sumeet 3rd August, 2016

    int number_of_eigen_modes;          // added by sumeet 1st August, 2016

    float current_time;

    int zlib_compression_level;
    int flag_write_element_output;

    std::string file_name;          // Name of the HDF5 file
    std::string model_name;          // Name of the HDF5 file
    std::string stage_name;          // Name of the HDF5 file
    std::string previous_stage_name; // Name of the HDF5 file

    //herr_t, hid_t, and hsize_t are types defined in the HDF5 library
    herr_t status;                  // For error reporting. Each time a HDF5 function that returns herr_t is called, the macro HDF5_CHECK_ERROR should get called.
    hid_t id_file;                  // object id of the file


    // HDF5 "hid_t" handles to objects within file
    hid_t id_model_group;           // object id of the base group for models
    hid_t id_elements_group;        // object id of the base group for models
    hid_t id_nodes_group;           // object id of the base group for models
    hid_t id_time_vector;           // object id of the time vector
    hid_t id_number_of_time_steps;
    hid_t id_number_of_elements;
    hid_t id_number_of_nodes;
    hid_t id_number_of_gauss_points;
    hid_t id_analysis_options;

    // For Nodes
    hid_t id_nodes_ndofs         , id_index_to_nodes_ndofs;
    hid_t id_nodes_coordinates   , id_index_to_nodes_coordinates;
    hid_t id_nodes_displacements , id_index_to_nodes_outputs;
    hid_t id_nodes_partition     ;
    hid_t id_unbalanced_forces ;
    hid_t id_support_reactions;
    hid_t id_Constrained_Nodes;
    hid_t id_Constrained_DOFs;

    //For Elements
    hid_t id_elements_connectivity , id_index_to_elements_connectivity;
    hid_t id_elements_gausscoords  , id_index_to_elements_gausscoords;
    hid_t id_elements_output       , id_index_to_elements_output;
    hid_t id_gauss_output;
    hid_t id_elements_class_desc;
    hid_t id_elements_materialtag;
    hid_t id_elements_classtag;
    hid_t id_elements_partition;

    //For Eigen Value Analysis [sumeet 1st august, 2016]
    hid_t id_eigen_analysis_group; 
    hid_t id_number_of_modes;
    hid_t id_eigen_modes;
    hid_t id_eigen_values;
    hid_t id_eigen_frequencies;
    hid_t id_eigen_periods;

    // added by sumeet 3rd August, 2016 for substeps output
    hid_t id_trial_nodes_displacements; 
    hid_t id_trial_elements_output;
    hid_t id_trial_gauss_output;

    //Some property lists
    hid_t group_creation_plist;
    hid_t dataset_creation_plist;
    hid_t dataset_access_plist;
    hid_t dataset_xfer_plist;

    //Some flags
    bool create_nodeMeshData_arrays;
    bool create_nodeDisplacements_arrays;
    bool create_nodeVelocities_arrays;
    bool create_nodeAccelerations_arrays;
    bool create_nodeReactionForces_arrays;
    bool create_elementMeshData_arrays;
    bool create_elementOutput_arrays;

    //Positions within some arrays
    int pos_nodes_outputs;
    int pos_nodes_coordinates;
    int pos_elements_outputs;
    int pos_elements_gausscoords;
    int pos_elements_connectivity;
    int pos_loadpatterns_names;

    //Temporary mesh arrays

    //For nodes
    ID Number_of_DOFs;
    std::vector<float> Coordinates;
    ID Index_to_Coordinates;
    ID Index_to_Generalized_Displacements;

    //For Elements
    ID Connectivity;
    ID Index_to_Connectivity;
    ID Index_to_Element_Outputs;
    std::vector<float> Gauss_Point_Coordinates;
    ID Index_to_Gauss_Point_Coordinates;
    std::vector<std::string> LoadPattern_names;
    ID Material_tags;
    ID Class_Tags;
    ID Element_Partition;
    ID Node_Partition;
    ID SPNodes;
    ID SPDofs;
    std::vector<int> Element_Class_Desc;

    //For Materials
    hid_t id_material;
    std::vector<std::string> Materials;

    int numof_NO_COLLECTIVE_calls;
    int numof_CHUNK_INDEPENDENT_calls;
    int numof_CHUNK_COLLECTIVE_calls;
    int numof_CHUNK_MIXED_calls;

};

void call_hdf5_flush_next_chance(void);


#endif // #ifndef H5OutputWriter_H