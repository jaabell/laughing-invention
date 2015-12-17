#Boris Jeremic 06Mar2009

PWD := $(shell pwd)

PATH_TO_CompGeoMechUCD_FiniteElements  = $(PWD)

include ../Makefile.Compilers

INCLUDES  = -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/matrix \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/tagged \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/material \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/material/nD \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/recorder \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/recorder/response \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/handler \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/actor \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/analysis \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/actor/channel \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/actor/actor \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/element \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain/domain \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain/load \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain/damping \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain/partitioner \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../ModifiedOpenSeesServices/domain/component \
            -I$(PATH_TO_CompGeoMechUCD_FiniteElements)/../nDarray \

.SUFFIXES:
.SUFFIXES:.o .cpp

.DEFAULT:
	@$(ECHO) "Unknown target $@"

.cpp.o:
	$(CC++) $(C++FLAGS) $(INCLUDES) -c $<



# all: $(OBJS)
all:
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/Contact_Elements; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p_brick_8_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_20_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_8_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_ltensor; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_elastic; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_elastic; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_ltensor; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_ltensor; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_elastic; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/elastic_beam; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/rank_one_deficient_elastic_pinned_fixed_beam; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/shell_andes; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_variable_nodes_8_to_27; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_variable_number_of_gauss_points; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_variable_number_of_gauss_points; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_variable_number_of_gauss_points; $(MAKE);
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/nonlinear_shear_beam; $(MAKE);
	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_element; $(MAKE);
	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_element_apply_displacement; $(MAKE);
#	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p_large_deformation_brick_8_nodes; $(MAKE);
#	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_large_deformation_total_Lagrangian_20_nodes; $(MAKE);



# Miscellaneous

clean:
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/Contact_Elements; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p_brick_8_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_20_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_8_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_ltensor; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_elastic; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_elastic; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_ltensor; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_ltensor; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_elastic; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/elastic_beam; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/rank_one_deficient_elastic_pinned_fixed_beam; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_variable_nodes_8_to_27; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_variable_number_of_gauss_points; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_variable_number_of_gauss_points; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_variable_number_of_gauss_points; $(MAKE) clean;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/nonlinear_shear_beam; $(MAKE) clean;

	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_element; $(MAKE) clean;
	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/shell_andes_3_nodes; $(MAKE) clean;
	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_ele1ment_apply_displacement; $(MAKE) clean;

cleanall:
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/Contact_Elements; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p_brick_8_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_20_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/coupled_u-p-U_brick_8_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_ltensor; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_elastic; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_elastic; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_ltensor; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_ltensor; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_elastic; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/elastic_beam; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/rank_one_deficient_elastic_pinned_fixed_beam; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/shell_andes; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_variable_nodes_8_to_27; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_8_nodes_variable_number_of_gauss_points; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_20_nodes_variable_number_of_gauss_points; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/solid_brick_27_nodes_variable_number_of_gauss_points; $(MAKE) cleanall;
	$(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/nonlinear_shear_beam; $(MAKE) cleanall;


	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_element; $(MAKE) cleanall;
	# $(CD) $(PATH_TO_CompGeoMechUCD_FiniteElements)/penalty_element_apply_displacement; $(MAKE) cleanall;
