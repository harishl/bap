#include "BAPTVPartitioner.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::setprecision;



BAPTVPartitioner::BAPTVPartitioner(BAPPackage& aPackage)
:  BAPPartitioner(aPackage),           // init base class
   mNumVes(aPackage.NumVessels()),
   mVes(array<TVVessel>(1, mNumVes)),
   mNumSect(aPackage.NumSections()),
   mSect(array<TVSection>(1, mNumSect)),
   mTrans(aPackage.Transhipments()),
   mDist(aPackage.Distances()),
   mTraceFile(mPackage.PartitioningTraceFilename()),
   mRuntimeAnalyzer(0),
   mSolnExists(false)
{
   gettimeofday(&mStartTime, NULL);
   array<int>  temp;
   
   // Init vessel array with vessel objects
   temp = aPackage.VesselLengths();
   
   for (int i = 1; i <= mNumVes; i++)
   {
      TVVessel v(i, temp[i]);
      v.StartTimeZone(aPackage.StartTimeZone(i));
      v.EndTimeZone(aPackage.EndTimeZone(i));
      v.Arrival(aPackage.Arrival(i));
      v.Departure(aPackage.Departure(i));
      
      // find all neighbouring vessels
      for (int j = 1; j <= mNumVes; j++)
      {
         if (j == i) continue;         // ignore self loop
         
         if (mTrans(i, j) > 0  ||  mTrans(j, i) > 0)
         {
            v.AddNeighbour(j);
            v.AddTranshipment(mTrans(i, j));
            v.AddTranshipment(mTrans(j, i));
         }
      }

      v.Export(mTrans(0, i));
      v.Import(mTrans(i, 0));
      mVes[i] = v;
   }

   // Init section array with section objects
   temp = aPackage.SectionLengths();
   
   for (int i = 1, t = aPackage.NumTimeZones(); i <= mNumSect; i++)
      mSect[i] = TVSection(i, t, temp[i]);

   ReadParameterFile();
}


BAPTVPartitioner::~BAPTVPartitioner()
{
   // nothing to do
}


void BAPTVPartitioner::Print(const int& aWidth,
                             const int& aDetail) const
{
   cout  << "--== BAPTVPartitioner ==--" << endl
         << "  ID = " << ID() << endl
         << "  Name = " << Name() << endl
         << "  Number of sections = " << mNumSect
         << ", Number of vessels = " << mNumVes << endl
         << "  Random seed = " << mRandomSeed << endl
         << "  Transhipment = " << mTranshipment
         << ", Penalty = " << mPenalty << endl << endl;

   for (int i = 1; i <= mNumVes; i++)
      if (aDetail > 0)
         mVes[i].Print(aWidth, aDetail);
      else
         cout  << "ves " << setw(4) << i
               << " in sect " << setw(2) << mVes[i].Section() << endl;

   for (int k = 1; k <= mNumSect; k++)
      if (aDetail > 0)
         mSect[k].Print(aWidth, aDetail);
      else
      {
         cout  << "sect " << setw(2) << k << " with vessels: " << endl;
         int   i;
         const set<int>& V = mSect[k].Vessels();
         forall(i, V)
         {
            cout << i << " ";
         }
         cout << endl;
      }
}


// The Solve method is called to perform
// the vessel partitioning algorithm.
//   this version calls GenerateInitialSolution
//    (which creates a random initial solution)
//
void BAPTVPartitioner::Solve()
{

   GenerateInitialSolution();
   CalcInitialObjVal();

   gettimeofday(&mEndTime, NULL);

   PrintSummary();
   WriteSolutionFile();
   UpdatePackage();
}


void BAPTVPartitioner::PrintSummary() const
{
   cout  << mPackage.ProjectFilename() << " --- "
         << mNumVes << " vessels, " << mNumSect << " sections" << endl
         << "Transhipment = " << mTranshipment << endl
         << "Penalty      = " << mPenalty << endl
         << "Obj Value    = " << CalcObjVal() << endl
         << "Time taken (sec) = "
         << (mEndTime.tv_sec - mStartTime.tv_sec) +
            (mEndTime.tv_usec - mStartTime.tv_usec) / 1000000.0
         << endl
         << endl;
}



