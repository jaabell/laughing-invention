///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Boris Jeremic  & Jose Abell
// DATE:               September 2014
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



#ifndef ShearBeamLT_CPP
#define ShearBeamLT_CPP

#include "ShearBeamLT.h"
#include <LTensorDisplay.h>
#include <HDF5_Channel.h>



DTensor2 ShearBeamLT::gp_coords(1, 3, 0.0);
DTensor1 ShearBeamLT::gp_weight(1, 0.0);

// Matrix ShearBeamLT::K( 6, 6);
// Matrix ShearBeamLT::M( 6, 6);

// Vector ShearBeamLT::P( 6 );

ShearBeamLT::ShearBeamLT( int element_number,
                          int node_numb_1, int node_numb_2, double area,
                          NDMaterialLT *Globalmmodel)
    : Element( element_number, ELE_TAG_ShearBeamLT ),
      rho( 0.0 ), connectedExternalNodes( 2 ),
      Ki( 0 ), Q( 6 ), bf(3), K( 6, 6),
      M( 6, 6), P(6), gauss_points(1, 3), outputVector(ShearBeamLT_OUTPUT_SIZE)
{

    this->setMaterialTag(Globalmmodel->getTag());
    
    rho = Globalmmodel->getRho();
    mmodel = Globalmmodel;
    Area = area;

    material_array[0] = mmodel->getCopy();


    connectedExternalNodes( 0 ) = node_numb_1;
    connectedExternalNodes( 1 ) = node_numb_2;

    theNodes[0] = 0;
    theNodes[1] = 0;

    is_mass_computed = false;

}



//====================================================================
ShearBeamLT::ShearBeamLT(): Element( 0, ELE_TAG_ShearBeamLT ),
    rho( 0.0 ), connectedExternalNodes( 2 ) , Ki( 0 ), mmodel( 0 ),
    Q( 6 ), bf(3), K( 6, 6),
    M( 6, 6), P(6),
    gauss_points(1, 3),
    outputVector(ShearBeamLT_OUTPUT_SIZE)
{
    // initialized = false;
    is_mass_computed = false;

    //Don't know the nodes yet (used in parallel, recvSElf is called later and the nodes are known then)
    connectedExternalNodes( 0 ) = -1;
    connectedExternalNodes( 1 ) = -1;

    theNodes[0] = 0;
    theNodes[1] = 0;

}




//####DESTROYER!###############################################################
ShearBeamLT::~ShearBeamLT ()
{
    // Delete the NDMaterials at each integration point
    if ( material_array[0] )
    {
        delete material_array[0];
    }

    if ( Ki != 0 )
    {
        delete Ki;
    }
}


//Compute gauss point coordinates
void ShearBeamLT::computeGaussPoint()
{


    const Vector &nd1Crds = theNodes[0]->getCrds();
    const Vector &nd2Crds = theNodes[1]->getCrds();

    if (nd1Crds(0) != nd2Crds[0] || nd1Crds(1) != nd2Crds[1])
    {
        cerr << "ShearBeamLT::computeGaussPoint() Warning!! ShearBeam with tag " << getTag() << " is not vertical! \n\n";
    }

    gauss_points(0, 0) = (nd1Crds(0) + nd2Crds(0)) / 2;
    gauss_points(0, 1) = (nd1Crds(1) + nd2Crds(1)) / 2;
    gauss_points(0, 2) = (nd1Crds(2) + nd2Crds(2)) / 2;

    gp_weight(0) = 1.0;

    length = abs(nd1Crds(2) - nd2Crds(2));

}


////#############################################################################


//=============================================================================
int ShearBeamLT::getNumExternalNodes () const
{
    return 2;
}


//=============================================================================
const ID &ShearBeamLT::getExternalNodes ()
{
    return connectedExternalNodes;
}

Node **
ShearBeamLT::getNodePtrs( void )
{
    return theNodes;
}

//=============================================================================
int ShearBeamLT::getNumDOF ()
{
    return 6;
}

