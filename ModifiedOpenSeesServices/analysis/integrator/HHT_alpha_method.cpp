// Based on HHT_alpha_method implementation from Frank McKenna
// Nima Tafazzoli and Boris Jeremic, CompGeoMech UCD, May 2012

#include <HHT_alpha_method.h>
#include <FE_Element.h>
#include <LinearSOE.h>
#include <AnalysisModel.h>
#include <Vector.h>
#include <DOF_Group.h>
#include <DOF_GrpIter.h>
#include <AnalysisModel.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


HHT_alpha_method::HHT_alpha_method()
    : TransientIntegrator( INTEGRATOR_TAGS_HHT_alpha_method ),
      alpha( 0.0 ), gamma( 0.5 ), beta( 0.25 ),
      c1( 0.0 ), c2( 0.0 ), c3( 0.0 ),
      Ut( 0 ), Utdot( 0 ), Utdotdot( 0 ),  U( 0 ), Udot( 0 ), Udotdot( 0 ),
      Ualpha( 0 ), Udotalpha( 0 )
{

}

HHT_alpha_method::HHT_alpha_method( double _alpha )
    : TransientIntegrator( INTEGRATOR_TAGS_HHT_alpha_method ),
      alpha( _alpha ), gamma( 0.5 - _alpha ), beta( ( 1 - _alpha ) * ( 1 - _alpha ) * 0.25 ),
      c1( 0.0 ), c2( 0.0 ), c3( 0.0 ),
      Ut( 0 ), Utdot( 0 ), Utdotdot( 0 ),  U( 0 ), Udot( 0 ), Udotdot( 0 ),
      Ualpha( 0 ), Udotalpha( 0 )
{

}


HHT_alpha_method::HHT_alpha_method( double _alpha,
                                    double _beta,
                                    double _gamma )
    : TransientIntegrator( INTEGRATOR_TAGS_HHT_alpha_method ),
      alpha( _alpha ), gamma( _gamma ), beta( _beta ),
      c1( 0.0 ), c2( 0.0 ), c3( 0.0 ),
      Ut( 0 ), Utdot( 0 ), Utdotdot( 0 ),  U( 0 ), Udot( 0 ), Udotdot( 0 ),
      Ualpha( 0 ), Udotalpha( 0 )
{

}



HHT_alpha_method::~HHT_alpha_method()
{
    // clean up the memory created
    if ( Ut != 0 )
    {
        delete Ut;
    }

    if ( Utdot != 0 )
    {
        delete Utdot;
    }

    if ( Utdotdot != 0 )
    {
        delete Utdotdot;
    }

    if ( U != 0 )
    {
        delete U;
    }

    if ( Udot != 0 )
    {
        delete Udot;
    }

    if ( Udotdot != 0 )
    {
        delete Udotdot;
    }

    if ( Ualpha != 0 )
    {
        delete Ualpha;
    }

    if ( Udotalpha != 0 )
    {
        delete Udotalpha;
    }
}


int
HHT_alpha_method::initialize( void )
{
    return 0;
}


int
HHT_alpha_method::newStep( double deltaT )
{
    //std::cout << "HHT :: newStep() " << std::endl;
    if ( beta == 0 || gamma == 0 )
    {
        cerr << "HHT_alpha_method::newStep() - error in variable\n";
        cerr << "gamma = " << gamma << " beta= " << beta << endln;
        return -1;
    }

    if ( deltaT <= 0.0 )
    {
        cerr << "HHT_alpha_method::newStep() - error in variable\n";
        cerr << "dT = " << deltaT << endln;
        return -2;
    }

    // Update the constants of the method
    c1 = 1.0;
    c2 = 1.0 / (gamma * deltaT); //   Used to be: gamma / ( beta * deltaT ); renamed to c4  (Oct 2, 2012, JAbell)
    c3 = 1.0 / ( beta * deltaT * deltaT );
    c4 = gamma / ( beta * deltaT );

    a1 = ( 1.0 - gamma / beta );
    a2 = ( deltaT ) * ( 1.0 - 0.5 * gamma / beta );
    a3 = -1.0 / ( beta * deltaT );
    a4 = 1 - 0.5 / beta;

    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if ( U == 0 )
    {
        cerr << "HHT_alpha_method::newStep() - domainChange() failed or hasn't been called\n";
        return -3;
    }

    // set response at t to be that at t+delta t of previous step
    ( *Ut ) = *U;
    ( *Utdot ) = *Udot;
    ( *Utdotdot ) = *Udotdot;

    // set new velocity and accelerations at t + delta t


    Udot->addVector( a1, *Utdotdot, a2 );
    Udotdot->addVector( a4, *Utdot, a3 );

    ( *Ualpha ) = *Ut;
    ( *Udotalpha ) = *Utdot;

    //Ualpha->addVector((1+alpha), *U, -alpha);
    Udotalpha->addVector( ( 1 - alpha ), *Udot, alpha );

    // set the new trial response quantities

    theModel->setResponse( *Ualpha, *Udotalpha, *Udotdot );

    // increment the time and apply the load
    double time = theModel->getCurrentDomainTime();
    time += deltaT;

    if ( theModel->updateDomain( time, deltaT ) < 0 )
    {
        cerr << "HHT::newStep() - failed to update the domain\n";
        return -4;
    }

    return 0;
}



