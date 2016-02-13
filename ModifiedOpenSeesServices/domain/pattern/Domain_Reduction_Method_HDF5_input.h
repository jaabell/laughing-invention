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


#ifndef Domain_Reduction_Method_HDF5_input_h
#define Domain_Reduction_Method_HDF5_input_h

// Purpose:
#include <LoadPattern.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>
#include <Domain.h>
#include <NodeIter.h>
#include <Node.h>
#include <ElementIter.h>
#include <Element.h>
#include <Channel.h>
#include <ErrorHandler.h>
#include <hdf5.h>
#include <map>


#include <algorithm>  // For std::min and std::max functions


#include <string>


#define DRM_NUM_OF_PRECOMPUTED_TIMESTEPS 50

// #define DRMerror std::cerr << std::endl << __PRETTY_FUNCTION__ << " - ERROR: \n"
// #define DRMwarning std::cout << std::endl <<  __PRETTY_FUNCTION__ << " - Warning: \n"
// #define DRMout std::cout << std::endl << __PRETTY_FUNCTION__ << " : \n"

#define DRMerror std::cerr  << "DRMHDF5(" << myrank << ") - ERROR: "
#define DRMwarning std::cout  <<  "DRMHDF5(" << myrank << ") - Warning: "
#define DRMout std::cout  << "DRMHDF5(" << myrank << ") : "

#define DRMHDF5_MAX_RETURN_OPEN_OBJS 100

class Vector;
class Matrix;

class Domain_Reduction_Method_HDF5_input : public LoadPattern
{
public:
    Domain_Reduction_Method_HDF5_input();
    Domain_Reduction_Method_HDF5_input(int tag, std::string HDF5filename_, double cFactor_ = 1.0);
    ~Domain_Reduction_Method_HDF5_input();
    void clean_all_data(); // Called by destructor and if domain changes

    void setDomain(Domain *theDomain);
    void applyLoad(double time);
    void Print(ostream &s, int flag = 0);

    // methods for o/p
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel,
                    FEM_ObjectBroker &theBroker);

    //  method to obtain a blank copy of the LoadPattern
    LoadPattern *getCopy(void);

    virtual std::string getName() const
    {
        return "Domain_Reduction_Method_HDF5_input: " + HDF5filename;
    }



protected:

    void ComputeDRMLoads();
    Vector *getNodalLoad(int node, double time);

    void intitialize();

private:

    std::string HDF5filename;

    ID *Elements;   // integer list containing the DRM elements
    ID *Nodes;      // integer list containing the nodes belonging to the DRM elements
    ID *IsBoundary; // integer list containing a flag indicating whether that node is a boundary node

    Vector *times;
    Matrix *DRMForces;  // matrix containing the DRM loads

    double t, t1, t2, tfinal;        // specifies the time increment used in load path vector
    double cFactor;      // additional factor on the returned load factor

    int step, step1, step2;

    int number_of_DRM_elements;
    int number_of_DRM_nodes;
    int number_of_DRM_nodes_b;
    int number_of_DRM_nodes_e;
    int number_of_timesteps;
    int thetimeSteps;

    int maxnodetag;

    bool is_initialized;

    int number_of_local_elements;

    std::map<int, int> nodetag2index;
    std::map<int, int> nodetag2hdf5_pos;

    //HDF5 handles
    hid_t id_drm_file;
    hid_t id_displacements;
    hid_t id_displacements_dataspace;
    hid_t id_accelerations;
    hid_t id_accelerations_dataspace;

    hid_t id_one_node_memspace;

    hid_t id_xfer_plist;

    int myrank;
};

#endif
