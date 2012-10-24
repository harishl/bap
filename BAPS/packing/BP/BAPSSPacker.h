/**********************************************************************
 *
 * Filename    : BAPSSPacker.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.06
 * Date        : Jan. 1, 1999
 *
 * Description : Header file for class BAPSSPacker.
 *               It inherites from BAPBPVESPacker class.
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
 **********************************************************************/


#ifndef __BAPSSPACKER_H__
#define __BAPSSPACKER_H__


#include "Vessel.h"
#include "BAPBPVESPacker.h"


#include <LEDA/list.h>


class BAPSSPacker : public BAPBPVESPacker
{
   public:

      //
      // In single-section model, we will separate vessels 
      // for each section, then do packing within each 
      // section separately. 
      //
      
      BAPSSPacker(BAPPackage & aPackage, Bool aOutput = FALSE); 
      ~BAPSSPacker();


      void Solve();


      //
      // Method GetPackingWidth inherits from parent, 
      // Attribute mPackage and mPackingWidth inherits from 
      // parent, too.
      //

   protected:
      Bool Output;
      
   private:

      //
      // Method to handle vessels for offline heuristics
      //

      void HandleVessels(list<Vessel> & aVesselList,
                         int* SortParam, int ParamSize);
};


#endif
