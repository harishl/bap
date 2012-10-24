/**********************************************************************
 *
 * Filename    : BAPBPVESPacker.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : Nov. 11, 1998
 *
 * Description : This is an abstract class. It's the parent class for 
 *               all packers of my BAP packing system. It inherits 
 *               from class BAPPacker.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : Currently, it's the parent class of BAPSSPacker and
 *               BAPMSPacker, the single-section packer and teh multiple
 *               section packer.
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 **********************************************************************/


#ifndef __BAPBPVESPACKER_H__
#define __BAPBPVESPACKER_H__


#include "Vessel.h"
#include "BAPPacker.h"
#include "BAPPackage.h"


#include <LEDA/list.h>


//
// Class inherits from class BAPPacker
//

class BAPBPVESPacker : public BAPPacker
{
   public:

      //
      // All packer classes will be created based on 
      // an object of class BAPPackage. It contains 
      // all information needed by all packers inside.
      //

      BAPBPVESPacker(BAPPackage & aPackage);
      ~BAPBPVESPacker();


      //
      // Access function
      // Section number from 1 to n
      //

      int GetPackingWidth(int aSectNumber) const;
      int GetDensity(int aSectNumber) const;


      // 
      // Get the set of vessels for the given section
      //

      list<Vessel> VesselSetForSection(int aSectNumber) const;
      
      
      //
      // The virtual function Solve() inherits from parent class.
      // The attribute "mPackage" also inherits from it.
      //

      virtual void Solve() = 0;

   protected:

      //
      // We first init them to the densities of vessels in each
      // section; then we will calculate the actual packing 
      // width for this packing algorithm.
      //

      list<int> mPackingWidth;
      list<int> mDensity;


      void CalcPackingWidth();


   private:

      // 
      // Utility to initialize packing width
      //

      void InitPackingWidth();
      void CalcDensity();

};

#endif

