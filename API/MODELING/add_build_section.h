///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              September 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Build Section</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of build section.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!



//! Inputs:
//! - SectionTag: unique section object tag
//! - MaterialNumber: unique material object tag
//! - GJ: tortional stiffness




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n



int add_build_section(int SectionNumber,
                      int MaterialNumber,
                      int GJ)
{
    SectionRepres* sectionRepres = 0;
    sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr << "Error: (add_build_section) cannot retrieve section!" << endl;
        return -1;
    }

    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;

    if (fiberSectionRepr == 0)
    {
        cerr << "Error: (add_build_section) memory allocating for FiberSectionRepr!" << endl;
        return -1;
    }

    int i, j, k;
    int numFibers;
    int numPatches;

    Patch** patch;

    int  numReinfLayers;
    ReinfLayer** reinfLayer;

    numPatches     = fiberSectionRepr->getNumPatches();
    patch          = fiberSectionRepr->getPatches();
    numReinfLayers = fiberSectionRepr->getNumReinfLayers();
    reinfLayer     = fiberSectionRepr->getReinfLayers();

    int   numSectionRepresFibers = fiberSectionRepr->getNumFibers();
    Fiber**  sectionRepresFibers = fiberSectionRepr->getFibers();

    numFibers = numSectionRepresFibers;

    for (i = 0; i < numPatches; i++)
    {
        numFibers += patch[i]->getNumCells();
    }

    for (i = 0; i < numReinfLayers; i++)
    {
        numFibers += reinfLayer[i]->getNumReinfBars();
    }

    static Vector fiberPosition(2);
    int    matTag;

    ID     fibersMaterial(numFibers - numSectionRepresFibers) ;
    Matrix fibersPosition(2, numFibers - numSectionRepresFibers);
    Vector fibersArea(numFibers - numSectionRepresFibers);


    int  numCells;
    Cell** cell;


    k = 0;

    for (i = 0; i < numPatches; i++)
    {
        numCells  = patch[i]->getNumCells();
        matTag    = patch[i]->getMaterialID();


        cell = patch[i]->getCells();

        if (cell == 0)
        {
            cerr << "Error: (add_build_section) out of run to create fibers!" << endl;
            return -1;
        }


        for (j = 0; j < numCells; j++)
        {
            fibersMaterial(k) = matTag;
            fibersArea(k)     = cell[j]->getArea();
            fiberPosition     = cell[j]->getCentroidPosition();

            fibersPosition(0, k) = fiberPosition(0);
            fibersPosition(1, k) = fiberPosition(1);

            k++;
        }

        for (j = 0; j < numCells; j++)
        {
            delete cell[j];
        }

        delete [] cell;
    }



    ReinfBar* reinfBar;
    int numReinfBars;

    for (i = 0; i < numReinfLayers; i++)
    {
        numReinfBars = reinfLayer[i]->getNumReinfBars();
        reinfBar     = reinfLayer[i]->getReinfBars();
        matTag  = reinfLayer[i]->getMaterialID();

        for (j = 0; j < numReinfBars; j++)
        {
            fibersMaterial(k) = matTag;
            fibersArea(k)     = reinfBar[j].getArea();
            fiberPosition     = reinfBar[j].getPosition();

            fibersPosition(0, k) = fiberPosition(0);
            fibersPosition(1, k) = fiberPosition(1);

            k++;
        }

        delete [] reinfBar;
    }





    Fiber** fiber = new Fiber *[numFibers];

    if (fiber == 0)
    {
        cerr << "Error: (add_build_section) unable to allocate fibers!" << endl;
        return -1;
    }


    // copy the section repres fibers
    for (i = 0; i < numSectionRepresFibers; i++)
    {
        fiber[i] = sectionRepresFibers[i];
    }

    UniaxialMaterial* material;


    static Vector fiberPosition2(2);

    k = 0;

    for (i = numSectionRepresFibers; i < numFibers; i++)
    {

        material = theDomain.getUniaxialMaterial(fibersMaterial(k));

        if (material == 0)
        {
            cerr << "Error: (add_build_section) invalid material ID for patch!" << endl;
            return -1;
        }

        fiberPosition2(0) = fibersPosition(0, k);
        fiberPosition2(1) = fibersPosition(1, k);

        fiber[i] = new UniaxialFiber3d(k, *material, fibersArea(k), fiberPosition2);

        if (fibersArea(k) < 0)
        {
            cerr << "ERROR: " << fiberPosition2(0) << " " << fiberPosition2(1) << endln;
        }

        if (!fiber[k])
        {
            cerr <<  "WARNING: (add_build_section) unable to allocate fiber \n";
            return -1;
        }

        k++;
    }



    SectionForceDeformation* section = 0;


    section = new FiberSectionGJ(SectionNumber, numFibers, fiber, GJ);


    for (i = 0; i < numFibers; i++)
    {
        delete fiber[i];
    }

    delete [] fiber;


    if (section == 0)
    {
        cerr <<  "WARNING: (add_build_section) - cannot construct section\n";
        return -1;
    }

    if (theDomain.addSection (*section) < 0)
    {
        cerr <<  "WARNING: (add_build_section) - cannot add section\n";
        return -1;
    }


    return 0;
};





