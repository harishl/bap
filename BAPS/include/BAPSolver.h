/******************************************************************
 *
 * Filename    : BAPSolver.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.01b
 * Date        : 29 May 98
 *
 * Description : Interface file for problem solver class.
 *
 * Reference   : nil
 *
 * Notes       : This class calls the BAP partitioning and packing
 *               algorithms.  It serves as an interface between the
 *               BAP Package class and the BAP algorithms.
 *
 * Changes     : nil
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 ******************************************************************/


#ifndef __BAP_SOLVER__
#define __BAP_SOLVER__

#include "def.h"
#include "BAPBase.h"
#include "BAPPackage.h"
#include "../partitioning/GP/BAPGPPartitioner.h"
#include "../partitioning/TV/BAPTVPartitioner.h"
#include "../packing/BP/BAPBPPacker.h"

class BAPSolver : public BAPBase
{
public:
   BAPSolver(BAPPackage& aPackage, string aJob = "all");
   ~BAPSolver();

   //
   // Debugging facility
   //
   // prints information on the current solver with the given
   // indentation width and at the given detail level
   void  Print(int aWidth = 1, int aDetail = 0) const;

   //
   // Accessor member functions
   //
   // returns the BAP package associated with this solver
   BAPPackage& Package() const;

   //
   // Modifier member functions
   //
   // activates the solution to the BAP problem
   void  Solve();
   void  DoPartitioning();
   void  DoPacking();
   void  ReadParameterFile();

private:
   BAPPackage& mPackage;
   // Possible values:
   //    All  = run partitioning and packing
   //    Part = run partitioning only and stop
   //    Pack = read partitioning solution and run packing only
   string      mJob;
   string      mPartitioner;
   string      mPacker;
};


#endif

