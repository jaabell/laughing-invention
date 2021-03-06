///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         .
// DATE:               .
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



// Original file, from OpenSees project, see ORIGINAL header below,
// was significantly changed, improved, massacred and massaged, so that
// this implementation/file now constitutes quite a novel development.
// In addition, ORIGINAL OpenSees license (see below) assigned
// (wrongly) copyright to the
// Regents of the University of California.
// ORIGINAL license also gave a number of permissions (see below)
// that we used.
//
//--------------------------------------------------------------------------
// Original OpenSees License:
// Copyright @ 1999,2000  The Regents of the University of California.
// All Rights Reserved.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for educational, research and non-profit purposes, without
// fee, and without a written agreement is hereby granted, provided that the
// above copyright notice, this paragraph and the following three paragraphs
// appear in all copies.
//
// Permission to incorporate this software into commercial products may be
// obtained by contacting the University of California.
// [Office of Technology Licensing,
// 2150 Shattuck Avenue #150
// Berkeley, CA 94720-1620,
// (510) 643-7201]
//
// This software program and documentation are copyrighted by The Regents of
// the University of California. The software program and documentation are
// supplied "as is", without any accompanying services from The Regents. The
// Regents does not warrant that the operation of the program will be
// uninterrupted or error-free. The end-user understands that the program was
// developed for research purposes and is advised not to rely exclusively on
// the program for any reason.
//
// IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
// DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
// LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
// DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE. THE UNIVERSITY OF CALIFORNIA SPECIFICALLY
// DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
// SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
// CALIFORNIA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
//--------------------------------------------------------------------------
//
//
// Original header for this file:





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

// $Revision: 1.26 $
// $Date: 2004/11/24 22:49:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/domain/Domain.cpp,v $


// FileP: ~/domain/domain/Domain.C
//
// Written: fmk
// Created: Fri Sep 20 15:27:47: 1996
// Revision: A
//
// Purpose: This file contains the class definition for Domain
// Domain is a container class. The class is responsible for holding
// and providing access to the Elements, Nodes, LoadCases, SP_Constraints
// and MP_Constraints. These objects are all added to the Domain by a
// ModelBuilder.
//
// What: "@(#) Domain.C, revA"

#include <stdlib.h>
#include <sstream>
#include <limits>

#include <OPS_Globals.h>
#include <Domain.h>

#include <ElementIter.h>
#include <NodeIter.h>
#include <ElementalLoadIter.h>
#include <NodalLoadIter.h>
#include <Element.h>
#include <Node.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <NodalLoad.h>
#include <ElementalLoad.h>
#include <LoadPattern.h>

// Nima Tafazzoli (Sep. 2012)
#include <UniaxialMaterialIter.h>
#include <NDMaterialIter.h>
#include <NDMaterialLTIter.h>
#include <SingleDomUniaxialMaterialIter.h>
#include <SingleDomNDMaterialIter.h>
#include <SingleDomNDMaterialLTIter.h>


#include <ArrayOfTaggedObjects.h>
#include <ArrayOfTaggedObjectsIter.h>
#include <MapOfTaggedObjects.h>
#include <MapOfTaggedObjectsIter.h>

#include <SingleDomEleIter.h>
#include <SingleDomNodIter.h>
#include <SingleDomSP_Iter.h>
#include <SingleDomMP_Iter.h>
#include <LoadPatternIter.h>
#include <SingleDomAllSP_Iter.h>

#include <Vertex.h>
#include <Graph.h>

#include <Channel.h>
// #include <HDF5_Channel.h>

#include <Analysis.h>
// #include <FE_Datastore.h>
#include <FEM_ObjectBroker.h>

#include <UniaxialMaterial.h>
#include <NDMaterial.h>
#include <NDMaterialLT.h>
#include <SectionForceDeformation.h>
#include <SectionRepres.h>

#include <MultiSupportPattern.h>
#include <classTags.h>

#include <AccelerationField.h>
#include <Damping.h>
#include <Rayleigh.h>

#include <fstream>
using std::ifstream;
using namespace std;


# include <ESSITimer.h>

# ifdef _PDD
# include<NanoTimer.h>
# include <VertexIter.h>
# endif

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>

Domain *ops_TheActiveDomain = 0;


Domain::Domain()
    :
    output_is_enabled(true), element_output_is_enabled(true), have_written_static_mesh_data(false),
    output_every_nsteps(1),
    currentTime( 0.0 ), committedTime( 0.0 ), dT( 0.0 ), currentGeoTag( 0 ),
    hasDomainChangedFlag( false ),
    eleGraphBuiltFlag( false ),  nodeGraphBuiltFlag( false ), theNodeGraph( 0 ),
    theElementGraph( 0 ),
    commitTag( 0 ),
    theBounds(6), theEigenvalues(0), theEigenvalueSetTime(0), numberOfDomainNodeDOFs(0),  numberOfDomainElementOutputs(0)
{

    // init the arrays for storing the domain components
    theElements = new ArrayOfTaggedObjects( 4096 );
    theNodes    = new ArrayOfTaggedObjects( 4096 );

    // *****************************************************************************************
    // Nima Tafazzoli (added on Sep. 2nd 2009)
    // Instead of Using TclModelBuilder

    // Nima Tafazzoli (Sep. 3rd 2009)
    // Adding Uniaxial Material and NdMatrerial separately since we have to send
    // the pointer of specific type of material to element

    theUniaxialMaterials = new ArrayOfTaggedObjects( 32 );
    theNDMaterials = new ArrayOfTaggedObjects( 32 );
    theNDMaterialLTs = new ArrayOfTaggedObjects( 32 );
    theSections  = new ArrayOfTaggedObjects( 32 );
    theSectionRepresents = new ArrayOfTaggedObjects( 32 );
    theMultipleSupports = new ArrayOfTaggedObjects( 256 );


    // Nima Tafazzoli (October 2010)
    // removing acceleration fields as an input argument for elements and add it to Domain
    theAccelerationFields = new ArrayOfTaggedObjects( 10 );
    //     BodyForce *thebodyforcezero = new BodyForce(0, 0, 0, 0);
    //     this->addBodyForce(thebodyforcezero);
    theDampings = new ArrayOfTaggedObjects( 10 );
    //     Damping *theDamping = 0;
    //     theDamping = new Rayleigh(0, 0, 0, "Initial_Stiffness");
    //     this->addDamping(theDamping);

    // *****************************************************************************************

    theSPs      = new ArrayOfTaggedObjects( 256 );
    theMPs      = new ArrayOfTaggedObjects( 256 );
    theLoadPatterns = new ArrayOfTaggedObjects( 32 );

    // init the iters
    theEleIter = new SingleDomEleIter( theElements );
    theNodIter = new SingleDomNodIter( theNodes );
    theSP_Iter = new SingleDomSP_Iter( theSPs );
    theMP_Iter = new SingleDomMP_Iter( theMPs );
    theLoadPatternIter = new LoadPatternIter( theLoadPatterns );
    allSP_Iter = new SingleDomAllSP_Iter( *this );


    // Nima Tafazzoli (Sep. 2012)
    theUniMaterialIter = new SingleDomUniaxialMaterialIter( theUniaxialMaterials );
    theNDMaterialIter  = new SingleDomNDMaterialIter( theNDMaterials );
    theNDMaterialLTIter  = new SingleDomNDMaterialLTIter( theNDMaterialLTs );

    // check that there was space to create the data structures
    if ( theElements == 0 || theNodes == 0 ||
            theSPs == 0 || theMPs == 0 ||
            theEleIter == 0 || theNodIter == 0 ||
            theMP_Iter == 0 || theSP_Iter == 0 ||
            theUniMaterialIter == 0 || theNDMaterialIter == 0 ||
            theNDMaterialLTIter == 0 ||
            theLoadPatterns == 0 || theLoadPatternIter == 0 )
    {

        cerr << "Domain::Domain() - out of memory\n";
        exit( -1 );
    }

    theBounds( 0 ) = std::numeric_limits<double>::infinity();
    theBounds( 1 ) = -std::numeric_limits<double>::infinity();
    theBounds( 2 ) = std::numeric_limits<double>::infinity();
    theBounds( 3 ) = -std::numeric_limits<double>::infinity();
    theBounds( 4 ) = std::numeric_limits<double>::infinity();
    theBounds( 5 ) = -std::numeric_limits<double>::infinity();

    maxElementsTag = 0;
    maxNodesTag = 0;
    maxUniaxialMaterialsTag = 0;
    maxNDMaterialsTag = 0;
    maxSectionsTag = 0;
    maxSectionRepresentsTag = 0;
    maxMultipleSupportsTag = 0;
    maxAccelerationFieldsTag = 0;
    maxDampingsTag = 0;
    maxSPsTag = 0;
    maxMPsTag = 0;
    maxLoadPatternsTag = 0;


    ELE_TAG_DESC_ARRAY;
    Element_Class_Desc.assign(ele_tag_desc_array,ele_tag_desc_array+ELE_TAG_DESC_ARRAY_SIZE);
    reaction_output_is_enabled = false;

}


Domain::Domain( int numNodes, int numElements, int numSPs, int numMPs,
                int numLoadPatterns, int numUniaxialMat, int numNDMaterial, int numNDMaterialLT, int numSections,
                int numofSectionRepresents,
                int nummultipleexcitation, int numAccelerationFields, int numDamping )
    : output_is_enabled(true), element_output_is_enabled(true), have_written_static_mesh_data(false),
      output_every_nsteps(1),
      currentTime( 0.0 ), committedTime( 0.0 ), dT( 0.0 ), currentGeoTag( 0 ),
      hasDomainChangedFlag( false ),
      eleGraphBuiltFlag( false ), nodeGraphBuiltFlag( false ), theNodeGraph( 0 ),
      theElementGraph( 0 ),
      commitTag( 0 ),
      theBounds( 6 ), theEigenvalues( 0 ), theEigenvalueSetTime( 0 ), numberOfDomainNodeDOFs(0),  numberOfDomainElementOutputs(0)
{
    // init the arrays for storing the domain components
    theElements = new ArrayOfTaggedObjects( numElements );
    theNodes    = new ArrayOfTaggedObjects( numNodes );

    // *****************************************************************************************
    // Nima Tafazzoli (added on Sep. 2nd 2009)
    // Instead of Using TclModelBuilder
    theUniaxialMaterials = new ArrayOfTaggedObjects( numUniaxialMat );
    theNDMaterials = new ArrayOfTaggedObjects( numNDMaterial );
    theNDMaterialLTs = new ArrayOfTaggedObjects( numNDMaterialLT );
    theSections  = new ArrayOfTaggedObjects( numSections );
    theSectionRepresents = new ArrayOfTaggedObjects( numofSectionRepresents );
    theMultipleSupports = new ArrayOfTaggedObjects( nummultipleexcitation );
    theAccelerationFields = new ArrayOfTaggedObjects( numAccelerationFields );
    AccelerationField *theaccelerationfieldzero = new AccelerationField( 0, 0, 0, 0 );
    this->addAccelerationField( theaccelerationfieldzero );
    theDampings = new ArrayOfTaggedObjects( numDamping );
    Damping *theDamping = 0;
    theDamping = new Rayleigh( 0, 0, 0, "Initial_Stiffness" );
    this->addDamping( theDamping );

    // *****************************************************************************************

    theSPs      = new ArrayOfTaggedObjects( numSPs );
    theMPs      = new ArrayOfTaggedObjects( numMPs );
    theLoadPatterns = new ArrayOfTaggedObjects( numLoadPatterns );

    // init the iters
    theEleIter = new SingleDomEleIter( theElements );
    theNodIter = new SingleDomNodIter( theNodes );
    theSP_Iter = new SingleDomSP_Iter( theSPs );
    theMP_Iter = new SingleDomMP_Iter( theMPs );
    theLoadPatternIter = new LoadPatternIter( theLoadPatterns );
    allSP_Iter = new SingleDomAllSP_Iter( *this );


    // Nima Tafazzoli (Sep. 2012)
    theUniMaterialIter = new SingleDomUniaxialMaterialIter( theUniaxialMaterials );
    theNDMaterialIter  = new SingleDomNDMaterialIter( theNDMaterials );
    theNDMaterialLTIter  = new SingleDomNDMaterialLTIter( theNDMaterialLTs );

    // check that there was space to create the data structures
    if ( theElements == 0 || theNodes == 0 ||
            theSPs == 0 || theMPs == 0 ||
            theEleIter == 0 || theNodIter == 0 ||
            theMP_Iter == 0 || theSP_Iter == 0 ||
            theUniMaterialIter == 0 || theNDMaterialIter == 0 || theNDMaterialLTIter == 0 ||
            theLoadPatterns == 0 || theLoadPatternIter == 0 )
    {

        cerr << ( "Domain::Domain(int, int, ...) - out of memory\n" );
    }

    theBounds( 0 ) = std::numeric_limits<double>::infinity();
    theBounds( 1 ) = -std::numeric_limits<double>::infinity();
    theBounds( 2 ) = std::numeric_limits<double>::infinity();
    theBounds( 3 ) = -std::numeric_limits<double>::infinity();
    theBounds( 4 ) = std::numeric_limits<double>::infinity();
    theBounds( 5 ) = -std::numeric_limits<double>::infinity();


    ELE_TAG_DESC_ARRAY;
    Element_Class_Desc.assign(ele_tag_desc_array,ele_tag_desc_array+ELE_TAG_DESC_ARRAY_SIZE);
    reaction_output_is_enabled = false;
}


// *****************************************************************************************
// Nima Tafazzoli (changed on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
Domain::Domain( TaggedObjectStorage &theNodesStorage,
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
                TaggedObjectStorage &theAccelerationFieldStorage,
                TaggedObjectStorage &theDampingStorage )
    : output_is_enabled(true), element_output_is_enabled(true), have_written_static_mesh_data(false),
      output_every_nsteps(1),
      currentTime( 0.0 ), committedTime( 0.0 ), dT( 0.0 ), currentGeoTag( 0 ),
      hasDomainChangedFlag( false ),
      eleGraphBuiltFlag( false ), nodeGraphBuiltFlag( false ), theNodeGraph( 0 ),
      theElementGraph( 0 ),
      theElements( &theElementsStorage ),
      theNodes( &theNodesStorage ),
      theUniaxialMaterials( &theUniaxialMaterialStorage ),
      theNDMaterials( &theNDMaterialStorage ),
      theNDMaterialLTs( &theNDMaterialLTStorage ),
      theSections( &theSectionStorage ),
      theSectionRepresents( &theSectionRepresentsStorage ),
      theMultipleSupports( &theMultiSupportStorage ),
      theAccelerationFields( &theAccelerationFieldStorage ),
      theDampings( &theDampingStorage ),
      theSPs( &theSPsStorage ),
      theMPs( &theMPsStorage ),
      theLoadPatterns( &theLoadPatternsStorage ),
      commitTag( 0 ),
      theBounds( 6 ), theEigenvalues( 0 ), theEigenvalueSetTime( 0 ), numberOfDomainNodeDOFs(0),  numberOfDomainElementOutputs(0)
{
    // init the iters
    theEleIter = new SingleDomEleIter( theElements );
    theNodIter = new SingleDomNodIter( theNodes );
    theSP_Iter = new SingleDomSP_Iter( theSPs );
    theMP_Iter = new SingleDomMP_Iter( theMPs );
    theLoadPatternIter = new LoadPatternIter( theLoadPatterns );
    allSP_Iter = new SingleDomAllSP_Iter( *this );


    // Nima Tafazzoli (Sep. 2012)
    theUniMaterialIter = new SingleDomUniaxialMaterialIter( theUniaxialMaterials );
    theNDMaterialIter  = new SingleDomNDMaterialIter( theNDMaterials );
    theNDMaterialLTIter  = new SingleDomNDMaterialLTIter( theNDMaterialLTs );

    // check that the containers are empty
    if ( theElements->getNumComponents() != 0 ||
            theNodes->getNumComponents() != 0 ||
            theSPs->getNumComponents() != 0 ||
            theMPs->getNumComponents() != 0 ||
            theLoadPatterns->getNumComponents() != 0 )
    {

        cerr << ( "Domain::Domain(&, & ...) - out of memory\n" );
    }

    // check that there was space to create the data structures
    if ( theElements == 0 || theNodes == 0 ||
            theSPs == 0 || theMPs == 0 ||
            theEleIter == 0 || theNodIter == 0 ||
            theMP_Iter == 0 || theSP_Iter == 0 ||
            theUniMaterialIter == 0 || theNDMaterialIter == 0 || theNDMaterialLTIter == 0 ||
            theLoadPatterns == 0 || theLoadPatternIter == 0 )
    {

        cerr << "FATAL Domain::Domain(TaggedObjectStorage, ...) - ";
        cerr << "Ran out of memory\n";
        exit( -1 );
    }

    theBounds( 0 ) = std::numeric_limits<double>::infinity();
    theBounds( 1 ) = -std::numeric_limits<double>::infinity();
    theBounds( 2 ) = std::numeric_limits<double>::infinity();
    theBounds( 3 ) = -std::numeric_limits<double>::infinity();
    theBounds( 4 ) = std::numeric_limits<double>::infinity();
    theBounds( 5 ) = -std::numeric_limits<double>::infinity();


    ELE_TAG_DESC_ARRAY;
    Element_Class_Desc.assign(ele_tag_desc_array,ele_tag_desc_array+ELE_TAG_DESC_ARRAY_SIZE);
    reaction_output_is_enabled = false;
}



