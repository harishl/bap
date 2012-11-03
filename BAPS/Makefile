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
GP = $(LIB_DIR)/libGP.a
TV = $(LIB_DIR)/libTV.a
PART = $(GP)

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
LIBS = -lSolver -lBP -lGP -lBAPS -lleda -lX11
#LIBS = -lSolver -lBP -lTV -lBAPS -lleda -lX11
BAPS = $(BIN_DIR)/BAPS

.cpp.o:
	@echo "starting from this line"
	$(CC) $(CCFLAGS) $(INCL_DIR) -c $<


###############################
# BAPS v2.0 Main Program
###############################
$(BAPS) : $(OBJS)
	@echo "	compiling/linking BAPS 2.0"
	cd src; make baps; cd ..
	$(CC) $(CCFLAGS) src/baps.o -o $@ -I$(INCL_DIR) -L$(LIB_DIR) -L$(LFLAGS) $(LIBS)


###############################
# Partitioning modules
###############################
$(GP) :
	@echo "	checking GP $(GP)"
	cd partitioning/GP; make; cd ../..
	
$(TV) :
	@echo "	checking tv $(TV)"
	cd partitioning/TV; make; cd ../..


###############################
# Packing modules
###############################
$(BP) :
	@echo "	checking bp $(BP)"
	cd packing/BP; make; cd ../..

###############################
# BAPS v2.0 modules
###############################
$(SOLVER) :
	@echo "	checking solver $(SOLVER)"
	cd src; make solver; cd ..

$(BAPS_LIB) :
	@echo "	checking $(BAPS_LIB)"
	cd src; make; cd ..


tar:
	cd ..; rm -f baps2.0.tgz; tar zc --exclude *.o --exclude *.a --exclude *.sol --exclude BAPS -f baps2.0.tgz BAPSv2.0; cd BAPSv2.0


clean:
	@echo "  cleaning $(BAPS_LIB) and $(SOLVER)"
	cd src; make clean; cd ../lib; make clean; cd ..
	rm -rf *~ *.bak *.out *.o core
	@echo "  cleaning $(GP)"
	cd partitioning/GP; make clean; cd ../..
	@echo "  cleaning $(TV)"
	cd partitioning/TV; make clean; cd ../..
	@echo "  cleaning $(BP)"
	cd packing/BP; make clean; cd ../..

check:
	test/test.sh