int
HHT_alpha_method::formEleTangent( FE_Element* theEle )
{
    // J. Abell : Added de (alpha+1.0) factor which was missing from the C and K matrices. (Aug 31, 2012)
    theEle->zeroTangent();

    if ( statusFlag == CURRENT_TANGENT )
    {
        theEle->addKtToTang( c1 * ( alpha + 1.0 ) );
        theEle->addCtoTang( c2 * ( alpha + 1.0 ) );
        theEle->addMtoTang( c3 );
    }
    else if ( statusFlag == INITIAL_TANGENT )
    {
        // J. Abell : Added de (alpha+1.0) factor which was missing from the C and K matrices. (Aug 31, 2012)
        theEle->addKiToTang( c1 * ( alpha + 1.0 ) );
        theEle->addCtoTang( c2 * ( alpha + 1.0 ) );
        theEle->addMtoTang( c3 );
    }

    return 0;
}



int
HHT_alpha_method::formNodTangent( DOF_Group* theDof )
{
    // J. Abell : Added de (alpha+1.0) factor which was missing from the C and K matrices. (Aug 31, 2012)
    theDof->zeroTangent();
    theDof->addMtoTang( c3 );
    theDof->addCtoTang( c2 * ( alpha + 1.0 ) );

    return( 0 );
}



int
HHT_alpha_method::domainChanged()
{
    AnalysisModel* myModel = this->getAnalysisModelPtr();
    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    const Vector& x = theLinSOE->getX();
    int size = x.Size();

    // if damping factors exist set them in the ele & node of the domain
    //  if (alphaM != 0.0 || betaK != 0.0 || betaKi != 0.0 || betaKc != 0.0)
    //    myModel->setRayleighDampingFactors(alphaM, betaK, betaKi, betaKc);

    // create the new Vector objects
    if ( Ut == 0 || Ut->Size() != size )
    {

        // delete the old
        if ( Ut != 0 )
        {
            delete Ut;
        }

        if ( Utdot != 0 )
        {
            delete Utdot;
        }

        if ( Utdotdot != 0 )
        {
            delete Utdotdot;
        }

        if ( U != 0 )
        {
            delete U;
        }

        if ( Udot != 0 )
        {
            delete Udot;
        }

        if ( Udotdot != 0 )
        {
            delete Udotdot;
        }

        if ( Ualpha != 0 )
        {
            delete Ualpha;
        }

        if ( Udotalpha != 0 )
        {
            delete Udotalpha;
        }

        // create the new
        Ut          = new Vector( size );
        Utdot       = new Vector( size );
        Utdotdot    = new Vector( size );
        U           = new Vector( size );
        Udot        = new Vector( size );
        Udotdot     = new Vector( size );
        Ualpha      = new Vector( size );
        Udotalpha   = new Vector( size );

        // check we obtained the new
        if ( Ut == 0 || Ut->Size() != size ||
                Utdot == 0 || Utdot->Size() != size ||
                Utdotdot == 0 || Utdotdot->Size() != size ||
                U == 0 || U->Size() != size ||
                Udot == 0 || Udot->Size() != size ||
                Udotdot == 0 || Udotdot->Size() != size ||
                Ualpha == 0 || Ualpha->Size() != size ||
                Udotalpha == 0 || Udotalpha->Size() != size )
        {

            cerr << "HHT_alpha_method::domainChanged - ran out of memory\n";

            // delete the old
            if ( Ut != 0 )
            {
                delete Ut;
            }

            if ( Utdot != 0 )
            {
                delete Utdot;
            }

            if ( Utdotdot != 0 )
            {
                delete Utdotdot;
            }

            if ( U != 0 )
            {
                delete U;
            }

            if ( Udot != 0 )
            {
                delete Udot;
            }

            if ( Udotdot != 0 )
            {
                delete Udotdot;
            }

            if ( Ualpha != 0 )
            {
                delete Ualpha;
            }

            if ( Udotalpha != 0 )
            {
                delete Udotalpha;
            }

            Ut = 0;
            Utdot = 0;
            Utdotdot = 0;
            U = 0;
            Udot = 0;
            Udotdot = 0;
            Udotalpha = 0;
            Ualpha = 0;
            return -1;
        }
    }

    // now go through and populate U, Udot and Udotdot by iterating through
    // the DOF_Groups and getting the last committed velocity and accel

    DOF_GrpIter& theDOFs = myModel->getDOFs();
    DOF_Group* dofPtr;

    while ( ( dofPtr = theDOFs() ) != 0 )
    {
        const ID& id = dofPtr->getID();
        int idSize = id.Size();


        int i;
        const Vector& disp = dofPtr->getCommittedDisp();

        for ( i = 0; i < idSize; i++ )
        {
            int loc = id( i );

            if ( loc >= 0 )
            {
                ( *U )( loc ) = disp( i );
            }
        }

        const Vector& vel = dofPtr->getCommittedVel();

        for ( i = 0; i < idSize; i++ )
        {
            int loc = id( i );

            if ( loc >= 0 )
            {
                ( *Udot )( loc ) = vel( i );
            }
        }

        const Vector& accel = dofPtr->getCommittedAccel();

        for ( i = 0; i < idSize; i++ )
        {
            int loc = id( i );

            if ( loc >= 0 )
            {
                ( *Udotdot )( loc ) = accel( i );
            }
        }

        /** NOTE WE CAN't DO TOGETHER BECAUSE DOF_GROUPS USING SINGLE VECTOR ******
        for (int i=0; i < id.Size(); i++) {
          int loc = id(i);
          if (loc >= 0) {
        (*U)(loc) = disp(i);
        (*Udot)(loc) = vel(i);
        (*Udotdot)(loc) = accel(i);
          }
        }
        *******************************************************************************/

    }

    return 0;
}


