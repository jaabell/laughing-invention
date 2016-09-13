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

// Written: fmk

#include <UniaxialMaterial.h>
#include <string.h>
#include <Information.h>
// #include <MaterialResponse.h>
#include <float.h>
#include <Vector.h>
#include <stdlib.h>

#include <TaggedObject.h>
#include <MapOfTaggedObjects.h>

static MapOfTaggedObjects theUniaxialMaterialObjects;

bool OPS_addUniaxialMaterial(UniaxialMaterial *newComponent)
{
    return theUniaxialMaterialObjects.addComponent(newComponent);
}

UniaxialMaterial *OPS_getUniaxialMaterial(int tag)
{

    TaggedObject *theResult = theUniaxialMaterialObjects.getComponentPtr(tag);
    if (theResult == 0)
    {
        std::cerr << "UniaxialMaterial *getUniaxialMaterial(int tag) - none found with tag: " << tag << endln;
        return 0;
    }
    UniaxialMaterial *theMat = (UniaxialMaterial *)theResult;

    return theMat;
}

void OPS_clearAllUniaxialMaterial(void)
{
    theUniaxialMaterialObjects.clearAll();
}


UniaxialMaterial::UniaxialMaterial(int tag, int clasTag)
    : Material(tag, clasTag)
{

}

UniaxialMaterial::~UniaxialMaterial()
{

}


int
UniaxialMaterial::setTrial(double strain, double &stress, double &tangent, double strainRate)
{
    int res = this->setTrialStrain(strain, strainRate);
    if (res == 0)
    {
        stress = this->getStress();
        tangent = this->getTangent();
    }
    else
    {
        std::cerr << "UniaxialMaterial::setTrial() - material failed in setTrialStrain()\n";
    }

    return res;
}

// default operation for strain rate is zero
double
UniaxialMaterial::getStrainRate(void)
{
    return 0.0;
}


// default operation for damping tangent is zero
double
UniaxialMaterial::getDampTangent(void)
{
    return 0.0;
}

// default operation for secant stiffness
/*
  double
  UniaxialMaterial::getSecant (void)
  {
  double strain = this->getStrain();
  double stress = this->getStress();

  if (strain != 0.0)
  return stress/strain;
  else
  return this->getTangent();
  }
*/

double
UniaxialMaterial::getRho(void)
{
    return 0.0;
}

UniaxialMaterial*
UniaxialMaterial::getCopy(SectionForceDeformation *s)
{
    return this->getCopy();
}

// Response*
// UniaxialMaterial::setResponse(const char **argv, int argc,
//                               ostream &theOutput)
// {
//     Response *theResponse = 0;

//     if ( (strcmp(argv[0], "stress") == 0) ||
//             (strcmp(argv[0], "tangent") == 0) ||
//             (strcmp(argv[0], "strain") == 0) ||
//             (strcmp(argv[0], "stressStrain") == 0) ||
//             (strcmp(argv[0], "stressANDstrain") == 0) ||
//             (strcmp(argv[0], "stressAndStrain") == 0) ||
//             (strcmp(argv[0], "stressStrain") == 0) ||
//             (strcmp(argv[0], "stressANDstrain") == 0) ||
//             (strcmp(argv[0], "stressAndStrain") == 0) ||
//             (strcmp(argv[0], "stressStrainTangent") == 0) ||
//             (strcmp(argv[0], "stressANDstrainANDtangent") == 0) ||
//             (strstr(argv[0], "stressSensitivity") != 0) ||
//             (strstr(argv[0], "strainSensitivity") != 0) )
//     {

//         theOutput.tag("UniaxialMaterialOutput");
//         theOutput.attr("matType", this->getClassType());
//         theOutput.attr("matTag", this->getTag());

//         // stress
//         if (strcmp(argv[0], "stress") == 0)
//         {
//             theOutput.tag("ResponseType", "sigma11");
//             theResponse =  new MaterialResponse(this, 1, this->getStress());
//         }
//         // tangent
//         else if (strcmp(argv[0], "tangent") == 0)
//         {
//             theOutput.tag("ResponseType", "C11");
//             theResponse =  new MaterialResponse(this, 2, this->getTangent());
//         }

//         // strain
//         else if (strcmp(argv[0], "strain") == 0)
//         {
//             theOutput.tag("ResponseType", "eps11");
//             theResponse =  new MaterialResponse(this, 3, this->getStrain());
//         }

//         else if (strcmp(argv[0], "plasticStrain") == 0)
//         {
//             theOutput.tag("ResponseType", "eps11");
//             theResponse =  new MaterialResponse(this, 6, this->getStrain());
//         }

