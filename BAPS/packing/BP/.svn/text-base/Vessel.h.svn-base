/**********************************************************************
 *
 * Filename    : Vessel.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : July 15, 1998
 *
 * Description : Interface file for class Vessel. 
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : Change function names. ---CLW, June 7, 1998.
 *               Add in functions to compare two Vessel objects.
 *                                      ---CLW, July 15, 1998.
 *               Add in functions to various comparing.
 *                                      ---CLW, August 5, 1998.
 *               Add in functions to compare packing space.
 *                                      ---CLW, September 3, 1998.
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 **********************************************************************/


#ifndef __VESSEL_H__
#define __VESSEL_H__


#include "PackDef.h"
#include "BAPBase.h"


// Class inherited from BAPBase class.
class Vessel
{
   public:
      // Class constructors
      // This constructor will create a dummy vessel with length 0
      Vessel();
      // Create vessel with given parameters
      Vessel(int aID, int aLength = 0, int aArriveTime = 0, 
             int aDepartureTime = 0, Bool aStatus = FALSE, 
             int aWharfMark = -1, int aNumTimeZones = 0, 
             int aBigDensity = 0, int aAverageDensity = 0);
      // Copy constructor
      Vessel(const Vessel& aVessel);

      // Class destructor
      ~Vessel();
      
      // Assignment operator =
      Vessel& operator=(const Vessel& aVessel);

      // Debugger function.
      void Print(int aWidth = 1, int aDetail = 0) const;

      // Access functions.
      int  GetVesID() const;
      int  GetVesLength() const;
      int  GetVirtualLength() const;
      int  GetArriveTime() const;
      Bool GetPackStatus() const;
      int  GetBigDensity() const;
      int  GetPackingSpace() const;
      int  GetDepartureTime() const;
      int  GetAverageDensity() const;
      int  GetNumberTimeZones() const;
      int  GetWharfMarkAssignedTo() const;
      int  GetVirtualWharfMark() const;

      // Modification functions.
      void SetVesID(int aID);
      void SetVesLength(int aLength);
      void SetVirtualLength(int aVirtualLength);
      void SetArriveTime(int aArriveTime);
      void SetPackStatus(Bool aStatus);
      void SetBigDensity(int aBigDensity);
      void SetDepartureTime(int aDepartureTime);
      void SetAverageDensity(int aAverageDensity);
      void SetNumberTimeZones(int aNumTimeZones);
      void SetWharfMarkAssignedTo(int aWharfMark);
      void SetVirtualWharfMark(int aVirtualWharfMark);

      // Functions for comparing, input and output.
      
      // Default compare function
      // Non-increasing length
      friend int 
         compare(const Vessel& Lhs, const Vessel& Rhs);
      
      // Compare by vessel ID
      friend int 
         Comp_VesID_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_VesID_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by vessel length
      friend int 
         Comp_Length_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_Length_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by vessel wharfmark assigned.
      friend int 
         Comp_WharfMark_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_WharfMark_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by vessel arrive time
      friend int 
         Comp_ArriveTime_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_ArriveTime_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by vessel depart time
      friend int 
         Comp_DepartTime_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_DepartTime_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by density of the busiest Time Zone covered by vessel
      friend int 
         Comp_BigDensity_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_BigDensity_Decrease(const Vessel& Lhs, const Vessel& Rhs);   
         
      // Compare by average density of Time Zones covered by vessel
      friend int 
         Comp_AverageDensity_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_AverageDensity_Decrease(const Vessel& Lhs, const Vessel& Rhs);
         
      // Compare by number of Time Zones covered by vessel
      friend int 
         Comp_NumberTimeZone_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_NumberTimeZone_Decrease(const Vessel& Lhs, const Vessel& Rhs);

      // Compare by packing space   
      friend int 
         Comp_PackingSpace_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int 
         Comp_PackingSpace_Decrease(const Vessel& Lhs, const Vessel& Rhs);
      
      // Compare by duration
      friend int 
         Comp_Duration_Increase(const Vessel& Lhs, const Vessel& Rhs);
      friend int
         Comp_Duration_Decrease(const Vessel& Lhs, const Vessel& Rhs);

      // Input and output operator overloadding          
      friend ostream& operator<<(ostream& aOutput, const Vessel& aVessel);
      friend istream& operator>>(istream& aInput, Vessel& aVessel);

      // Check whether two vessels are overlapped
      friend Bool
         IsOverlap(const Vessel& Lhs, const Vessel& Rhs);


      // For Gergov's 
      friend int
         Comp_Gerg(const Vessel& Lhs, const Vessel& Rhs);

      // Logical operators
      friend Bool
         operator==(const Vessel& Lhs, const Vessel& Rhs);

   private:
      // Private attributes.
      int  mID;
      int  mLength;
      int  mArriveTime;
      int  mDepartureTime;
      Bool mStatus;
      int  mWharfMark;
      int  mNumTimeZones;
      int  mBigDensity;
      int  mAverageDensity;

      // For use of Gergov's Algorithm
      int mVirtualLength;
      int mVirtualWharfMark;
};


#endif
