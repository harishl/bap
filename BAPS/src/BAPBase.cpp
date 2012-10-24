/******************************************************************
 *
 * Filename    : BAPBase.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.0b
 * Date        : 11 Feb 98
 *
 * Description : Implements the generic BAP base class.  All objects of
 *               BAP domain inherit from this class.
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


#include "BAPBase.h"
using namespace std;

BAPBase::BAPBase() : mID(0), mName("Nameless")
{
}


BAPBase::BAPBase(long aID, leda::string aName)
: mID(aID), mName(aName)
{
}


BAPBase::~BAPBase()
{
}


long   BAPBase::ID() const { return mID; }

leda::string BAPBase::Name() const { return mName; }

leda::string BAPBase::Name(leda::string aName)
{
   leda::string   temp = mName;
   mName = aName;
   return temp;
}
