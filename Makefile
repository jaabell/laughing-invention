
# error:
#     @echo "Please choose one of the following targets: sequential_release, parallel_release, sequential_debug, parallel_debug"
error:
	@echo "Please run make with one of following targets: "
	@echo "   -> make sequential_release "
	@echo "   -> make parallel_release "
	@echo "   -> make sequential_debug "
	@echo "   -> make parallel_debug"
	@# Alternatively: "Please RTFI(nstall)F(ile)"
	@exit 2

sequential: sequential_release

sequential_release: 
	$(eval export Compiler = GNU)
	$(eval export DEBUG_MODE = NO_DEBUG)
	$(eval export PROGRAMMING_MODE = SEQUENTIAL)
	$(MAKE) essi
	

sequential_debug:
	$(eval export Compiler = GNU)
	$(eval export DEBUG_MODE = DEBUG)
	$(eval export PROGRAMMING_MODE = SEQUENTIAL)
	$(MAKE) essi

parallel: parallel_release

parallel_release:
	$(eval export Compiler = PARALLEL)
	$(eval export DEBUG_MODE = NO_DEBUG)
	$(eval export PROGRAMMING_MODE = PARALLEL)
	$(MAKE) essi

parallel_debug:
	$(eval export Compiler = PARALLEL)
	$(eval export DEBUG_MODE = DEBUG)
	$(eval export PROGRAMMING_MODE = PARALLEL)
	$(MAKE) essi

essi: libraries
	cd ./DSL; $(MAKE);

parser: 
	cd ./DSL; $(MAKE);

# libraries: mk_includes
libraries:
	cd ./nDarray; $(MAKE);
	cd ./ModifiedOpenSeesServices; $(MAKE);
	cd ./CompGeoMechUCD_FiniteElements; $(MAKE);
	cd ./CompGeoMechUCD_MaterialModeling; $(MAKE);
	cd ./CompGeoMechUCD_Miscellaneous; $(MAKE);
	cd ./small_utility_programs; $(MAKE);
	cd ./UCB_Elements; $(MAKE);

# .PHONY: mk_includes
# mk_includes:
# 	@echo "=============================================================="
# 	@echo "   Compiling ESSI with options:"
# 	@echo "   Compiler         = $(Compiler)"
# 	@echo "   Debug mode       = $(DEBUG_MODE)"
# 	@echo "   Programming Mode = $(PROGRAMMING_MODE)"
# 	@echo "=============================================================="
# 	$(eval include Makefile.Compilers)
# 	$(eval include Makefile.Includes)

.PHONY: clean
clean:
	$(eval export Compiler = just_cleaning)
	$(eval export DEBUG_MODE = just_cleaning)
	$(eval export PROGRAMMING_MODE = just_cleaning)
	cd ./nDarray; $(MAKE) clean;
	cd ./ModifiedOpenSeesServices; $(MAKE) clean;
	cd ./CompGeoMechUCD_FiniteElements; $(MAKE) clean;
	cd ./CompGeoMechUCD_MaterialModeling; $(MAKE) clean;
	cd ./CompGeoMechUCD_Miscellaneous; $(MAKE) clean;
	cd ./small_utility_programs; $(MAKE) clean;
	cd ./UCB_Elements; $(MAKE) clean;
	cd ./DSL; $(MAKE) clean;

.PHONY: cleanall
cleanall: clean


