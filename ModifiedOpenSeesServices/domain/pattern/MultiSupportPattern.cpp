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

// $Revision: 1.2 $
// $Date: 2003/02/14 23:01:00 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/pattern/MultiSupportPattern.cpp,v $

// Written: fmk 11/00
// Revised:
//
// Purpose: This file contains the class definition for MultiSupportPattern.
// MultiSupportPattern is an abstract class.

#include <MultiSupportPattern.h>
#include <GroundMotion.h>
# include <ImposedMotionSP.h>

#include <Domain.h>
#include <SP_Constraint.h>
#include <SP_ConstraintIter.h>
#include <stdlib.h>
#include <Channel.h>
#include <ErrorHandler.h>
#include <ArrayOfTaggedObjects.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


MultiSupportPattern::MultiSupportPattern(int tag, int _classTag)
    : LoadPattern(tag, _classTag),
      theMotions(0), theMotionTags(0, 16), numMotions(0)
{

}


MultiSupportPattern::MultiSupportPattern(int tag)
    : LoadPattern(tag, PATTERN_TAG_MultiSupportPattern),
      theMotions(0), theMotionTags(0, 16), numMotions(0)
{

}


MultiSupportPattern::~MultiSupportPattern()
{
    // invoke the destructor on all ground motions supplied
    for (int i = 0; i < numMotions; i++)
    {
        delete theMotions[i];
    }

    if (theMotions != 0)
        //    free ((void *)theMotions);
    {
        delete [] theMotions;
    }
}



void
MultiSupportPattern::applyLoad(double time)
{
    SP_Constraint* sp;
    SP_ConstraintIter& theIter = this->getSPs();

    while ((sp = theIter()) != 0)
    {
        sp->applyConstraint(time);
    }
}


int
MultiSupportPattern::addMotion(GroundMotion& theMotion, int tag)
{
    // ensure no motion with given tag already added
    if (theMotionTags.getLocation(tag) >= 0)
    {
        cerr << "MultiSupportPattern::addMotion - could not add new, motion wih same tag exists\n";
        return -1;
    }

    // make space for new
    GroundMotion** newMotions = new GroundMotion *[numMotions + 1];

    //  GroundMotion **newMotions = (GroundMotion **)malloc(sizeof(GroundMotion *)*(numMotions+1));
    if (newMotions == 0)
    {
        cerr << "MultiSupportPattern::addMotion - could not add new, out of mem\n";
        return -1;
    }

    // copy old
    for (int i = 0; i < numMotions; i++)
    {
        newMotions[i] = theMotions[i];
    }

    // add the new motion to new
    newMotions[numMotions] = &theMotion;

    // delete the old
    if (theMotions != 0)
    {
        delete [] theMotions;
    }

    // reset
    theMotions = newMotions;
    theMotionTags[numMotions] = tag;
    numMotions++;

    return 0;
}



GroundMotion*
MultiSupportPattern::getMotion(int tag)
{
    int loc = theMotionTags.getLocation(tag);

    if (loc < 0)
    {
        return 0;
    }
    else
    {
        return theMotions[loc];
    }

}


bool
MultiSupportPattern::addNodalLoad(NodalLoad*)
{
    cerr << "MultiSupportPattern::addNodalLoad() - cannot add NodalLoad to EQ pattern\n";
    return false;
}

bool
MultiSupportPattern::addElementalLoad(ElementalLoad*)
{
    cerr << "MultiSupportPattern::addElementalLoad() - cannot add ElementalLoad to EQ pattern\n";
    return false;
}



