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


// Style guide. ---
// * Use CamelCase for member functions, constructors, destructors and class names have intial upper case
//   letter and other functions have lower-case initial letter.
// * Use underscores and lower cases for variable names (e.g yield_function_value)
//      Exception:  when variables are named consistently with theory (e.g. K for striffness matrix, etc.)
// * i,j,k,l,m,n,p,q are reserved for indexes into arrays (iterating over components)
//     -Note: the type of these can be integer or other fancy types, e.g. when using LTensor class
//             to store tensors you can use i for the indexing class objects.
// * Use "Allman" code formatting style. Better yet, use an auto code formatter like AStyle (http://astyle.sourceforge.net/)
//   there are many plugins which interface this into editors (SublimeAStyleFormatter for sublime text editor)
// * Use the most recent C++ standard supported by your compiler. Code should compile without
//   warnings using the -Wall flag or equivalent. In case this cannot be done, comment on why.
// * Delay allocating resources as much as possible (at least until setDomain function gets called).
// * Clean up resources as soon as possible.
// * Think parallel. Have to implement sendSelf() and receiveSelf() functions.
//
// 2015 Jose Abell - UCD CompGeoMech

#include <NewElementTemplate.h>
#include <classTags.h> // Must define the class tag for the new element in this file. 






//==================================================================================================
// Constructor. Receive all input parameters. Should not allocate resources!
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
NewElementTemplate::NewElementTemplate(int tag, int node1, .....):
    Element(tag, ELE_TAG_NewElementTemplate),   //ATTENTION! Define the class tag in classTags.h with provided encoding formula [Sumeet,2016] 
    // add more initializers
{
    this->setMaterialTag(material->getTag());   // for setting the material id to the element

    you must implement
}



//==================================================================================================
// Empty constructor.  Create an empty element (with possibly a bad state)
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
NewElementTemplate::NewElementTemplate():
    Element(0, ELE_TAG_NewElementTemplate), //ATTENTION! Define the class tag in classTags.h with provided encoding formula [Sumeet,2016]
    // add more initializers setting internal variables to null values
{
    you must implement
}



//==================================================================================================
// Destructor. Deallocate resources used by element.
//   * Input: void
//   * Output: void
NewElementTemplate::~NewElementTemplate()
{
    you must implement
}



//==================================================================================================
// returns the number of nodes of the element.
//   * Input: void
//   * Output: number of nodes
int NewElementTemplate::getNumExternalNodes(void) const
{
    you must implement
    return number_of_nodes;
}



//==================================================================================================
// Return an ID (integer vector) with the external nodes
//   * Input: void
//   * Output: ID with tags of external nodes
const ID &NewElementTemplate::getExternalNodes(void)
{
    you must implement
    return external_nodes;
}



//==================================================================================================
// Return pointer array to the nodes
//   * Input: void
//   * Output: node pointer array.
Node **NewElementTemplate::getNodePtrs(void)
{
    you must implement
    return nodes;
}



//==================================================================================================
//Return the number of dofs in the element.
//   * Input: void
//   * Output: number of dofs (sum of dofs over all of element's nodes)
int NewElementTemplate::getNumDOF(void)
{
    you must implement
}



//==================================================================================================
// Receives a domain pointer, and sets the local domain pointer through calling the base class setDomain.
// At this point the domain is defined and set, one can allocate resources (get nodal pointers, compute
// some internal variables like lengths, volumes, etc. ).
// Usually we'll set the node pointers here (will be needed for getNodePtrs function).
// Also, we'll check that the given nodes are defined (you get a valid pointer to them) and
// that they have the right number of DOFS (implementation specific)
//   * Input: domain pointer (see Domain.h)
//   * Output: void
void NewElementTemplate::setDomain(Domain *theDomain)
{
    // check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        //set node pointers to null
    }
    else
    {
        //Use the domain to set the node pointers...
        //nodePointers[0] = theDomain->getNode(Nd1);
        //nodePointers[1] = theDomain->getNode(Nd2);
        // Check the pointers...
        // if (nodePointers == 0)
        // {
        //     bad error, usually means node was never
        //  return;
        // }
        // Check the number of DOFs
        // if(nodePointers[0]->getNumberDOF() != MY_NUMBER_OF_DOFS)
        // {
        //     print a tantrum
        //     return;
        // }
        //
        // More checks maybe
        //
        // Set the base class domain pointer
        
        this->DomainComponent::setDomain(theDomain);
    }

