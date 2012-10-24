/**********************************************************************
 *
 * Filename    : SortVessel.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : Aug. 13, 1998
 *
 * Description : This class is written to do various sorting for a 
 *               list of vessels according to different compare 
 *               rules.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : nil
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 ********************************************************************/

#ifndef __SORT_VESSEL_H__
#define __SORT_VESSEL_H__

#include "Vessel.h"
#include <LEDA/list.h>


// Define the number of compare rules. Now, we have
// 20 distinct compare ruls.
const int NUM_FUNC = 20;

// This is an array of function pointers which point to
// distinct compare functions.
typedef int (*COMP_FUNC)(const Vessel&, const Vessel&);


class SortVessel
{
   public:
      SortVessel();
      ~SortVessel();

      // This function will set the parameter list, which will
      // tell you how to compare two vessels.
      void SetParamList(int *aParamList, int aSize);

      // Access functions.
      int  GetParamSize() const { return mParamSize; }
      int* GetParamList() const { return mParamList; }

      // Function to sort a vessel list according to the 
      // parameter list.
      void Sort(list<Vessel>& aVesselList);
   
   // Compare function defined by parameter list.
   friend int Compare(const Vessel& Lhs, const Vessel& Rhs);

   private:
      int *mParamList;
      int mParamSize;
};

#endif
