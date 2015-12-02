/* *******************************************************************************
**  Example: Cantilever Pure Bending
**  Written: fmk
**
**  Reference:
**  J.C.Simo and L.Vu-Quoc
**  "A Three-Dimensional Finite-Strain Rod Model. Part II: Computational Aspects"
**  Computer Methods in Applied Mechanics and Engineering, 58(1986), 79-11
** ****************************************************************************** */

#include "ElasticSection3d.h"
#include "FiberSectionGJ.h"
#include "ElasticMaterial.h"
#include "CorotCrdTransf3d.h"
#include "UniaxialFiber3d.h"
#include "DispBeamColumn3d.h"
#include "LobattoBeamIntegration.h"

#include <Vector.h>
#include <Node.h>
#include <Domain.h>
#include <SP_Constraint.h>
#include <LinearSeries.h>
#include <LoadPattern.h>
#include <NodalLoad.h>
// #include <StandardStream.h>

// includes for the analysis classes
#include <StaticAnalysis.h>
#include <AnalysisModel.h>
#include <NewtonRaphson.h>
#include <CTestNormDispIncr.h>
#include <PlainHandler.h>
#include <DOF_Numberer.h>
#include <RCM.h>
#include <LoadControl.h>
#include <BandGenLinSOE.h>
#include <BandGenLinLapackSolver.h>

// #include <

// init the global variabled defined in OPS_Globals.h
//StandardStream sserr;
//OPS_Stream *opserrPtr = &sserr;
double ops_Dt;

#define numDivision 5

