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

// $Revision: 1.12 $
// $Date: 2004/11/24 22:49:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/domain/Domain.h,v $

// Written: fmk
// Created: Fri Sep 20 15:27:47: 1996
// Revision: A
//
// Description: This file contains the class definition for Domain.
// Domain is a container class. The class is responsible for holding
// and providing access to the Elements, Nodes, SP_Constraints
// MP_Constraints, and LoadPatterns.
//
// What: "@(#) Domain.h, revA"

#ifndef Domain_h
#define Domain_h

#include <iostream>
#include <Vector.h>

#ifndef _bool_h
#include <bool.h>
#endif

class Element;
class Node;


//*****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
class UniaxialMaterial;
class NDMaterial;
class NDMaterialLT;                             //Jose added on Nov., 13 2013
class SectionForceDeformation;
class SectionRepres;
class MultiSupportPattern;

// Nima Tafazzoli (Oct. 2010)
class AccelerationField;
class Damping;
//class Rayleigh;
//*****************************************************************************************

class SP_Constraint;
class MP_Constraint;
class NodalLoad;
class ElementalLoad;
class LoadPattern;

class ElementIter;
class NodeIter;
class SP_ConstraintIter;
class MP_ConstraintIter;
class LoadPatternIter;

class SingleDomEleIter;
class SingleDomNodIter;
class SingleDomSP_Iter;
class SingleDomMP_Iter;
class SingleDomAllSP_Iter;

//Nima Tafazzoli (Sep. 2012)
class UniaxialMaterialIter;
class NDMaterialIter;
class SingleDomUniaxialMaterialIter;
class SingleDomNDMaterialIter;



// class MeshRegion;
//class Recorder;
class Graph;
class NodeGraph;
class ElementGraph;
class Channel;
class FEM_ObjectBroker;
class FE_Datastore;

class TaggedObjectStorage;

class Domain
{
    public:
        Domain();
        Domain(int numNodes, int numElements, int numSPs, int numMPs,
               int numLoadPatterns, int numUniaxialMat, int numNDMaterial, int numSections,
               int numofSectionRepresents,
               int nummultipleexcitation, int numAccelerationFields, int numDamping);

        //*****************************************************************************************
        // Nima Tafazzoli (changed on Sep. 2nd 2009)
        // Instead of Using TclModelBuilder
        Domain(TaggedObjectStorage &theNodesStorage,
               TaggedObjectStorage &theElementsStorage,
               TaggedObjectStorage &theMPsStorage,
               TaggedObjectStorage &theSPsStorage,
               TaggedObjectStorage &theLoadPatternsStorage,
               TaggedObjectStorage &theUniaxialMaterialStorage,
               TaggedObjectStorage &theNDMaterialStorage,
               TaggedObjectStorage &theSectionStorage,
               TaggedObjectStorage &theSectionRepresentsStorage,
               TaggedObjectStorage &theMultiSupportStorage,
               TaggedObjectStorage &theAccelerationFieldsStorage,
               TaggedObjectStorage &theDampings);

        Domain(TaggedObjectStorage &theStorageType);
        virtual ~Domain();

        //*****************************************************************************************
        // methods to populate a domain
        virtual  bool addElement(Element *);
        virtual  bool addNode(Node *);
        virtual  bool addElementDatabase(Element *);

        //*****************************************************************************************
        // Nima Tafazzoli (added on Sep. 2nd 2009)
        // Instead of Using TclModelBuilder
        virtual   int addUniaxialMaterial(UniaxialMaterial &theMaterial);
        virtual   int addNDMaterial(NDMaterial &theMaterial);
        virtual   int addNDMaterialLT(NDMaterialLT &theMaterial);
        virtual   int addSection(SectionForceDeformation &theSection);
        virtual   int addSectionRepres(SectionRepres &theSectionRepres);
        virtual   int addMultipleSupport(MultiSupportPattern &theMultiSupportPattern);
        virtual   int addAccelerationField(AccelerationField *theAccelerationField);
        virtual   int addDamping(Damping *theDampings);

        virtual  bool addSP_Constraint(SP_Constraint *);
        virtual  bool addMP_Constraint(MP_Constraint *);
        virtual  bool addLoadPattern(LoadPattern *);

        // methods to add components to a LoadPattern object
        virtual  bool addSP_Constraint(SP_Constraint *, int loadPatternTag);
        virtual  bool addNodalLoad(NodalLoad *, int loadPatternTag);
        virtual  bool addElementalLoad(ElementalLoad *, int loadPatternTag);

        //*****************************************************************************************
        // methods to remove the components
        virtual void clearAll(void);
        virtual Element       *removeElement(int tag);
        virtual Node          *removeNode(int tag);

        //*****************************************************************************************
        // Nima Tafazzoli (added on Sep. 2nd 2009)
        // Instead of Using TclModelBuilder
        virtual UniaxialMaterial  *removeUniaxialMaterial(int tag);
        virtual NDMaterial  *removeNDMaterial(int tag);

