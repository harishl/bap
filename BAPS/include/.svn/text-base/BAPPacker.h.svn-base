/******************************************************************
 *
 * Filename    : BAPPacker.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.0
 * Date        : 11 May 98
 *
 * Description : Interface file for the abstract BAP Packing Stub
 *
 * Reference   : nil
 *
 * Notes       : The BAP Packing algorithm
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


#ifndef __BAP_PACKER__
#define __BAP_PACKER__

#include "def.h"
#include "BAPPackage.h"


class BAPPacker : public BAPBase
{
public:
   BAPPacker(BAPPackage& aPackage);
   ~BAPPacker();

   void Print(int aWidth = 1, int aDetail = 0) const;

   virtual void Solve() = 0;

protected:
   BAPPackage& mPackage;
};

#endif

