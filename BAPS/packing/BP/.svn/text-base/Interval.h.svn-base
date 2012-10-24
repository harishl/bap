/**********************************************************************
 *
 * Filename    : Interval.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0 
 * Date        : 09-27-98
 *
 * Description : Header file for class Interval. It keeps an interval, 
 *               which can be open, close, or half open. It supports 
 *               all interval operations:
 *               --- Union: return the union of two intersecting 
 *                          intervals;
 *               --- Intersect: return the intersection of two
 *                              intervals;
 *               --- SymDiff: return the symmitric difference of two 
 *                            intervals;
 *               --- Length: return the size of the interval.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : 
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __INTERVAL_H__
#define __INTERVAL_H__


#include "PackDef.h"


#include <iostream.h>


class Interval
{
   public:
      
      //
      // Each interval contains four parameters: the left point,
      // the right point, the open or close status of each end.
      //    0: close; 
      //    1: open.
      //
      
      Interval();
      Interval(double aLeftPoint, 
               double aRightPoint, 
               int    aLeftStatus = 0, 
               int    aRightStatus = 0);
      Interval(const Interval & aInterval);
      ~Interval();


      //
      // Assignment operator.
      //
      
      Interval & operator=(const Interval & aInterval);


      //
      // Equality operators.
      //
      
      Bool operator==(const Interval & aInterval) const;
      Bool operator!=(const Interval & aInterval) const;

      
      //
      // Debugger facility
      //
      
      void Print() const;

      
      //
      // Access methods
      //
      
      double GetLeftPoint()   const { return mLeftPoint; }
      double GetRightPoint()  const { return mRightPoint; }
      double GetLength()      const { return (mRightPoint - mLeftPoint); }
      int    GetLeftStatus()  const { return mLeftStatus; }
      int    GetRightStatus() const { return mRightStatus; }


      //
      // Modification methods
      //
      
      void SetLeftPoint(double aLeftPoint)   { mLeftPoint = aLeftPoint; }
      void SetRightPoint(double aRightPoint) { mRightPoint = aRightPoint; }
      void SetLeftStatus(int aLeftStatus)    { mLeftStatus = aLeftStatus; }
      void SetRightStatus(int aRightStatus)  { mRightStatus = aRightStatus; }
      
      
      //
      // Determine interval is empty or not.
      //
      
      Bool IsEmpty() const 
           { return ((*this).GetLength() == 0 ? TRUE : FALSE); }
      
      
      //
      // Check whether the two intervals can be combined to 
      // a single interval.
      //
      
      friend Bool NextTo(const Interval& Lhs, const Interval& Rhs);
      

      //
      // Only when the two intervals are next to each other.
      //
      
      friend Interval 
             Union(const Interval& Lhs, const Interval& Rhs);
      

      //
      // No constraint.
      //
      
      friend Interval 
             Intersect(const Interval & Lhs, const Interval & Rhs);
      friend int
             compare(const Interval & Lhs, const Interval & Rhs);
      friend ostream &
             operator<<(ostream & aOstream, const Interval & aInterval);
      friend istream &
             operator>>(istream & aIstream, Interval & aInterval);
         
         
   private:
   
      double mLeftPoint;
      double mRightPoint;
      int    mLeftStatus;
      int    mRightStatus;
};


#endif
