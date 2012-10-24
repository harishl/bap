/**********************************************************************
 *
 * Filename    : BAPSSAPPPacker.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : Nov. 23, 1998
 *
 * Description : This packer will using approximation algorithms to 
 *               do packing.
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


#ifndef __BAPSSAPPPACKER_H__
#define __BAPSSAPPPACKER_H__


#include "BAPBPVESPacker.h"


#include <LEDA/string.h>


class BAPSSAPPPacker : public BAPBPVESPacker
{
   public:
      
      BAPSSAPPPacker(BAPPackage & aPackage,
                     string       aPackAlgo,
                     Bool         aOutput = FALSE);

      ~BAPSSAPPPacker();

      void Solve();

      string GetPackAlgo() const;


   private:

      string mPackAlgo;
      Bool   mOutput;
};


#endif
