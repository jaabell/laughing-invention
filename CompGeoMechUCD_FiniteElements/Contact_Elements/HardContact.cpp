///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:		   Sumeet Kumar Sinha, Jose Abell, Boris Jeremic
// DATE:			   Thu 21 May 2015 10:00:12 PM PDT
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



#include <HardContact.h>
#include <classTags.h> // Must define the class tag for the new element in this file. 
#include <iostream>
#include <cmath> // for checking if the vlue is nan


/************************************* Previous Code **************************************************************************/

//==================================================================================================
// Constructor. Receive all input parameters. Should not allocate resources!
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
HardContact::HardContact(int tag, int node1, int node2, double kn_, double kt_, double cn_, double ct_,  double mu_, double e1_x, double e1_y, double e1_z):
	Element(tag, ELE_TAG_HardContact),
	kn(kn_),
	kt(kt_),
	cn(cn_),
	ct(ct_),
	mu(mu_),
	B(3, 6),
	external_nodes(2),
	is_in_contact(false),
	is_in_contact_prev(false)
{

	tA = 0;
	tC = 0;
	R = 0;
	g = 0;
	g_prev = 0;
	C = 0;
	tC_pred =0;
	external_nodes(0) = node1;
	external_nodes(1) = node2;
	nodes[0] = 0;
	nodes[1] = 0;

	// cout << "HardContact Element " <<  tag << endl;
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

	g_commit  =0;
	g_elastic =0;
	tC_pred_commit = 0;
	is_in_contact_commit = false;

}

//==================================================================================================
// Empty constructor.  Create an empty element (with possibly a bad state)
//   * Input: Defined by user. At least should receive an integer tag, so that base class can be initialized.
//   * Output: void
HardContact::HardContact():
	Element(0, ELE_TAG_HardContact), //ATTENTION! Define the class tag in classTags.h
	kn(0.0),
	kt(0.0),
	cn(0.0),
	ct(0.0),
	mu(0.0),
	B(3, 6),
	external_nodes(2),
	is_in_contact(false),
	is_in_contact_prev(false)
{

	tA = 0;
	tC = 0;
	R = 0;
	g = 0;
	g_prev = 0;
	C = 0;
	tC_pred =0;
	external_nodes(0) = -1;
	external_nodes(1) = -1;
	nodes[0] = 0;
	nodes[1] = 0;

	g_commit = 0;
	g_elastic = 0;
	tC_pred_commit = 0;
	is_in_contact_commit = false;

	this->setNumberOfBoundaryNodes(1);

}



//==================================================================================================
// Destructor. Deallocate resources used by element.
//   * Input: void
//   * Output: void
HardContact::~HardContact()
{

	if ( tA != NULL )
	{
		delete tA;
		tA = NULL;
	}
	if ( g_commit != NULL )
	{
		delete g_commit;
		g_commit = NULL;
	}
	if ( g_elastic != NULL )
	{
		delete g_elastic;
		g_elastic = NULL;
	}
	if ( tC_pred_commit != NULL )
	{
		delete tC_pred_commit;
		tC_pred_commit = NULL;
	}
	if ( tC != NULL )
	{
		delete tC;
		tC = NULL;
	}
	if ( tC_pred != NULL )
	{
		delete tC_pred;
		tC_pred = NULL;
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
int HardContact::getNumExternalNodes(void) const
{
	return 2;
}



//==================================================================================================
// Return an ID (integer vector) with the external nodes
//   * Input: void
//   * Output: ID with tags of external nodes
const ID &HardContact::getExternalNodes(void)
{
	return external_nodes;
}



//==================================================================================================
// Return pointer array to the nodes
//   * Input: void
//   * Output: node pointer array.
Node **HardContact::getNodePtrs(void)
{
	return nodes;
}



//==================================================================================================
//Return the number of dofs in the element.
//   * Input: void
//   * Output: number of dofs (sum of dofs over all of element's nodes)
int HardContact::getNumDOF(void)
{
	return 6;
}



//==================================================================================================
// Receives a domain pointer, and sets the local domain pointer through calling the base class setDomain.
// At this point the domain is defined and set, one can allocate resources (get nodal pointers, compute
// some internal variables like lengths, volumes, etc. )
//   * Input: domain pointer (see Domain.h)
//   * Output: void
void HardContact::setDomain(Domain *theDomain)
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
			cerr << "FATAL ERROR HardContact (tag: " << this->getTag() << "), node not found in domain\n";
			return;
		}

		//Check we're connected to nodes of the right number of dofs
		int dofNd1 = nodes[0]->getNumberDOF();
		int dofNd2 = nodes[1]->getNumberDOF();

		if ( dofNd1 != 3 || dofNd2 != 3 )
		{
			cerr << "FATAL ERROR HardContact (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
			return;
		}

		// All is good, we can set the domain.
		this->DomainComponent::setDomain(theDomain);
		initialize();
		update();
		// commitState();
	}

}




