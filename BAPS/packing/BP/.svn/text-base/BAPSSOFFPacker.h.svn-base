/**********************************************************************
 *
 * Filename    : BAPSSOFFHEUPacker.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : Nov. 12, 1998
 *
 * Description : Interface file for class BAPSSOFFHEUPacker. This packer
 *               uses offline heuristics to pack vessels.
 *               Note: we assume that the list of vessels are sorted 
 *               by given order. It's the packing order.
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


#ifndef __BAPSSOFFPACKER_H__
#define __BAPSSOFFPACKER_H__


#include "FreeSpace.h"


#include <LEDA/string.h>


class BAPSSOFFPacker
{
   public:
   
      BAPSSOFFPacker(list<Vessel>& aVesselList,
                     string        aPackAlgo,
                     int           aSectionLength,
                     Bool          aCompPackWidth = FALSE,
                     Bool          aOutput = FALSE);
      
      ~BAPSSOFFPacker();


      void Solve();
      int GetPackingWidth() const;


   private:

      Bool          mCompPackWidth;
      Bool          mOutput;
      list<Vessel>& mVesselList;
      string        mPackAlgo;
      int           mSectionLength;
      
      
      //
      // Private methods
      // To find the set of vessels intersecting with a 
      // given vessel;
      // 
      
      list<Vessel> FindIV(list<Vessel> & aVesselList, 
                          int            aVesIndex);
      

      //
      // To find the set of free space for a given vessel
      //
      
      void FindFS(FreeSpace &    aFreeSpace, 
                  list<Vessel> & aIV, 
                  int            aSectLength);   

};

#endif
