///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Jose Abell
// DATE:               Nov 2014
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////



// Purpose: This file contains the class definition for Domain_Reduction_Method_HDF5_input.
// Domain_Reduction_Method_HDF5_input is an subclass of loadPattern.

#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <hdf5.h>
#include <Domain_Reduction_Method_HDF5_input.h>

#include <Channel.h>
#include <OPS_Globals.h>
#include <map>

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif


#include <limits>   //For std::numeric_limits<double>::epsilon()



#define numNodeDOF 3  // Only nodes with 3-dofs per node canbe used in DRM... :/

using namespace std;

Domain_Reduction_Method_HDF5_input::Domain_Reduction_Method_HDF5_input()
    : LoadPattern(0, PATTERN_TAG_Domain_Reduction_Method_HDF5_Input),
      Elements(0),
      Nodes(0),
      IsBoundary(0),
      times(0),
      DRMForces(0),
      maxnodetag(0)
{
    is_initialized = false;
    t =  t1 =  t2 =  tfinal = 0;
    cFactor = 0;
    step = step1 = step2 = 0;
}

Domain_Reduction_Method_HDF5_input::Domain_Reduction_Method_HDF5_input
(
    int tag,
    std::string HDF5filename_
)
    : LoadPattern(tag, PATTERN_TAG_Domain_Reduction_Method_HDF5_Input),
      HDF5filename(HDF5filename_),
      Elements(0),
      Nodes(0),
      IsBoundary(0),
      times(0),
      DRMForces(0),
      maxnodetag(0)
{
    is_initialized = false;
    t =  t1 =  t2 =  tfinal = 0;
    cFactor = 0;
    step = step1 = step2 = 0;
}

void Domain_Reduction_Method_HDF5_input::intitialize()
{
    Domain *theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }

    //===========================================================================
    // Open the specified file for read only
    //===========================================================================

    hid_t file_access_plist   = H5Pcreate(H5P_FILE_ACCESS);

#ifdef _PARALLEL_PROCESSING
    // H5Pset_fapl_mpio(file_access_plist, MPI_COMM_WORLD, MPI_INFO_NULL);