        // Nima Tafazzoli (added on Oct. 2010)
        virtual AccelerationField  *removeAccelerationField(int tag);
        virtual Damping  *removeDamping(int tag);

        //*****************************************************************************************
        virtual SP_Constraint *removeSP_Constraint(int tag);
        virtual SP_Constraint *removeSP_Constraint(int nodeTag, int dof, int loadPatternTag);
        virtual MP_Constraint *removeMP_Constraint(int tag);
        virtual int removeMP_Constraints(int constrainedNodeTag);

        //*****************************************************************************************
        // Nima Tafazzoli (added on June 2011)

        virtual LoadPattern   *removeLoadPattern(int loadTag);
        virtual NodalLoad     *removeNodalLoad(int tag, int loadPattern);
        virtual ElementalLoad *removeElementalLoad(int tag, int loadPattern);
        virtual SP_Constraint *removeSP_Constraint(int tag, int loadPattern);

        // methods to access the components of a domain
        virtual  ElementIter       &getElements();
        virtual  NodeIter          &getNodes();
        virtual  SP_ConstraintIter &getSPs();
        virtual  MP_ConstraintIter &getMPs();
        virtual  LoadPatternIter   &getLoadPatterns();
        virtual  SP_ConstraintIter &getDomainAndLoadPatternSPs();
        virtual  UniaxialMaterialIter  &getUniaxialMaterials();
        virtual  NDMaterialIter        &getNDMaterials();

        //Accessing nodes an elements by tag
        virtual  Element       *getElement(int tag);
        virtual  Node          *getNode(int tag);


        //*****************************************************************************************
        // Nima Tafazzoli (added on Sep. 2nd 2009)
        // Instead of Using TclModelBuilder
        virtual  UniaxialMaterial  *getUniaxialMaterial(int tag);
        virtual  NDMaterial  *getNDMaterial(int tag);
        virtual  NDMaterialLT  *getNDMaterialLT(int tag);
        virtual  SectionForceDeformation *getSection(int tag);
        virtual  SectionRepres *getSectionRepres(int tag);
        virtual  MultiSupportPattern *getMultipleSupport(int tag);
        virtual  AccelerationField *getAccelerationField(int tag);
        virtual  Damping *getDamping(int tag);
        virtual  SP_Constraint *getSP_Constraint(int tag);
        virtual  MP_Constraint *getMP_Constraint(int tag);
        virtual  LoadPattern   *getLoadPattern(int tag);

        // methods to query the state of the domain
        //*****************************************************************************************
        virtual double  getCurrentTime(void) const;
        virtual int     getCommitTag(void) const;
        virtual int getNumElements(void) const;
        virtual int getNumNodes(void) const;
        virtual int getNumSPs(void) const;
        virtual int getNumMPs(void) const;
        virtual int getNumLoadPatterns(void) const;
        virtual const Vector &getPhysicalBounds(void);

        // methods to get element and node graphs
        virtual  Graph    *getElementGraph(void);
        virtual  Graph  &getNodeGraph(void);

        // methods to update the domain
        virtual  void setCommitTag(int newTag);
        virtual  void setCurrentTime(double newTime);
        virtual  void setCommittedTime(double newTime);
        virtual  void applyLoad(double pseudoTime);
        virtual  void setLoadConstant(void);
        virtual  int  initialize(void);
        //    virtual  int  setRayleighDampingFactors(double alphaM, double betaK, double betaK0, double betaKc);

        //*****************************************************************************************
        // Nima Tafazzoli
        // (July 2011)
        virtual  int  setDampingFactorsforElement(int ElementTag, int DampingTag);
        virtual  int  setDampingFactorsforNode(int NodeTag, int DampingTag);
        // (June 2012)
        virtual  int  eigenAnalysis(int numMode);
        // (September 2012)
        virtual  int  CheckMesh(const char *);
        // (January 2013)
        virtual  int  getNumberof8GPBrickElements(void);
        virtual  int  getNumberof27GPBrickElements(void);
        virtual  int  getNumberofLineElements(void);
        //*****************************************************************************************


        virtual  int  commit(void);
        virtual  int  revertToLastCommit(void);
        virtual  int  revertToStart(void);
        virtual  int  update(void);
        virtual  int  update(double newTime, double dT);
        virtual  int  newStep(double dT);


        // methods for eigenvalue analysis
        virtual int setEigenvalues(const Vector &theEigenvalues);
        virtual const Vector &getEigenvalues(void);
        virtual double getTimeEigenvaluesSet(void);

        // methods for other objects to determine if model has changed
        virtual void domainChange(void);
        virtual int hasDomainChanged(void);
        virtual void setDomainChangeStamp(int newStamp);

        // methods for output
        //virtual int  addRecorder(Recorder &theRecorder);
        //virtual int  removeRecorders(void);
        //virtual int  removeRecorder(int tag);

        //     virtual int  addRegion(MeshRegion &theRegion);
        //     virtual MeshRegion *getRegion(int region);

        virtual void Print(ostream &s, int flag = 0);
        friend ostream &operator<<(ostream &s, Domain &M);