//==================================================================================================
// Accept current state of the element and save it. (If applicable)
// I this is a gauss-point based element, one calls commitState on
// the material pointers (Gauss points) owned by this element.
// return 0 if success.
//   * Input: void
//   * Output: error flag, 0 if success
int HardContact::commitState(void)
{
	
	*tA = *tC;
	*g_commit = *g;
	is_in_contact_commit = is_in_contact;
	*g_elastic = (*tA)/kt; (*g_elastic)(2)=(*g)(2);
	*tC_pred_commit = *tC_pred; 
	
	R->Zero();
	R->addMatrixTransposeVector(1, B, *tA, 1.0);


	/////////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////
	cout.precision(10);
	cout << "******************************************** Commit State *************************************\n";
	cout << "*g_commit " <<  *g_commit;
	cout << "is_in_contact_commit " <<  is_in_contact_commit << "\n";
	cout << "*Commit Resisting Force -  " << *tA ;
	cout << "*g_elastic - " << *g_elastic ;
	cout << "*tC_pred " << *tC_pred;
	cout << "###############################################################################################\n";
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}




//==================================================================================================
// Revert the state of the element to the last committed state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int HardContact::revertToLastCommit(void)
{
	*tC = *tA;
	*g = *g_commit;
	*g_prev = *g_commit;
	is_in_contact = is_in_contact_commit;
	*g_elastic = (*tA)/kt; (*g_elastic)(2)=(*g)(2);
	*tC_pred = *tC_pred_commit;

	return 0;
}




//==================================================================================================
// Revert the state of the element to the initial state.
// Must call for gausspoints if needed.
//   * Input: void
//   * Output: error flag, 0 if success
int HardContact::revertToStart(void)
{
	// g_elastic->Zero();
	// tC_pred_commit ->Zero();
	// tA->Zero();
	// g_commit->Zero();
	// is_in_contact_commit = true;
	return 0;
}




