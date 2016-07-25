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

// $Revision: 1.36 $
// $Date: 2006/03/03 18:53:31 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/TclModelBuilderUniaxialMaterialCommand.cpp,v $


// Written: fmk, MHS
// Created: 07/99
//
// Description: This file contains the function invoked when the user invokes
// the uniaxialMaterial command in the interpreter.
//
// What: "@(#) TclModelBuilderUniaxialMaterialCommand.C, revA"

#include <TclModelBuilder.h>

#include <ElasticMaterial.h>    // fmk
#include <ElasticPPMaterial.h>  // fmk
#include <ParallelMaterial.h>   // fmk
#include <HardeningMaterial.h>  // MHS
#include <Steel01.h>            // MHS
#include <Steel02.h>                    // FF 
#include <Steel03.h>            // KM
#include <Concrete01.h>         // MHS
#include <Concrete02.h>         // MHS
#include <Concrete04.h>
#include <HystereticMaterial.h> // MHS
#include <EPPGapMaterial.h>     // Mackie
#include <ViscousMaterial.h>    // Sasani
#include <PathIndependentMaterial.h>    // MHS
#include <MinMaxMaterial.h> // MHS
#include <FatigueMaterial.h>    // Patxi
#include <SeriesMaterial.h>     // MHS
#include <ENTMaterial.h>        // MHS
#include <CableMaterial.h>  // CC
#include <BoucWenMaterial.h>    // Terje
#include <Pinching4Material.h>   // NM
#include <BarSlipMaterial.h>     // NM
#include <iostream>
using namespace std;

#include <Domain.h>             // RWB bringing in Domain for PyLiq
// Boris Jeremic took out 20Mar2008
//#include <PySimple1.h>            // RWB
//#include <PyLiq1.h>               // RWB
//#include <TzSimple1.h>            // RWB
//#include <TzLiq1.h>               // RWB

#include <Vector.h>
#include <string.h>


#ifdef _LIMITSTATEMATERIAL
extern UniaxialMaterial*
Tcl_AddLimitStateMaterial(ClientData clientData, Tcl_Interp* interp, int argc, TCL_Char** argv);
#endif


//#include <packages.h>



typedef struct uniaxialPackageCommand
{
    char* funcName;
    int (*funcPtr)(ClientData clientData, Tcl_Interp* interp,  int argc,
                   TCL_Char** argv, TclModelBuilder*);
    struct uniaxialPackageCommand* next;
} UniaxialPackageCommand;

static UniaxialPackageCommand* theUniaxialPackageCommands = NULL;

static void printCommand(int argc, TCL_Char** argv)
{
    cerr << "Input command: ";

    for (int i = 0; i < argc; i++)
    {
        cerr << argv[i] << " ";
    }

    cerr << endln;
}


// external functions

int
TclCommand_ReinforcingSteel(ClientData clientData, Tcl_Interp* interp, int argc,
                            TCL_Char** argv, TclModelBuilder* theTclBuilder);


UniaxialMaterial*
TclModelBuilder_addFedeasMaterial(ClientData clientData, Tcl_Interp* interp, int argc,
                                  TCL_Char** argv, TclModelBuilder* theTclBuilder);

UniaxialMaterial*
TclModelBuilder_addDrainMaterial(ClientData clientData, Tcl_Interp* interp, int argc,
                                 TCL_Char** argv, TclModelBuilder* theTclBuilder);

UniaxialMaterial*
TclModelBuilder_addSnapMaterial(ClientData clientData, Tcl_Interp* interp, int argc,
                                TCL_Char** argv, TclModelBuilder* theTclBuilder);

UniaxialMaterial*
TclModelBuilder_addPyTzQzMaterial(ClientData clientData, Tcl_Interp* interp, int argc,
                                  TCL_Char** argv, TclModelBuilder* theTclBuilder, Domain* theDomain);
int
TclCommand_KinematicHardening(ClientData clientData, Tcl_Interp* interp, int argc,
                              TCL_Char** argv, TclModelBuilder* theTclBuilder);

