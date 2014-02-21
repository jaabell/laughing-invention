// @ rkaul@stanford.edu
// @ ggd@stanford.edu

#include <TclModelBuilder.h>
#include <string.h>
#include <Vector.h>

#include <YieldSurface_BC.h>
#include <YS_Section2D01.h>
#include <YS_Section2D02.h>

static void printCommand(int argc, TCL_Char** argv)
{
    cerr << "Input command: ";

    for (int i = 0; i < argc; i++)
    {
        cerr << argv[i] << " ";
    }

    cerr << endln;
}

SectionForceDeformation*
TclModelBuilderYS_SectionCommand(ClientData clienData, Tcl_Interp* interp, int argc,
                                 TCL_Char** argv, TclModelBuilder* theBuilder)
{
    if (argc < 3)
    {
        cerr << "WARNING insufficient number of arguments\n";
        printCommand(argc, argv);
        return 0;
    }

    int tag;

    if (Tcl_GetInt(interp, argv[2], &tag) != TCL_OK)
    {
        cerr << "WARNING invalid section tag\n";
        printCommand(argc, argv);
        return 0;
    }

    SectionForceDeformation* theModel = 0;

    if (strcmp(argv[1], "YS_Section2D01") == 0 ||
            strcmp(argv[1], "YS_Section2d01") == 0)
    {

        if (argc < 7)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: section YS_Section2D01 tag? E? A? Iz? ysTag? <algo?>" << endln;
            return 0;
        }

        int algo, ysTag;
        double E, A, Iz;
        int indx = 3;

        if (Tcl_GetDouble (interp, argv[indx++], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetDouble (interp, argv[indx++], &A) != TCL_OK)
        {
            cerr << "WARNING invalid A" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetDouble (interp, argv[indx++], &Iz) != TCL_OK)
        {
            cerr << "WARNING invalid Iz" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetInt (interp, argv[indx++], &ysTag) != TCL_OK)
        {
            cerr << "WARNING invalid ysTag" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        YieldSurface_BC* ys = theBuilder->getYieldSurface_BC(ysTag);

        if (ys == 0)
        {
            cerr << "WARNING yield surface does not exist\n";
            cerr << "yieldSurface: " << ysTag;
            cerr << "\nsection YieldSurface: " << tag << endln;
            return 0;
        }

        bool useKr = true;

        if(argc > indx)
        {
            if (Tcl_GetInt(interp, argv[indx++], &algo) != TCL_OK)
            {
                cerr << "WARNING invalid algo" << endln;
                cerr << " section: " << tag << endln;
                return 0;
            }

            if(algo == 0)
            {
                useKr = false;
            }
        }

        theModel = new YS_Section2D01(tag, E, A, Iz, ys, useKr);
    }

    else if (strcmp(argv[1], "YS_Section2D02") == 0 ||
             strcmp(argv[1], "YS_Section2d02") == 0)
    {

        if (argc < 8)
        {
            cerr << "WARNING invalid number of arguments\n";
            printCommand(argc, argv);
            cerr << "Want: section YS_Section2D01 tag? E? A? Iz? maxPlastRot? ysTag? <algo?>" << endln;
            return 0;
        }

        int algo, ysTag;
        double E, A, Iz, maxPlstkRot;
        int indx = 3;

        if (Tcl_GetDouble (interp, argv[indx++], &E) != TCL_OK)
        {
            cerr << "WARNING invalid E" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetDouble (interp, argv[indx++], &A) != TCL_OK)
        {
            cerr << "WARNING invalid A" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetDouble (interp, argv[indx++], &Iz) != TCL_OK)
        {
            cerr << "WARNING invalid Iz" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetDouble (interp, argv[indx++], &maxPlstkRot) != TCL_OK)
        {
            cerr << "WARNING maxPlstkRot " << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        if (Tcl_GetInt (interp, argv[indx++], &ysTag) != TCL_OK)
        {
            cerr << "WARNING invalid ysTag" << endln;
            cerr << " section: " << tag << endln;
            return 0;
        }

        YieldSurface_BC* ys = theBuilder->getYieldSurface_BC(ysTag);

        if (ys == 0)
        {
            cerr << "WARNING yield surface does not exist\n";
            cerr << "yieldSurface: " << ysTag;
            cerr << "\nsection YieldSurface: " << tag << endln;
            return 0;
        }

        bool useKr = true;

        if(argc > indx)
        {
            if (Tcl_GetInt(interp, argv[indx++], &algo) != TCL_OK)
            {
                cerr << "WARNING invalid algo" << endln;
                cerr << " section: " << tag << endln;
                return 0;
            }

            if(algo == 0)
            {
                useKr = false;
            }
        }

        theModel = new YS_Section2D02(tag, E, A, Iz, maxPlstkRot, ys, useKr);
    }

    return theModel;
}
