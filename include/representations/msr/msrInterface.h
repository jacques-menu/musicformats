/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrInterface___
#define ___msrInterface___

#include "msrOah.h"


namespace MusicFormats
{

// books
// ------------------------------------------------------
S_msrBook createBook ();

// scores
// ------------------------------------------------------

S_msrScore createScore ();

//_______________________________________________________________________________
void displayMsrScoreShort (
  S_msrScore     theMsrScore,
  const std::string&  passDescription);

void displayMsrScoreFull (
  S_msrScore     theMsrScore,
  const std::string&  passDescription);

//______________________________________________________________________________
EXP void displayMsrScoreSummary (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts,
  const std::string&  passDescription);

//______________________________________________________________________________
EXP void displayMsrScoreNames (
  S_msrScore     theMsrScore,
  S_msrOahGroup& msrOpts,
  const std::string&  passDescription);

//_______________________________________________________________________________
void displayMsrScoreSlices (
  S_msrScore    theMsrScore,
  const std::string& passDescription);

// parts
// ------------------------------------------------------
S_msrPart createPartInScore (S_msrScore theMsrScore);

// staves
// ------------------------------------------------------
S_msrStaff createStaffInPart (
  int staffNumber,
  S_msrPart part);

// voices
// ------------------------------------------------------
S_msrVoice createRegularVoiceInStaff (
  int          inputLineNumber,
  int          voiceNumber,
  const S_msrStaff&   staff);

// measures
// ------------------------------------------------------
S_msrMeasure createMeasureInVoice (
  int        inputLineNumber,
  std::string     measureNumber,
             const S_msrVoice& voice);


}


#endif
