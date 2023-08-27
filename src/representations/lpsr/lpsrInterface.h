/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrInterface___
#define ___lpsrInterface___

#include "msrTypesForwardDeclarations.h"
#include "lpsrTypesForwardDeclarations.h"

#include "msrOah.h"
#include "lpsrOah.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_msrScore createScore ();

//_______________________________________________________________________________
EXP void displayLpsrScore (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);

EXP void displayLpsrScoreFull (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);

//______________________________________________________________________________
EXP void displayLpsrScoreSummary (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);

//______________________________________________________________________________
EXP void displayLpsrScoreNames (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);

//______________________________________________________________________________
EXP void displayLpsrScoreFlatView (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);

//______________________________________________________________________________
EXP void displayLpsrScoreSlices (
  const S_lpsrScore&     theLpsrScore,
  const S_msrOahGroup&   msrOpts,
  const S_lpsrOahGroup&  lpsrOpts,
  const std::string&     passDescription);


}


#endif // ___lpsrInterface___
