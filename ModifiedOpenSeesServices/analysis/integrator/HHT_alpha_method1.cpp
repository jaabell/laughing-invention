// Based on HHT implementation from Frank McKenna and Andreas Schellenberg
// Boris Jeremic, CompGeoMech UCD, 30April2009


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


// Nima Tafazzoli removed rayleigh factors (March 2010)
// default constructor
HHT_alpha_method::HHT_alpha_method()
    : TransientIntegrator(INTEGRATOR_TAGS_HHT_alpha_method),
      alpha(0.0),
      beta(0.0),
      gamma(0.0),
      deltaT(0.0),
      //    alphaM(0.0),
      //    betaK(0.0),
      //    betaKi(0.0),
      //    betaKc(0.0),
      c1(0.0),
      c2(0.0),
      c3(0.0),
      Ut(0),
      Utdot(0),
      Utdotdot(0),
      U(0),
      Udot(0),
      Udotdot(0),
      Ualpha(0),
      Ualphadot(0)
{

}




HHT_alpha_method::HHT_alpha_method(double _alpha,
                                   double _beta,
                                   double _gamma)
    : TransientIntegrator(INTEGRATOR_TAGS_HHT_alpha_method),
      alpha(_alpha + 1.0),
      beta(_beta),
      gamma(_gamma),
      deltaT(0.0),
      //      alphaM(0.0),
      //      betaK(0.0),
      //      betaKi(0.0),
      //      betaKc(0.0),
      c1(0.0),
      c2(0.0),
      c3(0.0),
      Ut(0),
      Utdot(0),
      Utdotdot(0),
      U(0),
      Udot(0),
      Udotdot(0),
      Ualpha(0),
      Ualphadot(0)
{

}


// HHT_alpha_method::HHT_alpha_method(double _alpha,
//                                    double _beta,
//                                    double _gamma,
//                                    double _alphaM,
//                                    double _betaK,
//                                    double _betaKi,
//                                    double _betaKc)
//     : TransientIntegrator(INTEGRATOR_TAGS_HHT_alpha_method),
//     alpha(_alpha+1.0),
//     beta(_beta),
//     gamma(_gamma),
//     deltaT(0.0),
//     alphaM(_alphaM),
//     betaK(_betaK),
//     betaKi(_betaKi),
//     betaKc(_betaKc),
//     c1(0.0),
//     c2(0.0),
//     c3(0.0),
//     Ut(0),
//     Utdot(0),
//     Utdotdot(0),
//     U(0),
//     Udot(0),
//     Udotdot(0),
//     Ualpha(0),
//     Ualphadot(0)
// {
//
// }


HHT_alpha_method::~HHT_alpha_method()
{
    // clean up the memory created
    if (Ut != 0)
    {
        delete Ut;
    }

    if (Utdot != 0)
    {
        delete Utdot;
    }

    if (Utdotdot != 0)
    {
        delete Utdotdot;
    }

    if (U != 0)
    {
        delete U;
    }

    if (Udot != 0)
    {
        delete Udot;
    }

    if (Udotdot != 0)
    {
        delete Udotdot;
    }

    if (Ualpha != 0)
    {
        delete Ualpha;
    }

    if (Ualphadot != 0)
    {
        delete Ualphadot;
    }
}


