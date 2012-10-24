/**********************************************************************
 *
 * Filename    : VesselTransform.h
 * Author      : Chen Li Wen
 *
 * Version     : 1.0
 * Date        : 09-27-98
 *
 * Description : This is the header file for class VesselTransform, 
 *               which change some of the parameters of vessels and 
 *               update BAPPackage. 
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
 *               Department of Computer Science
 *               School of Computing
 *               National University of Singapore
 *
 **********************************************************************/

#ifndef __VESSEL_TRANSFORM_H__
#define __VESSEL_TRANSFORM_H__

#include <iostream.h>
#include <LEDA/list.h>
#include <LEDA/string.h>
#include "Vessel.h"

class VesselTransform
{
   public:
      // Class construction. The default way of transformation is 
      // round up length of each vessel to the nearest power of two,
      // then sort vessels by nonincreasing length.
      VesselTransform(list<Vessel>& aVesselList, string aMethod = "");
      ~VesselTransform();
      
      // Debugger facility
      void Print(int aWidth = 1, int aDetail = 0) const;
      
      // Method to transform vessels.
      // Other methods can be added in.
      void Transform();
      
   private:
      void RoundUpPower2();
      
      list<Vessel>& mVesselList;
      string mMethod;
};

#endif
