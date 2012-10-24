/******************************************************************
 *
 * Filename    : BAPPackage.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.02b
 * Date        : 14 May 99
 *
 * Description : This file implements the package class.
 *
 * Reference   : nil
 *
 * Notes       : This class interfaces the algorithm functions with
 *               the BAP domain classes.  It has to encapsulate all
 *               necessary information for solving the problem, based
 *               on our problem model:
 *               - the set of sections       S
 *               - the set of vessels        V
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


#include "BAPPackage.h"

using std::istream;
using std::ostream;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::setprecision;
using leda::string;
using leda::queue;

//
// Support routines for return structures defined in header file
// Required by LEDA
//
istream& operator>>(istream& aIS, Indicators& aI)
{
   aIS >> aI.TranshipmentCost >> aI.Density >> aI.ABD;
   return aIS;
}

ostream& operator<<(ostream& aOS, const Indicators& aI)
{
   aOS << "(" << aI.TranshipmentCost << ", "
              << aI.Density << ", " << aI.ABD << ")";
   return aOS;
}

// DOTW : 5 May 2000
//
// Comment out these two functions for LEDA 3.5.2
//istream& operator>>(istream& aIS, set<int>& aS)
//{
//   return aIS;
//}
//
//ostream& operator<<(ostream& aOS, const set<int>& aS)
//{
//   return aOS;
//}


//
// Implementation of the package class
//
BAPPackage::BAPPackage(string aProjFile)
: BAPBase(), mSectionLengths(0), mVesselLengths(0), mArrivals(0),
  mDepartures(0), mDistances(0), mTranshipments(0), mSolution(0),
  mTimeZones(0), mVesselsInTimeZone(0), mTimeZonesByVessel(0),
  mDensity(0), mABD(0), mProjectFilename(aProjFile),
  mTranshipmentCost(0), mBAPPath(getenv("BAPHOME")),
  mSolutionExists(false), mObjectiveValue(0), mPenalty(0)
{
#ifdef _DEBUG
   cout << "mBAPPATH = " << mBAPPath << endl;
#endif

   string   ProjectFile = mBAPPath + "/project/" + aProjFile;

   char     buf[80];
   string   token, mode = "nil", SectFile, VesFile, TransFile;
   ifstream ProjFile(ProjectFile.c_str());

   if (!ProjFile)
   {
      cerr << "Cannot open project file: " << ProjectFile << endl;
      exit (-1);
   }

#ifdef _DEBUG
   cout << "Reading project file..." << endl;
#endif

   while (!ProjFile.eof())
   {
      ProjFile.getline(buf, 80);

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

      if ((token.length() == 0) || (token == "#"))
         continue;                     // Ignore blanks and comments
      if (token[0] == '_')             // Keyword found:
         mode = token;                 //   mode = keyword
      else
      {
#ifdef _DEBUG
         int result = (mode == "_DATASET");
         cout << "mode = " << mode << ", result = " << result << endl;
#endif

         if (mode == "_DATASET")
         {
            assert(token[0] == 'd');   // Must start with 'd'

            string         prefix = mBAPPath + "/database/" + token;

#ifdef _DEBUG
            cout << "prefix = " << prefix << endl;
#endif

            DIR            *dirp;
            struct dirent  *direntp;

            dirp = opendir(prefix.c_str());

            while ((direntp = readdir(dirp)) != NULL)
            {
               string   filename(direntp->d_name);

#ifdef _DEBUG
               cout << "filename = " << filename << endl;
#endif

               if ("sect" == filename.head(4))
                  SectFile = prefix + "/" + filename;
               else if ("ves" == filename.head(3))
                  VesFile = prefix + "/" + filename;
               else if ("trans" == filename.head(5))
                  TransFile = prefix + "/" + filename;
            }
            closedir(dirp);
         }
         else if (mode == "_PART_SOLN_FILENAME")
         {
            mPartitioningFilename = mBAPPath + "/solution/" + token;
         }
         else if (mode == "_PARAM_FILENAME")
         {
            mParamFilename = mBAPPath + "/param/" + token;
         }
         else if (mode == "_PACK_SOLN_FILENAME")
         {
            mPackingFilename = mBAPPath + "/solution/" + token;
         }
         else if (mode == "_PART_TRACE_FILENAME")
         {
            mPartitioningTraceFilename = mBAPPath + "/log/" + token;
         }
         else if (mode == "_PACK_TRACE_FILENAME")
         {
            mPackingTraceFilename = mBAPPath + "/log/" + token;
         }
      }
   }

#ifdef _DEBUG
   cout << "done" << endl;
   cout << "SectFile = " << SectFile << ", VesFile = " << VesFile
        << ", TransFile = " << TransFile << endl;
#endif

   Input(SectFile, VesFile, TransFile);

   // Initialize solution
   int   V = mVesselLengths->high();
   mSolution = new array<IntPair>(0, V);

   for (int i = 0; i <= V; i++)
   {
      (*mSolution)[i].First(-1);
      (*mSolution)[i].Second(-1);
   }

   // Initialize indicators
   mTranshipmentCost = 0;
   mDensity = 0;
   mABD = 0;

   ComputeTimeZones();
}

BAPPackage::BAPPackage(string aSectFile, string aVesFile, string aTransFile)
: BAPBase(), mSectionLengths(0), mVesselLengths(0), mArrivals(0),
  mDepartures(0), mDistances(0), mTranshipments(0), mSolution(0),
  mTimeZones(0), mVesselsInTimeZone(0), mTimeZonesByVessel(0),
  mDensity(0), mABD(0), mSolutionExists(false), mObjectiveValue(0),
  mPenalty(0)
{
   Input(aSectFile, aVesFile, aTransFile);

   // Initialize solution
   int   V = mVesselLengths->high();
   mSolution = new array<IntPair>(0, V);

   for (int i = 0; i <= V; i++)
   {
      (*mSolution)[i].First(-1);
      (*mSolution)[i].Second(-1);
   }

   // Initialize indicators
   mTranshipmentCost = 0;
   mDensity = 0;
   mABD = 0;

   ComputeTimeZones();
}


BAPPackage::~BAPPackage()
{
//   delete mSectionLengths;
//   delete mDistances;
//   delete mVesselLengths;
//   delete mArrivals;
//   delete mDepartures;
//   delete mTranshipments;
//   delete mSolution;
}


void BAPPackage::ComputeTimeZones()
{
   int   V = NumVessels();
   int   minTime = MAXINT, maxTime = 0;
   int   i, j;

   // Locate minimum and maximum time in schedule
   for (i = 1; i <= V; i++)
   {
      if ((*mArrivals)[i] < minTime)
         minTime = (*mArrivals)[i];
      if ((*mDepartures)[i] > maxTime)
         maxTime = (*mDepartures)[i];
   }

   // Array to store (TimePoint, VesselID, flag) for both arrivals and
   // departures (so times 2)
   // flag = True if TimePoint is an arrival, False otherwise
   array<Triple>  timePoints(V*2);

   for (i = 1, j = 0; i <= V; i++, j += 2)
   {
      timePoints[j].Time      = (*mArrivals)[i];
      timePoints[j].ID        = i;
      timePoints[j].Arrive    = true;

      timePoints[j+1].Time    = (*mDepartures)[i];
      timePoints[j+1].ID      = i;
      timePoints[j+1].Arrive  = false;
   }

   timePoints.sort();                  // According to time points

   // Initialize plane sweep with first time point
   bool  leaving = false;              // To locate an arrival
   int   start = timePoints[0].Time, end, TZIndex = 0;
   int   index;
   typedef set<int> VesselSet, TZSet;
   VesselSet vSet;                     // To store vessel IDs
   vSet.insert(timePoints[0].ID);
   queue<IntPair>    tzQueue;          // To store time zones
   queue<VesselSet>  tzVessels;        // Vessels in time zone
   array<TZSet>      tzSpanned(0, V);  // Time zones spanned by vessel

   // Sweep across remaining time points to locate time zones
   for (i = 1; i <= timePoints.high(); i++)
   {
      index = i;

      if (timePoints[i].Arrive)        // Found an arrival
      {
         if (leaving)
         {
            // Was scanning departures, so start new time zone
            ++TZIndex;
            end = timePoints[i].Time;

            // Add new time zone
            tzQueue.append(IntPair(start, end));

            // Mark all vessels in this time zone, and
            // mark time zone spanned by each vessel
            int      v;
            set<int> tempSet, carrySet;

            forall(v, vSet)
            {
               tempSet.insert(v);
               tzSpanned[v].insert(TZIndex);

               if ((*mDepartures)[v] > end)
                  carrySet.insert(v);
            }

            tzVessels.append(tempSet);

            start = end;               // new start time
            leaving = false;
            vSet = carrySet;
            // Remember to insert current vessel being scanned
            vSet.insert(timePoints[i].ID);
         }
         else
         {
            // Was scanning arrivals, just add vessel in
            vSet.insert(timePoints[i].ID);
         }
      }
      else                             // Found a departure
      {
         leaving = true;               // switch to scanning departures

         // Just add vessel
         vSet.insert(timePoints[i].ID);
         end = timePoints[i].Time;
      }
   }

   if (vSet.size() > 0)                // last time zone of vessels
   {
      ++TZIndex;
      tzQueue.append(IntPair(start, end));

      int      v;
      set<int> tempSet;

      forall(v, vSet)
      {
         tempSet.insert(v);
         tzSpanned[v].insert(TZIndex);
      }

      tzVessels.append(tempSet);
   }

   // Initialize our time zone array
   int   TZ = tzQueue.size();

   mTimeZones = new array<IntPair>(1, TZ);

   for (i = 1; i <= TZ; i++)
   {
      IntPair  p = tzQueue.pop();
      int   s = p.Start(), e = p.End();

      (*mTimeZones)[i].First(s);
      (*mTimeZones)[i].Second(e);
   }

   // Initialize vessels in current time zone array
   mVesselsInTimeZone = new set<int>[TZ+1];

   for (i = 1; i <= TZ; i++)
   {
      VesselSet   VSet = tzVessels.pop();
      int         v;

      forall(v, VSet)
         mVesselsInTimeZone[i].insert(v);
   }

   // Initialize time zones spanned by vessel array
   mTimeZonesByVessel = new set<int>[V+1];

   for (i = 1; i <= V; i++)
   {
      mTimeZonesByVessel[i] = tzSpanned[i];
   }
}


void BAPPackage::Input(string aSectFile, string aVesFile, string aTransFile)
{
   char     buf[80];
   int      V = 0, S = 0;
   string   token, mode;


   ifstream SectFile(aSectFile.c_str()), VesFile(aVesFile.c_str()), TransFile(aTransFile.c_str());


   if (!SectFile)
   {
      cerr << "Cannot open section file: " << aSectFile << endl;
      exit (-1);
   }
   if (!VesFile)
   {
      cerr << "Cannot open vessel file: " << aVesFile << endl;
      exit (-1);
   }
   if (!TransFile)
   {
      cerr << "Cannot open transhipment file: " << aTransFile << endl;
      exit (-1);
   }

   // Read section file
   mode = "nil";

   while (!SectFile.eof())
   {
      SectFile.getline(buf,80);

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

      if ((token.length() == 0) || (token == "#"))
         continue;
      if (token[0] == '_')
         mode = token;
      else
      {
         if (mode == "_NUM_SECT")
         {
            S = atoi(token) - 1;
            mSectionLengths = new array<int>(0, S);
            mDistances = new array2<int>(0, S, 0, S);
         }
         else if (mode == "_SECTION")
         {
            int   i = atoi(token);
            if (i == 0)
               continue;
            token = strtok(NULL, " "); // now at section name
            token = strtok(NULL, " "); // now at section length
            (*mSectionLengths)[i] = atoi(token);
         }
         else if (mode == "_INTERSECT_DIST")
         {
            int   i = atoi(token);
            token = strtok(NULL, " ");
            int   j = atoi(token);
            token = strtok(NULL, " ");
            (*mDistances)(i, j) = atoi(token);
         }
      }
   }

   // Read vessel file
   mode = "nil";

   while (!VesFile.eof())
   {
      VesFile.getline(buf,80);

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

      if ((token.length() == 0) || (token == "#"))
         continue;
      if (token[0] == '_')
         mode = token;
      else
      {
         if (mode == "_NUM_VES")
         {
            V = atoi(token) - 1;
            mVesselLengths = new array<int>(0, V);
            mArrivals = new array<int>(0, V);
            mDepartures = new array<int>(0, V);
            mTranshipments = new array2<int>(0, V, 0, V);
         }
         else if (mode == "_VESSEL")
         {
            int   i = atoi(token);
            if (i == 0)
               continue;
            token = strtok(NULL, " "); // now at vessel name
            token = strtok(NULL, " "); // now at voyage name
            token = strtok(NULL, " "); // now at vessel class
            token = strtok(NULL, " "); // now at vessel length
            (*mVesselLengths)[i] = atoi(token);
         }
         else if (mode == "_VES_SCHEDULE")
         {
            int   i = atoi(token);
            if (i == 0)
               continue;
            token = strtok(NULL, " "); // now at arrival time
            (*mArrivals)[i] = atoi(token);
            token = strtok(NULL, " "); // now at departure time
            (*mDepartures)[i] = atoi(token);
         }
      }
   }

   // Initialize the transhipments to all 0 (added LeongHW; 10-2008)
   for (int i = 0; i <= V; i++) 
     for (int j = 0; j <= V; j++) 
       (*mTranshipments)(i, j) = 0;


   // Read transhipment file
   mode = "nil";

   while (!TransFile.eof())
   {
      TransFile.getline(buf,80);

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

      if ((token.length() == 0) || (token == "#"))
         continue;
      if (token[0] == '_')
         mode = token;
      else
      {
         if (mode == "_TRANS_MAT")
         {
            int   i = atoi(token);
            token = strtok(NULL, " ");
            int   j = atoi(token);
            token = strtok(NULL, " ");
			int cost = atoi(token);
            (*mTranshipments)(i, j) = cost;
         }
      }
   }
}


void BAPPackage::Print(int aW, int aDetail) const
{
   cout  << tab(aW) << "--== BAP Package ==--" << endl
         << tab(aW) << "  ID         = " << ID() << endl
         << tab(aW) << "  Name       = " << Name() << endl
         << tab(aW) << "  #Sections  = " << NumSections() << endl
         << tab(aW) << "  #Vessels   = " << NumVessels() << endl
         << tab(aW) << "  #TimeZones = " << NumTimeZones() << endl;

   if (aDetail > 0)
   {
      int   S = mSectionLengths->high();
      int   V = mVesselLengths->high();
      int   TZ = mTimeZones->high();
      int   i, j;

      cout  << endl;

      // Display sections S info
      cout  << tab(aW) << "Sections S info:" << endl;

      for (i = 1; i <= S; i++)
         cout  << tab(aW) << "  S" << setw(3) << i
               << ", len =" << setw(4) << (*mSectionLengths)[i] << endl;

      cout  << endl;

      // Display vessels V info
      cout  << tab(aW) << "Vessels V info:" << endl;

      for (i = 1; i <= V; i++)
         cout  << tab(aW) << "  V" << setw(4) << i
               << ", len =" << setw(4) << (*mVesselLengths)[i]
               << ", arr =" << setw(4) << (*mArrivals)[i]
               << ", dep =" << setw(4) << (*mDepartures)[i] << endl;

      cout  << endl;

      // Display intersection distances D info
      cout  << tab(aW) << "Intersection distances D info:" << endl;

      for (i = 0; i <= S; i++)
         for (j = 0; j <= S; j++)
            cout  << tab(aW) << "  " << setw(2) << i << " -> "
                  << setw(2) << j << " = " << setw(3)
                  << (*mDistances)(i, j) << endl;

      cout  << endl;

      // Display transhipment flows F info
      cout  << tab(aW) << "Transhipment flows F info:" << endl;

      for (i = 0; i <= V; i++)
         for (j = 0; j <= V; j++)
            if ((*mTranshipments)(i, j) > 0)
               cout  << tab(aW) << "  " << setw(3) << i << " -> "
                     << setw(3) << j << " = " << setw(3)
                     << (*mTranshipments)(i, j) << endl;

      cout  << endl;


      // Display useful information
      // (only if computed)
      if (mTimeZones)
      {
         cout  << tab(aW) << "Time zone info:" << endl;

         cout  << tab(aW) << "time zone # (start, end) : set of vessels"
               << endl;

         for (i = 1; i <= TZ; i++)
         {
            cout  << tab(aW) << "  TZ" << setw(3) << i
                  << " (" << setw(3) << (*mTimeZones)[i].First()
                  << "," << setw(3) << (*mTimeZones)[i].Second()
                  << "):";

            set<int> VSet = Vessels(i);
            int      v;

            forall(v, VSet)
               cout << setw(4) << v;

            cout << endl;
         }

         cout  << tab(aW) << "vessel # : set of time zones" << endl;

         for (i = 1; i <= V; i++)
         {
            cout  << tab(aW) << "  V" << setw(4) << i << " :";

            set<int> TZSet = TimeZones(i);
            int      tz;

            forall(tz, TZSet)
               cout << setw(3) << tz;

            cout  << "  (" << StartTimeZone(i) << ", " << EndTimeZone(i)
                  << ")" << endl;
         }

         cout  << endl;
      }

      // Display solution information
      cout  << tab(aW) << "Solution info:  vessel->(section, wharfmark)"
            << endl;

      for (i = 1; i <= V; i++)
         cout  << tab(aW) << "  V" << setw(4) << i
               << " -> " << (*mSolution)[i] << endl;

      cout  << endl;
   }
}


//
// Accessor member functions
//

// path/file name information
string BAPPackage::ProjectFilename() const
{
   return mProjectFilename;
}

string BAPPackage::PartitioningFilename() const
{
   return mPartitioningFilename;
}

string BAPPackage::PackingFilename() const
{
   return mPackingFilename;
}

string BAPPackage::ParamFilename() const
{
   return mParamFilename;
}

string BAPPackage::PartitioningTraceFilename() const
{
   return mPartitioningTraceFilename;
}

string BAPPackage::PackingTraceFilename() const
{
   return mPackingTraceFilename;
}

string BAPPackage::BAPPath() const
{
   return mBAPPath;
}

// sections S
int BAPPackage::NumSections() const
{
   return mSectionLengths->high();
}

array<int> BAPPackage::SectionLengths() const
{
   return (*mSectionLengths);
}

// vessels V
int BAPPackage::NumVessels() const
{
   return mVesselLengths->high();
}

array<int> BAPPackage::VesselLengths() const
{
   return (*mVesselLengths);
}

array<int> BAPPackage::Arrivals() const
{
   return (*mArrivals);
}

array<int> BAPPackage::Departures() const
{
   return (*mDepartures);
}

// intersection distances D
array2<int> BAPPackage::Distances() const
{
   return (*mDistances);
}

// transhipment flows F
array2<int> BAPPackage::Transhipments() const
{
   return (*mTranshipments);
}

// solution information
array<IntPair> BAPPackage::Solution() const
{
   return (*mSolution);
}

int BAPPackage::SectionAssignedTo(int aVesselID) const
{
   return (*mSolution)[aVesselID].First();
}

int BAPPackage::WharfmarkAssignedTo(int aVesselID) const
{
   return (*mSolution)[aVesselID].Second();
}

// solution indicators
unsigned long BAPPackage::ObjectiveValue() const
{
   return mObjectiveValue;
}

unsigned long BAPPackage::Penalty() const
{
   return mPenalty;
}

unsigned long BAPPackage::TranshipmentCost() const
{
   return mTranshipmentCost;
}

double BAPPackage::Density() const
{
   return mDensity;
}

double BAPPackage::ABD() const
{
   return mABD;
}

// useful information
int BAPPackage::NumTimeZones() const
{
   return mTimeZones->high();
}

array<IntPair> BAPPackage::TimeZones() const
{
   return (*mTimeZones);
}

set<int> BAPPackage::TimeZones(int aVesselID) const
{
   return mTimeZonesByVessel[aVesselID];
}

int BAPPackage::Arrival(int aVesselID) const
{
   return (*mArrivals)[aVesselID];
}

int BAPPackage::Departure(int aVesselID) const
{
   return (*mDepartures)[aVesselID];
}

set<int> BAPPackage::Vessels(int aTimeZoneID) const
{
   return mVesselsInTimeZone[aTimeZoneID];
}

int BAPPackage::StartTimeZone(int aVesselID) const
{
   set<int> tzSet = TimeZones(aVesselID);
   int      tz, i = 0, temp;

   forall(temp, tzSet)
   {
      i++;

      if (1 == i) tz = temp;
   }

   return tz;
}

int BAPPackage::EndTimeZone(int aVesselID) const
{
   set<int> tzSet = TimeZones(aVesselID);
   int      tz, i = 0, temp, size = tzSet.size();

   forall(temp, tzSet)
   {
      i++;

      if (size == i) tz = temp;
   }

   return tz;
}


//
// Modifier member functions
//
int BAPPackage::SectionAssignedTo(int aVesselID, int aSectionID)
{
   int   temp = (*mSolution)[aVesselID].First();
   (*mSolution)[aVesselID].First(aSectionID);
   return   temp;
}

int BAPPackage::WharfmarkAssignedTo(int aVesselID, int aWharfmark)
{
   int   temp = (*mSolution)[aVesselID].Second();
   (*mSolution)[aVesselID].Second(aWharfmark);
   return   temp;
}

unsigned long BAPPackage::ObjectiveValue(unsigned long anObjValue)
{
   unsigned long   temp = mObjectiveValue;
   mObjectiveValue = anObjValue;
   return   temp;
}

unsigned long BAPPackage::Penalty(unsigned long aPenalty)
{
   unsigned long   temp = mPenalty;
   mPenalty = aPenalty;
   return   temp;
}

unsigned long BAPPackage::TranshipmentCost(unsigned long aCost)
{
   unsigned long   temp = mTranshipmentCost;
   mTranshipmentCost = aCost;
   return   temp;
}

double BAPPackage::Density(double aDensity)
{
   double   temp = mDensity;
   mDensity = aDensity;
   return   temp;
}

double BAPPackage::ABD(double aDemand)
{
   double   temp = mABD;
   mABD = aDemand;
   return   temp;
}

bool BAPPackage::SolutionExists() const
{
   return mSolutionExists;
}

void BAPPackage::ReadSolution()
{
   ReadSolution(mPartitioningFilename);
}

void BAPPackage::ReadSolution(string aSolFile)
{
   char     buf[255];
   string   token, mode = "nil";

   ifstream SolFile(aSolFile.c_str());

   if (!SolFile)
   {
      mSolutionExists = false;
      return;
   }
   else
   {
      mSolutionExists = true;

      while (!SolFile.eof())
      {
         SolFile.getline(buf,80);

         for (int i = 0; i < 80; i++)     // Convert carriage-return to space
            if (13 == (int) buf[i])
               buf[i] = ' ';

         if (buf[0] == ' ' || buf[0] == 0)
            continue;

         token = strtok(buf, " ");
         
         if (token.length() == 0)         // empty line, ignore
            continue;
         if (token == "#")                // comment line, ignore
            continue; 
         if (token[0] == '_')             // keyword, change mode
            mode = token;
         else                             // process data based on mode
         {
            if (mode == "_NUM_VESSELS")
            {
               assert(NumVessels() + 1 == atoi(token));
            }  
            else if (mode == "_NUM_UNALLOCATED")
            {
               mUnallocatedVessels = atoi(token);
            }
            else if (mode == "_OBJECTIVE_VALUE")
            {
               mObjectiveValue = atol(token);
            }
            else if (mode == "_TRANSHIPMENT_VALUE")
            {
               mTranshipmentCost = atol(token);
            }
            else if (mode == "_PENALTY_VALUE")
            {
               mPenalty = atol(token);
            }
            else if (mode == "_ALLOCATION")
            {
               int   i = atoi(token);     // vessel index
               token = strtok(NULL, " "); 
               int   k = atoi(token);     // section index
               token = strtok(NULL, " ");
               int   w = atoi(token);     // wharfmark index
               
               // Modifies the BAP package
               SectionAssignedTo(i, k);
               WharfmarkAssignedTo(i, w);
            }  
         }  
      }  
   }
}


void BAPPackage::AnalyzeSolution() const
{
//   cout << "Analyzing " << mProjectFilename << " ..." << endl;

   // Find unallocated vessels
   set<int> unallocSet;

   for (int i = 1; i <= NumVessels(); i++)
   {
      if ((*mSolution)[i].First() == -1)
         unallocSet.insert(i);
   }

   // Compute total transshipment cost and total containers involved
   unsigned long  transhipment = 0;
   unsigned long  numContainers = 0;

   for (int i = 1; i <= NumVessels(); i++)
   {
      int   s1 = (*mSolution)[i].First();

      if (s1 == -1) continue;

      for (int j = i + 1; j <= NumVessels(); j++)
      {
         int   s2 = (*mSolution)[j].First();

         if (s2 == -1) continue;

         if ((*mTranshipments)(i, j) > 0  ||  (*mTranshipments)(j, i) > 0)
         {
            // outflow + inflow
            transhipment += (*mTranshipments)(i, j) * (*mDistances)(s1, s2) +
                            (*mTranshipments)(j, i) * (*mDistances)(s2, s1);

            numContainers += (*mTranshipments)(i, j) +
                             (*mTranshipments)(j, i);
         }
      }
      
      // export + import
      if ((*mTranshipments)(i, 0) > 0  ||  (*mTranshipments)(0, i) > 0)
      {
         transhipment += (*mTranshipments)(i, 0) * (*mDistances)(s1, 0) +
                         (*mTranshipments)(0, i) * (*mDistances)(0, s1);

         numContainers += (*mTranshipments)(i, 0) +
                          (*mTranshipments)(0, i);
      }
   }

   cout.setf(ios::fixed);
   cout << mProjectFilename << endl
        << "  avg dist moved per container = "
        << setprecision(2) << transhipment / (double) numContainers
        << endl;
   cout.setf(ios::fixed, ios::floatfield);

//   cout << "  Analyzation done" << endl;
}


void BAPPackage::VerifySolution() const
{
   cout << "Verifying " << mProjectFilename << "..." << endl;

   // Verify number of unallocated vessels
   int   unallocated = 0;
   set<int> unallocSet;

   for (int i = 1; i <= NumVessels(); i++)
   {
      if ((*mSolution)[i].First() == -1)
      {
         unallocSet.insert(i);
         ++unallocated;
      }
   }

   if (unallocated != mUnallocatedVessels)
   {
      cout << "BAPPackage unallocated vessels = "
           << mUnallocatedVessels << endl
           << "Calculated unallocated vessels = "
           << unallocated << endl
           << "Calculated unallocated vessels list:" << endl;

      while (!unallocSet.empty())
      {
         int   v = unallocSet.choose();
         cout << v << " ";
         unallocSet.del(v);
      }
      cout << endl;
   }

   // Verify each section density <= 1.0
   for (int t = 1; t <= NumTimeZones(); t++)
   {
      set<int> vSet = Vessels(t);

      for (int k = 1; k <= NumSections(); k++)
      {
         int   v, density = 0;
         
         forall(v, vSet)
         {
            // Add vessel length if vessel is berthed at section
            if ((*mSolution)[v].First() == k)
               density += (*mVesselLengths)[v];
         }

         if (density > (*mSectionLengths)[k])
         {
            cout << "BAPPackage section " << k << " length = "
                 << (*mSectionLengths)[k] << endl
                 << "Calculated density at timezone ("
                 << (*mTimeZones)[t].First() << ", "
                 << (*mTimeZones)[t].Second() << ") = "
                 << density << endl;
         }
      }
   }
   
   // Verify transhipment cost
   unsigned long  transhipment = 0;

   for (int i = 1; i <= NumVessels(); i++)
   {
      int   s1 = (*mSolution)[i].First();

      if (s1 == -1) continue;

      for (int j = i + 1; j <= NumVessels(); j++)
      {
         int   s2 = (*mSolution)[j].First();

         if (s2 == -1) continue;

         if ((*mTranshipments)(i, j) > 0  ||  (*mTranshipments)(j, i) > 0)
         {
            // outflow + inflow
            transhipment += (*mTranshipments)(i, j) * (*mDistances)(s1, s2) +
                            (*mTranshipments)(j, i) * (*mDistances)(s2, s1);

//            cout  << setw(4) << i << setw(5) << j
//                  << setw(10) << (*mTranshipments)(i, j) +
//                  (*mTranshipments)(j, i) << " * "
//                  << setw(5) << ((*mDistances)(s1, s2) +
//                     (*mDistances)(s2, s1))/2 << " = "
//                  << (*mTranshipments)(i, j) * (*mDistances)(s1, s2) +
//                     (*mTranshipments)(j, i) * (*mDistances)(s2, s1)
//                  << "     Trans = " << transhipment << endl;
         }
      }
      
      // export + import
      if ((*mTranshipments)(i, 0) > 0  ||  (*mTranshipments)(0, i) > 0)
      {
         transhipment += (*mTranshipments)(i, 0) * (*mDistances)(s1, 0) +
                         (*mTranshipments)(0, i) * (*mDistances)(0, s1);

//         cout  << setw(4) << i << " ("
//               << setw(5) << (*mTranshipments)(i, 0) << " + "
//               << setw(5) << (*mTranshipments)(0, i) << ") * "
//               << setw(5) << ((*mDistances)(s1, 0) +
//                  (*mDistances)(0, s1))/2 << " = "
//               << (*mTranshipments)(i, 0) * (*mDistances)(s1, 0) +
//                  (*mTranshipments)(0, i) * (*mDistances)(0, s1)
//               << "     Trans = " << transhipment << endl;
      }
   }
   
   if (transhipment != mTranshipmentCost)
   {
      cout << "BAPPackage transhipment cost = "
           << mTranshipmentCost << endl
           << "Calculated transhipment cost = "
           << transhipment << endl;
   }
   
   // Verify penalty cost
   unsigned long  penalty = 0;

   for (int i = 1; i <= NumVessels(); i++)
   {
      int   s1 = (*mSolution)[i].First();
      
      for (int j = i + 1; j <= NumVessels(); j++)
      {
         int   s2 = (*mSolution)[j].First();
         
         if (s1 != -1  &&  s2 != -1) continue;

         // else either s1 == -1 or s2 == -1 or both == -1

         // outflow + inflow
         penalty += ((*mTranshipments)(i, j) + (*mTranshipments)(j, i)) *
                     INFINITY_BAP;
      }

      // export + import
      if (s1 == -1)
      {
         penalty += ((*mTranshipments)(i, 0) + (*mTranshipments)(0, i)) *
                    INFINITY_BAP;
      }
   }

   if (penalty != mPenalty)
   {
      cout << "BAPPackage penalty = " << mPenalty << endl
           << "Calculated penalty = " << penalty << endl;
   }
   
   // Verify objective value
   unsigned long  objValue = transhipment + penalty;
   
   if (objValue != mObjectiveValue)
   {
      cout << "BAPPackage objective value = "
           << mObjectiveValue << endl
           << "Calculated objective value = "
           << objValue << endl;
   }

   cout << "  Verification done" << endl;
}


void BAPPackage::WriteSolution(string aCreator, string aParamFile) const
{
/*
   ofstream SolFile(mPartitioningFilename);

   if (!SolFile)
   {
      cerr << "Cannot create solution file: " << mPartitioningFilename << endl;
      return;
   }

   // Process unallocated vessels
   const int   V = NumVessels();
   set<int>    UnallocVes;

   for (int i = 1; i <= V; i++)
      if (SectionAssignedTo(i) == -1)
         UnallocVes.add(i);


   // Write comments
   // ( Date() auto-inserts a carriage-return! )
   SolFile  << "#" << endl
            << "# Solution File created by " << aCreator << endl
            << "# Date = " << Date()
            << "# Parameter File = " << aParamFile << endl
            << "#" << endl << endl;

   SolFile  << "_NUM_UNALLOCATED" << endl
            << UnallocVes.size() << endl << endl;

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
            << endl;
   SolFile.setf(0, ios::floatfield);

   // Write allocated vessels
   array<int>  Arrivals = Arrivals(), Departures = Departures();

   SolFile  << "# Final Allocation Solution" << endl
            << "# <ves#> <sect> <wharf> <berth time> <departure time>"
            << endl << endl
            << "_ALLOCATION" << endl;

   SolFile  << "0 0 0 0 0" << endl;    // Write the sea port

   for (int i = 1; i <= V; i++)
   {
      if (SectionAssignedTo(i) > 0)
      {
         SolFile  << i
                  << " " << SectionAssignedTo(i)
                  << " " << WharfmarkAssignedTo(i)
                  << " " << Arrivals[i]
                  << " " << Departures[i]
                  << endl;
      }
   }

   // Write unallocated vessels, if any
   if (UnallocVes.size() > 0)
   {
      SolFile  << endl << "_UNALLOCATED_VESSELS" << endl;

      for (int i = 1; i <= V; i++)
      {
         if (SectionAssignedTo(i) < 0)
         {
            SolFile  << i
                     << " " << SectionAssignedTo(i)
                     << " " << WharfmarkAssignedTo(i)
                     << " " << Arrivals[i]
                     << " " << Departures[i]
                     << endl;
         }
      }
   }

   SolFile << endl << endl;

   SolFile.close();
*/
}

