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

#include "BAPTVVessel.h"

using std::cout;
using std::endl;


TVVessel::TVVessel()
: mID(0), mLength(0), mStartTimeZone(0), mEndTimeZone(0), mSection(0),
  mImport(0), mExport(0), mArrival(0), mDeparture(0), mContainers(0),
  mTranshipment(0)
{
}


TVVessel::TVVessel(int anID, int aLength)
: mID(anID), mLength(aLength), mStartTimeZone(0), mEndTimeZone(0),
  mSection(0), mImport(0), mExport(0), mArrival(0), mDeparture(0),
  mContainers(0), mTranshipment(0)
{
}


TVVessel::TVVessel(const TVVessel& aVessel)
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


TVVessel::~TVVessel()
{
}


TVVessel& TVVessel::operator=(const TVVessel& aVessel)
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


void TVVessel::Print(const int& aWidth, const int& aDetail) const
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


istream& operator>>(istream& anIS, TVVessel& aVessel)
{
   return anIS;
}


ostream& operator<<(ostream& anOS, const TVVessel& aVessel)
{
   return anOS << "vessel " << setw(3) << aVessel.mID;
}


int compare(const TVVessel& aV1, const TVVessel& aV2)
{
   return compare(aV1.mID, aV2.mID);
}


int TVVessel::ID() const
{
   return mID;
}


int TVVessel::Length() const
{
   return mLength;
}


int TVVessel::Import() const
{
   return mImport;
}


int TVVessel::Export() const
{
   return mExport;
}


int TVVessel::Transhipment() const
{
   return mTranshipment;
}


int TVVessel::Containers() const
{
   return mContainers;
}


int TVVessel::StartTimeZone() const
{
   return mStartTimeZone;
}


int TVVessel::EndTimeZone() const
{
   return mEndTimeZone;
}


int TVVessel::Arrival() const
{
   return mArrival;
}


int TVVessel::Departure() const
{
   return mDeparture;
}


int TVVessel::Section() const
{
   return mSection;
}


const set<int>& TVVessel::Neighbours() const
{
   return mNeighbours;
}


const set<int>& TVVessel::Destinations() const
{
   return mPotentialDestinations;
}


int TVVessel::Import(int aNumContainers)
{
   mContainers += aNumContainers;
   mImport = aNumContainers;
   return mImport;
}


int TVVessel::Export(int aNumContainers)
{
   mContainers += aNumContainers;
   mExport = aNumContainers;
   return mExport;
}


void TVVessel::AddTranshipment(int aNumContainers)
{
   mContainers += aNumContainers;
   mTranshipment += aNumContainers;
}


int TVVessel::StartTimeZone(int aTimeZone)
{
   mStartTimeZone = aTimeZone;
   return mStartTimeZone;
}


int TVVessel::EndTimeZone(int aTimeZone)
{
   mEndTimeZone = aTimeZone;
   return mEndTimeZone;
}


int TVVessel::Arrival(int aTime)
{
   mArrival = aTime;
   return mArrival;
}


int TVVessel::Departure(int aTime)
{
   mDeparture = aTime;
   return mDeparture;
}


int TVVessel::Section(int aSection)
{
   mSection = aSection;
   return mSection;
}


void TVVessel::AddNeighbour(int aVessel)
{
   assert(!mNeighbours.member(aVessel));
   mNeighbours.insert(aVessel);
}


void TVVessel::AddDestination(int aSection)
{
   mPotentialDestinations.insert(aSection);
}


void TVVessel::RemoveDestination(int aSection)
{
   assert(mPotentialDestinations.member(aSection));
   mPotentialDestinations.del(aSection);
}

