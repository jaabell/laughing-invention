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

// $Revision: 1.7 $
// $Date: 2006/09/26 18:38:29 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/dispBeamColumn/Attic/TclDispBeamColumnCommand.cpp,v $

// Written: MHS
// Created: Feb 2001
//
// Description: This file contains the implementation of the
// TclModelBuilder_addDispBeamColumn() command.

#include <stdlib.h>
#include <string.h>
#include <Domain.h>

#include <TclModelBuilder.h>

#include <DispBeamColumn2d.h>
#include <DispBeamColumn3d.h>

#include <LobattoBeamIntegration.h>
#include <LegendreBeamIntegration.h>
#include <RadauBeamIntegration.h>
#include <NewtonCotesBeamIntegration.h>
#include <UserDefinedBeamIntegration.h>

#include <HingeMidpointBeamIntegration.h>
#include <HingeEndpointBeamIntegration.h>
#include <HingeRadauBeamIntegration.h>
#include <HingeRadauTwoBeamIntegration.h>
#include <UserDefinedHingeIntegration.h>
#include <DistHingeIntegration.h>

//#include <TrapezoidalBeamIntegration.h>
//#include <FixedLocationBeamIntegration.h>
//#include <LowOrderBeamIntegration.h>
//#include <MidDistanceBeamIntegration.h>

#include <ElasticSection2d.h>
#include <ElasticSection3d.h>

extern void printCommand(int argc, TCL_Char** argv);

