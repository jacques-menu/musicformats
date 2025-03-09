/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrInterface___
#define ___bsrInterface___

#include "msrScores.h"

#include "bsrScores.h"

#include "msrOah.h"
#include "bsrOah.h"


namespace MusicFormats
{

// books
// ------------------------------------------------------

// scores
// ------------------------------------------------------

//_______________________________________________________________________________
EXP void displayBsrScore (
  const S_bsrScore&    bsrScore,
  const S_msrOahGroup& msrOpts,
  const S_bsrOahGroup& bsrOpts,
  const std::string&   passDescription);

EXP void displayBsrScoreFull (
  const S_bsrScore&    bsrScore,
  const S_msrOahGroup& msrOpts,
  const S_bsrOahGroup& bsrOpts,
  const std::string&   passDescription);

// parts
// ------------------------------------------------------

// staves
// ------------------------------------------------------

// voices
// ------------------------------------------------------

// measures
// ------------------------------------------------------

}


#endif // ___bsrInterface___