//=============================================================================
void ShearBeamLT::setDomain ( Domain *theDomain )
{
    // Check Domain is not null - invoked when object removed from a domain
    if ( theDomain == 0 )
    {
        theNodes[0] = 0;
        theNodes[1] = 0;
    }
    else
    {
        int Nd1 = connectedExternalNodes( 0 );
        int Nd2 = connectedExternalNodes( 1 );


        theNodes[0] = theDomain->getNode( Nd1 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[0] == 0 )
        {
            theNodes[0] = theDomain->getOutsideNode( Nd1 );
        }

# endif
        theNodes[1] = theDomain->getNode( Nd2 );
# ifdef _PARALLEL_PROCESSING

        if ( theNodes[1] == 0 )
        {
            theNodes[1] = theDomain->getOutsideNode( Nd2 );
        }

# endif

        if ( theNodes[0] == 0 || theNodes[1] == 0 )
        {
            cerr << "FATAL ERROR ShearBeamLT (tag: " << this->getTag() << "), node not found in domain\n";
            exit( -1 );
        }

        int dofNd1 = theNodes[0]->getNumberDOF();
        int dofNd2 = theNodes[1]->getNumberDOF();

        if ( dofNd1 != 3 || dofNd2 != 3 )
        {
            cerr << "FATAL ERROR ShearBeamLT (tag: " << this->getTag() << "), has differing number of DOFs at its nodes\n";
            exit( -1 );
        }

        this->DomainComponent::setDomain( theDomain );
    }

    computeGaussPoint();
}

//=============================================================================
int ShearBeamLT::commitState ()
{

    int retVal = 0;

    // call element commitState to do any base class stuff
    if ( ( retVal = this->Element::commitState() ) != 0 )
    {
        cerr << "ShearBeamLT::commitState () - failed in base class";
    }

    Vector pp = getResistingForce();

    retVal += material_array[0]->commitState();


    // formOutput();

    return retVal;
}


void ShearBeamLT::formOutput()
{
    int ii = 0;
    static DTensor2 stress(3, 3);
    static DTensor2 strain(3, 3);
    static DTensor2 plstrain(3, 3);

    stress *= 0;
    strain *= 0;
    plstrain *= 0;

    strain = material_array[0]->getStrainTensor();
    plstrain = material_array[0]->getPlasticStrainTensor();
    stress = material_array[0]->getStressTensor();

    //Write strain
    outputVector(ii++) = strain(0, 0);
    outputVector(ii++) = strain(1, 1);
    outputVector(ii++) = strain(2, 2);
    outputVector(ii++) = strain(0, 1);
    outputVector(ii++) = strain(0, 2);
    outputVector(ii++) = strain(1, 2);

    //Write plastic strain
    outputVector(ii++) = plstrain(0, 0);
    outputVector(ii++) = plstrain(1, 1);
    outputVector(ii++) = plstrain(2, 2);
    outputVector(ii++) = plstrain(0, 1);
    outputVector(ii++) = plstrain(0, 2);
    outputVector(ii++) = plstrain(1, 2);


    //Write stress
    outputVector(ii++) = stress(0, 0);
    outputVector(ii++) = stress(1, 1);
    outputVector(ii++) = stress(2, 2);
    outputVector(ii++) = stress(0, 1);
    outputVector(ii++) = stress(0, 2);
    outputVector(ii++) = stress(1, 2);

}

//=============================================================================
int ShearBeamLT::revertToLastCommit ()
{
    int retVal = 0;

    retVal += material_array[0]->revertToLastCommit();

    return retVal;
}

//=============================================================================
int ShearBeamLT::revertToStart ()
{
    int retVal = 0;

    retVal += material_array[0]->revertToStart();

    return retVal;
}


