/******************************************************************
 *
 * Filename    : BAPPackage.h
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.02b
 * Date        : 14 May 99
 *
 * Description : Interface file for problem package class.
 *
 * Reference   : nil
 *
 * Notes       : This class interfaces the algorithm functions with
 *               the BAP domain classes.  It has to encapsulate all
 *               necessary information for solving the problem, based
 *               on our problem model:
 *               - the set of sections S
 *               - the set of vessels  V
 *               - the intersection distance D
 *               - the transhipment flows    F
 *
 * Changes     : 
 *               14 May 99
 *               Add ReadSolution() and WriteSolution() methods
 *               Add mSolutionExists, mObjectiveValue and mPenalty attributes
 *               Add VerifySolution() method
 *
 * Copyright   : Copyright (c) 1998, 1999
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 ******************************************************************/


#ifndef __BAP_PACKAGE__
#define __BAP_PACKAGE__

#include "def.h"
#include "BAPBase.h"

using leda::string;
using leda::set;
using leda::array;
using leda::array2;

//
// Structures are used to pass mass information to the user
//
struct Indicators
{
   double   ObjectiveValue;
   double   Penalty;
   double   TranshipmentCost;
   double   Density;
   double   ABD;
};


class BAPPackage : public BAPBase
{
public:
   // This class takes in either
   // (i)   a project filename
   // (ii)  section, vessel and transshipment filenames
   // and constructs the necessary attributes and data structures for
   // storing the essential information
   BAPPackage(string aProjFile);
   BAPPackage(string aSectFile, string aVesFile, string aTransFile);
   ~BAPPackage();

   //
   // Debugging facility
   //
   // prints the current package information using the given indentation
   // width and in the given detail level
   void        Print(int aWidth = 1, int aDetail = 0) const;

   //
   // Accessor member functions
   //

   //
   // Sections S info
   //
   // returns the number of sections
   int         NumSections() const;
   // returns an array A of section lengths,
   // such that A[i] = length of section i
   array<int>  SectionLengths() const;

   //
   // Vessels V info
   //
   // returns the number of vessels
   int         NumVessels() const;
   // returns an array A of vessel lengths,
   // such that A[i] = length of vessel i
   array<int>  VesselLengths() const;
   // returns an array A of vessel arrival times,
   // such that A[i] = arrival time of vessel i
   array<int>  Arrivals() const;
   // returns an array A of vessel departure times,
   // such that A[i] = departure time of vessel i
   array<int>  Departures() const;

   //
   // Intersection Distances D info
   //
   // returns a 2-dimensional array A of intersection distances,
   // such that A(k, l) = distance from section k to section l
   array2<int> Distances() const;

   //
   // Transhipment Flows F info
   //
   // returns a 2-dimensional array A of transshipment flows,
   // such that A(i, j) = number of containers being exchanged between
   // vessel i and vessel j
   array2<int> Transhipments() const;

   //
   // Solution Information
   //
   // returns an array A of (section, wharfmark) info,
   // so A[i].First() = section assigned to vessel i
   //    A[i].Second() = wharfmark assigned to vessel i
   array<IntPair> Solution() const;
   // returns the section assigned to the given vessel
   int            SectionAssignedTo(int aVesselID) const;
   // returns the wharfmark assigned to the given vessel
   int            WharfmarkAssignedTo(int aVesselID) const;

   //
   // Solution Indicators
   //
   // returns the objective value
   unsigned long  ObjectiveValue() const;
   // returns the penalty value
   unsigned long  Penalty() const;
   // returns the transshipment cost of the current solution
   unsigned long  TranshipmentCost() const;
   // returns the density of the current vessel set
   double         Density() const;
   // returns the ABD (Average Berth Demand) of the current vessel set
   double         ABD() const;

   //
   // Useful Information
   //
   // returns true if existing solution is found, false otherwise
   bool           SolutionExists() const;
   // returns the number of time zones wrt to the current vessel set
   int            NumTimeZones() const;
   // returns an array A of time zone info,
   // such that A[i].First() = start time of time zone i
   //           A[i].End() = end time of time zone i
   array<IntPair> TimeZones() const;
   // returns the set of time zones spanned by the given vessel
   set<int>       TimeZones(int aVesselID) const;
   // returns the set of vessels in the given time zone
   set<int>       Vessels(int aTimeZoneID) const;
   // returns the start (first) time zone of the given vessel
   int            StartTimeZone(int aVesselID) const;
   // returns the end (last) time zone of the given vessel
   int            EndTimeZone(int aVesselID) const;
   // returns the arrival time of the given vessel
   int            Arrival(int aVesselID) const;
   // returns the departure time of the given vessel
   int            Departure(int aVesselID) const;

   // returns the BAPPath specified by shell variable $BAPHOME
   string         BAPPath() const;
   // returns the project filename
   string         ProjectFilename() const;
   // returns the parameter filename
   string         ParamFilename() const;
   // returns the partitioning solution filename
   string         PartitioningFilename() const;
   // returns the packing solution filename
   string         PackingFilename() const;
   // returns the partitioning trace file name
   string         PartitioningTraceFilename() const;
   // returns the packing trace file name
   string         PackingTraceFilename() const;

   // writes the solution
   void           WriteSolution(string aCreator, string aParamFile) const;
   // verifies the solution
   void           VerifySolution() const;
   // analyzes the solution
   void           AnalyzeSolution() const;

   //
   // Modifier member functions
   //

   // reads the solution
   void           ReadSolution();
   // reads the specific solution
   void           ReadSolution(string aSolFile);

   //
   // Functions to do assignment of vessels
   //
   // assigns the given vessel to the given section
   int      SectionAssignedTo(int aVesselID, int aSectionID);
   // assigns the given vessel to the given wharfmark
   int      WharfmarkAssignedTo(int aVesselID, int aWharfmark);

   //
   // Change individual indicators
   //
   // sets the objective value
   unsigned long  ObjectiveValue(unsigned long anObjValue);
   // sets the penalty value
   unsigned long  Penalty(unsigned long aPenalty);
   // sets the transshipment cost to the given cost
   unsigned long  TranshipmentCost(unsigned long aCost);
   // sets the density to the given density
   double   Density(double aDensity);
   // sets the ABD to the given ABD
   double   ABD(double aDemand);


private:
   //
   // Modifier member functions
   //
   void     ComputeTimeZones();
   void     Input(string aSectFile, string aVesFile, string aTransFile);

   string const    mProjectFilename;
   string          mBAPPath;
   string          mParamFilename;
   string          mPartitioningFilename;
   string          mPackingFilename;
   string          mPartitioningTraceFilename;
   string          mPackingTraceFilename;
   bool            mSolutionExists;

   // sections S
   array<int>     *mSectionLengths;

   // vessels V
   array<int>     *mVesselLengths;
   array<int>     *mArrivals;
   array<int>     *mDepartures;

   // intersection distances D
   array2<int>    *mDistances;

   // transhipment flows F
   array2<int>    *mTranshipments;

   // solution
   array<IntPair> *mSolution;    // (section, wharfmark) pair

   // useful information
   array<IntPair> *mTimeZones;   // (start time, end time) pair
   set<int>       *mVesselsInTimeZone;
   set<int>       *mTimeZonesByVessel;

   // indicators
   unsigned long   mObjectiveValue;
   unsigned long   mPenalty;
   unsigned long   mTranshipmentCost;
   double          mDensity;
   double          mABD;

   int             mUnallocatedVessels;
};


#endif
