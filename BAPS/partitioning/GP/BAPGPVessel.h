/******************************************************************
 *
 * Filename    : BAPTVVessel.h
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

#ifndef __BAP_GP_VESSEL__
#define __BAP_GP_VESSEL__


#include "def.h"

using std::istream;
using std::ostream;
using leda::set;

class GPVessel
{
public:
   GPVessel();
   GPVessel(int anID, int aLength);
   GPVessel(const GPVessel& aVessel);
   ~GPVessel();
   GPVessel& operator=(const GPVessel& aVessel);
   void Print(const int& aWidth = 1, const int& aDetail = 0) const;

   friend   istream& operator>>(istream& anIS, GPVessel& aVessel);
   friend   ostream& operator<<(ostream& anOS, const GPVessel& aVessel);
   friend   int compare(const GPVessel& aV1, const GPVessel& aV2);

   int      ID() const;
   int      Length() const;
   int      Containers() const;
   int      Import() const;
   int      Export() const;
   int      Transhipment() const;
   int      StartTimeZone() const;
   int      EndTimeZone() const;
   int      Arrival() const;
   int      Departure() const;
   int      Section() const;
   const set<int>& Neighbours() const;
   const set<int>& Destinations() const;

   int      Import(int aNumContainers);
   int      Export(int aNumContainers);
   int      StartTimeZone(int aTimeZone);
   int      EndTimeZone(int aTimeZone);
   int      Arrival(int aTime);
   int      Departure(int aTime);
   int      Section(int aSection);
   void     AddNeighbour(int aVessel);
   void     AddTranshipment(int aValue);
   void     AddDestination(int aSection);
   void     RemoveDestination(int aSection);

private:
   int      mID;
   int      mLength;
   int      mContainers;
   int      mImport, mExport, mTranshipment;
   int      mStartTimeZone, mEndTimeZone;
   int      mArrival, mDeparture;
   int      mSection;
   set<int> mNeighbours;
   set<int> mPotentialDestinations;
};


#endif
