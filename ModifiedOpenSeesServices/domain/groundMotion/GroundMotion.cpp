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

// $Revision: 1.9 $
// $Date: 2004/07/07 00:51:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/groundMotion/GroundMotion.cpp,v $

// Written: fmk
// Created: 05/98
// Revision: A


#include <GroundMotion.h>
#include <TimeSeriesIntegrator.h>
#include <TrapezoidalTimeSeriesIntegrator.h>
#include <OPS_Globals.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <ID.h>

#include <iostream>
using namespace std;

GroundMotion::GroundMotion(TimeSeries* dispSeries,
                           TimeSeries* velSeries,
                           TimeSeries* accelSeries,
                           TimeSeriesIntegrator* theIntegratr,
                           double dTintegration)
    : MovableObject(GROUND_MOTION_TAG_GroundMotion),
      theAccelSeries(accelSeries), theVelSeries(velSeries),
      theDispSeries(dispSeries), theIntegrator(theIntegratr),
      data(3), delta(dTintegration)
{

}



GroundMotion::GroundMotion(int theClassTag)
    : MovableObject(theClassTag),
      theAccelSeries(0), theVelSeries(0), theDispSeries(0), theIntegrator(0),
      data(3), delta(0.01)
{

}

GroundMotion::~GroundMotion()
{
    if (theAccelSeries != 0)
    {
        delete theAccelSeries;
    }

    if (theVelSeries != 0)
    {
        delete theVelSeries;
    }

    if (theDispSeries != 0)
    {
        delete theDispSeries;
    }

    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }
}


void
GroundMotion::setIntegrator(TimeSeriesIntegrator* integrator)
{
    if (theIntegrator != 0)
    {
        delete theIntegrator;
    }

    theIntegrator = integrator;
}

TimeSeries*
GroundMotion::integrate(TimeSeries* theSeries, double delta)
{
    // check that an integrator & accel series exist
    if(theIntegrator == 0)
    {
        cerr << "WARNING:GroundMotion::integrate() - no TimeSeriesIntegrator provided - will use Trapezoidal. \n";
        theIntegrator = new TrapezoidalTimeSeriesIntegrator();

        if(theIntegrator == 0)
        {
            cerr << "WARNING:GroundMotion::integrate() - no TimeSeriesIntegrator provided - failed to create a Trapezoidal .. memory problems! \n";
            return 0;
        }
    }

    if(theSeries == 0)
    {
        cerr << "GroundMotion::integrate - no TimeSeries specified\n";
        return 0;
    }

    // integrate the series, if no vel series exists set it to new one
    TimeSeries* theNewSeries = theIntegrator->integrate(theSeries, delta);

    if(theNewSeries == 0)
    {
        cerr << "GroundMotion::integrate - no TimeSeriesIntegrator failed to integrate\n";
        return 0;
    }

    return theNewSeries;
}

double
GroundMotion::getDuration(void)
{
    if (theAccelSeries != 0)
    {
        return theAccelSeries->getDuration();
    }
    else
    {
        return 0.0;
    }
}

double
GroundMotion::getPeakAccel(void)
{
    if (theAccelSeries != 0)
    {
        return theAccelSeries->getPeakFactor();
    }
    else
    {
        return 0.0;
    }

}

double
GroundMotion::getPeakVel(void)
{
    if (theVelSeries != 0)
    {
        return theVelSeries->getPeakFactor();
    }

    // if theAccel is not 0, integrate accel series to get a vel series
    else if (theAccelSeries != 0)
    {
        theVelSeries = this->integrate(theAccelSeries, delta);

        if (theVelSeries != 0)
        {
            return theVelSeries->getPeakFactor();
        }
        else
        {
            return 0.0;
        }
    }

    return 0.0;
}

double
GroundMotion::getPeakDisp(void)
{
    if (theDispSeries != 0)
    {
        return theDispSeries->getPeakFactor();
    }

    // if theVel is not 0, integrate vel series to get disp series
    else if (theVelSeries != 0)
    {
        theDispSeries = this->integrate(theVelSeries, delta);

        if (theDispSeries != 0)
        {
            return theDispSeries->getPeakFactor();
        }
        else
        {
            return 0.0;
        }
    }

    // if theAccel is not 0, integrate vel series to get disp series
    else if (theAccelSeries != 0)
    {
        theVelSeries = this->integrate(theAccelSeries, delta);

        if (theVelSeries != 0)
        {
            theDispSeries = this->integrate(theVelSeries, delta);

            if (theDispSeries != 0)
            {
                return theDispSeries->getPeakFactor();
            }
            else
            {
                return 0.0;
            }
        }
        else
        {
            return 0.0;
        }
    }

    return 0.0;
}

