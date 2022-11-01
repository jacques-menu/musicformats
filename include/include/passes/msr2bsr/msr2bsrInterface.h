/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrInterface___
#define ___msr2bsrInterface___

#include "exports.h"

#include "bsr.h"

#include "msrOah.h"
#include "bsrOah.h"


namespace MusicFormats
{
//_______________________________________________________________________________
EXP S_bsrScore translateMsrToBsr (
  S_msrScore    originalMsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts,
  const string& passNumber,
  const string& passDescription);


} // namespace MusicFormats


#endif
