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


#ifndef BrickSurfaceLoad_from_Contact_Pore_Pressure_CPP
#define BrickSurfaceLoad_from_Contact_Pore_Pressure_CPP


#include <BrickSurfaceLoad_from_Contact_Pore_Pressure.h>
#include <Vector.h>
#include <Domain.h>
#include <Element.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <iostream>
using namespace std;


Vector BrickSurfaceLoad_from_Contact_Pore_Pressure::dataV8(8);
Vector BrickSurfaceLoad_from_Contact_Pore_Pressure::dataV16(16);
Vector BrickSurfaceLoad_from_Contact_Pore_Pressure::dataV18(18);


BrickSurfaceLoad_from_Contact_Pore_Pressure::BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag,
        const ID& theElementTags,
        int Node_1, int Node_2, int Node_3, int Node_4,
        double ContactElementNumber1, double ContactElementNumber2,
        double ContactElementNumber3, double ContactElementNumber4)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      contact_element_number_1(ContactElementNumber1),
      contact_element_number_2(ContactElementNumber2),
      contact_element_number_3(ContactElementNumber3),
      contact_element_number_4(ContactElementNumber4),
      data(0)
{

    data = &dataV8;

}



BrickSurfaceLoad_from_Contact_Pore_Pressure::BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag,
        const ID& theElementTags,
        int Node_1, int Node_2, int Node_3, int Node_4,
        int Node_5, int Node_6, int Node_7, int Node_8,
        double ContactElementNumber1, double ContactElementNumber2,
        double ContactElementNumber3, double ContactElementNumber4,
        double ContactElementNumber5, double ContactElementNumber6,
        double ContactElementNumber7, double ContactElementNumber8)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      node5(Node_5), node6(Node_6), node7(Node_7), node8(Node_8),
      contact_element_number_1(ContactElementNumber1),
      contact_element_number_2(ContactElementNumber2),
      contact_element_number_3(ContactElementNumber3),
      contact_element_number_4(ContactElementNumber4),
      contact_element_number_5(ContactElementNumber5),
      contact_element_number_6(ContactElementNumber6),
      contact_element_number_7(ContactElementNumber7),
      contact_element_number_8(ContactElementNumber8),
      data(0)
{

    data = &dataV16;

}




BrickSurfaceLoad_from_Contact_Pore_Pressure::BrickSurfaceLoad_from_Contact_Pore_Pressure(int tag,
        const ID& theElementTags,
        int Node_1, int Node_2, int Node_3, int Node_4,
        int Node_5, int Node_6, int Node_7, int Node_8, int Node_9,
        double ContactElementNumber1, double ContactElementNumber2,
        double ContactElementNumber3, double ContactElementNumber4,
        double ContactElementNumber5, double ContactElementNumber6,
        double ContactElementNumber7, double ContactElementNumber8,
        double ContactElementNumber9)
    : ElementalLoad(tag, LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure, theElementTags),
      node1(Node_1), node2(Node_2), node3(Node_3), node4(Node_4),
      node5(Node_5), node6(Node_6), node7(Node_7), node8(Node_8), node9(Node_9),
      contact_element_number_1(ContactElementNumber1),
      contact_element_number_2(ContactElementNumber2),
      contact_element_number_3(ContactElementNumber3),
      contact_element_number_4(ContactElementNumber4),
      contact_element_number_5(ContactElementNumber5),
      contact_element_number_6(ContactElementNumber6),
      contact_element_number_7(ContactElementNumber7),
      contact_element_number_8(ContactElementNumber8),
      contact_element_number_9(ContactElementNumber9),
      data(0)
{

    data = &dataV18;

}



BrickSurfaceLoad_from_Contact_Pore_Pressure::BrickSurfaceLoad_from_Contact_Pore_Pressure()
    : ElementalLoad(LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure)
{

}


BrickSurfaceLoad_from_Contact_Pore_Pressure::~BrickSurfaceLoad_from_Contact_Pore_Pressure()
{

}


