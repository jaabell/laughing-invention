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



#include <PathSeries_Disp_TimeHistory_Penalty.h>
#include <math.h>

#include <iomanip>
using std::ios;

#include <fstream>
using std::ifstream;

#include <iostream>
using namespace std;


PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty()
    : LoadPattern(0, PATTERN_TAG_PathSeries_Disp_TimeHistory_Penalty),
      thePath(0), time(0), currentTimeLoc(0), pathTimeIncr(0),
      loadvalue_previous(0),
      cFactor(0.0), isConstant(0)
{
    // does nothing
}





PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty(int tag,
        double PenaltyStiffness,
        int NodeNumber,
        int dof_to_be_shaken,
        double theFactor,
        const char* fileName)
    : LoadPattern(tag, PATTERN_TAG_PathSeries_Disp_TimeHistory_Penalty),
      nodeTag(NodeNumber),
      dof(dof_to_be_shaken),
      penalty_stiffness(PenaltyStiffness),
      thePath(0), time(0),
      currentTimeLoc(0),
      pathTimeIncr(0),
      loadvalue_previous(0),
      cFactor(theFactor), isConstant(0)
{


    // determine the number of data points
    int numDataPoints = 0;
    double dataPoint;
    ifstream theFile;

    // first open and go through file counting entries
    theFile.open(fileName, ios::in);

    if (!theFile)
    {
        cerr << "WARNING - PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty()";
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

            cerr << "WARNING PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty() - out of memory\n ";

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
            cerr << "WARNING - PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty()";
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




PathSeries_Disp_TimeHistory_Penalty::PathSeries_Disp_TimeHistory_Penalty(int tag,
        double PenaltyStiffness,
        int NodeNumber,
        int dof_to_be_shaken,
        double theTimeIncr,
        double theFactor,
        const char* fileName)
    : LoadPattern(tag, PATTERN_TAG_PathSeries_Disp_TimeHistory_Penalty),
      nodeTag(NodeNumber),
      penalty_stiffness(PenaltyStiffness),
      dof(dof_to_be_shaken),
      thePath(0), time(0),
      currentTimeLoc(0), pathTimeIncr(theTimeIncr),
      loadvalue_previous(0),
      cFactor(theFactor)
{


    // determine the number of data points .. open file and count num entries
    int numDataPoints = 0;
    double dataPoint;

    ifstream theFile;
    theFile.open(fileName);


    if (!theFile)
    {
        cerr << "WARNING - PathSeries::PathSeries()";
        cerr << " - could not open file " << fileName << endln;
    }
    else
    {
        while (theFile >> dataPoint)
        {
            numDataPoints++;
        }
    }

    theFile.close();


    // create a vector and read in the data
    if (numDataPoints != 0)
    {

        // first open the file
        ifstream theFile1;
        theFile1.open(fileName, ios::in);

        if (!theFile1)
        {
            cerr << "WARNING - PathSeries::PathSeries()";
            cerr << " - could not open file " << fileName << endln;
        }
        else
        {

            // now create the vector
            thePath = new Vector(numDataPoints);

            // ensure we did not run out of memory
            if (thePath == 0 || thePath->Size() == 0)
            {
                cerr << "PathSeries::PathSeries() - ran out of memory constructing";
                cerr << " a Vector of size: " << numDataPoints << endln;

                if (thePath != 0)
                {
                    delete thePath;
                }

                thePath = 0;
            }

            // read the data from the file
            else
            {
                int count = 0;

                while (theFile1 >> dataPoint)
                {
                    (*thePath)(count) = dataPoint;
                    count++;
                }
            }

            // finally close the file
            theFile1.close();
        }
    }


}







PathSeries_Disp_TimeHistory_Penalty::~PathSeries_Disp_TimeHistory_Penalty()
{
    // does nothing
}


void
PathSeries_Disp_TimeHistory_Penalty::setDomain(Domain* theDomain)
{

    this->LoadPattern::setDomain(theDomain);

}


void
PathSeries_Disp_TimeHistory_Penalty::setLoadConstant(void)
{
    isConstant = 1;
}

double
PathSeries_Disp_TimeHistory_Penalty::getFactor(double pseudoTime)
{

    // check for a quick return
    if (pseudoTime < 0.0 || thePath == 0)
    {
        return 0.0;
    }

    if (  isConstant == 0)
    {

        if ( pathTimeIncr == 0 )
        {

            // determine indexes into the data array whose boundary holds the time
            double time1 = (*time)(currentTimeLoc);

            // check for another quick return
            if (pseudoTime == time1)
            {
                double loadvalue_current = cFactor * (*thePath)[currentTimeLoc];
                double difference_value = loadvalue_current - loadvalue_previous;
                loadvalue_previous = loadvalue_current;
                return penalty_stiffness * difference_value;
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
                while ((pseudoTime > time2) && (currentTimeLoc < sizem1))
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

            double value1 = (*thePath)[currentTimeLoc];
            double value2 = (*thePath)[currentTimeLoc + 1];
            double loadvalue_current = cFactor * (value1 + (value2 - value1) * (pseudoTime - time1) / (time2 - time1));
            double difference_value = loadvalue_current - loadvalue_previous;
            loadvalue_previous = loadvalue_current;
            return penalty_stiffness * difference_value;

        }

        else

        {

            // determine indexes into the data array whose boundary holds the time
            double incr = pseudoTime / pathTimeIncr;


            int incr1 = round(incr);
            int incr2 = incr1 + 1;

            if (incr2 > thePath->Size() + 1)
            {
                return 0.0;
            }
            else
            {
                double value1 = (*thePath)[incr1];
                double value2 = (*thePath)[incr2];
                double loadvalue_current = cFactor * (value1 + (value2 - value1) * (pseudoTime / pathTimeIncr - incr1));
                double difference_value = loadvalue_current - loadvalue_previous;
                loadvalue_previous = loadvalue_current;
                return penalty_stiffness * difference_value;
            }


        }

    }
    else
    {
        return 0;
    }


}






// double
// PathSeries_Disp_TimeHistory_Penalty::getFactor(double pseudoTime)
// {
//
//   // check for a quick return
//   if (pseudoTime < 0.0 || thePath == 0)
//     return 0.0;
//
//   double loadValue = 0;
//
// if ( pathTimeIncr == 0 )
// {
//
//   // determine indexes into the data array whose boundary holds the time
//   double time1 = (*time)(currentTimeLoc);
//
//   // check for another quick return
//   if (pseudoTime == time1)
//   {
//     double loadvalue_current = cFactor * (*thePath)[currentTimeLoc];
//     double difference_value = loadvalue_current - loadvalue_previous;
//     loadvalue_previous = loadvalue_current;
//     return penalty_stiffness * difference_value;
//   }
//
//   int size = time->Size();
//   int sizem1 = size - 1;
//   int sizem2 = size - 2;
//
//   // check we are not at the end
//   if (pseudoTime > time1 && currentTimeLoc == sizem1)
//     return 0.0;
//
//   if (pseudoTime < time1 && currentTimeLoc == 0)
//     return 0.0;
//
//   // otherwise go find the current interval
//   double time2 = (*time)(currentTimeLoc+1);
//   if (pseudoTime > time2)
//   {
//     while ((pseudoTime > time2) && (currentTimeLoc < sizem2))
//     {
//       currentTimeLoc++;
//       time1 = time2;
//       time2 = (*time)(currentTimeLoc+1);
//     }
//     // if pseudo time greater than ending time reurn 0
//     if (pseudoTime > time2)
//       return 0.0;
//
//   } else if (pseudoTime < time1) {
//     while ((pseudoTime < time1) && (currentTimeLoc > 0))
//     {
//       currentTimeLoc--;
//       time2 = time1;
//       time1 = (*time)(currentTimeLoc);
//     }
//     // if starting time less than initial starting time return 0
//     if (pseudoTime < time1)
//       return 0.0;
//   }
//
//   double value1 = (*thePath)[currentTimeLoc];
//   double value2 = (*thePath)[currentTimeLoc+1];
//   double loadvalue_current = cFactor * (value1 + (value2-value1)*(pseudoTime-time1)/(time2 - time1));
//   double difference_value = loadvalue_current - loadvalue_previous;
//   loadvalue_previous = loadvalue_current;
//   return penalty_stiffness * difference_value;
//
// }
//
// else
//
// {
//
//   // determine indexes into the data array whose boundary holds the time
//   double incr = pseudoTime/pathTimeIncr;
//
//
// //   int incr1 = floor(incr);
//
//   int incr1 = round(incr);
//
//   int incr2 = incr1+1;
//
//   if (incr2 > thePath->Size()+1)
//     return 0.0;
//   else {
//     double value1 = (*thePath)[incr1];
//     double value2 = (*thePath)[incr2];
//     double loadvalue_current = cFactor * (value1 + (value2-value1)*(pseudoTime/pathTimeIncr - incr1));
//     double difference_value = loadvalue_current - loadvalue_previous;
//     loadvalue_previous = loadvalue_current;
//     loadValue = penalty_stiffness * difference_value;
//   }
//
//
// }
//
//
// return loadValue;
//
// }






void
PathSeries_Disp_TimeHistory_Penalty::applyLoad(double time)
{

    Domain* theDomain = this->getDomain();

    if (theDomain == 0)
    {
        return;
    }


    Node* theNode;

    theNode = theDomain->getNode(nodeTag);

    if (theNode == 0)
    {
        //Out by Babak and replaced by following ... 6/26/13:
        //-----
        //        cerr << "PathSeries_Disp_TimeHistory_Penalty - no nodes associtated to the nodeTag " << nodeTag << "\n";
        //        exit(1);
        //-----

        //Added by Babak 6/26/13:
        //--------
# ifdef _PARALLEL_PROCESSING
        if ( !theDomain->hasInternalNode(nodeTag) && (theDomain->Domain::getNode (nodeTag ) == 0 ))
        {
            return;
        }

# else

        cerr << "PathSeries_Force_TimeHistory - no nodes associtated to the nodeTag " << nodeTag << "\n";
        exit(1);
#endif
        //--------
    }

    //Create the nodal load vector accoding to the DOFs the node has
    int numDOF = theNode->getNumberDOF();
    Vector* nodalLoad = new Vector(numDOF);



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

    theNode->addUnbalancedLoad(*nodalLoad, true);
    delete nodalLoad;

}



//Implemented by Babak Jume 24 2013
//---------
int
PathSeries_Disp_TimeHistory_Penalty::sendSelf(int commitTag, Channel& theChannel)
{
    static ID intData(5);
    intData(0) = this->getTag();
    intData(1) = nodeTag;
    intData(2) = dof;
    intData(3) = isConstant;
    intData(4) = time->Size(); //time and thePath vectors size ...

    int dataTag = this->getDbTag();

    //Sending idData:
    //---
    if (theChannel.sendID(dataTag, commitTag, intData) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::sendSelf() - channel failed to send intData\n";
        return -1;
    }

    //---


    static Vector doubleData(2);
    doubleData(0) = cFactor;
    doubleData(1) = penalty_stiffness;

    //Sending doubleData:
    //---
    if (theChannel.sendVector(dataTag, commitTag, doubleData) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::sendSelf() -- failed to send doubleData\n";
        return -1;
    }

    //---


    //Sending time vector:
    //---
    if (theChannel.sendVector(dataTag, commitTag, *time) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::sendSelf() -- failed to send time\n";
        return -1;
    }

    //---


    //Sending thePath vector:
    //---
    if (theChannel.sendVector(dataTag, commitTag, *thePath) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::sendSelf() -- failed to send thePath\n";
        return -1;
    }

    //---

    return 0;
}
//---------

//Implemented by Babak Jume 24 2013
//---------
int
PathSeries_Disp_TimeHistory_Penalty::receiveSelf(int commitTag, Channel& theChannel,
        FEM_ObjectBroker& theBroker)
{
    int dataTag = this->getDbTag();



    //Receiving idData ...
    //---
    static ID intData(5);

    if (theChannel.receiveID(dataTag, commitTag, intData) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::receiveSelf() -- failed to recv intData\n";
        return -1;
    }

    this->setTag(intData(0));
    nodeTag = intData(1);
    dof = intData(2);
    isConstant = intData(3);


    //---



    //Receiving doubleData ...
    //---
    static Vector doubleData(2);

    if (theChannel.receiveVector(dataTag, commitTag, doubleData) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::receiveSelf() -- failed to recv doubleData\n";
        return -1;
    }

    cFactor = doubleData(0);
    penalty_stiffness = doubleData(1);
    //---


    time = new Vector(intData(4));

    //Receiving time ...
    //---
    if (theChannel.receiveVector(dataTag, commitTag, *time) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::receiveSelf() -- failed to recv time\n";
        return -1;
    }

    //---

    thePath = new Vector(intData(4));

    //Receiving time ...
    //---
    if (theChannel.receiveVector(dataTag, commitTag, *thePath) < 0)
    {
        std::cerr << "PathSeries_Disp_TimeHistory_Penalty::receiveSelf() -- failed to recv thePath\n";
        return -1;
    }

    //---
    return 0;
}
//---------


void
PathSeries_Disp_TimeHistory_Penalty::Print(ostream& s, int flag)
{
    s << "PathSeries_Disp_TimeHistory_Penalty " << this->getTag() << endln;

}



// method to obtain a blank copy of the LoadPattern
//Implemented by Babak June 2013
LoadPattern*
PathSeries_Disp_TimeHistory_Penalty::getCopy(void)
{

    /*
        LoadPattern *theCopy = new PathSeries_Disp_TimeHistory_Penalty(this->getTag(),
                       penalty_stiffness,
                       nodeTag,
                       dof,
                       cFactor,
                       const char *fileName)

      cerr << "PathSeries_Disp_TimeHistory_Penalty::getCopy() - not yet implemented\n";*/
    return this;
}


