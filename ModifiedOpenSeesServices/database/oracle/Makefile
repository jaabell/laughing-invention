# This is the include e that provided by ORACLE

# updated by:
# Nima Tafazzoli, November 2009, UCD

current_directory := $(shell pwd)
ModifiedOpenSeesServicesdir := $(current_directory)/../..

include $(ORACLE_HOME)/precomp/lib/env_precomp.mk
include ../../../Makefile.Includes
include ../../../Makefile.Compilers


# My definitions of Macros
MY_CFLAGS=$(GFLAG) $(OPTIMIZE) $(CDEBUG) $(CCFLAGS) $(QACCFLAGS) $(PFLAGS)\
	$(SHARED_CFLAG) $(USRFLAGS)
MYC++		= /usr/pubsw/bin/c++
MYCC		= /opt/SUNWspro/bin/cc
MYCC_INCLUDE	= $(I_SYM). $(PRECOMPPUBLIC) -I$(ORACLE_HOME)/precomp/public
PROCFLAGS	= CODE=CPP PARSE=PARTIAL CPP_SUFFIX=cc include=/usr/include \
include		= /opt/SUNWspro/WS6/include/CC
PRODUCT_LIBHOME	= /usr/pubsw/apps/oracle/8.1.7/lib

# Macros for Lex & Yacc.
LEXLIBS = -ly -ll
LEX = flex
YACC = yacc

OBJS	= FE_Datastore.o FileDatastore.o DB_Datastore.o TclDatabaseCommands.o \
	  DBmethods.o

# Compilation control

lib:	$(OBJS)

all:	$(OBJS) TokenMatch.o 
	g++ $(LINKFLAGS) TokenMatch.o  DBmethods.o \
	FE_Datastore.o DB_Datastore.o \
	$(FE_LIBRARY) $(MACHINE_LINKLIBS) \
	$(MACHINE_NUMERICAL_LIBS) $(MACHINE_SPECIFIC_LIBS) $(METIS_LIBRARY) \
	$(PROLDLIBS) -o mytest -L/export/junpeng/xerces/lib -lxerces-c1_5_1 -lc
#
#	-L$(LIBHOME) $(PROLDLIBS) -R$(PRODUCT_LIBHOME) -o mytest


xmlTest:  xmltest.o
	$(LINKER) $(LINKFLAGS) xmltest.o  DBmethods.o \
	FE_Datastore.o DB_Datastore.o \
	$(FE_LIBRARY) $(MACHINE_LINKLIBS) \
	$(MACHINE_NUMERICAL_LIBS) $(MACHINE_SPECIFIC_LIBS) $(METIS_LIBRARY) \
	$(PROLDLIBS)  -o mytest -L/export/junpeng/xerces/lib -lxerces-c1_5_1 -lc

dqlyacc.c dqlyacc.h:	dqlyacc.y
	${YACC} -vd dqlyacc.y
	mv y.tab.h dqlyacc.h
	mv y.tab.c dqlyacc.c
	mv y.output dqlyacc.out

dqllex.o:	dqllex.l 


.SUFFIXES: 	.pc .c .o .pgm .l .y

.pc.c:
	$(PROC) iname=$*.pc
.c.o:
	$(MYCC) $(MY_CFLAGS) -c $*.c
.l.o:
	${LEX} $<
	mv lex.yy.c $*.c
	${MYCC} -o $@ $*.c ${LEXLIBS}


# Miscellaneous
tidy:	
	@$(RM) $(RMFLAGS) Makefile.bak *~ *.cc #*# core

clean: tidy
	@$(RM) $(RMFLAGS) $(OBJS) *.o 

spotless: clean

wipe: spotless

# DO NOT DELETE THIS LINE -- make depend depends on it.
