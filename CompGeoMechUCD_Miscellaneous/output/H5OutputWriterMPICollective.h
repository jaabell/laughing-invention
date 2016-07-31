///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//``This  source code is Copyrighted in U.S., by the The Regents of the University
//of California, for an indefinite period, and anybody caught using it without our
//permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              H5OutputWriterMPICollective.h
// CLASS:             H5OutputWriterMPICollective
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

#include <algorithm>  // For std::min and std::max functions

#ifndef H5OutputWriterMPICollective_H
#define H5OutputWriterMPICollective_H

// #define HDF5_CREATE_CHECK
// #define HDF5_CHECK_ERROR  {if(status < 0){cout << "status = " << status << endl; }}//return(-1);}}
inline void hdf5_check_error(herr_t status);
// {
// 	if (status < 0)
// 	{
// 		cout << "status = " << status << endl;
// 	}
// }

#define H5OutputWriterMPICollective_RANDOMSTRNG_LEN 16
#define H5OutputWriterMPICollective_CHECK_DEFINITION_MODE if (inElementDefinitionMode | inNodeDefinitionMode) \
		{ \
				std::cerr << "H5OutputWriterMPICollective::send##() -> still in definition mode. Element or node must end definition mode by calling endElementDescription() or endNodeDescription() before exiting! " << endl;  \
				return -1; \
		}
#define H5OutputWriterMPICollective_MAX_STRING_SIZE 1024

//Comment to untoggle
// #define _DEBUG_H5OutputWriterMPICollective 1;

#ifdef _DEBUG_H5OutputWriterMPICollective
#define H5OutputWriterMPICollective_COUNT_OBJS  nobjects = H5Fget_obj_count( id_file, H5F_OBJ_ALL ); \
		cout << "Objects open = "  << nobjects << endl;
#else
#define H5OutputWriterMPICollective_COUNT_OBJS
#endif

#ifdef _DEBUG_H5OutputWriterMPICollective
ssize_t nobjects;
#endif


#define H5OutputWriterMPICollective_CLEAN if(stack_length == 0) \
		{ H5Oclose(id_current_object); }

//These control the efficiency access to HDF5 file (tuning knobs)
#define H5OutputWriterMPICollective_CHUNK_NSLOTS 10007                        // Good if it is a prime number 101 1009  10007 (http://primes.utm.edu/lists/small/100000.txt)
#define H5OutputWriterMPICollective_CHUNK_NBYTES_OVER_SIZEOF_CHUNK 10        // 
#define H5OutputWriterMPICollective_CHUNK_TIMEDIM 10                        // Size of chunks in time dimension
#define H5OutputWriterMPICollective_CHUNK_MAXDIMENSION 10000
#define H5OutputWriterMPICollective_META_BLOCK_SIZE 2048*16                 // Not used
#define H5OutputWriterMPICollective_SIEVE_BUFFER_SIZE 1024*1024             // Not used because of chunking
#define H5OutputWriterMPICollective_PREEMPTION_POLICY 0.75
#define H5OutputWriterMPICollective_TIMESTRING_MAX_SIZE 64

#define H5OutputWriterMPICollective_MAX_RETURN_OPEN_OBJS 100

class H5OutputWriterMPICollective: public OutputWriter
{

public:  // To meet with OutputWriter interfacec
	H5OutputWriterMPICollective();
	H5OutputWriterMPICollective(std::string filename_in,
	                            std::string model_name_in,
	                            std::string stage_name_in,
	                            int nsteps);

	~H5OutputWriterMPICollective();

	virtual int setTime(double t);

	// Mesh output
	int writeGlobalMeshData(unsigned int number_of_nodes_in,
	                        unsigned int number_of_elements_in,
	                        unsigned int max_node_tag_in,
	                        unsigned int max_element_tag_in,
	                        unsigned int number_of_dofs_in,
	                        unsigned int number_of_outputs_in,
	                        unsigned int Total_Number_of_Gauss_Points,
                            unsigned int Total_Number_of_Connectivity_Nodes);
	// virtual int writeNumberOfNodes(unsigned int numberOfNodes_ ) ;
	// virtual int writeNumberOfElements(unsigned int numberOfElements_ ) ;

