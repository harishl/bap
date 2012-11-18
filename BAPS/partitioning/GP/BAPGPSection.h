/******************************************************************
 *
 * Filename    : BAPTVSection.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.01b
 * Date        : 10 Jun 98
 *
 * Description : Implements the TVSection class (?)
 *
 * Reference   : nil
 *
 * Notes       : 
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

#ifndef __BAP_GP_SECTION__
#define __BAP_GP_SECTION__


#include "def.h"
#include "BAPGPVessel.h"

using std::istream;
using std::ostream;
using leda::array;
using leda::set;

class GPSection
{
public:
   GPSection();
   GPSection(int anID, int aMaxTimeZone, int aLength);
   GPSection(const GPSection& aSection);
   ~GPSection();
   GPSection& operator=(const GPSection& aSection);
   void Print(const int& aWidth = 1, const int& aDetail = 0) const;

   friend istream& operator>>(istream& anIS, GPSection& aSection);
   friend ostream& operator<<(ostream& anOS, const GPSection& aSection);
   friend int compare(const GPSection& aS1, const GPSection& aS2);

   int         ID() const;
   int         Length() const;
   bool        CanAccommodate(const GPVessel& aVessel) const;
   const		set<int>&   Vessels() const;

   void        Add(const GPVessel& aVessel);
   void        Remove(const GPVessel& aVessel);
   int  	   Capacity(const GPVessel& aVessel);

private:
   int         mID;
   int         mMaxTimeZone;
   int         mLength;
   array<int>  mCapacity;
   set<int>    mVessels;
};


#endif
