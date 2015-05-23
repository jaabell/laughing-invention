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
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:      Jose Abell, Boris Jeremic
// DATE:            Thu 21 May 2015 10:00:12 PM PDT
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


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

#include <FrictionalPenaltyContact.h>
#include <classTags.h> // Must define the class tag for the new element in this file. 






//==================================================================================================
// Constructor. Receive all input parameters. Should not allocate resources!
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
FrictionalPenaltyContact::FrictionalPenaltyContact(int tag, int node1, int node2, double kn_, double kt_, double mu_, double e1_x, double e1_y, double e1_z):
    Element(tag, ELE_TAG_FrictionalPenaltyContact),
    kn(kn_),
    kt(kt_),
    mu(mu_),
    is_in_contact(false),
    is_in_contact_prev(false),
    B(3, 6),
    external_nodes(2)
{
    d_ij0 = 0;
    d_ij0_prev = 0;
    tA = 0;
    tC = 0;
    R = 0;
    g = 0;
    g_prev = 0;
    C = 0;
    external_nodes(0) = node1;
    external_nodes(1) = node2;
    nodes[0] = 0;
    nodes[1] = 0;


    // B matrix is initialized now .
    // This is to avoid storing the normal vector's components apart from B matrix
    // which is redundant, 'cause the normal vector components are the elements
    // B[0,3], B[0,4], B[0,5] of the B matrix.

    Vector x_local(3), y_local(3), z_local(3);
    double e1_norm = sqrt(e1_x * e1_x + e1_y * e1_y + e1_z * e1_z );
    z_local(0) = e1_x / e1_norm;
    z_local(1) = e1_y / e1_norm;
    z_local(2) = e1_z / e1_norm;

    if ((abs(z_local(0)) < 1e-5) && (abs(z_local(1)) < 1e-5))  // Element is vertical, essentially
    {
        y_local(0) =  0.0;
        y_local(1) =  1.0;
        y_local(2) =  0.0;

        x_local(0) = y_local(1) * z_local(2) - y_local(2) * z_local(1);
        x_local(1) = y_local(2) * z_local(0) - y_local(0) * z_local(2);
        x_local(2) = y_local(0) * z_local(1) - y_local(1) * z_local(0);
    }
    else
    {
        // creating a temporary vector (global y axis)
        Vector temp_vector(3);
        temp_vector(0) =  0.0;
        temp_vector(1) =  0.0;
        temp_vector(2) =  1.0;

        // creating the z_local axis from cross product of z_local and temp_vector
        // which will be perpendicular to x_local
        x_local(0) = z_local(1) * temp_vector(2) - z_local(2) * temp_vector(1);
        x_local(1) = z_local(2) * temp_vector(0) - z_local(0) * temp_vector(2);
        x_local(2) = z_local(0) * temp_vector(1) - z_local(1) * temp_vector(0);

        // creating the y_local axis from cross product of x_local and z_local
        // which will be perpendicular to x_local and z_local
        y_local(0) = z_local(1) * x_local(2) - z_local(2) * x_local(1);
        y_local(1) = z_local(2) * x_local(0) - z_local(0) * x_local(2);
        y_local(2) = z_local(0) * x_local(1) - z_local(1) * x_local(0);
    }

    // compute length (norm) of vectors
    double x_local_norm = sqrt(x_local(0) * x_local(0) + x_local(1) * x_local(1) + x_local(2) * x_local(2));
    double y_local_norm = sqrt(y_local(0) * y_local(0) + y_local(1) * y_local(1) + y_local(2) * y_local(2));
    double z_local_norm = sqrt(z_local(0) * z_local(0) + z_local(1) * z_local(1) + z_local(2) * z_local(2));

    // find the normalized local vectors
    for (int i = 0; i < 3; i++)
    {
        x_local(i) = x_local(i) / x_local_norm;
        y_local(i) = y_local(i) / y_local_norm;
        z_local(i) = z_local(i) / z_local_norm;

        B(0, i) = -x_local(i);
        B(1, i) = -y_local(i);
        B(2, i) = -z_local(i);
        B(0, i + 3) =  x_local(i);
        B(1, i + 3) =  y_local(i);
        B(2, i + 3) =  z_local(i);
    }
}



