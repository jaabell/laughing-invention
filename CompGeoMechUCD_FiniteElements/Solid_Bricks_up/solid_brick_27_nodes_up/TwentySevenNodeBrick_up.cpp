///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic and Jose && Sumeet 
// DATE:               NOv. 2013
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

/////////////////////////////////////////////////////////////////////////////////////
// Sumeet Kumar Sinha [September, 2016]
// This is a Header file that is taken from MasterTwentySevenNodeBrick_up.h
// The macros (#define) that go into this master BrickHeaderFile are 
//
// BRICK_TYPE -> define the element type like "EightNodeBrick_up"
// BRICK_ELEMENT_NAME -> Name of the brick in string Eg:- "EightNodeBrick_up"
// NUMBER_OF_NODES    -> Number of nodes in the brick elemnt Eg:- 8 for eight node brick
// ELE_TAG_BRICK_TYPE -> ELE_TAFG for the brick element
// Order              -> Order of intergration
//
// This master header file was created to make the code moduluar and consistent 
// over all brick elements
/////////////////////////////////////////////////////////////////////////////////////

#ifndef TwentySevenNodeBrick_up_CPP
#define TwentySevenNodeBrick_up_CPP

#include "TwentySevenNodeBrick_up.h"

#undef  ELE_TAG_BRICK_TYPE
#define ELE_TAG_BRICK_TYPE  ELE_TAG_TwentySevenNodeBrick_up

#undef  Order
#define Order 3


#include "../MasterTwentySevenNodeBrick_up.cpp"


#endif