//Additionally one can allocate resources at this point.
you must implement

}




//==================================================================================================
// Accept current state of the element and save it. (If applicable)
// I this is a gauss-point based element, one calls commitState on
// the material pointers (Gauss points) owned by this element.
// return 0 if success.
//   * Input: void
//   * Output: error flag, 0 if success
int NewElementTemplate::commitState(void)
{
    you must implement
    return 0;
}




//==================================================================================================
// Revert the state of the element to the last committed state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int NewElementTemplate::revertToLastCommit(void)
{
    you must implement
    return 0;
}




//==================================================================================================
// Revert the state of the element to the initial state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int NewElementTemplate::revertToStart(void)
{
    you must implement
    return 0;
}




//==================================================================================================
// Update the state of the element. I.E. compute new tangent stiffness,
// compute new resisting force, advance state variables.
// These changes should not be permanent until commit function is called.
//   * Input: void
//   * Output: error flag, 0 if success
int NewElementTemplate::update(void)
{
    you must implement
    return 0;
}




//==================================================================================================
//  (optionl) Set the elemental load to zero.
//   * Input: void
//   * Output: void
void NewElementTemplate::zeroLoad(void)
{
    // optional to implement
    return 0;
}





//==================================================================================================
// (optionl) Add a new elemental load. This will modify the resisting force vector.
//   * Input: ElementalLoad pointer and a loadFactor.
//   * Output: error flag, 0 if success
//  Notes:
//   * ElementalLoads have a type interger (a tag defined elsewhere) and a Vector (array
//     of doubles) with data. Use these to generate the elemental load scaled by the
//    load factor (which is also the time-step of the analysis).
int NewElementTemplate::addLoad(ElementalLoad *theLoad, double loadFactor)
{
    // optional to implement
    //
    // Some code to get the type and data. Example is for self_weight.
    // int type;
    // const Vector &data = theLoad->getData(type, loadFactor);
    //
    // if (type == LOAD_TAG_ElementSelfWeight)  // Load tags are defined in classTags.h
    // {
    //     do something, like add a the forces to the resisting_force vector.
    // }
    return 0;
}






//==================================================================================================
//  Add intertial terms to resisting force vector.
//   * Input: A vector with nodal accelerations???
//   * Output: error flag, 0 if success
//  Notes: use node pointers to get accelerations from nodes,
// form an acceleration vector and multiply this with the mass matrix, then
// add this into the load unbalance (with negative sign, cause it is inertia)
int NewElementTemplate::addInertiaLoadToUnbalance(const Vector &accel)
{
    you must implement
    return 0;
}






//==================================================================================================
// Functions to obtain stiffness, mass, damping and residual information
//   * Input: void
//   * Output: reference to tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = NewElementTemplate::getNumDOF();
//   Pro tip. If this matrix computes the tangent stiffness, then
//   it can be stored as a static member variable so that all elements share
//   the same memory space (each element overwrites the tangent). This saves
//   memory.
const Matrix &NewElementTemplate::getTangentStiff(void)
{
    you must implement
    return K;
}