#endif


    unsigned flags = H5F_ACC_RDONLY;
    id_drm_file = H5Fopen(HDF5filename.c_str(), flags, file_access_plist);

    if (id_drm_file < 0)
    {
        DRMerror << "Unable to open file: " << HDF5filename << endl;
        return;
    }


    //===========================================================================
    // Read "Elements" array, get number of DRM elements and figure out which
    // ones correspond to this subdomain.
    //===========================================================================

    hsize_t data_dims[1];
    hsize_t data_maxdims[1];
    hsize_t rank_one_array = 1;
    hsize_t id_memspace = 0;

    hsize_t id_elements = H5Dopen(id_drm_file, "Elements", H5P_DEFAULT);

    if (id_elements < 0)
    {
        DRMerror << "Could not open \"Elements\" array!\n ";
        return;
    }

    //Open dataspace and get number of elements from it
    hsize_t id_dataspace = H5Dget_space(id_elements);
    int ndims =  H5Sget_simple_extent_ndims( id_dataspace );

    int* all_DRM_elements_list = 0;
    if (ndims == 0) // Handle the single-element case
    {
        number_of_DRM_elements = 1;
        all_DRM_elements_list = new int[number_of_DRM_elements];
        data_dims[0] = 1;
        data_maxdims[0] = 1;
        id_memspace  = H5Screate_simple(rank_one_array, data_dims, data_maxdims);       // create dataspace of memory
        H5Dread(id_elements, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, all_DRM_elements_list);
    }
    else if (ndims == 1)
    {

        H5Sget_simple_extent_dims(id_dataspace, data_dims, data_maxdims );
        number_of_DRM_elements = data_dims[0];

        //Temporary array to hold DRM element ids
        all_DRM_elements_list  = new int[number_of_DRM_elements];

        //Now read the array and report number of elements found
        id_memspace  = H5Screate_simple(rank_one_array, data_dims, data_maxdims);       // create dataspace of memory
        H5Dread(id_elements, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, all_DRM_elements_list);
    }
    else
    {
        DRMerror << " \"Elements\" array should be scalar or at most 1-D\n";
    }

    DRMout << "Dataset \"" << HDF5filename << "\" has " << number_of_DRM_elements << " DRM elements on layer.\n";
    // Find out whether the elements have been defined in current model. In parallel, subdomain might not
    // contain all the defined elements, so keep only those that are local.
    number_of_local_elements = 0;
    for (int i = 0; i < number_of_DRM_elements; i++)
    {
        if (theDomain->getElement(all_DRM_elements_list[i]) != 0)
        {
            number_of_local_elements++;
        }
        else
        {
            all_DRM_elements_list[i] = -1;
        }
    }
    DRMout << "Found " << number_of_local_elements << " elements\n";


    if (number_of_local_elements == 0)
    {
        is_initialized = true;
        return; // No use doing anything else.
    }


    // Now create the elements array (one copy per subdomain in case of parallel) and
    // assign elements which belong here.
    Elements = new ID(number_of_local_elements);
    int e = 0;
    for (int i = 0; i < number_of_DRM_elements; i++)
    {
        if (all_DRM_elements_list[i] >= 0)
        {
            (*Elements)[e] = all_DRM_elements_list[i];
            e++;
        }
    }
    // TODO: Do a MPI_reduce to find out whether all elements specified in file are present in
    // the whole model.

    //Cleanup
    delete all_DRM_elements_list;
    H5Dclose(id_elements);
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);




    //===========================================================================
    // Read "Nodes"and "IsBoundary" arrays
    //===========================================================================

    hsize_t id_nodes = H5Dopen(id_drm_file, "DRM Nodes", H5P_DEFAULT);
    hsize_t id_is_boundary = H5Dopen(id_drm_file, "Is Boundary Node", H5P_DEFAULT);

    if (id_nodes < 0)
    {
        DRMerror << "Could not open \"DRM Nodes\" array!\n ";
        return;
    }

    if (id_is_boundary < 0)
    {
        DRMerror << "Could not open \"Is Boundary Node\" array!\n ";
        return;
    }

    //Open dataspace and get number of elements from it
    id_dataspace = H5Dget_space(id_nodes);
    ndims =  H5Sget_simple_extent_ndims( id_dataspace );

    if (ndims != 1)
    {
        DRMerror << "\"DRM Nodes\" should be a 1-D array.\n";
        return;
    }

    H5Sget_simple_extent_dims(id_dataspace, data_dims, data_maxdims );
    number_of_DRM_nodes = data_dims[0];

    //Temporary array to hold DRM element ids
    int* all_DRM_nodes_list = new int[number_of_DRM_nodes];
    int* all_DRM_is_boundary_list = new int[number_of_DRM_nodes];

    //Now read the array and report number of elements found
    id_memspace  = H5Screate_simple(rank_one_array, data_dims, data_maxdims);       // create dataspace of memory
    H5Dread(id_nodes, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, all_DRM_nodes_list);
    H5Dread(id_is_boundary, H5T_NATIVE_INT, id_memspace, id_dataspace, H5P_DEFAULT, all_DRM_is_boundary_list);
    DRMout << "Dataset \"" << HDF5filename  << "\" has " <<  number_of_DRM_nodes << " DRM nodes on layer.\n";


    //Figure out if the nodes are defined in current domain
    int number_of_local_nodes = 0;
    for (int i = 0; i < number_of_DRM_nodes; i++)
    {
        if (theDomain->getNode(all_DRM_nodes_list[i]) != 0)
        {
            number_of_local_nodes++;
        }
        else
        {
            all_DRM_nodes_list[i] = -1;
        }
    }


    Nodes = new ID(number_of_local_nodes);
    IsBoundary = new ID(number_of_local_nodes);
    int n = 0;
    for (int i = 0; i < number_of_DRM_nodes; i++)
    {
        if (all_DRM_nodes_list[i] >= 0)
        {
            (*Nodes)[n] = all_DRM_nodes_list[i];
            (*IsBoundary)[n] = all_DRM_is_boundary_list[i];
            nodetag2hdf5_pos.insert(std::pair<int, int>((*Nodes)[i], i));
            n++;

            if (all_DRM_nodes_list[i] > maxnodetag)
            {
                maxnodetag = all_DRM_nodes_list[i];
            }
        }
    }

    DRMout << "Found " << number_of_local_nodes << " nodes\n";

    DRMForces = new Matrix(3 * number_of_local_nodes, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS);

    // TODO: Do a MPI_reduce to find out whether all elements specified in file are present in
    // the whole model.

    //Create an index which relates node tag to location in arrays (for faster access)
    for (int i = 0; i < number_of_local_nodes; i++)
    {
        nodetag2index.insert(std::pair<int, int>((*Nodes)[i], i));
    }



    delete all_DRM_nodes_list;
    delete all_DRM_is_boundary_list;
    H5Dclose(id_nodes);
    H5Dclose(id_is_boundary);
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);


    //===========================================================================
    // Read "Time" arrays
    //===========================================================================

    hsize_t id_time = H5Dopen(id_drm_file, "Time", H5P_DEFAULT);

    if (id_time < 0)
    {
        DRMerror << "Could not open \"Time\" array!\n ";
        return;
    }

    id_dataspace = H5Dget_space(id_time);
    ndims =  H5Sget_simple_extent_ndims( id_dataspace );

    if (ndims != 1)
    {
        DRMerror << "\"Time\" array should be a 1-D array.\n";
        return;
    }

    H5Sget_simple_extent_dims(id_dataspace, data_dims, data_maxdims );
    number_of_timesteps = data_dims[0];

    double* all_timesteps = new double[number_of_timesteps];

    id_memspace  = H5Screate_simple(rank_one_array, data_dims, data_maxdims);       // create dataspace of memory

    // cout << "Reading HDF5 dataset \n";
    H5Dread(id_time, H5T_NATIVE_DOUBLE, id_memspace, id_dataspace, H5P_DEFAULT, all_timesteps);


    DRMout << "Dataset \"" << HDF5filename  << "\" has " <<  number_of_timesteps << " time-steps.\n";

    times = new Vector(all_timesteps, number_of_timesteps);

    tfinal = (*times)[number_of_timesteps - 1];

    // delete all_timesteps;
    H5Dclose(id_time);
    H5Sclose(id_dataspace);
    H5Sclose(id_memspace);


    //===========================================================================
    // Open Displacements and Accelerations arrays for reading
    //===========================================================================

    id_displacements = H5Dopen(id_drm_file, "Displacements", H5P_DEFAULT);

    if (id_displacements < 0)
    {
        DRMerror << "Could not open \"Displacements\" array!\n ";
        return;
    }

    id_displacements_dataspace = H5Dget_space(id_displacements);

    id_accelerations = H5Dopen(id_drm_file, "Accelerations", H5P_DEFAULT);

    if (id_accelerations < 0)
    {
        DRMerror << "Could not open \"Accelerations\" array!\n ";
        return;
    }
    id_accelerations_dataspace = H5Dget_space(id_accelerations);

    hsize_t rank_two_array = 2;
    hsize_t one_node_data_dims[2] = {3, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS};
    hsize_t one_node_data_maxdims[2] = {3, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS};
    id_one_node_memspace  = H5Screate_simple(rank_two_array, one_node_data_dims, one_node_data_maxdims);       // create dataspace of memory


    id_xfer_plist = H5Pcreate( H5P_DATASET_XFER);

