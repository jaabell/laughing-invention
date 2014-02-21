include ../Makefile.Compilers
include ../Makefile.Includes

#LIBS = $(PARALLEL_LIBS) $(SEQUENTIAL_LIBS) 
OBJS = feiparser.lex.o feiparser.tab.o argcheck.o feiexceptions.o
# command_suggestion.o

%.o : %.cpp
	$(CC++) $(C++FLAGS) $(INCLUDES) $(MACHINE_INCLUDES) -c $< -o $@

#  all:  makedocumentation document feiparser 
all: essi 

makedocumentation: makedocumentation.o generate_command_patterns.o
	$(LINKER) makedocumentation.o -o makedocumentation
# 	$(LINKER) generate_command_patterns.o -o generate_command_patterns
#	$(LINKER) command_suggestion.o -o command_suggestion
#          command_suggestion.o
         
document:
	./makedocumentation
# 	./generate_command_patterns

essi:  $(OBJS)
	$(LINKER) $(LINKFLAGS) \
	$(OBJS) \
	$(LIBS) \
	-o ../bin/$@


###########################################################################
cleanall:
	$(RM) $(RMFLAGS) $(OBJS) *.o core* *.out *.output 
	$(RM) $(RMFLAGS) $(OBJS) *.tex *.txt fei.tags 
	$(RM) $(RMFLAGS)  makedocumentation feiparser ../bin/essi 
	
