/******************************************************************
 *
 * Filename    : BAPTVVessel.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.01b
 * Date        : 10 Jun 98
 *
 * Description : Implements the TVVessel class (?)
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

#include "BAPGPVessel.h"

using std::cout;
using std::endl;


GPVessel::GPVessel()
: mID(0), mLength(0), mStartTimeZone(0), mEndTimeZone(0), mSection(0),
  mImport(0), mExport(0), mArrival(0), mDeparture(0), mContainers(0),
  mTranshipment(0), locked(0)
{
}


GPVessel::GPVessel(int anID, int aLength)
: mID(anID), mLength(aLength), mStartTimeZone(0), mEndTimeZone(0),
  mSection(0), mImport(0), mExport(0), mArrival(0), mDeparture(0),
  mContainers(0), mTranshipment(0), locked(0)
{
}


GPVessel::GPVessel(const GPVessel& aVessel)
: mID(aVessel.mID), mLength(aVessel.mLength),
  mImport(aVessel.mImport), mExport(aVessel.mExport),
  mTranshipment(aVessel.mTranshipment),
  mContainers(aVessel.mContainers),
  mStartTimeZone(aVessel.mStartTimeZone),
  mEndTimeZone(aVessel.mEndTimeZone),
  mArrival(aVessel.mArrival),
  mDeparture(aVessel.mDeparture),
  mSection(aVessel.mSection),
  mNeighbours(aVessel.mNeighbours),
  mPotentialDestinations(aVessel.mPotentialDestinations)
{
}


GPVessel::~GPVessel()
{
}


GPVessel& GPVessel::operator=(const GPVessel& aVessel)
{
   if (this != &aVessel)
   {
      mID = aVessel.mID;
      mLength = aVessel.mLength;
      mImport = aVessel.mImport;
      mExport = aVessel.mExport;
      mTranshipment = aVessel.mTranshipment;
      mContainers = aVessel.mContainers;
      mStartTimeZone = aVessel.mStartTimeZone;
      mEndTimeZone = aVessel.mEndTimeZone;
      mArrival = aVessel.mArrival;
      mDeparture = aVessel.mDeparture;
      mSection = aVessel.mSection;
      mNeighbours = aVessel.mNeighbours;
      mPotentialDestinations = aVessel.mPotentialDestinations;
   }

   return (*this);
}


void GPVessel::Print(const int& aWidth, const int& aDetail) const
{
   cout  << "TVVessel" << setw(5) << mID
         << ", length =" << setw(4) << mLength
         << ", [" << setw(3) << mStartTimeZone
         << "," << setw(3) << mEndTimeZone
//         << "] In = " << setw(4) << mImport
//         << " Out = " << setw(4) << mExport
         << "], in section" << setw(3) << mSection << endl;

   if (aDetail > 1)
   {
      cout  << tab(3) << "neighbours =";

      int   v;

      forall(v, mNeighbours)
      {
         cout  << setw(5) << v;
      }

      cout << endl;

      cout  << tab(3) << "destinations =";

      forall(v, mPotentialDestinations)
      {
         cout  << setw(3) << v;
      }

      cout << endl;
   }

   cout << endl;
}


istream& operator>>(istream& anIS, GPVessel& aVessel)
{
   return anIS;
}


ostream& operator<<(ostream& anOS, const GPVessel& aVessel)
{
   return anOS << "vessel " << setw(3) << aVessel.mID;
}


int compare(const GPVessel& aV1, const GPVessel& aV2)
{
   return compare(aV1.mID, aV2.mID);
}


int GPVessel::ID() const
{
   return mID;
}


int GPVessel::Length() const
{
   return mLength;
}


int GPVessel::Import() const
{
   return mImport;
}


int GPVessel::Export() const
{
   return mExport;
}


int GPVessel::Transhipment() const
{
   return mTranshipment;
}


int GPVessel::Containers() const
{
   return mContainers;
}


int GPVessel::StartTimeZone() const
{
   return mStartTimeZone;
}


int GPVessel::EndTimeZone() const
{
   return mEndTimeZone;
}


int GPVessel::Arrival() const
{
   return mArrival;
}


int GPVessel::Departure() const
{
   return mDeparture;
}


int GPVessel::Section() const
{
   return mSection;
}


const set<int>& GPVessel::Neighbours() const
{
   return mNeighbours;
}


const set<int>& GPVessel::Destinations() const
{
   return mPotentialDestinations;
}


int GPVessel::Import(int aNumContainers)
{
   mContainers += aNumContainers;
   mImport = aNumContainers;
   return mImport;
}


int GPVessel::Export(int aNumContainers)
{
   mContainers += aNumContainers;
   mExport = aNumContainers;
   return mExport;
}


void GPVessel::AddTranshipment(int aNumContainers)
{
   mContainers += aNumContainers;
   mTranshipment += aNumContainers;
}


int GPVessel::StartTimeZone(int aTimeZone)
{
   mStartTimeZone = aTimeZone;
   return mStartTimeZone;
}


int GPVessel::EndTimeZone(int aTimeZone)
{
   mEndTimeZone = aTimeZone;
   return mEndTimeZone;
}


int GPVessel::Arrival(int aTime)
{
   mArrival = aTime;
   return mArrival;
}


int GPVessel::Departure(int aTime)
{
   mDeparture = aTime;
   return mDeparture;
}


int GPVessel::Section(int aSection)
{
   mSection = aSection;
   return mSection;
}


void GPVessel::AddNeighbour(int aVessel)
{
   assert(!mNeighbours.member(aVessel));
   mNeighbours.insert(aVessel);
}


void GPVessel::AddDestination(int aSection)
{
   mPotentialDestinations.insert(aSection);
}


void GPVessel::RemoveDestination(int aSection)
{
   assert(mPotentialDestinations.member(aSection));
   mPotentialDestinations.del(aSection);
}

