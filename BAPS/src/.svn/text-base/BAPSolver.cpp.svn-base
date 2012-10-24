/******************************************************************
 *
 * Filename    : BAPSolver.cpp
 * Author      : David Ong Tat-Wee
 *
 * Version     : 1.01b
 * Date        : 29 May 98
 *
 * Description : Implements the BAP Solver class
 *
 * Reference   : nil
 *
 * Notes       : This class calls the BAP partitioning and packing
 *               algorithms.  It serves as an interface between the
 *               BAP Package class and the BAP algorithms.
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


#include "BAPSolver.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

BAPSolver::BAPSolver(BAPPackage& aPackage, string aJob)
: BAPBase(), mPackage(aPackage), mJob(aJob),
  mPartitioner("GP"), mPacker("BP")
{
   ReadParameterFile();
}

BAPSolver::~BAPSolver()
{
}

void  BAPSolver::Print(int aW, int aDetail) const
{
   cout  << tab(aW) << "--== BAP Solver ==--" << endl
         << tab(aW) << "  ID   =         " << ID() << endl
         << tab(aW) << "  Name =         " << Name() << endl
         << endl;

   mPackage.Print(aW + INDENT, aDetail);
}

BAPPackage& BAPSolver::Package() const
{
   return mPackage;
}

void BAPSolver::Solve()
{
   if (mJob == "all")
   {
cout << "running all" << endl;

      DoPartitioning();
      DoPacking();
   }
   else if (mJob == "part")
   {
cout << "running partitioning" << endl;

      DoPartitioning();
   }
   else if (mJob == "pack")
   {
cout << "running packing" << endl;

//      mPackage.ReadSolution();
      DoPacking();
   }
   else
   {
      cerr << "BAPSolver: unknown job '" << mJob << "'" << endl;
   }
}


void BAPSolver::DoPartitioning()
{
   BAPPartitioner* Partitioner;

   if (mPartitioner == "TV")
      Partitioner = new BAPTVPartitioner(mPackage);
   //else if(mPartitioner=="new partitioner")
   else
   {
      cerr << "Unknown partitioner: " << mPartitioner << endl;
      return;
   }

   Partitioner->Solve();

   delete Partitioner;
}


void BAPSolver::DoPacking()
{
   BAPPacker*  Packer;

   if (mPacker == "BP")
      Packer = new BAPBPPacker(mPackage);
   else
   {
      cerr << "Unknown packer: " << mPacker << endl;
      return;
   }

   Packer->Solve();

   delete Packer;
}


void BAPSolver::ReadParameterFile()
{
   string   paramFile = mPackage.ParamFilename();
   char     buf[255];
   string   token, mode = "nil";
   ifstream ParamFile(paramFile.c_str());

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
         if (mode == "_PARTITIONING_ALGO")
         {
            mPartitioner = token;
         }
         else if (mode == "_PACKING_ALGO")
         {
            mPacker = token;
         }
      }
   }
}