void BAPTVPartitioner::InitSolution()
{
   // Init working variables
   mTranshipment = 0;
   mPenalty = 0;
   mUnallocVes.clear();
   for (int i = 1; i <= mNumVes; i++)
   {
      mVes[i].Section(UNASSIGNED);   // all vessels unassigned
      mUnallocVes.insert(i);
   }

   ResetVesselDestinations();
}

void BAPTVPartitioner::ResetVesselDestinations()
{
   // Add potential destination sections to each vessel
   for (int i = 1; i <= mNumVes; i++)
      for (int k = 1; k <= mNumSect; k++)
         if (k != mVes[i].Section())
            mVes[i].AddDestination(k);
}


void BAPTVPartitioner::CalcInitialObjVal()
{
   ComputeObjVal(mTranshipment, mPenalty);
}


void BAPTVPartitioner::ComputeObjVal(unsigned long& aTrans,
                                     unsigned long& aPenalty) const
{
   aTrans = aPenalty = 0;

   for (int v1 = 1; v1 <= mNumVes; v1++)
   {
      const int&        s1 = mVes[v1].Section();
      const set<int>&   neighbours = mVes[v1].Neighbours();
      int               v2;

      // Calculate inter-vessel transhipment costs
      forall(v2, neighbours)
      {
         if (v2 < v1) continue;

         const int&     s2 = mVes[v2].Section();
         
         if (s1 == UNASSIGNED  ||  s2 == UNASSIGNED)
            aPenalty += TotalFlow(v1, v2) * LONGDISTANCE;
         else
         {
            aTrans += TotalFlow(v1, v2) * D(s1, s2);
//            cout  << setw(4) << v1 << setw(5) << v2
//                  << setw(10) << TotalFlow(v1, v2) << " * "
//                  << setw(5) << D(s1, s2) << " = "
//                  << TotalFlow(v1, v2) * D(s1, s2)
//                  << "     Trans = " << aTrans << endl;
         }
      }

      // Calculate import,export costs
      if (s1 == UNASSIGNED)
         aPenalty += (mVes[v1].Import() + mVes[v1].Export()) * LONGDISTANCE;
      else
      {
         aTrans += (mVes[v1].Import() + mVes[v1].Export()) * D(s1, 0);
//            cout  << setw(4) << v1 << " ("
//                  << setw(5) << mVes[v1].Import() << " + "
//                  << setw(5) << mVes[v1].Export() << ") * "
//                  << setw(5) << D(s1, 0) << " = "
//                  << (mVes[v1].Import() + mVes[v1].Export()) * D(s1, 0)
//                  << "     Trans = " << aTrans << endl;
      }
   }
}


unsigned long BAPTVPartitioner::CalcObjVal() const
{
   unsigned long  ObjVal = mTranshipment + mPenalty;

   // Safeguards against overflow
   assert(ObjVal >= mTranshipment);
   assert(ObjVal >= mPenalty);

   return ObjVal;
}

unsigned int BAPTVPartitioner::TotalFlow(const int& v1, const int& v2) const
{
   return (unsigned int) (mTrans(v1, v2) + mTrans(v2, v1));
}

unsigned int BAPTVPartitioner::TotalFlow(const TVVessel& v1, const TVVessel& v2) const
{
   return (unsigned int) (mTrans(v1.ID(), v2.ID()) + mTrans(v2.ID(), v1.ID()));
}

unsigned int BAPTVPartitioner::D(const int& s1, const int& s2) const
{
   if (s1 >= 0  &&  s2 >= 0)           // take port into account also
      return (unsigned int) mDist(s1, s2);
   else
      return LONGDISTANCE;
}

unsigned int BAPTVPartitioner::D(const TVSection& s1, const TVSection& s2) const
{
   if (s1.ID() >= 0  &&  s2.ID() >= 0) // take port into account also
      return (unsigned int) mDist(s1.ID(), s2.ID());
   else
      return LONGDISTANCE;
}