//=============================================================================
const Matrix &ShearBeamLT::getTangentStiff()
{
    static DTensor4 stifftensor(2, 3, 3, 2, 0.0);
    static DTensor4 E_elpl(2, 3, 3, 2, 0.0);
    static DTensor4 Kkt( 2, 3, 3, 2, 0.0);
    static DTensor2 dhGlobal( 2, 3, 0.0 );

    stifftensor *= 0;
    E_elpl *= 0;
    Kkt *= 0;
    dhGlobal *= 0;

    dhGlobal(0, 2) = -1 / length;
    dhGlobal(1, 2) = 1 / length;

    E_elpl          = material_array[0]->getTangentTensor();


    // This is the actual integration
    // FIXME: This can be more efficient! note that 2/3rds of dhGlobal are zero
    Index < 'a' > a;
    Index < 'b' > b;
    Index < 'c' > c;
    Index < 'd' > d;
    stifftensor(i, a, c, j) = dhGlobal(i, b) * E_elpl(a, b, c, d) * dhGlobal(j, d) * length * Area;

    int Ki = 0;
    int Kj = 0;

    for ( int ii = 1 ; ii <= 2 ; ii++ )
    {
        for ( int jj = 1 ; jj <= 2 ; jj++ )
        {
            for ( int kk = 1 ; kk <= 3 ; kk++ )
            {
                for ( int ll = 1 ; ll <= 3 ; ll++ )
                {
                    Ki = kk + 3 * ( ii - 1 );
                    Kj = ll + 3 * ( jj - 1 );
                    K( Ki - 1 , Kj - 1 ) = stifftensor( ii - 1, kk - 1, ll - 1, jj - 1 );
                }
            }
        }
    }

    return K;
}




//=============================================================================
const Matrix &ShearBeamLT::getInitialStiff ()
{

    if ( Ki == 0 )
    {
        Ki = new Matrix( this->getTangentStiff() );
    }

    if ( Ki == 0 )
    {
        cerr << "FATAL ShearBeamLT::getInitialStiff() -";
        cerr << "ran out of memory\n";
        exit( -1 );
    }

    return *Ki;
}

//=============================================================================

const Matrix &ShearBeamLT::getMass ()
{
    if (not is_mass_computed)
    {
        //Consistent mass matrix
        double m = rho * Area * length;
        M( 0 , 0 ) = m / 3;
        M( 1 , 1 ) = m / 3;
        M( 2 , 2 ) = m / 3;
        M( 3 , 3 ) = m / 3;
        M( 4 , 4 ) = m / 3;
        M( 5 , 5 ) = m / 3;

        M(0, 3) = m / 6;
        M(3, 0) = m / 6;
        M(1, 4) = m / 6;
        M(4, 1) = m / 6;
        M(2, 5) = m / 6;
        M(5, 2) = m / 6;

        is_mass_computed = true;
    }

    return M;
}

//=============================================================================
void ShearBeamLT::zeroLoad( void )
{
    Q.Zero();
    // P.Zero();
}



