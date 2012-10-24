/******************************************************************
 *
 * Filename    : BAPPartitioner.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.01b
 * Date        : 29 May 98
 *
 * Description : Interface file for the abstract BAP Partitioning class
 *               Be sure to inherit a non-abstract class from this class
 *               and call it with the BAPSolver.
 *
 * Reference   : nil
 *
 * Notes       : This class should not be modified without proper
 *               authorization.
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


#ifndef __BAP_PARTITIONER__
#define __BAP_PARTITIONER__

#include "def.h"
#include "BAPPackage.h"


class BAPPartitioner : public BAPBase
{
public:
   BAPPartitioner(BAPPackage& aPackage);
   ~BAPPartitioner();

   void  Print(int aWidth = 1, int aDetail = 0) const;

   virtual void  Solve() = 0;          // Abstract member function

protected:
   BAPPackage& mPackage;
};

#endif