//         // strain
//         else if ((strcmp(argv[0], "stressStrain") == 0) ||
//                  (strcmp(argv[0], "stressANDstrain") == 0) ||
//                  (strcmp(argv[0], "stressAndStrain") == 0))
//         {
//             theOutput.tag("ResponseType", "sig11");
//             theOutput.tag("ResponseType", "eps11");
//             theResponse =  new MaterialResponse(this, 4, Vector(2));
//         }

//         else if ((strcmp(argv[0], "stressStrainTangent") == 0) ||
//                  (strcmp(argv[0], "stressANDstrainANDtangent") == 0))
//         {
//             theOutput.tag("ResponseType", "sig11");
//             theOutput.tag("ResponseType", "eps11");
//             theOutput.tag("ResponseType", "C11");
//             theResponse =  new MaterialResponse(this, 5, Vector(3));
//         }

//         // stress sensitivity for local sensitivity recorder purpose.  Quan 2009
//         // limit:  no more than 10000 random variables/sensitivity parameters
//         else if (strstr(argv[0], "stressSensitivity") != 0)
//         {
//             char *token = strtok((char *) argv[0], " ");
//             if (token != NULL)
//             {
//                 token = strtok(NULL, " ");
//             }
//             int gradient = atoi(token);
//             theOutput.tag("ResponseType", "sigsens11");
//             theResponse =  new MaterialResponse(this, gradient + 10000, this->getStress());
//         }
//         // strain sensivitiy
//         else if (strstr(argv[0], "strainSensitivity") != 0)
//         {
//             char *token = strtok((char *) argv[0], " ");
//             if (token != NULL)
//             {
//                 token = strtok(NULL, " ");
//             }
//             int gradient = atoi(token);
//             theOutput.tag("ResponseType", "epssens11");
//             theResponse =  new MaterialResponse(this, gradient + 20000, this->getStrain());
//         }

//         theOutput.endTag();
//     }

//     return theResponse;

// }

// int
// UniaxialMaterial::getResponse(int responseID, Information &matInfo)
// {
//     static Vector stressStrain(2);
//     static Vector stressStrainTangent(3);
//     // each subclass must implement its own stuff

//     // added for sensitivity recorder. Quan 2009
//     if ((responseID > 10000) && (responseID < 20000))
//     {
//         matInfo.setDouble(this->getStressSensitivity(responseID - 10000, false));
//         return 0;
//     }
//     else if (responseID > 20000)
//     {
//         matInfo.setDouble(this->getStrainSensitivity(responseID - 20000));
//         return 0;
//     }

//     double kInit;
//     double stress;
//     double strain;

//     switch (responseID)
//     {
//     case 1:
//         matInfo.setDouble(this->getStress());
//         return 0;

//     case 2:
//         matInfo.setDouble(this->getTangent());
//         return 0;

//     case 3:
//         matInfo.setDouble(this->getStrain());
//         return 0;

//     case 6: // an approx to plastic strain
//         strain = this->getStrain();
//         stress = this->getStress();
//         kInit = this->getTangent();
//         strain = strain - stress / kInit;
//         matInfo.setDouble(strain);
//         return 0;

//     case 4:
//         stressStrain(0) = this->getStress();
//         stressStrain(1) = this->getStrain();
//         matInfo.setVector(stressStrain);
//         return 0;

//     case 5:
//         stressStrainTangent(0) = this->getStress();
//         stressStrainTangent(1) = this->getStrain();
//         stressStrainTangent(2) = this->getTangent();
//         matInfo.setVector(stressStrainTangent);
//         return 0;
//     default:
//         return -1;
//     }
// }


// // AddingSensitivity:BEGIN ////////////////////////////////////////
// double
// UniaxialMaterial::getStressSensitivity(int gradIndex, bool conditional)
// {
//     return 0.0;
// }

// double
// UniaxialMaterial::getStrainSensitivity(int gradIndex)
// {
//     return 0.0;
// }

// double
// UniaxialMaterial::getTangentSensitivity(int gradIndex)
// {
//     return 0.0;
// }

// double
// UniaxialMaterial::getInitialTangentSensitivity(int gradIndex)
// {
//     return 0.0;
// }

// double
// UniaxialMaterial::getRhoSensitivity(int gradIndex)
// {
//     return 0.0;
// }

// double
// UniaxialMaterial::getDampTangentSensitivity(int gradIndex)
// {
//     return 0.0;
// }

// int
// UniaxialMaterial::commitSensitivity(double strainSensitivity, int gradIndex, int numGrads)
// {
//     return -1;
// }

double
UniaxialMaterial::getInitialTangent (void)
{
    std::cerr << "UniaxialMaterial::getInitialTangent() -- this mehtod " << endln
              << " is not implemented for the selected material. " << endln;
    return 0.0;
}



int
UniaxialMaterial::CheckMesh( ofstream &checkmesh_file )
{
    return 0;
}
// AddingSensitivity:END //////////////////////////////////////////

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