//==================================================================================================
// Functions to obtain initial stiffness
//   * Input: void
//   * Output: reference to initial tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = NewElementTemplate::getNumDOF();
const Matrix &NewElementTemplate::getInitialStiff(void)
{
    you must implement
    return *K;  // --suggested variable name for stiffness :). Will be a pointer, so that it can be after construction.
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = NewElementTemplate::getNumDOF();
const Matrix &NewElementTemplate::getDamp(void)
{
    // optional to you must implement
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = NewElementTemplate::getNumDOF();
const Matrix &NewElementTemplate::getMass(void)
{
    // optional to implement
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = NewElementTemplate::getNumDOF();
const Vector &NewElementTemplate::getResistingForce(void)
{
    you must implement
}






//==================================================================================================
// (optional) Add inertial terms to resisting force.
//   * Input: void
//   * Output: Vector of doubles with new resisting force.
//   Note: Regularly, this function calls getResistingForce() and then
//         adds inertial terms.
const Vector &NewElementTemplate::getResistingForceIncInertia(void)
{
    // (optional to implement)
}




//==================================================================================================
// (optional, a must if you want to do parallel processing)
//  Send all state data of the element through a channel (usually an MPI_Channel)
//   * Input: a reference to the Channel to use.
//   * Output: error flag, 0 if success
// Note: This function is usually very involved, and should do a lot of checking
// for pointers and for success of the send.
// Note2: setDomain(...) *might* not be called before using this function.
int NewElementTemplate::sendSelf(int commitTag, Channel &theChannel)
{
    // Useful constructs
    // int error_flag;
    // error_flag = theChannel.sendVector(0, 0, double_data);  // the first two parameters are deprecated
    //
    //  Check that error_flag is not < 0
    //
    // theChannel.sendID(0, 0, integer_data);  //  the first two parameters are deprecated
    //
    //  Check that error_flag is not < 0
    you must implement
    return 0;
}




//==================================================================================================
// (optional, a must if you want to do parallel processing)
//  Receive all state data of the element through a channel (usually an MPI_Channel)
//  this data comes from an element that is calling sendSelf in some other process.
//   * Input: a reference to the Channel to use.
//   * Output: error flag, 0 if success
// Note: This function is called after setDomain() so all resources should be made available.
int NewElementTemplate::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // Useful constructs
    // int error_flag;
    // theChannel.receiveVector(0, 0, double_data);  // the first two parameters are deprecated
    //  Check that error_flag is not < 0
    // theChannel.receiveID(0, 0, integer_data);  //  the first two parameters are deprecated
    //  Check that error_flag is not < 0

    you must implement
    return 0;
}



//==================================================================================================
// Print out element info
//   * Input: an ostream to print stuff into, and a flag
//   * Output: void
// Print stuff into the ostream by using the "<<" operator.
// The flag can be used to request different levels of printing, ie.
// a flag of 0 might be very basic information, while flag > 0 might
// give increasing ammount of info.
void NewElementTemplate::Print(ostream &s, int flag = 0)
{
    you must implement
}





//==================================================================================================
// Check element correctness
//   * Input: an ostream to print stuff into (print details of what is being checked here.)
//   * Output: an error flag (<0 if element is not right in some way)
//  Note: be verbose print element tag, etc. Print out only if an error is encountered.
int NewElementTemplate::CheckMesh(ofstream &)
{
    you must implement
}







//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: number of double outputs for the element (size of the output vector)
int NewElementTemplate::getOutputSize() const
{
    you must implement
}





//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: Vector (array of doubles) with the element output.
const vector<float> &NewElementTemplate::getElementOutput()
{
    you must implement
}





//==================================================================================================
// Return a Matrix with the coordinates of Gauss points (or points where outputs are generated, could be the endpoints of a beam)
//   * Input: void
//   * Output: Matrix (array of doubles) with the gauss coordinates
//  Note:  Format is
//    gauss_coordinates[0,:] = [x_0,y_0,z_0]  -- Coordinates of first Gauss point
//    gauss_coordinates[1,:] = [x_1,y_1,z_1]  -- Coordinates of second Gauss point
//     ...
//    gauss_coordinates[Ngauss,:] = [x_Ngauss,y_Ngauss,z_Ngauss]  -- Coordinates of Ngauss-th Gauss point
Matrix &NewElementTemplate::getGaussCoordinates(void)
{
    you must implement
}





//==================================================================================================
// Add you own member functions at the end!