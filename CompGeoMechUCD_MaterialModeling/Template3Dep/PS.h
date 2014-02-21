
//##################################################################################
//# COPYRIGHT (C):     :-))                                                        #
//# PROJECT:           Object Oriented Finite Element Program                      #
//# PURPOSE:           General platform for elaso-plastic constitutive model       #
//#                    implementation                                              #
//# CLASS:             PotentialSurface(the base class for all potential surfaces) #
//#                                                                                #
//# VERSION:                                                                       #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )    #
//# TARGET OS:         DOS || UNIX || . . .                                        #
//# DESIGNER(S):       Boris Jeremic, Zhaohui Yang                                 #
//# PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                                 #
//#                                                                                #
//#                                                                                #
//# DATE:              08-03-2000                                                  #
//# UPDATE HISTORY:                                                                #
//#                                                                                #
//#                                                                                #
//#                                                                                #
//#                                                                                #
//# SHORT EXPLANATION: The goal is to create a platform for efficient and easy     #
//#                    implemetation of any elasto-plastic constitutive model!     #
//#                                                                                #
//##################################################################################
//

#ifndef PS_H
#define PS_H

#include <stresst.h>
#include <straint.h>
#include <BJtensor.h>

#include "EPState.h"

//Guanzhou
#include <Channel.h>
#include <G3Globals.h>
#include <MovableObject.h>

class PotentialSurface : public MovableObject
{
    public:
        PotentialSurface(int clsTag) : MovableObject(clsTag) {};//Guanzhou added, PS has to be tagged send/recv
        virtual PotentialSurface* newObj()  = 0;  //create a colne of itself
        virtual ~PotentialSurface() {};       // Codewizard requires virtual destructor
        double  q() const
        {
            return 0.0;
        }; // Codewizard does not like function defined in class definition
        virtual tensor dQods(const EPState* EPS ) const =  0;  //pure virtual func
        virtual tensor d2Qods2(const EPState* EPS ) const = 0; //pure virtual func
        //    virtual void print() = 0; //pure virtual func

        // Added for Consistent Algorithm, Z. Cheng, Jan 2004
        // Ref. Jeremic & Sture, Mechanics of Cohesive-Frictional Materials, Vol.2 165-183 (1997)
        virtual tensor d2Qodsds1(const EPState* EPS ) const;
        virtual tensor d2Qodsds2(const EPState* EPS ) const;
        virtual tensor d2Qodsds3(const EPState* EPS ) const;
        virtual tensor d2Qodsds4(const EPState* EPS ) const;
        virtual tensor d2Qodsdt1(const EPState* EPS ) const;
        virtual tensor d2Qodsdt2(const EPState* EPS ) const;
        virtual tensor d2Qodsdt3(const EPState* EPS ) const;
        virtual tensor d2Qodsdt4(const EPState* EPS ) const;

        //Guanzhou added basic prototype for send/recv
        int sendSelf(int commitTag, Channel& theChannel)
        {
            return 0;
        };
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker)
        {
            return 0;
        };

        //      //================================================================================
        //      // Overloaded Insertion Operator
        //      // prints an PotentialSurface's contents
        //      //================================================================================
        //      friend ostream& operator<< (ostream& os, const PotentialSurface & PS)
        //      {
        //         os << "Potential Surface Parameters: " << endln;
        //         return os;
        //      }
};


#endif

