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
class NDMaterialLTIter;
class SingleDomUniaxialMaterialIter;
class SingleDomNDMaterialIter;
class SingleDomNDMaterialLTIter;



// class MeshRegion;
//class Recorder;
class Graph;
class NodeGraph;
class ElementGraph;
// class Channel;
#include <HDF5_Channel.h>
#include <OutputWriter.h>
#include <H5OutputWriter.h>
class FEM_ObjectBroker;
class FE_Datastore;

class TaggedObjectStorage;

class Domain
{
    public:
        //This class has 4 constructors..... four!
        Domain();
        Domain(int numNodes, int numElements, int numSPs, int numMPs,
               int numLoadPatterns, int numUniaxialMat, int numNDMaterial, int numNDMaterialLT, int numSections,
               int numofSectionRepresents,
               int nummultipleexcitation, int numAccelerationFields, int numDamping);
        Domain(TaggedObjectStorage &theNodesStorage,
               TaggedObjectStorage &theElementsStorage,
               TaggedObjectStorage &theMPsStorage,
               TaggedObjectStorage &theSPsStorage,
               TaggedObjectStorage &theLoadPatternsStorage,
               TaggedObjectStorage &theUniaxialMaterialStorage,
               TaggedObjectStorage &theNDMaterialStorage,
               TaggedObjectStorage &theNDMaterialLTStorage,
               TaggedObjectStorage &theSectionStorage,
               TaggedObjectStorage &theSectionRepresentsStorage,
               TaggedObjectStorage &theMultiSupportStorage,
               TaggedObjectStorage &theAccelerationFieldsStorage,
               TaggedObjectStorage &theDampings);
        Domain(TaggedObjectStorage &theStorageType);

        //And a destructor... only one. C++ standard only allows one
        virtual ~Domain();

        //*****************************************************************************************
        // methods to populate a domain
        virtual  bool addElement(Element *);
        virtual  bool addNode(Node *);
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
        virtual Element            *removeElement(int tag);
        virtual Node               *removeNode(int tag);
        virtual UniaxialMaterial   *removeUniaxialMaterial(int tag);
        virtual NDMaterial         *removeNDMaterial(int tag);
        virtual NDMaterialLT       *removeNDMaterialLT(int tag);
        virtual AccelerationField *removeAccelerationField(int tag);
        virtual Damping            *removeDamping(int tag);
        virtual SP_Constraint      *removeSP_Constraint(int tag);
        virtual SP_Constraint      *removeSP_Constraint(int nodeTag, int dof, int loadPatternTag);
        virtual MP_Constraint      *removeMP_Constraint(int tag);
        virtual int                 removeMP_Constraints(int constrainedNodeTag);
        virtual LoadPattern        *removeLoadPattern(int loadTag);
        virtual NodalLoad          *removeNodalLoad(int tag, int loadPattern);
        virtual ElementalLoad      *removeElementalLoad(int tag, int loadPattern);
        virtual SP_Constraint      *removeSP_Constraint(int tag, int loadPattern);


        // methods to access the components of a domain
        virtual  ElementIter               &getElements();
        virtual  NodeIter                  &getNodes();
        virtual  SP_ConstraintIter         &getSPs();
        virtual  MP_ConstraintIter         &getMPs();
        virtual  LoadPatternIter           &getLoadPatterns();
        virtual  SP_ConstraintIter         &getDomainAndLoadPatternSPs();
        virtual  UniaxialMaterialIter      &getUniaxialMaterials();
        virtual  NDMaterialIter            &getNDMaterials();
        virtual  NDMaterialLTIter          &getNDMaterialLTs();
        virtual  Element                   *getElement(int tag);
        virtual  Node                      *getNode(int tag);
        virtual  UniaxialMaterial          *getUniaxialMaterial(int tag);
        virtual  NDMaterial                *getNDMaterial(int tag);
        virtual  NDMaterialLT              *getNDMaterialLT(int tag);
        virtual  SectionForceDeformation   *getSection(int tag);
        virtual  SectionRepres             *getSectionRepres(int tag);
        virtual  MultiSupportPattern       *getMultipleSupport(int tag);
        virtual  AccelerationField         *getAccelerationField(int tag);
        virtual  Damping                   *getDamping(int tag);
        virtual  SP_Constraint             *getSP_Constraint(int tag);
        virtual  MP_Constraint             *getMP_Constraint(int tag);
        virtual  LoadPattern               *getLoadPattern(int tag);
        virtual double                      getCurrentTime(void) const;
        virtual int                         getCommitTag(void) const;
        virtual int                         getNumElements(void) const;
        virtual int                         getNumNodes(void) const;
        virtual int                         getNumSPs(void) const;
        virtual int                         getNumMPs(void) const;
        virtual int                         getNumLoadPatterns(void) const;
        virtual const Vector               &getPhysicalBounds(void);
        virtual  Graph                     *getElementGraph(void);
        virtual  Graph                     &getNodeGraph(void);

        // methods to update the domain
        virtual  void setCommitTag(int newTag);
        virtual  void setCurrentTime(double newTime);
        virtual  void setCommittedTime(double newTime);
        virtual  void applyLoad(double pseudoTime);
        virtual  void setLoadConstant(void);
        virtual  int  initialize(void);

