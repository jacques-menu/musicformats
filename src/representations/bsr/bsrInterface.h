/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrInterface___
#define ___bsrInterface___

#include "msrOah.h"

#include "bsr.h"


namespace MusicFormats
{

// books
// ------------------------------------------------------

// scores
// ------------------------------------------------------

//_______________________________________________________________________________
EXP void displayBsrFirstScore_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts);

EXP void displayBsrFirstScoreShort_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts);

//_______________________________________________________________________________
EXP void displayFinalizedBsrScore_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts);

EXP void displayFinalizedBsrScoreShort_OptionalPass (
  S_bsrScore    originalBsrScore,
  S_msrOahGroup msrOpts,
  S_bsrOahGroup bsrOpts);


// parts
// ------------------------------------------------------

// staves
// ------------------------------------------------------

// voices
// ------------------------------------------------------

// measures
// ------------------------------------------------------

} // namespace MusicFormats


#endif
