#
# BAPS v2.1 Makefile
# DOTW 25 Apr 2000
# Last updated by Francis Ng - 21 Sep 2007
#   following changes in sunfire's leda and gcc path
#

###############################
# Compiler stuff
###############################
CC = /usr/bin/g++
CCFLAGS = -O2
LFLAGS = /home/harish/bapm3/LEDA


###############################
# BAPS Dependencies
###############################
BIN_DIR = $(BAPHOME)/bin
LIB_DIR = $(BAPHOME)/lib
INCL_DIR = $(BAPHOME)/include

###
# Partitioning modules
###
TV = $(LIB_DIR)/libTV.a
PART = $(TV)

###
# Packing modules
###
BP = $(LIB_DIR)/libBP.a
PACK = $(BP)

###
# Solver module
###
SOLVER = $(LIB_DIR)/libSolver.a

###
# BAPS modules
###
BAPS_LIB = $(LIB_DIR)/libBAPS.a

OBJS = $(PART) $(PACK) $(SOLVER) $(BAPS_LIB)
###
# DOTW: ordering of libraries here VERY important!
#       must be: -lSolver -lBP -lTV -lBAPS -lG -lL -lm
#       else cannot link properly
###
LIBS = -lSolver -lBP -lTV -lBAPS -lleda -lX11
BAPS = $(BIN_DIR)/BAPS

.cpp.o:
	@echo "starting from this line"
	$(CC) $(CCFLAGS) $(INCL_DIR) -c $<


###############################
# BAPS v2.0 Main Program
###############################
$(BAPS) : $(OBJS)
	@echo "	compiling/linking BAPS 2.0"
	cd src; dmake baps; cd ..
	$(CC) $(CCFLAGS) src/baps.o -o $@ -I$(INCL_DIR) -L$(LIB_DIR) -L$(LFLAGS) $(LIBS)


###############################
# Partitioning modules
###############################
$(TV) :
	@echo "	checking tv $(TV)"
	cd partitioning/TV; dmake; cd ../..


###############################
# Packing modules
###############################
$(BP) :
	@echo "	checking bp $(BP)"
	cd packing/BP; dmake; cd ../..

###############################
# BAPS v2.0 modules
###############################
$(SOLVER) :
	@echo "	checking solver $(SOLVER)"
	cd src; dmake solver; cd ..

$(BAPS_LIB) :
	@echo "	checking $(BAPS_LIB)"
	cd src; dmake; cd ..


tar:
	cd ..; rm -f baps2.0.tgz; tar zc --exclude *.o --exclude *.a --exclude *.sol --exclude BAPS -f baps2.0.tgz BAPSv2.0; cd BAPSv2.0


clean:
	@echo "  cleaning $(BAPS_LIB) and $(SOLVER)"
	cd src; dmake clean; cd ../lib; dmake clean; cd ..
	rm -rf *~ *.bak *.out *.o core
	@echo "  cleaning $(TV)"
	cd partitioning/TV; dmake clean; cd ../..
	@echo "  cleaning $(BP)"
	cd packing/BP; dmake clean; cd ../..

check:
	test/test.sh
