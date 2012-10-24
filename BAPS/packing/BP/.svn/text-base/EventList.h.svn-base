/**********************************************************************
 *
 * Filename    : EventList.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.03
 * Date        : July 15, 1998
 *
 * Description : Interface file for class EventList. EventList is a 
 *               set of Events which sorted by compare function defined
 *               for Event class. 
 *
 *               Now, we implement it as a linked list of Events.
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

#ifndef __EVENTLIST_H__
#define __EVENTLIST_H__

#include "Event.h"
#include "Vessel.h"
#include "BAPBase.h"

#include <LEDA/list.h>

class EventList : public BAPBase
{
   public:
      EventList();
      ~EventList();

      void        Print(int aWidth = 1, int aDetail = 0) const;

      int         GetLength() const;
      Event       GetEvent(int aIndex) const;
      void        Build(list<Vessel> aVesselList);

   private:
      // Private functions for building EventList
      void        InsertEvent(Event aEvent);
      void        Sort();
      
      // Length of EventList
      int         mLength;
      list<Event> mEventList;
};

#endif
