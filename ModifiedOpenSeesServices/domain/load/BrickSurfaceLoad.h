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
//
//         ________
//       2 |   5   | 1
//         |       |
//         |6     8|
//       3 |___7___| 4
//
//
//
//         ________
//       2 |   5   | 1
//         |       |
//         |6  9  8|
//       3 |_______| 4
//             7
//


#ifndef BrickSurfaceLoad_h
#define BrickSurfaceLoad_h


#include <ElementalLoad.h>

class BrickSurfaceLoad : public ElementalLoad
{
public:
    BrickSurfaceLoad(int tag, const ID& theElementTags,
                     int Node_1, int Node_2, int Node_3, int Node_4,
                     double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                     double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4);

    BrickSurfaceLoad(int tag, const ID& theElementTags,
                     int Node_1, int Node_2, int Node_3, int Node_4,
                     int Node_5, int Node_6, int Node_7, int Node_8,
                     double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                     double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4,
                     double SurfaceLoadMagnitude5, double SurfaceLoadMagnitude6,
                     double SurfaceLoadMagnitude7, double SurfaceLoadMagnitude8);

    BrickSurfaceLoad(int tag, const ID& theElementTags,
                     int Node_1, int Node_2, int Node_3, int Node_4,
                     int Node_5, int Node_6, int Node_7, int Node_8, int Node_9,
                     double SurfaceLoadMagnitude1, double SurfaceLoadMagnitude2,
                     double SurfaceLoadMagnitude3, double SurfaceLoadMagnitude4,
                     double SurfaceLoadMagnitude5, double SurfaceLoadMagnitude6,
                     double SurfaceLoadMagnitude7, double SurfaceLoadMagnitude8,
                     double SurfaceLoadMagnitude9);





    BrickSurfaceLoad();
    ~BrickSurfaceLoad();

    const Vector& getData(int& type, double loadFactor);

    int sendSelf(int commitTag, Channel& theChannel);
    int receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker);

    virtual void Print(ostream& s, int flag = 0);

protected:

private:
    Vector* data;
    static Vector dataV8;
    static Vector dataV16;
    static Vector dataV18;

    int node1;
    int node2;
    int node3;
    int node4;
    int node5;
    int node6;
    int node7;
    int node8;
    int node9;

    double surface_load_magnitude1;
    double surface_load_magnitude2;
    double surface_load_magnitude3;
    double surface_load_magnitude4;
    double surface_load_magnitude5;
    double surface_load_magnitude6;
    double surface_load_magnitude7;
    double surface_load_magnitude8;
    double surface_load_magnitude9;
};

#endif

