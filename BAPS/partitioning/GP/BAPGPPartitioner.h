#include "def.h"
#include "BAPPackage.h"
#include "BAPPartitioner.h"
#include "BAPGPSection.h"
#include "BAPGPVessel.h"
#include "/home/harish/bapm3/LEDA/incl/LEDA/core/list.h"
#include "/home/harish/bapm3/LEDA/incl/LEDA/core/array.h"
#include "/home/harish/bapm3/LEDA/incl/LEDA/core/random_source.h"
#include <sys/time.h>
#include </usr/include/time.h>

using leda::random_source;
using leda::list;
using leda::array;

// Using LEDA's random source type
typedef random_source Random;

class TP {
public:
	int timePoint;
	bool isArrival;
	int vesselNumber;
};

class TimeZone {
public:
	int start;
	int end;
};

class BAPGPPartitioner : public BAPPartitioner
{
public:
   BAPGPPartitioner(BAPPackage& aPackage);
   ~BAPGPPartitioner();

   void Solve();
   // Debugging facility
   void Print(const int& aWidth = 1, const int& aDetail = 0) const;

private:
   // I/O methods
   string Date() const;
   void PrintSummary() const;
   void ReadParameterFile();
   void WriteSolutionFile() const;
   void WriteTraceFile(string aStr) const;
   void WriteTraceFile(long aLong) const;
   void WriteTraceFile(long double aDouble) const;

   // Command-line methods
   // Initial solution generation methods

   // Abstractions
   void Assign(GPVessel& v, GPSection& s);
   void AssignVesselToRandomSection(GPVessel& v);
   void CalcInitialObjVal();
   unsigned long CalcObjVal() const;
   void ComputeObjVal(unsigned long& aTrans, unsigned long& aPenalty) const;
   void GenerateInitialSolution();
   //void PlaneSweep();
   void GenSolnRandom();
   void GenSolnZoneDensity();
   inline bool GotMoreMoves() const;
   void InitSolution();
   void ResetVesselDestinations();
   void UpdatePackage() const;

   unsigned int TotalFlow(const int& v1, const int& v2) const;
   unsigned int TotalFlow(const GPVessel& v1, const GPVessel& v2) const;
   unsigned int D(const int& s1, const int& s2) const;
   unsigned int D(const GPSection& s1, const GPSection& s2) const;

   // Constants
   static const unsigned int  LONGDISTANCE = INFINITY_BAP;
   static const int           UNASSIGNED = -1;

   // Debug control
   unsigned int   mOutput;
   unsigned int   mPrintBucket;
   unsigned int   mPrintSections;
   unsigned int   mPrintVessels;
   unsigned int   mRuntimeAnalyzer;
   unsigned int   mSummary;
   string         mTraceFile;

   // Algorithm control
   timeval        mStartTime, mEndTime;
   int            mRandomSeed;
   unsigned int   mMaxPasses;
   unsigned int   mInitialSolnGenerator;
   bool           mSolnExists;

   // Information
   unsigned long  mTranshipment;
   unsigned long  mPenalty;

   // Data structures
   const int      mNumVes;
   const int      mNumSect;
   array<GPVessel>  mVes;
   array<GPSection> mSect;
   array2<int>    mTrans;
   array2<int>    mDist;
   set<int>       mUnallocVes;
   Random         mRandom;
};

