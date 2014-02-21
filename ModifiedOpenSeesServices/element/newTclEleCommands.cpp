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

// $Revision: 1.14 $
// $Date: 2002/04/12 21:42:24 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/TclElementCommands.cpp,v $


// File: ~/element/TclElementCommands.C
//
// Written: fmk
// Created: 07/99
// Revision: A
//
// Description: This file contains the implementation of the TclElementCommands.
// The file contains the routine TclElementCommands which is invoked by the
// TclModelBuilder.
//
// What: "@(#) TclModelBuilder.C, revA"

#include <stdlib.h>
#include <string.h>
#include <iiostream>
#include <Domain.h>

#include <ElasticBeam2d.h>
#include <ElasticBeam3d.h>

//Zhaohui Yang (UCD)
#include <EightNodeBrick.h>
#include <TwentyNodeBrick.h>

#include <CrdTransf2d.h>
#include <CrdTransf3d.h>

#include <TclModelBuilder.h>

//
// SOME STATIC POINTERS USED IN THE FUNCTIONS INVOKED BY THE INTERPRETER
//

extern void printCommand(int argc, char** argv);

//
// THE PROTOTYPES OF THE FUNCTIONS INVOKED BY THE INTERPRETER
//

extern int
TclModelBuilder_addFeapTruss(ClientData clientData, Tcl_Interp* interp,  int argc,
                             char** argv, Domain*, TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addTruss(ClientData clientData, Tcl_Interp* interp,  int argc,
                         char** argv, Domain*, TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addElasticBeam(ClientData clientData, Tcl_Interp* interp,  int argc,
                               char** argv, Domain*, TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addBrick(ClientData clientData, Tcl_Interp* interp,
                         int argc, char** argv, Domain*,
                         TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addBBarBrick(ClientData clientData, Tcl_Interp* interp,
                             int argc, char** argv, Domain*,
                             TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addShellMITC4(ClientData clientData, Tcl_Interp* interp,
                              int argc, char** argv, Domain*,
                              TclModelBuilder*, int argStart);

extern int
TclModelBuilder_addConstantPressureVolumeQuad(ClientData, Tcl_Interp*, int, char**,
        Domain*, TclModelBuilder*);

extern int
TclModelBuilder_addSimpleJoint2D(ClientData, Tcl_Interp*, int, char**,
                                 Domain*, TclModelBuilder*);

extern int
TclModelBuilder_addEnhancedQuad(ClientData, Tcl_Interp*, int, char**,
                                Domain*, TclModelBuilder*);

extern int
TclModelBuilder_addNineNodeMixedQuad(ClientData, Tcl_Interp*, int, char**,
                                     Domain*, TclModelBuilder*);


// GLF
extern int
TclModelBuilder_addZeroLength(ClientData, Tcl_Interp*, int, char**,
                              Domain*, TclModelBuilder*);

// MHS
extern int
TclModelBuilder_addZeroLengthSection(ClientData, Tcl_Interp*, int, char**,
                                     Domain*, TclModelBuilder*);

// MHS
extern int
TclModelBuilder_addZeroLengthND(ClientData, Tcl_Interp*, int, char**,
                                Domain*, TclModelBuilder*);


// REMO
extern int
TclModelBuilder_addFrameElement(ClientData, Tcl_Interp*, int, char**,
                                Domain*, TclModelBuilder*);

// MHS
extern int
TclModelBuilder_addBeamWithHinges(ClientData, Tcl_Interp*, int, char**,
                                  Domain*, TclModelBuilder*);
extern int
TclModelBuilder_addFourNodeQuad(ClientData, Tcl_Interp*, int, char**,
                                Domain*, TclModelBuilder*);
extern int
TclModelBuilder_addDispBeamColumn(ClientData, Tcl_Interp*, int, char**,
                                  Domain*, TclModelBuilder*);


//Boris Jeremic & Zhaohui
extern int TclModelBuilder_addEightNodeBrick(ClientData,
        Tcl_Interp*,
        int,
        char**,
        Domain*,
        TclModelBuilder*,
        int);
//Boris Jeremic & Zhaohui
extern int TclModelBuilder_addTwentyNodeBrick(ClientData,
        Tcl_Interp*,
        int,
        char**,
        Domain*,
        TclModelBuilder*,
        int);

//Boris Jeremic & Xiaoyan 01/07/2002
extern int TclModelBuilder_addEightNodeBrick_u_p_U(ClientData,
        Tcl_Interp*,
        int,
        char**,
        Domain*,
        TclModelBuilder*,
        int);
//Boris Jeremic & Xiaoyan 01/07/2002
extern int TclModelBuilder_addTwentyNodeBrick_u_p_U(ClientData,
        Tcl_Interp*,
        int,
        char**,
        Domain*,
        TclModelBuilder*,
        int);


int
TclModelBuilderElementCommand(ClientData clientData, Tcl_Interp* interp,
                              int argc, char** argv,
                              Domain* theTclDomain, TclModelBuilder* theTclBuilder)
{
    // ensure the destructor has not been called -
    if (theTclBuilder == 0)
    {
        cerr << "WARNING builder has been destroyed\n";
        return TCL_ERROR;
    }

    // check at least two arguments so don't segemnt fault on strcmp
    if (argc < 2)
    {
        cerr << "WARNING need to specify an element type\n";
        cerr << "Want: element eleType <specific element args>\n";
        cerr << "Valid types: truss, elasticBeamColumn, nonlinearBeamColumn\n";
        return TCL_ERROR;
    }

    if (strcmp(argv[1], "fTruss") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addFeapTruss(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder, eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "truss") == 0 || strcmp(argv[1], "corotTruss") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addTruss(clientData, interp, argc, argv,
                                              theTclDomain, theTclBuilder, eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "elasticBeamColumn") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addElasticBeam(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder, eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "nonlinearBeamColumn") == 0)
    {
        int result = TclModelBuilder_addFrameElement(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "dispBeamColumn") == 0)
    {
        int result = TclModelBuilder_addDispBeamColumn(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "beamWithHinges") == 0)
    {
        int result = TclModelBuilder_addBeamWithHinges(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "quad") == 0)
    {
        int result = TclModelBuilder_addFourNodeQuad(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "enhancedQuad") == 0)
    {
        int result = TclModelBuilder_addEnhancedQuad(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if ((strcmp(argv[1], "bbarQuad") == 0) || (strcmp(argv[1], "mixedQuad") == 0))
    {
        int result = TclModelBuilder_addConstantPressureVolumeQuad(clientData, interp,
                     argc, argv,
                     theTclDomain,
                     theTclBuilder);
        return result;
    }
    else if ((strcmp(argv[1], "nineNodeMixedQuad") == 0)
             || (strcmp(argv[1], "nineNodeQuad") == 0))
    {
        int result = TclModelBuilder_addNineNodeMixedQuad(clientData, interp,
                     argc, argv,
                     theTclDomain,
                     theTclBuilder);
        return result;
    }
    else if ((strcmp(argv[1], "shell") == 0) || (strcmp(argv[1], "shellMITC4") == 0))
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addShellMITC4(clientData, interp,
                     argc, argv,
                     theTclDomain,
                     theTclBuilder,
                     eleArgStart);
        return result;
    }

    //Boris Jeremic & Zhaohui
    else if (strcmp(argv[1], "Brick8N") == 0)
    {

        int eleArgStart = 1;
        int result = TclModelBuilder_addEightNodeBrick(clientData,
                     interp,
                     argc,
                     argv,
                     theTclDomain,
                     theTclBuilder,
                     eleArgStart);
        return result;
    }

    //Boris Jeremic & Zhaohui
    else if (strcmp(argv[1], "Brick20N") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addTwentyNodeBrick(clientData,
                     interp,
                     argc,
                     argv,
                     theTclDomain,
                     theTclBuilder,
                     eleArgStart);
        return result;
    }

    //Boris Jeremic & Zhaohui
    else if (strcmp(argv[1], "Brick8N_u_p_U") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addEightNodeBrick_u_p_U(clientData,
                     interp,
                     argc,
                     argv,
                     theTclDomain,
                     theTclBuilder,
                     eleArgStart);
        return result;
    }
    //Boris Jeremic & Zhaohui
    else if (strcmp(argv[1], "Brick20N_u_p_U") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addTwentyNodeBrick_u_p_U(clientData,
                     interp,
                     argc,
                     argv,
                     theTclDomain,
                     theTclBuilder,
                     eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "stdBrick") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addBrick(clientData, interp, argc, argv,
                                              theTclDomain, theTclBuilder, eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "bbarBrick") == 0)
    {
        int eleArgStart = 1;
        int result = TclModelBuilder_addBBarBrick(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder, eleArgStart);
        return result;
    }
    else if (strcmp(argv[1], "zeroLength") == 0)
    {
        int result = TclModelBuilder_addZeroLength(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "zeroLengthSection") == 0)
    {
        int result = TclModelBuilder_addZeroLengthSection(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "zeroLengthND") == 0)
    {
        int result = TclModelBuilder_addZeroLengthND(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else if (strcmp(argv[1], "SimpleJoint2D") == 0)
    {
        int result = TclModelBuilder_addSimpleJoint2D(clientData, interp, argc, argv,
                     theTclDomain, theTclBuilder);
        return result;
    }
    else
    {
        cerr << "WARNING unknown element type: " <<  argv[1];
        cerr << "Valid types: truss, elasticBeamColumn, nonlinearBeamColumn, " << endln
               << "beamWithHinges, zeroLength, quad, brick, shellMITC4\n";
        return TCL_ERROR;
    }
}
