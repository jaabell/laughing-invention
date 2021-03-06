///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli , Boris Jeremic
// DATE:               May 2012
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



#include <PathTimeSeries_Force_TimeHistory.h>
#include <math.h>
#include <iostream>
using namespace std;


#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory()
    : LoadPattern(0, PATTERN_TAG_PathTimeSeries_Force_TimeHistory),
      thePath(0),
      time(0),
      currentTimeLoc(0),
      cFactor(0.0),
      pathTimeIncr(0),
      nodeTag(0),
      dof(0),
      loadValue(0),
      isConstant(0)
{
    // does nothing
}


PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory(int tag,
        int NodeNumber,
        int dof_to_be_shaken,
        double theFactor,
        const char *fileName)
    : LoadPattern(tag, PATTERN_TAG_PathTimeSeries_Force_TimeHistory),
      thePath(0),
      time(0),
      currentTimeLoc(0),
      cFactor(theFactor),
      pathTimeIncr(0),
      nodeTag(NodeNumber),
      dof(dof_to_be_shaken),
      loadValue(0),
      isConstant(0)
{


    // determine the number of data points
    int numDataPoints = 0;
    double dataPoint;
    ifstream theFile;

    // first open and go through file counting entries
    theFile.open(fileName, ios::in);

    if (!theFile)
    {
        cerr << "WARNING - PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory()";
        cerr << " - could not open file " << fileName << endln;
    }
    else
    {
        while (theFile >> dataPoint)
        {
            numDataPoints++;
            theFile >> dataPoint;   // Read in second value of pair
        }
    }

    theFile.close();

    // create a vector and read in the data
    if (numDataPoints != 0)
    {

        // now create the two vector
        thePath = new Vector(numDataPoints);
        time = new Vector(numDataPoints);

        // ensure did not run out of memory creating copies
        if (thePath == 0 || thePath->Size() == 0 || time == 0 || time->Size() == 0)
        {

            cerr << "WARNING PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory() - out of memory\n ";

            if (thePath != 0)
            {
                delete thePath;
            }

            if (time != 0)
            {
                delete time;
            }

            thePath = 0;
            time = 0;
        }

        // first open the file and read in the data
        ifstream theFile1;
        theFile1.open(fileName, ios::in);

        if (!theFile1)
        {
            cerr << "WARNING - PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory()";
            cerr << " - could not open file " << fileName << endln;
            delete thePath;
            delete time;
            thePath = 0;
            time = 0;
        }
        else   // read in the time and then read the value
        {
            int count = 0;

            while (theFile1 >> dataPoint)
            {
                (*time)(count) = dataPoint;
                theFile1 >> dataPoint;
                (*thePath)(count) = dataPoint;
                count++;
            }

            // finally close the file
            theFile1.close();
        }

    }


}




PathTimeSeries_Force_TimeHistory::PathTimeSeries_Force_TimeHistory(int tag,
        int NodeNumber,
        int dof_to_be_shaken,
        double theTimeIncr,
        double theFactor,
        int currentTimeLoc_,
        int isConstant_,
        double loadValue_,
        Vector *thePath_,
        Vector *time_)
    : LoadPattern(tag, PATTERN_TAG_PathTimeSeries_Force_TimeHistory),
      thePath(0),
      time(0),
      currentTimeLoc(currentTimeLoc_),
      cFactor(theFactor),
      pathTimeIncr(theTimeIncr),
      nodeTag(NodeNumber),
      dof(dof_to_be_shaken),
      loadValue(loadValue_),
      isConstant(isConstant_)
{
    thePath = new Vector(*thePath_);
    time = new Vector(*time_);

}






PathTimeSeries_Force_TimeHistory::~PathTimeSeries_Force_TimeHistory()
{
    // does nothing
}


void
PathTimeSeries_Force_TimeHistory::setDomain(Domain *theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}



void
PathTimeSeries_Force_TimeHistory::setLoadConstant(void)
{
    isConstant = 1;
}


double
PathTimeSeries_Force_TimeHistory::getFactor(double pseudoTime)
{

    // check for a quick return
    if (pseudoTime < 0.0 || thePath == 0)
    {
        return 0.0;
    }




    // determine indexes into the data array whose boundary holds the time
    double time1 = (*time)(currentTimeLoc);

    // check for another quick return
    if (pseudoTime == time1)
    {
        return cFactor * (*thePath)[currentTimeLoc];
    }

    int size = time->Size();
    int sizem1 = size - 1;
    int sizem2 = size - 2;

    // check we are not at the end
    if (pseudoTime > time1 && currentTimeLoc == sizem1)
    {
        return 0.0;
    }

    if (pseudoTime < time1 && currentTimeLoc == 0)
    {
        return 0.0;
    }

    // otherwise go find the current interval
    double time2 = (*time)(currentTimeLoc + 1);

    if (pseudoTime > time2)
    {
        while ((pseudoTime > time2) && (currentTimeLoc < sizem2))
        {
            currentTimeLoc++;
            time1 = time2;
            time2 = (*time)(currentTimeLoc + 1);
        }

        // if pseudo time greater than ending time reurn 0
        if (pseudoTime > time2)
        {
            return 0.0;
        }

    }
    else if (pseudoTime < time1)
    {
        while ((pseudoTime < time1) && (currentTimeLoc > 0))
        {
            currentTimeLoc--;
            time2 = time1;
            time1 = (*time)(currentTimeLoc);
        }

        // if starting time less than initial starting time return 0
        if (pseudoTime < time1)
        {
            return 0.0;
        }
    }


    if (isConstant == 0)
    {
        double value1 = (*thePath)[currentTimeLoc];
        double value2 = (*thePath)[currentTimeLoc + 1];
        loadValue = cFactor * (value1 + (value2 - value1) * (pseudoTime - time1) / (time2 - time1));
    }


    return loadValue;


}