int HHT_alpha_method::newStep(double _deltaT)
{
    deltaT = _deltaT;

    if (beta == 0 || gamma == 0 )
    {
        cerr << "HHT_alpha_method::newStep() - error in variable\n";
        cerr << "gamma = " << gamma << " beta = " << beta << endln;
        return -1;
    }

    if (deltaT <= 0.0)
    {
        cerr << "HHT_alpha_method::newStep() - error in variable\n";
        cerr << "dT = " << deltaT << endln;
        return -2;
    }

    // get a pointer to the AnalysisModel
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    // set the constants
    c1 = 1.0;
    c2 = gamma / (beta * deltaT);
    c3 = 1.0 / (beta * deltaT * deltaT);

    if (U == 0)
    {
        cerr << "HHT_alpha_method::newStep() - domainChange() failed or hasn't been called\n";
        return -3;
    }

    // set response at t to be that at t+deltaT of previous step
    (*Ut) = *U;
    (*Utdot) = *Udot;
    (*Utdotdot) = *Udotdot;

    // increment the time to t+alpha*deltaT and apply the load
    double time = theModel->getCurrentDomainTime();
    time += alpha * deltaT; //BJ: original from HHT, this is how it was before change

    //    time += (alpha+1.0)*deltaT; //BJ: new form, changed
    //    theModel->applyLoadDomain(time);
    if (theModel->updateDomain(time, deltaT) < 0)
    {
        cerr << "HHT_alpha_method::newStep() - failed to update the domain\n";
        return -4;
    }

    // determine new velocities and accelerations at t+deltaT
    double a1 = (1.0 - gamma / beta);
    double a2 = deltaT * (1.0 - 0.5 * gamma / beta);
    Udot->addVector(a1, *Utdotdot, a2);

    double a3 = -1.0 / (beta * deltaT);
    double a4 = 1.0 - 0.5 / beta;
    Udotdot->addVector(a4, *Utdot, a3);

    // determine the velocities at t+alpha*deltaT
    (*Ualphadot) = *Utdot;
    Ualphadot->addVector((1.0 - alpha), *Udot, alpha); //BJ: original from HHT, this is how it was before change
    //    Ualphadot->addVector((2.0-alpha), *Udot, (alpha+1.0)); //BJ: new form, changed

    // set the trial response quantities for the nodes
    //     theModel->setVel(*Ualphadot);
    //     theModel->setAccel(*Udotdot);


    (*Ualpha) = *Ut;
    theModel->setResponse(*Ualpha, *Ualphadot, *Udotdot);


    return 0;
}


int HHT_alpha_method::revertToLastStep()
{
    // set response at t+deltaT to be that at t .. for next step
    if (U != 0)
    {
        (*U) = *Ut;
        (*Udot) = *Utdot;
        (*Udotdot) = *Utdotdot;
    }

    return 0;
}


int HHT_alpha_method::formEleTangent(FE_Element* theEle)
{
    theEle->zeroTangent();

    if (statusFlag == CURRENT_TANGENT)
    {
        theEle->addKtToTang(alpha * c1); //BJ: original from HHT, this is how it was before change
        //        theEle->addKtToTang((alpha+1.0)*c1); //BJ: new form, changed
        theEle->addCtoTang((alpha + 1.0)*c2);//BJ: original from HHT, this is how it was before change
        //        theEle->addCtoTang((alpha+1.0)*c2); //BJ: new form, changed
        theEle->addMtoTang(c3);
    }
    else if (statusFlag == INITIAL_TANGENT)
    {
        theEle->addKiToTang(alpha * c1); //BJ: original from HHT, this is how it was before change
        //        theEle->addKiToTang((alpha+1.0)*c1);//BJ: new form, changed
        theEle->addCtoTang((alpha + 1.0)*c2); //BJ: original from HHT, this is how it was before change
        //        theEle->addCtoTang((alpha+1.0)*c2); //BJ: new form, changed
        theEle->addMtoTang(c3);
    }

    return 0;
}


int HHT_alpha_method::formNodTangent(DOF_Group* theDof)
{
    theDof->zeroTangent();

    theDof->addCtoTang((alpha + 1.0)*c2);//BJ: original from HHT, this is how it was before change
    //    theDof->addCtoTang((alpha+1.0)*c2); //BJ: new form, changed
    theDof->addMtoTang(c3);

    return 0;
}


