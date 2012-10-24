/**********************************************************************
 *
 * Filename    : IntervalSet.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : 09-28-98
 *
 * Description : Header file for class IntervalSet. It contains a set 
 *               of disjoint intervals and provides some operations.
 *               Intervals are sorted by non-decreasing left points.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : nil
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 **********************************************************************/


#ifndef __INTERVAL_SET_H__
#define __INTERVAL_SET_H__


#include "Interval.h"


#include <LEDA/list.h>


class IntervalSet
{
   public:

      //
      // Constructor of class IntervalSet. It contains
      // a set of disjoint intervals.
      //
      
      IntervalSet();
      IntervalSet(const list<Interval> & aListOfInterval);
      IntervalSet(const IntervalSet & aIntervalSet);
      ~IntervalSet();

      
      //
      // Assignment operator 
      //
      
      IntervalSet & operator=(const IntervalSet & aIntervalSet);

      
      //
      // Debugger facility
      //
      
      void Print() const;
      
      
      //
      // Access methods
      //
      
      int      GetLength() const { return mLength; }
      Interval GetInterval(int aIndex) const
               { return mListOfInterval.inf(mListOfInterval[aIndex]); }

      
      //
      // Operations
      // For Remove, the interval being removed should
      // be in the interval set.
      //
      
      void Insert(Interval & aInterval);
      void Remove(Interval & aInterval);

      // 
      // Merge two sets of intervals, the result is an IntervalSet.
      //
      
      friend IntervalSet
             merge(const IntervalSet & Lhs, const IntervalSet & Rhs);
      
      friend ostream& 
             operator<<(ostream& os, const IntervalSet& aIntervalSet);
      friend istream&
             operator>>(istream& is, IntervalSet& aIntervalSet);

      
   private:
      
      list<Interval> mListOfInterval;
      int            mLength;
};


#endif

