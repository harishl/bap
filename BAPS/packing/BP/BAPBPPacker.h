/******************************************************************
 *
 * Filename    : BAPBPPacker.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.1
 * Date        : Nov. 12, 1998
 *
 * Description : Interface file for the BAP BP Packing Stub. 
 *               This packer will do the packing subtask using
 *               Bin-Packing algorithms. It will call different
 *               packer to do the packing task according to different
 *               parameters passed to it.
 *
 * Reference   : nil
 *
 * Notes       : Derived from class BAPPacker.
 *
 * Changes     : 10/05/99, Chen Li Wen
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 ******************************************************************/


#ifndef __BAPBPPACKER__
#define __BAPBPPACKER__


#include "PackDef.h"
#include "BAPPacker.h"


#include <LEDA/core/list.h>

using leda::list;


class BAPBPPacker : public BAPPacker
{
   public:
   
      BAPBPPacker(BAPPackage& aPackage);
      ~BAPBPPacker();

   
      //
      // Accessor member functions
      // Write information to solution file
      // If computing packing width, write packing width to
      // solution files; otherwise, write unpacked vessels to 
      // solution files.
      //
      
      void   WriteSolutionFile(Bool aCompPackWidth) const;

      
      //
      // Modifier member functions
      // Read solution from partitioning solution file
      //
      
      void   ReadSolutionFile();
      
      
      //
      // Call different packer to do packing due to 
      // different parameters
      //
      
      void   Solve();


   private:
      
      //
      // Keep packing width and density
      //
      
      list<int> mPackingWidth;
      list<int> mDensity;
      
};


#endif