int HHT_alpha_method::domainChanged()
{
    AnalysisModel* myModel = this->getAnalysisModelPtr();
    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    const Vector& x = theLinSOE->getX();
    int size = x.Size();

    // Nima Tafazzoli removed (March 2010)
    // if damping factors exist set them in the ele & node of the domain
    //     if (alphaM != 0.0 || betaK != 0.0 || betaKi != 0.0 || betaKc != 0.0)
    //         myModel->setRayleighDampingFactors(alphaM, betaK, betaKi, betaKc);

    // create the new Vector objects
    if (Ut == 0 || Ut->Size() != size)
    {

        // delete the old
        if (Ut != 0)
        {
            delete Ut;
        }

        if (Utdot != 0)
        {
            delete Utdot;
        }

        if (Utdotdot != 0)
        {
            delete Utdotdot;
        }

        if (U != 0)
        {
            delete U;
        }

        if (Udot != 0)
        {
            delete Udot;
        }

        if (Udotdot != 0)
        {
            delete Udotdot;
        }

        if (Ualpha != 0)
        {
            delete Ualpha;
        }

        if (Ualphadot != 0)
        {
            delete Ualphadot;
        }

        // create the new
        Ut = new Vector(size);
        Utdot = new Vector(size);
        Utdotdot = new Vector(size);
        U = new Vector(size);
        Udot = new Vector(size);
        Udotdot = new Vector(size);
        Ualpha = new Vector(size);
        Ualphadot = new Vector(size);

        // check we obtained the new
        if (Ut == 0 || Ut->Size() != size ||
                Utdot == 0 || Utdot->Size() != size ||
                Utdotdot == 0 || Utdotdot->Size() != size ||
                U == 0 || U->Size() != size ||
                Udot == 0 || Udot->Size() != size ||
                Udotdot == 0 || Udotdot->Size() != size ||
                Ualpha == 0 || Ualpha->Size() != size ||
                Ualphadot == 0 || Ualphadot->Size() != size)
        {

            cerr << "HHT_alpha_method::domainChanged - ran out of memory\n";

            // delete the old
            if (Ut != 0)
            {
                delete Ut;
            }

            if (Utdot != 0)
            {
                delete Utdot;
            }

            if (Utdotdot != 0)
            {
                delete Utdotdot;
            }

            if (U != 0)
            {
                delete U;
            }

            if (Udot != 0)
            {
                delete Udot;
            }

            if (Udotdot != 0)
            {
                delete Udotdot;
            }

            if (Ualpha != 0)
            {
                delete Ualpha;
            }

            if (Ualphadot != 0)
            {
                delete Ualphadot;
            }

            Ut = 0;
            Utdot = 0;
            Utdotdot = 0;
            U = 0;
            Udot = 0;
            Udotdot = 0;
            Ualpha = 0;
            Ualphadot = 0;

            return -1;
        }
    }

    // now go through and populate U, Udot and Udotdot by iterating through
    // the DOF_Groups and getting the last committed velocity and accel
    DOF_GrpIter& theDOFs = myModel->getDOFs();
    DOF_Group* dofPtr;

    while ((dofPtr = theDOFs()) != 0)
    {
        const ID& id = dofPtr->getID();
        int idSize = id.Size();

        int i;
        const Vector& disp = dofPtr->getCommittedDisp();

        for (i = 0; i < idSize; i++)
        {
            int loc = id(i);

            if (loc >= 0)
            {
                (*U)(loc) = disp(i);
            }
        }

        const Vector& vel = dofPtr->getCommittedVel();

        for (i = 0; i < idSize; i++)
        {
            int loc = id(i);

            if (loc >= 0)
            {
                (*Udot)(loc) = vel(i);
            }
        }

        const Vector& accel = dofPtr->getCommittedAccel();

        for (i = 0; i < idSize; i++)
        {
            int loc = id(i);

            if (loc >= 0)
            {
                (*Udotdot)(loc) = accel(i);
            }
        }
    }

    return 0;
}


