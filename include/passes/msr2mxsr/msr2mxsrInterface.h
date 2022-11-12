/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2mxsrInterface___
#define ___msr2mxsrInterface___

#include "elements.h"
#include "factory.h"
#include "xml.h"
#include "xmlfile.h"

#include "exports.h"

#include "mfTiming.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP Sxmlelement translateMsrToMxsr (
  const S_msrScore theMsrScore,
  S_msrOahGroup    msrOpts,
  std::string           passNumber,
  std::string           passDescription,
  mfTimingItemKind mfTimingItemKind);


}


#endif
