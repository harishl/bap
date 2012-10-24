/**********************************************************************
 *
 * Filename    : FreeSpace.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.03
 * Date        : July 15, 1998
 *
 * Description : Interface file for class FreeSpace. It's a collection 
 *               FreeBlocks which can packed to vessels. 
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : Add the function Last. July 12, 1998.
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __FREESPACE_H__
#define __FREESPACE_H__

#include "FreeBlock.h"
#include "Vessel.h"

#include <LEDA/list.h>

class FreeSpace
{
   public:
      FreeSpace();
      FreeSpace(int aSize);
      FreeSpace(list<FreeBlock>& aListOfFreeBlocks);
      FreeSpace(const FreeSpace& aFreeSpace);
      ~FreeSpace();

      // Debugger facility
      void Print(int aWidth = 1, int aDetail = 0) const;

      // Initialization
      void Initialize(int aSize);

      // Useful Functions for doing packing and liberating.
      void Remove(FreeBlock& aFreeBlock);
      void Substitute(FreeBlock& OldFB, FreeBlock& NewFB);

      // If aDirection is 0, it will insert before, else, will insert after
      void Insert(FreeBlock& aPosition, FreeBlock& aFreeBlock, 
                  int aDirection = 0);
      
      // Assign Vessel to FreeBlcok.
      void AssignVessel(FreeBlock& aFreeBlock, Vessel& aVessel);
      
      // Liberating Vessel from FreeBlock
      void Liberation(Vessel& aVessel);
      
      // Access function
      list<FreeBlock> GetFreeBlockList() const;
      FreeBlock First() const;
      FreeBlock Last() const;
      int GetLength() const;

   private:
      list<FreeBlock> mFreeBlockList;
};

#endif
