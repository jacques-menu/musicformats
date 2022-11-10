/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrInterface___
#define ___lpsrInterface___

// #include "lpsr.h"


namespace MusicFormats
{

// books
// ------------------------------------------------------

// scores
// ------------------------------------------------------
S_msrScore createScore ();

//_______________________________________________________________________________
EXP void displayLpsrScoreShort (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passDescription);

EXP void displayLpsrScoreFull (
  const S_lpsrScore theLpsrScore,
  S_msrOahGroup     msrOpts,
  S_lpsrOahGroup    lpsrOpts,
  const string&     passDescription);


// parts
// ------------------------------------------------------

// staves
// ------------------------------------------------------

// voices
// ------------------------------------------------------

// measures
// ------------------------------------------------------

}


#endif
