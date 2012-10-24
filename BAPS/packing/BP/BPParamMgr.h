/*************************************************************************
 *
 * Filename    : BPParamMgr.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.03
 * Date        : July 15, 1998
 *
 * Description : Interface file for class BPParamMgr to manage the Bin-
 *               Packing parameter file.
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
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef _BP_PARAM_MGR_
#define _BP_PARAM_MGR_

#include <stdio.h>
#include <fstream.h>

#include "PackDef.h"
#include "DataMgrDef.h"

#include <LEDA/string.h>

class BPParamMgr
{
   public:
      BPParamMgr(string aBPParamFile);
      BPParamMgr();
      ~BPParamMgr();
   
      // Debugger facility
      void     Testing();

      // Data access methods
      string   GetSortParam();
      Bool     GetCompPackWidth();
      Bool     GetConsiderTrans();
      Bool     GetOutput();
      string   GetProblemModel();
      string   GetAlgoVersion();
      string   GetPackAlgo();
   
      // File related methods
      void     Update();
      int      Save();
      int      SaveAs(char *aBPParamFile);
      
   private:
      char     ConfigFileName[255];
      char     TmpFileName[255];
      char     BPParamFileName[255];
      FILE     *BPParamFile;
      ofstream TempBPParamFile;
      Bool     CompPackWidth;
      Bool     ConsiderTrans;
      Bool     Output;
      char     ProblemModel[10];
      char     AlgoVersion[10];
      char     PackAlgo[10];
      char     SortParam[50];

      // Methods
      int     readBPParamFile(char *aBPParamFile);
      int     writeTempBPParamFile();
      char    *getCurrentDate();
      char    *removeCRLF(char *aString);
};

#endif