        virtual  int  setDampingFactorsforElement(int ElementTag, int DampingTag);
        virtual  int  setDampingFactorsforNode(int NodeTag, int DampingTag);

        virtual  int  eigenAnalysis(int numMode);

        virtual  int  CheckMesh(const char *);

        virtual  int  commit(void);
        virtual  int  revertToLastCommit(void);
        virtual  int  revertToStart(void);
        virtual  int  update(void);
        virtual  int  update(double newTime, double dT);
        virtual  int  newStep(double dT);

        virtual int setEigenvalues(const Vector &theEigenvalues);
        virtual const Vector &getEigenvalues(void);
        virtual double getTimeEigenvaluesSet(void);


        virtual void domainChange(void);
        virtual int hasDomainChanged(void);
        virtual void setDomainChangeStamp(int newStamp);


        virtual void Print(ostream &s, int flag = 0);
        friend ostream &operator<<(ostream &s, Domain &M);

        virtual int sendSelf(int commitTag, Channel &theChannel);
        virtual int receiveSelf(int commitTag, Channel &theChannel,
                                FEM_ObjectBroker &theBroker);


        virtual int setHDF5_Channel(std::string filename_in,
                                    std::string model_name_in,
                                    std::string stage_name_in,
                                    int nsteps);

        virtual int setOutputWriter(std::string filename_in,
                                    std::string model_name_in,
                                    std::string stage_name_in,
                                    int nsteps);

        virtual int setNumberOfOutputSteps(int nsteps);
        virtual int setOutputEveryNsteps(int output_every_nsteps);

        virtual int enableOutput(bool is_output_enabled);
        virtual int enableElementOutput(bool is_element_output_enabled);


        virtual int getMaxElementsTag();
        virtual int getMaxNodesTag();
        virtual int getMaxUniaxialMaterialsTag();
        virtual int getMaxNDMaterialsTag();
        virtual int getMaxNDMaterialLTsTag();
        virtual int getMaxSectionsTag();
        virtual int getMaxSectionRepresentsTag();
        virtual int getMaxMultipleSupportsTag();
        virtual int getMaxAccelerationFieldsTag();
        virtual int getMaxDampingsTag();
        virtual int getMaxSPsTag();
        virtual int getMaxMPsTag();
        virtual int getMaxLoadPatternsTag();


        virtual bool recvNode(Node *theNod);
        virtual int  resetMultipleSupport(const int loadPatternTag)
        {
            return 0;
        };
        virtual bool hasInternalNode(int);
        virtual Node *getOutsideNode(int tag)
        {
            return 0;
        };


        virtual int calculateNodalReactions(int flag);

        HDF5_Channel theHDF5_Channel;

    protected:
        virtual int buildEleGraph(Graph *theEleGraph);
        virtual int buildNodeGraph(Graph *theNodeGraph);

        H5OutputWriter theOutputWriter;

        bool output_is_enabled;
        bool element_output_is_enabled;
        bool have_written_static_mesh_data;
        int  output_every_nsteps;
        int  countdown_til_output;

        double currentTime;               // current pseudo time

    private:
        double committedTime;             // the committed pseudo time
        double dT;                        // difference between committed and current time
        int    currentGeoTag;             // an integer used to mark if domain has changed
        bool   hasDomainChangedFlag;      // a bool flag used to indicate if GeoTag needs to be ++

        bool eleGraphBuiltFlag;
        bool nodeGraphBuiltFlag;

        Graph *theNodeGraph;
        Graph *theElementGraph;

        TaggedObjectStorage  *theElements;
        TaggedObjectStorage  *theNodes;
        TaggedObjectStorage *theUniaxialMaterials;
        TaggedObjectStorage *theNDMaterials;
        TaggedObjectStorage *theNDMaterialLTs;
        TaggedObjectStorage *theSections;
        TaggedObjectStorage *theSectionRepresents;
        TaggedObjectStorage *theMultipleSupports;
        TaggedObjectStorage *theAccelerationFields;
        TaggedObjectStorage *theDampings;
        TaggedObjectStorage  *theSPs;
        TaggedObjectStorage  *theMPs;
        TaggedObjectStorage  *theLoadPatterns;

        SingleDomEleIter      *theEleIter;
        SingleDomNodIter      *theNodIter;
        SingleDomSP_Iter      *theSP_Iter;
        SingleDomMP_Iter      *theMP_Iter;
        LoadPatternIter       *theLoadPatternIter;
        SingleDomAllSP_Iter   *allSP_Iter;
        SingleDomUniaxialMaterialIter  *theUniMaterialIter;
        SingleDomNDMaterialIter        *theNDMaterialIter;
        SingleDomNDMaterialLTIter        *theNDMaterialLTIter;


        int commitTag;

        Vector theBounds;
        Vector *theEigenvalues;
        double theEigenvalueSetTime;

        int lastChannel;

    public:
        int maxElementsTag;
        int maxNodesTag;
        int maxUniaxialMaterialsTag;
        int maxNDMaterialsTag;
        int maxNDMaterialLTsTag;
        int maxSectionsTag;
        int maxSectionRepresentsTag;
        int maxMultipleSupportsTag;
        int maxAccelerationFieldsTag;
        int maxDampingsTag;
        int maxSPsTag;
        int maxMPsTag;
        int maxLoadPatternsTag;

};

#endif


