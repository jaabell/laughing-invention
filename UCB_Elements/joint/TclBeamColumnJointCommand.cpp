/* ****************************************************************** **
**    Opensees - Open System for Earthquake Engineering Simulation    **
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

// $Revision: 1.3 $
// $Date: 2004/10/06 19:21:45 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/joint/TclBeamColumnJointCommand.cpp,v $

// Written: NM (nmitra@u.washington.edu)
// Created: April 2002
// Revised: August 2004
//
// Description: This file contains the implementation of the commands used
// to add beam column joint to a model.
// Update: Optional User interfaces added in.


#include <stdlib.h>
#include <string.h>

#include <BeamColumnJoint2d.h>
#include <BeamColumnJoint3d.h>
#include <Information.h>
#include <ElementResponse.h>

#include <Domain.h>
#include <Node.h>
#include <Channel.h>  // parallel prog. not yet implemented 
#include <FEM_ObjectBroker.h>
#include <Renderer.h>
#include <UniaxialMaterial.h>
#include <TclModelBuilder.h>

extern void printCommand(int argc, TCL_Char** argv);
static Domain* theTclModelBuilderDomain = 0;
static TclModelBuilder* theTclModelBuilder = 0;

int TclModelBuilder_addBeamColumnJoint(ClientData clientData, Tcl_Interp* interp, int argc,
                                       TCL_Char** argv, Domain* theTclDomain,
                                       TclModelBuilder* theTclBuilder, int eleArgStart)
{
    // ensure destructor not called
    if (theTclBuilder == 0)
    {
        std::cerr << "Warning builder has been destroyed \n";
        return TCL_ERROR;
    }

    theTclModelBuilderDomain = theTclDomain;
    theTclModelBuilder = theTclBuilder;

    int NDM, NDF;

    NDM = theTclModelBuilder->getNDM();   // dimension of the structure (1d, 2d, or 3d)
    NDF = theTclModelBuilder->getNDF();   // number of degrees of freedom per node


    if ((NDM == 2 && NDF == 3) || (NDM == 3 && NDF == 6))
    {

        // check no of arguments
        if ((argc - eleArgStart) != 19 && (argc - eleArgStart) != 21)
        {
            std::cerr << "WARNING insufficient arguments\n";
            printCommand(argc, argv);
            std::cerr << "Want: element beamColumnJoint eleTag? node1? node2? node3? node4? matTag1? matTag2? matTag3?\n";
            std::cerr << "matTag4? matTag5? matTag6? matTag7? matTag8? matTag9? matTag10? matTag11? matTag12? matTag13?\n";
            std::cerr << "<ElementHeightFactor? ElementWidthFactor?>\n";

            return TCL_ERROR;
        }

        int id, nd1, nd2, nd3, nd4, matId1, matId2, matId3, matId4, matId5, matId6, matId7, matId8, matId9, matId10;
        int matId11, matId12, matId13;
        double hgtfac, wdtfac;

        UniaxialMaterial* theMaterial1 = 0;
        UniaxialMaterial* theMaterial2 = 0;
        UniaxialMaterial* theMaterial3 = 0;
        UniaxialMaterial* theMaterial4 = 0;
        UniaxialMaterial* theMaterial5 = 0;
        UniaxialMaterial* theMaterial6 = 0;
        UniaxialMaterial* theMaterial7 = 0;
        UniaxialMaterial* theMaterial8 = 0;
        UniaxialMaterial* theMaterial9 = 0;
        UniaxialMaterial* theMaterial10 = 0;
        UniaxialMaterial* theMaterial11 = 0;
        UniaxialMaterial* theMaterial12 = 0;
        UniaxialMaterial* theMaterial13 = 0;

        if (Tcl_GetInt(interp, argv[1 + eleArgStart], &id) != TCL_OK)
        {
            std::cerr << "WARNING invalid beamColumnJoint eleTag" << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[2 + eleArgStart], &nd1) != TCL_OK)
        {
            std::cerr << "WARNING invalid Node 1\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[3 + eleArgStart], &nd2) != TCL_OK)
        {
            std::cerr << "WARNING invalid Node 2\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[4 + eleArgStart], &nd3) != TCL_OK)
        {
            std::cerr << "WARNING invalid Node 3\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[5 + eleArgStart], &nd4) != TCL_OK)
        {
            std::cerr << "WARNING invalid Node 4\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[6 + eleArgStart], &matId1) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 1\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[7 + eleArgStart], &matId2) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 2\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[8 + eleArgStart], &matId3) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 3\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[9 + eleArgStart], &matId4) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 4\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[10 + eleArgStart], &matId5) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 5\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[11 + eleArgStart], &matId6) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 6\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[12 + eleArgStart], &matId7) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 7\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[13 + eleArgStart], &matId8) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 8\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[14 + eleArgStart], &matId9) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 9\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[15 + eleArgStart], &matId10) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 10\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[16 + eleArgStart], &matId11) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 11\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[17 + eleArgStart], &matId12) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 12\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }

        if (Tcl_GetInt(interp, argv[18 + eleArgStart], &matId13) != TCL_OK)
        {
            std::cerr << "WARNING invalid Material Tag 13\n";
            std::cerr << "beamColumnJoint Element: " << id << endln;
            return TCL_ERROR;
        }


        if ((argc - eleArgStart) == 21)
        {
            if (Tcl_GetDouble(interp, argv[19 + eleArgStart], &hgtfac) != TCL_OK)
            {
                std::cerr << "WARNING invalid factor for height\n";
                std::cerr << "beamColumnJoint Element: " << id << endln;
                return TCL_ERROR;
            }

            if (Tcl_GetDouble(interp, argv[20 + eleArgStart], &wdtfac) != TCL_OK)
            {
                std::cerr << "WARNING invalid factor for width\n";
                std::cerr << "beamColumnJoint Element: " << id << endln;
                return TCL_ERROR;
            }
        }

        if (matId1 != 0)
        {
            theMaterial1 = theTclBuilder->getUniaxialMaterial(matId1);

            if (theMaterial1 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId1;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial1 = 0;
        }

        if (matId2 != 0)
        {
            theMaterial2 = theTclBuilder->getUniaxialMaterial(matId2);

            if (theMaterial2 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId2;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial2 = 0;
        }

        if (matId3 != 0)
        {
            theMaterial3 = theTclBuilder->getUniaxialMaterial(matId3);

            if (theMaterial3 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId3;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial3 = 0;
        }

        if (matId4 != 0)
        {
            theMaterial4 = theTclBuilder->getUniaxialMaterial(matId4);

            if (theMaterial4 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId4;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial4 = 0;
        }

        if (matId5 != 0)
        {
            theMaterial5 = theTclBuilder->getUniaxialMaterial(matId5);

            if (theMaterial5 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId5;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial5 = 0;
        }

        if (matId6 != 0)
        {
            theMaterial6 = theTclBuilder->getUniaxialMaterial(matId6);

            if (theMaterial6 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId6;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial6 = 0;
        }

        if (matId7 != 0)
        {
            theMaterial7 = theTclBuilder->getUniaxialMaterial(matId7);

            if (theMaterial7 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId7;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial7 = 0;
        }

        if (matId8 != 0)
        {
            theMaterial8 = theTclBuilder->getUniaxialMaterial(matId8);

            if (theMaterial8 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId8;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial8 = 0;
        }

        if (matId9 != 0)
        {
            theMaterial9 = theTclBuilder->getUniaxialMaterial(matId9);

            if (theMaterial9 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId9;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial9 = 0;
        }


        if (matId10 != 0)
        {
            theMaterial10 = theTclBuilder->getUniaxialMaterial(matId10);

            if (theMaterial10 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId10;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial10 = 0;
        }

        if (matId11 != 0)
        {
            theMaterial11 = theTclBuilder->getUniaxialMaterial(matId11);

            if (theMaterial11 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId11;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial11 = 0;
        }

        if (matId12 != 0)
        {
            theMaterial12 = theTclBuilder->getUniaxialMaterial(matId12);

            if (theMaterial12 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId12;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial12 = 0;
        }

        if (matId13 != 0)
        {
            theMaterial13 = theTclBuilder->getUniaxialMaterial(matId13);

            if (theMaterial13 == 0)
            {
                std::cerr << "WARNING material not found\n";
                std::cerr << "Material: " << matId13;
                std::cerr << "\nbeamColumnJoint element: " << id << endln;
                return TCL_ERROR;
            }
        }
        else
        {
            theMaterial13 = 0;
        }

        // create Beam Column Joint Element and add it to Domain
        Element* theBeamColumnJoint = 0;

        if (NDM == 2)
        {
            if ((argc - eleArgStart) == 19)
            {
                theBeamColumnJoint = new BeamColumnJoint2d(id, nd1, nd2, nd3, nd4, *theMaterial1, *theMaterial2,
                        *theMaterial3, *theMaterial4, *theMaterial5, *theMaterial6, *theMaterial7, *theMaterial8,
                        *theMaterial9, *theMaterial10, *theMaterial11, *theMaterial12, *theMaterial13);
            }
            else if ((argc - eleArgStart) == 21)
            {
                theBeamColumnJoint = new BeamColumnJoint2d(id, nd1, nd2, nd3, nd4, *theMaterial1, *theMaterial2,
                        *theMaterial3, *theMaterial4, *theMaterial5, *theMaterial6, *theMaterial7, *theMaterial8,
                        *theMaterial9, *theMaterial10, *theMaterial11, *theMaterial12, *theMaterial13, hgtfac, wdtfac);
            }
        }
        else if (NDM == 3)
        {
            if ((argc - eleArgStart) == 19)
            {
                theBeamColumnJoint = new BeamColumnJoint3d(id, nd1, nd2, nd3, nd4, *theMaterial1, *theMaterial2,
                        *theMaterial3, *theMaterial4, *theMaterial5, *theMaterial6, *theMaterial7, *theMaterial8,
                        *theMaterial9, *theMaterial10, *theMaterial11, *theMaterial12, *theMaterial13);
            }
            else if ((argc - eleArgStart) == 21)
            {
                theBeamColumnJoint = new BeamColumnJoint3d(id, nd1, nd2, nd3, nd4, *theMaterial1, *theMaterial2,
                        *theMaterial3, *theMaterial4, *theMaterial5, *theMaterial6, *theMaterial7, *theMaterial8,
                        *theMaterial9, *theMaterial10, *theMaterial11, *theMaterial12, *theMaterial13, hgtfac, wdtfac);
            }
        }

        if (theBeamColumnJoint == 0)
        {
            std::cerr << "WARNING ran out of memory creating elements\n";
            std::cerr << "beamColumnJoint element: " << id << endln;
            return TCL_ERROR;
        }

        if (theTclDomain->addElement(theBeamColumnJoint) == false)
        {
            std::cerr << "WARNING could not add element to the domain\n";
            std::cerr << "beamColumnJoint element: " << id << endln;
            delete theBeamColumnJoint;
            return TCL_ERROR;
        }

    }
    else
    {
        std::cerr << "WARNING NDM = " << NDM << " and NDF = " << NDF << "is imcompatible with available frame elements";
        return TCL_ERROR;
    }

    //the element succesfully created and added to the domain
    return TCL_OK;
}
