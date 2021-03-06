/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.11 $
// $Date: 2003/02/25 23:32:47 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/beamWithHinges/BeamWithHinges3d.h,v $

#ifndef BeamWithHinges3d_h
#define BeamWithHinges3d_h

#include <Element.h>
#include <Matrix.h>
#include <Vector.h>
#include <ID.h>

class Node;
class Channel;
class FEM_ObjectBroker;

class SectionForceDeformation;

class CrdTransf;
// class Response;
// class Renderer;

class BeamWithHinges3d: public Element
{
    public:
        BeamWithHinges3d(int tag, int nodeI, int nodeJ,
                         double E, double A, double Iz, double Iy, double G, double J,
                         SectionForceDeformation &sectionRefI, double hingeIlen,
                         SectionForceDeformation &sectionRefJ, double hingeJlen,
                         double massDensPerUnitLength,
                         int max, double tol,
                         double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                         double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                         double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);
        BeamWithHinges3d();
        ~BeamWithHinges3d();

        int getNumExternalNodes(void) const;
        const ID &getExternalNodes(void);
        Node **getNodePtrs(void);

        int getNumDOF(void);
        void setDomain(Domain *theDomain);

        int commitState(void);
        int revertToLastCommit(void);
        int revertToStart(void);

        int update(void);
        const Matrix &getTangentStiff(void);
        const Matrix &getInitialStiff(void);
        const Matrix &getMass(void);

        void zeroLoad(void);
        int addLoad(ElementalLoad *theLoad, double loadFactor);
        int addInertiaLoadToUnbalance(const Vector &accel);
        const Vector &getResistingForce(void);
        const Vector &getResistingForceIncInertia(void);

        int sendSelf(int commitTag, Channel &theChannel);
        int receiveSelf(int commitTag, Channel &theChannel,
                     FEM_ObjectBroker &theBroker);

        // Response* setResponse(const char** argv, int argc, Information& info);
        // int getResponse(int responseID, Information& info);

        int setParameter(const char **argv, int argc, Information &info);
        int updateParameter(int parameterID, Information &info);

        void Print(ostream &s, int flag = 0);
        //   int displaySelf(Renderer &theViewer, int displayMode, float fact);


        // Nima Tafazzoli moved from transformation (Nov. 2012)
        int getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis);
        int computeElemtLengthAndOrient();
        int initialize();
        const Vector &getGlobalResistingForce(const Vector &pb, const Vector &p0);
        const Matrix &getInitialGlobalStiffMatrix (const Matrix &KB);
        const Matrix &getGlobalStiffMatrix (const Matrix &KB, const Vector &pb);
        const Vector &getBasicTrialDisp (void);
        const Vector &getBasicIncrDisp (void);
        const Vector &getBasicIncrDeltaDisp (void);


    protected:

    private:

        void setNodePtrs(Domain *theDomain);
        void setHinges(void);

        void getForceInterpMatrix(Matrix &b, double x, const ID &c);
        void getDistrLoadInterpMatrix(Matrix &bp, double x, const ID &c);

        double E, A, Iz, Iy, G, J;
        double beta1, beta2;
        double rho;
        SectionForceDeformation *section[2];
        CrdTransf *theCoordTransf;

        ID connectedExternalNodes;
        Node *theNodes[2];

        Matrix fs[2];
        Vector sr[2];
        Vector  e[2];

        Matrix kb;
        Vector q;
        Vector load;

        Matrix kbCommit;
        Vector qCommit;
        Vector eCommit[2];

        // Nima Tafazzoli (Nov. 2012) moved from transformation
        double R[3][3];  // Transformation matrix
        double *nodeIInitialDisp, *nodeJInitialDisp;
        bool initialDispChecked;
        double *nodeIOffset, *nodeJOffset;  // rigid joint offsets
        double L;           // undeformed element length


        int initialFlag;

        int maxIter;
        double tolerance;

        Matrix *sp;  // Applied section forces due to element loads, 5 x nSections
        double p0[5]; // Reactions in the basic system due to element loads
        double v0[5]; // Basic deformations due to element loads on the interior

        static Matrix theMatrix;
        static Vector theVector;
        static double workArea[];
};

#endif
