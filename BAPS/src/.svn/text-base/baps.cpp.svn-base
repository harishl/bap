/*************************************************************************
 *
 * File Name: BAPS.cpp
 *            BAPS v2.0
 * Author   : David Ong Tat Wee
 * Date     : 25 Apr 2000
 * 
 * Copyright (C) 1998, 1999, 2000
 * All Rights Reserved by
 * The RAS Group, Algorithms Lab
 * School of Computing
 * National University of Singapore
 *
 *************************************************************************/

#include "def.h"
#include "BAPPackage.h"
#include "BAPSolver.h"

using std::cout;
using std::endl;
using std::cerr;

// Global Constants
const int   TAB = 3, DETAIL = 2;

int main(int argc, char *argv[])
{
   if (argc != 2  &&  argc != 3)
   {
      string   Name(argv[0]);
      cout  << "Usage:" << endl
            << endl
            << "   " << Name << " <Project File>" << endl
            << "   Eg. " << Name << " proj-d002-gp001-001.prj" << endl
            << endl
            << "Copyright (C) 1998 All Rights Reserved by" << endl
            << "The RAS Group, Algorithms Lab" << endl
            << "The National University of Singapore" << endl
            << endl;
      exit (0);
   }

   //
   // DOTW : 25 Apr 2000
   //
   // Safety check for environment variables BAPHOME and BAPTMP to be
   // set before running program
   //
   if (getenv("BAPHOME") == NULL)
   {
      cerr << "environment variable BAPHOME not set!" << endl;
      exit(0);
   }
   if (getenv("BAPTMP") == NULL)
   {
      cerr << "environment variable BAPTMP not set!" << endl;
      exit(0);
   }

   string      ProjectFile(argv[1]);
   string      Job("all");
   BAPPackage  TheBAP(ProjectFile);
   
   if (argc == 3)
   {
      Job = argv[2];
   }

   BAPSolver   Solver(TheBAP, Job);
   Solver.Solve();

   cout << "End of program" << endl;
   return 0;
}