double
GroundMotion::getAccel(double time)
{
    if (time < 0.0)
    {
        return 0.0;
    }

    if (theAccelSeries != 0)
    {
        return theAccelSeries->getFactor(time);
    }
    else
    {
        return 0.0;
    }
}



double
GroundMotion::getVel(double time)
{
    if (time < 0.0)
    {
        return 0.0;
    }

    if (theVelSeries != 0)
    {
        return theVelSeries->getFactor(time);
    }

    // if theAccel is not 0, integrate accel series to get a vel series
    else if (theAccelSeries != 0)
    {
        cerr << " WARNING: GroundMotion::getVel(double time) - integration is required to get the ground velocities from the ground accelerations\n";
        theVelSeries = this->integrate(theAccelSeries, delta);

        if (theVelSeries != 0)
        {
            return theVelSeries->getFactor(time);

        }
        else
        {
            cerr << " WARNING: GroundMotion::getVel(double time) - failed to integrate\n";
            return 0.0;
        }
    }

    return 0.0;
}

double
GroundMotion::getDisp(double time)
{
    if (time < 0.0)
    {
        return 0.0;
    }

    if (theDispSeries != 0)
    {
        return theDispSeries->getFactor(time);
    }

    // if theVel is not 0, integrate vel series to get disp series
    else if (theVelSeries != 0)
    {
        cerr << " WARNING: GroundMotion::getDisp(double time) - integration is required to get the ground displacements from the ground velocities\n";
        theDispSeries = this->integrate(theVelSeries, delta);

        if (theDispSeries != 0)
        {
            return theDispSeries->getFactor(time);
        }
        else
        {
            return 0.0;
        }
    }

    // if theAccel is not 0, integrate vel series to get disp series
    else if (theAccelSeries != 0)
    {
        cerr << " WARNING: GroundMotion::getDisp(double time) - default integration required to get the ground displacements from the ground velocities via the ground accelerations\n";
        theVelSeries = this->integrate(theAccelSeries, delta);

        if (theVelSeries != 0)
        {
            theDispSeries = this->integrate(theVelSeries, delta);

            if (theDispSeries != 0)
            {
                return theDispSeries->getFactor(time);
            }
            else
            {
                return 0.0;
            }
        }
        else
        {
            return 0.0;
        }
    }

    return 0.0;
}

const Vector&
GroundMotion::getDispVelAccel(double time)
{
    if (time < 0.0)
    {
        data(0) = 0.0;
        data(1) = 0.0;
        data(2) = 0.0;
        return data;
    }

    if (theAccelSeries != 0 && theVelSeries != 0 && theDispSeries != 0)
    {
        data(0) = theDispSeries->getFactor(time);
        data(1) = theVelSeries->getFactor(time);
        data(2) = theAccelSeries->getFactor(time);
    }
    else
    {
        data(2) = this->getAccel(time);
        data(1) = this->getVel(time);
        data(0) = this->getDisp(time);
    }

    return data;
}


int
GroundMotion::sendSelf(int commitTag, Channel& theChannel)
{
    int dbTag = this->getDbTag();

    static ID idData(8);

    if (theAccelSeries != 0)
    {
        idData(0) = theAccelSeries->getClassTag();
        int seriesDbTag = theAccelSeries->getDbTag();

        if (seriesDbTag == 0)
        {
            seriesDbTag = theChannel.getDbTag();
            theAccelSeries->setDbTag(seriesDbTag);
        }

        idData(1) = seriesDbTag;
    }
    else
    {
        idData(0) = -1;
    }

    if (theVelSeries != 0)
    {
        idData(2) = theVelSeries->getClassTag();
        int seriesDbTag = theVelSeries->getDbTag();

        if (seriesDbTag == 0)
        {
            seriesDbTag = theChannel.getDbTag();
            theVelSeries->setDbTag(seriesDbTag);
        }

        idData(3) = seriesDbTag;
    }
    else
    {
        idData(2) = -1;
    }

    if (theDispSeries != 0)
    {
        idData(4) = theDispSeries->getClassTag();
        int seriesDbTag = theDispSeries->getDbTag();

        if (seriesDbTag == 0)
        {
            seriesDbTag = theChannel.getDbTag();
            theDispSeries->setDbTag(seriesDbTag);
        }

        idData(5) = seriesDbTag;
    }
    else
    {
        idData(4) = -1;
    }

    if (theIntegrator != 0)
    {
        idData(6) = theIntegrator->getClassTag();
        int seriesDbTag = theIntegrator->getDbTag();

        if (seriesDbTag == 0)
        {
            seriesDbTag = theChannel.getDbTag();
            theIntegrator->setDbTag(seriesDbTag);
        }

        idData(7) = seriesDbTag;
    }
    else
    {
        idData(6) = -1;
    }

    int res = theChannel.sendID(dbTag, commitTag, idData);

    if (res < 0)
    {
        cerr << "GroundMotionRecord::sendSelf() - channel failed to send data\n";
        return res;
    }

    // now send the series
    if (theAccelSeries != 0)
    {
        res = theAccelSeries->sendSelf(commitTag, theChannel);

        if (res < 0)
        {
            cerr << "GroundMotionRecord::sendSelf - failed to send accel series\n";
            return res;
        }
    }

    if (theVelSeries != 0)
    {
        res = theVelSeries->sendSelf(commitTag, theChannel);

        if (res < 0)
        {
            cerr << "GroundMotionRecord::sendSelf - failed to send velocity series\n";
            return res;
        }
    }

    if (theDispSeries != 0)
    {
        res = theDispSeries->sendSelf(commitTag, theChannel);

        if (res < 0)
        {
            cerr << "GroundMotionRecord::sendSelf - failed to send disp series\n";
            return res;
        }
    }

    if (theIntegrator != 0)
    {
        res = theIntegrator->sendSelf(commitTag, theChannel);

        if (res < 0)
        {
            cerr << "GroundMotionRecord::sendSelf - failed to send disp series\n";
            return res;
        }
    }

    return 0;
}