//==================================================================================================
// Empty constructor.  Create an empty element (with possibly a bad state)
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
FrictionalPenaltyContact::FrictionalPenaltyContact():
    Element(0, ELE_TAG_FrictionalPenaltyContact), //ATTENTION! Define the class tag in classTags.h
    kn(0.0),
    kt(0.0),
    mu(0.0),
    is_in_contact(false),
    is_in_contact_prev(false),
    B(3, 6),
    external_nodes(2)
{

    d_ij0 = 0;
    d_ij0_prev = 0;
    tA = 0;
    tC = 0;
    R = 0;
    g = 0;
    g_prev = 0;
    C = 0;
    external_nodes(0) = -1;
    external_nodes(1) = -1;
    nodes[0] = 0;
    nodes[1] = 0;
}



//==================================================================================================
// Destructor. Deallocate resources used by element.
//   * Input: void
//   * Output: void
FrictionalPenaltyContact::~FrictionalPenaltyContact()
{

    if ( d_ij0 != NULL )
    {
        delete d_ij0;
        d_ij0 = NULL;
    }
    if ( d_ij0_prev != NULL )
    {
        delete d_ij0_prev;
        d_ij0_prev = NULL;
    }
    if ( tA != NULL )
    {
        delete tA;
        tA = NULL;
    }
    if ( tC != NULL )
    {
        delete tC;
        tC = NULL;
    }
    if ( R != NULL )
    {
        delete R;
        R = NULL;
    }
    if ( g != NULL )
    {
        delete g;
        g = NULL;
    }
    if ( g_prev != NULL )
    {
        delete g_prev;
        g_prev = NULL;
    }
    if ( C != NULL )
    {
        delete C;
        C = NULL;
    }
    if ( nodes != NULL )
    {
        nodes[0] = 0;
        nodes[1] = 0;
    }
}



//==================================================================================================
// returns the number of nodes of the element.
//   * Input: void
//   * Output: number of nodes
int FrictionalPenaltyContact::getNumExternalNodes(void) const
{
    // cout << "tag = " << this->getTag() << endl;
    // cout << "Number of node = 2\n";
    return 2;
}



//==================================================================================================
// Return an ID (integer vector) with the external nodes
//   * Input: void
//   * Output: ID with tags of external nodes
const ID &FrictionalPenaltyContact::getExternalNodes(void)
{
    // cout << "External nodes = " << external_nodes << endl;
    return external_nodes;
}



//==================================================================================================
// Return pointer array to the nodes
//   * Input: void
//   * Output: node pointer array.
Node **FrictionalPenaltyContact::getNodePtrs(void)
{
    return nodes;
}



//==================================================================================================
//Return the number of dofs in the element.
//   * Input: void
//   * Output: number of dofs (sum of dofs over all of element's nodes)
int FrictionalPenaltyContact::getNumDOF(void)
{
    return 6;
}



//==================================================================================================
// Receives a domain pointer, and sets the local domain pointer through calling the base class setDomain.
// At this point the domain is defined and set, one can allocate resources (get nodal pointers, compute
// some internal variables like lengths, volumes, etc. )
//   * Input: domain pointer (see Domain.h)
//   * Output: void
void FrictionalPenaltyContact::setDomain(Domain *theDomain)
{

    // Check Domain is not null - invoked when object removed from a domain
    if (theDomain == 0)
    {
        //Set domain pointers to zero
        nodes[0] = 0;
        nodes[1] = 0;
    }
    else
    {
        int Nd1 = external_nodes( 0 );
        int Nd2 = external_nodes( 1 );

        //Obtain the nodes pointers
        nodes[0] = theDomain->getNode( Nd1 );
        nodes[1] = theDomain->getNode( Nd2 );

# ifdef _PARALLEL_PROCESSING
// In parallel case, check whether its really an outside node

        if ( nodes[0] == 0 )
        {
            nodes[0] = theDomain->getOutsideNode( Nd1 );
        }
        if ( nodes[1] == 0 )
        {
            nodes[1] = theDomain->getOutsideNode( Nd2 );
        }
# endif

        //Check  whether all nodes existed (we got a valid pointer)
        if ( nodes[0] == 0 || nodes[1] == 0  )
        {
            cerr << "FATAL ERROR FrictionalPenaltyContact (tag: " << this->getTag() << "), node not found in domain\n";
            return;
        }

        //Check we're connected to nodes of the right number of dofs
        int dofNd1 = nodes[0]->getNumberDOF();
        int dofNd2 = nodes[1]->getNumberDOF();

        if ( dofNd1 != 3 || dofNd2 != 3 )
        {
            cerr << "FATAL ERROR FrictionalPenaltyContact (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            return;
        }

        // All is good, we can set the domain.
        this->DomainComponent::setDomain(theDomain);
        initialize();
    }
}