//==================================================================================================
// Update the state of the element. I.E. compute new tangent stiffness,
// compute new resisting force, advance state variables.
// These changes should not be permanent until commit function is called.
//   * Input: void
//   * Output: error flag, 0 if success
int HardContact::update(void)
{
	double tol = 0;
	computeGap();
	static Vector delg(3); delg.Zero();	     		// correct gap fucntion
	static Vector trial_tC(3); trial_tC.Zero();     // Predicted Forces

	/////////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////
	cout << "************************************ Iteration Steps **********************************\n";
	cout << "is_in_contact_prev " <<  is_in_contact_prev << "\n";
	cout << "is_in_contact " <<  is_in_contact << "\n";	
	cout << "*g " <<  *g;
	cout << "*tC_pred " << *tC_pred;
	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (is_in_contact){

		////////////////// Editted by Sumeet //////////////////////////
		// The situataions that can happen inside it are as follows ///
		// 1) No Contact to Contact ///////////////////////////////////
		// 2) Contact to Contact Relaxation////////////////////////////
		// 3) Contact to Contact Penetration///////////////////////////
		/////////////////////////////////////////////////////////////// 

		if (is_in_contact_prev){
			delg = *g - *g_prev;	///// Contact to Contact ////
		}
		else{
			delg = *g;				///// No Contact to Contact ////
		}

		/////////////////////// Setting elastic Tangent Stiffness ///////////////////////
		C->Zero(); (*C)(0, 0) = kt;	(*C)(1, 1) = kt; (*C)(2, 2) = kn;
		//////////////////// Computing predictive forces (tB) ///////////////////////////

		tC_pred->addMatrixVector(1, *C, delg, 1.0); 			/// Correct Normal change in Predicted Shear ///
		trial_tC = *tC_pred;

		// // ///////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////	
		// cout << "*tC_pred " <<  *tC_pred;	
		// cout << "*trial_tC " <<  trial_tC;
		// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////////////////
		//////////////////// Compute Yield function at prediction point /////////////////
		///////////// checking if yf_B = (mu*N + Shear Force) >0  ///////////////////////
		/////////////////////////////////////////////////////////////////////////////////

		static double yf_B; yf_B=0, yf_B = sqrt(trial_tC(0)*trial_tC(0) + trial_tC(1)*trial_tC(1)) + mu*trial_tC(2);

		if (yf_B > tol){ /////// Sliding Condition /////////

			/////////////////////////////////// Finding the Loading direction //////////////////////////////////////////////////
			static Vector eta(3); eta.Zero();eta=trial_tC;eta(2)=0;double norm_eta = eta.Norm();				// eta -> unit plastic flow direction (non-associative)

	        if (norm_eta > tol){
	            eta = eta / norm_eta;
	       		trial_tC(0) = -1*mu*trial_tC(2)*eta(0); 
	       		trial_tC(1) = -1*mu*trial_tC(2)*eta(1);
	       		if(mu>0)
	       			trial_tC(2) = -1*sqrt(trial_tC(0)*trial_tC(0) + trial_tC(1)*trial_tC(1))/mu;
	       	}
	        else{
	            trial_tC(0) =0;
	            trial_tC(0) =0;
	        }

	        // cout << "Checking The compatibity " << (sqrt( trial_tC(0)*trial_tC(0) + trial_tC(1)*trial_tC(1)) + mu*trial_tC(2) ) << endl;
	        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// // ///////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////	
			// cout << "*tC_pred " <<  *tC_pred;	
			// cout << "*trial_tC " <<  trial_tC;
			// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	        //////////////////////////// Calculating the crossing increament force to yield surface /////////////////////////
	       	static Vector zeta(3); zeta.Zero(); zeta=trial_tC;							// zeta -> unit normal to yield surface
	       	eta = trial_tC; eta(2)=0;
	       	norm_eta = eta.Norm();
	        if (norm_eta > tol){
	            zeta = zeta / norm_eta;
		        ///////////////////////////////// Updating the stiffness matrix ///////////////////////////////////////////////////////////
	        	zeta(2)=mu;	zeta = zeta / zeta.Norm();								// unit normal to yield surface
		        double Beta = 0;     												// Hardening parameter
				static Matrix Cplastic(3,3); 
				Cplastic(0,0) = eta(0)*zeta(0)*kt; 	Cplastic(0,1) = eta(0)*zeta(1)*kt; 	Cplastic(0,2) = eta(0)*zeta(2)*kn;
				Cplastic(1,0) = eta(1)*zeta(0)*kt; 	Cplastic(1,1) = eta(1)*zeta(1)*kt;	Cplastic(1,2) = eta(1)*zeta(2)*kn;
				C->Zero(); (*C)(0, 0) = kt;	(*C)(1, 1) = kt; (*C)(2, 2) = kn;
				*C  = *C - 1.0/( Beta + (eta(0)*zeta(0)+eta(1)*zeta(1)) ) * Cplastic;
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	        }
	        else{

	        	if(mu==0){
	        		C->Zero(); (*C)(2, 2) = kn;
	        	}
	        	else{
	        		C->Zero(); (*C)(0, 0) = kt;	(*C)(1, 1) = kt; (*C)(2, 2) = kn;
	        	}

	        }

			///////////////////////////////// Calculating The Actual Force Increament///////////////////////////////////////////////////
			////////////////////////////////////////// In This case its Zero ///////////////////////////////////////////////////////////
			////////////////////////////////////////// So Can Be commented Out /////////////////////////////////////////////////////////
			// delg = yf_B/kt*eta;
			// del_tC.addMatrixVector(1, *C, delg, 1.0);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			// /////////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////////////
			// cout << "I am Sliding \n";
			// cout << "eta " << eta;
			// cout << "zeta " << zeta; 
			// cout << "New Code Stiffness During Sliding " << *C ;
			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		}

	}
	else{

		tC_pred->Zero();

		////////////////// Editted by Sumeet //////////////////////////
		// The situataions that can happen inside it are as follows ///
		// 1) No Contact to No Contact ////////////////////////////////
		// 2) Contact to No Contact ///////////////////////////////////
		/////////////////////////////////////////////////////////////// 

		//////////////////// Computing predictive forces (del_tC) ///////////////////////////

		// if (is_in_contact_prev)
		// 	del_tC.addMatrixVector(0, *C, *(g_prev), -1.0); ///// Contact to No Contact ////
		C->Zero();
		trial_tC(0) =0; trial_tC(1) =0; trial_tC(2) =0;
		/////////////////////////////////////////////////////////////////////////////////

	}

	*tC = trial_tC ;			    // correct local force
	*g_prev = *g;					// defining the previous gap

	// cout << "Checking The compatibity " << (sqrt( (*tC)(0)*(*tC)(0) + (*tC)(1)*(*tC)(1)) + mu*(*tC)(2) ) << endl;


	// /////////////////////////////////////// Sumeet :: Printing for Debugging //////////////////////////////////////////////////	
	// cout.precision(10);
	// cout << "*del_gap " <<  delg;
	// // cout << "del_tC " << del_tC;
	// cout << "tC " <<  *tC;
	// cout << "C " <<  *C << endl;;
	// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}




//==================================================================================================
//  (optionl) Set the elemental load to zero.
//   * Input: void
//   * Output: void
void HardContact::zeroLoad(void)
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
int HardContact::addLoad(ElementalLoad *theLoad, double loadFactor)
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
int HardContact::addInertiaLoadToUnbalance(const Vector &accel)
{
	//cout << "HardContact::addInertiaLoadToUnbalance()\n";
	return 0;
}






//==================================================================================================
// Functions to obtain stiffness, mass, damping and residual information
//   * Input: void
//   * Output: reference to tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = HardContact::getNumDOF();
//   Pro tip. If this matrix computes the tangent stiffness, then
//   it can be stored as a static member variable so that all elements share
//   the same memory space (each element overwrites the tangent). This saves
//   memory.
const Matrix &HardContact::getTangentStiff(void)
{
	static Matrix K(6, 6);
	K.Zero();
	if (is_in_contact)
	{
		K.addMatrixTripleProduct(0.0, B, *C, 1.0); // B' * C * B
	}

	return K;
}





//==================================================================================================
// Functions to obtain initial stiffness
//   * Input: void
//   * Output: reference to initial tangent stiffness matrix (of size nDOF x nDOF,
//             where nDOF = HardContact::getNumDOF();
const Matrix &HardContact::getInitialStiff(void)
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
//             where nDOF = HardContact::getNumDOF();
const Matrix &HardContact::getDamp(void)
{
	// cout << "HardContact::getDamp(void)\n";
	static Matrix C(6, 6);
	C.Zero();

	Matrix Clocal(3, 3);

	Clocal(0, 0) = ct;
	Clocal(1, 1) = ct;
	Clocal(2, 2) = cn;

	C.addMatrixTripleProduct(0.0, B, Clocal, 1.0); // B' * C * B
	return C;
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = HardContact::getNumDOF();
const Matrix &HardContact::getMass(void)
{
	static Matrix Mzero(6, 6);
	return Mzero;
}






//==================================================================================================
// (optional) If element provides its own damping matrix, then this function returns it
//   * Input: void
//   * Output: reference to damping stiffness matrix (of size nDOF x nDOF,
//             where nDOF = HardContact::getNumDOF();
const Vector &HardContact::getResistingForce(void)
{
	R->Zero();
	R->addMatrixTransposeVector(1, B, *tC, 1);
	return *R;
}






//==================================================================================================
// (optional) Add inertial terms to resisting force.
//   * Input: void
//   * Output: Vector of doubles with new resisting force.
//   Note: Regularly, this function calls getResistingForce() and then
//         adds inertial terms.
const Vector &HardContact::getResistingForceIncInertia(void)
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
int HardContact::sendSelf(int commitTag, Channel &theChannel)
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
	
    ID idData( 2 );

    idData( 0 ) = this->getTag();
    idData( 1 ) = (int) is_in_contact_commit;

    if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
        return -1;
    }

    // send double data
    Vector floatData(5);
    floatData(0) = kn;
    floatData(1) = kt;
    floatData(2) = cn;
    floatData(3) = ct;
    floatData(4) = mu;

    if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    ////////// Commited local Contact forces vector /////////////////////////////////

    if ( theChannel.sendVector( 0, commitTag, *tA ) < 0 )
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    if ( theChannel.sendVector( 0, commitTag, *g_commit ) < 0 )
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    if ( theChannel.sendVector( 0, commitTag, *tC_pred_commit ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    if (theChannel.sendMatrix( 0,commitTag,	B ) < 0)
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    if (theChannel.sendMatrix( 0,commitTag,	*C ) < 0)
    {
        cerr << "WARNING HardContact::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
        return -1;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    string tmp_string;

    tmp_string = stiffness_type;
    if ( theChannel.sendString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send String stiffness_type\n";
        return -1;
    }

    tmp_string = damping_type;
    if ( theChannel.sendString( 0, commitTag, damping_type ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send String damping_type\n";
        return -1;
    }

    // Sending the nodes

    if ( theChannel.sendID( 0, commitTag, external_nodes ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
        return -1;
    }


	return 0;
}




//==================================================================================================
// (optional, a must if you want to do parallel processing)
//  Receive all state data of the element through a channel (usually an MPI_Channel)
//  this data comes from an element that is calling sendSelf in some other process.
//   * Input: a reference to the Channel to use.
//   * Output: error flag, 0 if success
// Note: This function is called after setDomain() so all resources should be made available.
int HardContact::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
	// Useful constructs
	// int error_flag;
	// theChannel.receiveVector(0, 0, double_data);  // the first two parameters are deprecated
	//  Check that error_flag is not < 0
	// theChannel.receiveID(0, 0, integer_data);  //  the first two parameters are deprecated
	//  Check that error_flag is not < 0

    initialize();
    ID idData( 2 );

    if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve ID idData\n";
        return -1;
    }

    this->setTag(idData( 0 ));
    is_in_contact_commit = (bool) idData( 1 );


    Vector floatData(5);
    if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }

    kn = floatData(0);
    kt = floatData(1);
    cn = floatData(2);
    ct = floatData(3);
    mu = floatData(4);

	////////////////////// Commited local Contact forces vector /////////////////////////////////

    if ( theChannel.receiveVector( 0, commitTag, *tA) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }

    *tC = *tA;

    if ( theChannel.receiveVector( 0, commitTag, *g_commit ) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }

    *g_prev = *g_commit;

    if ( theChannel.receiveVector( 0, commitTag, *tC_pred_commit ) < 0 )
    {
        cerr << "WARNING EightNodeBrickLT::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }

    *tC_pred = *tC_pred_commit;

    if (theChannel.receiveMatrix( 0,commitTag,	B ) < 0)
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }

    if (theChannel.receiveMatrix( 0,commitTag,	*C ) < 0)
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
        return -1;
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////

    string tmp_string;
    if ( theChannel.receiveString( 0, commitTag,  tmp_string) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve String stiffness_type\n";
        return -1;
    }
    stiffness_type = tmp_string;

    if ( theChannel.receiveString( 0, commitTag, tmp_string ) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve String damping_type\n";
        return -1;
    }
    damping_type = tmp_string;

    // Recieve the nodes

    if ( theChannel.receiveID( 0, commitTag, external_nodes ) < 0 )
    {
        cerr << "WARNING HardContact::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
        return -1;
    }

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
void HardContact::Print(ostream &s, int flag )
{
	// you must implement
	if (flag >= 0)
	{
		s << " HardContact, tag =  " << this->getTag() << endl;
	}
}





//==================================================================================================
// Check element correctness
//   * Input: an ostream to print stuff into (print details of what is being checked here.)
//   * Output: an error flag (<0 if element is not right in some way)
//  Note: be verbose print element tag, etc. Print out only if an error is encountered.
int HardContact::CheckMesh(ofstream &)
{
	return 0;
}



//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: number of double outputs for the element (size of the output vector)

#define HardContact_NOUTPUT 9

int HardContact::getOutputSize() const
{
	return  HardContact_NOUTPUT;
}





//==================================================================================================
// Output interface functions
//   * Input: void
//   * Output: Vector (array of doubles) with the element output.
const Vector &HardContact::getOutput()
{
	static Vector output_vector(HardContact_NOUTPUT);

	output_vector(0) = (*g_commit)(0);
	output_vector(1) = (*g_commit)(1);
	output_vector(2) = (*g_commit)(2);
	output_vector(3) = (*tA)(0);
	output_vector(4) = (*tA)(1);
	output_vector(5) = (*tA)(2);
	output_vector(6) = (*g_elastic)(0);
	output_vector(7) = (*g_elastic)(1);
	output_vector(8) = (*g_elastic)(2);

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
Matrix &HardContact::getGaussCoordinates(void)
{
	// you must implement
	static Matrix gauss_coordinates(2, 3);
	return gauss_coordinates;
}





//==================================================================================================
// Add you own member functions at the end!

void HardContact::computeGap()
{

	static Vector d_ij(3); d_ij.Zero();				// Distance between nodes
	const Vector &xi = nodes[0]->getCrds();	 		// Coordinates vector of node i
	const Vector &ui = nodes[0]->getTrialDisp();	// Displacement vector of node i
	const Vector &xj = nodes[1]->getCrds(); 		// Coordinates vector of node j
	const Vector &uj = nodes[1]->getTrialDisp(); 	// Displacement vector of node j

	g->Zero();
	d_ij = (xj + uj) - (xi + ui);

	for (int i = 0; i < 3; i++)
	{
		(*g)(0) += B(0, 3 + i) * ( d_ij(i));
		(*g)(1) += B(1, 3 + i) * (d_ij(i));
		(*g)(2) += B(2, 3 + i) * d_ij(i);
	}

	is_in_contact_prev = is_in_contact;
	is_in_contact=false;

	if ((*g)(2) <= 0 )
		is_in_contact=true;

	return;

}




void HardContact::initialize()
{

	tA = new Vector(3);          // Current commitred local forces  t = [t_T1, t_T2, t_N]
	tC = new Vector(3);          // Current trial local forces  t = [t_T1, t_T2, t_N]
	R = new Vector(6);           // Current resisting forces
	g = new Vector(3);
	g_prev = new Vector(3);
	g_commit = new Vector(3);
	C = new Matrix(3, 3);
	tC_pred = new Vector(3);
	g_elastic = new Vector(3);
	tC_pred_commit = new Vector(3);

	return ;
}

//==================================================================================================
// Finds the cross product of two vectors and stores in the third vector
// vect3[] = vect1[] X vect2[] 
void HardContact::cross_product(double vect1[],double vect2[],double vect3[]){

	vect3[0] = vect1[1] * vect2[2] - vect1[2] * vect2[1];
	vect3[1] = vect1[2] * vect2[0] - vect1[0] * vect2[2];
	vect3[2] = vect1[0] * vect2[1] - vect1[1] * vect2[0];
}

//==================================================================================================
// Finds the norm of the vector and stores in the norm variable
// norm = sqrt(vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2])
void HardContact::norm(double vect[],double* nrm){

	*nrm = abs(sqrt(vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]));
	// cout << "norm -" << "\n";
}