int main(int argc, const char **argv)
{
    Domain *theDomain = new Domain();

    /* *******************
    ** use elastic section
    ** ******************* */

    // add nodes
    for (int i = 0; i <= numDivision; i++)
    {
        double xCrd = i * 1.0 / numDivision;
        Node *node = new Node(i + 1, 6, xCrd, 0., 0.);
        theDomain->addNode(node);
    }

    // fix node 1
    for (int i = 0; i <= 5; i++)
    {
        SP_Constraint *sp = new SP_Constraint(i, 1, i, 0, true);
        theDomain->addSP_Constraint(sp);
    }

    // create a section and transformation for the elements
    int np = 5;
    ElasticSection3d *section = new ElasticSection3d(1, 1., 1., 2., 2., 1., 1.);
    SectionForceDeformation **theSections = new SectionForceDeformation *[np];
    for (int i = 0; i < np; i++)
    {
        theSections[i] = section;
    }
    Vector vecXZ(3);
    vecXZ(0) = 0.;
    vecXZ(1) = 1.;
    vecXZ(2) = 0.;
    Vector offset(3);
    offset.Zero();
    CorotCrdTransf3d *crdTransf = new CorotCrdTransf3d(1, vecXZ, offset, offset);
    BeamIntegration *beamInteg = new LobattoBeamIntegration();


    // add the elements
    for (int i = 1; i <= numDivision; i++)
    {
        Element *theEle = new DispBeamColumn3d(i, i, i + 1, np, theSections,
                                               *beamInteg, *crdTransf);
        theDomain->addElement(theEle);

    }

    // construct a linear time series object using constructor:
    //        LinearSeries()

    TimeSeries *theSeries = new LinearSeries();

    // construct a load pattren using constructor:
    //        LoadPattern(tag)
    // and then set it's TimeSeries and add it to the domain

    LoadPattern *theLoadPattern = new LoadPattern(1);
    theLoadPattern->setTimeSeries(theSeries);
    theDomain->addLoadPattern(theLoadPattern);

    // construct a nodal load using constructor:
    //        NodalLoad(tag, nodeID, Vector &)
    // first construct a Vector of size 2 and set the values NOTE C INDEXING
    // then construct the load and add it to the domain

    Vector theLoadValues(6);
    theLoadValues(1) = 4.0 * 3.14159; // 4PI results in 1 full loop
    NodalLoad *theLoad = new NodalLoad(1, numDivision + 1, theLoadValues);
    theDomain->addNodalLoad(theLoad, 1);

    // create an Analysis object to perform a static analysis of the model
    int numSteps = 40;
    AnalysisModel     *theModel = new AnalysisModel();
    ConvergenceTest   *theTest = new CTestNormDispIncr(1.0e-6, 6, 4);
    EquiSolnAlgo      *theSolnAlgo = new NewtonRaphson(*theTest);
    StaticIntegrator  *theIntegrator = new LoadControl(1.0 / numSteps, 1, 1.0 / numSteps, 1.0 / numSteps);
    ConstraintHandler *theHandler = new PlainHandler();
    RCM               *theRCM = new RCM();
    DOF_Numberer      *theNumberer = new DOF_Numberer(*theRCM);
    BandGenLinSolver  *theSolver = new BandGenLinLapackSolver();
    LinearSOE         *theSOE = new BandGenLinSOE(*theSolver);

    StaticAnalysis    theAnalysis(*theDomain,
                                  *theHandler,
                                  *theNumberer,
                                  *theModel,
                                  *theSolnAlgo,
                                  *theSOE,
                                  *theIntegrator);

    theAnalysis.setConvergenceTest(*theTest);

    // perform the analysis & print out the results for the domain
    theAnalysis.analyze(numSteps);
    Node *theNode = theDomain->getNode(numDivision + 1);
    cout << *theNode;


    cout << *theDomain;

    exit(0);
    /* ************************************
    ** use elastic material & Fiber Section
    ** ************************************ */

    // clear out previous elements, nodes, etc.
    theDomain->clearAll();

    // add nodes
    for (int i = 0; i <= numDivision; i++)
    {
        double xCrd = i * 1.0 / numDivision;
        Node *node = new Node(i + 1, 6, xCrd, 0., 0.);
        theDomain->addNode(node);
    }

    // fix node 1
    for (int i = 0; i <= 5; i++)
    {
        SP_Constraint *sp = new SP_Constraint(1, i, 0., true);
        theDomain->addSP_Constraint(sp);
    }

    // create a section and transformation for the elements
    FiberSectionGJ *section1 = new FiberSectionGJ(1, 0, 0, 1e10);
    ElasticMaterial *theMaterial = new ElasticMaterial(1, 1.0, 0.0);

    Vector pos(2);

    pos(0) = 1;
    pos(1) = 1;
    Fiber *theFiber = new UniaxialFiber3d(1, *theMaterial, 10., pos);
    section1->addFiber(*theFiber);

    pos(0) = -1;
    pos(1) = 1;
    theFiber = new UniaxialFiber3d(2, *theMaterial, 10., pos);
    section1->addFiber(*theFiber);

    pos(0) = -1;
    pos(1) = -1;
    theFiber = new UniaxialFiber3d(3, *theMaterial, 10., pos);
    section1->addFiber(*theFiber);

    pos(0) = 1;
    pos(1) = -1;
    theFiber = new UniaxialFiber3d(4, *theMaterial, 10., pos);
    section1->addFiber(*theFiber);

    SectionForceDeformation **theSections1 = new SectionForceDeformation *[np];
    for (int i = 0; i < np; i++)
    {
        theSections1[i] = section1;
    }

    // add the elements
    for (int i = 1; i <= numDivision; i++)
    {
        Element *theEle = new DispBeamColumn3d(i, i, i + 1, np, theSections1,
                                               *beamInteg, *crdTransf);
        theDomain->addElement(theEle);
    }

    // construct a linear time series object using constructor:
    //        LinearSeries()

    TimeSeries *theSeries1 = new LinearSeries();

    // construct a load pattren using constructor:
    //        LoadPattern(tag)
    // and then set it's TimeSeries and add it to the domain

    LoadPattern *theLoadPattern1 = new LoadPattern(1);
    theLoadPattern1->setTimeSeries(theSeries1);
    theDomain->addLoadPattern(theLoadPattern1);

    // construct a nodal load using constructor:
    //        NodalLoad(tag, nodeID, Vector &)
    // first construct a Vector of size 2 and set the values NOTE C INDEXING
    // then construct the load and add it to the domain

    NodalLoad *theLoad1 = new NodalLoad(1, numDivision + 1, theLoadValues);
    theDomain->addNodalLoad(theLoad1, 1);

    // create an Analysis object to perform a static analysis of the model
    theAnalysis.domainChanged();
    theAnalysis.analyze(numSteps);


    Node *theNode1 = theDomain->getNode(numDivision + 1);

    // opserr << *theNode1;
}