int
TclModelBuilderUniaxialMaterialCommand (ClientData clientData, Tcl_Interp* interp, int argc,
                                        TCL_Char** argv, TclModelBuilder* theTclBuilder, Domain* theDomain)
{
    // Make sure there is a minimum number of arguments
    if (argc < 3)
    {
        cerr << "WARNING insufficient number of uniaxial material arguments\n";
        cerr << "Want: uniaxialMaterial type? tag? <specific material args>" << endln;
        return TCL_ERROR;
    }

    // Pointer to a uniaxial material that will be added to the model builder
    UniaxialMaterial* theMaterial = 0;

    // Check argv[2] for uniaxial material type
    if (strcmp(argv[1], "Elastic") == 0)
    {
        if (argc < 4 || argc > 5)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Elastic tag? E? <eta?>" << endln;
            return TCL_ERROR;
        }

        int tag;
        double E;
        double eta = 0.0;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Elastic tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxiaMaterial Elastic: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 5)
        {
            if (Tcl_GetDouble(interp, argv[4], &eta) != TCL_OK)
            {
                cerr << "WARNING invalid eta\n";
                cerr << "uniaxialMaterial Elastic: " << tag << endln;
                return TCL_ERROR;
            }
        }

        // Parsing was successful, allocate the material
        theMaterial = new ElasticMaterial(tag, E, eta);
        //dum << tag " << tag << " E " << E << " eta " << eta <<endln;

    }
    else if (strcmp(argv[1], "ReinforcingSteel") == 0)
    {
        return TclCommand_ReinforcingSteel(clientData, interp, argc, argv, theTclBuilder);
    }

    else if (strcmp(argv[1], "ENT") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial ENT tag? E?" << endln;
            return TCL_ERROR;
        }

        int tag;
        double E;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial ENT tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxiaMaterial ENT: " << tag << endln;
            return TCL_ERROR;
        }

        // Parsing was successful, allocate the material
        theMaterial = new ENTMaterial(tag, E);

    }

    else if (strcmp(argv[1], "ElasticPP") == 0)
    {
        if (argc < 5)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial ElasticPP tag? E? epsy?" << endln;
            return TCL_ERROR;
        }

        int tag;
        double E, ep;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial ElasticPP tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxialMaterial ElasticPP: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &ep) != TCL_OK)
        {
            cerr << "WARNING invalid epsy\n";
            cerr << "uniaxialMaterial ElasticPP: " << tag << endln;
            return TCL_ERROR;
        }

        // read in additional parameters eyn and ezero
        if (argc > 6)
        {
            double eyn, ez;

            if (Tcl_GetDouble(interp, argv[5], &eyn) != TCL_OK)
            {
                cerr << "WARNING invalid eyn\n";
                cerr << "uniaxialMaterial ElasticPP: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[6], &ez) != TCL_OK)
            {
                cerr << "WARNING invalid ez\n";
                cerr << "uniaxialMaterial ElasticPP: " << tag << endln;
                return TCL_ERROR;
            }

            theMaterial = new ElasticPPMaterial(tag, E, ep, eyn, ez);
        }
        else
        {
            // Parsing was successful, allocate the material
            theMaterial = new ElasticPPMaterial(tag, E, ep);
        }
    }

    else if (strcmp(argv[1], "ElasticPPGap") == 0)
    {
        if (argc < 6)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial ElasticPPGap tag? E? fy? gap? <eta?> <damage>" << endln;
            return TCL_ERROR;
        }

        int tag;
        int damage = 0;
        double eta = 0.0;
        double E, fy, gap;


        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial ElasticPPGap tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxialMaterial ElasticPPGap: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &fy) != TCL_OK)
        {
            cerr << "WARNING invalid fy\n";
            cerr << "uniaxialMaterial ElasticPPGap: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &gap) != TCL_OK)
        {
            cerr << "WARNING invalid gap\n";
            cerr << "uniaxialMaterial ElasticPPGap: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc > 6 && strcmp(argv[6], "damage") == 0)
        {
            damage = 1;
        }
        else if (argc > 7 && strcmp(argv[7], "damage") == 0)
        {
            damage = 1;

            if (Tcl_GetDouble(interp, argv[6], &eta) != TCL_OK)
            {
                cerr << "WARNING invalid eta\n";
                cerr << "uniaxialMaterial ElasticPPGap: " << tag << endln;
                return TCL_ERROR;
            }
        }

        // Parsing was successful, allocate the material
        theMaterial = new EPPGapMaterial(tag, E, fy, gap, eta, damage);

    }

    else if (strcmp(argv[1], "Hardening") == 0)
    {
        if (argc < 7)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Hardening tag? E? sigmaY? H_iso? H_kin? <eta?>" << endln;
            return TCL_ERROR;
        }

        int tag;
        double E, sigmaY, Hiso, Hkin;
        double eta = 0.0;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Hardening tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxialMaterial Hardening: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &sigmaY) != TCL_OK)
        {
            cerr << "WARNING invalid sigmaY\n";
            cerr << "uniaxialMaterial Hardening: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &Hiso) != TCL_OK)
        {
            cerr << "WARNING invalid H_iso\n";
            cerr << "uniaxialMaterial Hardening: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &Hkin) != TCL_OK)
        {
            cerr << "WARNING invalid H_kin\n";
            cerr << "uniaxialMaterial Hardening: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc > 7 && Tcl_GetDouble(interp, argv[7], &eta) != TCL_OK)
        {
            cerr << "WARNING invalid eta\n";
            cerr << "uniaxialMaterial Hardening: " << tag << endln;
            return TCL_ERROR;
        }

        // Parsing was successful, allocate the material
        theMaterial = new HardeningMaterial (tag, E, sigmaY, Hiso, Hkin, eta);
    }

    else if (strcmp(argv[1], "BoucWen") == 0)
    {
        if (argc < 12)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial BoucWen tag? alpha? ko? n? gamma?" << endln
                 << " beta? Ao? deltaA? deltaNu? deltaEta?" << endln;
            return TCL_ERROR;
        }

        int tag;
        double alpha, ko, n, gamma, beta, Ao, deltaA, deltaNu, deltaEta;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial BoucWen tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &alpha) != TCL_OK)
        {
            cerr << "WARNING invalid alpha\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &ko) != TCL_OK)
        {
            cerr << "WARNING invalid ko\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &n) != TCL_OK)
        {
            cerr << "WARNING invalid n\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &gamma) != TCL_OK)
        {
            cerr << "WARNING invalid gamma\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[7], &beta) != TCL_OK)
        {
            cerr << "WARNING invalid beta\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[8], &Ao) != TCL_OK)
        {
            cerr << "WARNING invalid Ao\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[9], &deltaA) != TCL_OK)
        {
            cerr << "WARNING invalid deltaA\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[10], &deltaNu) != TCL_OK)
        {
            cerr << "WARNING invalid deltaNu\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[11], &deltaEta) != TCL_OK)
        {
            cerr << "WARNING invalid deltaEta\n";
            cerr << "uniaxialMaterial BoucWen: " << tag << endln;
            return TCL_ERROR;
        }

        // Check if the user has given a tolerance for the Newton scheme
        double tolerance = 1.0e-8;

        if (argc > 12)
        {
            if (Tcl_GetDouble(interp, argv[12], &tolerance) != TCL_OK)
            {
                cerr << "WARNING invalid tolerance\n";
                cerr << "uniaxialMaterial BoucWen: " << tolerance << endln;
                return TCL_ERROR;
            }
        }

        // Check if the user has given a maxNumIter for the Newton scheme
        int maxNumIter = 20;

        if (argc > 13)
        {
            if (Tcl_GetInt(interp, argv[13], &maxNumIter) != TCL_OK)
            {
                cerr << "WARNING invalid maxNumIter\n";
                cerr << "uniaxialMaterial BoucWen: " << maxNumIter << endln;
                return TCL_ERROR;
            }
        }

        // Parsing was successful, allocate the material
        theMaterial = new BoucWenMaterial(tag, alpha, ko, n, gamma, beta,
                                          Ao, deltaA, deltaNu, deltaEta, tolerance, maxNumIter);
    }

    else if (strcmp(argv[1], "Parallel") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Parallel tag? tag1? tag2? ...";
            cerr << " <-min min?> <-max max?>" << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Parallel tag" << endln;
            return TCL_ERROR;
        }

        int numMaterials = argc - 3;

        if (numMaterials == 0)
        {
            cerr << "WARNING no component material(s) provided\n";
            cerr << "uniaxialMaterial Parallel: " << tag << endln;
            return TCL_ERROR;
        }

        // Create an array to hold pointers to component materials
        UniaxialMaterial** theMats = new UniaxialMaterial *[numMaterials];

        // For each material get the tag and ensure it exists in model already
        for (int i = 0; i < numMaterials; i++)
        {
            int tagI;

            if (Tcl_GetInt(interp, argv[i + 3], &tagI) != TCL_OK)
            {
                cerr << "WARNING invalid component tag\n";
                cerr << "uniaxialMaterial Parallel: " << tag << endln;
                return TCL_ERROR;
            }

            UniaxialMaterial* theMat = theTclBuilder->getUniaxialMaterial(tagI);

            if (theMat == 0)
            {
                cerr << "WARNING component material does not exist\n";
                cerr << "Component material: " << argv[i + 3];
                cerr << "\nuniaxialMaterial Parallel: " << tag << endln;
                delete [] theMats;
                return TCL_ERROR;
            }
            else
            {
                theMats[i] = theMat;
            }
        }

        // Parsing was successful, allocate the material
        theMaterial = new ParallelMaterial(tag, numMaterials, theMats);

        // Deallocate the temporary pointers
        delete [] theMats;
    }

    else if (strcmp(argv[1], "Series") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Series tag? tag1? tag2? ..." << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Series tag" << endln;
            return TCL_ERROR;
        }

        int numMaterials = argc - 3;
        UniaxialMaterial** theMats = new UniaxialMaterial *[numMaterials];

        // For each material get the tag and ensure it exists in model already
        for (int i = 3; i < argc; i++)
        {
            int tagI;

            if (Tcl_GetInt(interp, argv[i], &tagI) != TCL_OK)
            {
                cerr << "WARNING invalid component tag\n";
                cerr << "uniaxialMaterial Series: " << tag << endln;
                delete [] theMats;
                return TCL_ERROR;
            }

            UniaxialMaterial* theMat = theTclBuilder->getUniaxialMaterial(tagI);

            if (theMat == 0)
            {
                cerr << "WARNING component material does not exist\n";
                cerr << "Component material: " << tagI;
                cerr << "\nuniaxialMaterial Series: " << tag << endln;
                delete [] theMats;
                return TCL_ERROR;
            }
            else
            {
                theMats[i - 3] = theMat;
            }
        }

        // Parsing was successful, allocate the material
        theMaterial = new SeriesMaterial(tag, numMaterials, theMats);

        // Deallocate the temporary pointers
        delete [] theMats;
    }

    else if (strcmp(argv[1], "Steel01") == 0)
    {
        // Check that there is the minimum number of arguments
        if (argc < 6)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Steel01 tag? fy? E0? b?";
            cerr << " <a1? a2? a3? a4?>" << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Steel01 tag" << endln;
            return TCL_ERROR;
        }

        // Read required Steel01 material parameters
        double fy, E, b;

        if (Tcl_GetDouble(interp, argv[3], &fy) != TCL_OK)
        {
            cerr << "WARNING invalid fy\n";
            cerr << "uniaxialMaterial Steel01: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E0\n";
            cerr << "uniaxialMaterial Steel01: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &b) != TCL_OK)
        {
            cerr << "WARNING invalid b\n";
            cerr << "uniaxialMaterial Steel01: " << tag << endln;
            return TCL_ERROR;
        }

        // Read optional Steel01 material parameters
        double a1, a2, a3, a4;

        if (argc > 6)
        {
            if (argc < 10)
            {
                cerr << "WARNING insufficient number of hardening parameters\n";
                cerr << "uniaxialMaterial Steel01: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[6], &a1) != TCL_OK)
            {
                cerr << "WARNING invalid a1\n";
                cerr << "uniaxialMaterial Steel01: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[7], &a2) != TCL_OK)
            {
                cerr << "WARNING invalid a2\n";
                cerr << "uniaxialMaterial Steel01: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[8], &a3) != TCL_OK)
            {
                cerr << "WARNING invalid a3\n";
                cerr << "uniaxialMaterial Steel01: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[9], &a4) != TCL_OK)
            {
                cerr << "WARNING invalid a4\n";
                cerr << "uniaxialMaterial Steel01: " << tag << endln;
                return TCL_ERROR;
            }

            // Parsing was successful, allocate the material
            theMaterial = new Steel01 (tag, fy, E, b, a1, a2, a3, a4);
        }
        else
            // Parsing was successful, allocate the material
        {
            theMaterial = new Steel01 (tag, fy, E, b);
        }
    }

    else if (strcmp(argv[1], "Steel2") == 0 || strcmp(argv[1], "Steel02") == 0)
    {
        if (argc < 6)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Steel02 tag? fy? E? b? <R0? cR1? cR2? <a1? a2? a3? a4?>>" << endln;
            return 0;
        }

        double fy, E, b;
        double R0, cR1, cR2;
        double a1, a2, a3, a4;

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid tag\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[3], &fy) != TCL_OK)
        {
            cerr << "WARNING invalid fy\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[4], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[5], &b) != TCL_OK)
        {
            cerr << "WARNING invalid b\n";
            printCommand(argc, argv);
            return 0;
        }

        if (argc > 8)
        {
            if (Tcl_GetDouble(interp, argv[6], &R0) != TCL_OK)
            {
                cerr << "WARNING invalid R0\n";
                printCommand(argc, argv);
                return 0;
            }

            if (Tcl_GetDouble(interp, argv[7], &cR1) != TCL_OK)
            {
                cerr << "WARNING invalid cR1\n";
                printCommand(argc, argv);
                return 0;
            }

            if (Tcl_GetDouble(interp, argv[8], &cR2) != TCL_OK)
            {
                cerr << "WARNING invalid cR2\n";
                printCommand(argc, argv);
                return 0;
            }

            if (argc > 12)
            {
                if (Tcl_GetDouble(interp, argv[9], &a1) != TCL_OK)
                {
                    cerr << "WARNING invalid a1\n";
                    printCommand(argc, argv);
                    return 0;
                }

                if (Tcl_GetDouble(interp, argv[10], &a2) != TCL_OK)
                {
                    cerr << "WARNING invalid a2\n";
                    printCommand(argc, argv);
                    return 0;
                }

                if (Tcl_GetDouble(interp, argv[11], &a3) != TCL_OK)
                {
                    cerr << "WARNING invalid a3\n";
                    printCommand(argc, argv);
                    return 0;
                }

                if (Tcl_GetDouble(interp, argv[12], &a4) != TCL_OK)
                {
                    cerr << "WARNING invalid a4\n";
                    printCommand(argc, argv);
                    return 0;
                }

                theMaterial = new Steel02(tag, fy, E, b, R0, cR1, cR2, a1, a2, a3, a4);
            }
            else
            {
                theMaterial = new Steel02(tag, fy, E, b, R0, cR1, cR2);
            }
        }
        else
        {
            theMaterial = new Steel02(tag, fy, E, b);
        }

    }


    else if (strcmp(argv[1], "Steel03") == 0)
    {
        // Check that there is the minimum number of arguments
        if (argc < 9)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Steel03 tag? fy? E0? b? r? cR1 cR2?";
            cerr << " <a1? a2? a3? a4?>" << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Steel03 tag" << endln;
            return TCL_ERROR;
        }


        // Read required Steel01 material parameters
        double fy, E, b, r, cR1, cR2;

        if (Tcl_GetDouble(interp, argv[3], &fy) != TCL_OK)
        {
            cerr << "WARNING invalid fy\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }


        if (Tcl_GetDouble(interp, argv[4], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E0\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &b) != TCL_OK)
        {
            cerr << "WARNING invalid b\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &r) != TCL_OK)
        {
            cerr << "WARNING invalid r\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }


        if (Tcl_GetDouble(interp, argv[7], &cR1) != TCL_OK)
        {
            cerr << "WARNING invalid cR1\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }


        if (Tcl_GetDouble(interp, argv[8], &cR2) != TCL_OK)
        {
            cerr << "WARNING invalid cR2\n";
            cerr << "uniaxialMaterial Steel03: " << tag << endln;
            return TCL_ERROR;
        }

        // Read optional Steel01 material parameters
        double a1, a2, a3, a4;

        if (argc > 9)
        {
            if (argc < 13)
            {
                cerr << "WARNING insufficient number of hardening parameters\n";
                cerr << "uniaxialMaterial Steel03: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[9], &a1) != TCL_OK)
            {
                cerr << "WARNING invalid a1\n";
                cerr << "uniaxialMaterial Steel03: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[10], &a2) != TCL_OK)
            {
                cerr << "WARNING invalid a2\n";
                cerr << "uniaxialMaterial Steel03: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[11], &a3) != TCL_OK)
            {
                cerr << "WARNING invalid a3\n";
                cerr << "uniaxialMaterial Steel03: " << tag << endln;
                return TCL_ERROR;
            }


            if (Tcl_GetDouble(interp, argv[12], &a4) != TCL_OK)
            {
                cerr << "WARNING invalid a4\n";
                cerr << "uniaxialMaterial Steel03: " << tag << endln;
                return TCL_ERROR;
            }


            // Parsing was successful, allocate the material
            theMaterial = new Steel03 (tag, fy, E, b, r, cR1, cR2, a1, a2, a3, a4);
        }
        else
            // Parsing was successful, allocate the material
        {
            theMaterial = new Steel03 (tag, fy, E, b, r, cR1, cR2);
        }

    }



    else if (strcmp(argv[1], "Concrete01") == 0)
    {
        if (argc < 7)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Concrete01 tag? fpc? epsc0? fpcu? epscu?" << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Concrete01 tag" << endln;
            return TCL_ERROR;
        }

        // Read required Concrete01 material parameters
        double fpc, epsc0, fpcu, epscu;

        if (Tcl_GetDouble(interp, argv[3], &fpc) != TCL_OK)
        {
            cerr << "WARNING invalid fpc\n";
            cerr << "Concrete01 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &epsc0) != TCL_OK)
        {
            cerr << "WARNING invalid epsc0\n";
            cerr << "Concrete01 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &fpcu) != TCL_OK)
        {
            cerr << "WARNING invalid fpcu\n";
            cerr << "Concrete01 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &epscu) != TCL_OK)
        {
            cerr << "WARNING invalid epscu\n";
            cerr << "Concrete01 material: " << tag << endln;
            return TCL_ERROR;
        }

        // Parsing was successful, allocate the material
        theMaterial = new Concrete01(tag, fpc, epsc0, fpcu, epscu);
    }

    else if (strcmp(argv[1], "Concrete2") == 0 || strcmp(argv[1], "Concrete02") == 0)
    {
        if (argc < 10)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Concrete02 tag? fpc? epsc0? fpcu? epscu? rat? ft? Ets?" << endln;
            return 0;
        }

        double fpc, epsc0, fpcu, epscu;
        double rat, ft, Ets;
        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Concrete02 tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &fpc) != TCL_OK)
        {
            cerr << "WARNING invalid fpc\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[4], &epsc0) != TCL_OK)
        {
            cerr << "WARNING invalid epsc0\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[5], &fpcu) != TCL_OK)
        {
            cerr << "WARNING invalid fpcu\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[6], &epscu) != TCL_OK)
        {
            cerr << "WARNING invalid epscu\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[7], &rat) != TCL_OK)
        {
            cerr << "WARNING invalid rat\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[8], &ft) != TCL_OK)
        {
            cerr << "WARNING invalid ft\n";
            printCommand(argc, argv);
            return 0;
        }

        if (Tcl_GetDouble(interp, argv[9], &Ets) != TCL_OK)
        {
            cerr << "WARNING invalid Ets\n";
            printCommand(argc, argv);
            return 0;
        }

        theMaterial = new Concrete02(tag, fpc, epsc0, fpcu, epscu, rat, ft, Ets);
    }



    else if (strcmp(argv[1], "Hysteretic") == 0)
    {
        if (argc != 20 && argc != 19 && argc != 16 && argc != 15)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Hysteretic tag? mom1p? rot1p? mom2p? rot2p? <mom3p? rot3p?> "
                 << "\nmom1n? rot1n? mom2n? rot2n? <mom3n? rot3n?> pinchX? pinchY? damfc1? damfc2? <beta?>";
            return TCL_ERROR;
        }

        int tag;
        double mom1p, mom2p, mom3p;
        double rot1p, rot2p, rot3p;
        double mom1n, mom2n, mom3n;
        double rot1n, rot2n, rot3n;
        double pinchX, pinchY;
        double damfc1, damfc2;
        double beta = 0.0;

        int i = 2;

        if (Tcl_GetInt(interp, argv[i++], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Hysteretic tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &mom1p) != TCL_OK)
        {
            cerr << "WARNING invalid mom1p\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &rot1p) != TCL_OK)
        {
            cerr << "WARNING invalid rot1p\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &mom2p) != TCL_OK)
        {
            cerr << "WARNING invalid mom2p\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &rot2p) != TCL_OK)
        {
            cerr << "WARNING invalid rot2p\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc > 16)
        {
            if (Tcl_GetDouble(interp, argv[i++], &mom3p) != TCL_OK)
            {
                cerr << "WARNING invalid mom3p\n";
                cerr << "Hysteretic material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &rot3p) != TCL_OK)
            {
                cerr << "WARNING invalid rot3p\n";
                cerr << "Hysteretic material: " << tag << endln;
                return TCL_ERROR;
            }
        }

        if (Tcl_GetDouble(interp, argv[i++], &mom1n) != TCL_OK)
        {
            cerr << "WARNING invalid mom1n\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &rot1n) != TCL_OK)
        {
            cerr << "WARNING invalid rot1n\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &mom2n) != TCL_OK)
        {
            cerr << "WARNING invalid mom2n\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &rot2n) != TCL_OK)
        {
            cerr << "WARNING invalid rot2n\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc > 16)
        {
            if (Tcl_GetDouble(interp, argv[i++], &mom3n) != TCL_OK)
            {
                cerr << "WARNING invalid mom3n\n";
                cerr << "Hysteretic material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &rot3n) != TCL_OK)
            {
                cerr << "WARNING invalid rot3n\n";
                cerr << "Hysteretic material: " << tag << endln;
                return TCL_ERROR;
            }
        }

        if (Tcl_GetDouble(interp, argv[i++], &pinchX) != TCL_OK)
        {
            cerr << "WARNING invalid pinchX\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &pinchY) != TCL_OK)
        {
            cerr << "WARNING invalid pinchY\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &damfc1) != TCL_OK)
        {
            cerr << "WARNING invalid damfc1\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &damfc2) != TCL_OK)
        {
            cerr << "WARNING invalid damfc2\n";
            cerr << "Hysteretic material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 20 || argc == 16)
        {
            if (Tcl_GetDouble(interp, argv[i++], &beta) != TCL_OK)
            {
                cerr << "WARNING invalid beta\n";
                cerr << "Hysteretic material: " << tag << endln;
                return TCL_ERROR;
            }
        }

        // Parsing was successful, allocate the material

        if (argc > 16)
            theMaterial = new HystereticMaterial (tag,
                                                  mom1p, rot1p, mom2p, rot2p, mom3p, rot3p,
                                                  mom1n, rot1n, mom2n, rot2n, mom3n, rot3n,
                                                  pinchX, pinchY, damfc1, damfc2, beta);

        else
            theMaterial = new HystereticMaterial (tag,
                                                  mom1p, rot1p, mom2p, rot2p,
                                                  mom1n, rot1n, mom2n, rot2n,
                                                  pinchX, pinchY, damfc1, damfc2, beta);
    }

    else if (strcmp(argv[1], "Concrete04") == 0)
    {
        //        cerr << argc << endln;
        if (argc != 10 && argc != 9 && argc != 7)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Concrete04 tag? fpc? epsc0? epscu? Ec0? <ft? etu? <beta?> >" << endln;
            return TCL_ERROR;
        }

        int tag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Concrete04 tag"
                 << endln;
            return TCL_ERROR;
        }

        // Read required Concrete04 material parameters
        double fpc, epsc0, ft, epscu, Ec0, etu, beta;

        if (Tcl_GetDouble(interp, argv[3], &fpc) != TCL_OK)
        {
            cerr << "WARNING invalid fpc\n";
            cerr << "Concrete04 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &epsc0) != TCL_OK)
        {
            cerr << "WARNING invalid epsc0\n";
            cerr << "Concrete04 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &epscu) != TCL_OK)
        {
            cerr << "WARNING invalid epscu\n";
            cerr << "Concrete04 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &Ec0) != TCL_OK)
        {
            cerr << "WARNING invalid Ec0\n";
            cerr << "Concrete04 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 9 || argc == 10)
        {
            if (Tcl_GetDouble(interp, argv[7], &ft) != TCL_OK)
            {
                cerr << "WARNING invalid ft\n";
                cerr << "Concrete04 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[8], &etu) != TCL_OK)
            {
                cerr << "WARNING invalid etu\n";
                cerr << "Concrete04 material: " << tag << endln;
                return TCL_ERROR;
            }
        }

        if (argc == 10)
        {
            if (Tcl_GetDouble(interp, argv[9], &beta) != TCL_OK)
            {
                cerr << "WARNING invalid beta\n";
                cerr << "Concrete04 material: " << tag << endln;
                return TCL_ERROR;
            }
        }


        // Parsing was successful, allocate the material
        if (argc == 10)
        {
            theMaterial = new Concrete04(tag, fpc, epsc0, epscu, Ec0,
                                         ft, etu, beta);
        }
        else if (argc == 9)
        {
            theMaterial = new Concrete04(tag, fpc, epsc0, epscu, Ec0,
                                         ft, etu);
        }
        else if (argc == 7)
        {
            theMaterial = new Concrete04(tag, fpc, epsc0, epscu, Ec0);
        }
    }

    else if (strcmp(argv[1], "Viscous") == 0)
    {
        if (argc < 5)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Viscous tag? C? alpha?" << endln;
            return TCL_ERROR;
        }

        int tag;
        double C, a;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid tag\n";
            cerr << "Viscous material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &C) != TCL_OK)
        {
            cerr << "WARNING invalid C\n";
            cerr << "Viscous material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &a) != TCL_OK)
        {
            cerr << "WARNING invalid alpha\n";
            cerr << "Viscous material: " << tag << endln;
            return TCL_ERROR;
        }

        theMaterial = new ViscousMaterial (tag, C, a);
    }

    else if (strcmp(argv[1], "PathIndependent") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial PathIndependent tag? matTag?" << endln;
            return TCL_ERROR;
        }

        int tag, matTag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid tag\n";
            cerr << "PathIndependent material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[3], &matTag) != TCL_OK)
        {
            cerr << "WARNING invalid matTag\n";
            cerr << "PathIndependent material: " << tag << endln;
            return TCL_ERROR;
        }

        UniaxialMaterial* material = theTclBuilder->getUniaxialMaterial(matTag);

        if (material == 0)
        {
            cerr << "WARNING material does not exist\n";
            cerr << "material: " << matTag;
            cerr << "\nuniaxialMaterial PathIndependent: " << tag << endln;
            return TCL_ERROR;
        }

        theMaterial = new PathIndependentMaterial (tag, *material);
    }

    else if (strcmp(argv[1], "MinMax") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial MinMax tag? matTag?";
            cerr << " <-min min?> <-max max?>" << endln;
            return TCL_ERROR;
        }

        int tag, matTag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial MinMax tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[3], &matTag) != TCL_OK)
        {
            cerr << "WARNING invalid component tag\n";
            cerr << "uniaxialMaterial MinMax: " << tag << endln;
            return TCL_ERROR;
        }

        // Search for min and max strains
        double epsmin = NEG_INF_STRAIN;
        double epsmax = POS_INF_STRAIN;

        for (int j = 4; j < argc; j++)
        {
            if (strcmp(argv[j], "-min") == 0)
            {
                if ((j + 1) >= argc || Tcl_GetDouble (interp, argv[j + 1], &epsmin) != TCL_OK)
                {
                    cerr << "WARNING invalid min\n";
                    cerr << "uniaxialMaterial MinMax: " << tag << endln;
                    return TCL_ERROR;
                }

                j++;
            }

            if (strcmp(argv[j], "-max") == 0)
            {
                if ((j + 1) >= argc || Tcl_GetDouble (interp, argv[j + 1], &epsmax) != TCL_OK)
                {
                    cerr << "WARNING invalid max\n";
                    cerr << "uniaxialMaterial MinMax: " << tag << endln;
                    return TCL_ERROR;
                }

                j++;
            }
        }

        UniaxialMaterial* theMat = theTclBuilder->getUniaxialMaterial(matTag);

        if (theMat == 0)
        {
            cerr << "WARNING component material does not exist\n";
            cerr << "Component material: " << matTag;
            cerr << "\nuniaxialMaterial MinMax: " << tag << endln;
            return TCL_ERROR;
        }

        // Parsing was successful, allocate the material
        theMaterial = new MinMaxMaterial(tag, *theMat, epsmin, epsmax);

    }

    else if (strcmp(argv[1], "Fatigue") == 0)
    {
        if (argc < 4)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Fatigue tag? matTag?";
            cerr << " <-D_max dmax?> <-e0 e0?> <-m m?>" << endln;
            cerr << " <-min min?> <-max max?>" << endln;
            return TCL_ERROR;
        }

        int tag, matTag;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Fatigue tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[3], &matTag) != TCL_OK)
        {
            cerr << "WARNING invalid component tag\n";
            cerr << "uniaxialMaterial Fatigue: " << tag << endln;
            return TCL_ERROR;
        }

        double Dmax      =  1.0;
        double E0        =  0.191;
        double m         = -0.458;
        double epsmin    = NEG_INF_STRAIN;
        double epsmax    = POS_INF_STRAIN;

        for (int j = 4; j < argc; j++)
        {
            if (strcmp(argv[j], "-Dmax") == 0)
            {
                if ((j + 1 >= argc) ||
                        (Tcl_GetDouble (interp, argv[j + 1], &Dmax) != TCL_OK))
                {
                    cerr << "WARNING invalid -Dmax";
                    cerr << "uniaxialMaterial Fatigue: " << tag << endln;
                    return TCL_ERROR;
                }
            }
            else if (strcmp(argv[j], "-E0") == 0)
            {
                if ((j + 1 >= argc) ||
                        (Tcl_GetDouble (interp, argv[j + 1], &E0) != TCL_OK))
                {
                    cerr << "WARNING invalid -E0";
                    cerr << "uniaxialMaterial Fatigue: " << tag << endln;
                    return TCL_ERROR;
                }
            }
            else if (strcmp(argv[j], "-m") == 0)
            {
                if ((j + 1 >= argc) ||
                        (Tcl_GetDouble (interp, argv[j + 1], &m) != TCL_OK))
                {
                    cerr << "WARNING invalid -m";
                    cerr << "uniaxialMaterial Fatigue: " << tag << endln;
                    return TCL_ERROR;
                }
            }
            else if (strcmp(argv[j], "-min") == 0)
            {
                if ((j + 1 >= argc) ||
                        (Tcl_GetDouble (interp, argv[j + 1], &epsmin) != TCL_OK))
                {
                    cerr << "WARNING invalid -min ";
                    cerr << "uniaxialMaterial Fatigue: " << tag << endln;
                    return TCL_ERROR;
                }
            }
            else if (strcmp(argv[j], "-max") == 0)
            {
                if ((j + 1 >= argc) ||
                        (Tcl_GetDouble (interp, argv[j + 1], &epsmax) != TCL_OK))
                {
                    cerr << "WARNING invalid -max";
                    cerr << "uniaxialMaterial Fatigue: " << tag << endln;
                    return TCL_ERROR;
                }
            }

            j++;
        }


        UniaxialMaterial* theMat = theTclBuilder->getUniaxialMaterial(matTag);

        if (theMat == 0)
        {
            cerr << "WARNING component material does not exist\n";
            cerr << "Component material: " << matTag;
            cerr << "\nuniaxialMaterial Fatigue: " << tag << endln;
            return TCL_ERROR;
        }

        // Parsing was successful, allocate the material
        theMaterial = new FatigueMaterial(tag, *theMat, Dmax, E0,
                                          m, epsmin, epsmax);

    }
    else if (strcmp(argv[1], "Cable") == 0)
    {
        if (argc != 7)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Cable tag? Prestress? E? effUnitWeight? L_element?>" << endln;
            return TCL_ERROR;
        }

        int tag;
        double Ps, E, unitWeight, L_element;

        if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Cable tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[3], &Ps) != TCL_OK)
        {
            cerr << "WARNING invalid Prestress\n";
            cerr << "uniaxiaMaterial Cable: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[4], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E\n";
            cerr << "uniaxiaMaterial Cable: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[5], &unitWeight) != TCL_OK)
        {
            cerr << "WARNING invalid unit weight\n";
            cerr << "uniaxiaMaterial Cable: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[6], &L_element) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Cable Length " << endln;
            return TCL_ERROR;
        }


        theMaterial = new CableMaterial(tag, Ps, E, unitWeight, L_element);
    }

    else if (strcmp(argv[1], "Pinching4") == 0)
    {
        if (argc != 42 && argc != 31 )
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial Pinching4 tag? stress1p? strain1p? stress2p? strain2p? stress3p? strain3p? stress4p? strain4p? "
                 << "\n<stress1n? strain1n? stress2n? strain2n? stress3n? strain3n? stress4n? strain4n?> rDispP? rForceP? uForceP? "
                 << "\n<rDispN? rForceN? uForceN?> gammaK1? gammaK2? gammaK3? gammaK4? gammaKLimit? gammaD1? gammaD2? gammaD3? gammaD4? "
                 << "\ngammaDLimit? gammaF1? gammaF2? gammaF3? gammaF4? gammaFLimit? gammaE? CycleOrEnergyDamage? ";
            return TCL_ERROR;
        }

        int tag, tDmg;
        double stress1p, stress2p, stress3p, stress4p;
        double strain1p, strain2p, strain3p, strain4p;
        double stress1n, stress2n, stress3n, stress4n;
        double strain1n, strain2n, strain3n, strain4n;
        double rDispP, rForceP, uForceP, rDispN, rForceN, uForceN;
        double gammaK1, gammaK2, gammaK3, gammaK4, gammaKLimit;
        double gammaD1, gammaD2, gammaD3, gammaD4, gammaDLimit;
        double gammaF1, gammaF2, gammaF3, gammaF4, gammaFLimit;
        double gammaE;

        int i = 2;

        if (Tcl_GetInt(interp, argv[i++], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid uniaxialMaterial Pinching4 tag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &stress1p) != TCL_OK)
        {
            cerr << "WARNING invalid stress1p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &strain1p) != TCL_OK)
        {
            cerr << "WARNING invalid strain1p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &stress2p) != TCL_OK)
        {
            cerr << "WARNING invalid stress2p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &strain2p) != TCL_OK)
        {
            cerr << "WARNING invalid strain2p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &stress3p) != TCL_OK)
        {
            cerr << "WARNING invalid stress3p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &strain3p) != TCL_OK)
        {
            cerr << "WARNING invalid strain3p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &stress4p) != TCL_OK)
        {
            cerr << "WARNING invalid stress4p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &strain4p) != TCL_OK)
        {
            cerr << "WARNING invalid strain4p\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 42)
        {
            if (Tcl_GetDouble(interp, argv[i++], &stress1n) != TCL_OK)
            {
                cerr << "WARNING invalid stress1n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &strain1n) != TCL_OK)
            {
                cerr << "WARNING invalid strain1n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &stress2n) != TCL_OK)
            {
                cerr << "WARNING invalid stress2n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &strain2n) != TCL_OK)
            {
                cerr << "WARNING invalid strain2n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &stress3n) != TCL_OK)
            {
                cerr << "WARNING invalid stress3n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &strain3n) != TCL_OK)
            {
                cerr << "WARNING invalid strain3n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &stress4n) != TCL_OK)
            {
                cerr << "WARNING invalid stress4n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &strain4n) != TCL_OK)
            {
                cerr << "WARNING invalid strain4n\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

        }


        if (Tcl_GetDouble(interp, argv[i++], &rDispP) != TCL_OK)
        {
            cerr << "WARNING invalid rDispP\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &rForceP) != TCL_OK)
        {
            cerr << "WARNING invalid rForceP\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &uForceP) != TCL_OK)
        {
            cerr << "WARNING invalid uForceP\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 42)
        {
            if (Tcl_GetDouble(interp, argv[i++], &rDispN) != TCL_OK)
            {
                cerr << "WARNING invalid rDispN\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &rForceN) != TCL_OK)
            {
                cerr << "WARNING invalid rForceN\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[i++], &uForceN) != TCL_OK)
            {
                cerr << "WARNING invalid uForceN\n";
                cerr << "Pinching4 material: " << tag << endln;
                return TCL_ERROR;
            }
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaK1) != TCL_OK)
        {
            cerr << "WARNING invalid gammaK1\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaK2) != TCL_OK)
        {
            cerr << "WARNING invalid gammaK2\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaK3) != TCL_OK)
        {
            cerr << "WARNING invalid gammaK3\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaK4) != TCL_OK)
        {
            cerr << "WARNING invalid gammaK4\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaKLimit) != TCL_OK)
        {
            cerr << "WARNING invalid gammaKLimit\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaD1) != TCL_OK)
        {
            cerr << "WARNING invalid gammaD1\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaD2) != TCL_OK)
        {
            cerr << "WARNING invalid gammaD2\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaD3) != TCL_OK)
        {
            cerr << "WARNING invalid gammaD3\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaD4) != TCL_OK)
        {
            cerr << "WARNING invalid gammaD4\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaDLimit) != TCL_OK)
        {
            cerr << "WARNING invalid gammaDLimit\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaF1) != TCL_OK)
        {
            cerr << "WARNING invalid gammaF1\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaF2) != TCL_OK)
        {
            cerr << "WARNING invalid gammaF2\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaF3) != TCL_OK)
        {
            cerr << "WARNING invalid gammaF3\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaF4) != TCL_OK)
        {
            cerr << "WARNING invalid gammaF4\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaFLimit) != TCL_OK)
        {
            cerr << "WARNING invalid gammaFLimit\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[i++], &gammaE) != TCL_OK)
        {
            cerr << "WARNING invalid gammaE\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        int y;
        y = i;

        if ((strcmp(argv[y], "cycle") == 0) || (strcmp(argv[y], "Cycle") == 0) || (strcmp(argv[y], "DamageCycle") == 0) || (strcmp(argv[y], "damageCycle") == 0))
        {
            tDmg = 1;
        }
        else if ((strcmp(argv[y], "energy") == 0) || (strcmp(argv[y], "Energy") == 0) || (strcmp(argv[y], "DamageEnergy") == 0) || (strcmp(argv[y], "damageEnergy") == 0))
        {
            tDmg = 0;
        }
        else
        {
            cerr << "WARNING invalid type of damage calculation specified\n";
            cerr << "Pinching4 material: " << tag << endln;
            return TCL_ERROR;
        }

        // allocate the pinching material
        if (argc == 42)
        {
            theMaterial = new Pinching4Material (tag,
                                                 stress1p, strain1p, stress2p, strain2p, stress3p, strain3p, stress4p, strain4p,
                                                 stress1n, strain1n, stress2n, strain2n, stress3n, strain3n, stress4n, strain4n,
                                                 rDispP, rForceP, uForceP, rDispN, rForceN, uForceN,
                                                 gammaK1, gammaK2, gammaK3, gammaK4, gammaKLimit,
                                                 gammaD1, gammaD2, gammaD3, gammaD4, gammaDLimit,
                                                 gammaF1, gammaF2, gammaF3, gammaF4, gammaFLimit, gammaE, tDmg);
        }

        if (argc == 31)
        {
            theMaterial = new Pinching4Material (tag,
                                                 stress1p, strain1p, stress2p, strain2p, stress3p, strain3p, stress4p, strain4p,
                                                 rDispP, rForceP, uForceP,
                                                 gammaK1, gammaK2, gammaK3, gammaK4, gammaKLimit,
                                                 gammaD1, gammaD2, gammaD3, gammaD4, gammaDLimit,
                                                 gammaF1, gammaF2, gammaF3, gammaF4, gammaFLimit, gammaE, tDmg);
        }
    }

    else if (strcmp(argv[1], "BarSlip") == 0)
    {
        if (argc != 17 && argc != 15)
        {
            cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            cerr << "Want: uniaxialMaterial BarSlip tag? fc? fy? Es? fu? Eh? db? ld? nb? width? depth? bsflag? type? <damage? unit?>"  << endln;
            return TCL_ERROR;
        }

        int tag, nb, bsf, typ, dmg, unt;
        double fc, fy, Es, fu, Eh, ld, width, depth, db;

        int argStart = 2;

        if (Tcl_GetInt(interp, argv[argStart++], &tag) != TCL_OK)
        {
            cerr << "WARNING invalid tag\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &fc) != TCL_OK)
        {
            cerr << "WARNING invalid fc\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &fy) != TCL_OK)
        {
            cerr << "WARNING invalid fy\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &Es) != TCL_OK)
        {
            cerr << "WARNING invalid Es\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &fu) != TCL_OK)
        {
            cerr << "WARNING invalid fu\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &Eh) != TCL_OK)
        {
            cerr << "WARNING invalid Eh\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &db) != TCL_OK)
        {
            cerr << "WARNING invalid db\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &ld) != TCL_OK)
        {
            cerr << "WARNING invalid ld\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[argStart++], &nb) != TCL_OK)
        {
            cerr << "WARNING invalid nbars\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &width) != TCL_OK)
        {
            cerr << "WARNING invalid width\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetDouble(interp, argv[argStart++], &depth) != TCL_OK)
        {
            cerr << "WARNING invalid depth\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        int y;
        y = argStart;


        if ((strcmp(argv[y], "strong") == 0) || (strcmp(argv[y], "Strong") == 0) || (strcmp(argv[y], "weak") == 0) || (strcmp(argv[y], "Weak") == 0))
        {
            if ((strcmp(argv[y], "strong") == 0) || (strcmp(argv[y], "Strong") == 0))
            {
                bsf = 0;
            }

            if ((strcmp(argv[y], "weak") == 0) || (strcmp(argv[y], "Weak") == 0))
            {
                bsf = 1;
            }
        }
        else
        {
            cerr << "WARNING invalid bond strength specified\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        y ++;

        if ((strcmp(argv[y], "beamtop") == 0) || (strcmp(argv[y], "beamTop") == 0) ||
                (strcmp(argv[y], "beambot") == 0) || (strcmp(argv[y], "beamBot") == 0) || (strcmp(argv[y], "beambottom") == 0) || (strcmp(argv[y], "beamBottom") == 0) ||
                (strcmp(argv[y], "beam") == 0) || (strcmp(argv[y], "Beam") == 0) || (strcmp(argv[y], "Column") == 0) || (strcmp(argv[y], "column") == 0))
        {
            if ((strcmp(argv[y], "beamtop") == 0) || (strcmp(argv[y], "beamTop") == 0) || (strcmp(argv[y], "beam") == 0) || (strcmp(argv[y], "Beam") == 0))
            {
                typ = 0;
            }

            if ((strcmp(argv[y], "beambot") == 0) || (strcmp(argv[y], "beamBot") == 0) || (strcmp(argv[y], "beambottom") == 0) || (strcmp(argv[y], "beamBottom") == 0))
            {
                typ = 1;
            }

            if ((strcmp(argv[y], "column") == 0) || (strcmp(argv[y], "Column") == 0))
            {
                typ = 2;
            }
        }
        else
        {
            cerr << "WARNING invalid location of bar specified\n";
            cerr << "BarSlip: " << tag << endln;
            return TCL_ERROR;
        }

        if (argc == 17)
        {
            y ++;

            if ((strcmp(argv[y], "damage1") == 0) || (strcmp(argv[y], "Damage1") == 0) || (strcmp(argv[y], "damage2") == 0) || (strcmp(argv[y], "Damage2") == 0) ||
                    (strcmp(argv[y], "nodamage") == 0) || (strcmp(argv[y], "Nodamage") == 0) || (strcmp(argv[y], "NoDamage") == 0) || (strcmp(argv[y], "noDamage") == 0))
            {
                if ((strcmp(argv[y], "damage1") == 0) || (strcmp(argv[y], "Damage1") == 0))
                {
                    dmg = 1;
                }
                else if ((strcmp(argv[y], "damage2") == 0) || (strcmp(argv[y], "Damage2") == 0))
                {
                    dmg = 2;
                }
                else if ((strcmp(argv[y], "nodamage") == 0) || (strcmp(argv[y], "Nodamage") == 0) || (strcmp(argv[y], "NoDamage") == 0) || (strcmp(argv[y], "noDamage") == 0))
                {
                    dmg = 0;
                }

            }
            else
            {
                cerr << "WARNING invalid damage specified\n";
                cerr << "BarSlip: " << tag << endln;
                return TCL_ERROR;
            }

            y ++;

            if ((strcmp(argv[y], "mpa") == 0) || (strcmp(argv[y], "MPa") == 0) || (strcmp(argv[y], "mPa") == 0) || (strcmp(argv[y], "Mpa") == 0) ||
                    (strcmp(argv[y], "psi") == 0) || (strcmp(argv[y], "Psi") == 0) || (strcmp(argv[y], "PSI") == 0) || (strcmp(argv[y], "Pa") == 0) ||
                    (strcmp(argv[y], "pa") == 0) ||  (strcmp(argv[y], "psf") == 0) || (strcmp(argv[y], "Psf") == 0) || (strcmp(argv[y], "PSF") == 0) ||
                    (strcmp(argv[y], "ksi") == 0) || (strcmp(argv[y], "Ksi") == 0) || (strcmp(argv[y], "KSI") == 0) || (strcmp(argv[y], "ksf") == 0) ||
                    (strcmp(argv[y], "Ksf") == 0) || (strcmp(argv[y], "KSF") == 0))
            {
                if ((strcmp(argv[y], "mpa") == 0) || (strcmp(argv[y], "MPa") == 0) || (strcmp(argv[y], "mPa") == 0) || (strcmp(argv[y], "Mpa") == 0))
                {
                    unt = 1;
                }
                else if ((strcmp(argv[y], "psi") == 0) || (strcmp(argv[y], "Psi") == 0) || (strcmp(argv[y], "PSI") == 0))
                {
                    unt = 2;
                }
                else if ((strcmp(argv[y], "Pa") == 0) || (strcmp(argv[y], "pa") == 0))
                {
                    unt = 3;
                }
                else if ((strcmp(argv[y], "psf") == 0) || (strcmp(argv[y], "Psf") == 0) || (strcmp(argv[y], "PSF") == 0))
                {
                    unt = 4;
                }
                else if ((strcmp(argv[y], "ksi") == 0) || (strcmp(argv[y], "Ksi") == 0) || (strcmp(argv[y], "KSI") == 0))
                {
                    unt = 5;
                }
                else if ((strcmp(argv[y], "ksf") == 0) || (strcmp(argv[y], "Ksf") == 0) || (strcmp(argv[y], "KSF") == 0))
                {
                    unt = 6;
                }
            }
            else
            {
                cerr << "WARNING invalid unit specified\n";
                cerr << "BarSlip: " << tag << endln;
                return TCL_ERROR;
            }
        }

        // allocate the material
        if (argc == 15 )
        {
            theMaterial = new BarSlipMaterial (tag, fc, fy, Es, fu, Eh, db, ld, nb, width, depth, bsf, typ);
        }

        if (argc == 17)
        {
            theMaterial = new BarSlipMaterial (tag, fc, fy, Es, fu, Eh, db, ld, nb, width, depth, bsf, typ, dmg, unt);
        }

    }

    else
    {
        // Fedeas
        theMaterial = TclModelBuilder_addFedeasMaterial(clientData, interp, argc, argv, theTclBuilder);

        //// Boris Jeremic took out 21Mar2008
        //      // Drain
        //      if (theMaterial == 0)
        //  theMaterial = TclModelBuilder_addDrainMaterial(clientData, interp, argc, argv, theTclBuilder);

        //// Boris Jeremic took out 21Mar2008
        //      // SNAP
        //      if (theMaterial == 0)
        //  theMaterial = TclModelBuilder_addSnapMaterial(clientData, interp, argc, argv, theTclBuilder);

        // Boris Jeremic took out 20Mar2008
        //      // Py, Tz, Qz models
        //      if (theMaterial == 0)
        //  theMaterial = TclModelBuilder_addPyTzQzMaterial(clientData, interp, argc, argv, theTclBuilder, theDomain);
        //
        #ifdef _LIMITSTATEMATERIAL

        // LimitState
        if (theMaterial == 0)
        {
            theMaterial = Tcl_AddLimitStateMaterial(clientData, interp, argc, argv);
        }

        #endif
    }

    //GZ out if (theMaterial == 0) {
    //GZ out
    //GZ out   //
    //GZ out   // maybe element in a package already loaded
    //GZ out   //  loop through linked list of loaded functions comparing names & if find call it
    //GZ out   //
    //GZ out
    //GZ out   UniaxialPackageCommand *matCommands = theUniaxialPackageCommands;
    //GZ out   bool found = false;
    //GZ out   while (matCommands != NULL && found == false) {
    //GZ out    if (strcmp(argv[1], matCommands->funcName) == 0) {
    //GZ out
    //GZ out      int result = (*(matCommands->funcPtr))(clientData, interp, argc, argv, theTclBuilder);
    //GZ out      return result;
    //GZ out    } else
    //GZ out      matCommands = matCommands->next;
    //GZ out   }
    //GZ out
    //GZ out
    //GZ out   //
    //GZ out   // maybe element command exists in a dll in library path
    //GZ out   //  so try loading package of same name as material name containg
    //GZ out   //  a c function "TclCommand_MatName"
    //GZ out   //
    //GZ out
    //GZ out   void *libHandle;
    //GZ out   int (*funcPtr)(ClientData clientData, Tcl_Interp *interp,  int argc,
    //GZ out             TCL_Char **argv, TclModelBuilder *);
    //GZ out   int matNameLength = strlen(argv[1]);
    //GZ out   char *tclFuncName = new char[matNameLength+12];
    //GZ out   strcpy(tclFuncName, "TclCommand_");
    //GZ out   strcpy(&tclFuncName[11], argv[1]);
    //GZ out   int res = getLibraryFunction(argv[1], tclFuncName, &libHandle, (void **)&funcPtr);
    //GZ out
    //GZ out   delete [] tclFuncName;
    //GZ out
    //GZ out   if (res == 0) {
    //GZ out
    //GZ out    //
    //GZ out    // add loaded function to list of functions
    //GZ out    //
    //GZ out    char *matName = new char[matNameLength+1];
    //GZ out    strcpy(matName, argv[1]);
    //GZ out    UniaxialPackageCommand *theMatCommand = new UniaxialPackageCommand;
    //GZ out    theMatCommand->funcPtr = funcPtr;
    //GZ out    theMatCommand->funcName = matName;
    //GZ out    theMatCommand->next = theUniaxialPackageCommands;
    //GZ out    theUniaxialPackageCommands = theMatCommand;
    //GZ out
    //GZ out    int result = (*funcPtr)(clientData, interp,
    //GZ out                argc, argv,
    //GZ out                theTclBuilder);
    //GZ out
    //GZ out    return result;
    //GZ out   }
    //GZ out }

    //
    // if still here the element command does not exist
    //

    if (theMaterial == 0)
    {
        cerr << "WARNING could not create uniaxialMaterial " << argv[1] << endln;
        return TCL_ERROR;
    }

    // Now add the material to the modelBuilder
    if (theTclBuilder->addUniaxialMaterial(*theMaterial) < 0)
    {
        cerr << "WARNING could not add uniaxialMaterial to the domain\n";
        cerr << *theMaterial << endln;
        delete theMaterial; // invoke the material objects destructor, otherwise mem leak
        return TCL_ERROR;
    }

    return TCL_OK;
}
