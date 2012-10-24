/******************************************************************
 *
 * Filename    : BAPPartitioner.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.0
 * Date        : 11 May 98
 *
 * Description : Implements the BAP Partitioning Stub
 *
 * Reference   : nil
 *
 * Notes       : This class is not to be modified without proper
 *               authorization
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


#include "BAPPartitioner.h"

using std::cout;
using std::endl;

BAPPartitioner::BAPPartitioner(BAPPackage& aPackage)
: BAPBase(), mPackage(aPackage)
{
}


BAPPartitioner::~BAPPartitioner()
{
}


void BAPPartitioner::Print(int aW, int aDetail) const
{
   cout  << tab(aW) << "--== BAPPartitioner ==--" << endl
         << tab(aW) << "  ID        = " << ID() << endl
         << tab(aW) << "  Name      = " << Name() << endl
         << endl;
 
   mPackage.Print(aW + INDENT, aDetail);
}

