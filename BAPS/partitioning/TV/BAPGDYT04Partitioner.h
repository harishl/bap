#include "def.h"
#include "BAPPackage.h"
#include "BAPPartitioner.h"
#include "BAPTVSection.h"
#include "BAPTVVessel.h"
#include <LEDA/core/list.h>
#include <LEDA/core/random_source.h>
#include <sys/time.h>
#include </usr/include/time.h>
#include <LEDA/core/_p_queue.h>

using leda::random_source;
using leda::p_queue;
 
// Using LEDA's random source type
typedef random_source Random;


class BAPGDYT04Partitioner : public BAPPartitioner
{
public:
   BAPGDYT04Partitioner(BAPPackage& aPackage);
   ~BAPGDYT04Partitioner();

   void Solve();
   // Debugging facility
   void Print(const int& aWidth = 1, const int& aDetail = 0) const;
//   int compareArrival(const TVVessel& aV1, const TVVessel& aV2);

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
   void Assign(TVVessel& v, TVSection& s);
   void AssignVesselToRandomSection(TVVessel& v);
   void CalcInitialObjVal();
   unsigned long CalcObjVal() const;
   void ComputeObjVal(unsigned long& aTrans, unsigned long& aPenalty) const;
   void GenerateInitialSolution();
   void GenSolnGreedy();
   inline bool GotMoreMoves() const;
   void InitSolution();
   void ResetVesselDestinations();
   void UpdatePackage() const;

   unsigned int TotalFlow(const int& v1, const int& v2) const;
   unsigned int TotalFlow(const TVVessel& v1, const TVVessel& v2) const;
   unsigned int D(const int& s1, const int& s2) const;
   unsigned int D(const TVSection& s1, const TVSection& s2) const;

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
   array<TVVessel>  mVes;
   array<TVSection> mSect;
   array2<int>    mTrans;
   array2<int>    mDist;
   set<int>       mUnallocVes;
   Random         mRandom;
};