int
HHT_alpha_method::update( const Vector& deltaU )
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if ( theModel == 0 )
    {
        cerr << "WARNING HHT_alpha_method::update() - no AnalysisModel set\n";
        return -1;
    }

    // check domainChanged() has been called, i.e. Ut will not be zero
    if ( Ut == 0 )
    {
        cerr << "WARNING HHT_alpha_method::update() - domainChange() failed or not called\n";
        return -2;
    }

    // check deltaU is of correct size
    if ( deltaU.Size() != U->Size() )
    {
        cerr << "WARNING HHT_alpha_method::update() - Vectors of incompatable size ";
        cerr << " expecting " << U->Size() << " obtained " << deltaU.Size() << endln;
        return -3;
    }

    //  determine the response at t+deltaT
    ( *U ) += deltaU;

    Udot->addVector( 1.0, deltaU, c4 );
    Udotdot->addVector( 1.0, deltaU, c3 );

    // determine displacement and velocity at t+alpha*deltaT
    ( *Ualpha ) = *Ut;
    ( *Udotalpha ) = *Utdot;

    Ualpha->addVector( ( 1.0 - alpha ), *U, alpha );
    Udotalpha->addVector( ( 1.0 - alpha ), *Udot, alpha );


    // update the responses at the DOFs
    theModel->setResponse( *Ualpha, *Udotalpha, *Udotdot );

    if ( theModel->updateDomain() < 0 )
    {
        cerr << "HHT_alpha_method::update() - failed to update the domain\n";
        return -4;
    }

    return 0;
}

int
HHT_alpha_method::commit( void )
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if ( theModel == 0 )
    {
        cerr << "WARNING HHT_alpha_method::commit() - no AnalysisModel set\n";
        return -1;
    }

    // update the responses at the DOFs
    theModel->setResponse( *U, *Udot, *Udotdot );
    theModel->updateDomain();

    return theModel->commitDomain();
}

int
HHT_alpha_method::sendSelf( int cTag, Channel& theChannel )
{
    //     Vector data(8);
    Vector data( 3 );
    data( 0 ) = alpha;
    data( 1 ) = beta;
    data( 2 ) = gamma;
    //     data(3) = 1.0;
    //     data(4) = alphaM;
    //     data(5) = betaK;
    //     data(6) = betaKi;
    //     data(7) = betaKc;

    if ( theChannel.sendVector( this->getDbTag(), cTag, data ) < 0 )
    {
        cerr << "WARNING HHT_alpha_method::sendSelf() - could not send data\n";
        return -1;
    }

    return 0;
}

int
HHT_alpha_method::receiveSelf( int cTag, Channel& theChannel, FEM_ObjectBroker& theBroker )
{
    //     Vector data(8);
    Vector data( 4 );

    if ( theChannel.receiveVector( this->getDbTag(), cTag, data ) < 0 )
    {
        cerr << "WARNING HHT_alpha_method::receiveSelf() - could not receive data\n";
        return -1;
    }

    alpha = data( 0 );
    beta = data( 1 );
    gamma = data( 2 );
    //     alpha = data(0);
    //     beta = data(1);
    //     gamma = data(2);

    //     alphaM = data(4);
    //     betaK = data(5);
    //     betaKi = data(6);
    //     betaKc = data(7);

    return 0;
}

void
HHT_alpha_method::Print( ostream& s, int flag )
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if ( theModel != 0 )
    {
        double currentTime = theModel->getCurrentDomainTime();
        s << "\t HHT_alpha_method - currentTime: " << currentTime << endln;
        s << "  alpha: " << alpha << " beta: " << beta  << " gamma: " << gamma << endln;
        s << "  c1: " << c1 << " c2: " << c2 << " c3: " << c3 << endln;
        //        s << "  Rayleigh Damping - alphaM: " << alphaM;
        //        s << "  betaK: " << betaK << "   betaKi: " << betaKi << endln;
    }
    else
    {
        s << "\t HHT_alpha_method - no associated AnalysisModel\n";
    }
}