void
PathTimeSeries_Force_TimeHistory::applyLoad(double time)
{

    Domain *theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }


    Node *theNode;

    theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory - no nodes associtated to the nodeTag " << nodeTag << "\n";
        // exit(1);
    }
    else
    {
        //Create the nodal load vector accoding to the DOFs the node has
        int numDOF = theNode->getNumberDOF();
        Vector *nodalLoad = new Vector(numDOF);



        for (int i = 0; i < numDOF; i++)
        {
            if ( i == (dof - 1) )
            {
                (*nodalLoad)(i) = getFactor(time);
            }
            else
            {
                (*nodalLoad)(i) = 0;
            }
        }

        theNode->addUnbalancedLoad(*nodalLoad);
        delete nodalLoad;
    }
}



int
PathTimeSeries_Force_TimeHistory::sendSelf(int commitTag, Channel &theChannel)
{
    static ID IDdata(6);
    IDdata(0) = currentTimeLoc;
    IDdata(1) = nodeTag;
    IDdata(2) = dof;
    IDdata(3) = isConstant;
    IDdata(4) = thePath->Size();
    IDdata(5) = this->getTag();

    int res = theChannel.sendID(0, commitTag, IDdata);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::sendSelf() - channel failed to send IDdata\n";
        return res;
    }

    static Vector Vectordata(3);
    Vectordata(0) = cFactor;
    Vectordata(1) = pathTimeIncr;
    Vectordata(2) = loadValue;
    res = theChannel.sendVector(0, commitTag, Vectordata);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::sendSelf() - channel failed to send Vectordata\n";
        return res;
    }

    res = theChannel.sendVector(0, commitTag, *thePath);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::sendSelf() - channel failed to send thePath vector\n";
        return res;
    }


    res = theChannel.sendVector(0, commitTag, *time);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::sendSelf() - channel failed to send time vector\n";
        return res;
    }




    return 0;
}


int
PathTimeSeries_Force_TimeHistory::receiveSelf(int commitTag, Channel &theChannel,
        FEM_ObjectBroker &theBroker)
{
    static ID IDdata(6);
    int res = theChannel.receiveID(0, commitTag, IDdata);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::receiveSelf() - channel failed to receive IDdata\n";
        return res;
    }

    currentTimeLoc = IDdata(0);
    nodeTag        = IDdata(1);
    dof            = IDdata(2);
    isConstant     = IDdata(3);
    int size       = IDdata(4);
    int tag        = IDdata(5);
    this->setTag(tag);

    thePath = new Vector(size);
    time = new Vector(size);

    static Vector Vectordata(3);
    res = theChannel.receiveVector(0, commitTag, Vectordata);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::receiveSelf() - channel failed to receive Vectordata\n";
        return res;
    }

    cFactor      = Vectordata(0);
    pathTimeIncr = Vectordata(1);
    loadValue    = Vectordata(2);


    res = theChannel.receiveVector(0, commitTag, *thePath);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::receiveSelf() - channel failed to receive thePath vector\n";
        return res;
    }


    res = theChannel.receiveVector(0, commitTag, *time);
    if (res < 0)
    {
        cerr << "PathTimeSeries_Force_TimeHistory::receiveSelf() - channel failed to receive time vector\n";
        return res;
    }


    return 0;
}


void
PathTimeSeries_Force_TimeHistory::Print(ostream &s, int flag)
{
    s << "PathTimeSeries_Force_TimeHistory " << this->getTag() << endln;

}



// method to obtain a blank copy of the LoadPattern
LoadPattern *
PathTimeSeries_Force_TimeHistory::getCopy(void)
{

    // cerr << "PathTimeSeries_Force_TimeHistory::getCopy() - not yet implemented\n";
    LoadPattern *theCopy = new PathTimeSeries_Force_TimeHistory(this->getTag(),
            nodeTag,
            dof,
            pathTimeIncr,
            cFactor,
            currentTimeLoc,
            isConstant,
            loadValue,
            thePath,
            time);

    return theCopy;
}