//==================================================================================================
// Accept current state of the element and save it. (If applicable)
// I this is a gauss-point based element, one calls commitState on
// the material pointers (Gauss points) owned by this element.
// return 0 if success.
//   * Input: void
//   * Output: error flag, 0 if success
int FrictionalPenaltyContact::commitState(void)
{
    *tA = *tC;
    *g_prev = *g;
    *d_ij0_prev = *d_ij0;
    is_in_contact_prev = is_in_contact;
    return 0;
}




//==================================================================================================
// Revert the state of the element to the last committed state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int FrictionalPenaltyContact::revertToLastCommit(void)
{
    *tC = *tA;
    *g = *g_prev;
    *d_ij0 = *d_ij0_prev;
    is_in_contact = is_in_contact_prev;
    // you must implement
    return 0;
}




//==================================================================================================
// Revert the state of the element to the initial state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int FrictionalPenaltyContact::revertToStart(void)
{
    // you must implement
    return 0;
}




//==================================================================================================
// Update the state of the element. I.E. compute new tangent stiffness,
// compute new resisting force, advance state variables.
// These changes should not be permanent until commit function is called.
//   * Input: void
//   * Output: error flag, 0 if success
int FrictionalPenaltyContact::update(void)
{
    computeGap();

    // cout << " B        = " << B ;
    // cout << " gap      = " << *g ;
    // cout << " gap_prev = " << *g_prev ;

    if (is_in_contact)
    {
        // cout << "In Contact!\n";
        //Set elastic tangent
        C->Zero();
        (*C)(0, 0) = kt;
        (*C)(1, 1) = kt;
        (*C)(2, 2) = kn;

        // cout << "  *C = " << *C ;
        // cout << "  *tA = " << *tA ;

        //Compute prediction force (B)
        Vector tB = *tA;
        tB.addMatrixVector(1.0, *C, *g - *g_prev, 1.0);
        // cout << "  tB = " << tB ;

        //Compute Yield function at prediction point
        Vector t_TB(2);         // Shear Force predictor
        t_TB(0) = tB(0);
        t_TB(1) = tB(1);

        double norm_t_TB = t_TB.Norm();
        Vector s_B(2);
        if (norm_t_TB > 0)
        {
            s_B = t_TB / norm_t_TB;
        }

        Vector A_B(3);
        A_B(0) = s_B(0);
        A_B(1) = s_B(1);
        A_B(2) = mu;

        double yf_B = A_B ^ tB;

        if (yf_B > 0) // Sliding
        {
            // cout << "Sliding!\n";
            Vector b_B(3);
            b_B(0) = s_B(0);
            b_B(1) = s_B(1);
            double den = A_B ^ ((*C) * b_B);
            double delta_nu = yf_B / den;

            // Compute corrected forces
            *tC = tB - delta_nu * (*C) * b_B;

            //Update local stiffness
            Matrix Celast = *C;
            const double *Cdata = Celast.getData();
            const double *bdata = b_B.getData();
            const double *adata = A_B.getData();
            for (int i = 0; i < 3; i++)
                for (int n = 0; n < 3; n++)
                    for (int j = 0; j < 3; j++)
                        for (int m = 0; m < 3; m++)
                        {
                            (*C)(i, j) =  (*C)(i, j) -  Cdata[3 * i + m] * bdata[m] * adata[n] * Cdata[3 * n + j] / den;
                        }
            // cout << "C_t = " << *C ;

        }
        else // Sticking (yf_B < 0)
        {
            // cout << "Sticking!\n";
            *tC = tB;
        }
    }
    else
    {
        // cout << "Not in contact!\n";
    }

    // cout << "  *tC = " << *tC << endl;

    return 0;
}




//==================================================================================================
//  (optionl) Set the elemental load to zero.
//   * Input: void
//   * Output: void
void FrictionalPenaltyContact::zeroLoad(void)
{
    // optional to implement
    return;
}





//==================================================================================================
// (optionl) Add a new elemental load. This will modify the resisting force vector.
//   * Input: ElementalLoad pointer and a loadFactor.
//   * Output: error flag, 0 if success
//  Notes:
//   * ElementalLoads have a type interger (a tag defined elsewhere) and a Vector (array
//     of doubles) with data. Use these to generate the elemental load scaled by the
//    load factor (which is also the time-step of the analysis).
int FrictionalPenaltyContact::addLoad(ElementalLoad *theLoad, double loadFactor)
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
int FrictionalPenaltyContact::addInertiaLoadToUnbalance(const Vector &accel)
{
    //cout << "FrictionalPenaltyContact::addInertiaLoadToUnbalance()\n";
    return 0;
}






