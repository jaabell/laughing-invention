///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         .
// DATE:               .
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



// Original file, from OpenSees project, see ORIGINAL header below,
// was significantly changed, improved, massacred and massaged, so that
// this implementation/file now constitutes quite a novel development.
// In addition, ORIGINAL OpenSees license (see below) assigned
// (wrongly) copyright to the
// Regents of the University of California.
// ORIGINAL license also gave a number of permissions (see below)
// that we used.
//
//--------------------------------------------------------------------------
// Original OpenSees License:
// Copyright @ 1999,2000  The Regents of the University of California.
// All Rights Reserved.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for educational, research and non-profit purposes, without
// fee, and without a written agreement is hereby granted, provided that the
// above copyright notice, this paragraph and the following three paragraphs
// appear in all copies.
//
// Permission to incorporate this software into commercial products may be
// obtained by contacting the University of California.
// [Office of Technology Licensing,
// 2150 Shattuck Avenue #150
// Berkeley, CA 94720-1620,
// (510) 643-7201]
//
// This software program and documentation are copyrighted by The Regents of
// the University of California. The software program and documentation are
// supplied "as is", without any accompanying services from The Regents. The
// Regents does not warrant that the operation of the program will be
// uninterrupted or error-free. The end-user understands that the program was
// developed for research purposes and is advised not to rely exclusively on
// the program for any reason.
//
// IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
// DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
// LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
// DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE. THE UNIVERSITY OF CALIFORNIA SPECIFICALLY
// DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE
// SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
// CALIFORNIA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
//--------------------------------------------------------------------------
//
//
// Original header for this file:





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

// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:21 $
// $Source: /usr/local/cvs/OpenSees/SRC/graph/partitioner/Metis.h,v $


// Written: fmk
//
// Description: This file contains the class definition for Metis.
// Metis is a type of GraphPartitioner which uses 'METIS - Unstructured
// Graph Partitioning And Sparse Matrix Ordering System', developed by
// G. Karypis and V. Kumar at the University of Minnesota. The metis
// files are found in metis-2.0 which were downloaded.
//     This class provides the C++ interface for metis which will allow
// it to fit seamlessly into our system.
//
// What: "@(#) Metis.h, revA"

#ifndef Metis_h
#define Metis_h

#include <GraphPartitioner.h>
#include <GraphNumberer.h>

#ifndef _bool_h
#include <bool.h>
#endif

class Metis : public GraphPartitioner, public GraphNumberer
{
public:
    Metis(int numParts = 1);
    Metis(int Ptype,
          int Mtype,
          int coarsenTo,
          int Rtype,
          int IPtype,
          int numParts = 1);
    ~Metis();

    bool setOptions(int Ptype,
                    int Mtype,
                    int coarsenTo,
                    int Rtype,
                    int IPtype);

    bool setDefaultOptions(void);

    //int partition(Graph &theGraph, int numPart);
    int partition(Graph* theGraph, int numPart);

    // the follwing methods are if the object is to be used as a numberer
    const ID& number(Graph& theGraph, int lastVertex = -1);
    const ID& number(Graph& theGraph, const ID& lastVertices);

    int sendSelf(int commitTag,
                 Channel& theChannel);
    int receiveSelf(int commitTag, Channel& theChannel,
                    FEM_ObjectBroker& theBroker);


protected:

private:
    bool checkOptions(void);

    int myPtype ;   // package type:
    //  pmetis = 1
    //  kmetis = 2

    int myMtype;        // type of matching scheme:
    //  random = 1
    //      heavy edge = 2
    //      light edge = 3
    //  heavy clique = 4
    //  modified heavy edge = 5
    //      sorted random = 11
    //  sorted heavy edge =21
    //  sorted modified heavy edge = 51

    int myCoarsenTo;     // the number of vertices the graph is coarsened down to
    //      if pmetis default is 100
    //  if kmetis default is 2000

    int myRtype;        // type of refinement policy:
    //  greedy = 1
    //  kernighan-lin = 2
    //      combo greedy and K-L = 3
    //  boundary greedy = 11
    //      boundary K-L = 12
    //  combo of boundary greedy and boundary K-L = 13,
    //      no-refinement = 20

    int myIPtype;   // type of bisection algo:
    //      graph growing partition = 1,
    //      greedy graph growing partition = 2,
    //      spectral bisection = 3,
    //      graph growing followed by K-L = 4

    bool defaultOptions;

    int   numPartitions; // needed if to be used as a numberer
    ID*    theRefResult;
};

#endif

