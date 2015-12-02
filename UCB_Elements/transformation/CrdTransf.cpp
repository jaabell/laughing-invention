/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** See Copyright end of file.                                         **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision: 1.5 $
// $Date: 2009-08-19 17:53:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/coordTransformation/CrdTransf.cpp,v $

// Written: Remo Magalhaes de Souza (rmsouza@ce.berkeley.edu)
// Created: 04/2000
// Revision: A
//
// Description: This file contains the implementation for the CrdTransf class.
// CrdTransf provides the abstraction of a frame
// coordinate transformation. It is an abstract base class and
// thus no objects of  it's type can be instatiated.

#include <CrdTransf.h>
#include <Vector.h>

#include <TaggedObject.h>
#include <MapOfTaggedObjects.h>

static MapOfTaggedObjects theCrdTransfObjects;

bool OPS_AddCrdTransf(CrdTransf *newComponent)
{
    return theCrdTransfObjects.addComponent(newComponent);
}

CrdTransf *OPS_GetCrdTransf(int tag)
{

    TaggedObject *theResult = theCrdTransfObjects.getComponentPtr(tag);
    if (theResult == 0)
    {
        std::cerr << "CrdTransf *getCrdTransf(int tag) - none found with tag: " << tag << endln;
        return 0;
    }
    CrdTransf *theSeries = (CrdTransf *)theResult;

    return theSeries;
}

void OPS_ClearAllCrdTransf(void)
{
    theCrdTransfObjects.clearAll();
}


// constructor:
CrdTransf::CrdTransf(int tag, int classTag): TaggedObject(tag), MovableObject(classTag)
{
}

// destructor:
CrdTransf::~CrdTransf()
{
}

// const Vector &
// CrdTransf::getBasicDisplSensitivity(int gradNumber)
// {
//     std::cerr << "WARNING CrdTransf::getBasicDisplSensitivity() - this method "
//               << " should not be called." << endln;

//     static Vector dummy(1);
//     return dummy;
// }

// const Vector &
// CrdTransf::getGlobalResistingForceShapeSensitivity(const Vector &pb,
//         const Vector &p0,
//         int gradNumber)
// {
//     std::cerr << "ERROR CrdTransf::getGlobalResistingForceSensitivity() - has not been"
//               << " implemented yet for the chosen transformation." << endln;

//     static Vector dummy(1);
//     return dummy;
// }

// const Vector &
// CrdTransf::getGlobalResistingForceShapeSensitivity(const Vector &pb,
//         const Vector &p0)
// {
//     std::cerr << "ERROR CrdTransf::getGlobalResistingForceSensitivity() - has not been"
//               << " implemented yet for the chosen transformation." << endln;

//     static Vector dummy(1);
//     return dummy;
// }

// const Vector &
// CrdTransf::getBasicTrialDispShapeSensitivity(void)
// {
//     std::cerr << "ERROR CrdTransf::getBasicTrialDispShapeSensitivity() - has not been"
//               << " implemented yet for the chosen transformation." << endln;

//     static Vector dummy(1);
//     return dummy;
// }



// // --Quan
// const Vector &
// CrdTransf::getBasicDisplSensitivity(int gradNumber, int)
// {
//     std::cerr << "WARNING CrdTransf::getBasicDisplSensitivity() - this method "
//               << " should not be called." << endln;

//     static Vector dummy(1);
//     return dummy;
// }

/*
Copyright @ 1999,2000 The Regents of the University of California (The Regents).
All Rights Reserved.

The Regents grants permission, without fee and without a written license agreement,
for (a) use, reproduction, modification, and distribution of this software and its
documentation by educational, research, and non-profit entities for noncommercial
purposes only; and (b) use, reproduction and modification of this software by other
entities for internal purposes only. The above copyright notice, this paragraph and
the following three paragraphs must appear in all copies and modifications of the
software and/or documentation.

Permission to incorporate this software into products for commercial distribution
may be obtained
by contacting the University of California
Office of Technology Licensing
2150 Shattuck Avenue #510,
Berkeley, CA 94720-1620,
(510) 643-7201.

This software program and documentation are copyrighted by The Regents of the University
of California. The Regents does not warrant that the operation of the program will be
uninterrupted or error-free. The end-user understands that the program was developed
for research purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE
AND ITS DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
REGENTS GRANTS NO EXPRESS OR IMPLIED LICENSE IN ANY PATENT RIGHTS OF REGENTS BUT HAS
IMPLEMENTED AN INDIVIDUAL CONTRIBUTOR LICENSE AGREEMENT FOR THE OPENSEES PROJECT AT THE
UNIVERISTY OF CALIFORNIA, BERKELEY TO BENEFIT THE END USER.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND
ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS
NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/