#ifdef _PARALLEL_PROCESSING
    int numProcesses_world;
    int processID_world;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID_world);

    numProcesses = numProcesses_world - 1;

    //Fork a new group of processes with its own communicator
    // so that SOE solution can be done un a subset of all MPI processes.

    //Ranks 1 through numProcesses are assigned to worker_world... since rank 0 has no elements!
    worker_ranks = new int[numProcesses_world - 1];
    if (worker_ranks == NULL)
    {
        cerr << "DRMHDF5::setSize(int MaxDOFtag) - could not allocate new ranks vector of size " << numProcesses <<  " \n";
    }

    for (int rank = 1; rank < numProcesses_world ; rank++)
    {
        worker_ranks[rank - 1] = rank;
    }


    //Get the MPI world
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    MPI_Group_incl(world_group, numProcesses_world - 1, worker_ranks, &worker_group);


    /* Create new communicator and then perform collective communications */

    MPI_Comm_create(MPI_COMM_WORLD, worker_group, &worker_comm);
    int all_elements = 0;
    MPI_Reduce(&number_of_local_elements, &all_elements, 1, MPI_INTEGER, MPI_SUM, 0, worker_comm);

    if (all_elements != number_of_DRM_elements)
    {
        cerr << "DRMHDF5::setSize(int MaxDOFtag) - could not find all elements in domain.  " << numProcesses <<  " \n";
        exit(-1);
    }

    int all_nodes = 0;
    MPI_Reduce(&number_of_local_nodes, &all_nodes, 1, MPI_INTEGER, MPI_SUM, 0, worker_comm);

    if (all_nodes != number_of_DRM_nodes)
    {
        cerr << "DRMHDF5::setSize(int MaxDOFtag) - could not find all nodes in domain.  " << numProcesses <<  " \n";
        exit(-1);
    }
    // MPI_reduce()
