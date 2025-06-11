/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrInterface___
#define ___msrInterface___

#include "displayMsrVoicesFlatViewVisitor.h"

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
void displayMsrScore (
  const S_msrScore&   theMsrScore,
  const std::string&  passDescription);

void displayMsrScoreFull (
  const S_msrScore&   theMsrScore,
  const std::string&  passDescription);

//______________________________________________________________________________
EXP void displayMsrScoreSummary (
  S_msrScore           theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription);

//______________________________________________________________________________
EXP void displayMsrScoreNames (
  S_msrScore           theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription);

//______________________________________________________________________________
EXP void displayMsrScoreVoicesFlatView (
  S_msrScore                    theMsrScore,
  const S_msrOahGroup&          msrOpts,
  const std::string&            passDescription,
  msrVoicesFlatViewDetailedKind displayVoicesFlatViewDetailsKind);

//_______________________________________________________________________________
void displayMsrScoreMeasuresSlices (
  const S_msrScore&    theMsrScore,
  const S_msrOahGroup& msrOpts,
  const std::string&   passDescription);

// parts
// ------------------------------------------------------
S_msrPart createPartInScore (
  const S_msrScore& theMsrScore);

// staves
// ------------------------------------------------------
S_msrStaff createStaffInPart (
  const mfStaffNumber& staffNumber,
  S_msrPart part);

// voices
// ------------------------------------------------------
S_msrVoice createRegularVoiceInStaff (
  const mfInputLineNumber& inputLineNumber,
  const mfVoiceNumber&     voiceNumber,
  const S_msrStaff& staff);

// measures
// ------------------------------------------------------
S_msrMeasure createMeasureInVoice (
  const mfInputLineNumber& inputLineNumber,
  const mfMeasureNumber& measureNumber,
  const S_msrVoice& voice);


}


#endif // ___msrInterface___
