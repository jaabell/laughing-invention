///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli and Boris Jeremic && Sumeet 
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
// BRICK_TYPE -> define the element type like "TwentySevenNodeBrick_up"
// BRICK_ELEMENT_NAME -> Name of the brick in string Eg:- "TwentySevenNodeBrick_up"
// NUMBER_OF_NODES    -> Number of nodes in the brick elemnt Eg:- 8 for TwentySeven node brick
//
// This master header file was created to make the code moduluar and consistent 
// over all brick elements
/////////////////////////////////////////////////////////////////////////////////////

#ifndef TwentySevenNodeBrickOrderOne_up_H
#define TwentySevenNodeBrickOrderOne_up_H

#undef BRICK_TYPE
#define BRICK_TYPE TwentySevenNodeBrickOrderOne_up

#undef BRICK_ELEMENT_NAME
#define BRICK_ELEMENT_NAME "TwentySevenNodeBrickOrderOne_up"

#undef NUMBER_NODES
#define NUMBER_NODES 27


#include "../MasterTwentySevenNodeBrick_up.h"

#endif
