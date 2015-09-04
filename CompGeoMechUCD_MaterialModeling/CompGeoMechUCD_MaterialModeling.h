///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               January 2014
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


//Base classes for materials
#include "./NDMaterial.h"
#include "./LTensor_Based/NDMaterialLT.h"


//Accelerated
//******************** Accelerated Constitutive Models ***********************
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/ConstitutiveMaterialParameter.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesPP.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesIH.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesKH.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/vonMisesLinearKH.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerPP.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerIH.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/DruckerPragerKH.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/CamClay.h"
#include "../CompGeoMechUCD_MaterialModeling/Accelerated3Dep/Pisano_model.h"

//New template materials
#include "./NewTemplate3Dep/NewTemplate3Dep.h"
#include "./NewTemplate3Dep/MaterialParameter.h"
#include "./NewTemplate3Dep/ElasticState.h"
#include "./NewTemplate3Dep/Isotropic_Elastic.h"
#include "./NewTemplate3Dep/elnp_Elastic.h"
#include "./NewTemplate3Dep/DM04_Elastic.h"
#include "./NewTemplate3Dep/YieldFunction.h"
#include "./NewTemplate3Dep/PlasticFlow.h"
#include "./NewTemplate3Dep/ScalarEvolution.h"
#include "./NewTemplate3Dep/Linear_Eeq.h"
#include "./NewTemplate3Dep/TensorEvolution.h"
#include "./NewTemplate3Dep/Linear_Eij.h"
#include "./NewTemplate3Dep/AF_Eij.h"
#include "./NewTemplate3Dep/ElasticState.h"
#include "./NewTemplate3Dep/Isotropic_Elastic.h"
#include "./NewTemplate3Dep/elnp_Elastic.h"
#include "./NewTemplate3Dep/DP_YF.h"
#include "./NewTemplate3Dep/VM_YF.h"
#include "./NewTemplate3Dep/RMC_YF.h"
#include "./NewTemplate3Dep/CC_YF.h"
#include "./NewTemplate3Dep/DM04_YF.h"
#include "./NewTemplate3Dep/DP_PF.h"
#include "./NewTemplate3Dep/VM_PF.h"
#include "./NewTemplate3Dep/RMC_PF.h"
#include "./NewTemplate3Dep/CC_PF.h"
#include "./NewTemplate3Dep/DM04_PF.h"
#include "./NewTemplate3Dep/Linear_Eeq.h"
#include "./NewTemplate3Dep/CC_Ev.h"
#include "./NewTemplate3Dep/Linear_Eij.h"
#include "./NewTemplate3Dep/AF_Eij.h"
#include "./NewTemplate3Dep/DM04_alpha_Eij.h"
#include "./NewTemplate3Dep/DM04_z_Eij.h"
#include "./NewTemplate3Dep/SANISAND_p0_bar.h"
#include "./NewTemplate3Dep/SANISAND_alpha_Eij.h"
#include "./NewTemplate3Dep/SANISAND_PF.h"
#include "./NewTemplate3Dep/SANISAND_YF.h"
#include "./NewTemplate3Dep/SANISAND_Elastic.h"
#include "./NewTemplate3Dep/SANISAND_z_Eij.h"
#include "./NewTemplate3Dep/PressureDependent_Elastic.h"


//LTensor based material models
#include "./LTensor_Based/vonMisesPPLT.h"
#include "./LTensor_Based/PisanoLT.h"
#include "./LTensor_Based/NewPisanoLT.h"
#include "./LTensor_Based/ElasticIsotropic3DLT.h"

//Other materials
#include "./ElasticIsotropic3D.h"
#include "./ElasticCrossAnisotropic.h"


//New Template-Template Classic Elastoplastic Models
#include "ClassicElastoplasticMaterial/AllCEMModels.h"

//Constitutive testing
#include "./BardetConstraint.h"
#include "./BardetConstraintLT.h"