        virtual int sendSelf(int commitTag, Channel &theChannel);
        virtual int recvSelf(int commitTag, Channel &theChannel,
                             FEM_ObjectBroker &theBroker);

        virtual int sendSelfDatabase(int commitTag, Channel &theChannel);
        virtual int recvSelfDatabase(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

        //Babak Added on Aug 2012:
        void Dump_All_Nodes_Displacement_Onefileperprocess(void);
        void Dump_All_Nodes_Displacement_Singlefile(void);
        //Babak Added on October 2012:
        void Dump_All_Nodes_Displacement_Singlefile_GIDFormat(void);

        //=======================================================================================
        // Nima Tafazzoli added for saving results using mySQL, November 2012
        //     virtual int saveResults(int commitTag, Channel &theChannel);
        //     virtual int restoreResults(int commitTag, Channel &theChannel,
        //                                FEM_ObjectBroker &theBroker);
        //     virtual Vector& restoreNodalDisplacement(int commitTag, int NodeNumber, int dof, int stepNumber, Channel &theChannel,
        //                                              FEM_ObjectBroker &theBroker);
        //     virtual double restoreNodalDisplacement(int commitTag, int NodeNumber, int dof, int stepNumber, Channel &theChannel,
        //                                              FEM_ObjectBroker &theBroker);
        virtual int saveNodalDisplacements(int stepNumber, Channel &theChannel);
        virtual int save8GPBrickStresses(int stepNumber, Channel &theChannel);
        virtual int save27GPBrickStresses(int stepNumber, Channel &theChannel);
        virtual int saveLineElementForces(int stepNumber, Channel &theChannel);
        virtual int addDatabase(FE_Datastore &theDatabase);
        virtual FE_Datastore &getDatabase(int tag);
        virtual int removeDatabase(int tag);
        //=======================================================================================



# ifdef _PDD //Guanzhou added
        virtual bool recvNode(Node *theNod);
# endif

#ifdef _PARALLEL_PROCESSING
        //Guanzhou added this func
        virtual int  resetMultipleSupport(const int loadPatternTag)
        {
            return 0;
        };
        virtual bool hasInternalNode(int);
        //virtual int  resetRecorders(void);
        virtual Node *getOutsideNode(int tag)
        {
            return 0;
        };

#endif

        virtual int calculateNodalReactions(int flag);


    protected:
        virtual int buildEleGraph(Graph *theEleGraph);
        virtual int buildNodeGraph(Graph *theNodeGraph);

        //Recorder **theRecorders;
        //int numRecorders;


        // Nima Tafazzoli, Nov. 2012
        FE_Datastore **theDatabases;
        int numDatabases;


    private:
        double currentTime;               // current pseudo time
        double committedTime;             // the committed pseudo time
        double dT;                        // difference between committed and current time
        int    currentGeoTag;             // an integer used to mark if domain has changed
        bool   hasDomainChangedFlag;      // a bool flag used to indicate if GeoTag needs to be ++
        int    theDbTag;                   // the Domains unique database tag == 0
        int    lastGeoSendTag;            // the value of currentGeoTag when sendSelf was last invoked
        int    dbEle, dbNod, dbSPs, dbMPs, dbLPs; // database tags for storing info


        bool eleGraphBuiltFlag;
        bool nodeGraphBuiltFlag;

        Graph *theNodeGraph;
        Graph *theElementGraph;

        TaggedObjectStorage  *theElements;
        TaggedObjectStorage  *theNodes;

        //*****************************************************************************************
        // Nima Tafazzoli (added on Sep. 2nd 2009)
        // Instead of Using TclModelBuilder
        TaggedObjectStorage *theUniaxialMaterials;
        TaggedObjectStorage *theNDMaterials;
        TaggedObjectStorage *theSections;
        TaggedObjectStorage *theSectionRepresents;
        TaggedObjectStorage *theMultipleSupports;
        TaggedObjectStorage *theAccelerationFields;
        TaggedObjectStorage *theDampings;
        //*****************************************************************************************

        TaggedObjectStorage  *theSPs;
        TaggedObjectStorage  *theMPs;
        TaggedObjectStorage  *theLoadPatterns;

        SingleDomEleIter      *theEleIter;
        SingleDomNodIter      *theNodIter;
        SingleDomSP_Iter      *theSP_Iter;
        SingleDomMP_Iter      *theMP_Iter;
        LoadPatternIter       *theLoadPatternIter;
        SingleDomAllSP_Iter   *allSP_Iter;


        // Nima Tafazzoli (Sep. 2012)
        SingleDomUniaxialMaterialIter  *theUniMaterialIter;
        SingleDomNDMaterialIter        *theNDMaterialIter;

        //     MeshRegion **theRegions;
        //     int numRegions;

        int commitTag;

        Vector theBounds;

        Vector *theEigenvalues;
        double theEigenvalueSetTime;

        // Nima Tafazzoli added Jan. 2013
        int number_of_8GP_brick_elements;
        int number_of_27GP_brick_elements;
        int number_of_line_elements;

        int lastChannel;
};

#endif


