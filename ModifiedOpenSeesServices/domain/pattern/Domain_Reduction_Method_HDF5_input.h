//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Plastic Bowl (aka Domain Reduction) implementation:
//#                    This file contains the class definition for Domain_Reduction_Method_HDF5_input.
//#                    Domain_Reduction_Method_HDF5_input is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# CLASS:             Domain_Reduction_Method_HDF5_input
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:
//# PROGRAMMER(S):
//# Based on:
//#
//#
//# DATE:              March 2011
//# UPDATE HISTORY:
//#
//#
//===============================================================================

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


#include <string>


#define DRM_NUM_OF_PRECOMPUTED_TIMESTEPS 10

#define DRMerror std::cerr << std::endl << __PRETTY_FUNCTION__ << " - ERROR: \n"
#define DRMwarning std::cout << std::endl <<  __PRETTY_FUNCTION__ << " - Warning: \n"
#define DRMout std::cout << std::endl << __PRETTY_FUNCTION__ << " : \n"

class Vector;
class Matrix;

class Domain_Reduction_Method_HDF5_input : public LoadPattern
{
public:
    Domain_Reduction_Method_HDF5_input();
    Domain_Reduction_Method_HDF5_input(int tag, std::string HDF5filename_);
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

    std::map<int, int> nodetag2index;

    //HDF5 handles
    hid_t id_drm_file;
    hid_t id_displacements;
    hid_t id_displacements_dataspace;
    hid_t id_accelerations;
    hid_t id_accelerations_dataspace;

    hid_t id_one_node_memspace;

    hid_t id_xfer_plist;
};

#endif
