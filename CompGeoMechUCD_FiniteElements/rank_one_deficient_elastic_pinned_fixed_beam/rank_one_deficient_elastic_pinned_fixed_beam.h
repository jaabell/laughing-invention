///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Beam element to be connected to solid elements (9 dof)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Jose Abell, Boris Jeremic
//
// DATE:              December 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//============================================================================
// NOTE:
// This beam has zero stiffness components for torsion. It should be used
// either as a connection of solid and another beam element or in case
// of being used on its own, the corresponding dof for torsion (dof 7)
// should be fixed!
// Node 1 should have 3 dofs while node 2 should have 6 dofs.
//============================================================================


#ifndef rank_one_deficient_elastic_pinned_fixed_beam_h
#define rank_one_deficient_elastic_pinned_fixed_beam_h


#include <Domain.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

#include <Information.h>
#include <ElementResponse.h>
#include <ElementalLoad.h>
#include <SectionForceDeformation.h>
#include <ID.h>
#include <math.h>
#include <stdlib.h>


#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>

class Channel;
class Information;
class Response;
class SectionForceDeformation;

class rank_one_deficient_elastic_pinned_fixed_beam : public Element
{
    public:
        rank_one_deficient_elastic_pinned_fixed_beam();
        rank_one_deficient_elastic_pinned_fixed_beam(int tag, double A, double E, double G,
                double Jx, double Iy, double Iz, int Nd1, int Nd2,
                double rho, int sectionTag,
                double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

        rank_one_deficient_elastic_pinned_fixed_beam(int tag, int Nd1, int Nd2, SectionForceDeformation* section,
                double rho,
                double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

        ~rank_one_deficient_elastic_pinned_fixed_beam();

        const char* getClassType(void) const
        {
            return "rank_one_deficient_elastic_pinned_fixed_beam";
        };

        int getNumExternalNodes(void) const;
        const ID& getExternalNodes(void);
        Node** getNodePtrs(void);

        int getNumDOF(void);
        void setDomain(Domain* theDomain);

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        int update(void);
        const Matrix& getTangentStiff(void);
        const Matrix& getInitialStiff(void);
        const Matrix& getMass(void);

        void zeroLoad(void);
        int addLoad(ElementalLoad* theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector& accel);

        const Vector& getResistingForce(void);
        const Vector& getResistingForceIncInertia(void);

        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        void Print(ostream& s, int flag = 0);

        Response* setResponse (const char** argv, int argc, Information& info);
        int getResponse (int responseID, Information& info);

        // Nima Tafazzoli moved from transformation (Nov. 2012)
        int getLocalAxes(Vector& XAxis, Vector& YAxis, Vector& ZAxis);
        int computeElemtLengthAndOrient();
        int initialize();
        const Matrix& getGlobalStiffnessMatrix (const Matrix& KB);
        const Matrix& getGlobalConsistentMassMatrix (const Matrix& KB);
        const Matrix& getConsistentMass(void);
        Vector* getForce(void);



    private:
        double A, E, G, Jx, Iy, Iz;

        double rho;
        int sectionTag;

        Vector* nodeIOffset; // rigid joint offsets
        Vector* nodeJOffset; // rigid joint offsets

        Matrix Mass;
        Matrix Stiffness;
        int builtK;
        int builtM;

        Matrix R;       // Transformation matrix
        Vector P;
        Vector Q;
        Vector q;

        ID  connectedExternalNodes;

        double L;       // undeformed element length

        Vector* nodeIInitialDisp;
        Vector* nodeJInitialDisp;
        bool initialDispChecked;

        Node* theNodes[2];
};

#endif


