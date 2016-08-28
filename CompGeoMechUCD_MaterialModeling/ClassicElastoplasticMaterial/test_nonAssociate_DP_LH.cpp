
#include "ClassicElastoplasticMaterial.h"
// #include "ConstitutiveModels/VonMisesLinearHardening.h"
// #include "ConstitutiveModels/DruckerPragerNonAssociateArmstrongFrederick.h"
#include "ConstitutiveModels/DruckerPragerNonAssociateLinearHardening.h"

// #include "myReader.h"

using namespace std;

void print_this_step(const int &printStep, NDMaterialLT* test_material){
	// print the input strain:
	cout<<"-----step "<<printStep<<" start------"<<endl;
	auto input_strain = test_material -> getStrainTensor();
	cout<<"The current strain: "<<endl;
	cout<<input_strain<<endl;

	// Output the calculated stress
	auto stress_result = test_material -> getStressTensor();
	cout<<"The output stress: "<<endl;
	cout<<stress_result<<endl;
	cout<<"-----step end------"<<endl;
}

int main(){


	// The corresponding constructor for reference:
	// VonMisesLinearHardening(int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu, double rho_);

	// Define and input the material properties. 
	int    material_tag             =1;
	double isotropic_hardening_rate =0;
	double elastic_modulus          =100;
	double poisson_ratio            =0.0;
	double density                  =0.0;
	double drucker_prager_angle_k   =2;
	double kinematic_hardening_rate=100;   
	// double kinematic_hardening_AF_cr=100; 
	double initial_confining_stress =5.0/3.0;  
	double nonAssociate_plastic_flow_xi=0.5;
	double nonAssociate_plastic_flow_Kd=0.2;

	// DruckerPragerNonAssociateArmstrongFrederick(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double E, double nu, double rho_, double p0, double xi, double Kd)

	auto DP_NonAssoc_LH= new DruckerPragerNonAssociateLinearHardening(
		material_tag,
		drucker_prager_angle_k,
		kinematic_hardening_rate,
		isotropic_hardening_rate,
		elastic_modulus,
		poisson_ratio,
		density, 
		initial_confining_stress,
		nonAssociate_plastic_flow_xi,
		nonAssociate_plastic_flow_Kd
		);


 	// Set the integration rule and tolerance to the material:
	int method = (int) NDMaterialLT_Constitutive_Integration_Method::Forward_Euler;
	double f_relative_tol=0.001;
	double stress_relative_tol=0.001;
	double n_max_iterations=100;

    NDMaterialLT::set_constitutive_integration_method(
    	method,
     	f_relative_tol,
     	stress_relative_tol,
     	n_max_iterations);

    // Output the parameters:
    cout<<"----------Start Testing---------"<<endl;
    cout<<"The input von Mises material parameters:"<<endl;
    cout<<"drucker_prager_angle_k   : "<<drucker_prager_angle_k   <<endl;
    cout<<"kinematic_hardening_rate : "<<kinematic_hardening_rate<<endl;
    cout<<"isotropic_hardening_rate : "<<isotropic_hardening_rate<<endl;
	cout<<"elastic_modulus          : "<<elastic_modulus<<endl;
	cout<<"poisson_ratio            : "<<poisson_ratio<<endl;
	cout<<"density                  : "<<density<<endl;
	cout<<"initial_confining_stress : "<<initial_confining_stress<<endl;
	cout<<"nonAssociate_plastic_flow_xi : "<<nonAssociate_plastic_flow_xi<<endl;
	cout<<"nonAssociate_plastic_flow_Kd : "<<nonAssociate_plastic_flow_Kd<<endl;
	cout<<"-------------------"<<endl;
	short mystep=0;
	cout<<"The initial state:"<<endl;
	auto initial_strain = DP_NonAssoc_LH -> getStrainTensor();
	cout<<"The initial strain state: "<<endl;
	cout<<initial_strain<<endl;

	auto initial_stress = DP_NonAssoc_LH -> getStressTensor();
	cout<<"The initial stress state: "<<endl;
	cout<<initial_stress<<endl;
	cout<<"-------------------"<<endl;
	
	// Step 1: elastic loading:
	// Define and input the strain to the material
	DTensor2 myinputStrain( 3, 3, 0.0 );
	myinputStrain(0,0)=0.02/sqrt(7);
	myinputStrain(0,1)=-0.02/sqrt(7);
	myinputStrain(1,0)=-0.02/sqrt(7);
	DP_NonAssoc_LH->setTrialStrainIncr(myinputStrain);
	DP_NonAssoc_LH->commitState();
	print_this_step(++mystep,DP_NonAssoc_LH);
	
	// Step 2: plastic loading:
	// Define and input the strain to the material
	// myinputStrain(0,0)= 0.01*(-1+sqrt(2.0/3.0));
	// myinputStrain(1,1)=-0.0025;
	for(int step=0; step<20; ++step){
		myinputStrain(0,0)=0.0;
		myinputStrain(1,0)=0.0;
		myinputStrain(0,1)=-0.01;
		DP_NonAssoc_LH->setTrialStrainIncr(myinputStrain);
		DP_NonAssoc_LH->commitState();
		print_this_step(++mystep,DP_NonAssoc_LH);
	}


	// // Step 3-5:  unloading:
	// for (short ll=0;ll<3;++ll){
	// 	myinputStrain(0,0)=-0.01;
	// 	// myinputStrain(1,1)=0.0;
	// 	// myinputStrain(2,2)=myinputStrain(1,1);
	// 	DP_NonAssoc_LH->setTrialStrainIncr(myinputStrain);
	// 	DP_NonAssoc_LH->commitState();
	// 	print_this_step(++mystep,DP_NonAssoc_LH);
	// }

	// // Step 6: plastic loading in the other direction:
	// myinputStrain(0,0)=-0.01;
	// // myinputStrain(1,1)=0.01;
	// // myinputStrain(2,2)=myinputStrain(1,1);
	// DP_NonAssoc_LH->setTrialStrainIncr(myinputStrain);
	// DP_NonAssoc_LH->commitState();
	// print_this_step(++mystep,DP_NonAssoc_LH);


	// // Step 7-9:  unloading:
	// for (short ll=0;ll<3;++ll){
	// 	myinputStrain(0,0)=0.01;
	// 	// myinputStrain(1,1)=0.0;
	// 	// myinputStrain(2,2)=myinputStrain(1,1);
	// 	DP_NonAssoc_LH->setTrialStrainIncr(myinputStrain);
	// 	DP_NonAssoc_LH->commitState();
	// 	print_this_step(++mystep,DP_NonAssoc_LH);
	// }



	return 0;




}