//======================================================================
const Vector &ShearBeamLT::getBodyForce( double loadFactor, const Vector &data )
{

    static Vector bforce( 6 );
    bforce.Zero();


    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    bf(0) = data(0);
    bf(1) = data(1);
    bf(2) = data(2);

    Vector ba( 6 ), bfx( 3 );
    bfx( 0 ) = bf( 0 ) * loadFactor;
    bfx( 1 ) = bf( 1 ) * loadFactor;
    bfx( 2 ) = bf( 2 ) * loadFactor;

    ba( 0 ) =  bfx( 0 );
    ba( 1 ) =  bfx( 1 );
    ba( 2 ) =  bfx( 2 );
    ba( 3 ) =  bfx( 0 );
    ba( 4 ) =  bfx( 1 );
    ba( 5 ) =  bfx( 2 );


    //Form equivalent body force
    this->getMass();

    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//======================================================================
const Vector &ShearBeamLT::getSurfaceForce( double loadFactor, const Vector &data )
{
    static Vector NodalForces(6);
    NodalForces.Zero();
    return NodalForces;
}



//=============================================================================
int ShearBeamLT::addLoad( ElementalLoad *theLoad, double loadFactor )
{

    int type;
    const Vector &data = theLoad->getData( type, loadFactor );

    if ( type == LOAD_TAG_ElementSelfWeight )
    {
        Vector Fbody = this->getBodyForce( loadFactor, data );
        Q.addVector( 1.0, Fbody, 1.0 );
    }


    return 0;
}




//=============================================================================
int ShearBeamLT::addInertiaLoadToUnbalance( const Vector &accel )
{
    // Check for a quick return
    if ( rho == 0.0 )
    {
        return 0;
    }

    // Get R * accel from the nodes
    const Vector &Raccel1 = theNodes[0]->getRV( accel );
    const Vector &Raccel2 = theNodes[1]->getRV( accel );

    if ( 3 != Raccel1.Size() || 3 != Raccel2.Size()   )
    {
        cerr << "ShearBeamLT::addInertiaLoadToUnbalance matrix and vector sizes are incompatable\n";
        return -1;
    }

    Vector ra( 6 );

    ra( 0 ) =  Raccel1( 0 );
    ra( 1 ) =  Raccel1( 1 );
    ra( 2 ) =  Raccel1( 2 );
    ra( 3 ) =  Raccel2( 0 );
    ra( 4 ) =  Raccel2( 1 );
    ra( 5 ) =  Raccel2( 2 );


    Q.addMatrixVector( 1.0, M, ra, -1.0 );
    return 0;
}

//=============================================================================
const Vector ShearBeamLT::FormEquiBodyForce( void )
{
    Vector bforce( 6 );

    // Check for a quick return
    if ( rho == 0.0 )
    {
        return bforce;
    }

    Vector ba( 6 );


    //Form equivalent body force
    bforce.addMatrixVector( 0.0, M, ba, 1.0 );

    return bforce;
}


//=============================================================================
const Vector &ShearBeamLT::getResistingForce()
{
    //P.Zero();

    const DTensor2& stress = material_array[0]->getStressTensor();
    static DTensor2 dhGlobal( 2, 3, 0.0 );
    static DTensor2 nodal_forces( 2, 3, 0.0 );

    // stress *= 0;
    dhGlobal *= 0;
    nodal_forces *= 0;

    dhGlobal(0, 2) = -1 / length;
    dhGlobal(1, 2) = 1 / length;

    nodal_forces(i, j) = Area * length * (dhGlobal( i, k ) * stress( j, k ) );

    for ( int i = 0; i < 2; i++ )
        for ( int j = 0; j < 3; j++ )
        {
            P( i * 3 + j ) = nodal_forces( i, j );
        }

    P.addVector( 1.0, Q, -1.0 );


    return P;
}

//=============================================================================
const Vector &ShearBeamLT::getResistingForceIncInertia ()
{
    // form resisting force
    this->getResistingForce();

    //
    // now add dynamic terms
    // P += M * a + C * v
    //

    if ( rho != 0.0 )
    {

        // form the mass matrix
        this->getMass();

        const Vector &accel1 = theNodes[0]->getTrialAccel();
        const Vector &accel2 = theNodes[1]->getTrialAccel();

        Vector a( 6 ); // originally 8

        a( 0 ) =  accel1( 0 );
        a( 1 ) =  accel1( 1 );
        a( 2 ) =  accel1( 2 );
        a( 3 ) =  accel2( 0 );
        a( 4 ) =  accel2( 1 );
        a( 5 ) =  accel2( 2 );


        // P += M * a
        P.addMatrixVector( 1.0, M, a, 1.0 );

        // add the damping forces if rayleigh damping
        if ( a0 != 0.0 || a1 != 0.0 || a2 != 0.0 || a3 != 0.0 )
        {
            P += this->getRayleighDampingForces();
        }

    }
    else
    {

        // add the damping forces if rayleigh damping
        if ( a1 != 0.0 || a2 != 0.0 || a3 != 0.0 )
        {
            P += this->getRayleighDampingForces();
        }
    }


    return P;

}


int ShearBeamLT::sendSelf ( int commitTag, Channel &theChannel )
{
    // if ( !initialized )
    // {
    //     //populate();
    // }


    // ID idData( 5 );

    // idData( 0 ) = this->getTag();
    // idData( 1 ) = numDOF;
    // idData( 2 ) = nodes_in_brick;
    // idData( 3 ) = order;
    // idData( 4 ) = material_array[0]->getClassTag();

    // if ( theChannel.sendID( 0, commitTag, idData ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send ID idData\n";
    //     return -1;
    // }

    // // send double data
    // Vector floatData(4);
    // floatData(0) = Volume;
    // floatData(1) = e_p;
    // floatData(2) = determinant_of_Jacobian;
    // floatData(3) = rho;

    // if ( theChannel.sendVector( 0, commitTag, floatData ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send Vector floatData\n";
    //     return -1;
    // }


    // // Send the nodes

    // if ( theChannel.sendID( 0, commitTag, connectedExternalNodes ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send ID connectedExternalNodes\n";
    //     return -1;
    // }


    // // material objects send themselves

    // for ( int i = 0; i < 8; i++ )
    // {
    //     if ( material_array[i]->sendSelf( commitTag, theChannel ) < 0 )
    //     {
    //         cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send material models\n";
    //         return -1;
    //     }
    // }


    // //Send Q
    // if ( theChannel.sendVector( 0, commitTag, Q ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send its Q\n";
    //     return -1;
    // }

    // //Send bf
    // if ( theChannel.sendVector( 0, commitTag, bf ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send its bf\n";
    //     return -1;
    // }

    // //Send the gauss points
    // if ( theChannel.sendMatrix( 0, commitTag, gauss_points ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send its Gauss point coordinates\n";
    //     return -1;
    // }

    // //Send outputVector
    // if ( theChannel.sendVector( 0, commitTag, outputVector ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::sendSelf() - " << this->getTag() << " failed to send its outputVector\n";
    //     return -1;
    // }



    return 0;

}

int ShearBeamLT::receiveSelf ( int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker )
{

    // if ( !initialized )
    // {
    //     populate();
    // }

    // ID idData( 5 );

    // if ( theChannel.receiveID( 0, commitTag, idData ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::receiveSelf() - failed to receive ID\n";
    //     return -1;
    // }

    // this->setTag( idData( 0 ) );
    // numDOF = idData(1);
    // nodes_in_brick = idData(2);
    // order = idData(3);


    // Vector floatData(4);
    // if ( theChannel.receiveVector( 0, commitTag, floatData ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::receiveSelf() - " << this->getTag() << " failed to recieve Vector floatData\n";
    //     return -1;
    // }
    // Volume                  = floatData(0) ;
    // e_p                     = floatData(1) ;
    // determinant_of_Jacobian = floatData(2) ;
    // rho                     = floatData(3) ;


    // // Recieve the nodes

    // if ( theChannel.receiveID( 0, commitTag, connectedExternalNodes ) < 0 )
    // {
    //     cerr << "WARNING ShearBeamLT::receiveSelf() - " << this->getTag() << " failed to recieve ID connectedExternalNodes\n";
    //     return -1;
    // }


    // if ( material_array[0] == 0 )
    // {
    //     int matClassTag = idData( 4 );
    //     for ( int i = 0; i < 8; i++ )
    //     {

    //         // Allocate new material with the sent class tag
    //         NDMaterialLT *ndmat = theBroker.getNewNDMaterialLT( matClassTag );
    //         if ( ndmat == 0 )
    //         {
    //             cerr << "ShearBeamLT::receiveSelf() - Broker could not create NDMaterialLT of class type " << matClassTag << endln;
    //             return -1;
    //         }

    //         // Now receive materials into the newly allocated space
    //         if ( ( ndmat )->receiveSelf( commitTag, theChannel, theBroker ) < 0 )
    //         {
    //             cerr << "ShearBeamLT::receiveSelf() - material " << i << "failed to recv itself\n";
    //             return -1;
    //         }

    //         material_array[i] = ndmat;
    //     }
    // }

    // // Q
    // if ( theChannel.receiveVector( 0, commitTag, Q ) < 0 )
    // {
    //     cerr << "ShearBeamLT::receiveSelf() - failed to recv Q!\n";
    //     return -1;
    // }

    // // bf
    // if ( theChannel.receiveVector( 0, commitTag, bf ) < 0 )
    // {
    //     cerr << "ShearBeamLT::receiveSelf() - failed to recv bf!\n";
    //     return -1;
    // }

    // // gauss_points
    // if ( theChannel.receiveMatrix( 0, commitTag, gauss_points ) < 0 )
    // {
    //     cerr << "ShearBeamLT::receiveSelf() - failed to recv gauss_points!\n";
    //     return -1;
    // }

    // // outputVector
    // if ( theChannel.receiveVector( 0, commitTag, outputVector ) < 0 )
    // {
    //     cerr << "ShearBeamLT::receiveSelf() - failed to recv outputVector!\n";
    //     return -1;
    // }

    return 0;

}


int ShearBeamLT::getObjectSize()
{
    cerr << "ShearBeamLT::getObjectSize() -- Not yet implemented" << endl;

    return 0;
    /*
    int size = 0;
    size += 11 * sizeof( int );
    size += 4 * sizeof( double );
    for( int i = 0; i < 8; i++ )
    {
    size += material_array[i]->getObjectSize();
    }
    return size;
    */
}

//=============================================================================
void ShearBeamLT::Print( ostream &s, int flag )
{
    // Nothing to do

}




int ShearBeamLT::update( void )
{

    static DTensor2 trial_disp( 2, 3, 0.0  );
    static DTensor2 trial_strain(3, 3, 0.0);
    static DTensor2 dhGlobal( 2, 3, 0.0 );

    trial_disp *= 0;
    trial_strain *= 0;
    dhGlobal *= 0;

    const Vector &TotDis1 = theNodes[0]->getTrialDisp();
    const Vector &TotDis2 = theNodes[1]->getTrialDisp();

    trial_disp(0, 0) = TotDis1(0);
    trial_disp(0, 1) = TotDis1(1);
    trial_disp(0, 2) = TotDis1(2);
    trial_disp(1, 0) = TotDis2(0);
    trial_disp(1, 1) = TotDis2(1);
    trial_disp(1, 2) = TotDis2(2);

    dhGlobal(0, 2) = -1 / length;
    dhGlobal(1, 2) = 1 / length;

    trial_strain(i, j) = (dhGlobal(k, i) * trial_disp(k, j) + dhGlobal(k, j) * trial_disp(k, i)) / 2;

    if ( ( material_array[0]->setTrialStrain( trial_strain ) ) )
    {
        cerr << "ShearBeamLT::update (tag: " << this->getTag() << "), Update Failed\n";
        return -1;
    }
    return 0;
}



int
ShearBeamLT::CheckMesh( ofstream &checkmesh_file )
{


    return 0;
}



//==================================================================================
Vector *
ShearBeamLT::getStress( void )
{
    cout << "ShearBeamLT::getStress( void ) got called!\n\n";
    // DTensor2 stress;//(3,3,0.0);
    // Vector *stresses = new Vector( 6 );   // FIXME: Who deallocates this guy???

    // stress = material_array[0]->getStressTensor();
    // ( *stresses )( 0 ) = stress( 0, 0 );
    // ( *stresses )( 1 ) = stress( 1, 1 );
    // ( *stresses )( 2 ) = stress( 2, 2 );
    // ( *stresses )( 3 ) = stress( 0, 1 );
    // ( *stresses )( 4 ) = stress( 0, 2 );
    // ( *stresses )( 5 ) = stress( 0, 2 );

    // return stresses;
    return 0;
}


Matrix &ShearBeamLT::getGaussCoordinates(void)
{
    return gauss_points;
}

int ShearBeamLT::getOutputSize() const
{
    return ShearBeamLT_OUTPUT_SIZE;
}


const Vector &ShearBeamLT::getOutput()
{
    formOutput();
    return outputVector;
}

int ShearBeamLT::startNewStage()
{
    material_array[0]->startNewStage();
    return 0;
}

#endif