int
MultiSupportPattern::sendSelf(int commitTag, Channel& theChannel)
{
    //Guanzhou added
    int dbTag = this->getDbTag();

    static ID data(3);
    data(0) = this->getTag();
    data(1) = numMotions;
    const int numSp = theSPs->getNumComponents();
    data(2) = numSp;

    static ID motionClassTag(numMotions);
    static ID motionDbTag(numMotions);

    for (int i = 0; i < numMotions; i++)
    {
        motionDbTag(i) = theMotions[i]->getDbTag();
        motionClassTag(i) = theMotions[i]->getClassTag();

        if (motionDbTag(i) == 0)
        {
            motionDbTag(i) = theChannel.getDbTag();
            theMotions[i]->setDbTag(motionDbTag(i));
        }
    }

    int res = theChannel.sendID(dbTag, commitTag, data);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::sendSelf() - channel failed to send data\n";
        return res;
    }

    res = theChannel.sendID(dbTag, commitTag, motionClassTag);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::sendSelf() - channel failed to send motionClassTag\n";
        return res;
    }

    res = theChannel.sendID(dbTag, commitTag, motionDbTag);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::sendSelf() - channel failed to send motionDbTag\n";
        return res;
    }

    for (int i = 0; i < numMotions; i++)
    {
        res = theMotions[i]->sendSelf(commitTag, theChannel);

        if (res < 0)
        {
            cerr << "MultiSupportPattern::sendSelf() - ground motion to send self\n";
            return res;
        }
    }

    //static ID myTag(numSp);
    static ID MotionIDs(numSp);
    //static ID NodeTags(numSp);
    //static ID DOFTags(numSp);

    //Domain *theDomain = this->getDomain();

    res = theChannel.sendID(dbTag, commitTag, theMotionTags);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::sendSelf() - channel failed to send theMotionTags\n";
        return res;
    }

    //res = theChannel.sendID(dbTag, commitTag, myTag);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::sendSelf() - channel failed to send myTag\n";
    //  return res;
    //}

    res = theChannel.sendID(dbTag, commitTag, MotionIDs);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::sendSelf() - channel failed to send MotionIDs\n";
        return res;
    }

    SP_ConstraintIter& theIter = this->getSPs();
    //SP_Constraint *sp;
    ImposedMotionSP* sp;

    //int count = 0;
    while ((sp = (ImposedMotionSP*)theIter()) != 0)
    {
        sp->sendSelf(commitTag, theChannel);
        //myTag(count) = sp->getTag();
        //MotionIDs(count) = sp->theGroundMotionID;
        //NodeTags(count) = sp->getNodeTag();
        //DOFTags(count) = sp->getDOF_Number();
        //if ( theDomain->getNode(NodeTags(count)) == 0 ) {
        //  this->removeSP_Constraint(myTag(count));
        //  delete sp;
        //}
        //count++;
    }

    //res = theChannel.sendID(dbTag, commitTag, NodeTags);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::sendSelf() - channel failed to send NodeTags\n";
    //  return res;
    //}
    //
    //res = theChannel.sendID(dbTag, commitTag, DOFTags);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::sendSelf() - channel failed to send DOFTags\n";
    //  return res;
    //}

    return 0;
}

