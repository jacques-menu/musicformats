/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrSkeletonBuilderInterface___
#define ___mxsr2msrSkeletonBuilderInterface___


// #include "msr.h"


namespace MusicFormats
{
//_______________________________________________________________________________
EXP S_msrScore translateMxsrToMsrSkeleton (
  Sxmlelement    theMxsr,
  S_msrOahGroup& msrOpts,
  const string&   passNumber,
  const string&   passDescription);

//_______________________________________________________________________________
EXP void displayMsrScoreSkeleton (
  S_msrOahGroup& msrOpts,
  S_msrScore     theMsrScore,
  const string&  passNumber,
  const string&  passDescription);


}


#endif