Domain::Domain( TaggedObjectStorage &theStorage )
    : output_is_enabled(true), element_output_is_enabled(true), have_written_static_mesh_data(false),
      output_every_nsteps(1),
      currentTime( 0.0 ), committedTime( 0.0 ), dT( 0.0 ), currentGeoTag( 0 ),
      hasDomainChangedFlag( false ),
      eleGraphBuiltFlag( false ), nodeGraphBuiltFlag( false ), theNodeGraph( 0 ),
      theElementGraph( 0 ),
      commitTag( 0 ),
      theBounds( 6 ), theEigenvalues( 0 ), theEigenvalueSetTime( 0 ), numberOfDomainNodeDOFs(0),   numberOfDomainElementOutputs(0)
{
    // init the arrays for storing the domain components
    theStorage.clearAll(); // clear the storage just in case populated
    theElements = &theStorage;
    theNodes    = theStorage.getEmptyCopy();

    // *****************************************************************************************
    // Nima Tafazzoli (added on Sep. 2nd 2009)
    // Instead of Using TclModelBuilder
    theUniaxialMaterials = theStorage.getEmptyCopy();
    theNDMaterials = theStorage.getEmptyCopy();
    theNDMaterialLTs = theStorage.getEmptyCopy();
    theSections = theStorage.getEmptyCopy();
    theSectionRepresents = theStorage.getEmptyCopy();
    theMultipleSupports = theStorage.getEmptyCopy();
    theAccelerationFields = theStorage.getEmptyCopy();
    theDampings = theStorage.getEmptyCopy();

    // *****************************************************************************************


    theSPs      = theStorage.getEmptyCopy();
    theMPs      = theStorage.getEmptyCopy();
    theLoadPatterns = theStorage.getEmptyCopy();
    theSectionRepresents = theStorage.getEmptyCopy();

    // init the iters
    theEleIter = new SingleDomEleIter( theElements );
    theNodIter = new SingleDomNodIter( theNodes );
    theSP_Iter = new SingleDomSP_Iter( theSPs );
    theMP_Iter = new SingleDomMP_Iter( theMPs );
    theLoadPatternIter = new LoadPatternIter( theLoadPatterns );
    allSP_Iter = new SingleDomAllSP_Iter( *this );


    // Nima Tafazzoli (Sep. 2012)
    theUniMaterialIter = new SingleDomUniaxialMaterialIter( theUniaxialMaterials );
    theNDMaterialIter  = new SingleDomNDMaterialIter( theNDMaterials );
    theNDMaterialLTIter  = new SingleDomNDMaterialLTIter( theNDMaterialLTs );

    // check that there was space to create the data structures
    if ( theElements == 0 || theNodes == 0 ||
            theSPs == 0 || theMPs == 0 ||
            theEleIter == 0 || theNodIter == 0 ||
            theMP_Iter == 0 || theSP_Iter == 0 ||
            theUniMaterialIter == 0 || theNDMaterialIter == 0 || theNDMaterialLTIter == 0 ||
            theLoadPatterns == 0 || theLoadPatternIter == 0 )
    {

        cerr << ( "Domain::Domain(ObjectStorage &) - out of memory\n" );
    }

    theBounds( 0 ) = std::numeric_limits<double>::infinity();
    theBounds( 1 ) = -std::numeric_limits<double>::infinity();
    theBounds( 2 ) = std::numeric_limits<double>::infinity();
    theBounds( 3 ) = -std::numeric_limits<double>::infinity();
    theBounds( 4 ) = std::numeric_limits<double>::infinity();
    theBounds( 5 ) = -std::numeric_limits<double>::infinity();


    ELE_TAG_DESC_ARRAY;
    Element_Class_Desc.assign(ele_tag_desc_array,ele_tag_desc_array+ELE_TAG_DESC_ARRAY_SIZE);
    reaction_output_is_enabled = false;
}



// ~Domain();
//  destructor, this calls delete on all components of the model,
//  i.e. calls delete on all that is added to the model.
//  WARNING: if 3rd constructor, TaggedObjectStorage objects passed
//      must have been created with new and nowhere else must the
//      destructor be called.

Domain::~Domain()
{

    this->Domain::clearAll();

    // delete all the storage objects
    // SEGMENT FAULT WILL OCCUR IF THESE OBJECTS WERE NOT CONSTRUCTED
    // USING NEW
    if ( theElements != 0 )
    {
        delete theElements;
    }

    // *****************************************************************************************
    // Nima Tafazzoli (added on Sep. 2nd 2009)
    // Instead of Using TclModelBuilder

    if ( theUniaxialMaterials != 0 )
    {
        delete theUniaxialMaterials;
    }

    if ( theNDMaterials != 0 )
    {
        delete theNDMaterials;
    }

    if ( theNDMaterialLTs != 0 )
    {
        delete theNDMaterialLTs;
    }

    if ( theSections != 0 )
    {
        delete theSections;
    }

    if ( theSectionRepresents != 0 )
    {
        delete theSectionRepresents;
    }

    // *****************************************************************************************


    if ( theNodes != 0 )
    {
        delete theNodes;
    }


    if ( theSPs != 0 )
    {
        delete theSPs;
    }

    if ( theMPs != 0 )
    {
        delete theMPs;
    }

    if ( theLoadPatterns != 0 )
    {
        delete theLoadPatterns;
    }

    if ( theEleIter != 0 )
    {
        delete theEleIter;
    }

    if ( theNodIter != 0 )
    {
        delete theNodIter;
    }

    if ( theSP_Iter != 0 )
    {
        delete theSP_Iter;
    }

    if ( theMP_Iter != 0 )
    {
        delete theMP_Iter;
    }

    if ( allSP_Iter != 0 )
    {
        delete allSP_Iter;
    }

    if ( theLoadPatternIter != 0 )
    {
        delete theLoadPatternIter;
    }

    if ( theEigenvalues != 0 )
    {
        delete theEigenvalues;
    }


    // Nima Tafazzoli (Sep. 2012)
    if ( theUniMaterialIter != 0 )
    {
        delete theUniMaterialIter;
    }

    if ( theNDMaterialIter != 0 )
    {
        delete theNDMaterialIter;
    }

    if ( theNDMaterialLTIter != 0 )
    {
        delete theNDMaterialLTIter;
    }


    // for ( int i = 0; i < numRecorders; i++ )
    // {
    //     delete theRecorders[i];
    // }

    // if ( theRecorders != 0 )
    // {
    //     delete [] theRecorders;
    //     theRecorders = 0;
    // }


    // Nima Tafazzoli (Nov. 2012)
    // for ( int i = 0; i < numDatabases; i++ )
    // {
    //     delete theDatabases[i];
    // }

    // if ( theDatabases != 0 )
    // {
    //     delete [] theDatabases;
    //     theDatabases = 0;
    // }


    //     for (i=0; i<numRegions; i++)
    //       delete theRegions[i];

    //     if (theRegions != 0) {
    //       delete [] theRegions;
    //       theRegions = 0;
    //     }

    if ( theNodeGraph != 0 )
    {
        delete theNodeGraph;
    }

    if ( theElementGraph != 0 )
    {
        delete theElementGraph;
    }

    if ( theMultipleSupports != 0 )
    {
        delete theMultipleSupports;
    }

    if ( theAccelerationFields != 0 )
    {
        delete theAccelerationFields;
    }

    if ( theDampings != 0 )
    {
        delete theDampings;
    }

    // theRecorders = 0;
    // numRecorders = 0;

    // theDatabases = 0;
    // numDatabases = 0;

    // number_of_8GP_brick_elements = 0;
    // number_of_27GP_brick_elements = 0;
    // number_of_line_elements = 0;

}


// void addElement(Element *);
//  Method to add an element to the model.


bool
Domain::addElement( Element *element )
{
    int eleTag = element->getTag();

#ifdef _G3DEBUG
    // check all the elements nodes exist in the domain
    const ID &nodes = element->getExternalNodes();
    int numDOF = 0;

    for ( int i = 0; i < nodes.Size(); i++ )
    {
        int nodeTag = nodes( i );
        Node *nodePtr = this->getNode( nodeTag );

        if ( nodePtr == 0 )
        {
            cerr << "WARNING Domain::addElement - In element ";
            cerr << "\n no Node " << nodeTag << " exists in the domain\n";
            return false;
        }

        numDOF += nodePtr->getNumberDOF();
    }

#endif

    // check if an Element with a similar tag already exists in the Domain

    TaggedObject *other = theElements->getComponentPtr( eleTag );

    if ( other != 0 )
    {
        cerr << "Domain::addElement - element with tag " << eleTag << "already exists in model\n";
        return false;
    }

    // add the element to the container object for the elements
    bool result = theElements->addComponent( element );

    if ( result == true )
    {
        element->setDomain( this );
        //element->update(); // Jose and Sumeet: Thinks its not requird, needs to be taken care by element itself?

        // finally check the ele has correct number of dof
#ifdef _G3DEBUG

        if ( numDOF != element->getNumDOF() )
        {

            cerr << "Domain::addElement - element " << eleTag << " - #DOF does not match with number at nodes\n";
            theElements->removeComponent( eleTag );
            return false;
        }

#endif

        // mark the Domain as having been changed
        this->domainChange();

        if (eleTag > maxElementsTag)
        {
            maxElementsTag = eleTag;
        }

        //Sumeet Auguts, 2016
        // look at classTags.h for the class_description encoding and how the modulus 
        // with 1000 gets back the no. of element outputs and similarly gauss point
        // and number of connectivitynodes..
        int class_dec = Element_Class_Desc[element->getClassTag()];
        numberOfDomainElementOutputs += class_dec%100;
        Number_of_Connectivity_Nodes += (class_dec/1000000)%100;
        Number_of_Gauss_Points       += (class_dec%100000)/100;
    }
    else
    {
        cerr << "Domain::addElement - element " << eleTag << "could not be added to container\n";
    }

    return result;
}




bool
Domain::addNode( Node *node )
{
    int nodTag = node->getTag();

    TaggedObject *other = theNodes->getComponentPtr( nodTag );

    if ( other != 0 )
    {
        cerr << "Domain::addNode - node with tag " << nodTag << "already exists in model\n";
        return false;
    }

    bool result = theNodes->addComponent( node );

    numberOfDomainNodeDOFs += node->getNumberDOF();

    if ( result == true )
    {
        node->setDomain( this );
        this->domainChange();

        if ( nodTag > maxNodesTag)
        {
            maxNodesTag = nodTag;
        }

        // see if the physical bounds are changed
        // note this assumes 0,0,0,0,0,0 as startup min,max values
        const Vector &crds = node->getCrds();
        int dim = crds.Size();

        if ( dim == 3 )
        {
            double x = crds( 0 );
            double y = crds( 1 );
            double z = crds( 2 );

            if ( x < theBounds( 0 ) )
            {
                theBounds( 2 ) = x;
            }

            if ( x > theBounds( 3 ) )
            {
                theBounds( 5 ) = x;
            }

            if ( y < theBounds( 1 ) )
            {
                theBounds( 2 ) = y;
            }

            if ( y > theBounds( 4 ) )
            {
                theBounds( 5 ) = y;
            }

            if ( z < theBounds( 2 ) )
            {
                theBounds( 2 ) = z;
            }

            if ( z > theBounds( 5 ) )
            {
                theBounds( 5 ) = z;
            }
        }

    }
    else
    {
        cerr << "Domain::addNode - node with tag " << nodTag << "could not be added to container\n";
    }

    return result;
}

