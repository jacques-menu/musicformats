/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2msrInterface___
#define ___msr2msrInterface___

#include "exports.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  S_msrScore           originalMsrScore,
  const S_msrOahGroup& msrOpts,
  S_msr2msrOahGroup    msr2msrOpts,
  mfPassIDKind         passIDKind,
  const std::string&   passDescription);

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  S_msrScore           originalMsrScore,
  const S_msrOahGroup& msrOpts,
  S_msr2msrOahGroup    msr2msrOpts,
  mfPassIDKind         passIDKind,
  const std::string    passDescription,
  S_msrPathToVoice     pathToVoice);


}


#endif // ___msr2msrInterface___
