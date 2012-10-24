/**********************************************************************
 *
 * Filename    : FreeBlock.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0a
 * Date        : June 14, 1998
 *
 * Description : Interface file for class FreeBlock. It's the rectangle 
 *               space which is free to pack vessels. 
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
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __FREEBLOCK_H__
#define __FREEBLOCK_H__

#include "BAPBase.h"
#include "Vessel.h"
#include "PackDef.h"

class FreeBlock : public BAPBase
{
   public:
      FreeBlock();
      FreeBlock(int aBeginPosition, int aSize);
      FreeBlock(const FreeBlock& aFreeBlock);
      ~FreeBlock();

      // Debugger facility
      void       Print(int aWidth = 1, int aDetail = 0) const;
      
      // Assignment operator =
      FreeBlock& operator=(const FreeBlock& aFreeBlock);
      
      // Modification functions
      void       SetBeginPosition(int aBeginPosition);
      void       SetEndPosition(int aEndPosition);
      void       SetSize(int aSize);
      
      // Access functions
      int        GetBeginPosition() const;
      int        GetEndPosition() const;
      int        GetSize() const;
      
      friend int      compare(const FreeBlock& Lhs, const FreeBlock& Rhs);
      friend ostream& operator<<(ostream& aOutput, const FreeBlock& aFBlock);
      friend istream& operator>>(istream& aInput, FreeBlock& aFBlock);

   private:
      int        mBeginPosition;
      int        mEndPosition;
      int        mSize;
};

#endif
