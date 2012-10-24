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

#ifndef __BAP_TV_SECTION__
#define __BAP_TV_SECTION__


#include "def.h"
#include "BAPTVVessel.h"

using std::istream;
using std::ostream;
using leda::array;
using leda::set;

class TVSection
{
public:
   TVSection();
   TVSection(int anID, int aMaxTimeZone, int aLength);
   TVSection(const TVSection& aSection);
   ~TVSection();
   TVSection& operator=(const TVSection& aSection);
   void Print(const int& aWidth = 1, const int& aDetail = 0) const;

   friend istream& operator>>(istream& anIS, TVSection& aSection);
   friend ostream& operator<<(ostream& anOS, const TVSection& aSection);
   friend int compare(const TVSection& aS1, const TVSection& aS2);

   int         ID() const;
   int         Length() const;
   bool        CanAccommodate(const TVVessel& aVessel) const;
   const set<int>&   Vessels() const;

   void        Add(const TVVessel& aVessel);
   void        Remove(const TVVessel& aVessel);

private:
   int         mID;
   int         mMaxTimeZone;
   int         mLength;
   array<int>  mCapacity;
   set<int>    mVessels;
};


#endif
