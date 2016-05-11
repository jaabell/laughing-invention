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
//
// PURPOSE:           Surface Load for Brick Elements
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              July 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


// Order of the nodes:
//
//         ________
//       2 |       | 1
//         |       |
//         |       |
//       3 |_______| 4
//
//
// Order of the nodes:
//
//         ________
//       2 |   5   | 1
//         |       |
//         |6     8|
//       3 |___7___| 4
//
//
// Order of the nodes:
//
//         ________
//       2 |   5   | 1
//         |       |
//         |6  9  8|
//       3 |_______| 4
//             7
//


#ifndef BrickSurfaceLoad_CPP
#define BrickSurfaceLoad_CPP


#include <BrickSurfaceLoad.h>
#include <Vector.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


Vector BrickSurfaceLoad::dataV8(8);
Vector BrickSurfaceLoad::dataV16(16);
Vector BrickSurfaceLoad::dataV18(18);


BrickSurfaceLoad::BrickSurfaceLoad(int tag,
                                   const ID& theElementTags,
                                   int Node_1, int Node_2, int Node_3, int Node_4,
                                   double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                                   double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      surface_load_magnitude1(SurfaceLoadMagnitude1),
      surface_load_magnitude2(SurfaceLoadMagnitude2),
      surface_load_magnitude3(SurfaceLoadMagnitude3),
      surface_load_magnitude4(SurfaceLoadMagnitude4),
      data(0)
{

    data = &dataV8; // Why would anyone do this??????

}



BrickSurfaceLoad::BrickSurfaceLoad(int tag,
                                   const ID& theElementTags,
                                   int Node_1, int Node_2, int Node_3, int Node_4,
                                   int Node_5, int Node_6, int Node_7, int Node_8,
                                   double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                                   double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4,
                                   double SurfaceLoadMagnitude5, double SurfaceLoadMagnitude6,
                                   double SurfaceLoadMagnitude7, double SurfaceLoadMagnitude8)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      node5(Node_5), node6(Node_6), node7(Node_7), node8(Node_8),
      surface_load_magnitude1(SurfaceLoadMagnitude1),
      surface_load_magnitude2(SurfaceLoadMagnitude2),
      surface_load_magnitude3(SurfaceLoadMagnitude3),
      surface_load_magnitude4(SurfaceLoadMagnitude4),
      surface_load_magnitude5(SurfaceLoadMagnitude5),
      surface_load_magnitude6(SurfaceLoadMagnitude6),
      surface_load_magnitude7(SurfaceLoadMagnitude7),
      surface_load_magnitude8(SurfaceLoadMagnitude8),
      data(0)
{

    data = &dataV16;

}




BrickSurfaceLoad::BrickSurfaceLoad(int tag,
                                   const ID& theElementTags,
                                   int Node_1, int Node_2, int Node_3, int Node_4,
                                   int Node_5, int Node_6, int Node_7, int Node_8, int Node_9,
                                   double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                                   double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4,
                                   double SurfaceLoadMagnitude5, double SurfaceLoadMagnitude6,
                                   double SurfaceLoadMagnitude7, double SurfaceLoadMagnitude8,
                                   double SurfaceLoadMagnitude9)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      node5(Node_5), node6(Node_6), node7(Node_7), node8(Node_8), node9(Node_9),
      surface_load_magnitude1(SurfaceLoadMagnitude1),
      surface_load_magnitude2(SurfaceLoadMagnitude2),
      surface_load_magnitude3(SurfaceLoadMagnitude3),
      surface_load_magnitude4(SurfaceLoadMagnitude4),
      surface_load_magnitude5(SurfaceLoadMagnitude5),
      surface_load_magnitude6(SurfaceLoadMagnitude6),
      surface_load_magnitude7(SurfaceLoadMagnitude7),
      surface_load_magnitude8(SurfaceLoadMagnitude8),
      surface_load_magnitude9(SurfaceLoadMagnitude9),
      data(0)
{

    data = &dataV18;

}



BrickSurfaceLoad::BrickSurfaceLoad()
    : ElementalLoad(LOAD_TAG_BrickSurfaceLoad),
      node1(0), node2(0), node3(0), node4(0),
      node5(0), node6(0), node7(0), node8(0), node9(0),
      surface_load_magnitude1(0.0),
      surface_load_magnitude2(0.0),
      surface_load_magnitude3(0.0),
      surface_load_magnitude4(0.0),
      surface_load_magnitude5(0.0),
      surface_load_magnitude6(0.0),
      surface_load_magnitude7(0.0),
      surface_load_magnitude8(0.0),
      surface_load_magnitude9(0.0),
      data(0)
{
    // data = &dataV18;
}


BrickSurfaceLoad::~BrickSurfaceLoad()
{

}


const Vector&
BrickSurfaceLoad::getData(int& type, double loadFactor)
{
    type = LOAD_TAG_BrickSurfaceLoad;

    if ( (*data).Size() == 8 )
    {
        (*data)(0) = node1;
        (*data)(1) = node2;
        (*data)(2) = node3;
        (*data)(3) = node4;
        (*data)(4) = surface_load_magnitude1;
        (*data)(5) = surface_load_magnitude2;
        (*data)(6) = surface_load_magnitude3;
        (*data)(7) = surface_load_magnitude4;

        // cout << "data = ";
        // for (int i = 0; i < 8; i++)
        // {
        //   cout << (*data)[i] << " ";
        // }
        // cout << endl;
    }
    else if ( (*data).Size() == 16 )
    {
        (*data)(0)  = node1;
        (*data)(1)  = node2;
        (*data)(2)  = node3;
        (*data)(3)  = node4;
        (*data)(4)  = node5;
        (*data)(5)  = node6;
        (*data)(6)  = node7;
        (*data)(7)  = node8;
        (*data)(8)  = surface_load_magnitude1;
        (*data)(9)  = surface_load_magnitude2;
        (*data)(10) = surface_load_magnitude3;
        (*data)(11) = surface_load_magnitude4;
        (*data)(12) = surface_load_magnitude5;
        (*data)(13) = surface_load_magnitude6;
        (*data)(14) = surface_load_magnitude7;
        (*data)(15) = surface_load_magnitude8;

    }
    else if ( (*data).Size() == 18 )
    {
        (*data)(0) = node1;
        (*data)(1) = node2;
        (*data)(2) = node3;
        (*data)(3) = node4;
        (*data)(4) = node5;
        (*data)(5) = node6;
        (*data)(6) = node7;
        (*data)(7) = node8;
        (*data)(8) = node9;
        (*data)(9)  = surface_load_magnitude1;
        (*data)(10) = surface_load_magnitude2;
        (*data)(11) = surface_load_magnitude3;
        (*data)(12) = surface_load_magnitude4;
        (*data)(13) = surface_load_magnitude5;
        (*data)(14) = surface_load_magnitude6;
        (*data)(15) = surface_load_magnitude7;
        (*data)(16) = surface_load_magnitude8;
        (*data)(17) = surface_load_magnitude9;
    }


    return *data;

}

int
BrickSurfaceLoad::sendSelf(int commitTag, Channel& theChannel)
{
    // cerr << "BrickSurfaceLoad::sendSelf() \n";
    this->ElementalLoad::sendSelf(commitTag, theChannel);

    static ID nodes(9);
    static Vector magnitudes(9);

    nodes(0) = node1;
    nodes(1) = node2;
    nodes(2) = node3;
    nodes(3) = node4;
    nodes(4) = node5;
    nodes(5) = node6;
    nodes(6) = node7;
    nodes(7) = node8;
    nodes(8) = node9;


    magnitudes(0) = surface_load_magnitude1;
    magnitudes(1) = surface_load_magnitude2;
    magnitudes(2) = surface_load_magnitude3;
    magnitudes(3) = surface_load_magnitude4;
    magnitudes(4) = surface_load_magnitude5;
    magnitudes(5) = surface_load_magnitude6;
    magnitudes(6) = surface_load_magnitude7;
    magnitudes(7) = surface_load_magnitude8;
    magnitudes(8) = surface_load_magnitude9;


    int result = theChannel.sendID(0, commitTag, nodes);
    if (result < 0)
    {
        cerr << "BrickSurfaceLoad::sendSelf() - failed to nodes\n";
        return result;
    }

    result = theChannel.sendVector(0, commitTag, magnitudes);
    if (result < 0)
    {
        cerr << "BrickSurfaceLoad::sendSelf() - magnitudes\n";
        return result;
    }
    // cout << "Send = ";
    // for (int i = 0; i < 9; i++)
    // {
    //     cout << nodes(i) << "(" << magnitudes(i) << ") ";
    // }
    // cout << endl;

    return 0;
}

int
BrickSurfaceLoad::receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{

    // cout << "BrickSurfaceLoad::receiveSelf(...)" << endl;
    this->ElementalLoad::receiveSelf(commitTag, theChannel, theBroker);

    const ID & elementtags = this->getElementTags();

    int nelem = elementtags.Size();
    // cerr << "BrickSurfaceLoad::receiveSelf() - nelem = " << nelem << " \n";

    if (nelem = 4)
    {
        data = &dataV8;
    }
    else if (nelem = 8)
    {
        data = &dataV16;
    }
    else if (nelem = 9)

    {
        data = &dataV18;
    }

    static ID nodes(9);
    static Vector magnitudes(9);

    int result = theChannel.receiveID(0, commitTag, nodes);
    if (result < 0)
    {
        cerr << "BrickSurfaceLoad::receiveSelf() - failed to nodes\n";
        return result;
    }

    result = theChannel.receiveVector(0, commitTag, magnitudes);
    if (result < 0)
    {
        cerr << "BrickSurfaceLoad::receiveSelf() - magnitudes\n";
        return result;
    }

    node1 = nodes(0);
    node2 = nodes(1);
    node3 = nodes(2);
    node4 = nodes(3);
    node5 = nodes(4);
    node6 = nodes(5);
    node7 = nodes(6);
    node8 = nodes(7);
    node9 = nodes(8);


    surface_load_magnitude1 = magnitudes(0);
    surface_load_magnitude2 = magnitudes(1);
    surface_load_magnitude3 = magnitudes(2);
    surface_load_magnitude4 = magnitudes(3);
    surface_load_magnitude5 = magnitudes(4);
    surface_load_magnitude6 = magnitudes(5);
    surface_load_magnitude7 = magnitudes(6);
    surface_load_magnitude8 = magnitudes(7);
    surface_load_magnitude9 = magnitudes(8);

    // cout << "Receive = ";
    // for (int i = 0; i < 9; i++)
    // {
    //     cout << nodes(i) << "(" << magnitudes(i) << ") ";
    // }
    // cout << endl;

    return 0;

}

void
BrickSurfaceLoad::Print(ostream& s, int flag)
{
    s << "BrickSurfaceLoad...";
    s << "Elements acted on: " << this->getElementTags();
}

#endif

