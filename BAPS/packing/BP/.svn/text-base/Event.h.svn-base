/**********************************************************************
 *
 * Filename    : Event.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.03
 * Date        : July 15, 1998
 *
 * Description : Interface file for class Event. Each Vessel has two 
 *               Events: the Arrive Event and the Departure Event.
 *
 * Reference   : nil
 *
 * Notes       : nil
 *
 * Changes     : DEPARTURE to DEPART; CLW, June 13, 1998.
 *
 * Copyright   : Copyright (c) 1998
 *               All rights reserved by
 *               Resource Allocation and Scheduling Group
 *               Department of Information Systems and Computer Science
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __EVENT_H__
#define __EVENT_H__

#include "Vessel.h"
#include "PackDef.h"
#include "BAPBase.h"

// Enumeration type for event type
enum EventType
{
   ARRIVE, 
   DEPART
};

class Event : public BAPBase
{
   public:
      Event();
      Event(Vessel aVessel, EventType aDepartOrArrive);
      Event(const Event& aEvent);
      ~Event();

      // Assignment operator =
      Event&    operator=(const Event& aEvent);
      
      // Debugger facility. if aDetail is greater than 0,
      // it will print detail information about the vessel 
      // belonged to.
      void      Print(int aWidth = 1, int aDetail = 0) const;
      
      // Determine the Event Type
      Bool      IsArrive() const;
      Bool      IsDepart() const;
      
      // Access functions
      Vessel    GetVessel() const;
      int       GetTime() const;
      int       GetSize() const;

      // Modification functions
      void      SetTime(int aTime);
      void      SetSize(int aSize);

      // Functions for comparing different Events
   friend int      compare(const Event& aLEvent, const Event& aREvent);
   friend ostream& operator<<(ostream& aOutput, const Event& aEvent);
   friend istream& operator>>(istream& aInput, Event& aEvent);
   
   private:
      Vessel    mVessel;
      EventType mEventType;
      int       mTime;
      int       mSize;
};

#endif