	virtual int writeNodeMeshData(int tag     , const Vector &coords   , int ndofs ) ;
	virtual int writeElementMeshData(int tag  , std::string type , const ID &connectivity         , int materialtag , const Matrix &gausscoordinates, int length_of_output, int class_tag) ;
	virtual int writeElementPartitionData(int tag  , int partition) ;
	virtual int writeMaterialMeshData(int tag , std::string type , Vector &parameters) ;
	virtual int writeLoadPatternData(int tag , std::string name) ;

	// Results for Nodes
	virtual int writeDisplacements(  int nodeTag, const Vector &displacements) ;
	virtual int writeDummyDisplacements() ;
	virtual int writeVelocities(     int nodeTag, const Vector &velocities) ;
	virtual int writeAccelerations(  int nodeTag, const Vector &accelerations) ;
	virtual int writeReactionForces( int nodeTag, const Vector &reactionForces) ;

	// Results for Elements
	virtual int writeElementOutput(int elementTag, const Vector &output) ;
	virtual int writeDummyElementOutput() ;  //Needed for collective HDF5 calls


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

	hid_t createVariableLengthDoubleArray(hid_t here,
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

	hid_t createVariableLengthStringArray(hid_t here,
	                                      std::string name,
	                                      std::string attribute);

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


	hid_t writeVariableLengthStringArray(hid_t id_array,
	                                     hsize_t datasize,
	                                     hsize_t offset,
	                                     std::string &data);


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

	void writeMesh(void);
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
	int number_of_gausspoints;
	int current_time_step;
	int number_of_time_steps;
	int max_node_tag;
	int max_element_tag;
	int number_of_dofs;
	int number_of_outputs;

	int length_nodes_displacements_output;
	int length_nodes_velocities_output;
	int length_nodes_accelerations_output;
	int length_nodes_reaction_forcess_output;
	int length_element_output;

	double current_time;

	int zlib_compression_level;
	int flag_write_element_output;

	std::string file_name;          // Name of the HDF5 file
	std::string model_name;          // Name of the HDF5 file
	std::string stage_name;          // Name of the HDF5 file
	std::string previous_stage_name;          // Name of the HDF5 file

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
	hid_t id_analysis_options;

	// For Nodes
	hid_t id_nodes_ndofs         , id_index_to_nodes_ndofs;
	hid_t id_nodes_coordinates   , id_index_to_nodes_coordinates;
	hid_t id_nodes_displacements , id_index_to_nodes_outputs;
	hid_t id_nodes_velocities    ;
	hid_t id_nodes_accelerations ;
	hid_t id_nodes_reaction_forces ;

	//For Elements
	hid_t id_elements_nnodes;
	hid_t id_elements_connectivity , id_index_to_elements_connectivity;
	hid_t id_elements_noutputs;
	hid_t id_elements_ngauss;
	hid_t id_elements_gausscoords  , id_index_to_elements_gausscoords;
	hid_t id_elements_output       , id_index_to_elements_output;
	hid_t id_elements_type;
	hid_t id_elements_materialtag;
	hid_t id_elements_classtag;
	hid_t id_elements_partition;



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
	Vector Coordinates;
	ID Index_to_Coordinates;
	ID Index_to_Generalized_Displacements;

	//For Elements
	ID Number_of_Nodes;
	ID Connectivity;
	ID Index_to_Connectivity;
	ID Number_of_Output_Fields;
	ID Index_to_Outputs;
	ID Number_of_Gauss_Points;
	Vector Gauss_Point_Coordinates;
	ID Index_to_Gauss_Point_Coordinates;
	std::vector<std::string> Element_types;
	std::vector<std::string> LoadPattern_names;
	ID Material_tags;
	ID Class_Tags;
	ID Partition;

	int numof_NO_COLLECTIVE_calls;
	int numof_CHUNK_INDEPENDENT_calls;
	int numof_CHUNK_COLLECTIVE_calls;
	int numof_CHUNK_MIXED_calls;


};

void call_hdf5_flush_next_chance(void);


#endif // #ifndef H5OutputWriterMPICollective_H