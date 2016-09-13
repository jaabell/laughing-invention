///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli , Boris Jeremic
// DATE:               June 1st. 2010
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


//# PURPOSE:           Plastic Bowl (aka Domain Reduction) implementation:
//#                    This file contains the class definition for Domain_Reduction_Method.
//#                    Domain_Reduction_Method is a subclass of loadPattern,
//#                    which implements the plastic bowl loading
//#                    (aka Domain Reduction Method) as described
//#                    by Jacobo Bielak et al.
//# Based on:          PBowlLoading written by:
//#                    Jinxiu Liao, Zhaohui Yang, Boris Jeremic

#ifndef Domain_Reduction_Method_h
#define Domain_Reduction_Method_h

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

class Domain_Reduction_Method : public LoadPattern
{
public:
    Domain_Reduction_Method();
    Domain_Reduction_Method(int tag,
                            const char* PBEfName,
                            const char* DispfName,
                            const char* AccefName,
                            double theTimeIncr = 1.0,
                            double theFactor = 1.0,
                            int numDOF = 3,
                            // coordinates of "b" nodes for cubic plastic bowl
                            double xplus  = 0.0,
                            double xminus = 0.0,
                            double yplus  = 0.0,
                            double yminus = 0.0,
                            double zplus  = 0.0,
                            double zminus = 0.0);
    ~Domain_Reduction_Method();

    void setDomain(Domain* theDomain);
    void applyLoad(double time);
    void Print(ostream& s, int flag = 0);

    // methods for o/p
    int sendSelf(int commitTag, Channel& theChannel);
    int receiveSelf(int commitTag, Channel& theChannel,
                    FEM_ObjectBroker& theBroker);

    //  method to obtain a blank copy of the LoadPattern
    LoadPattern* getCopy(void);

protected:
    //void addPBElements(const ID &PBEle);    //Adding plastic bowl elements
    //void addPBNodes(const ID &PBNodes);     //Adding plastic bowl nodes
    //void addPBLoads(const Matrix &PBLoads); //Adding plastic bowl loades
    void CompPBLoads();        //Finding all plastic bowl nodes and compute the equivalent forces from plastic bowl loading
    Vector* getNodalLoad(int node, double time); //Getting the nodal load computed from plastic bowl loading corresponding to time

private:
    ID* PBowlElements;   // vector containing the plastic bowling elements
    ID* ExteriorNodes;   // vector containing the nodes on plastic bowl except boundary nodes
    ID* BoundaryNodes;   // vector containing the nodes on the boundary of the plastic bowl

    ID* NodeID;     //Guanzhou added to reduce input file size

    Matrix* PBowlLoads;  // matrix containing the plastic bowling loads

    Matrix* U;           // vector to store input displ. for all nodes and all time steps
    int UnumDataPoints;   // number of data points
    Matrix* Udd;         // vector to store input accel. for all nodes and all time steps
    int UddnumDataPoints;// number of data points

    int thetimeSteps;


    //Coordinates for the plastic box
    double PBTimeIncr;   // specifies the time increment used in load path vector
    double cFactor;      // additional factor on the returned load factor
    int numNodeDOF;   // number of dofs per node
    double xPlus;    // x-coor for the right surface
    double xMinus;       // x-coor for the left  surface
    double yPlus;    // y-coor for the right surface
    double yMinus;   // y-coor for the left surface
    double zPlus;    // z-coor for the up surface
    double zMinus;   // z-coor for the bottom surface

    bool LoadComputed;   // flag to indicate whether the equivalent force has been computed
};

#endif
