///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:
// DATE:
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
//      Exception:  when variables are named consistently with theory (e.g. K for striffness matrix, etc.)
// * Use underscores for variable names (e.g yield_function_value)
// * i,j,k,l,m,n,p,q are reserved for indexes into arrays (iterating over components)
//     -Note1: the type of these can be integer or other fancy types, ie. when using LTensor class
//             to store tensors.
// * Use "Allman" code styling. Use a code formatter like AStyle (http://astyle.sourceforge.net/)
//   there are many plugins which interface this into editors (SublimeAStyleFormatter for sublime text editor)
// * Use the most recent C++ standard supported by your compiler. Code should compile without
//   warnings using the -Wall flag or equivalent. In case this cannot be done, comment on why.
// * Delay allocating resources as much as possible (at least until setDomain function gets called).
// * Clean up resources as soon as possible.
// * Think parallel. Have to implement sendSelf() and receiveSelf() functions.
//
// 2015 Jose Abell - UCD CompGeoMech


// (!) More instructions and details on the implementation file NewElementTemplate.cpp

// Rename the header guard
#ifndef NewElementTemplate_h
#define NewElementTemplate_h


#include <Element.h>
#include <Matrix.h>
#include <Vector.h>


class Node;
class Channel;


class NewElementTemplate: public Element
{

public:

    // Constructor
    NewElementTemplate(int tag, int node1, .....);      //You must implement this

    // Empty constructor
    NewElementTemplate();

    //Destructor
    ~NewElementTemplate();

    // Functions to obtain information about dof & connectivity
    int getNumExternalNodes(void) const;
    const ID &getExternalNodes(void);
    Node **getNodePtrs(void);
    int getNumDOF(void);
    void setDomain(Domain *theDomain);


    // Functions to set the state of the element
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);
    int update(void);
    void zeroLoad(void);
    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);


    // Functions to obtain stiffness, mass, damping and residual information
    const Matrix &getTangentStiff(void);
    const Matrix &getInitialStiff(void);
    const Matrix &getDamp(void);
    const Matrix &getMass(void);
    const Vector &getResistingForce(void);
    const Vector &getResistingForceIncInertia(void);


    // Implement for parallel processing
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    // Print out element info
    void Print(ostream &s, int flag = 0);

    // Check element correctness
    int CheckMesh(ofstream &);

    // Give the element a name
    std::string getElementName() const
    {
        return "NewElementTemplate";
    }

    /*************************************************************************
    * Sumeet August, 2016
    * No. of ElementOutputs should be as per the Element_Class_tag_Desc 
    * See the classtags.h for more decription of encoding of 
    * Class_tag Descriptions.
    * For Optimization all the information about elements are encoded
    * in the Element_Class_tag Description 
    * NOTE:- Must Obey the Element_Class_Description [see classTags.h]
    **************************************************************************/
    const vector<float> &getElementOutput() ; // Declare if there is element output except at gauss points
    const vector<float> &getGaussOutput();    // Declare only if there is any gauss point and there is 18 outputs per gauss points i.e stress, strain and plastic strain
    Matrix &getGaussCoordinates(void);



protected:
    //Implementation-specific member functions...
    // Should be protected, because they're not going to be called
    // from outside the class, but you might want to inherit them




private:
    // All data must be private. Provide setter and getter methods if
    // this class interacts with other classes.

    static vector<float> Element_Output_Vector() ; // Declare if there is element output except at gauss points
    static vector<float> Gauss_Output_Vector();    // Declare only if there is any gauss point and there is 18 outputs per gauss points i.e stress, strain and plastic strain


};

#endif











