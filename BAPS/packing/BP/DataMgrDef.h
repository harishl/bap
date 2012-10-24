//////////////////////////////////////////////////////////////////////
//
// NAME:        DataMgrDef.h
// AUTHOR:      Foo Hee Meng
// Changed by:  Chen Li Wen (June 29, 1998)
// DATE:        20 Mar 1997
// PRODUCT:     Data Manager Constants ver 1.0
//
// DESCRIPTION: This is the constants file for the data manager classes.
//
// REFERENCE:   
//
// CHANGES:
//
// NOTES:
//
// COPYRIGHT:   Copyright (C) 1997
//              Resource Allocation and Scheduling Group
//              Department of Information Systems and Computer Science
//              National University of Singapore
//              All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DATA_MGR_DEF_
#define _DATA_MGR_DEF_

// BAP System Name
const char SYSTEM_NAME[] = "BAP System Version 1.0";

// Configuration Files
const char CONFIG_GAPARAM_FILENAME[] = "ga.rc";
const char CONFIG_BPPARAM_FILENAME[] = "bp.rc";

// Temporary File Names
const char TEMP_VES_FILENAME[] = "_ves.tmp";
const char TEMP_SECT_FILENAME[] = "_sect.tmp";
const char TEMP_TRANS_FILENAME[] = "_trans.tmp";
const char TEMP_PRJ_FILENAME[] = "_prj.tmp";
const char TEMP_GAPARAM_FILENAME[] = "_gaparam.tmp";
const char TEMP_BPPARAM_FILENAME[] = "_bpparam.tmp";
const char TEMP_FINSOL_FILENAME[] = "_finsol.tmp";
const char TEMP_INTSOL_FILENAME[] = "_intsol.tmp";
const char TEMP_GATRACE_FILENAME[] = "_gatrace.tmp";
const char TEMP_BPTRACE_FILENAME[] = "_bptrace.tmp";

// New File Names
const char NEW_VES_FILENAME[] = "ves-new.dat";
const char NEW_SECT_FILENAME[] = "sect-new.dat";
const char NEW_TRANS_FILENAME[] = "trans-new.dat";
const char NEW_PRJ_FILENAME[] = "prj-new.dat";
const char NEW_GAPARAM_FILENAME[] = "gaparam-new.par";
const char NEW_BPPARAM_FILENAME[] = "bpparam-new.par";
const char NEW_FINSOL_FILENAME[] = "finsol-new.sol";
const char NEW_INTSOL_FILENAME[] = "intsol-new.sol";
const char NEW_GATRACE_FILENAME[] = "gatrace-new.trc";
const char NEW_BPTRACE_FILENAME[] = "bptrace-new.trc";

// Temporary Files Used Only For BAP 1.0
const char BAP_FINSOL_FILENAME[] = "__finsol.tmp";
const char BAP_GATRACE_FILENAME[] = "__gatrace.tmp";
const char BAP_BPTRACE_FILENAME[] = "--bptrace.tmp";

#endif