void BAPTVPartitioner::UpdatePackage() const
{
   for (int i = 1; i <= mNumVes; i++)
      mPackage.SectionAssignedTo(i, mVes[i].Section());
   
   mPackage.TranshipmentCost(mTranshipment);

   // What happened to the penalty?!?
}


void BAPTVPartitioner::ReadParameterFile()
{
   string   paramFile = mPackage.ParamFilename();
   char     buf[255];
   string   token, mode = "nil";
   ifstream ParamFile(paramFile);

   if (!ParamFile)
   {
      cerr << "Cannot open parameter file: " << paramFile << endl;
      return;
   }

   while (!ParamFile.eof())
   {
      ParamFile.getline(buf,80);

#ifdef _DEBUG
         cout << "tokenizing: " << buf << endl;
#endif

      for (int i = 0; i < 80; i++)     // Convert carriage-return to space
         if (13 == (int) buf[i])
            buf[i] = ' ';

      if (buf[0] == ' ' || buf[0] == 0)
         continue;

      token = strtok(buf, " ");

#ifdef _DEBUG
      cout << "token = " << token << endl;
#endif

      if (token.length() == 0)         // empty line, ignore
         continue;
      if (token == "#")                // comment line, ignore
         continue; 
      if (token[0] == '_')             // keyword, change mode
         mode = token;
      else                             // process data based on mode
      {
         if (mode == "_RANDOM_SEED")
         {
            mRandomSeed = atoi(token);
            mRandom.set_seed(mRandomSeed);
         }  
         else if (mode == "_PRINT_SECTIONS")
         {
            mPrintSections = atoi(token);
         }
         else if (mode == "_PRINT_VESSELS")
         {
            mPrintVessels = atoi(token);
         }
         else if (mode == "_RUNTIME_ANALYZER")
         {
            mRuntimeAnalyzer = atoi(token);
         }
         else if (mode == "_SUMMARY")
         {
            mSummary = atoi(token);
         }
         // else nothing
      }  
   }
}


void BAPTVPartitioner::WriteSolutionFile() const
{
   string   solutionFile = mPackage.PartitioningFilename(),
            paramFile = mPackage.ParamFilename();

   ofstream SolFile(solutionFile);

   if (!SolFile)
   {
      cerr << "Cannot create solution file: " << solutionFile << endl;
      return;
   }

   // Write comments
   // ( Date() auto-inserts a carriage-return! )
   SolFile  << "#" << endl
            << "# Solution File created by BAPTVPartitioner" << endl
            << "# Date = " << Date()
            << "# Parameter File = " << paramFile << endl
            << "#" << endl << endl;

   SolFile  << "_NUM_UNALLOCATED" << endl
            << mUnallocVes.size() << endl << endl;

   // Reads from the BAP package
   const int V = mNumVes;

   // Write solution information
   SolFile.setf(ios::fixed);
   SolFile  << "_NUM_VESSELS" << endl
            << V + 1 << endl           // n+1 to account for port
            << endl
            << "_OBJECTIVE_VALUE" << endl
            << setprecision(2) << mTranshipment + mPenalty << endl
            << endl
            << "_TRANSHIPMENT_VALUE" << endl
            << setprecision(2) << mTranshipment << endl
            << endl
            << "_PENALTY_VALUE" << endl
            << setprecision(2) << mPenalty << endl
            << endl
            << "_TIME_TAKEN" << endl
            << setprecision(6)
            << (mEndTime.tv_sec - mStartTime.tv_sec) +
               (mEndTime.tv_usec - mStartTime.tv_usec) / 1000000.0
            << endl
            << endl;
   SolFile.setf(ios::fixed, ios::floatfield);

   // Write allocated vessels
   array<int>  Arrivals = mPackage.Arrivals(),
               Departures = mPackage.Departures();

   SolFile  << "# Final Allocation Solution" << endl
            << "# <ves#> <sect> <wharf> <berth time> <departure time>"
            << endl << endl
            << "_ALLOCATION" << endl;

   SolFile  << "0 0 0 0 0" << endl;    // Write the sea port

   for (int i = 1; i <= V; i++)
      if (mVes[i].Section() > 0)
         SolFile  << i << " " << mVes[i].Section() << " " << -1
                  << " " << Arrivals[i] << " " << Departures[i]
                  << endl;

   // Write unallocated vessels, if any
   if (mUnallocVes.size() > 0)
   {
      SolFile  << endl << "_UNALLOCATED_VESSELS" << endl;

      for (int i = 1; i <= V; i++)
         if (mVes[i].Section() < 0)
            SolFile  << i << " " << -1 << " " << -1
                     << " " << Arrivals[i] << " " << Departures[i]
                     << endl;
   }

   SolFile << endl << endl;

   SolFile.close();
}