// *****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
int
Domain::addUniaxialMaterial( UniaxialMaterial &theMaterial )
{
    int materialTag = theMaterial.getTag();

    // check if a Material with a similar tag already exists in the Domain
    TaggedObject *other = theUniaxialMaterials->getComponentPtr( materialTag );

    if ( other != 0 )
    {
        cerr << "Domain::addUniaxialMaterial - material with tag " << materialTag << "already exists in model\n";
        return false;
    }

    // add the element to the container object for the elements
    bool result = theUniaxialMaterials->addComponent( &theMaterial );

    if ( result == true )
    {
        if ( materialTag > maxUniaxialMaterialsTag)
        {
            maxUniaxialMaterialsTag = materialTag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::addUniaxialMaterial - material with tag " << materialTag << "could not be added to container\n";
        return -1;
    }

}
// *****************************************************************************************


// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
int
Domain::addNDMaterial( NDMaterial &theMaterial )
{
    int materialTag = theMaterial.getTag();

    // check if a Material with a similar tag already exists in the Domain
    TaggedObject *other = theNDMaterials->getComponentPtr( materialTag );

    if ( other != 0 )
    {
        cerr << "Domain::NDMaterial - material with tag " << materialTag << "already exists in model\n";
        return false;
    }

    // add the element to the container object for the elements
    bool result = theNDMaterials->addComponent( &theMaterial );

    if ( result == true )
    {
        if (materialTag > maxNDMaterialsTag)
        {
            maxNDMaterialsTag = materialTag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::NDMaterial - material with tag " << materialTag << "could not be added to container\n";
        return -1;
    }

}

// *****************************************************************************************


// Jose Abell (added on Nov. 13 2013)
int
Domain::addNDMaterialLT( NDMaterialLT &theMaterial )
{
    
    int materialTag = theMaterial.getTag();

    // check if a Material with a similar tag already exists in the Domain
    TaggedObject *other = theNDMaterialLTs->getComponentPtr( materialTag );

    if ( other != 0 )
    {
        cerr << "Domain::NDMaterialLT - material with tag " << materialTag << "already exists in model\n";
        return false;
    }

    // add the element to the container object for the elements
    bool result = theNDMaterialLTs->addComponent( &theMaterial );

    if ( result == true )
    {
        if (materialTag > maxNDMaterialLTsTag)
        {
            maxNDMaterialLTsTag = materialTag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::NDMaterialLT - material with tag " << materialTag << "could not be added to container\n";
        return -1;  // end Jose Abell addition
    }// end Jose Abell addition



}// end Jose Abell addition

// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
int
Domain::addMultipleSupport( MultiSupportPattern &theMultiSupportPattern )
{

    bool result = theMultipleSupports->addComponent( &theMultiSupportPattern );

    if ( result == true )
    {
        int tag = theMultiSupportPattern.getTag();
        if (tag > maxMultipleSupportsTag)
        {
            maxMultipleSupportsTag = tag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::MultiSupportPattern - failed to add MultiSupportPattern:\n ";
        return -1;
    }
}


// *****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
int
Domain::addSection( SectionForceDeformation &theSection )
{
    bool result = theSections->addComponent( &theSection );

    if ( result == true )
    {
        int tag = theSection.getTag();
        if (tag > maxSectionsTag)
        {
            maxSectionsTag = tag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::addSection - failed to add section:\n ";
        return -1;
    }
}
// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
int
Domain::addSectionRepres( SectionRepres &theSectionRepres )
{
    bool result = theSectionRepresents->addComponent( &theSectionRepres );


    if ( result == true )
    {
        int tag = theSectionRepres.getTag();
        if (tag > maxSectionRepresentsTag)
        {
            maxSectionRepresentsTag = tag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::addSectionRepres - failed to add SectionRepres\n";
        return -1;
    }
} //

// *****************************************************************************************




// Nima Tafazzoli (Oct. 2010)
int
Domain::addAccelerationField( AccelerationField *theAccelerationField )
{
    bool result = theAccelerationFields->addComponent( theAccelerationField );

    if ( result == true )
    {
        int tag = theAccelerationField->getTag();
        if (tag > maxAccelerationFieldsTag)
        {
            maxAccelerationFieldsTag = tag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::addAccelerationFields - failed to add theAccelerationField \n ";
        return -1; //
    } //
}// End nima add

// *****************************************************************************************
// Nima Tafazzoli (Oct. 2010)
int
Domain::addDamping( Damping *theDamping )
{
    bool result = theDampings->addComponent( theDamping );

    if ( result == true )
    {
        int tag = theDamping->getTag();
        if (tag > maxDampingsTag)
        {
            maxDampingsTag = tag;
        }
        return 0;
    }
    else
    {
        cerr << "Domain::addDamping - failed to add theDamping \n ";
        return -1;
    }
}

// *****************************************************************************************


// void addSP_Constraint(SP_Constraint *);
//  Method to add a constraint to the model.
//

bool
Domain::addSP_Constraint( SP_Constraint *spConstraint )
{
    // check that no other object with similar tag exists in model
    int tag = spConstraint->getTag();
    TaggedObject *other = theSPs->getComponentPtr( tag );

    if ( other != 0 )
    {
        return false;
    }

    bool result = theSPs->addComponent( spConstraint );

    if ( result == false )
    {
        cerr << "Domain::addSP_Constraint - cannot add constraint with tag" <<
             tag << "to the container\n";
        return false;
    }

    spConstraint->setDomain( this );
    this->domainChange();

    if (tag > maxSPsTag)
    {
        maxSPsTag = tag;
    }


    return true;
}


// void addMP_Constraint(MP_Constraint *);
//  Method to add a constraint to the model.
//

bool
Domain::addMP_Constraint( MP_Constraint *mpConstraint )
{
#ifdef _G3DEBUG
    // perform the checks
    int nodeConstrained = mpConstraint->getNodeConstrained();
    Node *nodePtr = this->getNode( nodeConstrained );

    if ( nodePtr == 0 )
    {
        cerr << "Domain::addMP_Constraint -cannot add as constrained node with tag" <<
             nodeConstrained << "does not exist in model\n";
        return false;
    }

    int nodeRetained = mpConstraint->getNodeRetained();
    nodePtr = this->getNode( nodeRetained );

    if ( nodePtr == 0 )
    {
        cerr << "Domain::addMP_Constraint - cannot add as retained node with tag" <<
             nodeRetained << "does not exist in model\n";

        return false;
    }



    // MISSING CODE
#endif

    // check that no other object with similar tag exists in model
    int tag = mpConstraint->getTag();
    TaggedObject *other = theMPs->getComponentPtr( tag );

    if ( other != 0 )
    {
        cerr << "Domain::addMP_Constraint - cannot add as constraint with tag" <<
             tag << "already exists in model";

        return false;
    }

    bool result = theMPs->addComponent( mpConstraint );

    if ( result == true )
    {
        mpConstraint->setDomain( this );
        this->domainChange();
        if (tag > maxMPsTag)
        {
            maxMPsTag = tag;
        }
    }
    else{
        cerr << "Domain::addMP_Constraint - cannot add constraint with tag" <<
             tag << "to the container\n";
    }

    return result;
}

bool
Domain::addLoadPattern( LoadPattern *load )
{
    // first check if a load pattern with a similar tag exists in model
    int tag = load->getTag();
    TaggedObject *other = theLoadPatterns->getComponentPtr( tag );

    if ( other != 0 )
    {
        cerr << "Domain::addLoadPattern - cannot add as LoadPattern with tag" <<
             tag << "already exists in model\n";

        return false;
    }

    // now we add the load pattern to the container for load pattrens
    bool result = theLoadPatterns->addComponent( load );

    if ( result == true )
    {
        load->setDomain( this );
        this->domainChange();
        if (tag > maxLoadPatternsTag)
        {
            maxLoadPatternsTag = tag;
        }

    }
    else{
        cerr << "Domain::addLoadPattern - cannot add LoadPattern with tag" <<
             tag << "to the container\n";
    }

    return result;
}


bool
Domain::addSP_Constraint( SP_Constraint *spConstraint, int pattern )
{
#ifdef _G3DEBUG
    // check the Node exists in the Domain
    int nodeTag = spConstraint->getNodeTag();
    Node *nodePtr = this->getNode( nodeTag );

    if ( nodePtr == 0 )
    {
        cerr << "Domain::addSP_Constraint - cannot add as node with tag" <<
             nodeTag << "does not exist in model\n";
        return false;
    }

    // check that the DOF specified exists at the Node
    int numDOF = nodePtr->getNumberDOF();

    if ( numDOF < spConstraint->getDOF_Number() )
    {
        cerr << "Domain::addSP_Constraint - cannot add as node with tag" <<
             nodeTag << "does not have associated constrained DOF\n";

        return false;
    }

#endif

    // now add it to the pattern
    TaggedObject *thePattern = theLoadPatterns->getComponentPtr( pattern );

    if ( thePattern == 0 )
    {
        cerr << "Domain::addSP_Constraint - cannot add as pattern with tag" <<
             pattern << "does not exist in domain\n";

        return false;
    }

    LoadPattern *theLoadPattern = ( LoadPattern *)thePattern;
    bool result = theLoadPattern->addSP_Constraint( spConstraint );

    if ( result == false )
    {
        cerr << "Domain::addSP_Constraint - " << pattern << "pattern could not add the SP_Constraint\n";

        return false;
    }

    spConstraint->setDomain( this );
    this->domainChange();

    int tag = spConstraint->getTag();
    if (tag > maxSPsTag)
    {
        maxSPsTag = tag;
    }


    return true;
}

bool
Domain::addNodalLoad( NodalLoad *load, int pattern )
{
    int nodTag = load->getNodeTag();
    Node *res = this->getNode( nodTag );

    if ( res == 0 )
    {
        cerr << "Domain::addNodalLoad() HI - no node with tag " << nodTag <<
             "exits in  the model, not adding the nodal load"  << endl;
        //  "exits in  the model, not adding the nodal load"  << *load << endl;
        return false;
    }

    // now add it to the pattern
    TaggedObject *thePattern = theLoadPatterns->getComponentPtr( pattern );

    if ( thePattern == 0 )
    {
        cerr << "Domain::addNodalLoad() - no pattern with tag" <<
             pattern << "in  the model, not adding the nodal load" << endl;
        //  pattern << "in  the model, not adding the nodal load"  << *load << endl;

        return false;
    }

    LoadPattern *theLoadPattern = ( LoadPattern *)thePattern;
    bool result = theLoadPattern->addNodalLoad( load );

    if ( result == false )
    {
        cerr << "Domain::addNodalLoad() - pattern with tag" <<
             pattern << "could not add the load" << endl;
        //  pattern << "could not add the load" << *load << endl;

        return false;
    }

    load->setDomain( this );  // done in LoadPattern::addNodalLoad()
    this->domainChange();

    return result;
}


bool
Domain::addElementalLoad( ElementalLoad *load, int pattern )
{
    bool element_exists_in_this_domain = false;
    const ID &elements = load->getElementTags();
    for (int i = 0; i < elements.Size(); i++)
    {
        if ( this->getElement(elements(i)) != 0)
        {
            element_exists_in_this_domain = true;
            break;
        }
    }

    if (element_exists_in_this_domain)
    {
        // now add it to the pattern
        TaggedObject *thePattern = theLoadPatterns->getComponentPtr( pattern );

        if ( thePattern == 0 )
        {
            cerr << "Domain::addNodalLoad() - no pattern with tag " << pattern <<
                 "exits in  the model, not adding the ele load " << endl;
            //  "exits in  the model, not adding the ele load " << *load << endl;

            return false;
        }

        LoadPattern *theLoadPattern = ( LoadPattern *)thePattern;
        bool result = theLoadPattern->addElementalLoad( load );

        if ( result == false )
        {
            cerr << "Domain::addNodalLoad() - no pattern with tag" <<
                 pattern << "in  the model, not adding the ele load" << endl;
            //  pattern << "in  the model, not adding the ele load" << *load << endl;
            return false;
        }


        // load->setDomain(this); // done in LoadPattern::addElementalLoad()
        this->domainChange();
        return result;
    }
    else
    {
        return true;
    }
}


// GENERAL NOTE ON REMOVAL OF COMPONENTS:
//   downward casts (while bad) are o.k. as only the type
//   of components can be added to the storage objects, e.g.
//   only elements can be added to theElements therefore
//   casting a DomainComponent from theElements to an Element is o.k.
//

void
Domain::clearAll( void )
{
    // cout << "Domain::clearAll( void )" << endl;
    // clear the loads and constraints from any load pattern
    LoadPatternIter &thePatterns = this->getLoadPatterns();
    LoadPattern *thePattern;

    while ( ( thePattern = thePatterns() ) != 0 )
    {
        thePattern->clearAll();
    }

    // clean out the containers
    theElements->clearAll();
    theNodes->clearAll();

    // ***************************************************
    // Nima Tafazzoli (added on Sep. 2nd 2009)
    // Instead of Using TclModelBuilder
    theUniaxialMaterials->clearAll();
    theNDMaterials->clearAll();
    theNDMaterialLTs->clearAll();
    // ***************************************************
    // Nima Tafazzoli added July 2012
    theAccelerationFields->clearAll();
    theDampings->clearAll();
    // ***************************************************



    theSPs->clearAll();
    theMPs->clearAll();
    theLoadPatterns->clearAll();


    // set the time back to 0.0
    currentTime = 0.0;
    committedTime = 0.0;
    dT = 0.0;

    // reset the bounds
    theBounds( 0 ) = std::numeric_limits<double>::infinity();
    theBounds( 1 ) = -std::numeric_limits<double>::infinity();
    theBounds( 2 ) = std::numeric_limits<double>::infinity();
    theBounds( 3 ) = -std::numeric_limits<double>::infinity();
    theBounds( 4 ) = std::numeric_limits<double>::infinity();
    theBounds( 5 ) = -std::numeric_limits<double>::infinity();

    // currentGeoTag = 0;
    // lastGeoSendTag = -1;

    // rest the flag to be as initial
    hasDomainChangedFlag = false;
    nodeGraphBuiltFlag = false;
    eleGraphBuiltFlag = false;
    have_written_static_mesh_data = false;
    countdown_til_output = 0;

#ifndef _PARALLEL_PROCESSING //Done in PartitionedDomain::clearAll for parallel case
    theOutputWriter.finalize();
#endif
}


Element *
Domain::removeElement( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theElements->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    // perform a downward cast to an Element (safe as only Element added to
    // this container, 0 the Elements DomainPtr and return the result of the cast
    Element *result = ( Element *)mc;

    //Sumeet Auguts, 2016
    // look at classTags.h for the class_description encoding and how the modulus 
    // with 1000 gets back the no. of element outputs and similarly gauss point
    // and number of connectivitynodes..
    int class_dec = Element_Class_Desc[result->getClassTag()];
    numberOfDomainElementOutputs -= class_dec%100;
    Number_of_Connectivity_Nodes -= (class_dec/1000000)%100;
    Number_of_Gauss_Points       -= (class_dec%100000)/100;

    return result;
}

Node *
Domain::removeNode( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theNodes->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // mark the domain has having changed
    this->domainChange();

    // perform a downward cast to a Node (safe as only Node added to
    // this container and return the result of the cast
    Node *result = ( Node *)mc;

    numberOfDomainNodeDOFs -= result->getNumberDOF();

    // result->setDomain(0);
    return result;
}

// *****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
UniaxialMaterial *
Domain::removeUniaxialMaterial( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theUniaxialMaterials->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    // perform a downward cast to an Element (safe as only Element added to
    // this container, 0 the Elements DomainPtr and return the result of the cast
    UniaxialMaterial *result = ( UniaxialMaterial *)mc;
    //  result->setDomain(0);
    return result;
}

// *****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
NDMaterial *
Domain::removeNDMaterial( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theNDMaterials->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    NDMaterial *result = ( NDMaterial *)mc;
    //  result->setDomain(0);
    return result;
}
// *****************************************************************************************
// Jose Abell (Dec. 2014)
NDMaterialLT *
Domain::removeNDMaterialLT( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theNDMaterialLTs->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    NDMaterialLT *result = ( NDMaterialLT *)mc;
    //  result->setDomain(0);
    return result;
}
// *****************************************************************************************
// Nima Tafazzoli (Oct. 2010)
AccelerationField *
Domain::removeAccelerationField( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theAccelerationFields->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    AccelerationField *result = ( AccelerationField *)mc;
    //  result->setDomain(0);
    return result;
}
// *****************************************************************************************
// Nima Tafazzoli (Oct. 2010)
Damping *
Domain::removeDamping( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theDampings->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // otherwise mark the domain as having changed
    this->domainChange();

    Damping *result = ( Damping *)mc;
    //  result->setDomain(0);
    return result;
}
// *****************************************************************************************


SP_Constraint *
Domain::removeSP_Constraint( int theNode, int theDOF, int loadPatternTag )
{

    SP_Constraint *theSP = 0;
    bool found = false;
    int spTag = 0;

    if ( loadPatternTag == -1 )
    {
        SP_ConstraintIter &theSPs = this->getSPs();

        while ( ( found == false ) && ( ( theSP = theSPs() ) != 0 ) )
        {
            int nodeTag = theSP->getNodeTag();
            int dof = theSP->getDOF_Number();

            if ( nodeTag == theNode && dof == theDOF )
            {
                spTag = theSP->getTag();
                found = true;
            }
        }

        if ( found == true )
        {
            return this->removeSP_Constraint( spTag );
        }

    }
    else
    {
        LoadPattern *thePattern = this->getLoadPattern( loadPatternTag );

        if ( thePattern != 0 )
        {
            SP_ConstraintIter &theSPs = thePattern->getSPs();

            while ( ( found == false ) && ( ( theSP = theSPs() ) != 0 ) )
            {
                int nodeTag = theSP->getNodeTag();
                int dof = theSP->getDOF_Number();

                if ( nodeTag == theNode && dof == theDOF )
                {
                    spTag = theSP->getTag();
                    found = true;
                }
            }

            if ( found == true )
            {
                return thePattern->removeSP_Constraint( spTag );
            }

        }
    }

    return 0;
}




SP_Constraint *
Domain::removeSP_Constraint( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theSPs->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // mark the domain as having changed
    this->domainChange();

    // perform a downward cast, set the objects domain pointer to 0
    // and return the result of the cast
    SP_Constraint *result = ( SP_Constraint *)mc;
    // result->setDomain(0);

    // should check that theLoad and result are the same
    return result;
}

MP_Constraint *
Domain::removeMP_Constraint( int tag )
{
    // remove the object from the container
    TaggedObject *mc = theMPs->removeComponent( tag );

    // if not there return 0
    if ( mc == 0 )
    {
        return 0;
    }

    // mark the domain as having changed
    this->domainChange();

    // perform a downward cast, set the objects domain pointer to 0
    // and return the result of the cast
    MP_Constraint *result = ( MP_Constraint *)mc;
    // result->setDomain(0);
    return result;
}


// *****************************************************************************************
// Nima Tafazzoli (June. 2011)

int
Domain::removeMP_Constraints( int nodeTag )
{
    ID tagsToRemove( 0 );
    int sizeTags = 0;
    MP_Constraint *theMP = 0;
    MP_ConstraintIter &theMPIter = this->getMPs();

    while ( ( theMP = theMPIter() ) != 0 )
    {
        int cNode = theMP->getNodeConstrained();

        if ( cNode == nodeTag )
        {
            int mpTag = theMP->getTag();
            tagsToRemove[sizeTags] = mpTag;
            sizeTags++;
        }
    }

    if ( sizeTags == 0 )
    {
        return 0;
    }

    for ( int i = 0; i < sizeTags; i++ )
    {
        int  tag = tagsToRemove( i );
        TaggedObject *mc = theMPs->removeComponent( tag );

        if ( mc != 0 )
        {
            delete mc;
        }
    }

    // mark the domain as having changed
    this->domainChange();

    return 0;
}

// *****************************************************************************************


LoadPattern *
Domain::removeLoadPattern( int tag )
{
    // remove the object from the container
    TaggedObject *obj = theLoadPatterns->removeComponent( tag );

    // if not there return 0
    if ( obj == 0 )
    {
        cout << "Domain :: could not remove load pattern with tag " << tag << ".\n";
        return 0;
    }

    // perform a downward cast, set the objects domain pointer to 0
    // and return the result of the cast
    LoadPattern *result = ( LoadPattern *)obj;
    // result->setDomain(0);

    //
    // now set the Domain pointer for all loads and SP constraints
    // in the loadPattern to be 0
    //

    NodalLoad *theNodalLoad;
    NodalLoadIter &theNodalLoads = result->getNodalLoads();

    while ( ( theNodalLoad = theNodalLoads() ) != 0 )
    {
        // theNodalLoad->setDomain(0);
    }

    ElementalLoad *theElementalLoad;
    ElementalLoadIter &theElementalLoads = result->getElementalLoads();

    while ( ( theElementalLoad = theElementalLoads() ) != 0 )
    {
        // theElementalLoad->setDomain(0);
    }

    int numSPs = 0;
    SP_Constraint *theSP_Constraint;
    SP_ConstraintIter &theSP_Constraints = result->getSPs();

    while ( ( theSP_Constraint = theSP_Constraints() ) != 0 )
    {
        numSPs++;
        // theSP_Constraint->setDomain(0);
    }

    // mark the domain has having changed if numSPs > 0
    // as the constraint handlers have to be redone
    if ( numSPs > 0 )
    {
        this->domainChange();
    }

    // finally return the load pattern
    return result;
}





NodalLoad *
Domain::removeNodalLoad( int tag, int loadPattern )
{
    // remove the object from the container
    LoadPattern *theLoadPattern = this->getLoadPattern( loadPattern );

    // if not there return 0
    if ( theLoadPattern == 0 )
    {
        return 0;
    }

    return theLoadPattern->removeNodalLoad( tag );
}


ElementalLoad *
Domain::removeElementalLoad( int tag, int loadPattern )
{
    // remove the object from the container
    LoadPattern *theLoadPattern = this->getLoadPattern( loadPattern );

    // if not there return 0
    if ( theLoadPattern == 0 )
    {
        return 0;
    }

    return theLoadPattern->removeElementalLoad( tag );
}


SP_Constraint *
Domain::removeSP_Constraint( int tag, int loadPattern )
{
    // remove the object from the container
    LoadPattern *theLoadPattern = this->getLoadPattern( loadPattern );

    // if not there return 0
    if ( theLoadPattern == 0 )
    {
        return 0;
    }

    SP_Constraint *theSP = theLoadPattern->removeSP_Constraint( tag );

    if ( theSP != 0 )
    {
        this->domainChange();
    }

    return theSP;
}

ElementIter &
Domain::getElements()
{
    theEleIter->reset();
    return *theEleIter;
}


NodeIter &
Domain::getNodes()
{
    theNodIter->reset();
    return *theNodIter;
}

SP_ConstraintIter &
Domain::getSPs()
{
    theSP_Iter->reset();
    return *theSP_Iter;;
}

SP_ConstraintIter &
Domain::getDomainAndLoadPatternSPs()
{
    allSP_Iter->reset();
    return *allSP_Iter;;
}


MP_ConstraintIter &
Domain::getMPs()
{
    theMP_Iter->reset();
    return *theMP_Iter;;
}


LoadPatternIter &
Domain::getLoadPatterns()
{
    theLoadPatternIter->reset();
    return *theLoadPatternIter;;
}


// ***************************************************************************
// Nima Tafazzoli added for checking the mesh for materials (September 2012)
UniaxialMaterialIter &
Domain::getUniaxialMaterials()
{
    theUniMaterialIter->reset();
    return *theUniMaterialIter;
}


NDMaterialIter &
Domain::getNDMaterials()
{
    theNDMaterialIter->reset();
    return *theNDMaterialIter;
}

NDMaterialLTIter &
Domain::getNDMaterialLTs()
{
    theNDMaterialLTIter->reset();
    return *theNDMaterialLTIter;
}
// ***************************************************************************




// GENERAL NOTE ON RETRIEVAL OF COMPONENT PTRs:
//   downward casts (while bad) are o.k. as only the type
//   of components can be added to the storage objects, e.g.
//   only elements can be added to theElements
//

Element *
Domain::getElement( int tag )
{
    TaggedObject *mc = theElements->getComponentPtr( tag );

    // if not there return 0 otherwise perform a cast and return that
    if ( mc == 0 )
    {
        return 0;
    }

    Element *result = ( Element *)mc;
    return result;
}


Node *
Domain::getNode( int tag )
{
    TaggedObject *mc = theNodes->getComponentPtr( tag );

    // if not there return 0 otherwise perform a cast and return that
    if ( mc == 0 )
    {
        return 0;
    }

    Node *result = ( Node *)mc;
    return result;
}

// *****************************************************************************************
// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
UniaxialMaterial *
Domain::getUniaxialMaterial( int tag )
{
    TaggedObject *mc = theUniaxialMaterials->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    UniaxialMaterial *result = ( UniaxialMaterial *)mc;
    return result;

}
// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
NDMaterial *
Domain::getNDMaterial( int tag )
{
    TaggedObject *mc = theNDMaterials->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    NDMaterial *result = ( NDMaterial *)mc;
    return result;

}
// *****************************************************************************************

// Jose Abell (added on Nov. 13 2013)
NDMaterialLT *
Domain::getNDMaterialLT( int tag )
{
    TaggedObject *mc = theNDMaterialLTs->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    NDMaterialLT *result = ( NDMaterialLT *)mc;
    return result;

}
// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
MultiSupportPattern *
Domain::getMultipleSupport( int tag )
{
    TaggedObject *mc = theMultipleSupports->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    MultiSupportPattern *result = ( MultiSupportPattern *)mc;
    return result;

}
// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
SectionForceDeformation *
Domain::getSection( int tag )
{
    TaggedObject *mc = theSections->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // do a cast and return
    SectionForceDeformation *result = ( SectionForceDeformation *)mc;
    return result;
}
// *****************************************************************************************

// Nima Tafazzoli (added on Sep. 2nd 2009)
// Instead of Using TclModelBuilder
SectionRepres *
Domain::getSectionRepres( int tag )
{
    TaggedObject *mc = theSectionRepresents->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    SectionRepres *result = ( SectionRepres *)mc;
    return result;
}

// *****************************************************************************************
// Nima Tafazzoli (Oct. 2010)
AccelerationField *
Domain::getAccelerationField( int tag )
{
    TaggedObject *mc = theAccelerationFields->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    AccelerationField *result = ( AccelerationField *)mc;
    return result;

}
// *****************************************************************************************
// Nima Tafazzoli (Oct. 2010)
Damping *
Domain::getDamping( int tag )
{
    TaggedObject *mc = theDampings->getComponentPtr( tag );

    if ( mc == 0 )
    {
        return 0;
    }

    // otherweise we do a cast and return
    Damping *result = ( Damping *)mc;
    return result;

}
// *****************************************************************************************

SP_Constraint *
Domain::getSP_Constraint( int tag )
{
    TaggedObject *mc = theSPs->getComponentPtr( tag );

    // if not there return 0 otherwise perform a cast and return that
    if ( mc == 0 )
    {
        return 0;
    }

    SP_Constraint *result = ( SP_Constraint *)mc;
    return result;
}

MP_Constraint *
Domain::getMP_Constraint( int tag )
{
    TaggedObject *mc = theMPs->getComponentPtr( tag );

    // if not there return 0 otherwise perform a cast and return that
    if ( mc == 0 )
    {
        return 0;
    }

    MP_Constraint *result = ( MP_Constraint *)mc;
    return result;
}

LoadPattern *
Domain::getLoadPattern( int tag )
{
    TaggedObject *mc = theLoadPatterns->getComponentPtr( tag );

    // if not there return 0 otherwise perform a cast and return that
    if ( mc == 0 )
    {
        return 0;
    }

    LoadPattern *result = ( LoadPattern *)mc;
    return result;
}


double
Domain::getCurrentTime( void ) const
{
    return currentTime;
}

int
Domain::getCommitTag( void ) const
{
    return commitTag;
}


int
Domain::getNumElements( void ) const
{
    return theElements->getNumComponents();
}
int
Domain::getNumNodes( void ) const
{
    return theNodes->getNumComponents();
}

int
Domain::getNumSPs( void ) const
{
    return theSPs->getNumComponents();
}


int
Domain::getNumMPs( void ) const
{
    return theMPs->getNumComponents();
}

int
Domain::getNumLoadPatterns( void ) const
{
    return theLoadPatterns->getNumComponents();
}


const Vector &
Domain::getPhysicalBounds( void )
{
    return theBounds;
}

Graph *
Domain::getElementGraph( void )
{
    if ( eleGraphBuiltFlag == false )
    {
        // if the current graph is out of date .. delete it so we can start again
        if ( theElementGraph != 0 )
        {
            delete theElementGraph;
            theElementGraph = 0;
        }

        // create an empty graph
        theElementGraph = new Graph( this->getNumElements() + START_VERTEX_NUM );

        if ( theElementGraph == 0 ) // if still 0 try a smaller one
        {
            theElementGraph = new Graph();

            if ( theElementGraph == 0 ) // if still 0 out of memory
            {
                cerr << "Domain::getElementGraph() - out of memory\n";
                exit( -1 );
            }
        }

        // now build the graph
        if ( this->buildEleGraph( theElementGraph ) == 0 )
        {
            eleGraphBuiltFlag = true;
        }
        else
        {
            cerr << "Domain::getElementGraph() - failed to build the element graph\n";
        }
    }

    
    theOutputWriter.initializePartitionData(maxNodesTag, maxElementsTag);


    // return the Graph
    return theElementGraph;
}



Graph &
Domain::getNodeGraph( void )
{
    if ( nodeGraphBuiltFlag == false )
    {

        // if the current graph is out of date .. delete it so we can start again
        if ( theNodeGraph != 0 )
        {
            delete theNodeGraph;
            theNodeGraph = 0;
        }

        // try to get a graph as big as we should need
        theNodeGraph = new Graph( this->getNumNodes() + START_VERTEX_NUM );

        if ( theNodeGraph == 0 ) // if still 0 try a smaller one
        {
            theNodeGraph = new Graph();

            if ( theNodeGraph == 0 ) // if still 0 out of memory
            {
                cerr << "Domain::getNodeGraph() - out of memory\n";
                exit( -1 );
            }
        }

        // now build the graph
        if ( this->buildNodeGraph( theNodeGraph ) == 0 )
        {
            nodeGraphBuiltFlag = true;
        }
        else
        {
            cerr << "Domain::getNodeGraph() - failed to build the node graph\n";
        }
    }

    // return the Graph
    return *theNodeGraph;
}


void
Domain::setCommitTag( int newTag )
{
    commitTag = newTag;
}

void
Domain::setCurrentTime( double newTime )
{
    currentTime = newTime;
    dT = currentTime - committedTime;
}

void
Domain::setCommittedTime( double newTime )
{
    committedTime = newTime;
    dT = currentTime - committedTime;
}


void
Domain::applyLoad( double timeStep )
{
    // set the current pseudo time in the domain to be newTime
    currentTime = timeStep;
    dT = currentTime - committedTime;

    //
    // first loop over nodes and elements getting them to first zero their loads
    //

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();

    while ( ( nodePtr = theNodeIter() ) != 0 )
    {
        nodePtr->zeroUnbalancedLoad();
    }

    Element *elePtr;
    ElementIter &theElemIter = this->getElements();

    while ( ( elePtr = theElemIter() ) != 0 )
        if ( elePtr->isSubdomain() == false )
        {
            elePtr->zeroLoad();
        }

    // now loop over load patterns, invoking applyLoad on them
    LoadPattern *thePattern;
    LoadPatternIter &thePatterns = this->getLoadPatterns();

    while ( ( thePattern = thePatterns() ) != 0 )
    {
        thePattern->applyLoad( timeStep );
    }

    //
    // finally loop over the MP_Constraints and SP_Constraints of the domain
    //

    MP_ConstraintIter &theMPs = this->getMPs();
    MP_Constraint *theMP;

    while ( ( theMP = theMPs() ) != 0 )
    {
        theMP->applyConstraint( timeStep );
    }

    SP_ConstraintIter &theSPs = this->getSPs();
    SP_Constraint *theSP;

    while ( ( theSP = theSPs() ) != 0 )
    {
        theSP->applyConstraint( timeStep );
    }
}


void
Domain::setLoadConstant( void )
{
    // loop over all the load patterns that are currently added to the domain
    // getting them to set their loads as now constant
    LoadPattern *thePattern;
    LoadPatternIter &thePatterns = this->getLoadPatterns();

    while ( ( thePattern = thePatterns() ) != 0 )
    {
        thePattern->setLoadConstant();
    }
}



int
Domain::initialize( void )
{
    Element *elePtr;
    ElementIter &theElemIter = this->getElements();

    while ( ( elePtr = theElemIter() ) != 0 )
        // lvalue needed here for M$ VC++ compiler -- MHS
    {
        //const Matrix &ret = elePtr->getInitialStiff();
        elePtr->getInitialStiff(); //lvalue not needed anymore (J.Abell, Tue 25 Mar 2014 09:15:32 PM PDT)
    }

    return 0;
}


// Nima Tafazzoli (July 2011)
int
Domain::setDampingFactorsforElement( int ElementTag, int DampingTag )
{
    int result = 0;

    double a0 = 0.0;
    double a1 = 0.0;
    double a2 = 0.0;
    double a3 = 0.0;
    //    int classtag;
    string stiffness_type;
    string damping_type;


    Element *elePtr;
    elePtr = this->getElement( ElementTag );


    if ( elePtr == NULL )
    {
        cerr << "Error: (Domain::setDampingFactorsforElement): Can not get element " << ElementTag << " from domain!" << endl;
        exit( -1 );
    }


    Damping *dampPtr;
    dampPtr = this->getDamping( DampingTag );
    damping_type = dampPtr->return_dampingtype();


    if ( dampPtr == NULL )
    {
        cerr << "Error: (Domain::setDampingFactorsforElement): Can not get damping " << DampingTag << " from domain!" << endl;
        exit( -1 );
    }


    a0 = dampPtr->return_a0();
    a1 = dampPtr->return_a1();
    a2 = dampPtr->return_a2();
    a3 = dampPtr->return_a3();
    stiffness_type = dampPtr->return_stiffnesstype();


    result += elePtr->setDampingFactors( a0, a1, a2, a3, stiffness_type, damping_type );



    return result;
}


// Nima Tafazzoli (July 2011)
int
Domain::setDampingFactorsforNode( int NodeTag, int DampingTag )
{
    int result = 0;

    double a0 = 0.0;

    Node *nodePtr;
    nodePtr = this->getNode( NodeTag );


    if ( nodePtr == NULL )
    {
        cerr << "Error: (Domain::setDampingFactorsforNode): Can not get node " << NodeTag << " from domain!" << endl;
        exit( -1 );
    }


    Damping *dampPtr;
    dampPtr = this->getDamping( DampingTag );


    if ( dampPtr == NULL )
    {
        cerr << "Error: (Domain::setDampingFactorsforNode): Can not get damping " << DampingTag << " from domain!" << endl;
        exit( -1 );
    }


    a0 = dampPtr->return_a0();


    result += nodePtr->setDampingFactor( a0 );


    return result;
}



int
Domain::commit( void )
{

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();
    Element *elePtr;
    ElementIter &theElemIter = this->getElements();
    
    //Do the actual commit of nodal displacements
    while ( ( nodePtr = theNodeIter() ) != 0 )
    {
        nodePtr->commitState();
    }

    //Commit all elements
    while ( ( elePtr = theElemIter() ) != 0 )
    {
        elePtr->commitState();
    }

    return 0;
}


/*************************************************************************************
* Added by Sumeet 3rd September, 2016, to output converged step 
* Call this function to output at every converged step and write in HDF5 file
**************************************************************************************/
int
Domain::output_step( void )
{
    
    bool Enable_Process_output = false;
    int rank=0; 
#ifdef _PARALLEL_PROCESSING  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank!=0)
        Enable_Process_output=true;
#else
    Enable_Process_output = true;
#endif

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();
    Element *elePtr;
    ElementIter &theElemIter = this->getElements();

    globalESSITimer.start("Domain_Mesh_Output");

    // Sumeet [September, 2016], to disable non_converged_iterations when output itself is disabled.
    non_converged_iterations_output_is_enabled = (output_is_enabled and non_converged_iterations_output_is_enabled); 

    if (output_is_enabled && countdown_til_output == 0)
    {
        //Write out static mesh data once!
        if (!have_written_static_mesh_data)
        {
#ifdef _PARALLEL_PROCESSING
            theOutputWriter.syncWriters();
#endif
            cout << " and  (" << rank << ")" ;
            cout << " - Outputting mesh.\n";
            globalESSITimer.start("HDF5_write_global_data");

            // cout  << " numberOfDomainElementOutputs " << numberOfDomainElementOutputs << endl;
            // cout  << " Number_of_Gauss_Points " << Number_of_Gauss_Points << endl;
            // cout  << " Number_of_Connectivity_Nodes " << Number_of_Connectivity_Nodes << endl;

            theOutputWriter.writeGlobalMeshData(this->getNumNodes(),
                                                this->getNumElements(),
                                                maxNodesTag,
                                                maxElementsTag,
                                                numberOfDomainNodeDOFs,
                                                numberOfDomainElementOutputs,
                                                this->Number_of_Gauss_Points,
                                                this->Number_of_Connectivity_Nodes);
            globalESSITimer.stop("HDF5_write_global_data");
            globalESSITimer.start("HDF5_buffer_nodes");

            //Write Node Mesh data! 
            for(int i =0 ; i<=maxNodesTag; i++){
                nodePtr = this->getNode(i);
                if(nodePtr) theOutputWriter.writeNodeMeshData(i, nodePtr->getCrds(), nodePtr->getNumberDOF());
            }

            theOutputWriter.EnableReactionOutput(reaction_output_is_enabled);  // enable/disable reactions output  [Sumeet]

            SP_Constraint *sp_ptr = 0;
            SP_ConstraintIter &theSP_Iter = this->getSPs();
            this->Number_of_Constrained_Dofs=0;

            while ( ( sp_ptr = theSP_Iter() ) != 0 )
            {
                int nodetag = sp_ptr->getNodeTag();
                int dof = sp_ptr->getDOF_Number();
                Number_of_Constrained_Dofs ++;

                theOutputWriter.writeSPConstraintsData(nodetag, dof);
            }



            globalESSITimer.stop("HDF5_buffer_nodes");

            globalESSITimer.start("HDF5_buffer_elements");


            //Write Element Mesh data!
            while ( ( elePtr = theElemIter() ) != 0 )
            {

                theOutputWriter.writeElementMeshData(elePtr->getTag() ,
                                                     elePtr->getExternalNodes(),
                                                     elePtr->getMaterialTag() ,
                                                     elePtr->getGaussCoordinates(),
                                                     elePtr->getElementclassTag());
            }

            /*********************** Write Material Data   - By Sumeet 30th July, 2016 */
            
            int number_of_materials = max(maxNDMaterialsTag,maxNDMaterialLTsTag);
            number_of_materials = max(number_of_materials,maxUniaxialMaterialsTag);
            theOutputWriter.reserveSpaceForDatasets(number_of_materials);

            // ///////////////////////// For Debugging //////////////////////////////////
            // cout << " maxNDMaterialsTag " << maxNDMaterialsTag << endl;
            // cout << " maxNDMaterialLTsTag " << maxNDMaterialLTsTag <<endl;
            // cout << " maxUniaxialMaterialsTag " << maxUniaxialMaterialsTag << endl;
            // cout << " number of materials " << number_of_materials << endl;
            // //////////////////////////////////////////////////////////////////////////

            Material *matPtr;
            NDMaterialIter &theMatIter = this->getNDMaterials();                theMatIter.reset();
            NDMaterialLTIter &theMatLTIter = this->getNDMaterialLTs();          theMatLTIter.reset();
            UniaxialMaterialIter &theUniMatIter = this->getUniaxialMaterials(); theUniMatIter.reset();

            std::stringstream material_info;

            while ( ( matPtr = theMatIter() ) != 0 )
            {
                material_info.str("") ;
                matPtr->Print(material_info,0);
                theOutputWriter.writeMaterialMeshData(matPtr->getTag(),material_info.str());

            }

            while ( ( matPtr = theMatLTIter() ) != 0 )
            {
                material_info.str("") ;
                matPtr->Print(material_info,0);
                theOutputWriter.writeMaterialMeshData(matPtr->getTag(),material_info.str());

            }

            while ( ( matPtr = theUniMatIter() ) != 0 )
            {
                material_info.str("") ;
                matPtr->Print(material_info,0);
                theOutputWriter.writeMaterialMeshData(matPtr->getTag(),material_info.str());

            }

            /**************************************************************************************/

            globalESSITimer.stop("HDF5_buffer_elements");
            globalESSITimer.start("HDF5_write");
            theOutputWriter.writeMesh();
            globalESSITimer.stop("HDF5_write");
            
            have_written_static_mesh_data = true;

            // Erase all temporaray arrays 
            theOutputWriter.DeleteMeshArrays();
            
            if(number_of_eigen_modes>=0){
                theOutputWriter.writeEigenMesh(number_of_eigen_modes);
                return 0;
            }

        }

        theOutputWriter.setTime(currentTime);
    }

    if (output_is_enabled && Enable_Process_output && (countdown_til_output == 0))
    {

        globalESSITimer.stop("Domain_Mesh_Output");
        globalESSITimer.start("Domain_Node_Commit_and_output");

        //Do node Output 
        int noutputs;
        theNodeIter = this->getNodes();

        int pos=0; hid_t id_displacement = theOutputWriter.getDisplacementId(); 

        for(int i =0 ; i<=maxNodesTag; i++){

            nodePtr = this->getNode(i);
            if(nodePtr){ 
                noutputs = nodePtr->getNumberDOF();
                theOutputWriter.StepOutput(id_displacement, nodePtr->getTrialDisp(), pos,noutputs);
                pos = pos + noutputs;
            }
        }

        globalESSITimer.stop("Domain_Node_Commit_and_output");
        globalESSITimer.start("Domain_Element_Commit_and_output");

        //Do element output!
        if (element_output_is_enabled)
        {
            int noutputs;
            theElemIter = this->getElements();

            int pos1=0; hid_t id_Elements_Output = theOutputWriter.getElementOutputId(); 
            int pos2=0; hid_t id_Gauss_Output = theOutputWriter.getGaussOutputId(); 
            while ( ( elePtr = theElemIter() ) != 0 )
            {
                
                noutputs = Element_Class_Desc[elePtr->getClassTag()]%100;
                if(noutputs) theOutputWriter.StepOutput(id_Elements_Output, elePtr->getElementOutput(),pos1, noutputs);
                pos1 = pos1 + noutputs;

                noutputs=((Element_Class_Desc[elePtr->getClassTag()]%100000)/100)*18;
                if(noutputs) theOutputWriter.StepOutput(id_Gauss_Output, elePtr->getGaussOutput(),pos2, noutputs);
                pos2 = pos2 + noutputs;

            }
        }

        if(reaction_output_is_enabled)
        {
            //Do Support Reactions Output [Sumeet August,2016]
            SP_Constraint *sp_ptr = 0;
            SP_ConstraintIter &theSP_Iter = this->getSPs();

            this->calculateNodalReactions(0);
            vector<float> Support_Reactions(Number_of_Constrained_Dofs);
            int index = 0;
            while ( ( sp_ptr = theSP_Iter() ) != 0 )
            {
                Support_Reactions[index++]= this->getNode(sp_ptr->getNodeTag())->getReaction()(sp_ptr->getDOF_Number());
                theOutputWriter.writeSupportReactions( Number_of_Constrained_Dofs, Support_Reactions);
            }
        }

        globalESSITimer.stop("Domain_Element_Commit_and_output");

        if (countdown_til_output == 0)
        {
            countdown_til_output = output_every_nsteps;
        }
        countdown_til_output--;
    }

    return 0;
}



/*************************************************************************************
* Added by Sumeet 3rd August, 2016, to output substep iteration steps for debugging 
* The function commits at every substep i,e the trail displacements and trial element
* output HDF5 Output file. It does not commit any displacements or element output
**************************************************************************************/
int
Domain::output_iteration( int global_iteration_no )
{
    // gets trial output from the nodes and elements. Also in charge of storing
    // the data to the outputwriter.

    bool Enable_Process_output = false;
#ifdef _PARALLEL_PROCESSING
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank!=0)
        Enable_Process_output=true;
#else
    Enable_Process_output = true;
#endif

    if(output_is_enabled && Enable_Process_output)
    {

        Node *nodePtr;
        NodeIter &theNodeIter = this->getNodes();
        Element *elePtr;
        ElementIter &theElemIter = this->getElements();

        if (output_is_enabled)
        {
            //Write out static mesh data once!
            if (global_iteration_no==1) {       
                theOutputWriter.writeIterationMesh();
            }        
        }

        theOutputWriter.setGlobalIterationNo(global_iteration_no);

        //get Trail Results from nodes
        int noutputs;
        theNodeIter.reset();
        int pos=0; hid_t id_trial_displacement = theOutputWriter.getTrialDisplacementId(); 
        for(int i =0 ; i<=maxNodesTag; i++){

            nodePtr = this->getNode(i);
            if(nodePtr){ 
                noutputs = nodePtr->getNumberDOF();
                theOutputWriter.IterationOutput(id_trial_displacement, nodePtr->getTrialDisp(), pos,noutputs);
                pos = pos + noutputs;
            }
        }

        //get Trail Results from nodes
        if (element_output_is_enabled )
        {
            int noutputs;
            theElemIter.reset();

            int pos1=0; hid_t id_Trial_Elements_Output = theOutputWriter.getTrialElementOutputId(); 
            int pos2=0; hid_t id_Trial_Gauss_Output = theOutputWriter.getTrialGaussOutputId(); 
            while ( ( elePtr = theElemIter() ) != 0 )
            {
                
                noutputs = Element_Class_Desc[elePtr->getClassTag()]%100;
                if(noutputs) theOutputWriter.IterationOutput(id_Trial_Elements_Output, elePtr->getElementOutput(),pos1, noutputs);
                pos1 = pos1 + noutputs;

                noutputs=((Element_Class_Desc[elePtr->getClassTag()]%100000)/100)*18;
                if(noutputs) theOutputWriter.IterationOutput(id_Trial_Gauss_Output, elePtr->getGaussOutput(),pos2, noutputs);
                pos2 = pos2 + noutputs;

            }

        }

        cout << "................... completed!!" << endl;;
    }



    return 0;
}


int
Domain::revertToLastCommit( void )
{
    //
    // first invoke revertToLastCommit  on all nodes and elements in the domain
    //

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();

    while ( ( nodePtr = theNodeIter() ) != 0 )
    {
        nodePtr->revertToLastCommit();
    }

    Element *elePtr;
    ElementIter &theElemIter = this->getElements();

    while ( ( elePtr = theElemIter() ) != 0 )
    {
        elePtr->revertToLastCommit();
    }

    // set the current time and load factor in the domain to last committed
    currentTime = committedTime;
    dT = 0.0;

    // apply load for the last committed time
    this->applyLoad( currentTime );

    return this->update();
}

int
Domain::revertToStart( void )
{
    //
    // first invoke revertToLastCommit  on all nodes and
    // elements in the domain
    //

    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();

    while ( ( nodePtr = theNodeIter() ) != 0 )
    {
        nodePtr->revertToStart();
    }

    Element *elePtr;
    ElementIter &theElements = this->getElements();

    while ( ( elePtr = theElements() ) != 0 )
    {
        elePtr->revertToStart();
    }

    /////////////////////////////////////////////////////

    // set the current time and load factor in the domain to last committed
    committedTime = 0;
    currentTime = 0;
    dT = 0.0;

    // apply load for the last committed time
    this->applyLoad( currentTime );

    return this->update();
}

int
Domain::update( void )
{
    // cout << "Domain::update( void ) -- Constitutive integration happening!\n";
    // set the global constants
    ops_Dt = dT;
    ops_TheActiveDomain = this;

    int ok = 0;

    globalESSITimer.start("Constitutive_Integration");
# ifdef _PDD

    NanoTimer myTime;
    Graph *theEleGraph = this->getElementGraph();
    // VertexIter &theVertexIter = theEleGraph->getVertices();
    Vertex *vertexPtr;
    ElementIter &theEles = this->getElements(); //has to be put here in order to reset the eleIter(), which firstly called by getElementGraph without rewind!!!!
    Element *theEle;

    while ( ( theEle = theEles() ) != 0 )
    {
        int eleTag = theEle->getTag();

        // while ( ( vertexPtr = theVertexIter() ) != 0 )
        // {
        //     if ( vertexPtr->getRef() == eleTag )
        //     {
        //         theVertexIter.reset();
        //         break;
        //     }
        // }
        // Vertex *
        vertexPtr = theEleGraph->getVertexPtr(eleTag);

        if ( vertexPtr != 0 )
        {
            // double start_time = MPI_Wtime();  // MPI WTime is not assured to be a hi-res clock, and isn't in Ubuntu14.04 when OpenMPI is installed from repos. Must build by hand to get this feature!!
            ESSITimer::tic();
            ok += theEle->update();
            double duration = ESSITimer::toc();
            // double end_time = MPI_Wtime();
            double oldweight = vertexPtr->getWeight();
            double newweight = oldweight + 1.0e6 * duration;
            // vertexPtr->setWeight(oldweight + 100.0 * (end_time - start_time)); //to be milisecond unit
            vertexPtr->setWeight(newweight); //to be milisecond unit

            // cout << eleTag <<  ": oldweight = " << oldweight << ", newweight = " << newweight << endl;
        }
        else
        {
            // cout << eleTag <<  ": no weighting info!" << endl;
            ok += theEle->update();
        }
    }

# else
    ElementIter &theEles = this->getElements();
    Element *theEle;

    while ( ( theEle = theEles() ) != 0 )
    {
        ok += theEle->update();
    }

# endif
#ifdef _PARALLEL_PROCESSING
    //Need to syncronize to get proper global timing
    MPI_Barrier( MPI_COMM_WORLD);
#endif
    globalESSITimer.stop("Constitutive_Integration");

    if ( ok != 0 )
    {
        cerr << "Domain::update - domain failed in update\n";
    }

    return ok;
}


int
Domain::update( double newTime, double dT )
{
    // cout << "Domain::update( double newTime, double dT ) -- applyLoad() getting called\n";
    this->applyLoad( newTime );
    this->update();     /// if this is un/commented, update(void)  gets called twice per step for an analysis with a Linear algorithm (with_no_convergence_check)
    return 0;
}



int
Domain::newStep( double dT )
{
    return 0;
}



int
Domain::setEigenvalues( const Vector & theValues )
{
    // make sure the eigen value vector is large enough
    if ( theEigenvalues == 0 || theEigenvalues->Size() != theValues.Size() )
    {

        // if not zero delete the old and create a new one
        if ( theEigenvalues != 0 )
        {
            delete theEigenvalues;
        }

        // create the new vector
        theEigenvalues = new Vector( theValues );
    }
    else

        // otherwise just a straight assignment
    {
        *theEigenvalues = theValues;
    }


    // now set the time at which eigen values were determined to be current domain time
    theEigenvalueSetTime = this->getCurrentTime();

    return 0;
}


const Vector &
Domain::getEigenvalues( void )
{
    // ensure the eigen values were set
    if ( theEigenvalues == 0 )
    {
        cerr << "Domain::getEigenvalues - Eigenvalues were never set\n";
        exit( -1 );
    }

    return *theEigenvalues;
}

double
Domain::getTimeEigenvaluesSet( void )
{
    return theEigenvalueSetTime;
}

void
Domain::setDomainChangeStamp( int newStamp )
{
    currentGeoTag = newStamp;
}


void
Domain::domainChange( void )
{
    hasDomainChangedFlag = true;
}


int
Domain::hasDomainChanged( void )
{
    // if the flag indicating the domain has changed since the
    // last call to this method has changed, increment the integer
    // and reset the flag
    bool result = hasDomainChangedFlag;
    hasDomainChangedFlag = false;

    if ( result == true )
    {
        currentGeoTag++;
        nodeGraphBuiltFlag = false;
        eleGraphBuiltFlag = false;
    }

    // return the integer so user can determine if domain has changed
    // since their last call to this method
    return currentGeoTag;
}

void
Domain::Print( ostream & s, int flag )
{

    s << "Current Domain Information\n";
    s << "\tCurrent Time: " << currentTime;
    s << "\ntCommitted Time: " << committedTime << endl;

    s << "\nNODE DATA: NumNodes: " << theNodes->getNumComponents() << "\n";
    //theNodes->Print(s, flag);

    s << "\nELEMENT DATA: NumEle: " << theElements->getNumComponents() << "\n";
    //theElements->Print(s, flag);

    s << "\nSP_Constraints: numConstraints: ";
    s << theSPs->getNumComponents() << "\n";
    //theSPs->Print(s, flag);

    s << "\nMP_Constraints: numConstraints: ";
    s << theMPs->getNumComponents() << "\n";
    //theMPs->Print(s, flag);

    s << "\nLOAD PATTERNS: num Patterns: ";
    s << theLoadPatterns->getNumComponents() << "\n\n";
    theLoadPatterns->Print( s, flag );
}

ostream &operator<<( ostream & s, Domain & M )
{
    M.Print( s );
    return s;
}




int
Domain::buildEleGraph( Graph * theEleGraph )
{
    int numVertex = this->getNumElements();

    // see if quick return

    if ( numVertex == 0 )
    {
        return 0;
    }


    // create another vertices array which aids in adding edges

    int *theElementTagVertices = 0;
    int maxEleNum = maxElementsTag;

    theElementTagVertices = new int[maxEleNum + 1];

    if ( theElementTagVertices == 0 )
    {
        cerr << "WARNING Domain::buildEleGraph ";
        cerr << " - Not Enough Memory for ElementTagVertices\n";
        return -1;
    }

    for ( int j = 0; j <= maxEleNum; j++ )
    {
        theElementTagVertices[j] = -1;
    }

    // cout << "Building element graph............\n";
    // cout << "     numVertex = " << numVertex << "\n";
    // cout << "     maxEleNum = " << maxEleNum << "\n\n";


    // now create the vertices with a reference equal to the element number.
    // and a tag which ranges from 0 through numVertex-1

    ElementIter &eleIter = this->getElements();
    Element *elePtr;
    int count = START_VERTEX_NUM;  // = 0 Defined in Vertex.h

    while ( ( elePtr = eleIter() ) != 0 )
    {
        int ElementTag = elePtr->getTag();
        Vertex *vertexPtr = new Vertex( count, ElementTag );

        if ( vertexPtr == 0 )
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Vertex\n";
            delete [] theElementTagVertices;
            return -1;
        }

        // # ifdef _PDD //Guanzhou added
        vertexPtr->setVertexSize( elePtr->getObjectSize() );
        //represents the (memory) size of vertex, which equals to the size of data
        //should be sent through SendSelf!
        // # endif
        theEleGraph->addVertex( vertexPtr );
        theElementTagVertices[ElementTag] = count++;
    }

    // We now need to determine which elements are asssociated with each node.
    // As this info is not in the Node interface we must build it; which we
    // do using vertices for each node, when we addVertex at thes nodes we
    // will not be adding vertices but element tags.

    Vertex **theNodeTagVertices = 0;
    int maxNodNum = maxNodesTag;
    Node *nodPtr;
    theNodeTagVertices = new Vertex *[maxNodNum + 1];

    if ( theNodeTagVertices == 0 )
    {
        cerr << "WARNING Domain::buildEleGraph ";
        cerr << " - Not Enough Memory for NodeTagVertices\n";
        return -1;
    }

    for ( int l = 0; l <= maxNodNum; l++ )
    {
        theNodeTagVertices[l] = 0;
    }

    // now create the vertices with a reference equal to the node number.
    // and a tag which ranges from 0 through numVertex-1 and placed in
    // theNodeTagVertices at a position equal to the node's tag.

    NodeIter &nodeIter = this->getNodes();
    count = START_VERTEX_NUM;

    while ( ( nodPtr = nodeIter() ) != 0 )
    {
        int nodeTag = nodPtr->getTag();
        Vertex *vertexPtr = new Vertex( count++, nodeTag );
        theNodeTagVertices[nodeTag] = vertexPtr;

        if ( vertexPtr == 0 )
        {
            cerr << "WARNING Domain::buildEleGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Node Vertex\n";
            delete [] theNodeTagVertices;
            return -1;
        }
    }

    // now add the the Elements to the nodes

    ElementIter &eleIter2 = this->getElements();

    while ( ( elePtr = eleIter2() ) != 0 )
    {
        int eleTag = elePtr->getTag();
        const ID &id = elePtr->getExternalNodes();

        int size = id.Size();

        for ( int i = 0; i < size; i++ )
        {
            theNodeTagVertices[id( i )]->addEdge( eleTag );
        }
    }


    // now add the edges to the vertices of our element graph;
    // this is done by looping over the Node vertices, getting their
    // Adjacenecy and adding edges between elements with common nodes
# ifdef _PDD
    Vertex *vertexPtr;

    // For all node tags
    for ( int k = 0; k <= maxNodNum; k++ )

        // Get a pointer to the graph vertex corresponding to tag "k"
        if ( ( vertexPtr = theNodeTagVertices[k] ) != 0 ) // If not available skip this node tag
        {
            // Get the list of elements connected to this node
            const ID &id = vertexPtr->getAdjacency();

            //Get the number of elements connected to this node
            int size = id.Size();

            for ( int i = 0; i < size; i++ )
            {
                int Element1 = id( i );


                Element *ele1 = this->getElement( Element1 );

                const ID &nodes_in_element1 = ele1->getExternalNodes();
                int number_of_nodes_in_element1 = nodes_in_element1.Size();

                //How many elements are in the boundary of an element (added 8/20/13):
                //-------------------------
                int Num1_Boundary_Nodes = ele1->getNumberOfBoundaryNodes();

                //--------------------------
                int vertexTag1 = theElementTagVertices[Element1];


                for ( int j = 0; j < size; j++ )
                    if ( i != j )
                    {

                        int Element2 = id( j );
                        int vertexTag2 = theElementTagVertices[Element2];

                        if ( vertexTag1 > vertexTag2 ) // This avoids doing double work
                        {
                            Element *ele2 = this->getElement( Element2 );


                            const ID &nodes_in_element2 = ele2->getExternalNodes();
                            int number_of_nodes_in_element2 = nodes_in_element2.Size();
                            //How many elements are in the boundary of an element (added 8/20/13):
                            //-------------------------
                            int Num2_Boundary_Nodes = ele2->getNumberOfBoundaryNodes();

                            //--------------------------
                            // Keep the smallest of number of boundary nodes
                            int number_of_common_nodes = Num1_Boundary_Nodes;
                            if ( Num1_Boundary_Nodes > Num2_Boundary_Nodes)
                            {
                                number_of_common_nodes = Num2_Boundary_Nodes;
                            }

                            //--------------------------
                            //Count the number of common nodes
                            int common = 0;

                            for ( int kk = 0; kk < number_of_nodes_in_element1; kk++ )
                            {
                                for ( int l = 0; l < number_of_nodes_in_element2; l++ )
                                {
                                    if ( nodes_in_element1( kk ) == nodes_in_element2( l ) )
                                    {
                                        common++;
                                    }
                                }
                            }

                            // cout << "Element" << ele1->getTag() << " and element " << ele2->getTag() << " have " << common << " nodes in common.";

                            // Adde edge for both vertices
                            if (  common == number_of_common_nodes  )
                            {
                                // cout << "  Vertex added!\n";
                                theEleGraph->addEdge( vertexTag1, vertexTag2 );
                                theEleGraph->addEdge( vertexTag2, vertexTag1 );
                            };
                        }

                    }
            }
        }

# else
    Vertex *vertexPtr;

    for ( int k = 0; k <= maxNodNum; k++ )
        if ( ( vertexPtr = theNodeTagVertices[k] ) != 0 )
        {

            const ID &id = vertexPtr->getAdjacency();

            int size = id.Size();

            for ( int i = 0; i < size; i++ )
            {
                int Element1 = id( i );

                int vertexTag1 = theElementTagVertices[Element1];

                for ( int j = 0; j < size; j++ )
                    if ( i != j )
                    {

                        int Element2 = id( j );
                        int vertexTag2 = theElementTagVertices[Element2];

                        // addEdge() adds for both vertices - do only once
                        if ( vertexTag1 > vertexTag2 )
                        {
                            theEleGraph->addEdge( vertexTag1, vertexTag2 );
                            theEleGraph->addEdge( vertexTag2, vertexTag1 );
                        }
                    }
            }
        }

# endif


    // done now delete theElementTagVertices, the node Vertices and
    // theNodeTagVertices

    delete [] theElementTagVertices;

    for ( int i = 0; i <= maxNodNum; i++ )
        if ( ( vertexPtr = theNodeTagVertices[i] ) != 0 )
        {
            delete vertexPtr;
        }

    delete [] theNodeTagVertices;

    // cout << "Done building ele graph.\n";


    return 0;

}

int
Domain::buildNodeGraph( Graph * theNodeGraph )
{
    int numVertex = this->getNumNodes();

    if ( numVertex == 0 )
    {
        return 0;
    }

    // create another vertices array which aids in adding edges

    int *theNodeTagVertices = 0;
    int maxNodNum = maxNodesTag;
    Node *nodPtr;
    theNodeTagVertices = new int [maxNodNum + 1];

    if ( theNodeTagVertices == 0 )
    {
        cerr << "WARNING Domain::buildNodeGraph ";
        cerr << " - Not Enough Memory for NodeTagVertices\n";
        return -1;
    }

    for ( int j = 0; j <= maxNodNum; j++ )
    {
        theNodeTagVertices[j] = -1;
    }

    // now create the vertices with a reference equal to the node number.
    // and a tag which ranges from START_VERTEX_NUM through
    // numNodes+START_VERTEX_NUM

    NodeIter &nodeIter = this->getNodes();
    int count = START_VERTEX_NUM;

    while ( ( nodPtr = nodeIter() ) != 0 )
    {
        int nodeTag = nodPtr->getTag();
        Vertex *vertexPtr = new Vertex( count, nodeTag );

        if ( vertexPtr == 0 )
        {
            cerr << "WARNING Domain::buildNodeGraph";
            cerr << " - Not Enough Memory to create ";
            cerr << count << "th Vertex\n";
            delete [] theNodeTagVertices;
            return -1;
        }

        // add the vertex to the graph
        theNodeGraph->addVertex( vertexPtr );
        theNodeTagVertices[nodeTag] = count++;
    }

    // now add the edges, by looping over the Elements, getting their
    // IDs and adding edges between all elements who share a node.

    Element *elePtr;
    ElementIter &eleIter = this->getElements();

    while ( ( elePtr = eleIter() ) != 0 )
    {
        const ID &id = elePtr->getExternalNodes();

        int size = id.Size();

        for ( int i = 0; i < size; i++ )
        {
            int node1 = id( i );
            int vertexTag1 = theNodeTagVertices[node1];

            for ( int j = 0; j < size; j++ )
                if ( i != j )
                {

                    int node2 = id( j );
                    int vertexTag2 = theNodeTagVertices[node2];

                    // addEdge() adds for both vertices - do only once
                    if ( vertexTag1 > vertexTag2 )
                    {
                        theNodeGraph->addEdge( vertexTag1, vertexTag2 );
                    }
                }
        }
    }

    // done now delete theNodeTagVertices
    delete [] theNodeTagVertices;

    return 0;
}


int
Domain::sendSelf( int cTag, Channel & theChannel )
{
    // update the commitTag and currentGeoTag
    commitTag = cTag;

    this->hasDomainChanged();

    // first we send info about the current domain flag and the number of
    // elements, nodes, constraints and load patterns currently in the domain
    int numEle, numNod, numSPs, numMPs, numLPs;
    numNod = theNodes->getNumComponents();
    numEle = theElements->getNumComponents();
    numSPs = theSPs->getNumComponents();
    numMPs = theMPs->getNumComponents();
    numLPs = theLoadPatterns->getNumComponents();

    ID domainData( 11 + 13 );  //jose added the maxtags
    domainData( 0 ) = currentGeoTag;

    domainData( 1 ) = numNod;
    domainData( 2 ) = numEle;
    domainData( 3 ) = numSPs;
    domainData( 4 ) = numMPs;
    domainData( 5 ) = numLPs;

    // add the database tag for the ID's storing node, element, constraints
    // and loadpattern data into domainData
    // NOTE: if these still 0 get new ones from the channel
    // if ( dbNod == 0 )
    // {
    //     dbNod = theChannel.getDbTag();
    //     dbEle = theChannel.getDbTag();
    //     dbSPs = theChannel.getDbTag();
    //     dbMPs = theChannel.getDbTag();
    //     dbLPs = theChannel.getDbTag();
    // }

    domainData( 6 ) = 0;// Deprecated by J.Abell     dbNod;
    domainData( 7 ) = 0;// Deprecated by J.Abell     dbEle;
    domainData( 8 ) = 0;// Deprecated by J.Abell     dbSPs;
    domainData( 9 ) = 0;// Deprecated by J.Abell     dbMPs;
    domainData( 10 ) = 0;// Deprecated by J.Abell     dbLPs;
    domainData( 11 ) = maxElementsTag;
    domainData( 12 ) = maxNodesTag;
    domainData( 13 ) = maxUniaxialMaterialsTag;
    domainData( 14 ) = maxNDMaterialsTag;
    domainData( 15 ) = maxNDMaterialLTsTag;
    domainData( 16 ) = maxSectionsTag;
    domainData( 17 ) = maxSectionRepresentsTag;
    domainData( 18 ) = maxMultipleSupportsTag;
    domainData( 19 ) = maxAccelerationFieldsTag;
    domainData( 20 ) = maxDampingsTag;
    domainData( 21 ) = maxSPsTag;
    domainData( 22 ) = maxMPsTag;
    domainData( 23 ) = maxLoadPatternsTag;





    if ( theChannel.sendID( 0, commitTag, domainData ) < 0 )
    {
        cerr << "Domain::send - channel failed to send the initial ID\n";
        return -1;
    }

    // send the time information
    Vector domainTime( 1 );
    domainTime( 0 ) = committedTime;

    if ( theChannel.sendVector( 0, commitTag, domainTime ) < 0 )
    {
        cerr << "Domain::send - channel failed to send the time Vector\n";
        return -2;
    }

    // now check if data defining the objects in the domain needs to be sent
    // NOTE THIS APPROACH MAY NEED TO CHANGE FOR VERY LARGE PROBLEMS IF CHANNEL CANNOT
    // HANDLE VERY LARGE ID OBJECTS.


    if ( lastChannel != theChannel.getTag() )//|| lastGeoSendTag != currentGeoTag )
    {

        lastChannel = theChannel.getTag();

        //
        // into an ID we are gonna place the class and db tags for each node so can rebuild
        // this ID we then send to the channel
        //

        // create the ID and get the node iter
        if ( numNod != 0 )
        {
            ID nodeData( numNod * 2 );
            Node *theNode;
            NodeIter &theNodes = this->getNodes();
            int loc = 0;

            // loop over nodes in domain adding their classTag and dbTag to the ID
            while ( ( theNode = theNodes() ) != 0 )
            {
                nodeData( loc ) = theNode->getClassTag();
                int dbTag = theNode->getDbTag();

                // if dbTag still 0 get one from Channel;
                // if this tag != 0 set the dbTag in node
                if ( dbTag == 0 ) // go get a new tag and setDbTag in ele if this not 0
                {
                    dbTag = theChannel.getDbTag();

                    if ( dbTag != 0 )
                    {
                        theNode->setDbTag( dbTag );
                    }
                }

                nodeData( loc + 1 ) = dbTag;
                loc += 2;
            }

            // now send the ID
            // if ( theChannel.sendID( dbNod, currentGeoTag, nodeData ) < 0 )
            if ( theChannel.sendID( 0, 0, nodeData ) < 0 )
            {
                cerr << "Domain::send - channel failed to send the node ID\n";
                return -2;
            }
        }

        // we do the same for elements as we did for nodes above .. see comments
        // for nodes if you can't figure whats going on!

        if ( numEle != 0 )
        {
            ID elementData( numEle * 2 );
            Element *theEle;
            ElementIter &theElements = this->getElements();
            int loc = 0;

            while ( ( theEle = theElements() ) != 0 )
            {
                elementData( loc ) = theEle->getClassTag();
                int dbTag = theEle->getDbTag();

                if ( dbTag == 0 ) // go get a new tag and setDbTag in ele if this not 0
                {
                    dbTag = theChannel.getDbTag();

                    if ( dbTag != 0 )
                    {
                        theEle->setDbTag( dbTag );
                    }
                }

                elementData( loc + 1 ) = dbTag;
                loc += 2;
            }

            // now send the ID
            // if ( theChannel.sendID( dbEle, currentGeoTag, elementData ) < 0 )
            if ( theChannel.sendID( 0, 0, elementData ) < 0 )
            {
                cerr << "Domain::send - channel failed to send the element ID\n";
                return -3;
            }
        }

        // we do the same for SP_Constraints as for Nodes above .. see comments
        // for nodes if you can't figure whats going on!

        if ( numSPs != 0 )
        {
            ID spData( numSPs * 2 );
            SP_Constraint *theSP;
            SP_ConstraintIter &theSPs = this->getSPs();
            int loc = 0;

            while ( ( theSP = theSPs() ) != 0 )
            {
                spData( loc ) = theSP->getClassTag();
                int dbTag = theSP->getDbTag();

                if ( dbTag == 0 ) // go get a new tag and setDbTag in ele if this not 0
                {
                    dbTag = theChannel.getDbTag();

                    if ( dbTag != 0 )
                    {
                        theSP->setDbTag( dbTag );
                    }
                }

                spData( loc + 1 ) = dbTag;
                loc += 2;
            }

            // if ( theChannel.sendID( dbSPs, currentGeoTag, spData ) < 0 )
            if ( theChannel.sendID( 0, 0, spData ) < 0 )
            {
                cerr << "Domain::send - channel failed to send the SP_Constraint ID\n";
                return -4;
            }
        }

        // we do the same for MP_Constraints as for Nodes above .. see comments
        // for nodes if you can't figure whats going on!

        if ( numMPs != 0 )
        {
            ID mpData( numMPs * 2 );
            MP_Constraint *theMP;
            MP_ConstraintIter &theMPs = this->getMPs();
            int loc = 0;

            while ( ( theMP = theMPs() ) != 0 )
            {
                mpData( loc ) = theMP->getClassTag();
                int dbTag = theMP->getDbTag();

                if ( dbTag == 0 ) // go get a new tag and setDbTag in ele if this not 0
                {
                    dbTag = theChannel.getDbTag();

                    if ( dbTag != 0 )
                    {
                        theMP->setDbTag( dbTag );
                    }
                }

                mpData( loc + 1 ) = dbTag;
                loc += 2;
            }

            // if ( theChannel.sendID( dbMPs, currentGeoTag, mpData ) < 0 )
            if ( theChannel.sendID( 0, 0, mpData ) < 0 )
            {
                cerr << "Domain::send - channel failed to send the MP_Constraint ID\n";
                return -5;
            }
        }

        // we do the same for LoadPatterns as we did for Nodes above .. see comments
        // for nodes if you can't figure whats going on!

        if ( numLPs != 0 )
        {
            ID lpData( numLPs * 2 );
            LoadPattern *theLP;
            LoadPatternIter &theLPs = this->getLoadPatterns();
            int loc = 0;

            while ( ( theLP = theLPs() ) != 0 )
            {
                lpData( loc ) = theLP->getClassTag();
                int dbTag = theLP->getDbTag();

                if ( dbTag == 0 ) // go get a new tag and setDbTag in ele if this not 0
                {
                    dbTag = theChannel.getDbTag();

                    if ( dbTag != 0 )
                    {
                        theLP->setDbTag( dbTag );
                    }
                }

                lpData( loc + 1 ) = dbTag;
                loc += 2;
            }

            // if ( theChannel.sendID( dbLPs, currentGeoTag, lpData ) < 0 )
            if ( theChannel.sendID( 0, 0, lpData ) < 0 )
            {
                cerr << "Domain::send - channel failed to send the LoadPattern ID\n";
                return -6;
            }
        }


        // now so that we don't do this next time if nothing in the domain has changed
        // lastGeoSendTag = currentGeoTag;
    }

    //
    // now we invoke sendSelf on each of the objects ..
    // NOTE: don't have to set the dbTags of the objects as just done this above
    //

    // send the nodes
    Node *theNode;
    NodeIter &theNodes = this->getNodes();

    while ( ( theNode = theNodes() ) != 0 )
    {
        if ( theNode->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "Domain::send - node with tag " << theNode->getTag() << " failed in sendSelf\n";
            return -7;
        }
    }

    // send the elements
    Element *theEle;
    ElementIter &theElements = this->getElements();

    while ( ( theEle = theElements() ) != 0 )
    {
        if ( theEle->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "Domain::send - element with tag " << theEle->getTag() << " failed in sendSelf\n";
            return -8;
        }
    }

    // send the single point constraints
    SP_Constraint *theSP;
    SP_ConstraintIter &theSPs = this->getSPs();

    while ( ( theSP = theSPs() ) != 0 )
    {
        if ( theSP->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "Domain::send - SP_Constraint with tag " << theSP->getTag() << " failed in sendSelf\n";
            return -9;
        }
    }

    // send the multi point constraints
    MP_Constraint *theMP;
    MP_ConstraintIter &theMPs = this->getMPs();

    while ( ( theMP = theMPs() ) != 0 )
    {
        if ( theMP->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "Domain::send - MP_Constraint with tag " << theMP->getTag() << " failed in sendSelf\n";
            return -10;
        }
    }

    // send the load patterns
    LoadPattern *theLP;
    LoadPatternIter &theLPs = this->getLoadPatterns();

    while ( ( theLP = theLPs() ) != 0 )
    {
        if ( theLP->sendSelf( commitTag, theChannel ) < 0 )
        {
            cerr << "Domain::send - LoadPattern with tag " << theLP->getTag() << " failed in sendSelf\n";
            return -11;
        }
    }

    // if get here we were successfull
    return commitTag;


}


int
Domain::receiveSelf( int cTag, Channel & theChannel, FEM_ObjectBroker & theBroker )
{
    // set the commitTag in the domain to cTag & update the getTag if needed
    commitTag = cTag;
    this->hasDomainChanged();

    // first we get the data about the state of the domain for this commitTag
    ID domainData( 11 + 13);

    // if ( theChannel.receiveID( theDbTag, commitTag, domainData ) < 0 )
    if ( theChannel.receiveID( 0, 0, domainData ) < 0 )
    {
        cerr << "Domain::recv - channel failed to recv the initial ID\n";
        return -1;
    }

    // recv the time information
    Vector domainTime( 1 );

    // if ( theChannel.receiveVector( theDbTag, commitTag, domainTime ) < 0 )
    if ( theChannel.receiveVector( 0, 0, domainTime ) < 0 )
    {
        cerr << "Domain::send - channel failed to recv thetime Vector\n";
        return -1;
    }

    currentTime = domainTime( 0 );
    committedTime = currentTime;

    //
    // now if the currentGeoTag does not agree with whats in the domain
    // we must wipe everything in the domain and recreate the domain based on the info from the channel
    //

    if ( currentGeoTag == 0 || lastChannel != theChannel.getTag() || domainData( 0 ) != currentGeoTag )
    {

        lastChannel = theChannel.getTag();

        // set the currrentGeoTag
        // int geoTag = domainData( 0 );

        int i, loc;
        int numEle, numNod, numSPs, numMPs, numLPs;

        // if receiving set lastGeoSendTag to be equal to currentGeoTag
        // at time all the data was sent if not we must clear out the objects and rebuild
        // lastGeoSendTag = domainData( 0 );

        // clear out the all the components in the current domain
        this->clearAll();

        currentTime = domainTime( 0 );
        committedTime = currentTime;

        //
        // now we rebuild the nodes
        //

        // first get the information from the domainData about the nodes
        numNod = domainData( 1 );
        // dbNod = domainData( 6 );

        if ( numNod != 0 )
        {
            ID nodeData( 2 * numNod );

            // now receive the ID about the nodes, class tag and dbTags
            // if ( theChannel.receiveID( dbNod, geoTag, nodeData ) < 0 )
            if ( theChannel.receiveID( 0, 0, nodeData ) < 0 )
            {
                cerr << "Domain::recv - channel failed to recv the node ID\n";
                return -2;
            }

            // now for each node we 1) get a new node of the correct type from the ObjectBroker
            // 2) ensure the node exists and set it's dbTag, 3) we invoke receiveSelf on this new
            // blank node and 4) add this node to the domain
            loc = 0;

            for ( i = 0; i < numNod; i++ )
            {
                int classTag = nodeData( loc );
                int dbTag = nodeData( loc + 1 );

                Node *theNode = theBroker.getNewNode( classTag );

                if ( theNode == 0 )
                {
                    cerr << "Domain::recv - cannot create node with classTag " << classTag << endl;
                    return -2;
                }

                theNode->setDbTag( dbTag );

                // if ( theNode->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                if ( theNode->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                {
                    cerr << "Domain::recv - node with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if ( this->addNode( theNode ) == false )
                {
                    cerr << "Domain::recv - could not add node with tag " << theNode->getTag() << " into domain\n!";
                    return -3;
                }

                loc += 2;
            }
        }

        //
        // now we rebuild the elements .. same as nodes above .. see above if can't understand!!
        //

        numEle = domainData( 2 );
        // dbEle = domainData( 7 );

        if ( numEle != 0 )
        {
            ID eleData( 2 * numEle );

            // if ( theChannel.receiveID( dbEle, geoTag, eleData ) < 0 )
            if ( theChannel.receiveID( 0, 0, eleData ) < 0 )
            {
                cerr << "Domain::recv - channel failed to recv the Ele ID\n";
                return -2;
            }

            loc = 0;

            for ( i = 0; i < numEle; i++ )
            {
                int classTag = eleData( loc );
                int dbTag = eleData( loc + 1 );

                Element *theEle = theBroker.getNewElement( classTag );

                if ( theEle == 0 )
                {
                    cerr << "Domain::recv - cannot create element with classTag " << classTag << endl;
                    return -2;
                }

                theEle->setDbTag( dbTag );

                if ( theEle->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                {
                    cerr << "Domain::recv - Ele with dbTag " << dbTag << " failed in receiveSelf()\n";
                    return -2;
                }

                if ( this->addElement( theEle ) == false )
                {
                    cerr << "Domain::recv - could not add Ele with tag " << theEle->getTag() << " into domain!\n";
                    return -3;
                }

                loc += 2;
            }
        }

        //
        // now we rebuild the SP_Constraints .. same as nodes above .. see above if can't understand!!
        //

        numSPs = domainData( 3 );
        // dbSPs = domainData( 8 );

        if ( numSPs != 0 )
        {
            ID spData( 2 * numSPs );

            // if ( theChannel.receiveID( dbSPs, geoTag, spData ) < 0 )
            if ( theChannel.receiveID( 0, 0, spData ) < 0 )
            {
                cerr << "Domain::recv - channel failed to recv the SP_Constraints ID\n";
                return -2;
            }

            loc = 0;

            for ( i = 0; i < numSPs; i++ )
            {
                int classTag = spData( loc );
                int dbTag = spData( loc + 1 );

                SP_Constraint *theSP = theBroker.getNewSP( classTag );

                if ( theSP == 0 )
                {
                    cerr << "Domain::recv - cannot create SP_Constraint with classTag " << classTag << endl;
                    return -2;
                }

                theSP->setDbTag( dbTag );

                if ( theSP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                {
                    cerr << "Domain::recv - SP_Constraint with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if ( this->addSP_Constraint( theSP ) == false )
                {
                    cerr << "Domain::recv - could not add SP_Constraint with tag " << theSP->getTag() << " into domain!\n";
                    return -3;
                }

                loc += 2;
            }
        }


        //
        // now we rebuild the MP_Constraints .. same as nodes above .. see above if can't understand!!
        //

        numMPs = domainData( 4 );
        // dbMPs = domainData( 9 );

        if ( numMPs != 0 )
        {
            ID mpData( 2 * numMPs );

            // if ( theChannel.receiveID( dbMPs, geoTag, mpData ) < 0 )
            if ( theChannel.receiveID( 0, 0, mpData ) < 0 )
            {
                cerr << "Domain::recv - channel failed to recv the MP_Constraints ID\n";
                return -2;
            }

            loc = 0;

            for ( i = 0; i < numMPs; i++ )
            {
                int classTag = mpData( loc );
                int dbTag = mpData( loc + 1 );

                MP_Constraint *theMP = theBroker.getNewMP( classTag );

                if ( theMP == 0 )
                {
                    cerr << "Domain::recv - cannot create MP_Constraint with classTag " << classTag << endl;
                    return -2;
                }

                theMP->setDbTag( dbTag );

                if ( theMP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                {
                    cerr << "Domain::recv - MP_Constraint with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if ( this->addMP_Constraint( theMP ) == false )
                {
                    cerr << "Domain::recv - could not add MP_Constraint with tag " << theMP->getTag() << " into domain!\n";
                    return -3;
                }

                loc += 2;
            }
        }

        //
        // now we rebuild the LoadPatterns .. same as nodes above .. see above if can't understand!!
        //

        numLPs = domainData( 5 );
        // dbLPs = domainData( 10 );

        if ( numLPs != 0 )
        {
            ID lpData( 2 * numLPs );

            // if ( theChannel.receiveID( dbLPs, geoTag, lpData ) < 0 )
            if ( theChannel.receiveID( 0, 0, lpData ) < 0 )
            {
                cerr << "Domain::recv - channel failed to recv the MP_Constraints ID\n";
                return -2;
            }

            loc = 0;

            for ( i = 0; i < numLPs; i++ )
            {
                int classTag = lpData( loc );
                int dbTag = lpData( loc + 1 );

                LoadPattern *theLP = theBroker.getNewLoadPattern( classTag );

                if ( theLP == 0 )
                {
                    cerr << "Domain::recv - cannot create MP_Constraint with classTag  " << classTag << endl;
                    return -2;
                }

                theLP->setDbTag( dbTag );

                if ( theLP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
                {
                    cerr << "Domain::recv - LoadPattern with dbTag " << dbTag << " failed in receiveSelf\n";
                    return -2;
                }

                if ( this->addLoadPattern( theLP ) == false )
                {
                    cerr << "Domain::recv - could not add LoadPattern with tag " << theLP->getTag() <<  " into the Domain\n";
                    return -3;
                }

                loc += 2;
            }
        }

        // set the currentGeoTag & mark domainChangeFlag as false
        // this way if restoring froma a database and domain has not changed for the analysis
        // the analysis will not have to to do a domainChanged() operation
        currentGeoTag = domainData( 0 );

        // lastGeoSendTag = currentGeoTag;
        hasDomainChangedFlag = false;


    }
    else
    {

        // in this block .. we have the components they just have to recv themselves again

        Node *theNode;
        NodeIter &theNodes = this->getNodes();

        while ( ( theNode = theNodes() ) != 0 )
        {
            if ( theNode->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
            {
                cerr << "Domain::recv - node with tag " << theNode->getTag() << " failed in receiveSelf\n";
                return -7;
            }
        }

        Element *theEle;
        ElementIter &theElements = this->getElements();

        while ( ( theEle = theElements() ) != 0 )
        {
            if ( theEle->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
            {
                cerr << "Domain::recv - element with tag " << theEle->getTag() <<  " failed in receiveSelf\n";
                return -8;
            }

            theEle->update();
        }

        SP_Constraint *theSP;
        SP_ConstraintIter &theSPs = this->getSPs();

        while ( ( theSP = theSPs() ) != 0 )
        {
            if ( theSP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
            {
                cerr << "Domain::recv - SP_Constraint with tag " << theSP->getTag() << " failed in receiveSelf\n";
                return -9;
            }
        }

        MP_Constraint *theMP;
        MP_ConstraintIter &theMPs = this->getMPs();

        while ( ( theMP = theMPs() ) != 0 )
        {
            if ( theMP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
            {
                cerr << "Domain::recv - MP_Constraint with tag " << theMP->getTag() << " failed in receiveSelf\n";
                return -10;
            }
        }

        LoadPattern *theLP;
        LoadPatternIter &theLPs = this->getLoadPatterns();

        while ( ( theLP = theLPs() ) != 0 )
        {
            if ( theLP->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
            {
                cerr << "Domain::recv - LoadPattern with tag" << theLP->getTag() << " failed in receiveSelf";
                return -11;
            }
        }
    }

    // now set the domains lastGeoSendTag and currentDomainChangedFlag
    // lastGeoSendTag = currentGeoTag;


    //Max tags (jose added)
    maxElementsTag           = domainData( 11 );
    maxNodesTag              = domainData( 12 );
    maxUniaxialMaterialsTag  = domainData( 13 );
    maxNDMaterialsTag        = domainData( 14 );
    maxNDMaterialLTsTag      = domainData( 15 );
    maxSectionsTag           = domainData( 16 );
    maxSectionRepresentsTag  = domainData( 17 );
    maxMultipleSupportsTag   = domainData( 18 );
    maxAccelerationFieldsTag = domainData( 19 );
    maxDampingsTag           = domainData( 20 );
    maxSPsTag                = domainData( 21 );
    maxMPsTag                = domainData( 22 );
    maxLoadPatternsTag       = domainData( 23 );


    // if get here we were successfull
    return 0;
}


bool
Domain::recvNode( Node * thePtr )
{
    // create a dummy Node & try adding it to the nodes
    Node *newDummy = new Node( *thePtr, false );

    if ( newDummy == 0 )
    {
        cerr << "Domain::recvNode - failed to copy Node " << thePtr->getTag() << "\n";
        return false;
    }

    bool result = this->addNode( newDummy );

    return result;

}



bool
Domain::hasInternalNode( int nodeTag )
{
    return false;
}



int
Domain::calculateNodalReactions( int flag )
{
    Node *theNode;
    Element *theElement;

    NodeIter &theNodes = this->getNodes();

    while ( ( theNode = theNodes() ) != 0 )
    {
        theNode->resetReactionForce( flag );
    }

    ElementIter &theElements = this->getElements();

    while ( ( theElement = theElements() ) != 0 )
    {
        theElement->addResistingForceToNodalReaction( flag );
    }

    return 0;
}

// Nima Tafazzoli added for eigen analysis, June 2012
int
Domain::eigenAnalysis(int numodes)
{
    return 0;
}

/*********************************************************************************
* sumeet 1st August, 2016
* Outputs the eigen_value analysis to hdf5 outpout file.
*********************************************************************************/ 
int Domain::Output_Eigen_Analysis()
{

    Node *nodePtr;
    int noutputs;
    int pos=0;

    for(int i =0 ; i<=maxNodesTag; i++){
        nodePtr = this->getNode(i);
        if(nodePtr){ 

            noutputs = nodePtr->getNumberDOF();
            Matrix matrix = nodePtr->getEigenvectors();

            int n = noutputs * number_of_eigen_modes;
            float data[n];

            for(int j =0; j<number_of_eigen_modes; j++){
                for(int k=0; k<noutputs; k++){
                    data[k*number_of_eigen_modes+j] = matrix(k,j);
                }
            }
            theOutputWriter.writeEigenModes(data,pos, noutputs);
            pos = pos + noutputs;
        }
    }


    const Vector &eigen_values = this->getEigenvalues();
    vector<float>  periodvalues(number_of_eigen_modes);
    vector<float>  frequencyvalues(number_of_eigen_modes);
    vector<float>  eigenvalues(number_of_eigen_modes);

    double Pi = 2 * asin(1.0);

    for (int i = 0; i < number_of_eigen_modes; i++)
    {
        double sqrtEigen = sqrt(eigen_values(i));
        eigenvalues    [i]    = eigen_values(i);
        periodvalues   [i]    = 2 * Pi / sqrtEigen;
        frequencyvalues[i]    = sqrtEigen / (2 * Pi);
    }

    theOutputWriter.writeEigen_Value_Frequency_Period ( eigenvalues, periodvalues, frequencyvalues);

    this->number_of_eigen_modes=-1;
    return 0;
}



// *******************************************************************************
// Nima Tafazzoli added for checking the mesh, September 2012

int Domain::CheckMesh( const char *check_mesh_file )
{

    // ************************** opening `the file ***********************
    cout << "================================\n";
    cout << "Beging mesh check procedure\n";
    cout << "================================\n";

    ofstream checkmesh_file;
    checkmesh_file.open( check_mesh_file );


    if ( checkmesh_file.bad() )
    {
        cerr << "WARNING - Domain::CheckMesh()";
        cerr << " - could not open file " << check_mesh_file << endl;
        exit( 2 );
    }



    cout << "Checking elements....\n";
    // ************************** checking elements ***********************

    Element *theElement;

    ElementIter &theElements = this->getElements();
    int count = 0;
    while ( ( theElement = theElements() ) != 0 )
    {
        theElement->CheckMesh( checkmesh_file );
        count ++;
        cout << "Verified " << count << " elements.\n";
    }


    // ************************** checking nodes ***********************

    Node *theNode;

    NodeIter &theNodes = this->getNodes();

    cout << "Checking nodes....\n";
    count = 0;
    while ( ( theNode = theNodes() ) != 0 )
    {
        theNode->CheckMesh( checkmesh_file );
        count ++;
    }
    cout << "Verified " << count << " nodes.\n";


    // ******************** checking uniaxial constitutive models *****************

    UniaxialMaterial *theUniaxialMaterial;

    UniaxialMaterialIter &theUniaxialMaterials = this->getUniaxialMaterials();

    cout << "Checking uniaxial materials....\n";
    count = 0;
    while ( ( theUniaxialMaterial = theUniaxialMaterials() ) != 0 )
    {
        theUniaxialMaterial->CheckMesh( checkmesh_file );
        count ++;
    }
    cout << "Verified " << count << " uniaxial materials.\n";


    // ******************** checking  constitutive models *****************

    NDMaterial *theNDMaterial;

    NDMaterialIter &theNDMaterials = this->getNDMaterials();

    cout << "Checking NDMaterials....\n";
    count = 0;
    while ( ( theNDMaterial = theNDMaterials() ) != 0 )
    {
        theNDMaterial->CheckMesh( checkmesh_file );
        count ++;
    }
    cout << "Verified " << count << " uniaxial NDMaterials.\n";

    NDMaterialLT *theNDMaterialLT;

    NDMaterialLTIter &theNDMaterialLTs = this->getNDMaterialLTs();
    cout << "Checking NDMaterialLTs....\n";
    count = 0;
    while ( ( theNDMaterialLT = theNDMaterialLTs() ) != 0 )
    {
        theNDMaterialLT->CheckMesh( checkmesh_file );
        count ++;
    }
    cout << "Verified " << count << " uniaxial NDMaterialLTs.\n";



    // ****************************************************************************

    checkmesh_file.close();


    cout << " Outputting mesh. \n";
    this->commit();

    cout << "\n\nDone checking mesh \n";
    cout << "Output written to:" << check_mesh_file << endl;
    cout << "================================\n";

    return 0;

}






// *******************************************************************************
// Jose Abell for saving results via HDF5

// int
// Domain::setHDF5_Channel(std::string filename_in,
//                         std::string model_name_in,
//                         std::string stage_name_in,
//                         int nsteps)
// {
//     theHDF5_Channel.initialize(filename_in, model_name_in, stage_name_in, nsteps);
//     return 0;
// }


int
Domain::setOutputWriter(std::string filename_in,
                        std::string model_name_in,
                        std::string stage_name_in,
                        int nsteps)
{
    theOutputWriter.initialize(filename_in, model_name_in, stage_name_in, nsteps);
    have_written_static_mesh_data = false;
    return 0;
}


int
Domain::setNumberOfOutputSteps(int nsteps)
{
    // cout << "setNumberOfOutputSteps ->  nsteps / output_every_nsteps = " << nsteps / output_every_nsteps << endl;
    theOutputWriter.set_number_of_time_steps( nsteps / output_every_nsteps);
    return 0;
}



int Domain::enableOutput(bool is_output_enabled_)
{
    output_is_enabled =  is_output_enabled_;
    have_written_static_mesh_data = false;
    return 0;
}


int Domain::enableElementOutput(bool is_element_output_is_enabled)
{
    element_output_is_enabled =  is_element_output_is_enabled;
    theOutputWriter.set_flag_write_element_output(is_element_output_is_enabled);
    have_written_static_mesh_data = false;
    return 0;
}


int Domain::setOutputEveryNsteps(int nsteps)
{
    output_every_nsteps = nsteps;
    countdown_til_output = 0;

    return 0;
}


int  Domain::getMaxElementsTag()
{
    return maxElementsTag;
}

int  Domain::getMaxNodesTag()
{
    return maxNodesTag;
}

int  Domain::getMaxUniaxialMaterialsTag()
{
    return maxUniaxialMaterialsTag;
}

int  Domain::getMaxNDMaterialsTag()
{
    return maxNDMaterialsTag;
}

int  Domain::getMaxNDMaterialLTsTag()
{
    return maxNDMaterialLTsTag;
}

int  Domain::getMaxSectionsTag()
{
    return maxSectionsTag;
}

int  Domain::getMaxSectionRepresentsTag()
{
    return maxSectionRepresentsTag;
}

int  Domain::getMaxMultipleSupportsTag()
{
    return maxMultipleSupportsTag;
}

int  Domain::getMaxAccelerationFieldsTag()
{
    return maxAccelerationFieldsTag;
}

int  Domain::getMaxDampingsTag()
{
    return maxDampingsTag;
}

int  Domain::getMaxSPsTag()
{
    return maxSPsTag;
}

int  Domain::getMaxMPsTag()
{
    return maxMPsTag;
}

int  Domain::getMaxLoadPatternsTag()
{
    return maxLoadPatternsTag;
}

int Domain::setOutputCompressionLevel(int level)
{
    theOutputWriter.set_zlib_compression_level(level);
    return 0;
}

int Domain::sendOutputOptionsToSubdomains()
{
    cerr << "Domain::sendOutputOptionsToSubdomains() - Should be done by subdomains";
    return 0;
}

int Domain::receiveOutputOptionsToSubdomains()
{
    cerr << "Domain::receiveOutputOptionsToSubdomains() - Should be done by subdomains";
    return 0;
}

int  Domain::zeroDisplacements()
{
    Node *nodePtr;
    NodeIter &theNodeIter = this->getNodes();

    while ( ( nodePtr = theNodeIter() ) != 0 )
    {
        nodePtr->zeroDisplacements();
    }

    return 0;
}

int Domain::setConstitutiveIntegrationMethod(int algorithm,
        double f_relative_tol, double stress_relative_tol, int n_max_iterations)
{
    int errorflag = -1;


// #ifdef _PARALLEL_PROCESSING
//     int rank = 0;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     cout << "setConstitutiveIntegrationMethod(" << rank << ") - \n";
//     cout << "algorithm = " << algorithm << endl;
//     cout << "f_relative_tol = " << f_relative_tol << endl;
//     cout << "stress_relative_tol = " << stress_relative_tol << endl;
//     cout << "n_max_iterations = " << n_max_iterations << endl;
// #endif


    if ((errorflag = NDMaterialLT::set_constitutive_integration_method(algorithm, f_relative_tol, stress_relative_tol, n_max_iterations)))
    {
        errorflag = 0;
    }
    else
    {
        cerr << "define_NDMaterialLT_constitutive_integration_algorithm() - Error. Constitutive algorithm not available.\n";
    }
    return errorflag;
}


void Domain::removeStrainFromElement(int tag)
{
    Element* ele = this->getElement(tag);
    if (ele != NULL)
    {
        ele->zeroStrain();
    }
    else
    {
        cerr << "Domain::removeStrainFromElement - element tag = " <<  tag << " not found!\n";
    }
    return;
}

void Domain::removeDisplacementFromNode(int tag)
{
    // cout << "!";
    Node* node = this->getNode(tag);
    if (node != NULL)
    {
        node->zeroDisplacements();
    }
    else
    {
        cerr << "Domain::removeDisplacementFromNode - node tag = " <<  tag << " not found!\n";
    }
    return;
}

/*************************************************************************************************
* Added by Sumeet 30th August 2016
* This function basically appendds the proces id information to Element_Partition 
* vector in H5OutputWriter. only for static Decomposition
***************************************************************************************************/
void Domain::add_Element_Partition_Info(int tag, int Partition_no){ // Added by [Sumeet August,2016]

   theOutputWriter.writeElementPartitionData(tag,Partition_no);

   return;

}

/*************************************************************************************************
* Added by Sumeet 30th August 2016
* This function basically appendds the procces id information to Node_Partition 
* vector in H5OutputWriter. only for static Decomposition
***************************************************************************************************/
void Domain::add_Node_Partition_Info(int tag, int Partition_no){    // Added by [Sumeet August,2016]

    theOutputWriter.writeNodePartitionData(tag,Partition_no);

    return;

}