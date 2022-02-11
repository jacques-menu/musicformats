/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2msrInterface___
#define ___msr2msrInterface___

#include "exports.h"

#include "msr.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicFormats
{
//_______________________________________________________________________________
S_msrScore translateMsrToMsr (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const string&     passNumber,
  const string&     passDescription);

//_______________________________________________________________________________
S_msrScore translateMsrToMsrAlongPathToVoice (
  S_msrScore        originalMsrScore,
  S_msrOahGroup     msrOpts,
  S_msr2msrOahGroup msr2msrOpts,
  const string&     passNumber,
  const string&     passDescription,
  S_msrPathToVoice  pathToVoice);

//_______________________________________________________________________________
EXP void displayResultingNewMsrScore_OptionalPass (
  const S_msrScore originalMsrScore);

EXP void displayResultingNewMsrScoreShort_OptionalPass (
  S_msrScore originalMsrScore);


} // namespace MusicFormats


#endif
