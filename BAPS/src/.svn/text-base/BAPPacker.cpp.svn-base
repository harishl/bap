/******************************************************************
 *
 * Filename    : BAPPacker.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.0
 * Date        : 11 May 98
 *
 * Description : Implements the BAP Packing Stub
 *
 * Reference   : nil
 *
 * Notes       : The BAP Packing algorithm
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


#include "BAPPacker.h"

using std::cout;
using std::endl;

BAPPacker::BAPPacker(BAPPackage& aPackage)
: BAPBase(), mPackage(aPackage)
{
}


BAPPacker::~BAPPacker()
{
}


void BAPPacker::Print(int aW, int aDetail) const
{
   cout  << tab(aW) << "--== BAPPacker ==--" << endl
         << tab(aW) << "  ID        = " << ID() << endl
         << tab(aW) << "  Name      = " << Name() << endl
         << endl;

   mPackage.Print(aW + INDENT, aDetail);
}