int
TclModelBuilder_addDispBeamColumn(ClientData clientData, Tcl_Interp* interp,
                                  int inArgc,
                                  TCL_Char** inArgv,
                                  Domain* theTclDomain,
                                  TclModelBuilder* theTclBuilder)
{
    // ensure the destructor has not been called -
    if (theTclBuilder == 0)
    {
        cerr << "WARNING builder has been destroyed\n";
        return TCL_ERROR;
    }

    int ndm = theTclBuilder->getNDM();
    int ndf = theTclBuilder->getNDF();

    int ok = 0;

    if (ndm == 2 && ndf == 3)
    {
        ok = 1;
    }

    if (ndm == 3 && ndf == 6)
    {
        ok = 1;
    }

    if (ok == 0)
    {
        cerr << "WARNING -- NDM = " << ndm << " and NDF = " << ndf
               << " not compatible with dispBeamColumn element" << endln;
        return TCL_ERROR;
    }

    // split possible lists present in argv
    char* List;

    List = Tcl_Merge (inArgc, inArgv);

    if (List == 0)
    {
        cerr << "WARNING - TclModelBuilder_addDispBeamColumn - problem merging list\n";
        return TCL_ERROR;
    }

    //  cerr << "List :" << List << endln;

    // remove braces from list
    for (int i = 0; List[i] != '\0'; i++)
    {
        if ((List[i] == '{')  ||  (List[i] == '}'))
        {
            List[i] = ' ';
        }
    }

    int argc;
    TCL_Char** argv;

    if (Tcl_SplitList(interp, List, &argc, &argv) != TCL_OK)
    {
        cerr <<  "WARNING - TclModelBuilder_addDispBeamColumn - problem spliting list\n";
        return TCL_ERROR;
    }

    Tcl_Free (List);


    if (argc < 6)
    {
        cerr << "WARNING insufficient arguments\n";
        printCommand(argc, argv);
        cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? ...\n";
        return TCL_ERROR;
    }

    int eleTag, iNode, jNode, transfTag;
    CrdTransf2d* theTransf2d = 0;
    CrdTransf3d* theTransf3d = 0;
    Element* theElement = 0;


    if (Tcl_GetInt(interp, argv[2], &eleTag) != TCL_OK)
    {
        cerr << "WARNING invalid dispBeamColumn eleTag" << endln;
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[3], &iNode) != TCL_OK)
    {
        cerr << "WARNING invalid iNode\n";
        cerr << "dispBeamColumn element: " << eleTag << endln;
        return TCL_ERROR;
    }

    if (Tcl_GetInt(interp, argv[4], &jNode) != TCL_OK)
    {
        cerr << "WARNING invalid jNode\n";
        cerr << "dispBeamColumn element: " << eleTag << endln;
        return TCL_ERROR;
    }


    //
    // fmk UNDOCUMENTED FEATURE -
    // all to take similar command to nonlinearBeamColumn & dispBeamColumn
    //

    if ((strcmp(argv[6], "Lobatto") != 0) &&
            (strcmp(argv[6], "Legendre") != 0) &&
            (strcmp(argv[6], "Radau") != 0) &&
            (strcmp(argv[6], "NewtonCotes") != 0) &&
            (strcmp(argv[6], "UserDefined") != 0) &&
            (strcmp(argv[6], "HingeMidpoint") != 0) &&
            (strcmp(argv[6], "HingeEndpoint") != 0) &&
            (strcmp(argv[6], "HingeRadau") != 0) &&
            (strcmp(argv[6], "HingeRadauTwo") != 0) &&
            (strcmp(argv[6], "UserHinge") != 0) &&
            (strcmp(argv[6], "DistHinge") != 0) &&
            (strcmp(argv[6], "Trapezoidal") != 0) &&
            (strcmp(argv[6], "FixedLocation") != 0) &&
            (strcmp(argv[6], "LowOrder") != 0) &&
            (strcmp(argv[6], "MidDistance") != 0))
    {

        int nIP, secTag;

        if (Tcl_GetInt(interp, argv[5], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[6], &secTag) != TCL_OK)
        {
            cerr << "WARNING invalid secTag\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* theSection = theTclBuilder->getSection(secTag);

        if (theSection == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTag;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[7], &transfTag) != TCL_OK)
        {
            cerr << "WARNING invalid transfTag\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        int argi = 8;
        int numIter = 0;
        double tol = 0.0;

        if (argc > argi)
        {
            if (strcmp(argv[argi], "-iter") == 0)
            {
                if (argc < argi + 3)
                {
                    cerr << "WARNING not enough -iter args need -iter numIter? tol?\n";
                    cerr << "dispBeamColumn element: " << eleTag << endln;
                    return TCL_ERROR;
                }

                if (Tcl_GetInt(interp, argv[argi + 1], &numIter) != TCL_OK)
                {
                    cerr << "WARNING invalid numIter\n";
                    cerr << "dispBeamColumn element: " << eleTag << endln;
                    return TCL_ERROR;
                }

                if (Tcl_GetDouble(interp, argv[argi + 2], &tol) != TCL_OK)
                {
                    cerr << "WARNING invalid tol\n";
                    cerr << "dispBeamColumn element: " << eleTag << endln;
                    return TCL_ERROR;
                }
            }
        }


        if (ndm == 2)
        {

            theTransf2d = theTclBuilder->getCrdTransf2d(transfTag);

            if (theTransf2d == 0)
            {
                cerr << "WARNING transformation not found\n";
                cerr << "transformation: " << transfTag;
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }
        }

        if (ndm == 3)
        {

            theTransf3d = theTclBuilder->getCrdTransf3d(transfTag);

            if (theTransf3d == 0)
            {
                cerr << "WARNING transformation not found\n";
                cerr << "transformation: " << transfTag;
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }
        }


        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (int i = 0; i < nIP; i++)
        {
            sections[i] = theSection;
        }

        LegendreBeamIntegration beamIntegr;

        if (ndm == 2)
        {
            if (tol == 0.0)
                theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                                                  beamIntegr, *theTransf2d);
            else
                theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                                                  beamIntegr, *theTransf2d);
        }
        else
        {
            if (tol == 0.0)
                theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                                                  *theTransf3d);
            else
                theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                                                  *theTransf3d);
        }

        delete [] sections;

        if (theElement == 0)
        {
            cerr << "WARNING ran out of memory creating element\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (theTclDomain->addElement(theElement) == false)
        {
            cerr << "WARNING could not add element to the domain\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            delete theElement;
            return TCL_ERROR;
        }

        return TCL_OK;
    }


    //
    // otherwise use correct format of command as found in current documentation
    //

    if (Tcl_GetInt(interp, argv[5], &transfTag) != TCL_OK)
    {
        cerr << "WARNING invalid transfTag\n";
        cerr << "dispBeamColumn element: " << eleTag << endln;
        return TCL_ERROR;
    }

    if (ndm == 2)
    {

        theTransf2d = theTclBuilder->getCrdTransf2d(transfTag);

        if (theTransf2d == 0)
        {
            cerr << "WARNING transformation not found\n";
            cerr << "transformation: " << transfTag;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }
    }

    if (ndm == 3)
    {

        theTransf3d = theTclBuilder->getCrdTransf3d(transfTag);

        if (theTransf3d == 0)
        {
            cerr << "WARNING transformation not found\n";
            cerr << "transformation: " << transfTag;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[6], "Lobatto") == 0 || strcmp(argv[6], "Legendre") == 0
            || strcmp(argv[6], "Radau") == 0 || strcmp(argv[6], "NewtonCotes") == 0
            || strcmp(argv[6], "Trapezoidal") == 0)
    {
        int secTag, nIP;

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? Lobatto secTag? nIP?\n";
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[7], &secTag) != TCL_OK)
        {
            cerr << "WARNING invalid secTag\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[8], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* theSection = theTclBuilder->getSection(secTag);

        if (theSection == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTag;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (int i = 0; i < nIP; i++)
        {
            sections[i] = theSection;
        }

        BeamIntegration* beamIntegr = 0;

        if (strcmp(argv[6], "Lobatto") == 0)
        {
            beamIntegr = new LobattoBeamIntegration();
        }
        else if (strcmp(argv[6], "Legendre") == 0)
        {
            beamIntegr = new LegendreBeamIntegration();
        }
        else if (strcmp(argv[6], "Radau") == 0)
        {
            beamIntegr = new RadauBeamIntegration();
        }
        else if (strcmp(argv[6], "NewtonCotes") == 0)
        {
            beamIntegr = new NewtonCotesBeamIntegration();
        }
        //else if (strcmp(argv[6],"Trapezoidal") == 0)
        //  beamIntegr = new TrapezoidalBeamIntegration();
        else
        {
            cerr << "ERROR: invalid integration type: " << argv[6] << endln;
            return TCL_ERROR;
        }

        if (ndm == 2)
            theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                                              *beamIntegr, *theTransf2d);
        else
            theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                                              *theTransf3d);

        delete beamIntegr;
        delete [] sections;
    }

    else if (strcmp(argv[6], "UserDefined") == 0)
    {

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? UserDefined nIP? secTag1? ... pt1? ... wt1? ...\n";
            return TCL_ERROR;
        }

        int nIP;

        if (Tcl_GetInt(interp, argv[7], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        ID secs(nIP);
        Vector pts(nIP);
        Vector wts(nIP);

        int i, j;

        for (i = 0, j = 8; i < nIP; i++, j++)
        {
            int sec;
            double pt, wt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + nIP], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + 2 * nIP], &wt) != TCL_OK)
            {
                cerr << "WARNING invalid wt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i) = sec;
            pts(i)  = pt;
            wts(i)  = wt;
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (i = 0; i < nIP; i++)
        {
            SectionForceDeformation* theSection = theTclBuilder->getSection(secs(i));

            if (theSection == 0)
            {
                cerr << "WARNING section not found\n";
                cerr << "Section: " << secs(i);
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            sections[i] = theSection;
        }

        UserDefinedBeamIntegration beamIntegr(nIP, pts, wts);

        if (ndm == 2)
            theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                                              beamIntegr, *theTransf2d);
        else
            theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                                              *theTransf3d);

        delete [] sections;
    }

    else if (strcmp(argv[6], "HingeMidpoint") == 0 ||
             strcmp(argv[6], "HingeRadau") == 0 ||
             strcmp(argv[6], "HingeRadauTwo") == 0 ||
             strcmp(argv[6], "HingeEndpoint") == 0)
    {

        if (argc < 12)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? type secTagI? lpI? secTagJ? lpJ? secTagE?\n";
            return TCL_ERROR;
        }

        int secTagI, secTagJ, secTagE;
        double lpI, lpJ;

        if (Tcl_GetInt(interp, argv[7], &secTagI) != TCL_OK)
        {
            cerr << "WARNING invalid secTagI\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[8], &lpI) != TCL_OK)
        {
            cerr << "WARNING invalid lpI\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[9], &secTagJ) != TCL_OK)
        {
            cerr << "WARNING invalid secTagJ\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[10], &lpJ) != TCL_OK)
        {
            cerr << "WARNING invalid lpJ\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[11], &secTagE) != TCL_OK)
        {
            cerr << "WARNING invalid secTagE\n";
            cerr << "forceBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* sectionI = theTclBuilder->getSection(secTagI);

        if (sectionI == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTagI;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* sectionJ = theTclBuilder->getSection(secTagJ);

        if (sectionJ == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTagJ;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* sectionE = theTclBuilder->getSection(secTagE);

        if (sectionJ == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTagE;
            cerr << "\nforceBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        BeamIntegration* beamIntegr = 0;

        SectionForceDeformation* sections[6];

        int Np;

        if (strcmp(argv[6], "HingeMidpoint") == 0)
        {
            beamIntegr = new HingeMidpointBeamIntegration(lpI, lpJ);
            Np = 4;
            sections[0] = sectionI;
            sections[1] = sectionE;
            sections[2] = sectionE;
            sections[3] = sectionJ;
        }
        else if (strcmp(argv[6], "HingeRadau") == 0)
        {
            beamIntegr = new HingeRadauBeamIntegration(lpI, lpJ);
            Np = 6;
            sections[0] = sectionI;
            sections[1] = sectionE;
            sections[2] = sectionE;
            sections[3] = sectionE;
            sections[4] = sectionE;
            sections[5] = sectionJ;
        }
        else if (strcmp(argv[6], "HingeRadauTwo") == 0)
        {
            beamIntegr = new HingeRadauTwoBeamIntegration(lpI, lpJ);
            Np = 6;
            sections[0] = sectionI;
            sections[1] = sectionI;
            sections[2] = sectionE;
            sections[3] = sectionE;
            sections[4] = sectionJ;
            sections[5] = sectionJ;
        }
        else
        {
            beamIntegr = new HingeEndpointBeamIntegration(lpI, lpJ);
            Np = 4;
            sections[0] = sectionI;
            sections[1] = sectionE;
            sections[2] = sectionE;
            sections[3] = sectionJ;
        }

        if (ndm == 2)
            theElement = new DispBeamColumn2d(eleTag, iNode, jNode, Np, sections,
                                              *beamIntegr, *theTransf2d);
        else
            theElement = new DispBeamColumn3d(eleTag, iNode, jNode, Np, sections,
                                              *theTransf3d);

        delete beamIntegr;
    }

    else if (strcmp(argv[6], "UserHinge") == 0)
    {

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? UserHinge E? A? Iz? <Iy? G? J?> npL? secTagL1? ... ptL1? ... wtL1? ... npR? secTagR1? ... ptR1? ... wtR1? ...\n";
            return TCL_ERROR;
        }

        double E, A, Iz, Iy, G, J;

        if (Tcl_GetDouble(interp, argv[7], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[8], &A) != TCL_OK)
        {
            cerr << "WARNING invalid A\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[9], &Iz) != TCL_OK)
        {
            cerr << "WARNING invalid I\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        int argStart = 10;

        if (ndm == 3)
        {
            if (Tcl_GetDouble(interp, argv[10], &Iy) != TCL_OK)
            {
                cerr << "WARNING invalid I\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[11], &G) != TCL_OK)
            {
                cerr << "WARNING invalid G\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[12], &J) != TCL_OK)
            {
                cerr << "WARNING invalid J\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            argStart = 13;
        }

        int npL, npR;

        if (Tcl_GetInt(interp, argv[argStart], &npL) != TCL_OK)
        {
            cerr << "WARNING invalid npL\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[argStart + 3 * npL + 1], &npR) != TCL_OK)
        {
            cerr << "WARNING invalid npR\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        int nIP = npL + npR;

        ID secs(nIP);
        Vector ptsL(npL);
        Vector wtsL(npL);
        Vector ptsR(npR);
        Vector wtsR(npR);

        int i, j;

        for (i = 0, j = argStart + 1; i < npL; i++, j++)
        {
            int sec;
            double pt, wt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + npL], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + 2 * npL], &wt) != TCL_OK)
            {
                cerr << "WARNING invalid wt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i) = sec;
            ptsL(i) = pt;
            wtsL(i) = wt;
        }

        for (i = 0, j = 1 + (argStart + 1) + 3 * npL; i < npR; i++, j++)
        {
            int sec;
            double pt, wt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + npR], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + 2 * npR], &wt) != TCL_OK)
            {
                cerr << "WARNING invalid wt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i + npL) = sec;
            ptsR(i)     = pt;
            wtsR(i)     = wt;
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP + 2];

        for (i = 0; i < nIP; i++)
        {
            SectionForceDeformation* theSection = theTclBuilder->getSection(secs(i));

            if (theSection == 0)
            {
                cerr << "WARNING section not found\n";
                cerr << "Section: " << secs(i);
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            sections[i] = theSection;
        }

        UserDefinedHingeIntegration beamIntegr(npL, ptsL, wtsL,
                                               npR, ptsR, wtsR);

        ElasticSection2d section2d(0, E, A, Iz);
        ElasticSection3d section3d(0, E, A, Iz, Iy, G, J);

        if (ndm == 2)
        {
            sections[nIP]   = &section2d;
            sections[nIP + 1] = &section2d;
            theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP + 2, sections,
                                              beamIntegr, *theTransf2d);
        }
        else
        {
            sections[nIP]   = &section3d;
            sections[nIP + 1] = &section3d;
            theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP + 2, sections,
                                              *theTransf3d);
        }

        delete [] sections;
    }

    else if (strcmp(argv[6], "DistHinge") == 0)
    {

        if (argc < 16)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? type type nIP? secTagI? lpI? secTagJ? lpJ? E? A? Iz? <Iy? G? J?>\n";
            return TCL_ERROR;
        }

        int secTagI, secTagJ;
        double lpI, lpJ;
        double E, A, Iz, Iy, G, J;
        int nIP;

        BeamIntegration* otherBeamInt = 0;

        if (strcmp(argv[7], "Lobatto") == 0)
        {
            otherBeamInt = new LobattoBeamIntegration();
        }
        else if (strcmp(argv[7], "Legendre") == 0)
        {
            otherBeamInt = new LegendreBeamIntegration();
        }
        else if (strcmp(argv[7], "Radau") == 0)
        {
            otherBeamInt = new RadauBeamIntegration();
        }
        else if (strcmp(argv[7], "NewtonCotes") == 0)
        {
            otherBeamInt = new NewtonCotesBeamIntegration();
        }
        //else if (strcmp(argv[7],"Trapezoidal") == 0)
        //  otherBeamInt = new TrapezoidalBeamIntegration();
        else
        {
            cerr << "ERROR: invalid integration type: " << argv[7] << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[8], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[9], &secTagI) != TCL_OK)
        {
            cerr << "WARNING invalid secTagI\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[10], &lpI) != TCL_OK)
        {
            cerr << "WARNING invalid lpI\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[11], &secTagJ) != TCL_OK)
        {
            cerr << "WARNING invalid secTagJ\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[12], &lpJ) != TCL_OK)
        {
            cerr << "WARNING invalid lpJ\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[13], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[14], &A) != TCL_OK)
        {
            cerr << "WARNING invalid A\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[15], &Iz) != TCL_OK)
        {
            cerr << "WARNING invalid I\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        if (ndm == 3 && argc > 18)
        {
            if (Tcl_GetDouble(interp, argv[16], &Iy) != TCL_OK)
            {
                cerr << "WARNING invalid Iy\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[17], &G) != TCL_OK)
            {
                cerr << "WARNING invalid G\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[18], &J) != TCL_OK)
            {
                cerr << "WARNING invalid J\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }
        }

        SectionForceDeformation* sectionI = theTclBuilder->getSection(secTagI);

        if (sectionI == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTagI;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        SectionForceDeformation* sectionJ = theTclBuilder->getSection(secTagJ);

        if (sectionJ == 0)
        {
            cerr << "WARNING section not found\n";
            cerr << "Section: " << secTagJ;
            cerr << "\ndispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        int numSections = 2 * nIP;
        SectionForceDeformation** sections = new SectionForceDeformation *[numSections];

        for (int i = 0; i < nIP; i++)
        {
            sections[i] = sectionI;
            sections[numSections - 1 - i] = sectionJ;
        }

        DistHingeIntegration beamIntegr(lpI, lpJ, *otherBeamInt);

        ElasticSection2d section2d(0, E, A, Iz);
        ElasticSection3d section3d(0, E, A, Iz, Iy, G, J);

        if (ndm == 2)
        {
            sections[numSections]   = &section2d;
            sections[numSections + 1] = &section2d;
            theElement = new DispBeamColumn2d(eleTag, iNode, jNode, numSections + 2,
                                              sections, beamIntegr, *theTransf2d);
        }
        else
        {
            sections[numSections]   = &section3d;
            sections[numSections + 1] = &section3d;
            theElement = new DispBeamColumn3d(eleTag, iNode, jNode, numSections + 2,
                                              sections, *theTransf3d);
        }

        if (otherBeamInt != 0)
        {
            delete otherBeamInt;
        }

        delete [] sections;
    }

    else if (strcmp(argv[6], "FixedLocation") == 0)
    {

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? FixedLocation nIP? secTag1? ... pt1? ... \n";
            return TCL_ERROR;
        }

        int nIP;

        if (Tcl_GetInt(interp, argv[7], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        ID secs(nIP);
        Vector pts(nIP);
        Vector wts(nIP);

        int i, j;

        for (i = 0, j = 8; i < nIP; i++, j++)
        {
            int sec;
            double pt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + nIP], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i) = sec;
            pts(i)  = pt;
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (i = 0; i < nIP; i++)
        {
            SectionForceDeformation* theSection = theTclBuilder->getSection(secs(i));

            if (theSection == 0)
            {
                cerr << "WARNING section not found\n";
                cerr << "Section: " << secs(i);
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            sections[i] = theSection;
        }

        /*
        FixedLocationBeamIntegration beamIntegr(nIP, pts);

        if (ndm == 2)
          theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                         beamIntegr, *theTransf2d);
        else
          theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                        *theTransf3d);
                        */

        delete [] sections;
    }

    else if (strcmp(argv[6], "LowOrder") == 0)
    {

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? LowOrder nIP? secTag1? ... pt1? ... wt1? ...\n";
            return TCL_ERROR;
        }

        int nIP;

        if (Tcl_GetInt(interp, argv[7], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        ID secs(nIP);
        Vector pts(nIP);
        Vector wts(nIP);

        int i, j;
        int nc = 0;

        for (i = 0, j = 8; i < nIP; i++, j++)
        {
            int sec;
            double pt, wt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i) = sec;

            if (Tcl_GetDouble(interp, argv[j + nIP], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            pts(i)  = pt;

            if (j + 2 * nIP < argc)
            {
                if (Tcl_GetDouble(interp, argv[j + 2 * nIP], &wt) != TCL_OK)
                {
                    cerr << "WARNING invalid wt\n";
                    cerr << "dispBeamColumn element: " << eleTag << endln;
                    return TCL_ERROR;
                }
                else
                {
                    wts(i)  = wt;
                    nc++;
                }
            }
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (i = 0; i < nIP; i++)
        {
            SectionForceDeformation* theSection = theTclBuilder->getSection(secs(i));

            if (theSection == 0)
            {
                cerr << "WARNING section not found\n";
                cerr << "Section: " << secs(i);
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            sections[i] = theSection;
        }

        /*
        LowOrderBeamIntegration beamIntegr(nIP, pts, nc, wts);

        if (ndm == 2)
          theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                         beamIntegr, *theTransf2d);
        else
          theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                        *theTransf3d);
                        */

        delete [] sections;
    }

    else if (strcmp(argv[6], "MidDistance") == 0)
    {

        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: element dispBeamColumn eleTag? iNode? jNode? transfTag? MidDistance nIP? secTag1? ... pt1? ... \n";
            return TCL_ERROR;
        }

        int nIP;

        if (Tcl_GetInt(interp, argv[7], &nIP) != TCL_OK)
        {
            cerr << "WARNING invalid nIP\n";
            cerr << "dispBeamColumn element: " << eleTag << endln;
            return TCL_ERROR;
        }

        ID secs(nIP);
        Vector pts(nIP);
        Vector wts(nIP);

        int i, j;

        for (i = 0, j = 8; i < nIP; i++, j++)
        {
            int sec;
            double pt;

            if (Tcl_GetInt(interp, argv[j], &sec) != TCL_OK)
            {
                cerr << "WARNING invalid sec\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[j + nIP], &pt) != TCL_OK)
            {
                cerr << "WARNING invalid pt\n";
                cerr << "dispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            secs(i) = sec;
            pts(i)  = pt;
        }

        SectionForceDeformation** sections = new SectionForceDeformation *[nIP];

        for (i = 0; i < nIP; i++)
        {
            SectionForceDeformation* theSection = theTclBuilder->getSection(secs(i));

            if (theSection == 0)
            {
                cerr << "WARNING section not found\n";
                cerr << "Section: " << secs(i);
                cerr << "\ndispBeamColumn element: " << eleTag << endln;
                return TCL_ERROR;
            }

            sections[i] = theSection;
        }

        /*
        MidDistanceBeamIntegration beamIntegr(nIP, pts);

        if (ndm == 2)
          theElement = new DispBeamColumn2d(eleTag, iNode, jNode, nIP, sections,
                         beamIntegr, *theTransf2d);
        else
          theElement = new DispBeamColumn3d(eleTag, iNode, jNode, nIP, sections,
                        *theTransf3d);
                        */

        delete [] sections;
    }

    else
    {
        cerr << "Unknown integration type: " << argv[6] << endln;
        cerr << "dispBeamColumn element: " << eleTag << endln;
        return TCL_ERROR;
    }

    if (theElement == 0)
    {
        cerr << "WARNING ran out of memory creating element\n";
        cerr << "dispBeamColumn element: " << eleTag << endln;
        return TCL_ERROR;
    }

    if (theTclDomain->addElement(theElement) == false)
    {
        cerr << "WARNING could not add element to the domain\n";
        cerr << "dispBeamColumn element: " << eleTag << endln;
        delete theElement;
        return TCL_ERROR;
    }

    // if get here we have sucessfully created the element and added it to the domain
    return TCL_OK;
}