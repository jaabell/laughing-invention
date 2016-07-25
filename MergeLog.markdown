<!--
This file is in "markdown" format (http://en.wikipedia.org/wiki/Markdown). 
It is easy to read but can be translated into better looking HTML via a tool
such as python markdown script (https://pypi.python.org/pypi/Markdown)

On ubuntu: sudo apt-get install markdown

And do:

markdown MergeLog.markdown > MergeLog.html

Then view MergeLog.html in a browser. :)
-->

Sequential and Parallel FEI Merge Log
=====================================

##### Document prepared by [Jose Abell](mailto:jaabell@ucdavis.edu) in Jan 2014.

This log summarizes the merging of branches *jaabell* and *babak* on the repository
`fei.git` in `git@powaqqatsi.engr.ucdavis.edu:fei.git`. 


**(!) This document is a UCD CompGeoMech internal document and it's distribution is prohibited.**

##Table of Contents

[TOC]

##Merge Log by folder

 fei branch *jaabell* and *babak* into will be merged into a new git repository. After this, 
 all history will be available in the old `FEI` repo to be renamed `feiold`.

 The strategy employed to merge these two was to start from two clones of either branches at their
 state as of Jan 22, 2014 by issuing

	git clone git@powaqqatsi.engr.ucdavis.edu:fei.git -b jaabell fei/
	git clone git@powaqqatsi.engr.ucdavis.edu:fei.git -b babak babak_fei/

Then use the program [meld](http://meldmerge.org/) to view changes folder by folder. Changes
were merged at discretion of the author in order to preserve the intended workflow for the
program and minimally impact compilation of this report.

It is expected that the code compiles and works properly after these changes as the
new repo is created.

Henceforth the following definitions apply:

>branch *jaabell* will be called **j**  
>branch *babak*   will be called **b** 

For each subfolder of the main program source root, the changes are logged and explained 
where necessary below.

###In `ARPACK/`:
  
  In **j**: the compiler (FC) used in the Makefile was changed to fortran -std=legacy
  whereas in **b**  compiler was g77.

 `Merge decision`:  Kept **j** version.


###In `BLAS/`:
 This folder is not present in **j**.
 
 `Merge decision`:  Do not include folder as is probably unnecesary.



###In `boost/`:
 In **b** some headers were referenced using relative paths, "../"

 `Merge decision`:  Kept **b** version.





###In `CBLAS/`:
 No differences between **j** and **b**.

 `Merge decision`:  Kept **j** (no differences)




###In `C_Binding_Commands/`:

Mostly older versions of API contained in **b** which had a void return. But, 
also some new ones for parallel analysis. 

`Merge decision`:  Kept **j**, incorporated changes from **b** as needed
                      with additional comments below.

* `C_Binding_Commands/MODELING`

 + `start_new_stage`: **j** and **b** versions are incompatible. **j** defaults the
   numbebrer to RCM while **b** does not define a numberer (needs to ne defined elsewhere).
    Kept both and renamed **b**'s version `start_new_stage_parallel` and **j** version
    `start_new_stage_sequential`.

 + `wipe_model`: Same as above with similar renaming.

 + **Missing files**. These files were in **b** but not in **j**. The reason these were 
   left out is because the have been renamed in **j** or are no longer needed due
   to the suggested workflow of the program. What follows is the dirdiff log.

Dirdiff command:

	dirdiff newfei/C_Binding_Commands/MODELING/ babak_fei/C_Building_Commands/MODELING/ | grep "Only in babak"
	
Dirdiff output after removing the `Only in babak_fei/C_Building_Commands/MODELING/: `

	01.h
	add_body_force.h
	add_constitutive_model_NDMaterial_dafalias_manzari.h
	add_constitutive_model_NDMaterial_sanisand.h
	add_displacement_time_history_ormsby_wavelet.h
	add_element_beam_displacement_based_2d.h
	add_element_beam_displacement_based_3d.h
	add_element_beam_elastic_3d.h
	add_element_beam_elastic_to_connect_to_Solid.h
	add_element_beam_force_based_2d.h
	add_element_beam_force_based_3d.h
	add_element_beam_linear_3d_transformation.h
	add_element_beam_rank_one_deficient_elastic_pinned_fixed_beam.h
	add_element_body_force.h
	add_element_brick_27node_parallel.h
	add_element_brick_8node_Parallel.h
	add_element_brick_selfweight.h
	add_element_contact_3dof_to_6dof.h
	add_element_contact_6dof_to_6dof.h
	add_element_nonlinear_two_functions_contact_3dof_to_3dof.h
	add_element_nonlinear_two_functions_contact_3dof_to_6dof.h
	add_element_nonlinear_two_functions_contact_6dof_to_6dof.h
	add_element_truss_element.h
	add_equaldof_to_two_nodes_from_file.h
	add_force_time_history_ormsby_wavelet.h
	add_force_time_history_ricker1st_wavelet.h
	add_force_time_history_ricker2nd_wavelet.h
	add_force_time_history_sine_wave.h
	add_from_file_damping_to_element.h
	add_load_constant_pattern.h
	add_load_linear_pattern.h
	add_load_path_series_pattern.h
	add_load_sinusoidal_pattern.h
	add_material_NDMaterial_vonmises_isotropic_hardening.h
	add_material_NDMaterial_vonmises_isotropic_hardening_TDep.h
	add_material_NDMaterial_vonmises_perfectly_plastic.h
	add_multiple_support_excitation_pattern.h
	add_ormsby_wavelet_for_multisupport_excitation.h
	add_raileigh_damping_to_element_from_file.h
	add_sine_wave_for_uniform_excitation.h
	add_sinusoidal_load_pattern.h
	add_support_to_nodes_from_file.h
	add_to_node_equaldof_to_two_nodes.h
	add_to_node_load.h
	add_to_node_mass.h
	add_to_node_single_point_constraint.h
	add_to_node_support.h
	add_to_node_support_to_all_dofs.h
	define_constraint_handler_penalty_for_analysis.h
	define_constraint_handler_plain_for_analysis.h
	define_constraint_handler_transformation_for_analysis.h
	remove_equaldof_node.h
	remove_load_pattern.h
	remove_support_for_node_by_fixity_number.h
	remove_support_for_node_by_node_number.h 

* `C_Binding_Commands/SIMULATION`
	
	+ Many files have been renamed, e.g. those names `analyze` changed for `simulate` and so on. 
	+ Similar changes as in `MODELLING` directory. Some functions returned `void` and were
	upgraded to return `int`.

The following files were at the root of `C_Binding_Commands` and were moved to SIMULATION.

	define_parallel_integrator_displacementcontrol_for_static_analysis.h
	define_parallel_numberer_plain_for_analysis.h
	define_parallel_numberer_rcm_for_analysis.h
	define_solver_umfpack_for_analysis.h
	parallel_analyze_static_multistep.h

Dirdiff command:

	dirdiff newfei/C_Binding_Commands/SIMULATION/ babak_fei/C_Building_Commands/SIMULATION/ | grep "Only in babak"

Dirdiff output:

	Only in babak_fei/C_Building_Commands/SIMULATION/: 
		add_nodes_from_file.h
		analyze_static_multistep.h
		analyze_static_onestep.h
		analyze_transient_multistep.h
		analyze_transient_onestep.h
		analyze_transient_variable_multistep.h
		analyze_transient_variable_onestep.h
		define_algorithm_linear_for_analysis.h


###In `CLAPACK/`:
Directories identical. 

 `Merge decision`:  Keep **j**.



###In `CompGeoMechUCD_FiniteElements/`:
 
`Merge decision`:  detailed below for each subfolder of `CompGeoMechUCD_FiniteElements/`

 * `./Contact_Elements`: 
 	Kept **b** changes in `ContactElement_3DOF_3DOF`, but kept **j** makefile.
 * `./coupled_u-p_brick_8_nodes/`: 
 	Kept **j** changes, mostly change `opserr` to `cerr`. But also Nima removed pressure from constructor.
 * `./coupled_u-p_large_deformation_brick_8_nodes/`:
 	Same as above.
 * `./coupled_u-p-U_brick_20_nodes/`:
 	Same as above.
 * `./coupled_u-p-U_brick_8_nodes/`:
 	Same as above.
 * `./elastic_beam/`:
 	Huge changes in **j** kept them. Basically removed external coordinate transformation and made it part of the element. 
 * `./elastic_beam_to_connect_to_solid/`:
 	*empty folder* Deleted!
 * `./penalty_element/`:
 	Kept **b** code but **j** makefile.
 * `./penalty_element_apply_displacement/`:
 	Kept **b** but replaced all `opserr` with `cerr`, also kept **j** makefile. Why is there no `.h` file here? 
 * `./rank_one_deficient_elastic_pinned_fixed_beam/`:
 	Same changes as beam. **j** version kepts.
 * `./shell_andes/`:
 	**To be done** Folder changed names from `./shell_andes_3_nodes/` to this between **j** and **b** so must merge them. **See note afterward**
 * `./shell_andes_3_nodes/`:
 	Kept **b** version, must meld with `./shell_andes/` **See note afterward**
 * `./shell_felippa/`:
 	Folder deleted! No longer needed.
 * `./solid_brick_20_nodes/`:
 	Kept **j**
 * `./solid_brick_20_nodes_elastic/`:
 	Kept **j**
 * `./solid_brick_20_nodes_variable_number_of_gauss_points/`:
 	Kept **j**
 * `./solid_brick_27_nodes/`:
 	Kept **j**
 * `./solid_brick_27_nodes_elastic/`:
 	Kept **j**
 * `./solid_brick_27_nodes_variable_number_of_gauss_points/`:
 	Kept **j**
 * `./solid_brick_8_nodes/`:
 	Kept **j**
 * `./solid_brick_8_nodes_elastic/`:
 	This replaces `EightNodeBrickElastic` in **b**
 * `./solid_brick_8_nodes_ltensor/`:
 	New folder.
 * `./solid_brick_8_nodes_variable_number_of_gauss_points/`:
 	New folder.
 * `./solid_brick_large_deformation_total_Lagrangian_20_nodes/`:
 	Kept **j**
 * `./solid_brick_large_deformation_total_Lagrangian_8_nodes/`:
 	Kept **j**
 * `./solid_brick_variable_nodes/`:
 	Kept **j**
 * `./solid_brick_variable_nodes_8_to_27/`:
 	Kept **j**
 * `./Makefile`:
 	Kept **j** makefile.

**Merge note for Andes Shell:** 

* `FourNodeAndesShell.cpp`: In general kept **j** version but added some changes by **b** regarding `recvSelf()` and `sendSelf()` functions. Also added a `fillM()` function needed in **b**.
* `FourNodeAndesShell.h`: Added `fillM()` function definition.

###In `CompGeoMechUCD_MaterialModeling/`: 

All files different! Mainly because of change from `opserr` to `cerr` but may be some others.
In general there have been a lot of bugfixes and additions since **j** and **b** branched out which were captured in **j**. 

 `Merge decision`:  Kept everything from **j**. Awaiting problems to arise to see if this was wrong.



###In `CompGeoMechUCD_Miscellaneous/`:

Nonexistent in **b**.
  
`Merge decision`:  Kept from **j**.



###In `CompGeoMechUCD_PEP/`:

Old PEP.

`Merge decision`:  Kept out awaiting new implementation.




###In `CompGeoMechUCD_SSEPFEM/`:

Olde SSEPFEM

`Merge decision`:  Kept out awaiting new implementation.




###In `CPP_EXAMPLES/`:

This folder was completely disjoint in its contents with no conflicts.

`Merge decision`:  Merge both sets. 



###In `DSL/`:
 
`Merge decision`:  Taken from **j**. Must check in the future for compatibility issues.



###In `gsl`:

Apparently not needed. 

`Merge decision`:  Do not include for the meantime.


###In `ITPACK`:

Apparently not needed. 

`Merge decision`:  Do not include for the meantime.

###In `lmfit`:

Apparently not needed. 

`Merge decision`:  Do not include for the meantime.


###In `ltensor`:

Not in **b**

`Merge decision`:  Included





###In `ModifiedOpenSeesServices/`:

Most of files flagged as different due to change from `opserr` to `cerr`. 
Also some changes in Makefile, to include `Makefile.Compilers` and such.
Additional changes by **b** were included in the code.

`Merge decision`:  By folder, any additional comments are added.

 * `./domain`: 

   +  `domain/pattern/Linear_Force_TimeHistory.h`: The constructor changed significantly between versions. **j** requests onlt the final value while **b** asks for `startTime`, `finishTime`, and `slope`. Kept **b** (replacing `opserr` for `cerr`) and left **j** version as `Linear_Force_TimeHistory_Alternative`.

 * `./element`: 

 * `./graph`: 

 * `./handler`: 

 * `./material`: 

 * `./matrix`: 

 * `./modelbuilder`: 

 * `./not_in_use`: 

 * `./recorder`: 

 * `./system_of_eqn`: 

 * `./tagged`: 

 * `./utility`: 





###In ``:


`Merge decision`:  




###In ``:


`Merge decision`:  



###In `nDarray/`:

Main thing was to keep Nima's additions in **j** and the comment out of
the vector typdef in `basics.h` lines 66-72 which defined a macro that replaced
`matrix` with `BJmatrix` and `vector` with `BJvector`. 

`Merge decision`:  Keep **j** and incorporate changes from **b**.



###In ``:


`Merge decision`:  


###In ``:


`Merge decision`:  




###In `root (./)`:



`Merge decision`:  