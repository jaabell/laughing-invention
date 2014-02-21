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
// PURPOSE:           Surface load for brick elements from contact element pore pressure
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              December 2013
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


#ifndef BrickSurfaceLoad_from_Contact_Pore_Pressure_h
#define BrickSurfaceLoad_from_Contact_Pore_Pressure_h


#include <ElementalLoad.h>

class BrickSurfaceLoad_from_Contact_Pore_Pressure : public ElementalLoad
{
    public:
        BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag, const ID& theElementTags,
                int Node_1, int Node_2, int Node_3, int Node_4,
                double ContactElementNumber1, double ContactElementNumber2,
                double ContactElementNumber3, double ContactElementNumber4);

        BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag, const ID& theElementTags,
                int Node_1, int Node_2, int Node_3, int Node_4,
                int Node_5, int Node_6, int Node_7, int Node_8,
                double ContactElementNumber1, double ContactElementNumber2,
                double ContactElementNumber3, double ContactElementNumber4,
                double ContactElementNumber5, double ContactElementNumber6,
                double ContactElementNumber7, double ContactElementNumber8);

        BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag, const ID& theElementTags,
                int Node_1, int Node_2, int Node_3, int Node_4,
                int Node_5, int Node_6, int Node_7, int Node_8, int Node_9,
                double ContactElementNumber1, double ContactElementNumber2,
                double ContactElementNumber3, double ContactElementNumber4,
                double ContactElementNumber5, double ContactElementNumber6,
                double ContactElementNumber7, double ContactElementNumber8,
                double ContactElementNumber9);





        BrickSurfaceLoad_from_Contact_Pore_Pressure();
        ~BrickSurfaceLoad_from_Contact_Pore_Pressure();

        const Vector& getData(int& type, double loadFactor);

        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker);
        void Print(ostream& s, int flag = 0);

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

        double contact_element_number_1;
        double contact_element_number_2;
        double contact_element_number_3;
        double contact_element_number_4;
        double contact_element_number_5;
        double contact_element_number_6;
        double contact_element_number_7;
        double contact_element_number_8;
        double contact_element_number_9;
};

#endif