void BAPTVPartitioner::WriteTraceFile(string aStr) const
{
   ofstream TraceFile(mTraceFile, ios::app);
   TraceFile  << aStr << endl;
   TraceFile.close();
}

void BAPTVPartitioner::WriteTraceFile(long aLong) const
{
   ofstream TraceFile(mTraceFile, ios::app);
   TraceFile   << aLong << endl;
   TraceFile.close();
}

void BAPTVPartitioner::WriteTraceFile(long double aDouble) const
{
   ofstream TraceFile(mTraceFile, ios::app);
   TraceFile.setf(ios::fixed);
   TraceFile  << setprecision(2) << aDouble << endl;
   TraceFile.close();
}


string BAPTVPartitioner::Date() const
{
   //
   // Query date and time
   //
   struct tm   *TimePtr;
   time_t      Time;

   time(&Time);
   TimePtr = localtime(&Time);

   return string(asctime(TimePtr));
}

// GenerateInitialSolution creates a vessel partitioning solution. 
// At the moment, it creates a random initial solution.
//   Called by: Solve();
//
void BAPTVPartitioner::GenerateInitialSolution()
{
   InitSolution();
   GenSolnRandom();
}


// This method actually create the random vessel partitioning
// solution. Each vessel is assigned a random section
// At the moment, it creates a random initial solution.
//   Called by: Solve();
void BAPTVPartitioner::GenSolnRandom()
{
   // List of candidate vessels for allocation
   array<int>  V(1, mNumVes);
   
   for (int i = 1; i <= mNumVes; i++)
      V[i] = i;                        // set content to vessel ID

   // Loop to do actual allocation
   int   i, VesselsLeft = mNumVes;

   while (VesselsLeft > 0)
   {
      i = mRandom(1, VesselsLeft);     // randomly pick a vessel
      int         vIndex = V[i];
      TVVessel&   v = mVes[vIndex];
      AssignVesselToRandomSection(v);
      // remove assigned vessel by replacing it with last vessel
      V[i] = V[VesselsLeft];
      VesselsLeft--;
   }
}


// Assigns Vessel v to a random section
//   Alg: save the candidate destination sections
//        for each randomly selected section
//        assign to it if there is room for it
//        else remove the section.
//   Note:vessel is unassigned if all sections fails.
//
void BAPTVPartitioner::AssignVesselToRandomSection(TVVessel& v)
{
   // List of candidate sections to be assigned to v
   set<int> sectSet = v.Destinations();   // Copy, preserve original
   array<int> S(1, sectSet.size());       // Array of destination sections
   int   i = 1, k;
   
   while (!sectSet.empty())
   {
      k = sectSet.choose();
      sectSet.del(k);
      S[i++] = k;
   }

   // Loop to do actual assignment
   int   SectionsLeft = S.size();

   while (SectionsLeft > 0  &&  v.Section() == UNASSIGNED)
   {
      i = mRandom(1, SectionsLeft);  // randomly pick a section
      int      k = S[i];
      
      if (mSect[k].CanAccommodate(v))
         Assign(v, mSect[k]);
      else
         S[i--] = S[SectionsLeft--]; // remove from further consideration
   }
}


void BAPTVPartitioner::Assign(TVVessel& v, TVSection& s)
{
   v.Section(s.ID());
   v.RemoveDestination(s.ID());
   s.Add(v);
   mUnallocVes.del(v.ID());
}


