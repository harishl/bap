#include "util.h"

//
// Implements the general purpose functions
//
string tab(int aW)
{
	return "";
	//return smanip<int>(&setTab, aW);
}

int Max(int a, int b)
{
   if (a > b)
      return a;
   else
      return b;
}

//
// Implements the class IntPair
//
IntPair::IntPair() : mX(0), mY(0)
{
   // empty body
}

IntPair::IntPair(int a, int b) : mX(a), mY(b)
{
   // empty body
}

IntPair::IntPair(const IntPair& aP) : mX(aP.mX), mY(aP.mY)
{
   // empty body
}

istream& operator>>(istream& aIS, IntPair& aP)
{
   aIS >> aP.mX >> aP.mY;
   return aIS;
}

ostream& operator<<(ostream& aOS, const IntPair& aP)
{
   aOS << "(" << aP.mX << "," << aP.mY << ")";
   return aOS;
}

int compare(const IntPair& aP1, const IntPair& aP2)
{
   if (aP1.mX < aP2.mX) return -1;
   if (aP1.mX > aP2.mX) return  1;
   if (aP1.mY < aP2.mY) return -1;
   if (aP1.mY < aP2.mY) return  1;
   return 0;
}

int   IntPair::First() const { return mX; }
int   IntPair::Start() const { return mX; }
int   IntPair::Second() const { return mY; }
int   IntPair::End() const { return mY; }

int   IntPair::First(int i)
{
   int   temp = mX;
   mX = i;
   return temp;
}

int   IntPair::Second(int i)
{
   int   temp = mY;
   mY = i;
   return temp;
}

int   IntPair::Start(int i) { return First(i); }
int   IntPair::End(int i) { return Second(i); }


//
// Implements the Triple structure
//
istream& operator>>(istream& aIS, Triple& aT)
{
   aIS >> aT.Time >> aT.ID >> aT.Arrive;
   return aIS;
}

ostream& operator<<(ostream& aOS, const Triple& aT)
{  
   aOS << "(" << setw(3) << aT.Time << "," << setw(3) << aT.ID << ")"
       << (aT.Arrive ? "Arrival" : "Departure");
   return aOS;
}
   
int compare(const Triple& aT1, const Triple& aT2)
{
   if (aT1.Time > aT2.Time) return 1;
   if (aT1.Time < aT2.Time) return -1;
   if ((aT1.Arrive) && (!aT2.Arrive)) return 1;    // favour departures
   if ((!aT1.Arrive) && (aT2.Arrive)) return -1;   //   to arrivals
   if (aT1.ID > aT2.ID) return 1;
   if (aT1.ID < aT2.ID) return -1;
   
   return 0;
}

int compare(const int& a, const int& b) {
	return a - b;
}
