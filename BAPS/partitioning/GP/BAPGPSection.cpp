/******************************************************************
 *
 * Filename    : BAPTVSection.cpp
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

#include "BAPGPSection.h"

using std::cout;
using std::endl;

GPSection::GPSection()
: mID(0), mMaxTimeZone(0), mLength(0)
{
}


GPSection::GPSection(int anID, int aMaxTimeZone, int aLength)
: mID(anID), mMaxTimeZone(aMaxTimeZone), mLength(aLength),
  mCapacity(array<int>(1, aMaxTimeZone))
{
   for (int i = 1; i <= aMaxTimeZone; i++)
   {
      mCapacity[i] = aLength;
   }
}


GPSection::GPSection(const GPSection& aSection)
: mID(aSection.mID),
  mLength(aSection.mLength),
  mMaxTimeZone(aSection.mMaxTimeZone),
  mCapacity(array<int>(aSection.mCapacity.low(),
                       aSection.mCapacity.high())),
  mVessels(aSection.mVessels)
{
   for (int i = 1; i <= mMaxTimeZone; i++)
   {
      mCapacity[i] = aSection.mCapacity[i];
   }
}


GPSection::~GPSection()
{
}


GPSection& GPSection::operator=(const GPSection& aSection)
{
   if (this != &aSection)
   {
      mID = aSection.mID;
      mLength = aSection.mLength;
      mMaxTimeZone = aSection.mMaxTimeZone;
      mCapacity = aSection.mCapacity;
      mVessels = aSection.mVessels;
   }

   return (*this);
}


void GPSection::Print(const int& aWidth, const int& aDetail) const
{
   cout  << "TVSection " << mID << ", length = " << mLength
         << ", max time zone = " << mMaxTimeZone << endl;

   if (aDetail > 1)
   {
      cout  << "capacity:" << endl;

      for (int i = 1; i <= mMaxTimeZone; i++)
      {
         cout  << setw(8) << mCapacity[i];
      }

      cout << endl;
   }

   cout << "vessels in section =" << endl;

   int   v;

   forall(v, mVessels)
   {
      cout  << setw(5) << v;
   }

   cout << endl << endl;
}


istream& operator>>(istream& anIS, GPSection& aSection)
{
   return anIS;
}


ostream& operator<<(ostream& anOS, const GPSection& aSection)
{
   return anOS << "section " << setw(2) << aSection.mID;
}


int compare(const GPSection& aS1, const GPSection& aS2)
{
   return compare(aS1.mID, aS2.mID);
}

int GPSection::ID() const
{
   return mID;
}


int GPSection::Length() const
{
   return mLength;
}


bool GPSection::CanAccommodate(const GPVessel& aVessel) const
{
   int   VesLen = aVessel.Length();

   for (int i = aVessel.StartTimeZone(); i <= aVessel.EndTimeZone(); i++)
   {
      if (mCapacity[i] < VesLen)
         return false;
   }

   return true;
}


const set<int>& GPSection::Vessels() const
{
   return mVessels;
}


void GPSection::Add(const GPVessel& aVessel)
{
   assert(!mVessels.member(aVessel.ID()));

   int   VesLen = aVessel.Length();

   for (int i = aVessel.StartTimeZone(); i <= aVessel.EndTimeZone(); i++)
   {
      mCapacity[i] -= VesLen;
      assert(mCapacity[i] >= 0);
   }

   mVessels.insert(aVessel.ID());
}


void GPSection::Remove(const GPVessel& aVessel)
{
   assert(mVessels.member(aVessel.ID()));

   int   VesLen = aVessel.Length();

   for (int i = aVessel.StartTimeZone(); i <= aVessel.EndTimeZone(); i++)
   {
      mCapacity[i] += VesLen;
      assert(mCapacity[i] <= mLength);
   }

   mVessels.del(aVessel.ID());
}

