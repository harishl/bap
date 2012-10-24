#ifndef __RAS_UTIL__
#define __RAS_UTIL__

#include <iostream>
#include <iomanip>
#include <LEDA/core/string.h>

using leda::string;
using std::istream;
using std::ostream;
using std::setw;

//
// General purpose functions
//
string tab(int aW);
int Max(int a, int b);


//
// General purpose class that stores a pair of integers
//
class IntPair
{
public:
   IntPair();
   IntPair(int, int);
   IntPair(const IntPair&);

   friend istream& operator>> (istream&, IntPair&);
   friend ostream& operator<< (ostream&, const IntPair&);
   friend int compare(const IntPair&, const IntPair&);

   int   First() const;
   int   Start() const;
   int   Second() const;
   int   End() const;

   int   First(int);
   int   Start(int);
   int   Second(int);
   int   End(int);

private:
   int   mX, mY;
};


//
// Structures for computing time zones
//
struct Triple
{
   int   Time, ID;
   bool  Arrive;
};

istream& operator>>(istream& aIS, Triple& aT);
ostream& operator<<(ostream& aOS, const Triple& aT);
int compare(const Triple& aT1, const Triple& aT2);
int compare(const int& a, const int& b);

#endif
