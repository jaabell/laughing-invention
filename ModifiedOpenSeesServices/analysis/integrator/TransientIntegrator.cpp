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

// $Revision: 1.7 $
// $Date: 2003/03/06 20:32:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/integrator/TransientIntegrator.cpp,v $


// File: ~/analysis/integrator/TransientIntegrator.C
//
// Written: fmk
// Created: Tue Sept 17 15:54:47: 1996
// Revision: A
//
// Description: This file contains the class definition for TransientIntegrator.
// TransientIntegrator is an algorithmic class for setting up the finite element
// equations for a static analysis and for Incrementing the nodal displacements
// with the values in the soln vector to the LinearSOE object.
//
// What: "@(#) TransientIntegrator.C, revA"

#include <TransientIntegrator.h>
#include <FE_Element.h>
#include <LinearSOE.h>
#include <AnalysisModel.h>
#include <Vector.h>
#include <DOF_Group.h>
#include <FE_EleIter.h>
#include <DOF_GrpIter.h>
#include <iostream>
using namespace std;

#ifdef _PARALLEL_PROCESSING
#include <mpi.h>
#endif

#include <time.h>



TransientIntegrator::TransientIntegrator(int clasTag)
    : IncrementalIntegrator(clasTag)
{

}

TransientIntegrator::~TransientIntegrator()
{

}

int
TransientIntegrator::formTangent(int statFlag)
{
    int result = 0;
    statusFlag = statFlag;

    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theLinSOE == 0 || theModel == 0)
    {
        cerr << "WARNING TransientIntegrator::formTangent() ";
        cerr << "no LinearSOE or AnalysisModel has been set\n";
        return -1;
    }

    // the loops to form and add the tangents are broken into two for
    // efficiency when performing parallel computations
    theLinSOE->zeroA();

    // loop through the DOF_Groups and add the unbalance
    DOF_GrpIter& theDOFs = theModel->getDOFs();
    DOF_Group* dofPtr;

    while ((dofPtr = theDOFs()) != 0)
    {
        if (theLinSOE->addA(dofPtr->getTangent(this), dofPtr->getID()) < 0)
        {
            cerr << "TransientIntegrator::formTangent() - failed to addA:dof\n";
            result = -1;
        }
    }

    // loop through the FE_Elements getting them to add the tangent
    FE_EleIter& theEles2 = theModel->getFEs();
    FE_Element* elePtr;

    while((elePtr = theEles2()) != 0)
    {
        if (theLinSOE->addA(elePtr->getTangent(this), elePtr->getID()) < 0)
        {
            cerr << "TransientIntegrator::formTangent() - failed to addA:ele\n";
            result = -2;
        }
    }

    return result;
}




int
TransientIntegrator::formEleResidual(FE_Element* theEle)
{
    theEle->zeroResidual();
    theEle->addRIncInertiaToResidual();
    return 0;
}

int
TransientIntegrator::formNodUnbalance(DOF_Group* theDof)
{
    theDof->zeroUnbalance();
    theDof->addPIncInertiaToUnbalance();
    return 0;
}

//-------------------------------------------------------------//

//Added by Babak for Eigen Analysis ... 1/15/13
int
TransientIntegrator::formK()
{

    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theLinSOE == 0 || theModel == 0)
    {
        cerr << "WARNING TransientIntegrator::formTangent() ";
        cerr << "no LinearSOE or AnalysisModel has been set\n";
        return -1;
    }

    // the loops to form and add the tangents are broken into two for
    // efficiency when performing parallel computations

    // loop through the FE_Elements getting them to form the tangent
    // FE_EleIter &theEles1 = theAnalysisModel->getFEs();
    FE_Element* elePtr;

    flagK = 0;

    //theLinSOE->zeroK();

    //while((elePtr = theEles1()) != 0)
    //  elePtr->formTangent(this);

    // loop through the FE_Elements getting them to add the tangent
    int result = 0;
    FE_EleIter& theEles2 = theModel->getFEs();

    while((elePtr = theEles2()) != 0)
    {
        this->formEleTangK(elePtr); //Forming the stiffness in element to be added to mass matix
        //if (theLinSOE->addK(elePtr->getTangent(this), elePtr->getID()) < 0) {
        cerr << "WARNING TransientIntegrator::formK -";
        cerr << " failed in addK for ID " << elePtr->getID();
        result = -2;
        //}
    }

    return result;
}

//-------------------------------------------------------------//

//Added by Babak for Eigen Analysis ... 1/15/13

int
TransientIntegrator::formM()
{
    LinearSOE* theLinSOE = this->getLinearSOEPtr();
    AnalysisModel* theModel = this->getAnalysisModelPtr();

    if (theLinSOE == 0 || theModel == 0)
    {
        cerr << "WARNING TransientIntegrator::formTangent() ";
        cerr << "no LinearSOE or AnalysisModel has been set\n";
        return -1;
    }


    // the loops to form and add the tangents are broken into two for
    // efficiency when performing parallel computations

    // loop through the FE_Elements getting them to form the tangent
    // FE_EleIter &theEles1 = theAnalysisModel->getFEs();
    FE_Element* elePtr;

    flagK = 1;
    //theLinSOE->zeroM();

    // while((elePtr = theEles1()) != 0)
    //     elePtr->formTangent(this);

    // loop through the FE_Elements getting them to add the tangent
    int result = 0;
    FE_EleIter& theEles2 = theModel->getFEs();

    while((elePtr = theEles2()) != 0)
    {
        this->formEleTangM(elePtr); //Forming the mass in element to be added to mass matix
        //if (theLinSOE->addM(elePtr->getTangent(this), elePtr->getID()) < 0)
        //{
        cerr << "WARNING TransientIntegrator::formM -";
        cerr << " failed in addA for ID " << elePtr->getID();
        result = -2;
        //}
    }

    DOF_Group* dofPtr;
    DOF_GrpIter& theDofs = theModel->getDOFs();

    while((dofPtr = theDofs()) != 0)
    {
        //      dofPtr->formTangent(this);
        this->formNodTangM(dofPtr); // Forming the mass in dof group to be added to mass matix
        //if (theLinSOE->addM(dofPtr->getTangent(this),dofPtr->getID()) < 0)
        //{
        cerr << "WARNING TransientIntegrator::formM -";
        cerr << " failed in addM for ID " << dofPtr->getID();
        result = -3;
        //}
    }

    return result;
}

//-------------------------------------------------------------//

//Added by Babak for Eigen Analysis ... 1/15/13
int
TransientIntegrator::formEleTangK(FE_Element* theEle)
{
    theEle->zeroTangent();
    theEle->addKtToTang(1.0);
    return 0;
}


//-------------------------------------------------------------//

//Added by Babak for Eigen Analysis ... 1/15/13
int
TransientIntegrator::formEleTangM(FE_Element* theEle)
{
    theEle->zeroTangent();
    theEle->addMtoTang(1.0);
    return 0;
}



//-------------------------------------------------------------//

//Added by Babak for Eigen Analysis ... 1/15/13
int
TransientIntegrator::formNodTangM(DOF_Group* theDof)
{
    theDof->zeroTangent();
    theDof->addMtoTang(1.0);
    return 0;
}