const Vector&
BrickSurfaceLoad_from_Contact_Pore_Pressure::getData(int& type, double loadFactor)
{
    type = LOAD_TAG_BrickSurfaceLoad_from_Contact_Pore_Pressure;


    Domain* theDomain = this->getDomain();

    if (theDomain == 0)
    {
        cerr << "BrickSurfaceLoad_from_Contact_Pore_Pressure::getData pointer to domain is empty!\n";
    }



    if ( (*data).Size() == 8 )
    {


        double SurfaceLoadMagnitude1 = theDomain->getElement(contact_element_number_1)->returnPressure();
        double SurfaceLoadMagnitude2 = theDomain->getElement(contact_element_number_2)->returnPressure();
        double SurfaceLoadMagnitude3 = theDomain->getElement(contact_element_number_3)->returnPressure();
        double SurfaceLoadMagnitude4 = theDomain->getElement(contact_element_number_4)->returnPressure();


        cout << "\nPressures: " << SurfaceLoadMagnitude1 << "   " << SurfaceLoadMagnitude2 << "   " << SurfaceLoadMagnitude3 << "   " << SurfaceLoadMagnitude4 << endl;


        (*data)(0) = node1;
        (*data)(1) = node2;
        (*data)(2) = node3;
        (*data)(3) = node4;
        (*data)(4) = SurfaceLoadMagnitude1;
        (*data)(5) = SurfaceLoadMagnitude2;
        (*data)(6) = SurfaceLoadMagnitude3;
        (*data)(7) = SurfaceLoadMagnitude4;

    }
    else if ( (*data).Size() == 16 )
    {


        double SurfaceLoadMagnitude1 = theDomain->getElement(contact_element_number_1)->returnPressure();
        double SurfaceLoadMagnitude2 = theDomain->getElement(contact_element_number_2)->returnPressure();
        double SurfaceLoadMagnitude3 = theDomain->getElement(contact_element_number_3)->returnPressure();
        double SurfaceLoadMagnitude4 = theDomain->getElement(contact_element_number_4)->returnPressure();
        double SurfaceLoadMagnitude5 = theDomain->getElement(contact_element_number_5)->returnPressure();
        double SurfaceLoadMagnitude6 = theDomain->getElement(contact_element_number_6)->returnPressure();
        double SurfaceLoadMagnitude7 = theDomain->getElement(contact_element_number_7)->returnPressure();
        double SurfaceLoadMagnitude8 = theDomain->getElement(contact_element_number_8)->returnPressure();


        (*data)(0)  = node1;
        (*data)(1)  = node2;
        (*data)(2)  = node3;
        (*data)(3)  = node4;
        (*data)(4)  = node5;
        (*data)(5)  = node6;
        (*data)(6)  = node7;
        (*data)(7)  = node8;
        (*data)(8)  = SurfaceLoadMagnitude1;
        (*data)(9)  = SurfaceLoadMagnitude2;
        (*data)(10) = SurfaceLoadMagnitude3;
        (*data)(11) = SurfaceLoadMagnitude4;
        (*data)(12) = SurfaceLoadMagnitude5;
        (*data)(13) = SurfaceLoadMagnitude6;
        (*data)(14) = SurfaceLoadMagnitude7;
        (*data)(15) = SurfaceLoadMagnitude8;

    }
    else if ( (*data).Size() == 18 )
    {


        double SurfaceLoadMagnitude1 = theDomain->getElement(contact_element_number_1)->returnPressure();
        double SurfaceLoadMagnitude2 = theDomain->getElement(contact_element_number_2)->returnPressure();
        double SurfaceLoadMagnitude3 = theDomain->getElement(contact_element_number_3)->returnPressure();
        double SurfaceLoadMagnitude4 = theDomain->getElement(contact_element_number_4)->returnPressure();
        double SurfaceLoadMagnitude5 = theDomain->getElement(contact_element_number_5)->returnPressure();
        double SurfaceLoadMagnitude6 = theDomain->getElement(contact_element_number_6)->returnPressure();
        double SurfaceLoadMagnitude7 = theDomain->getElement(contact_element_number_7)->returnPressure();
        double SurfaceLoadMagnitude8 = theDomain->getElement(contact_element_number_8)->returnPressure();
        double SurfaceLoadMagnitude9 = theDomain->getElement(contact_element_number_9)->returnPressure();


        (*data)(0) = node1;
        (*data)(1) = node2;
        (*data)(2) = node3;
        (*data)(3) = node4;
        (*data)(4) = node5;
        (*data)(5) = node6;
        (*data)(6) = node7;
        (*data)(7) = node8;
        (*data)(8) = node9;
        (*data)(9)  = SurfaceLoadMagnitude1;
        (*data)(10) = SurfaceLoadMagnitude2;
        (*data)(11) = SurfaceLoadMagnitude3;
        (*data)(12) = SurfaceLoadMagnitude4;
        (*data)(13) = SurfaceLoadMagnitude5;
        (*data)(14) = SurfaceLoadMagnitude6;
        (*data)(15) = SurfaceLoadMagnitude7;
        (*data)(16) = SurfaceLoadMagnitude8;
        (*data)(17) = SurfaceLoadMagnitude9;
    }


    return *data;

}

