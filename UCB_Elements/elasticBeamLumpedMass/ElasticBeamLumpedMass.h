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

// $Revision: 1.8 $
// $Date: 2003/03/11 20:42:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/elasticBeamColumn/ElasticBeamLumpedMass.h,v $


// File: ~/model/ElasticBeamLumpedMass.h
//
// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class definition for ElasticBeamLumpedMass.
// ElasticBeamLumpedMass is a plane frame member.

#ifndef ElasticBeamLumpedMass_h
#define ElasticBeamLumpedMass_h

#include <Element.h>
#include <Node.h>
#include <Matrix.h>
#include <Vector.h>

class Channel;
class Information;
class CrdTransf;
// class Response;
// class Renderer;
class SectionForceDeformation;

class ElasticBeamLumpedMass : public Element
{
    public:
        ElasticBeamLumpedMass();
        ElasticBeamLumpedMass(int tag, double A, double E, double G,
                              double Jx, double Iy, double Iz, int Nd1, int Nd2,
                              double rho, int sectionTag,
                              double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                              double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                              double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

        ElasticBeamLumpedMass(int tag, int Nd1, int Nd2, SectionForceDeformation *section,
                              double rho,
                              double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                              double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                              double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z);

        ~ElasticBeamLumpedMass();

        const char *getClassType(void) const
        {
            return "ElasticBeamLumpedMass";
        };

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
        int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

        void Print(ostream &s, int flag = 0);

        // Response* setResponse (const char** argv, int argc, Information& info);
        // int getResponse (int responseID, Information& info);

        // Nima Tafazzoli moved from transformation (Nov. 2012)
        int getLocalAxes(Vector &XAxis, Vector &YAxis, Vector &ZAxis);
        int computeElemtLengthAndOrient();
        int initialize();
        const Vector &getGlobalResistingForce(const Vector &pb, const Vector &p0);
        const Matrix &getInitialGlobalStiffMatrix (const Matrix &KB);
        const Matrix &getGlobalStiffMatrix (const Matrix &KB);
        const Vector &getBasicTrialDisp (void);

        Vector *getForce(void);

        std::string getElementName() const
        {
            return "ElasticBeamLumpedMass";
        }

    private:
        double A, E, G, Jx, Iy, Iz;

        double rho;
        Matrix R;       // Transformation matrix [Nima Tafazzoli (Nov. 2012)]
        int sectionTag;

        static Matrix K;
        static Vector P;
        Vector Q;

        static Matrix kb;
        Vector q;
        double q0[5];  // Fixed end forces in basic system (no torsion)
        double p0[5];  // Reactions in basic system (no torsion)

        Node *theNodes[2];

        ID  connectedExternalNodes;


        // Nima Tafazzoli (Nov. 2012) moved from transformation
        Vector *nodeIOffset; // rigid joint offsets
        Vector *nodeJOffset; // rigid joint offsets
        double L;       // undeformed element length
        Vector *nodeIInitialDisp;
        Vector *nodeJInitialDisp;
        bool initialDispChecked;

};

#endif


