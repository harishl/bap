/**********************************************************************
 *
 * Filename    : PackHeuristics.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.03
 * Date        : July 15, 1998
 *
 * Description : Interface file for class PackHeuristics. This class 
 *               contains some packing heuristics such as FF, BF and WF.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : Add in function Random_Fit. (CLW, 29/06/98)
 *               Add in function Next_Fit. (CLW, 29/07/98)
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               School of Computing
 *               Department of Computer Science
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __PACK_HEURISTICS_H__
#define __PACK_HEURISTICS_H__

#include "FreeSpace.h"
#include "FreeBlock.h"

#include <LEDA/string.h>

class PackHeuristics
{
   public:
      PackHeuristics();
      ~PackHeuristics();

      // Functions to find free block for vessel
      // For online algorithms
      int FindFreeBlock(FreeSpace& aFreeSpace, int aVesLength, 
                        FreeBlock& aFreeBlock, string aPackAlgo,
                        int& aPosition);

      // Function to find free block for vessel
      // For offline algorithms
      int FindFreeBlock(FreeSpace& aFreeSpace, int aVesLength,
                        FreeBlock& aFreeBlock, string aPackAlgo);
      
      // First Fit
      int First_Fit(FreeSpace& aFreeSpace, int aVesLength, 
                        FreeBlock& aFreeBlock);
      // Best Fit
      int Best_Fit(FreeSpace& aFreeSpace, int aVesLength, 
                        FreeBlock& aFreeBlock);
      // Worst Fit
      int Worst_Fit(FreeSpace& aFreeSpace, int aVesLength, 
                        FreeBlock& aFreeBlock);
      // Random Fit
      int Random_Fit(FreeSpace& aFreeSpace, int aVesLength, 
                        FreeBlock& aFreeBlock);

      // Next Fit
      int Next_Fit(FreeSpace& aFreeSpace, int aVesLength, 
                   FreeBlock& aFreeBlock, int& aPosition);
};

#endif
