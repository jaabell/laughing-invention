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

// $Revision: 1.3 $
// $Date: 2003/02/14 23:00:56 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/domain/partitioned/PartitionedDomain.h,v $


// Written: fmk
// Created: Wed Sep 25 15:27:47: 1996
// Revision: A
//
// Description: This file contains the class definition for PartitionedDomain.
// PartitionedDomain is an abstract class. The class is responsible for holding
// and providing access to the Elements, Nodes, SP_Constraints
// and MP_Constraints just like a normal domain. In addition the domain provides
// a method to partition the domain into Subdomains.
//
// ModelBuilder. There are no partitions in a PartitionedDomain.
//
// What: "@(#) PartitionedDomain.h, revA"

#ifndef PartitionedDomain_h
#define PartitionedDomain_h

#include <Domain.h>

class DomainPartitioner;
class Subdomain;
class SubdomainIter;
class  ArrayOfTaggedObjects;
class  PartitionedDomainSubIter;
class  PartitionedDomainEleIter;
class SingleDomEleIter;

class PartitionedDomain: public Domain
{
public:
    PartitionedDomain();
    PartitionedDomain(DomainPartitioner &thePartitioner);

    PartitionedDomain(int numNodes, int numElements,
                      int numSPs, int numMPs, int numLoadPatterns,
                      int numUniaxialMat, int numNDMaterial, int numNDMaterialLT, int numSections, //Added by Babak on December 2012
                      int numofSectionRepresents, int num3dGeomTransfs,//Added by Babak on December 2012
                      int nummultipleexcitation, int numBodyForces, int numDamping,//Added by Babak on December 2012
                      int numSubdomains,
                      DomainPartitioner &thePartitioner);

    virtual  ~PartitionedDomain();

    // public methods to populate a domain
    virtual  bool addElement(Element *elePtr);
    virtual  bool addNode(Node *nodePtr);

    virtual  bool addLoadPattern(LoadPattern *);
    virtual  bool addSP_Constraint(SP_Constraint *);
    virtual  bool addMP_Constraint(MP_Constraint *); //Guanzhou added
    virtual  bool addSP_Constraint(SP_Constraint *, int loadPatternTag);
    virtual  bool addNodalLoad(NodalLoad *, int loadPatternTag);
    virtual  bool addElementalLoad(ElementalLoad *, int loadPatternTag);

    // methods to remove the components
    virtual void clearAll(void);
    virtual Element *removeElement(int tag);
    virtual Node *removeNode(int tag);
    virtual SP_Constraint *removeSP_Constraint(int tag);
    virtual MP_Constraint *removeMP_Constraint(int tag);
    virtual LoadPattern *removeLoadPattern(int tag);

    // methods to access the elements
    virtual  ElementIter       &getElements();
    virtual  Element           *getElement(int tag);

    virtual  Node              *getNode(int tag);

    virtual  int        getNumElements(void) const;

    // public methods to update the domain
    virtual  void setCommitTag(int newTag);
    virtual  void setCurrentTime(double newTime);
    virtual  void setCommittedTime(double newTime);
    virtual  void applyLoad(double pseudoTime);
    virtual  void setLoadConstant(void);

    virtual  int commit(void);
    virtual  int output_step(void);                     // Added by Sumeet September, 2016 
    virtual  int output_iteration( int );               // Added by Sumeet 3rd August, 2016 for substep output
    virtual  int revertToLastCommit(void);
    virtual  int revertToStart(void);
    virtual  int update(void);
    virtual  int update(double newTime, double dT);
    virtual  int newStep(double dT);


    // virtual int  addRecorder(Recorder& theRecorder);
    // virtual int  removeRecorders(void);

    virtual  void Print(ostream &s, int flag = 0);

    // public member functions in addition to the standard domain
    virtual int setPartitioner(DomainPartitioner *thePartitioner);
    virtual int partition(int numPartitions);

# ifdef _PDD
    virtual int repartition(int numPartitions);
# endif
#ifdef _PARALLEL_PROCESSING
    //Guanzhou added this func
    virtual int resetMultipleSupport(const int loadPatternTag);
#endif
    //Guanzhou added
    virtual bool addSubdomain(Subdomain *theSubdomain);
    virtual int getNumSubdomains(void);
    virtual Subdomain *getSubdomainPtr(int tag);
    virtual SubdomainIter &getSubdomains(void);
    virtual Node *removeExternalNode(int tag);
    virtual Graph &getSubdomainGraph(void);
    virtual Node *getOutsideNode(int tag);
    /*    virtual BodyForce  *removeBodyForce(int tag); //Added by Babak on October 2012 (it is Domain and should be implemented in all of inhereted classes)
        virtual Damping  *removeDamping(int tag); */ //Added by Babak on October 2012 (it is Domain and should be implemented in all of inhereted classes)
    virtual  int return_element_numberofNodes(int);

    // friend classes
    friend class PartitionedDomainEleIter;


    // For output control
    // virtual int setNumberOfOutputSteps(int nsteps);
    // virtual int setOutputEveryNsteps(int output_every_nsteps);

    // virtual int enableOutput(bool is_output_enabled);
    // virtual int enableElementOutput(bool is_element_output_enabled);


    virtual int sendOutputOptionsToSubdomains();
    virtual int setConstitutiveIntegrationMethod(int algorithm,
            double f_relative_tol, double stress_relative_tol, int n_max_iterations);

protected:
    int barrierCheck(int result);
    DomainPartitioner *getPartitioner(void) const;
    virtual int buildEleGraph(Graph *theEleGraph);

private:
    TaggedObjectStorage  *elements;
    ArrayOfTaggedObjects *theSubdomains;
    DomainPartitioner    *theDomainPartitioner;

    SingleDomEleIter           *mainEleIter;  // for ele that belong to elements
    PartitionedDomainSubIter   *theSubdomainIter;
    PartitionedDomainEleIter   *theEleIter;

    Graph *mySubdomainGraph;    // a graph of subdomain connectivity
    int *Element_Node_Number_List; // store the number of each element's node added by babak at 2/25/13 Element_Node_Number_List(eleTag) = Number of nodes for the element  # EleTag-1
    int numberofDomainElements;   // store the number elements added by babak at 2/25/13

    bool have_populated_static_mesh_data;

};

#endif