int HHT_alpha_method::update(const Vector& deltaU)
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel == 0)
    {
        cerr << "WARNING HHT_alpha_method::update() - no AnalysisModel set\n";
        return -1;
    }

    // check domainChanged() has been called, i.e. Ut will not be zero
    if (Ut == 0)
    {
        cerr << "WARNING HHT_alpha_method::update() - domainChange() failed or not called\n";
        return -2;
    }

    // check deltaU is of correct size
    if (deltaU.Size() != U->Size())
    {
        cerr << "WARNING HHT_alpha_method::update() - Vectors of incompatible size ";
        cerr << " expecting " << U->Size() << " obtained " << deltaU.Size() << endln;
        return -3;
    }

    //  determine the response at t+deltaT
    (*U) += deltaU;

    Udot->addVector(1.0, deltaU, c2);

    Udotdot->addVector(1.0, deltaU, c3);

    // determine displacement and velocity at t+alpha*deltaT
    (*Ualpha) = *Ut;
    Ualpha->addVector((1.0 - alpha), *U, alpha); //BJ: original from HHT, this is how it was before change
    //    Ualpha->addVector((2.0-alpha), *U, (alpha+1.0));  //BJ: new form, changed

    (*Ualphadot) = *Utdot;
    Ualphadot->addVector((1.0 - alpha), *Udot, alpha); //BJ: original from HHT, this is how it was before change
    //    Ualphadot->addVector((2.0-alpha), *Udot, (alpha+1.0)); //BJ: new form, changed

    // update the response at the DOFs
    theModel->setResponse(*Ualpha, *Ualphadot, *Udotdot);

    if (theModel->updateDomain() < 0)
    {
        cerr << "HHT_alpha_method::update() - failed to update the domain\n";
        return -4;
    }

    return 0;
}


int HHT_alpha_method::commit(void)
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel == 0)
    {
        cerr << "WARNING HHT_alpha_method::commit() - no AnalysisModel set\n";
        return -1;
    }

    // update the response at the DOFs
    theModel->setResponse(*U, *Udot, *Udotdot);
    //    if (theModel->updateDomain() < 0)  {
    //        cerr << "HHT_alpha_method::commit() - failed to update the domain\n";
    //        return -4;
    //    }

    // set the time to be t+deltaT
    double time = theModel->getCurrentDomainTime();
    time += (1.0 - alpha) * deltaT; //BJ: original from HHT , this is how it was before change
    //    time += (2.0-alpha)*deltaT;        //BJ: new form, changed
    theModel->setCurrentDomainTime(time);

    return theModel->commitDomain();
}


int HHT_alpha_method::sendSelf(int cTag, Channel& theChannel)
{
    //    Vector data(7);
    Vector data(3);
    //    data(0) = alpha;//BJ: original from HHT , this is how it was before change
    data(0) = alpha;//I leave it like this, this is the actual alpha from input, right one
    data(1) = beta;
    data(2) = gamma;
    // Nima Tafazzoli (March 2010)
    //     data(3) = alphaM;
    //     data(4) = betaK;
    //     data(5) = betaKi;
    //     data(6) = betaKc;

    if (theChannel.sendVector(this->getDbTag(), cTag, data) < 0)
    {
        cerr << "WARNING HHT_alpha_method::sendSelf() - could not send data\n";
        return -1;
    }

    return 0;
}


int HHT_alpha_method::receiveSelf(int cTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
{
    //    Vector data(7);
    Vector data(3);

    if (theChannel.receiveVector(this->getDbTag(), cTag, data) < 0)
    {
        cerr << "WARNING HHT_alpha_method::receiveSelf() - could not receive data\n";
        return -1;
    }

    //    alpha  = data(0);//BJ: original from HHT  , this is how it was before change
    alpha  = data(0);// I leave it like this, this is the actual alpha from input, right one
    beta   = data(1);
    gamma  = data(2);

    // Nima Tafazzoli (March 2010)
    //     alphaM = data(3);
    //     betaK  = data(4);
    //     betaKi = data(5);
    //     betaKc = data(6);

    return 0;
}


void HHT_alpha_method::Print(ostream& s, int flag)
{
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theModel != 0)
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


