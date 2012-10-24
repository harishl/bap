#ifndef __RAS_DEF_H__
#define __RAS_DEF_H__

#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <dirent.h>

#include <LEDA/core/array.h>
#include <LEDA/core/array2.h>
#include <LEDA/core/set.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/list.h>
#include <LEDA/core/string.h>

#include "util.h"

// RAS Type Definitions
typedef int    RASTime;


// Utility Support
string tab(int i);


// Debugging Parameters
const int INDENT = 3;      // number of spaces to indent


// Infinity Value
const int INFINITY_BAP = 1000000; 


#endif