int
BrickSurfaceLoad_from_Contact_Pore_Pressure::sendSelf(int commitTag, Channel& theChannel)
{
    // (Nima) SHOULD BE CHECKED IN PARALLEL VERSION

    //   int dbTag = this->getDbTag();
    //   int myTag = this->getTag();
    //   const ID &theElements = this->getElementTags();
    //
    //   static ID idData(10);
    //   idData(0) = theElements.Size();
    //   idData(1) = myTag;
    //   idData(2) = node1;
    //   idData(3) = node2;
    //   idData(4) = node3;
    //   idData(5) = node4;
    //   idData(6) = contact_element_number_1;
    //   idData(7) = contact_element_number_2;
    //   idData(8) = contact_element_number_3;
    //   idData(9) = contact_element_number_4;
    //
    //
    //   int result = theChannel.sendID(dbTag, commitTag, idData);
    //   if (result < 0) {
    //     cerr << "BrickSurfaceLoad_from_Contact_Pore_Pressure::sendSelf() - failed to send data\n";
    //     return result;
    //   }
    //
    //   result = theChannel.sendID(dbTag, commitTag, theElements);
    //   if (result < 0) {
    //     cerr << "BrickSurfaceLoad_from_Contact_Pore_Pressure::sendSelf() - failed to send element tags\n";
    //     return result;
    //   }
    //
    return 0;
}

int
BrickSurfaceLoad_from_Contact_Pore_Pressure::recvSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{
    // (Nima) SHOULD BE CHECKED IN PARALLEL VERSION

    //   int dbTag = this->getDbTag();
    //   static ID idData(10);
    //
    //   int result = theChannel.recvID(dbTag, commitTag, idData);
    //   if (result < 0) {
    //     cerr << "BrickSurfaceLoad_from_Contact_Pore_Pressure::recvSelf() - failed to recv data\n";
    //     return result;
    //   }
    //
    //   int numEle = idData(0);
    //   int myTag = idData(1);
    //
    //   node1 = idData(2);
    //   node2 = idData(3);
    //   node3 = idData(4);
    //   node4 = idData(5);
    //   contact_element_number_1 = idData(6);
    //   contact_element_number_2 = idData(7);
    //   contact_element_number_3 = idData(8);
    //   contact_element_number_4 = idData(9);
    //
    //
    //   ID *NewTags = new ID(numEle);
    //
    //   result = theChannel.recvID(dbTag, commitTag, *NewTags);
    //   if (result < 0) {
    //     cerr << "BrickSurfaceLoad_from_Contact_Pore_Pressure::recvSelf() - failed to recv element tags\n";
    //     return result;
    //   }
    //
    //   this->setTag(myTag);
    //   this->setElementTags(*NewTags);
    //   delete NewTags;
    //
    return 0;
}

void
BrickSurfaceLoad_from_Contact_Pore_Pressure::Print(ostream& s, int flag)
{
    s << "BrickSurfaceLoad_from_Contact_Pore_Pressure...";
    s << "Elements acted on: " << this->getElementTags();
}

#endif