int
GroundMotion::receiveSelf(int commitTag, Channel& theChannel,
                       FEM_ObjectBroker& theBroker)
{
    int dbTag = this->getDbTag();

    static ID idData(8);
    int res = theChannel.receiveID(dbTag, commitTag, idData);

    if (res < 0)
    {
        cerr << "UniformExcitation::sendSelf() - channel failed to send data\n";
        return res;
    }

    int seriesClassTag = idData(0);

    if (seriesClassTag != -1)
    {
        int seriesDbTag = idData(1);

        if (theAccelSeries == 0 || theAccelSeries->getClassTag() != seriesClassTag)
        {
            if (theAccelSeries != 0)
            {
                delete theAccelSeries;
            }

            theAccelSeries = theBroker.getNewTimeSeries(seriesClassTag);

            if (theAccelSeries == 0)
            {
                cerr << "GroundMotionRecord::sendSelf - could not create a Series object\n";
                return -2;
            }
        }

        theAccelSeries->setDbTag(seriesDbTag);
        res = theAccelSeries->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "UniformExcitation::sendSelf() - accel series failed to send data\n";
            return res;
        }
    }

    seriesClassTag = idData(2);

    if (seriesClassTag != -1)
    {
        int seriesDbTag = idData(3);

        if (theVelSeries == 0 || theVelSeries->getClassTag() != seriesClassTag)
        {
            if (theVelSeries != 0)
            {
                delete theVelSeries;
            }

            theVelSeries = theBroker.getNewTimeSeries(seriesClassTag);

            if (theVelSeries == 0)
            {
                cerr << "GroundMotionRecord::sendSelf - could not create a Series object\n";
                return -2;
            }
        }

        theVelSeries->setDbTag(seriesDbTag);
        res = theVelSeries->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "UniformExcitation::sendSelf() - accel series failed to send data\n";
            return res;
        }
    }

    seriesClassTag = idData(4);

    if (seriesClassTag != -1)
    {
        int seriesDbTag = idData(5);

        if (theDispSeries == 0 || theDispSeries->getClassTag() != seriesClassTag)
        {
            if (theDispSeries != 0)
            {
                delete theDispSeries;
            }

            theDispSeries = theBroker.getNewTimeSeries(seriesClassTag);

            if (theDispSeries == 0)
            {
                cerr << "GroundMotionRecord::sendSelf - could not create a Series object\n";
                return -2;
            }
        }

        theDispSeries->setDbTag(seriesDbTag);
        res = theDispSeries->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "UniformExcitation::sendSelf() - accel series failed to send data\n";
            return res;
        }
    }

    seriesClassTag = idData(6);

    if (seriesClassTag != -1)
    {
        int seriesDbTag = idData(7);

        if (theIntegrator == 0 || theIntegrator->getClassTag() != seriesClassTag)
        {
            if (theIntegrator != 0)
            {
                delete theIntegrator;
            }

            theIntegrator = theBroker.getNewTimeSeriesIntegrator(seriesClassTag);

            if (theIntegrator == 0)
            {
                cerr << "GroundMotionRecord::sendSelf - could not create an Integrator object\n";
                return -2;
            }
        }

        theIntegrator->setDbTag(seriesDbTag);
        res = theIntegrator->receiveSelf(commitTag, theChannel, theBroker);

        if (res < 0)
        {
            cerr << "UniformExcitation::sendSelf() - accel series failed to send data\n";
            return res;
        }
    }

    return 0;
}