//! Inputs:
//! - SectionNumber: unique section object tag
//! - MaterialNumber: unique material object tag
//! - theDomain: place to store the objects

int add_build_section(int SectionNumber,
                      int MaterialNumber)
{
    SectionRepres* sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr <<  "WARNING: (add_build_section) cannot retrieve section\n";
        return -1;
    }

    // build the section
    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;


    if (fiberSectionRepr == 0)
    {
        cerr << "Error: (add_build_section) memory allocating for FiberSectionRepr!" << endl;
        return -1;
    }


    int i, j, k;
    int numFibers;
    int numPatches;

    Patch** patch;

    int  numReinfLayers;
    ReinfLayer** reinfLayer;

    numPatches     = fiberSectionRepr->getNumPatches();
    patch          = fiberSectionRepr->getPatches();
    numReinfLayers = fiberSectionRepr->getNumReinfLayers();
    reinfLayer     = fiberSectionRepr->getReinfLayers();

    int   numSectionRepresFibers = fiberSectionRepr->getNumFibers();
    Fiber**  sectionRepresFibers = fiberSectionRepr->getFibers();

    numFibers = numSectionRepresFibers;

    for (i = 0; i < numPatches; i++)
    {
        numFibers += patch[i]->getNumCells();
    }

    for (i = 0; i < numReinfLayers; i++)
    {
        numFibers += reinfLayer[i]->getNumReinfBars();
    }

    static Vector fiberPosition(2);
    int    matTag;

    ID     fibersMaterial(numFibers - numSectionRepresFibers) ;
    Matrix fibersPosition(2, numFibers - numSectionRepresFibers);
    Vector fibersArea(numFibers - numSectionRepresFibers);


    int  numCells;
    Cell** cell;


    k = 0;

    for (i = 0; i < numPatches; i++)
    {
        numCells  = patch[i]->getNumCells();
        matTag    = patch[i]->getMaterialID();


        cell = patch[i]->getCells();

        if (cell == 0)
        {
            cerr <<  "WARNING: (add_build_section) out of run to create fibers\n";
            //                abort();
            return -1;
        }


        for (j = 0; j < numCells; j++)
        {
            fibersMaterial(k) = matTag;
            fibersArea(k)     = cell[j]->getArea();
            fiberPosition     = cell[j]->getCentroidPosition();

            fibersPosition(0, k) = fiberPosition(0);
            fibersPosition(1, k) = fiberPosition(1);

            k++;
        }

        for (j = 0; j < numCells; j++)
        {
            delete cell[j];
        }

        delete [] cell;
    }



    ReinfBar* reinfBar;
    int numReinfBars;

    for (i = 0; i < numReinfLayers; i++)
    {
        numReinfBars = reinfLayer[i]->getNumReinfBars();
        reinfBar     = reinfLayer[i]->getReinfBars();
        matTag  = reinfLayer[i]->getMaterialID();

        for (j = 0; j < numReinfBars; j++)
        {
            fibersMaterial(k) = matTag;
            fibersArea(k)     = reinfBar[j].getArea();
            fiberPosition     = reinfBar[j].getPosition();

            fibersPosition(0, k) = fiberPosition(0);
            fibersPosition(1, k) = fiberPosition(1);

            k++;
        }

        delete [] reinfBar;
    }





    Fiber** fiber = new Fiber *[numFibers];

    if (fiber == 0)
    {
        cerr <<  "WARNING: (add_build_section) unable to allocate fibers \n";
        //                abort();
        return -1;
    }


    for (i = 0; i < numSectionRepresFibers; i++)
    {
        fiber[i] = sectionRepresFibers[i];
    }

    UniaxialMaterial* material;


    static Vector fiberPosition2(2);

    k = 0;

    for (i = numSectionRepresFibers; i < numFibers; i++)
    {

        material = theDomain.getUniaxialMaterial(fibersMaterial(k));

        if (material == 0)
        {
            cerr <<  "WARNING: (add_build_section) invalid material ID for patch\n";
            //                abort();
            return -1;
        }

        fiberPosition2(0) = fibersPosition(0, k);
        fiberPosition2(1) = fibersPosition(1, k);

        fiber[i] = new UniaxialFiber3d(k, *material, fibersArea(k), fiberPosition2);

        if (fibersArea(k) < 0)
        {
            cerr << "ERROR: " << fiberPosition2(0) << " " << fiberPosition2(1) << endln;
        }

        if (!fiber[k])
        {
            cerr <<  "WARNING: (add_build_section) unable to allocate fiber \n";
            //                abort();
            return -1;
        }

        k++;
    }



    SectionForceDeformation* section = 0;


    section = new FiberSection3d(SectionNumber, numFibers, fiber);


    for (i = 0; i < numFibers; i++)
    {
        delete fiber[i];
    }

    delete [] fiber;


    if (section == 0)
    {
        cerr <<  "WARNING: (add_build_section) - cannot construct section\n";
        //                abort();
        return -1;
    }

    if (theDomain.addSection (*section) < 0)
    {
        cerr <<  "WARNING: (add_build_section) - cannot add section\n";
        //                abort();
        return -1;
    }


    return 0;
};





