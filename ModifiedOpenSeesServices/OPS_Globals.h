#ifndef _OPS_Globals_h
#define _OPS_Globals_h

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

// $Revision: 1.5 $
// $Date: 2004/02/18 23:31:46 $
// $Source: /usr/local/cvs/OpenSees/SRC/OPS_Globals.h,v $

// Written: fmk
// Created: 11/99
//
// Description: This file contains global variables used in OpenSees files.
// if you change some of the variables, you must recompile ALL the code.

#define _USING_OpenSees_STREAMS
#include <iostream>
using namespace std;
//extern ostream& cerr;
#define endln "\n"

// #define _USING_STL_STREAMS
// #include <iostream>
// using std::cerr;
// using std::ostream;
// #define cerr cerr
// #define ostream ostream
// #define endln endl

#define _stdcall BLAH

#ifdef _TCL84
#define TCL_Char const char
#else
#define TCL_Char char
#endif


class Domain;
class Element;

#define MAX_FILENAMELENGTH 50

extern double   ops_Dt;                // current delta T for current domain doing an update
// extern double  *ops_Gravity;        // gravity factors for current domain undergoing an update
extern Domain  *ops_TheActiveDomain;   // current domain undergoing an update
extern Element *ops_TheActiveElement;  // current element undergoing an update

#endif
