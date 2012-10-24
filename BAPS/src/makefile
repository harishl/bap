LROOT=/home/harish/bapm3/LEDA
CC = /usr/bin/g++
CCFLAGS = -O2
IFLAGS = -I${LROOT}/incl
LFLAGS = -L${LROOT}
AR = ar rvs
BAPHOME=/home/harish/bapm3/BAPS
LIB_DIR = $(BAPHOME)/lib
INCL_DIR = $(BAPHOME)/include

LIB = $(LIB_DIR)/libBAPS.a

OBJS = BAPBase.o BAPPackage.o BAPPacker.o BAPPartitioner.o util.o

.SUFFIXES: .o .cpp

.cpp.o:
	$(CC) $(IFLAGS) $(CCFLAGS) -I$(INCL_DIR) -c $< $(LFLAGS)

$(LIB) : $(OBJS)
	$(AR) $(LIB) $?
	@echo "$(LIB) is updated"

solver :
	$(CC) $(IFLAGS) $(CCFLAGS) -I$(INCL_DIR) -L$(LIB_DIR) -c BAPSolver.cpp $(LFAGS)
	$(AR) $(LIB_DIR)/libSolver.a BAPSolver.o

baps : baps.o
	@echo "	BAPS main program compiled"

clean:
	rm -rf $(LIB) *~ *.bak *.out *.o core

