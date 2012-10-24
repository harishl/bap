/******************************************************************
 *
 * Filename    : BAPBase.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.0b
 * Date        : 11 Feb 98
 *
 * Description : Interface file for generic BAP base class.  All objects
 *               of BAP domain inherit from this class.
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
 ******************************************************************/


#ifndef __BAP_BASE__
#define __BAP_BASE__


#include "def.h"

class BAPBase
{
public:
   BAPBase();
   BAPBase(long aID, string aName = "Nameless");
   ~BAPBase();

   long           ID() const;
   string         Name() const;
   string         Name(string aName);
   virtual void   Print(int aWidth = 1, int aDetail = 0) const = 0;

private:
   const long  mID;
   string      mName;
};


#endif