#endif

    //===========================================================================
    // Set status to initialized and ready to compute loads
    //===========================================================================


    is_initialized = true;
}



Domain_Reduction_Method_HDF5_input::~Domain_Reduction_Method_HDF5_input()
{
    clean_all_data();
}

void Domain_Reduction_Method_HDF5_input::clean_all_data()
{

    // DRMout << "cleanup!\n";

    if ( Elements != 0)
    {
        delete Elements;
    }

    if ( Nodes != 0)
    {
        delete Nodes;
    }

    if ( IsBoundary != 0)
    {
        delete IsBoundary;
    }

    if ( times != 0)
    {
        delete times;
    }

    if ( DRMForces != 0)
    {
        delete DRMForces;
    }

    nodetag2index.clear();
    nodetag2hdf5_pos.clear();

    if (H5Dclose(id_displacements) < 0)
    {
        DRMerror << "Unable to close displacements dataset. " << endl;
    }
    if (H5Dclose(id_accelerations) < 0)
    {
        DRMerror << "Unable to close accelerations dataset. " << endl;
    }
    if (H5Sclose(id_displacements_dataspace) < 0)
    {
        DRMerror << "Unable to close displacements dataspace. " << endl;
    }
    if (H5Sclose(id_accelerations_dataspace) < 0)
    {
        DRMerror << "Unable to close accelerations dataspace. " << endl;
    }
    if (H5Sclose(id_one_node_memspace) < 0)
    {
        DRMerror << "Unable to close one node memspace. " << endl;
    }

    hid_t obj_id_list[H5OUTPUTWRITER_MAX_RETURN_OPEN_OBJS];
    hsize_t n_obj_open = 10;
    while (n_obj_open > 0)
    {
        int n_objects_closed = 0;
        n_obj_open = H5Fget_obj_count(id_drm_file, H5F_OBJ_DATASET | H5F_OBJ_GROUP | H5F_OBJ_ATTR | H5F_OBJ_LOCAL );
        cout << "Domain_Reduction_Method_HDF5_input -- N of HDF5 objects open = " << n_obj_open << endl;

        if (n_obj_open <= 0)
        {
            break;
        }

        int number_of_open_objects;

        //Close datasets
        number_of_open_objects = H5Fget_obj_ids( id_drm_file, H5F_OBJ_DATASET | H5F_OBJ_LOCAL, DRMHDF5_MAX_RETURN_OPEN_OBJS, obj_id_list );
        if (number_of_open_objects > 0)
        {
            cout << "                                   - Closing " <<  number_of_open_objects << " datasets.\n";
        }
        for (int i = 0; i < std::min(number_of_open_objects, DRMHDF5_MAX_RETURN_OPEN_OBJS) ; i++ )
        {
            H5Dclose(obj_id_list[i]);
            n_objects_closed++;
        }

        //Close groups
        number_of_open_objects = H5Fget_obj_ids( id_drm_file, H5F_OBJ_GROUP | H5F_OBJ_LOCAL, DRMHDF5_MAX_RETURN_OPEN_OBJS, obj_id_list );
        if (number_of_open_objects > 0)
        {
            cout << "                                   - Closing " <<  number_of_open_objects << " groups.\n";
        }
        for (int i = 0; i < std::min(number_of_open_objects, DRMHDF5_MAX_RETURN_OPEN_OBJS) ; i++ )
        {
            H5Gclose(obj_id_list[i]);
            n_objects_closed++;
        }

        //Close groups
        number_of_open_objects = H5Fget_obj_ids( id_drm_file, H5F_OBJ_ATTR | H5F_OBJ_LOCAL, DRMHDF5_MAX_RETURN_OPEN_OBJS, obj_id_list );
        if (number_of_open_objects > 0)
        {
            cout << "                                   - Closing " <<  number_of_open_objects << " attributes.\n";
        }
        for (int i = 0; i < std::min(number_of_open_objects, DRMHDF5_MAX_RETURN_OPEN_OBJS) ; i++ )
        {
            H5Aclose(obj_id_list[i]);
            n_objects_closed++;
        }

        if (n_objects_closed == 0)
        {
            //This guards against the possibility of H5Fget_obj_count misbehaving.
            // ot that the open objects are not datasets, groups or attributes.
            break;
        }

    }



    if (H5Fclose(id_drm_file) < 0)
    {
        DRMerror << "Unable to close HDF5 file \"" << HDF5filename << "\"\n";
    }

    is_initialized = false;
}