//==================================================================================================
// Functions to obtain stiffness, mass, damping and residual information
//   * Input: void
//   * Output: reference to tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = FrictionalPenaltyContact::getNumDOF();
//   Pro tip. If this matrix computes the tangent stiffness, then
//   it can be stored as a static member variable so that all elements share
//   the same memory space (each element overwrites the tangent). This saves
//   memory.
const Matrix &FrictionalPenaltyContact::getTangentStiff(void)
{
    static Matrix K(6, 6);
    K.Zero();
    if (is_in_contact)
    {
        K.addMatrixTripleProduct(0.0, B, *C, 1.0); // B' * C * B
    }

    // cout << "K = " << K << endl;

    return K;
}





//==================================================================================================
// Functions to obtain initial stiffness
//   * Input: void
//   * Output: reference to initial tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = FrictionalPenaltyContact::getNumDOF();
const Matrix &FrictionalPenaltyContact::getInitialStiff(void)
{
    static Matrix Kinit(6, 6);
    Kinit.Zero();

    Matrix Celast(3, 3);
    Celast(0, 0) = kt;
    Celast(1, 1) = kt;
    Celast(2, 2) = kn;

    Kinit.addMatrixTripleProduct(0.0, B, Celast, 1.0); // B' * C * B

    return Kinit;
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = FrictionalPenaltyContact::getNumDOF();
const Matrix &FrictionalPenaltyContact::getDamp(void)
{
    static Matrix Czero(6, 6);
    return Czero;
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = FrictionalPenaltyContact::getNumDOF();
const Matrix &FrictionalPenaltyContact::getMass(void)
{
    static Matrix Mzero(6, 6);
    return Mzero;
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = FrictionalPenaltyContact::getNumDOF();
const Vector &FrictionalPenaltyContact::getResistingForce(void)
{
    R->Zero();
    R->addMatrixTransposeVector(0, B, *tC, 1.0);
    // cout << "  R = " << *R << endl;
    return *R;
}






//==================================================================================================
// (optional) Add inertial terms to resisting force.
//   * Input: void
//   * Output: Vector of doubles with new resisting force.
//   Note: Regularly, this function calls getResistingForce() and then
//         adds inertial terms.
const Vector &FrictionalPenaltyContact::getResistingForceIncInertia(void)
{
    return getResistingForce();
}




//==================================================================================================
// (optional, a must if you want to do parallel processing)
//  Send all state data of the element through a channel (usually an MPI_Channel)
//   * Input: a reference to the Channel to use.
//   * Output: error flag, 0 if success
// Note: This function is usually very involved, and should do a lot of checking
// for pointers and for success of the send.
// Note2: setDomain(...) *might* not be called before using this function.
int FrictionalPenaltyContact::sendSelf(int commitTag, Channel &theChannel)
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
    // you must implement

    cerr << "FrictionalPenaltyContact::sendSelf() -- NOT IMPLEMENTED!!!\n";

    return 0;
}




//==================================================================================================
// (optional, a must if you want to do parallel processing)
//  Receive all state data of the element through a channel (usually an MPI_Channel)
//  this data comes from an element that is calling sendSelf in some other process.
//   * Input: a reference to the Channel to use.
//   * Output: error flag, 0 if success
// Note: This function is called after setDomain() so all resources should be made available.
int FrictionalPenaltyContact::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    // Useful constructs
    // int error_flag;
    // theChannel.receiveVector(0, 0, double_data);  // the first two parameters are deprecated
    //  Check that error_flag is not < 0
    // theChannel.receiveID(0, 0, integer_data);  //  the first two parameters are deprecated
    //  Check that error_flag is not < 0
    cerr << "FrictionalPenaltyContact::receiveSelf() -- NOT IMPLEMENTED!!!\n";

    // you must implement
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
void FrictionalPenaltyContact::Print(ostream &s, int flag )
{
    // you must implement
    if (flag >= 0)
    {
        s << " FrictionalPenaltyContact, tag =  " << this->getTag() << endl;
    }
}





//==================================================================================================
// Check element correctness
//   * Input: an ostream to print stuff into (print details of what is being checked here.)
//   * Output: an error flag (<0 if element is not right in some way)
//  Note: be verbose print element tag, etc. Print out only if an error is encountered.
int FrictionalPenaltyContact::CheckMesh(ofstream &)
{
    return 0;
}







//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: number of double outputs for the element (size of the output vector)

#define FRICTIONALPENALTYCONTACT_NOUTPUT 6

int FrictionalPenaltyContact::getOutputSize() const
{
    return  FRICTIONALPENALTYCONTACT_NOUTPUT;
}





//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: Vector (array of doubles) with the element output.
const Vector &FrictionalPenaltyContact::getOutput() const
{
    static Vector output_vector(FRICTIONALPENALTYCONTACT_NOUTPUT);

    output_vector(0) = (*g)(0);
    output_vector(1) = (*g)(1);
    output_vector(2) = (*g)(2);
    output_vector(3) = (*tA)(0);
    output_vector(4) = (*tA)(1);
    output_vector(5) = (*tA)(2);


    return output_vector;
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
Matrix &FrictionalPenaltyContact::getGaussCoordinates(void)
{
    // you must implement
    static Matrix gauss_coordinates(2, 3);
    return gauss_coordinates;
}





//==================================================================================================
// Add you own member functions at the end!

void FrictionalPenaltyContact::computeGap()
{
    Vector d_ij(3);// Distance between nodes
    const Vector &xi = nodes[0]->getCrds(); //Coordinates vector of node i
    const Vector &ui = nodes[0]->getTrialDisp(); //Displacement vector of node i
    const Vector &xj = nodes[1]->getCrds(); //Coordinates vector of node j
    const Vector &uj = nodes[1]->getTrialDisp(); //Displacement vector of node j

    // cout << nodes[0] << " " << nodes[1] << endl;

    // cout << " ndoes = " << external_nodes;
    // cout << " ui = " << ui << endl;
    // cout << " uj = " << uj << endl;
    // cout << " d_ij0_prev = " << *d_ij0_prev << endl;
    // cout << " d_ij0 = " << *d_ij0 << endl;

    // const double *Bptr = B.getData();

    g->Zero();
    d_ij = (xj + uj) - (xi + ui);

    // cout << " d_ij = " << d_ij << endl;

    for (int i = 0; i < 3; i++)
    {
        // (*g)(0) += Bptr[3 + i + 0 * 6] * ( d_ij(i) - (*d_ij0_prev)(i));
        // (*g)(1) += Bptr[3 + i + 1 * 6] * (d_ij(i) - (*d_ij0_prev)(i));
        // (*g)(2) += Bptr[3 + i + 2 * 6] * d_ij(i);                  //Normal gap is with reference to zero distance
        (*g)(0) += B(0, 3 + i) * ( d_ij(i) - (*d_ij0_prev)(i));
        (*g)(1) += B(1, 3 + i) * (d_ij(i) - (*d_ij0_prev)(i));
        (*g)(2) += B(2, 3 + i) * d_ij(i);
    }

    // Normal gap
    double g_N = (*g)(2);

    if (is_in_contact_prev)                     // If element was previously in contact...
    {
        if (g_N < 0)                           // ... and still is in contact
        {
            return;                                // ... proceed
        }
        else                                   // and now is no longer in contact
        {
            is_in_contact = false;                 // set to not in contact
            return;
        }
    }
    else                                   // If element was previously not in contact ...
    {
        if (g_N <= 0)                           // ... and now is in contact
        {
            is_in_contact = true;                  // ... set to being in contact
            *d_ij0 = d_ij;                          // ... record the point of contact

            // Recompute gap with reference to this new contact point
            g->Zero();
            for (int i = 0; i < 3; i++)
            {
                // (*g)(0) += Bptr[3 + i + 0 * 6] * ( d_ij(i) - (*d_ij0_prev)(i));
                // (*g)(1) += Bptr[3 + i + 1 * 6] * (d_ij(i) - (*d_ij0_prev)(i));
                // (*g)(2) += Bptr[3 + i + 2 * 6] * d_ij(i);                  //Normal gap is with reference to zero distance
                (*g)(0) += B(0, 3 + i) * ( d_ij(i) - (*d_ij0_prev)(i));
                (*g)(1) += B(1, 3 + i) * (d_ij(i) - (*d_ij0_prev)(i));
                (*g)(2) += B(2, 3 + i) * d_ij(i);
            }

            return;
        }
        else                                   // ... and is still not in contact
        {
            return;
        }
    }
}




void FrictionalPenaltyContact::initialize()
{
    d_ij0 = new Vector (3);     // Distance from node i to j at last contact
    d_ij0_prev = new Vector (3);     // Distance from node i to j at last contact
    tA = new Vector(3);          // Current commitred local forces  t = [t_T1, t_T2, t_N]
    tC = new Vector(3);          // Current trial local forces  t = [t_T1, t_T2, t_N]
    R = new Vector(6);          // Current resisting forces
    g = new Vector(3);
    g_prev = new Vector(3);
    C = new Matrix(3, 3);

    return ;
}