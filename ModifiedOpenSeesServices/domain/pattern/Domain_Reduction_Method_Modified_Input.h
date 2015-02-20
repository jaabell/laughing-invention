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
//#                    This file contains the class definition for Domain_Reduction_Method_Modified_Input.
//#                    Domain_Reduction_Method_Modified_Input is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# CLASS:             Domain_Reduction_Method_Modified_Input
//#
//# VERSION:           0.61803398874989 (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Nima Tafazzoli , Boris Jeremic
//# PROGRAMMER(S):     Nima Tafazzoli , Boris Jeremic
//# Based on:          PBowlLoading and Domain_Reduction_Method written by:
//#                    Jinxiu Liao, Zhaohui Yang, Nima Tafazzoli, Boris Jeremic
//#
//#
//# DATE:              March 2011
//# UPDATE HISTORY:
//#
//#
//===============================================================================

#ifndef Domain_Reduction_Method_Modified_Input_h
#define Domain_Reduction_Method_Modified_Input_h

// Purpose:
#include <LoadPattern.h>
#include <Matrix.h>
#include <Domain.h>
#include <NodeIter.h>
#include <Node.h>
#include <ElementIter.h>
#include <Element.h>
#include <stdlib.h>
#include <Channel.h>
#include <ErrorHandler.h>

#include <OPS_Globals.h>

class Vector;
class Matrix;

class Domain_Reduction_Method_Modified_Input : public LoadPattern
{
    public:
        Domain_Reduction_Method_Modified_Input();
        Domain_Reduction_Method_Modified_Input(int tag,
                                               const char *ElementsFilename,
                                               const char *DispfName,
                                               const char *AccefName,
                                               const char *boundaryNodesFile,
                                               const char *externalNodesFile,
                                               double theTimeIncr,
                                               double theFactor,
                                               int timeSteps,
                                               int Number_of_DRM_Elements,
                                               int Number_of_DRM_Nodes_b,
                                               int Number_of_DRM_Nodes_e);
        ~Domain_Reduction_Method_Modified_Input();

        void setDomain(Domain *theDomain);
        void applyLoad(double time);
        void Print(ostream &s, int flag = 0);

        // methods for o/p
        int sendSelf(int commitTag, Channel &theChannel);
        int receiveSelf(int commitTag, Channel &theChannel,
                        FEM_ObjectBroker &theBroker);

        //  method to obtain a blank copy of the LoadPattern
        LoadPattern *getCopy(void);

    protected:
        //void addPBElements(const ID &PBEle);    //Adding plastic bowl elements
        //void addPBNodes(const ID &PBNodes);     //Adding plastic bowl nodes
        //void addPBLoads(const Matrix &PBLoads); //Adding plastic bowl loades
        void CompPBLoads();        //Finding all plastic bowl nodes and compute the equivalent forces from plastic bowl loading
        Vector *getNodalLoad(int node, double time); //Getting the nodal load computed from plastic bowl loading corresponding to time

    private:
        ID *PBowlElements;   // vector containing the plastic bowling elements
        ID *ExteriorNodes;   // vector containing the nodes on plastic bowl except boundary nodes
        ID *BoundaryNodes;   // vector containing the nodes on the boundary of the plastic bowl

        ID *NodeID;     //Guanzhou added to reduce input file size

        Matrix *PBowlLoads;  // matrix containing the plastic bowling loads

        Matrix *U;           // vector to store input displ. for all nodes and all time steps
        int UnumDataPoints;   // number of data points
        Matrix *Udd;         // vector to store input accel. for all nodes and all time steps
        int UddnumDataPoints;// number of data points


        //Coordinates for the plastic box
        double PBTimeIncr;   // specifies the time increment used in load path vector
        double cFactor;      // additional factor on the returned load factor

        int number_of_DRM_elements;
        int number_of_DRM_nodes;
        int number_of_DRM_nodes_b;
        int number_of_DRM_nodes_e;
        int thetimeSteps;
        bool LoadComputed;   // flag to indicate whether the equivalent force has been computed
        char *DRMElementsFile;
        char *DRMNodesFile;
        char *DRMDisplacementsFile;
        char *DRMAccelerationsFile;
        int maxnodetag;
};

#endif