int
MultiSupportPattern::receiveSelf(int commitTag, Channel& theChannel,
                              FEM_ObjectBroker& theBroker)
{
    //Guanzhou added
    int dbTag = this->getDbTag();

    static ID data(3);
    int res = theChannel.receiveID(dbTag, commitTag, data);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv data\n";
        return res;
    }

    this->setTag(data(0));
    const int new_numMotions = data(1);
    const int num_sp = data(2);

    static ID motionClassTag(new_numMotions);
    static ID motionDbTag(new_numMotions);

    res = theChannel.receiveID(dbTag, commitTag, motionClassTag);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv motionClassTag\n";
        return res;
    }

    res = theChannel.receiveID(dbTag, commitTag, motionDbTag);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv motionDbTag\n";
        return res;
    }

    if ( theMotions != 0 )
    {
        for (int i = 0; i < numMotions; i++)
        {
            if ( theMotions[i] != 0 )
            {
                delete theMotions[i];
            }
        }

        delete [] theMotions;
    }

    numMotions = new_numMotions;

    theMotions = new GroundMotion *[numMotions];

    for (int i = 0; i < numMotions; i++)
    {
        theMotions[i] = theBroker.getNewGroundMotion(motionClassTag(i));

        if (theMotions[i] == 0)
        {
            cerr << "MultiSupportPattern::receiveSelf() - could not create a grond motion\n";
            return -3;
        }

        theMotions[i]->setDbTag(motionDbTag(i));
        res = theMotions[i]->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "MultiSupportPattern::receiveSelf() - motion could not receive itself \n";
            return res;
        }
    }

    //static ID myTag(num_sp);
    static ID MotionsIDs(num_sp);
    //static ID NodeTags(num_sp);
    //static ID DOFTags(num_sp);

    theMotionTags.Zero();
    theMotionTags.resize(numMotions);

    res = theChannel.receiveID(dbTag, commitTag, theMotionTags);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv theMotionTags\n";
        return res;
    }

    //res = theChannel.receiveID(dbTag, commitTag, myTag);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv myTag\n";
    //  return res;
    //}

    res = theChannel.receiveID(dbTag, commitTag, MotionsIDs);

    if (res < 0)
    {
        cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv MotionsIDs\n";
        return res;
    }

    //res = theChannel.receiveID(dbTag, commitTag, NodeTags);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv NodeTags\n";
    //  return res;
    //}
    //
    //res = theChannel.receiveID(dbTag, commitTag, DOFTags);
    //if (res < 0) {
    //  cerr << "MultiSupportPattern::receiveSelf() - channel failed to recv DOFTags\n";
    //  return res;
    //}

    //theMotionTags =  MotionsIDs;

    //SP_Constraint *theSP;
    ImposedMotionSP* theSP;

    //Domain *theDomain = this->getDomain();
    //
    for (int i = 0; i < num_sp; i++)
    {
        //if ( theDomain->getNode(NodeTags(i)) == 0 ) continue;
        theSP = (ImposedMotionSP*)theBroker.getNewSP(CNSTRNT_TAG_ImposedMotionSP);

        if (theSP == 0)
        {
            cerr << "MultiSupportPattern::receiveSelf() - could not create a imposed sp\n";
            return -3;
        }

        res = theSP->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "MultiSupportPattern::receiveSelf() - imposed sp could not receive itself \n";
            return res;
        }

        const int motionID = MotionsIDs(i);
        theSP->setGroundMotion(theMotions[motionID]);

        //theSP = new ImposedMotionSP(myTag(i), NodeTags(i), DOFTags(i),
        //  *theMotions[MotionsIDs(i)], MotionsIDs(i), false);
        if (this->addSP_Constraint((SP_Constraint*)theSP) == false)
        {
            cerr << "MultiSupportPattern::receiveSelf() - fail to add sp to pattern \n";
            return -1;
        }
    }

    return 0;
}

void
MultiSupportPattern::Print(ostream& s, int flag)
{
    s << "MultiSupportPattern  tag: " << this->getTag() << endln;
    SP_Constraint* sp;
    SP_ConstraintIter& theIter = this->getSPs();
    s << "Number of GM " << numMotions << " GMs: " << theMotionTags << endln;

    while ((sp = theIter()) != 0)
    {
        sp->Print(s, flag);
    }
}

LoadPattern*
MultiSupportPattern::getCopy(void)
{
    //replaced by Babak on Feb 4 2013:

    /*
    //--------------------
    LoadPattern *theCopy = new MultiSupportPattern(this->getTag());
    //if (theCopy == 0) {
    //  cerr << "MultiSupportPattern::getCopy() - ran out of memory\n";
      return theCopy; // in case fatal() does not exit
    //}
    //GroundMotion *gMotion;
    //for (int i=0; i<numMotions; i++) {
    //    gMotion = new
    //    theCopy->addMotion()
    //}
    */
    //----------

    //----------

    MultiSupportPattern* theCopy = new MultiSupportPattern(this->getTag());

    if (theCopy == 0)
    {
        cerr << "MultiSupportPattern::getCopy() - ran out of memory\n";
    }



    //theCopy->numMotions = this->numMotions;
    //theCopy->theMotionTags = this->theMotionTags;
    for (int i = 0; i < numMotions; i++)
    {
        theCopy->addMotion(*(this->getMotion(i + 1)), i + 1);
    }

    cerr << theCopy->theMotionTags << "\n";
    cerr << theCopy->numMotions << "\n";


    return theCopy; // in case fatal() does not exit

    //----------
}