void
Domain_Reduction_Method_HDF5_input::setDomain(Domain *theDomain)
{
    this->LoadPattern::setDomain(theDomain);
}


void
Domain_Reduction_Method_HDF5_input::applyLoad(double time)
{
    // DRMout << "time = " << time <<  "\n";
    if (not is_initialized)
    {
        intitialize();
    }

    //If we don't have elements, no point in doing anything :/
    if (number_of_local_elements > 0)
    {
        Domain *theDomain = this->getDomain();

        if (theDomain == 0)
        {
            return;
        }

        if (time > tfinal)
        {
            return;
        }

        if (Elements->Size() == 0)
        {
            return;
        }

        t = time;
        if ( (t < t1) or (t2 < t) )
        {
            this->ComputeDRMLoads();
        }


        Node *theNode;
        Vector *load = new Vector(3);

        int use_this_step = 0;
        // cout << "Searching for timestep to use: ";
        double macheps = std::numeric_limits<double>::epsilon() * (t2 - t1);
        for (int i = step1; i < step2; i++)
        {
            // cout << "                               " << i << " (" << (*times)[i] - time << ") ";
            if ((*times)[i] >= time or abs((*times)[i] - time) <= 10 * macheps)
            {
                // cout << "!\n";
                use_this_step -= 1;
                break;
            }
            // cout << ".\n";
            use_this_step++;
        }
        if (use_this_step < 0)
        {
            use_this_step = 0;
        }
        // cout << endl;
        // cout << " > DRM (t = " << t << ") Using offset # " << use_this_step << " step = " << step1 + use_this_step << ", time = " << (*times)[use_this_step + step1];

        double time1 =  (*times)[use_this_step + step1];
        double time2 =  (*times)[use_this_step + step1 + 1];
        double tau = (time - time1) / (time2 - time1);

        if ( abs(time1 - time) <= 10 * macheps)
        {
            tau = 0;
        }
        if ( abs(time2 - time) <= 10 * macheps)
        {
            tau = 1;
        }

        // cout << ", tau = " << tau  << endl;

        // ofstream fout("drmforces.txt", ios::app);
        // fout << time << " ";

        for (int i = 0; i < Nodes->Size(); i++)
        {

            int tag = (*Nodes)[i] ;
            theNode = theDomain->getNode( tag );

            if ( theNode == 0 )
            {
                continue;
            }

            // load = this->getNodalLoad((*Nodes)[i], time);

            int pos = nodetag2index[tag];
            for (int i = 0; i < 3; i++)
            {
                // (*load)(i) = (*DRMForces)(3 * pos + i, use_this_step);
                (*load)(i) = (*DRMForces)(3 * pos + i, use_this_step) * (1 - tau);
                (*load)(i) += (*DRMForces)(3 * pos + i, use_this_step + 1) * tau;
                // fout << (*load)(i) << " ";

                if (std::isnan((*load)(i)))
                {
                    cerr << "DRM HDF5:   NAN encountered!! \n";
                    cerr << "    (t = " << t << ") Using offset # " << use_this_step << " step = " << step1 + use_this_step << ", time = " << (*times)[use_this_step + step1];
                    cerr << "\n  load(i) = " << (*load)(i) << ", i = " << i << endl;
                }
            }

            //Take care of the minus sign in the effective seismic force for boundary nodes
            if ((*IsBoundary)[i])
            {
                (*load) = (*load) * (-1.0);
            }

            theNode->addUnbalancedLoad(*load);
        }
        // fout << endl;
        delete load;
    }
}

int
Domain_Reduction_Method_HDF5_input::sendSelf(int commitTag, Channel & theChannel)
{

    DRMout << "sending filename: " << HDF5filename << endl;

    if (theChannel.sendString(0, 0, HDF5filename) < 0)
    {
        cerr << "Domain_Reduction_Method_HDF5_input::sendSelf -- failed to send HDF5filename\n";
        return -1;
    }

    return 0;
}

