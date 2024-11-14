/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrSkeletonPopulatorInterface___
#define ___mxsr2msrSkeletonPopulatorInterface___

#include <iostream>

#include "exports.h"

#include "mxsr2msrSkeletonPopulator.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void populateMsrSkeletonFromMxsr (
  const Sxmlelement&    theMxsr,
  S_msrScore            scoreSkeletonToBePopulated,
  mxsrEventsCollection& theKnownEventsCollection,
  mfPassIDKind          passIDKind,
  const std::string&    passDescription);


}


#endif // ___mxsr2msrSkeletonPopulatorInterface___