int
Domain_Reduction_Method_HDF5_input::receiveSelf(int commitTag, Channel & theChannel,
        FEM_ObjectBroker & theBroker)
{
    DRMout << "receiving...\n";

    if (theChannel.receiveString(0, 0, HDF5filename) < 0)
    {
        cerr << "Domain_Reduction_Method_HDF5_input::receiveSelf -- failed to receive HDF5filename\n";
        return -1;
    }

    DRMout << "got filename: " << HDF5filename << endl;

    return 0;
}

void
Domain_Reduction_Method_HDF5_input::Print(ostream & s, int flag)
{

}



// method to obtain a blank copy of the LoadPattern
LoadPattern *
Domain_Reduction_Method_HDF5_input::getCopy(void)
{
    return new Domain_Reduction_Method_HDF5_input(this->getTag(), HDF5filename);
}

void
Domain_Reduction_Method_HDF5_input::ComputeDRMLoads()
{
//If we don't have elements, no point in doing anything :/
    if (number_of_local_elements > 0)
    {
        // clock_t init, final;

        // init = clock();

        Domain *theDomain = this->getDomain();

        Element *theElement = theDomain->getElement( (*Elements)[0] );
        int NIE = theElement->getNumExternalNodes();


        t1 = t2 = 0;
        step1 = step2 = 0;
        for (int i = 0; i < number_of_timesteps; i++)
        {
            if ((*times)[i] < t)
            {
                step1 = i;
            }
            else
            {
                break;
            }
        }
        t1 = (*times)[step1];
        step2 = step1 + DRM_NUM_OF_PRECOMPUTED_TIMESTEPS;

        int Nt = DRM_NUM_OF_PRECOMPUTED_TIMESTEPS;
        if (step2 > number_of_timesteps)
        {
            step2 = number_of_timesteps;
            Nt = step2 - step1;
        }
        t2 = (*times)[step2 - 1];



        // DRMout << "step1 = " << step1 << ", step2 = " << step2 << endl;
        // DRMout << "t1 = " << t1 << " <= t = " << t << " <= t2 = "  << t2 << endl;

        int NDOF = 3;

        // for (int i = 0; i < Nodes->Size(); i++)
        // {
        //     int tag = (*Nodes)[i];
        //     cout << i << " : " << tag << " -> " << nodetag2index[tag];
        //     if ((*IsBoundary)[i])
        //     {
        //         cout << " b" << endl;
        //     }
        //     else
        //     {
        //         cout << " e" << endl;
        //     }
        // }


        Matrix *Fm = new Matrix(NIE * NDOF, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS);
        Matrix *Fk  = new Matrix(NIE * NDOF, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS);
        Matrix *u_e = new Matrix(NIE * NDOF, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS);
        Matrix *udd_e = new Matrix(NIE * NDOF, DRM_NUM_OF_PRECOMPUTED_TIMESTEPS);

        double d[NIE * 3][DRM_NUM_OF_PRECOMPUTED_TIMESTEPS];
        double a[NIE * 3][DRM_NUM_OF_PRECOMPUTED_TIMESTEPS];

        DRMForces->Zero();

        for (int e = 0; e < Elements->Size(); e++)
        {
            int eleTag = (*Elements)[e];

            // cout << "eleTag = " << eleTag << " (" << e << " of " << Elements->Size()  << ") " << endl;

            //Pointer to current element
            theElement = theDomain->getElement(eleTag);

            //List of current element nodes
            const ID &elementNodes = theElement->getExternalNodes();

            //Identify boundary and exterior nodes for this element
            ID B_node(NIE);
            ID E_node(NIE);
            int nB = 0, nE = 0;
            // bool bdnode;

            for ( int ii = 0; ii < NIE; ii++)
            {
                int nodeTag = elementNodes(ii);

                if ( (*IsBoundary)[nodetag2index[nodeTag]] == 1 )
                {
                    B_node(nB) = ii;
                    nB ++;
                }
                else
                {
                    E_node(nE) = ii;
                    nE ++;
                }
            }

            if ( nB != 0 and nE != 0 )
            {
                // DRMout << "Element # " << eleTag << ", nB = " << nB << ", nE = " << nE << endl;
                //Mass and stiffness matrices
                Matrix Me = theElement->getMass();
                Matrix Ke = theElement->getTangentStiff();


                //Zero out the diagonal block of Boundary nodes
                for (int m = 0; m < nB; m++)
                    for (int n = 0; n < nB; n++)
                        for (int d = 0; d < NDOF; d++)
                            for (int e = 0; e < NDOF; e++)
                            {
                                Me( B_node(m)*NDOF + d, B_node(n)*NDOF + e ) = 0.0;
                                Ke( B_node(m)*NDOF + d, B_node(n)*NDOF + e ) = 0.0;
                            }


                //Zero out the diagonal block of Exterior nodes
                for (int m = 0; m < nE; m++)
                    for (int n = 0; n < nE; n++)
                        for (int d = 0; d < NDOF; d++)
                            for (int e = 0; e < NDOF; e++)
                            {
                                Me( E_node(m)*NDOF + d, E_node(n)*NDOF + e ) = 0.0;
                                Ke( E_node(m)*NDOF + d, E_node(n)*NDOF + e ) = 0.0;
                            }

                // cout <<
                for (int n = 0; n < NIE; n++)
                {
                    // cout << ".";
                    int nodeTag = elementNodes(n);
                    // int pos = nodetag2index[nodeTag];
                    int pos = nodetag2hdf5_pos[nodeTag];

                    hsize_t start[2]  = {(hsize_t) 3 * pos , (hsize_t)step1};
                    hsize_t stride[2] = {1       , 1};
                    hsize_t count[2]  = {1       , 1};
                    hsize_t block[2]  = {3       , (hsize_t) Nt};

                    H5Sselect_hyperslab(
                        id_displacements_dataspace,
                        H5S_SELECT_SET, start, stride, count, block );

                    H5Sselect_hyperslab(
                        id_accelerations_dataspace,
                        H5S_SELECT_SET, start, stride, count, block );

                    start[0] = start[1] = 0;

                    H5Sselect_hyperslab(
                        id_one_node_memspace,
                        H5S_SELECT_SET, start, stride, count, block );


                    H5Dread( id_displacements, H5T_NATIVE_DOUBLE, id_one_node_memspace,
                             id_displacements_dataspace, id_xfer_plist,  d );

                    H5Dread( id_accelerations, H5T_NATIVE_DOUBLE, id_one_node_memspace,
                             id_accelerations_dataspace, id_xfer_plist,  a );

                    //This can be optimized by using the arrays directly and also BLAS-2 kernels for matmul
                    for (int j = 0 ; j < Nt; j++)
                        for (int i = 0 ; i < 3; i++)
                        {
                            (*u_e)(3 * n + i, j) = d[i][j];
                            (*udd_e)(3 * n + i, j) = a[i][j];
                        }
                }
                // cout << endl;

                Fm->addMatrixProduct(0.0, Me, (*udd_e), 1.0);
                Fk->addMatrixProduct(0.0, Ke, (*u_e), 1.0);


                for (int k = 0; k < NIE; k++)
                {
                    int r = nodetag2index[elementNodes(k)];
                    for (int d = 0; d < NDOF; d++)
                    {
                        for (int j = 0 ; j < Nt; j++)
                        {
                            (*DRMForces)( r * NDOF + d, j) +=  (*Fk)(k * NDOF + d, j) + (*Fm)(k * NDOF + d, j);
                        }
                    }
                }
            }
        } // For elements


        // cout << "DRMFORCES = " << *DRMForces << endl;

        delete Fm;
        delete Fk;
        delete u_e;
        delete udd_e;
    }

}


Vector *
Domain_Reduction_Method_HDF5_input::getNodalLoad(int nodeTag, double time)
{
    //Get the node
    // Domain *theDomain = this->getDomain();
    // Node *theNode = theDomain->getNode(nodeTag);

    // if (theNode == 0)
    // {
    //     DRMerror <<  "no nodes associtated to the nodeTag " << nodeTag << "\n";
    //     exit(1);
    // }


    // Vector *nodalLoad = new Vector(3);

    // if (time < 0.0 )
    // {
    //     DRMwarning << "Returning zero load! \n";
    //     return nodalLoad;
    // }

    // int use_this_step = 0;
    // for (int i = step1; i < step2; i++)
    // {
    //     if ((*times)[i] > time)
    //     {
    //         break;
    //     }
    //     use_this_step++;
    // }

    // // cout << "step used = " << use_this_step << endl;

    // int pos = nodetag2index[nodeTag];
    // for (int i = 0; i < 3; i++)
    // {
    //     (*nodalLoad)(i) = (*DRMForces)(3 * pos + i, use_this_step);
    // }

    // return nodalLoad;

    return 0;
}

